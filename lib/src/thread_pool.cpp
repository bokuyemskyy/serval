#include "thread_pool.hpp"

#include "http_connection.hpp"

ThreadPool::ThreadPool(int num_threads) : m_num_threads(num_threads) {}
ThreadPool::~ThreadPool() = default;

void ThreadPool::start() {
    for (size_t i = 0; i < m_num_threads; ++i) {
        m_workers.emplace_back([this] {
            while (true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_cv.wait(lock, [this] { return m_stop || !m_tasks.empty(); });
                    if (m_stop && m_tasks.empty())
                        return;
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }
                task();
            }
        });
    }
}
void ThreadPool::stop() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = true;
    }
    m_cv.notify_all();
    for (auto& worker : m_workers)
        worker.join();
}

void ThreadPool::enqueue(Task task) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_stop) return;
        m_tasks.push(std::move(task));
    }
    m_cv.notify_one();
}
