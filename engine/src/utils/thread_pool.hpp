#pragma once

#include <functional>
#include <thread>
#include <vector>

namespace engine::utils {

class ThreadPool {
 public:
  using ThreadFunc = std::function<void()>;
  using ThreadsVec = std::vector<std::jthread>;

  ThreadPool() = default;
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  explicit ThreadPool(size_t threads_num) noexcept;
  void SetThreadsNum(size_t threads_num) noexcept;
  size_t GetThreadsNum() const noexcept;
  void Run(const ThreadFunc& thread_func);
  void JoinAll();

 private:
  ThreadsVec threads_;
  size_t threads_num_{};
};

}  // namespace engine::utils