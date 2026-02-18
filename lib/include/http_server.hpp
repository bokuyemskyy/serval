#pragma once

#include "../../server/include/config.hpp"
#include "event_poll.hpp"
#include "http_connection_handler.hpp"
#include "ilogger.hpp"
#include "socket.hpp"
#include "thread_pool.hpp"

#include <memory>

class HttpServer {
  public:
    explicit HttpServer(std::shared_ptr<IHttpRequestHandler> request_handler,
                        std::shared_ptr<ILogger>             logger = std::make_shared<NullLogger>(),
                        HttpServerConfig                     config = HttpServerConfig());
    ~HttpServer() = default;

    void start();
    void run();
    void stop();

  private:
    // State variables and members
    std::mutex                                               m_mutex;
    Socket                                                   m_listen_socket;
    std::unordered_map<int, std::shared_ptr<HttpConnection>> m_connections{};
    std::atomic_bool                                         m_running{false};

    // Other modules
    EventPoll                m_poll;
    HttpServerConfig         m_config;
    ThreadPool               m_thread_pool;
    HttpConnectionHandler    m_connection_handler;
    std::shared_ptr<ILogger> m_logger;

    // Methods
    void createListenSocket();

    void                            addConnection(std::shared_ptr<HttpConnection> conn);
    void                            rearmConnection(std::shared_ptr<HttpConnection> conn);
    void                            removeConnection(std::shared_ptr<HttpConnection> conn);
    std::shared_ptr<HttpConnection> getConnection(int fd);
};