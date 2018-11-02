/* Author: lipixun
 * Created Time : 2018-10-29 16:15:58
 *
 * File Name: waiting_queue-test.cc
 * Description:
 *
 *  Waiting queue test
 *
 */


#include "gtest/gtest.h"

#include "cppsync/waiting_queue.h"

#include <atomic>
#include <thread>
#include <memory>
#include <chrono>
#include <thread>

TEST(WaitingQueueTest, VoidStarCompleteBeforeWaiting) {
  bytespirit::cppsync::WaitingQueue<void*> wq;
  // Completes
  EXPECT_EQ(wq.Complete((void*)1), true);
  EXPECT_EQ(wq.Complete((void*)2), true);
  EXPECT_EQ(wq.Complete((void*)3), true);
  EXPECT_EQ(wq.Complete((void*)4), true);
  EXPECT_EQ(wq.Complete((void*)5), true);
  // Wait
  int i = 0;
  for (const auto& tag : wq) {
    ++i;
    EXPECT_EQ((int64_t)tag, i);
    if (i >= 5) {
      break;
    }
  }
}

TEST(WaitingQueueTest, VoidStarCompleteWhileWaiting) {
  bytespirit::cppsync::WaitingQueue<void*> wq;
  // Complete thread
  std::thread thread1([&] {
    EXPECT_EQ(wq.Complete((void*)1), true);
    EXPECT_EQ(wq.Complete((void*)2), true);
    EXPECT_EQ(wq.Complete((void*)3), true);
    EXPECT_EQ(wq.Complete((void*)4), true);
    EXPECT_EQ(wq.Complete((void*)5), true);
  });
  // Wait
  int i = 0;
  for (const auto& tag : wq) {
    ++i;
    EXPECT_EQ((int64_t)tag, i);
    if (i >= 5) {
      break;
    }
  }
  // Wait for thread
  thread1.join();
}

TEST(WaitingQueueTest, VoidStarCompleteWhileWaitingWithTimeout) {
  bytespirit::cppsync::WaitingQueue<void*> wq;
  // Complete thread
  std::thread thread1([&] {
    EXPECT_EQ(wq.Complete((void*)1), true);
    EXPECT_EQ(wq.Complete((void*)2), true);
    EXPECT_EQ(wq.Complete((void*)3), true);
    EXPECT_EQ(wq.Complete((void*)4), true);
    EXPECT_EQ(wq.Complete((void*)5), true);
  });
  // Wait
  int i = 0;
  for (const auto& tag : wq.IterWithTimeout(std::chrono::seconds(1))) {
    ++i;
    EXPECT_EQ((int64_t)tag, i);
  }
  EXPECT_EQ(i, 5);
  // Wait for thread
  thread1.join();
}
