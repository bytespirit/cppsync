/* Author: lipixun
 * Created Time : 2018-09-17 23:48:56
 *
 * File Name: barrier.h
 * Description:
 *
 *  Barrier lock
 *
 */

#ifndef GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_BARRIER_H_
#define GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_BARRIER_H_

#include <mutex>
#include <chrono>
#include <condition_variable>

#include "lock.h"

namespace bytespirit {
namespace cppsync {

// Barrier implements the barrier lock
class Barrier : public Lock {
 public:
  Barrier() : Barrier(0) {}
  Barrier(int count) : count_(count > 0 ? count : 0) {}

  // Wait for the lock
  auto Wait() -> void override {
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [this] { return completed_; });
  }
  // Wait for the lock with timeout
  auto Wait(std::chrono::seconds timeout) -> bool override {
    std::unique_lock<std::mutex> lk(m_);
    return cv_.wait_for(lk, timeout, [this] { return completed_; });
  }
  // Wait for the lock with deadline
  auto Wait(std::chrono::system_clock::time_point deadline) -> bool override {
    std::unique_lock<std::mutex> lk(m_);
    return cv_.wait_until(lk, deadline, [this] { return completed_; });
  }
  // Add 1 barrier
  auto Add() -> void {
    Add(1);
  }
  // Add more barriers
  auto Add(int count) -> void {
    if (count > 0 && !completed_ && !frozen_) {
      std::unique_lock<std::mutex> lk(m_);
      if (!completed_ && !frozen_) {
        count_ += count;
      }
    }
  }
  // Freeze this barrier. Add will be invalid after calling this.
  auto Freeze() -> void {
    if (!frozen_) {
      std::unique_lock<std::mutex> lk(m_);
      if (!frozen_) {
        frozen_ = true;
        if (count_ <= 0) {
          completed_ = true;
          cv_.notify_all();
        }
      }
    }
  }
  // Remove 1 barrier
  auto Done() -> void {
    Done(1);
  }
  // Remove some barriers
  auto Done(int count) -> void {
    if (count > 0 && !completed_) {
      std::unique_lock<std::mutex> lk(m_);
      if (!completed_) {
        count_ -= count;
        if (count_ <= 0 && frozen_) {
          completed_ = true;
          cv_.notify_all();
        }
      }
    }
  }
  // Check if the barrier is completed
  auto IsCompleted() -> bool {
    return completed_;
  }

 private:
  int count_;
  bool frozen_;
  bool completed_;
  std::mutex m_;
  std::condition_variable cv_;
};

} // namespace cppsync
} // namespace bytespirit

#endif // GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_BARRIER_H_
