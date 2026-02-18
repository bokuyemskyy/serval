#include "http_server.hpp"

#include "event_poll.hpp"
#include "http_connection_exception.hpp"
#include "http_server_config.hpp"
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

    m_poll.addFd(conn->fd(), PollEvent::READ | PollEvent::ONESHOT);
}

std::shared_ptr<HttpConnection> HttpServer::getConnection(int fd) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_connections.find(fd);
    if (it == m_connections.end())
        return nullptr;

    return it->second;
}

void HttpServer::rearmConnection(std::shared_ptr<HttpConnection> conn) {
    std::lock_guard<std::mutex> lock(m_mutex);

    switch (conn->state) {
        case HttpConnectionState::READING:
            m_poll.addFd(conn->fd(), PollEvent::READ | PollEvent::ONESHOT);
            break;
        case HttpConnectionState::WRITING:
            m_poll.addFd(conn->fd(), PollEvent::WRITE | PollEvent::ONESHOT);
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
            m_logger->log(LogLevel::DEBUG, "Poll received event " + std::to_string(static_cast<int>(events)) +
                                               " on fd " + std::to_string(fd));
            if (fd == m_listen_socket.fd()) {
                Socket client = m_listen_socket.accept();
                client.setNonBlocking(true);

                std::shared_ptr<HttpConnection> conn = std::make_shared<HttpConnection>(std::move(client));
                // m_connections[conn->fd()]            = conn;
                addConnection(conn);
            } else {
                auto conn = getConnection(fd);
                m_thread_pool.enqueue([this, conn] {
                    try {
                        m_connection_handler.handle(conn);
                    } catch (const HttpConnectionException& e) {
                        m_logger->log(LogLevel::WARN, "Connection error: " + std::string(e.what()));
                        removeConnection(conn);
                        return;
                    }
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