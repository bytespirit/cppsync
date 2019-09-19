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

#include "sync/event.h"

#include <thread>

TEST(EventTest, ManualResetEvent) {
  bool done = false;
  bytespirit::sync::Event event;
  {
    auto thread = std::thread([&] {
      event.Wait();
      done = true;
    });
    event.Set();
    thread.join();
    EXPECT_TRUE(done);
  }
  {
    done = false;
    auto thread = std::thread([&] {
      event.Wait();
      done = true;
    });
    thread.join();
    EXPECT_TRUE(done);
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
  bool done = false;
  bytespirit::sync::AutoResetEvent event;
  {
    auto thread = std::thread([&] {
      event.Wait();
      done = true;
    });
    event.Set();
    thread.join();
    EXPECT_TRUE(done);
  }
  {
    done = false;
    auto thread = std::thread([&] {
      // NOTE: The event is auto reseted
      done = event.Wait(std::chrono::seconds(1));
    });
    thread.join();
    EXPECT_FALSE(done);
  }
  {
    done = false;
    auto thread = std::thread([&] {
      event.Wait();
      done = true;
    });
    event.Set();
    thread.join();
    EXPECT_TRUE(done);
  }
}
