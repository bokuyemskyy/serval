#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

/**
 * @brief A thread pool for executing tasks concurrently.
 *
 * Tasks can be enqueued and will be processed by a fixed number of worker threads.
 */
class ThreadPool {
  public:
    using Task = std::function<void()>;

    /**
     * @brief Construct a ThreadPool with the given number of worker threads.
     *
     * @param num_threads Number of threads to create in the pool.
     */
    ThreadPool(int num_threads);

    /// Destructor. Stops all threads and cleans up resources.
    ~ThreadPool();

    /**
     * @brief Start the worker threads.
     *
     * Threads will begin processing tasks from the queue.
     */
    void start();

    /**
     * @brief Add a task to the queue.
     *
     * @param task The task to execute.
     */
    void enqueue(Task task);

    /**
     * @brief Stop the thread pool.
     *
     * Waits for all threads to finish and prevents new tasks from being executed.
     */
    void stop();

  private:
    bool                     m_stop = false;
    int                      m_num_threads;
    std::vector<std::thread> m_workers;
    std::queue<Task>         m_tasks;
    std::mutex               m_mutex;
    std::condition_variable  m_cv;
};