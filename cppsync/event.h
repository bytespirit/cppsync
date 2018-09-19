/* Author: lipixun
 * Created Time : 2018-09-17 23:40:05
 *
 * File Name: event.h
 * Description:
 *
 *  Event lock
 *
 */

#ifndef GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_EVENT_H_
#define GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_EVENT_H_

#include <mutex>
#include <chrono>
#include <condition_variable>

#include "lock.h"

namespace bytespirit {
namespace cppsync {

// EventBase is the base class of event locks
class EventBase : public Lock {
 public:
  EventBase(bool set, bool auto_reset) : set_(set), auto_reset_(auto_reset) {}

  // Wait for the lock
  auto Wait() -> void override {
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [this] { return set_; });
    OnOneWakedUp();
  }
  // Wait for the lock with timeout
  auto Wait(std::chrono::seconds timeout) -> bool override {
    std::unique_lock<std::mutex> lk(m_);
    if (cv_.wait_for(lk, timeout, [this] { return set_; })) {
      OnOneWakedUp();
      return true;
    }
    return false;
  }
  // Wait for the lock with deadline
  auto Wait(std::chrono::system_clock::time_point deadline) -> bool override {
    std::unique_lock<std::mutex> lk(m_);
    if (cv_.wait_until(lk, deadline, [this] { return set_; })) {
      OnOneWakedUp();
      return true;
    }
    return false;
  }
  // Set the event (Which will wake up waiter(s))
  auto Set() -> void {
    std::unique_lock<std::mutex> lk(m_);
    if (!set_) {
      set_ = true;
      if (auto_reset_) {
        cv_.notify_one();
      } else {
        cv_.notify_all();
      }
    }
  }
  // Set the event to unset state
  auto Reset() -> void {
    std::unique_lock<std::mutex> lk(m_);
    set_ = false;
  }

 protected:
  auto OnOneWakedUp() -> void {
    if (auto_reset_) {
      set_ = false;
    }
  }

 private:
  bool set_;
  bool auto_reset_;
  std::mutex m_;
  std::condition_variable cv_;
};

// Event implements a manual reset event lock
class Event : public EventBase {
 public:
  Event() : Event(false) {}
  Event(bool set) : EventBase(set, false) {}
};

// AutoResetEvent implements an auto reset event lock
class AutoResetEvent : public EventBase {
 public:
  AutoResetEvent() : AutoResetEvent(false) {}
  AutoResetEvent(bool set) : EventBase(set, true) {}
};

} // namespace cppsync
} // namespace bytespirit

#endif // GITHUB_BYTESPIRIT_CPPSYNC_CPPSYNC_EVENT_H_
