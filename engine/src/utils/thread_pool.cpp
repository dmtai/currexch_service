#include "thread_pool.hpp"

namespace engine::utils {

ThreadPool::ThreadPool(size_t threads_num) noexcept
    : threads_num_{threads_num} {}

void ThreadPool::SetThreadsNum(size_t threads_num) noexcept {
  threads_num_ = threads_num;
}

size_t ThreadPool::GetThreadsNum() const noexcept { return threads_num_; }

void ThreadPool::JoinAll() {
  for (auto& thread : threads_) {
    if (!thread.joinable()) {
      continue;
    }
    thread.join();
  }
}

void ThreadPool::Run(const ThreadFunc& thread_func) {
  for (size_t i = 0; i < threads_num_; ++i) {
    threads_.emplace_back(thread_func);
  }
}

}  // namespace engine::utils