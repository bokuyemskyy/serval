#pragma once

#include "http_connection.hpp"
#include "http_connection_handler.hpp"
#include "socket.hpp"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
class ThreadPool {
  public:
    using ConnectionHandler = std::function<void(HttpConnection)>;

    ThreadPool(int num_threads, HttpProtocol& protocol, HttpServerConfig& config);
    ~ThreadPool();

    void setConnectionHandler(const ConnectionHandler& connection_handler);

    void start();
    void stop();
    void enqueue(Socket client);

  private:
    int m_num_threads;
    std::vector<std::thread> m_workers;
    std::queue<Socket>       m_tasks;
    std::mutex               m_queue_mutex;
    std::condition_variable  m_cv;
    bool                     m_stop = false;

    ConnectionHandler m_connection_handler;

    HttpServerConfig& m_config;
    HttpProtocol&            m_protocol;
};