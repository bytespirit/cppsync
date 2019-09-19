/* Author: lipixun
 * Created Time : 2018-09-17 23:58:37
 *
 * File Name: signal.h
 * Description:
 *
 *  Signal lock (NOTE: This is different from semaphore lock)
 *
 */

#ifndef GITHUB_BYTESPIRIT_CPPSYNC_SYNC_SIGNAL_H_
#define GITHUB_BYTESPIRIT_CPPSYNC_SYNC_SIGNAL_H_

#include <mutex>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <condition_variable>

#include "lock.h"

namespace bytespirit {
namespace sync {

// Signal is a kind of `new` lock type which intends to help build a `lock chain` or a `lock tree`
// When a signal node of the `chain` or `tree` is emitted, all children signal nodes will be emitted too.
class Signal : public Lock {
 public:
  Signal() : emitted_(false) {}
  // Signal is not movable
  Signal(Signal&&) = delete;

  // Wait for the lock
  auto Wait() -> void override {
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [this] { return emitted_; });
  }
  // Wait for the lock with timeout
  auto Wait(std::chrono::seconds timeout) -> bool override {
    std::unique_lock<std::mutex> lk(m_);
    return cv_.wait_for(lk, timeout, [this] { return emitted_; });
  }
  // Wait for the lock with deadline
  auto Wait(std::chrono::system_clock::time_point deadline) -> bool override {
    std::unique_lock<std::mutex> lk(m_);
    return cv_.wait_until(lk, deadline, [this] { return emitted_; });
  }
  // Emit the signal
  auto Emit() -> void {
    if (!emitted_) {
      std::unique_lock<std::mutex> lk(m_);
      if (!emitted_) {
        emitted_ = true;
        cv_.notify_all();
        for (const auto& it : children_) {
          if (auto signal = it.second.lock()) {
            signal->Emit();
          }
        }
      }
    }
  }
  // Add a child to this signal
  auto AddChild(const std::shared_ptr<Signal>& signal) {
    if (signal != nullptr) {
      if (!emitted_) {
        std::unique_lock<std::mutex> lk(m_);
        if (!emitted_) {
          auto key = reinterpret_cast<uintptr_t>(signal.get());
          if (children_.find(key) == children_.end()) {
            children_[key] = std::weak_ptr<Signal>(signal);
          }
        } else {
          signal->Emit();
        }
      } else {
        signal->Emit();
      }
    }
  }
  // Remove a child from this signal
  auto RemoveChild(const std::shared_ptr<Signal>& signal) {
    if (signal != nullptr) {
      std::unique_lock<std::mutex> lk(m_);
      auto key = reinterpret_cast<uintptr_t>(signal.get());
      children_.erase(key);
    }
  }
  // Get emitted
  auto get_emitted() -> bool {
    return emitted_;
  }

  // New creates a new Signal object
  static auto New() -> std::shared_ptr<Signal> {
    return std::make_shared<Signal>();
  }
  // With creates a new Signal object as the child of the given signal object
  static auto With(const std::shared_ptr<Signal>& signal) -> std::shared_ptr<Signal> {
    auto new_signal = std::make_shared<Signal>();
    signal->AddChild(new_signal);
    return new_signal;
  }

 private:
  bool emitted_;
  std::mutex m_;
  std::condition_variable cv_;
  std::unordered_map<uintptr_t, std::weak_ptr<Signal>> children_;
};

} // namespace sync
} // namespace bytespirit

#endif // GITHUB_BYTESPIRIT_CPPSYNC_SYNC_SIGNAL_H_
