# Restaurant Order System

A C++17 console program that simulates a restaurant kitchen using multithreading.
One waiter thread takes orders from tables, and several cook threads cook them at the same time.
It shows the producer-consumer pattern with a thread-safe queue, a mutex and a condition variable.

## Features

- Menu of 10 dishes; the waiter takes orders table by table (0 = done, 0 = close)
- An invalid dish number shows "Invalid choice" and places no order
- Two cooks take orders from a shared counter at the same time
- Cooks wait on a condition variable when the counter is empty, so they don't waste CPU
- Clean shutdown at closing time: cooks finish the remaining orders, then go home
- Orders placed after closing are refused
- Built with CMake; 3 Google Test unit tests for the order queue

## How it works

- `OrderQueue` (in `include/` and `src/`) is the shared counter. It hides a `std::queue`, a `std::mutex` and a `std::condition_variable`.
- The waiter calls `addOrder()`, which locks the mutex, pushes the order and wakes one cook with `notify_one()`.
- Each cook calls `takeOrder()`. If the counter is empty, the cook sleeps until it is woken.
- At closing time the waiter calls `close()`. When the counter is closed **and** empty, `takeOrder()` returns `false`, and the cook goes home.

## Build and run

Requirements: Linux (tested on WSL Ubuntu), a C++17 compiler, CMake 3.16+, Google Test (`sudo apt install libgtest-dev`).

```bash
cmake -S . -B build
cmake --build build
./build/restaurant
```

## Run the tests

```bash
./build/tests
```

Expected result: `[  PASSED  ] 3 tests.`

## Known limitations

- Console output (`std::cout`) is printed outside the lock, so lines from different threads can occasionally mix on screen. The order data itself is always protected.
- Typing letters instead of a number at the menu is not handled yet.

More details: [SDLC plan](docs/sdlc-plan.md)