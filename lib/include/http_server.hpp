#pragma once

#include "event_poll.hpp"
#include "http_connection_handler.hpp"
#include "http_server_config.hpp"
#include "ilogger.hpp"
#include "socket.hpp"
#include "thread_pool.hpp"

#include <memory>

/**
 * @brief A multithreaded HTTP server.
 *
 * This class is RAII-compliant and thread-safe.
 * It manages listening for incoming connections, handling them via a request handler,
 * and processing I/O readiness events using an internal event poll and thread pool.
 */
class HttpServer {
  public:
    /**
     * @brief Construct a new HttpServer.
     *
     * @param request_handler Shared pointer to an implementation of IHttpRequestHandler.
     *                        All incoming requests are passed to this handler.
     * @param logger          Shared pointer to a logger. Defaults to NullLogger if not provided.
     * @param config          Server configuration defining behavior such as port, max connections, etc.
     */
    explicit HttpServer(std::shared_ptr<IHttpRequestHandler> request_handler,
                        std::shared_ptr<ILogger>             logger = std::make_shared<NullLogger>(),
                        HttpServerConfig                     config = HttpServerConfig());
    ~HttpServer() = default;

    /**
     * @brief Start listening for incoming connections.
     *
     * Initializes the listening socket and prepares the server for accepting connections.
     */
    void start();

    /**
     * @brief Stop the server.
     *
     * Closes all connections, stops the event poll, and cleans up resources.
     */
    void stop();

  private:
    // --------------------
    // State variables
    // --------------------
    std::mutex                                               m_mutex;
    Socket                                                   m_listen_socket;
    std::unordered_map<int, std::shared_ptr<HttpConnection>> m_connections{};
    std::atomic_bool                                         m_running{false};

    // --------------------
    // Server modules
    // --------------------
    EventPoll                m_poll;
    HttpServerConfig         m_config;
    ThreadPool               m_thread_pool;
    HttpConnectionHandler    m_connection_handler;
    std::shared_ptr<ILogger> m_logger;

    // --------------------
    // Internal methods
    // --------------------

    /**
     * @brief Creates the listening socket and binds it to the configured port.
     */
    void createListenSocket();

    /**
     * @brief Main server loop.
     *
     * Processes incoming events and dispatches connections to worker threads.
     * This method typically runs until stop() is called.
     */
    void run();

    /**
     * @brief Adds a new connection to the server.
     *
     * Adds the connection to the internal map and creates a one-shot read event in the poll.
     *
     * @param conn The connection to add.
     */
    void addConnection(std::shared_ptr<HttpConnection> conn);

    /**
     * @brief Rearms the one-shot event for a connection.
     *
     * Event type depends on the connection state (read/write).
     *
     * @param conn The connection to rearm.
     */
    void rearmConnection(std::shared_ptr<HttpConnection> conn);

    /**
     * @brief Removes a connection from the server.
     *
     * Removes it from the poll and the internal connections map.
     *
     * @param conn The connection to remove.
     */
    void removeConnection(std::shared_ptr<HttpConnection> conn);

    /**
     * @brief Retrieves a connection by its file descriptor.
     *
     * @param fd The file descriptor of the connection.
     * @return Shared pointer to the connection if found, nullptr otherwise.
     */
    std::shared_ptr<HttpConnection> getConnection(int fd);
};