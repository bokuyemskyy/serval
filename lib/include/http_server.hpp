#pragma once

#include "event_poll.hpp"
#include "http_connection_handler.hpp"
#include "http_server_config.hpp"
#include "ilogger.hpp"
#include "socket.hpp"
#include "thread_pool.hpp"

#include <memory>

class HttpServer {
  public:

    explicit HttpServer(HttpServerConfig config, std::shared_ptr<ILogger> logger);
    ~HttpServer() = default;

    void setRequestHandler(IHttpRequestHandler* request_handler);

    void start();
    void run();
    void stop();

  private:
    Socket    m_listen_socket;
    EventPoll m_poll;

    std::shared_ptr<ILogger> m_logger;

    HttpConnectionHandler m_connection_handler;
    HttpServerConfig m_config;
    HttpProtocol m_protocol;
    ThreadPool m_thread_pool;

    bool m_running = false;
};