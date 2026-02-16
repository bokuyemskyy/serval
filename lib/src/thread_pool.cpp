#include "thread_pool.hpp"

#include "http_connection.hpp"
#include "socket.hpp"


ThreadPool::ThreadPool(int num_threads, HttpProtocol& protocol, HttpServerConfig& config) : m_num_threads(num_threads), m_protocol(protocol), m_config(config) {}

ThreadPool::~ThreadPool() {
    stop();
}
void ThreadPool::setConnectionHandler(const ConnectionHandler& connection_handler) { m_connection_handler = connection_handler; }

void ThreadPool::start() {
    if (m_connection_handler == nullptr)
        throw std::invalid_argument("ConnectionHandler is nullptr");

    for (size_t i = 0; i < m_num_threads; ++i) {
        m_workers.emplace_back([this] {
            while (true) {
                Socket client;

                {
                    std::unique_lock<std::mutex> lock(m_queue_mutex);

                    m_cv.wait(lock, [this] { return m_stop || !m_tasks.empty(); });

                    if (m_stop && m_tasks.empty())
                        return;

                    client = std::move(m_tasks.front());
                    m_tasks.pop();
                }

                HttpConnection conn(std::move(client), m_protocol, m_config);
                m_connection_handler(std::move(conn));
            }
        });
    }
}
void ThreadPool::stop() {
    {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        m_stop = true;
    }
    m_cv.notify_all();
    for (auto& worker : m_workers)
        worker.join();
}

void ThreadPool::enqueue(Socket client) {
    {
        std::lock_guard<std::mutex> lock(m_queue_mutex);
        m_tasks.push(std::move(client));
    }
    m_cv.notify_one();
}
