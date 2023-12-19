#include "src/utils/thread_pool.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <unordered_set>

namespace engine::utils {

TEST(ThreadPoolTest, GetThreadsNum) {
  ThreadPool thread_pool{3};
  EXPECT_EQ(3, thread_pool.GetThreadsNum());
  thread_pool.SetThreadsNum(5);
  EXPECT_EQ(5, thread_pool.GetThreadsNum());
}

TEST(ThreadPoolTest, Run) {
  ThreadPool thread_pool{8};
  std::unordered_set<std::thread::id> thread_ids;
  const auto save_thread_id = [&]() {
    thread_ids.insert(std::this_thread::get_id());
  };
  thread_pool.Run(save_thread_id);
  thread_pool.JoinAll();
  EXPECT_EQ(8, thread_ids.size());
}

}  // namespace engine::utils