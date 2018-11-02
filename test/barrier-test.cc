/* Author: lipixun
 * Created Time : 2018-09-19 16:20:19
 *
 * File Name: barrier_test.cc
 * Description:
 *
 *  Barrier lock test
 *
 */

#include "gtest/gtest.h"

#include "cppsync/barrier.h"

#include <thread>
#include <memory>
#include <chrono>

TEST(BarrierTest, FromZero) {
  bytespirit::cppsync::Barrier barrier;
  bool flags[100];
  std::vector<std::unique_ptr<std::thread>> threads;
  for (int i = 0; i < 100; ++i) {
    flags[i] = false;
    barrier.Add();
    threads.emplace_back(new std::thread([&, i] {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      flags[i] = true;
      barrier.Done();
    }));
  }
  barrier.Freeze();
  barrier.Wait();
  EXPECT_TRUE(barrier.IsCompleted());
  for (size_t i = 0; i < 100; ++i) {
    EXPECT_TRUE(flags[i]) << "Thread [" << i << "] is not completed";
  }
  for (const auto& t : threads) {
    t->join();
  }
}
