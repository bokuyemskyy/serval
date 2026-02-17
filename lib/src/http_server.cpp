#include "http_server.hpp"

#include "../../server/include/config.hpp"
#include "event_poll.hpp"
#include "socket.hpp"

HttpServer::HttpServer(std::shared_ptr<IHttpRequestHandler> request_handler, std::shared_ptr<ILogger> logger,
                       HttpServerConfig config)
    : m_config(std::move(config)), m_thread_pool(m_config.worker_threads),
      m_connection_handler(request_handler, m_config), m_logger(logger) {}

void HttpServer::addConnection(std::shared_ptr<HttpConnection> conn) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (conn->state != HttpConnectionState::READING)
        throw std::runtime_error("Cannot add HttpConnection which is not in read state.");

    m_connections[conn->fd()] = conn;

    m_poll.addFd(conn->fd(), PollEvent::READ);
}

void HttpServer::rearmConnection(std::shared_ptr<HttpConnection> conn) {
    switch (conn->state) {
        case HttpConnectionState::READING:
            m_poll.modifyFd(conn->fd(), PollEvent::READ);
            break;
        case HttpConnectionState::WRITING:
            m_poll.modifyFd(conn->fd(), PollEvent::WRITE);
            break;
        case HttpConnectionState::CLOSING:
            throw std::runtime_error("Cannot rearm HttpConnection which is in close state.");
    }
}

void HttpServer::removeConnection(std::shared_ptr<HttpConnection> conn) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (conn->state != HttpConnectionState::CLOSING)
        throw std::runtime_error("Cannot close HttpConnection which is not in close state.");

    m_connections.erase(conn->fd());

    m_poll.removeFd(conn->fd());
}

void HttpServer::start() {
    createListenSocket();

    m_logger->log(LogLevel::INFO, "Ready to accept HTTP connections.");

    m_running.store(true);
    m_thread_pool.start();

    run();
}

void HttpServer::run() {
    while (m_running) {
        m_poll.wait(200);

        for (const auto& [fd, events] : m_poll.events()) {
            if (fd == m_listen_socket.fd()) {
                Socket client = m_listen_socket.accept();
                client.setNonBlocking(true);

                std::shared_ptr<HttpConnection> conn = std::make_shared<HttpConnection>(std::move(client));

                addConnection(conn);

                m_thread_pool.enqueue([this, conn] {
                    m_connection_handler.handle(conn);
                    if (conn->state == HttpConnectionState::CLOSING)
                        removeConnection(conn);
                    else
                        rearmConnection(conn);
                });
            }
        }
    }
}

void HttpServer::stop() {
    m_thread_pool.stop();
    m_running.store(false);
}

void HttpServer::createListenSocket() {
    m_listen_socket.create();
    m_listen_socket.setReuseAddr(true);
    m_listen_socket.setNonBlocking(true);
    m_listen_socket.bind(m_config.host, m_config.port);
    m_listen_socket.listen();

    m_poll.addFd(m_listen_socket.fd(), PollEvent::READ);
}