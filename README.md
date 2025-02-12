# Emscripten WASM Worker C++ Synchronisation Primitives
Header-only C++ library offering drop-in replacements for `std::mutex`, using Emscripten's WASM Worker API.

# Why?
You may wish to build multithreaded programs for Emscripten using WASM Workers, without compiling with `-pthread`.  In this case, `std::mutex` will not work (see https://github.com/emscripten-core/emscripten/issues/23661).

In this situation, `emscripten_sync::mutex` can be used as a drop-in replacement.

Even if you have `-pthread` enabled, `emscripten-sync::mutex` may offer better performance than `std::mutex` when used with WASM workers, as it may have lower overhead.  Benchmark both approaches to be sure you're choosing the best for your application.

# How?
Just use `emscripten_sync::mutex` wherever you'd use `std::mutex` explicitly, or where a template parameter defaults to `std::mutex`.

For example,
```cpp
std::mutex mutex;
std::scoped_lock lock{mutex};
```
can be replaced with
```cpp
emscripten_sync::mutex mutex;
std::scoped_lock lock{mutex};
```

# Contributions welcome
At the time of writing, only `mutex` is implemented.  Other synchronisation primitives could also be implemented using Emscripten's WASM worker API:

- TODO: implement `std::counting_semaphore` equivalent, using `emscripten_semaphore_release` and `emscripten_semaphore_waitinf_acquire` / `emscripten_semaphore_try_acquire`.
  - TODO: implement `std::binary_semaphore` equivalent using the above.
- TODO: implement `std::condition_variable` equivalent, using `emscripten_condvar_wait`, `emscripten_condvar_waitinf` and `emscripten_condvar_signal`. 

## Other useful libraries

You may also find the following Emscripten helper libraries useful:

- [Emscripten Browser File Library](https://github.com/Armchair-Software/emscripten-browser-file) - allows you to transfer files using the browser upload / download interface, into memory in your C++ program.
- [Emscripten Browser Clipboard Library](https://github.com/Armchair-Software/emscripten-browser-clipboard) - easy handling of browser copy and paste events in your C++ code.
- [Emscripten Browser Cursor](https://github.com/Armchair-Software/emscripten-browser-cursor) - easy manipulation of browser cursors, for Emscripten applications.
- [Emscripten Audio](https://github.com/Armchair-Software/emscripten-audio) - Lightweight C++ library to make it easy to work with realtime low-latency WASM audio worklets under Emscripten. 
