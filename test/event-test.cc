/* Author: lipixun
 * Created Time : 2018-09-19 00:08:24
 *
 * File Name: event-test.cc
 * Description:
 *
 *  Event lock test
 *
 */


#include "gtest/gtest.h"

#include "cppsync/event.h"

#include <chrono>
#include <thread>

TEST(EventTest, ManualResetEvent) {
  bool done = false;
  bytespirit::cppsync::Event event;
  {
    auto thread = std::thread([&] {
      event.Wait();
      done = true;
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    event.Set();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_TRUE(done);
    thread.join();
  }
  {
    done = false;
    auto thread = std::thread([&] {
      event.Wait();
      done = true;
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_TRUE(done);
    thread.join();
  }
  {
    done = false;
    event.Reset();
    auto thread = std::thread([&] {
      done = event.Wait(std::chrono::seconds(1));
    });
    thread.join();
    EXPECT_FALSE(done);
  }
}

TEST(EventTest, AutoResetEvent) {
}
