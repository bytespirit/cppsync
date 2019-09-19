/* Author: lipixun
 * Created Time : 2018-09-17 22:42:27
 *
 * File Name: lock.h
 * Description:
 *
 *  Lock base classes
 *
 */

#ifndef GITHUB_BYTESPIRIT_CPPSYNC_SYNC_LOCK_H_
#define GITHUB_BYTESPIRIT_CPPSYNC_SYNC_LOCK_H_

#include <chrono>

namespace bytespirit {
namespace sync {

// Lock is the base class of all kinds of locks
class Lock {
 public:
  Lock() = default;
  virtual ~Lock() = default;

  // Lock is not copiable
  Lock(const Lock&) = delete;
  auto operator=(const Lock&) -> Lock& = delete;

  // Wait for the lock
  auto virtual Wait() -> void = 0;
  // Wait for the lock with timeout
  auto virtual Wait(std::chrono::seconds timeout) -> bool = 0;
  // Wait for the lock with deadline
  auto virtual Wait(std::chrono::system_clock::time_point deadline) -> bool = 0;
};

}  // namespace sync
}  // namespace bytespirit

#endif  // GITHUB_BYTESPIRIT_CPPSYNC_SYNC_LOCK_H_
