#include "../include/http_server.hpp"

#include "../include/http_server_config.hpp"
#include "event_poll.hpp"
// #include "logger.hpp"
#include "socket.hpp"


HttpServer::HttpServer(HttpServerConfig config, std::shared_ptr<ILogger> logger) : m_config(std::move(config)), m_protocol(m_config),  m_thread_pool(m_config.worker_threads, m_protocol, m_config), m_logger(logger) {
    m_thread_pool.setConnectionHandler([this](HttpConnection conn) {
          this->m_connection_handler.handleConnection(std::move(conn));
      });
}

void HttpServer::setRequestHandler(IHttpRequestHandler* request_handler) { m_connection_handler.setRequestHandler(request_handler); }

void HttpServer::start() {
    m_listen_socket.create();
    m_listen_socket.setReuseAddr(true);
    m_listen_socket.setNonBlocking(true);
    m_listen_socket.bind(m_config.host, m_config.port);
    m_listen_socket.listen();

    m_poll.addFd(m_listen_socket.fd(), PollEvent::READ);

    // m_log_handler("[INFO] Ready to accept TCP connections.");
    m_running = true;

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

                m_thread_pool.enqueue(std::move(client));
            }
        }
    }
}

void HttpServer::stop() {
    m_thread_pool.stop();

    m_running = false;
}