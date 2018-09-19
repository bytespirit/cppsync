/* Author: lipixun
 * Created Time : 2018-09-19 17:04:13
 *
 * File Name: signal-test.cc
 * Description:
 *
 *  Signal lock test
 *
 */

#include "gtest/gtest.h"

#include "cppsync/signal.h"

#include <atomic>
#include <thread>
#include <memory>
#include <chrono>

TEST(SignalTest, FromZero) {
  auto signal = bytespirit::cppsync::Signal::New();
  bool flag1 = false, flag2 = false;
  std::thread thread1([&] {
    flag1 = signal->Wait(std::chrono::seconds(2));
  });
  std::thread thread2([&] {
    flag2 = bytespirit::cppsync::Signal::With(signal)->Wait(std::chrono::seconds(2));
  });
  signal->Emit();
  thread1.join();
  thread2.join();
  EXPECT_TRUE(flag1);
  EXPECT_TRUE(flag2);
  EXPECT_TRUE(bytespirit::cppsync::Signal::With(signal)->Wait(std::chrono::seconds(2)));
}
