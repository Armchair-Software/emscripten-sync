#pragma once

#include <emscripten/wasm_worker.h>

namespace emscripten_sync {

class mutex {
  /// A std::mutex-compatible implementation using Emscripten's WASM Worker API locking functions
  /// This can be used even when std::mutex is unavailable, without needing the -pthreads flag.
  /// This class can be used as a drop-in replacement for std::mutex for std::scoped_lock etc.

public:
  using native_handle_type = uint32_t;

private:
  emscripten_lock_t emscripten_lock{EMSCRIPTEN_LOCK_T_STATIC_INITIALIZER};

public:
  mutex() noexcept;
  mutex(mutex const&) = delete;
  ~mutex();

  void lock() noexcept;
  bool try_lock() noexcept;
  void unlock() noexcept;

  native_handle_type native_handle() const noexcept;
};

inline mutex::mutex() noexcept {
  /// Constructs the mutex. The mutex is in unlocked state after the constructor completes.
  emscripten_lock_init(&emscripten_lock);
}

inline mutex::~mutex() {
  /// Destroys the mutex.
  emscripten_lock_release(&emscripten_lock);
}

inline void mutex::lock() noexcept {
  /// Locks the mutex. If another thread has already locked the mutex, a call to lock will block execution until the lock is acquired.
  if(emscripten_wasm_worker_self_id() == 0) {                                   // 0 == main browser thread, others are worker IDs
    emscripten_lock_busyspin_waitinf_acquire(&emscripten_lock);
  } else {
    emscripten_lock_waitinf_acquire(&emscripten_lock);
  }
}

inline bool mutex::try_lock() noexcept {
  /// Tries to lock the mutex. Returns immediately. On successful lock acquisition returns true, otherwise returns false.
  return emscripten_lock_try_acquire(&emscripten_lock);
}

inline void mutex::unlock() noexcept {
  /// Unlocks the mutex.
  emscripten_lock_release(&emscripten_lock);
}

inline mutex::native_handle_type mutex::native_handle() const noexcept {
  /// Returns the underlying implementation-defined native handle object, in this case an emscripten_lock_t.
  return emscripten_lock;
}

}
