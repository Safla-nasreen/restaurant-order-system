# SDLC Plan: Restaurant Order System

This file explains how I apply the Software Development Life Cycle to this project.
It is updated whenever the real status of the project changes.

**Status words used in this repository**

| Status | Meaning |
|---|---|
| Planned | Decided, but no code yet |
| In Progress | Code started, not finished or not working yet |
| Implemented | Code works when run manually, but has no automated test yet |
| Tested | Covered by an automated test that passes |

Last updated: 20 Oct 2026

---

## 1. Planning and project scope

**Purpose:** decide what the program does, and what it does not do, before writing code.

**Scenario:** a small restaurant kitchen.
One waiter thread places orders on a shared counter (a queue).
Several cook threads take orders from the counter and cook them at the same time.
At closing time the waiter stops placing orders, the cooks finish the remaining orders, and the program ends cleanly.

**Why multithreading fits this scenario:** a waiter and cooks are independent workers who share one counter.
This is the producer-consumer problem, a common real-world multithreading pattern (for example, a network program receiving packets on one thread and processing them on others).

**Main technical goal:** show safe multithreading in C++: shared data protected by a mutex, threads waiting with a condition variable instead of busy-waiting, no race conditions or deadlocks, and a clean shutdown.

**In scope:** console program, one waiter, two or more cooks, a thread-safe order queue, clean shutdown, CMake build, automated tests.

**Out of scope:** GUI, database, billing, menu management, network communication.

**Approach (decided 19 Oct 2026):** build a working prototype in one file (`main.cpp`) first, then move one feature at a time into header and source files.
I chose this after starting with several files too early: a single file let me understand the program before splitting it.

**Evidence:** this section; the project folder created with `include/` and `src/`.

**Status:** Completed (scope may be revisited as the project grows).

**Done when:** the scenario, goal and scope are written down. ✔

---

## 2. Requirements analysis and acceptance criteria

**Purpose:** turn the scenario into small, checkable requirements, so I know when each feature is finished and what to test.

| ID | Requirement | Acceptance criteria | Status |
|---|---|---|---|
| REQ-001 | The waiter places orders (id + dish) on a shared counter. | Every order placed appears on the counter, in the order placed. | Implemented (main.cpp prototype) |
| REQ-002 | Two or more cooks take orders from the counter at the same time. | Each order is cooked exactly once; no order is lost or cooked twice. | Implemented (main.cpp prototype, 2 cooks) |
| REQ-003 | A cook with no order waits without using CPU (no busy-waiting). | Cooks sleep on a condition variable and wake when an order arrives. | Implemented (main.cpp prototype) |
| REQ-004 | Orders are taken in first-in, first-out order. | Order 1 is taken before order 2, and so on. | Implemented (main.cpp prototype) |
| REQ-005 | Each output line shows which cook cooks which order. | Output like `Cook 1 is cooking order 1: Biriyani`. | Implemented (main.cpp prototype) |
| REQ-006 | Clean shutdown at closing time: cooks finish remaining orders and exit without a fixed order count. | The program always ends; all threads are joined; no order is left on the counter. | Implemented (main.cpp prototype) |
| REQ-007 | The counter logic is a reusable thread-safe class (`OrderQueue`). | `main.cpp` uses only `addOrder`, `takeOrder` and `close`; the mutex is hidden inside the class. | Implemented (include/OrderQueue.h, src/OrderQueue.cpp) |
| REQ-008 | The project builds with CMake using C++17 and links the thread library. | `cmake` + `make` builds from a clean folder with no errors. | Planned |
| REQ-009 | Core behaviour is covered by automated tests (Google Test). | Tests for REQ-001, 002, 004 and 006 pass. | Planned |

**Known limitation (current prototype):** Fixed by REQ-006: cooks now stop when the counter is empty and closed, with no fixed order count. Verified manually with 5 orders and 2 cooks.

**Known limitation:** console output (`std::cout`) happens outside the lock, so lines printed by different threads could occasionally mix on screen. The order data itself is always protected inside `OrderQueue`.

**Evidence:** this table; `main.cpp` for the Implemented rows.

**Status:** In Progress

**Done when:** every requirement has acceptance criteria and a test case ID in `docs/test-plan.md`.

---

## 3. System and software design

**Purpose:** decide the parts of the program and how threads share data safely.

**Activities:** write `docs/design.md`, covering the components (`Order`, `OrderQueue`, waiter, cooks, `main`), the shared resources (the queue and the closed flag), how they are protected (one mutex, one condition variable), and the risks (race conditions, deadlock, missed wake-ups, shutdown).

**Design decisions made so far:**
- `Order` is a simple `struct` (id + dish).
- The counter is a `std::queue<Order>` protected by one `std::mutex`.
- Cooks wait with a `std::condition_variable`, using `wait()` inside a `while` loop (protects against spurious wake-ups and notify-before-wait).
- The waiter uses `std::lock_guard` (short critical section); cooks use `std::unique_lock`, because `wait()` must release and re-take the lock.
- Only one mutex, so lock-order deadlock cannot happen.

**Evidence:** `include/Order.h`, `include/OrderQueue.h`, `docs/design.md` (to be written).

**Status:** In Progress

**Done when:** `docs/design.md` describes every component and every shared resource, and matches the code.

---

## 4. Implementation and version control

**Purpose:** build the program in small, working steps, and record them in Git.

**Activities:**
- Prototype in `main.cpp`: steps 1–4 done (order struct, queue, waiter + cook threads, two cooks).
- Next: closing time (REQ-006).
- Then move the queue into `OrderQueue.h` / `OrderQueue.cpp`, one feature at a time, building after each move.
- Commit after each working milestone with a clear message.

**Evidence:** `main.cpp`, `include/`, `src/`, Git history.

**Status:** In Progress (no commits yet)

**Done when:** all requirements are implemented, the code is split into header and source files, and each milestone has a commit.

---

## 5. Verification and testing

**Purpose:** prove the requirements are met, not just "it ran once".

**Activities:** write `docs/test-plan.md` linking each `TEST-xxx` to a `REQ-xxx`; add Google Test tests; run the program several times, because thread timing changes between runs; record real bugs in `docs/defect-log.md`.

**Bugs found so far during manual runs (to be recorded in the defect log):**
- Cook wait condition was reversed (`while (!counter.empty())`), so the cook read from an empty queue and the program aborted. Fixed.
- Cook loop count didn't match the number of orders, so the program never ended. Fixed for now; REQ-006 fixes the root cause.

**Evidence:** manual runs only so far.

**Status:** Planned (no automated tests yet)

**Done when:** every requirement has at least one passing test, and the results are recorded honestly.

---

## 6. Release and build documentation

**Purpose:** anyone can download, build and run the project.

**Activities:** add `CMakeLists.txt` and `.gitignore` (ignore `build/` and the compiled `restaurant` program); write `README.md` with an overview, features, design summary, build steps and test steps; push to GitHub; tag `v1.0`.

**Evidence:** currently built by hand with `g++ main.cpp -o restaurant -pthread`.

**Status:** Planned

**Done when:** a fresh clone builds and runs by following only the README.

---

## 7. Maintenance and continuous improvement

**Purpose:** keep improving after v1.0 without breaking what works.

**Possible improvements (not committed to):** more cooks chosen at run time, cooking time per dish, a bounded counter size (the waiter waits when the counter is full).

**Status:** Planned

**Done when:** ongoing; each change goes through requirements → design → code → test again.

---

## How the phases feed back (iteration)

SDLC here is a loop, not a straight line. Real examples from this project:

- **Implementation → Planning:** the multi-file layout confused me before the program worked, so I changed the approach to a single-file prototype first (section 1).
- **Testing → Requirements:** the program hung when the cook loop count didn't match the order count. This showed a fixed count is fragile, so REQ-006 (clean shutdown) became a requirement.
