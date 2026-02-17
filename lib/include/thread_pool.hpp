#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  public:
    using Task = std::function<void()>;

    ThreadPool(int num_threads);
    ~ThreadPool();

    void start();
    void enqueue(Task task);
    void stop();

  private:
    bool m_stop = false;
    int m_num_threads;
    std::vector<std::thread> m_workers;
    std::queue<Task>       m_tasks;
    std::mutex               m_mutex;
    std::condition_variable  m_cv;
};