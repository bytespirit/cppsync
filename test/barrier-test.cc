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

TEST(BarrierTest, FromZero) {
  bytespirit::cppsync::Barrier barrier;
  std::vector<bool> flags;
  std::vector<std::unique_ptr<std::thread>> threads;
  for (int i = 0; i < 100; ++i) {
    flags.push_back(false);
    barrier.Add();
    threads.emplace_back(new std::thread([&, i] {
      flags[i] = true;
      barrier.Done();
    }));
  }
  barrier.Freeze();
  barrier.Wait();
  EXPECT_TRUE(barrier.IsCompleted());
  for (auto flag : flags) {
    EXPECT_TRUE(flag);
  }
  for (const auto& t : threads) {
    t->join();
  }
}
