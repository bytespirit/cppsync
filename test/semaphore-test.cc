/* Author: lipixun
 * Created Time : 2018-09-19 16:30:51
 *
 * File Name: semaphore-test.cc
 * Description:
 *
 *  Semaphore lock test
 *
 */

#include "gtest/gtest.h"

#include "sync/semaphore.h"

#include <atomic>
#include <thread>
#include <memory>
#include <chrono>

TEST(SemaphoreTest, FromZero) {
  bytespirit::sync::Semaphore sema;
  std::atomic_int counter = 0;
  std::thread thread([&] {
    while (true) {
      if (!sema.Wait(std::chrono::seconds(1))) {
        break;
      }
      counter.fetch_add(1.0);
    }
  });
  for (int i = 0; i < 100; ++i) {
    sema.Release();
  }
  thread.join();
  EXPECT_EQ(counter, 100);
}
