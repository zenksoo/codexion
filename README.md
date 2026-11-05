*This project has been created as part of the 42 curriculum by amissa.*

# Codexion

## Description

Codexion is a concurrency simulation written in C using POSIX threads. It adapts
the dining philosophers problem: coders sit around a circular table and share
USB dongles needed to compile their code.

Each coder runs in a separate thread and repeats three activities: **compile,
debug, and refactor**. Compiling requires both adjacent dongles. After use, each
dongle must cool down before another coder can acquire it. A coder burns out if
too much time passes without starting a new compile.

The goal is to coordinate access to shared resources, prevent deadlock, and
schedule competing requests fairly. The implementation uses mutexes, condition
variables, and a custom binary min-heap for each dongle. It supports FIFO and EDF
scheduling and uses a separate monitor thread to detect burnout and completion.

The simulation ends when a coder burns out or when every coder has completed at
least the required number of compiles. Coders can exceed their individual quota
while waiting for everyone else to finish.

## Instructions

### Build

Build on Linux with `make`, a C compiler available as `cc`, and POSIX threads.
The implementation uses `CLOCK_MONOTONIC` for both time measurements and
condition-variable timeouts. No Libft or additional library installation is
required.

From the project root:

```sh
make
```

This creates `./codexion` with the flags `-g -Wall -Wextra -Werror -pthread`.

| Command | Purpose |
| --- | --- |
| `make` or `make all` | Build the executable. |
| `make clean` | Remove object files and the build directory. |
| `make fclean` | Remove object files, the build directory, and the executable. |
| `make re` | Clean everything and rebuild. |

### Run

All eight arguments are mandatory, in this order:

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
  time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Meaning |
| --- | --- |
| `number_of_coders` | Number of coder threads and dongles; must be greater than zero. |
| `time_to_burnout` | Maximum time without starting a new compile, measured from the previous compile start or initial startup. |
| `time_to_compile` | Time spent compiling while holding both dongles. |
| `time_to_debug` | Time spent debugging after releasing the dongles. |
| `time_to_refactor` | Time spent refactoring before requesting dongles again. |
| `number_of_compiles_required` | Number of completed compiles required from every coder. |
| `dongle_cooldown` | Time a released dongle remains unavailable. |
| `scheduler` | Exactly `fifo` or `edf`, in lowercase. |

All durations are in **milliseconds**. Supply non-negative integers for numeric
arguments, with a positive coder count. The burnout interval includes compiling,
debugging, refactoring, and time waiting for dongles.

### Examples

Run four coders with FIFO scheduling, a 2000 ms burnout limit, 100 ms per activity,
a target of three compiles each, and a 20 ms dongle cooldown:

```sh
./codexion 4 2000 100 100 100 3 20 fifo
```

Run the same configuration with EDF scheduling:

```sh
./codexion 4 2000 100 100 100 3 20 edf
```

Observe burnout with one coder, who cannot acquire two distinct dongles:

```sh
./codexion 1 300 100 100 100 1 0 fifo
```

### Output

Each line contains elapsed milliseconds since simulation startup, a coder ID
from `1` to `number_of_coders`, and an action:

```text
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

A compile starts after two dongle acquisition messages. Exact timestamps and
interleaving depend on thread scheduling. Reaching the compile target ends the
simulation without a separate success message. Burnout is a simulation outcome;
it does not itself produce a nonzero process exit status.

## Scheduling policies

Each dongle has its own binary min-heap of pending requests, protected by that
dongle's mutex. Only the coder at the top of the heap can acquire the dongle,
and only when it is free and its cooldown has expired.

| Policy | Priority key | Behavior |
| --- | --- | --- |
| FIFO — First In, First Out | Increasing request sequence number for that dongle | Serves pending requests in arrival order. |
| EDF — Earliest Deadline First | `last_compile_start + time_to_burnout` | Serves the waiting coder with the earliest burnout deadline. |

Equal priority keys are resolved by the smaller coder ID, making EDF ties
deterministic. Requests are scheduled separately for each dongle; a coder must
acquire both before beginning a compile.

## Blocking cases handled

- **Deadlock prevention and Coffman's conditions:** Deadlock requires mutual
  exclusion, hold and wait, no preemption, and circular wait. Dongles remain
  exclusive, a coder may hold one while waiting for another, and resources are
  released voluntarily. For multiple coders, the implementation breaks circular
  wait through asymmetric acquisition: even-numbered coders take left then right,
  while odd-numbered coders take right then left.
- **Starvation prevention:** FIFO prevents later requests from overtaking older
  pending requests on the same dongle. EDF prioritizes the closest burnout
  deadline to help coders compile before their time runs out. Meeting all
  deadlines still depends on feasible timing parameters and OS scheduling.
- **Dongle cooldown:** Releasing a dongle sets
  `available_at = now_ms() + dongle_cooldown`. Waiting coders check this timestamp
  before acquiring it, including after a condition-variable notification.
- **Single coder:** With only one dongle, the coder waits for an unavailable
  second acquisition until the monitor detects burnout and wakes the thread.
- **Precise burnout detection:** The monitor checks elapsed time against each
  coder's last compile start and sleeps for 500 microseconds between scans.
  Monotonic time avoids jumps caused by changes to the system clock. The subject
  requires burnout reporting within 10 ms; actual latency also depends on system
  load and scheduling.
- **Log serialization:** A dedicated mutex protects each complete output line.
  Coder logging checks the shared stop flag, and the monitor prints the burnout
  message through the same output mutex.
- **Shutdown while waiting:** When the monitor stops the simulation, it broadcasts
  to every dongle condition variable. Activity sleeps also check the stop flag
  repeatedly, allowing threads to exit before they are joined and resources are
  cleaned up.

## Thread synchronization mechanisms

The program creates one `pthread_t` per coder and one monitor thread using
`pthread_create`. On normal simulation shutdown, the main thread waits for them
with `pthread_join` before destroying synchronization objects and freeing memory.

| Primitive | Protected state or purpose |
| --- | --- |
| `t_dongle.lock` (`pthread_mutex_t`) | The dongle's `busy` flag, cooldown timestamp, and request heap. |
| `t_dongle.cond` (`pthread_cond_t`) | Waiting for dongle availability, cooldown expiry, or shutdown. |
| `t_coder.lock` (`pthread_mutex_t`) | The coder's `last_compile_start` and `compile_count`. |
| `t_table.state_lock` (`pthread_mutex_t`) | Shared `ready` and `stop` flags. |
| `t_table.log_lock` (`pthread_mutex_t`) | Complete log writes from coder and monitor threads. |

For example, a coder checks availability and marks a dongle busy while holding
the same dongle mutex. Two neighboring coders therefore cannot both claim a free
dongle. Heap insertion, inspection, and removal happen under that lock too.

`pthread_cond_timedwait` releases the dongle mutex while waiting and reacquires it
before returning. The acquisition loop then rechecks availability, cooldown, heap
priority, and stop state. This handles spurious wakeups and ensures that a
broadcast does not grant ownership by itself. Release notifications use
`pthread_cond_broadcast`; timed waits also let a coder wake when cooldown expires.

Coders update their last compile start and completed compile count under their
own mutex. The monitor reads those fields under the same mutex, preventing data
races between updates and deadline/completion checks. The monitor sets `stop`
under `state_lock`, releases that lock, and wakes dongle waiters. Coders read
`stop` through `is_stoped`, which acquires the same state mutex.

At startup, coders and the monitor poll the protected `ready` flag until all coder
threads have been created. This implementation uses POSIX mutexes and condition
variables directly, with no separate custom event abstraction.

## Project structure

| File | Responsibility |
| --- | --- |
| `include/codexion.h` | Shared structures, constants, and function declarations. |
| `src/main.c`, `src/cli_argparser.c` | Entry point and command-line validation. |
| `src/init.c`, `src/cleanup.c` | Allocation, initialization, and resource cleanup. |
| `src/simulation.c` | Thread creation, startup coordination, interruptible sleeps, and joining. |
| `src/coder.c`, `src/monitor.c` | Coder activity loop, logging, burnout detection, and completion checks. |
| `src/dongle.c` | Dongle acquisition, release, waiting, and cooldown. |
| `src/queue.c`, `src/queue_helper.c` | Binary min-heap and priority comparison. |
| `src/time.c` | Monotonic timestamps in milliseconds. |

## Resources

- [Codexion subject, version 1.5](en.subject.txt) — project rules and evaluation requirements.
- [POSIX thread creation](https://man7.org/linux/man-pages/man3/pthread_create.3.html) — thread entry points and lifecycle.
- [POSIX mutex locking](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html) — mutual exclusion and lock ownership.
- [POSIX condition-variable waits](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html) — timed waits, mutex interaction, and spurious wakeups.
- [Clock functions and `CLOCK_MONOTONIC`](https://man7.org/linux/man-pages/man2/clock_gettime.2.html) — measuring elapsed time.
- [Dining Philosophers Problem and Deadlock](https://opencsf.org/Books/csf/html/DiningPhil.html) — resource contention, deadlock conditions, and prevention strategies.

### AI use

For this documentation update, AI assisted with reading the subject and source
files, drafting and organizing this README, and
checking the build and example commands. The assistance covered the README's
usage instructions and explanations of the existing scheduling and
synchronization code; it did not change the C implementation.
