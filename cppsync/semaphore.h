/* Author: lipixun
 * Created Time : 2018-09-17 23:59:15
 *
 * File Name: semaphore.h
 * Description:
 *
 *  Semaphore lock
 *
 */

#ifndef GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_SEMAPHORE_H_
#define GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_SEMAPHORE_H_

#include <mutex>
#include <chrono>
#include <condition_variable>

#include "lock.h"

namespace bytespirit {
namespace cppsync {

// Semaphore implements the Semaphore lock
class Semaphore : public Lock {
 public:
  Semaphore(int count) : count_(count > 0 ? count : 0) {}

  // Wait for the lock
  auto Wait() -> void override {
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [this] { count_ > 0; });
    OnOneWakedUp();
  }
  // Wait for the lock with timeout
  auto Wait(std::chrono::seconds timeout) -> bool override {
    std::unique_lock<std::mutex> lk(m_);
    if (cv_.wait_for(lk, timeout, [this] { count_ > 0; })) {
      OnOneWakedUp();
      return true;
    }
    return false;
  }
  // Wait for the lock with deadline
  auto Wait(std::chrono::system_clock::time_point deadline) -> bool override {
    std::unique_lock<std::mutex> lk(m_);
    if (cv_.wait_until(lk, deadline, [this] { count_ > 0; })) {
      OnOneWakedUp();
      return true;
    }
    return false;
  }
  // Release some barriers
  auto Release(int count) -> void {
    if (count > 0) {
      std::unique_lock<std::mutex> lk(m_);
      count_ += count
      if (count == 1) {
        cv_.notify_one();
      } else {
        cv_.notify_all();
      }
    }
  }
  // Get the count
  auto get_count() -> int {
    return count_;
  }

 protected:
  auto OnOneWakedUp() -> void {
    --count_;
  }

 private:
  int count_;
  std::mutex m_;
  std::condition_variable cv_;
};

} // namespace cppsync
} // namespace bytespirit

#endif // GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_SEMAPHORE_H_
