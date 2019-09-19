/* Author: lipixun
 * Created Time : 2018-10-28 17:37:23
 *
 * File Name: waiting_queue.h
 * Description:
 *
 *  The waiting queue
 *
 */

#ifndef GITHUB_BYTESPIRIT_CPPSYNC_SYNC_WAITING_QUEUE_H_
#define GITHUB_BYTESPIRIT_CPPSYNC_SYNC_WAITING_QUEUE_H_

#include <queue>
#include <mutex>
#include <chrono>
#include <iterator>
#include <functional>
#include <condition_variable>

namespace bytespirit {
namespace sync {

template<class T>
class WaitingQueue {
 public:

  explicit WaitingQueue() = default;
  // WaitingQueue is not movable
  WaitingQueue(WaitingQueue&&) = delete;
  ~WaitingQueue() = default;

  // Wait for next
  auto Wait(T* tag) -> bool {
    std::unique_lock<std::mutex> lk(m_);
    bool ok = false;
    cv_.wait(lk, std::bind(&WaitingQueue::OnOneCheck, this, &ok, tag));
    return ok;
  }
  // Wait for next with timeout
  auto Wait(std::chrono::seconds timeout, T* tag) -> bool {
    std::unique_lock<std::mutex> lk(m_);
    bool ok = false;
    return cv_.wait_for(lk, timeout, std::bind(&WaitingQueue::OnOneCheck, this, &ok, tag)) && ok;
  }
  // Wait for next with deadline
  auto Wait(std::chrono::system_clock::time_point deadline, T* tag) -> bool {
    std::unique_lock<std::mutex> lk(m_);
    bool ok = false;
    return cv_.wait_until(lk, deadline, std::bind(&WaitingQueue::OnOneCheck, this, &ok, tag)) && ok;
  }
  // Complete a tag
  auto Complete(T tag) -> bool {
    std::unique_lock<std::mutex> lk(m_);
    if (!closed_) {
      completed_tags_.push(tag);
      cv_.notify_one();
      return true;
    }
    return false;
  }
  // Close this waiting queue. This will wake up all waiters.
  auto Close() -> void {
    std::unique_lock<std::mutex> lk(m_);
    closed_ = true;
    cv_.notify_all();
  }
  //
  //
  // Support iterate tags by something like `for (const auto& tag : waiting_queue) {}`
  //
  //
  class iterator : public std::iterator<std::forward_iterator_tag, T> {
   public:
    using base = std::iterator<std::forward_iterator_tag, T>;
    using typename base::pointer;
    using typename base::reference;

    typedef std::function<bool(T* tag)> wait_function;

    auto operator++() -> iterator& { wait(); return *this; }
    auto operator*() -> reference { return tag_; }
    auto operator==(iterator other) const -> bool { return end_ && other.end_; }
    auto operator!=(iterator other) const -> bool { return !(*this == other); }

   protected:
    friend WaitingQueue<T>;

    explicit iterator(wait_function wait_func) : iterator(wait_func, false) {}
    explicit iterator(wait_function wait_func, bool end) : end_(end), wait_func_(wait_func) {
      wait();
    }

   private:
    auto wait() -> void {
      if (!end_) {
        end_ = !wait_func_(&tag_);
      }
    }

    T tag_;
    bool end_;
    wait_function wait_func_;
  };

  class iterating {
   public:
    auto begin() -> iterator { return iterator(wait_func_); }
    auto end() -> iterator { return iterator(nullptr, true); }

   protected:
    friend WaitingQueue<T>;

    iterating(typename iterator::wait_function wait_func) : wait_func_(wait_func) {}

   private:
    typename iterator::wait_function wait_func_;
  };

  auto begin() -> iterator { return iterator([this](T *tag) { return Wait(tag); }); }
  auto end() -> iterator { return iterator(nullptr, true); }
  // Iterate the tags with timeout
  auto IterWithTimeout(std::chrono::seconds timeout) -> iterating {
    return iterating([this, timeout](T* tag) { return Wait(timeout, tag); });
  }
  // Iterate the tags with deadline
  auto IterWithDeadline(std::chrono::system_clock::time_point deadline) -> iterating {
    return iterating([this, deadline](T* tag) { return Wait(deadline, tag); });
  }

 protected:
  auto OnOneCheck(bool* ok, T* tag) -> bool {
    if (completed_tags_.size() > 0) {
      *tag = completed_tags_.front();
      completed_tags_.pop();
      *ok = true;
      return true;
    } else if (closed_) {
      *ok = false;
      return true;
    }
    // Continue wait
    return false;
  }

 private:
  bool closed_ = false;
  std::queue<T> completed_tags_;
  std::mutex m_;
  std::condition_variable cv_;
};

extern template class WaitingQueue<void*>;

} // namespace sync
} // namespace bytespirit

#endif // GITHUB_BYTESPIRIT_CPPSYNC_SYNC_WAITING_QUEUE_H_
