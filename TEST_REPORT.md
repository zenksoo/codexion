# Codexion implementation review

Tested on 2026-11-05 against `en.subject.txt`, version 1.5. No implementation
changes were made during this review.

## Results

| Check | Result |
| --- | --- |
| Existing `codexion_tester.sh` | 43 passed, 0 failed; Valgrind checks skipped because Valgrind is not installed. |
| AddressSanitizer, UndefinedBehaviorSanitizer, and LeakSanitizer | Eight normal execution cases passed without diagnostics. Leak checks required execution outside the sandbox. |
| ThreadSanitizer | Eight normal execution cases passed without diagnostics. |
| Norminette | All 11 C files and the header passed. |
| Repeated feasible EDF workload | Unexpected burnout in 8 of 30 runs of the default Makefile build; also 6 of 30 runs of an `-O1` build. |
| Allocation failure injection | Failing any of the first nine application allocations caused an invalid access in cleanup. |
| Thread creation failure injection | Cleanup destroyed synchronization objects while previously created threads were still waiting for startup. |
| Controlled monitor interleaving | Reproduced a false burnout using a stale compile-start timestamp. |
| Additional argument validation | Sign-only values and overflowing integers were accepted. |
| Header dependency check | Changing the header does not trigger recompilation. |

Passing the ordinary tests does not cover the failure paths and scheduling
interleavings below. In particular, ThreadSanitizer checks data races; a program
can protect each individual access with a mutex and still make an incorrect
decision using an outdated snapshot.

Local test binaries, test-only wrappers, and captured logs are in
`/tmp/codexion-review-20261105/`. The original tester's output is in
`/tmp/codexion-tester-audit.log`.

## 1. High: EDF can burn out coders with feasible parameters

**Location:** `src/dongle.c:50` and `src/dongle.c:69`; priority comparison in
`src/queue_helper.c:15`.

Reproduce by running this command repeatedly:

```sh
make
./codexion 4 80 10 10 10 5 20 edf
```

The default build burned out in **8 of 30 runs**. One captured run let coder 1
compile at 0 and 30 ms, delayed coders 2 and 4 until 60 ms, and then reported
`81 3 burned out`. See `baseline-edf-13.log` in the diagnostics directory.

These parameters are feasible: coders 1 and 3 can compile at 0, 60, 120, ... ms,
and coders 2 and 4 at 30, 90, 150, ... ms. Each compile takes 10 ms, each release
has a 20 ms cooldown, debugging and refactoring take 20 ms combined, and every
coder's compile-start interval is 60 ms, below the 80 ms burnout limit.

The current policy selects a winner independently for each dongle. A coder can
hold one dongle while waiting for its second, and local priority/tie decisions
can delay another coder past its deadline. Correct heap ordering alone does not
establish the subject's EDF liveness guarantee.

**Suggested fix:** Coordinate acquisition and arbitration of the two required
resources so that partial ownership and repeated local wins cannot prevent a
feasible global schedule. Keep this workload as a repeated regression test.

## 2. High: allocation failures crash the cleanup path

**Location:** `src/cleanup.c:37`, especially the dereference at line 39;
partial initialization in `src/init.c:54`.

`destroy_dongles()` assumes `table->dongles` and all `n` entries exist. That is
false if allocating `table->args`, the dongle pointer array, a dongle, or an
early dongle's heap fails. Entries after a partially initialized dongle are also
uninitialized because the pointer array is not zeroed.

Test-only linker wrappers made one application `malloc` return `NULL` at a time.
For three coders, failures at allocation calls 1 through 9 all produced invalid
accesses. The first allocation failure produced:

```text
src/cleanup.c:39:21: runtime error: load of null pointer of type 'struct t_dongle *'
SUMMARY: AddressSanitizer: SEGV src/cleanup.c:39 in destroy_dongles
```

The wrapper source is `faults.c` in the diagnostics directory. With its compiled
binary, this reproduces the first failure:

```sh
AUDIT_FAIL_MALLOC=1 ASAN_OPTIONS=detect_leaks=0 \
  /tmp/codexion-review-20261105/codexion-faults 3 1000 10 10 10 2 0 edf
```

**Suggested fix:** Zero pointer arrays immediately after allocation, check array
and element pointers before dereferencing them, and track how many resources
were initialized. Apply the same partial-initialization discipline to coder
cleanup and synchronization initialization failures.

## 3. High: thread creation failure cleans up live threads

**Location:** `src/simulation.c:71`, `src/simulation.c:87`, and
`src/main.c:30`.

If a coder's `pthread_create()` fails, `run_simulation()` returns immediately.
The main thread then destroys mutexes and frees resources without stopping or
joining the monitor and any coder threads already created. Those threads are
still polling `ready`, which remains zero. `wait_for_start()` does not check a
stop flag, so simply setting `stop` would not release them either.

Failing the third `pthread_create()` reproduced this diagnostic before cleanup:

```text
AUDIT: failing pthread_create call 3
AUDIT: destroying mutex with 2 threads created and 0 joined
```

Reproduction with the test wrapper:

```sh
AUDIT_FAIL_CREATE=3 ASAN_OPTIONS=detect_leaks=0 \
  /tmp/codexion-review-20261105/codexion-faults 3 1000 10 10 10 2 0 edf
```

**Suggested fix:** Track successful thread creation, make the startup wait
cancellable, notify already-created threads of failure, join them, and only then
destroy shared synchronization objects and free their data.

## 4. High: the monitor can report a false burnout

**Location:** `src/monitor.c:49` and the compile-start update in
`src/coder.c:38`.

The monitor copies `last_compile_start` under the coder mutex, unlocks it, then
reads the clock and decides whether the coder burned out. A coder can update
its timestamp between the snapshot and that decision.

A deterministic two-thread harness, `deadline_race.c`, replayed this sequence
using a controlled clock:

1. The monitor reads a last compile start of 1000 ms; burnout interval is 100 ms.
2. The coder starts another compile at 1099 ms, before the old deadline, and
   stores that timestamp under its mutex.
3. The monitor resumes at 1100 ms and uses the stale 1000 ms snapshot.
4. It reports burnout even though the current deadline is 1199 ms.

The harness links the existing implementation without changing its source:

```text
now=1100 last_compile_start=1099 deadline=1199
expected burnout=0, observed burnout=1
```

**Suggested fix:** Make the timestamp/time comparison and burnout decision
consistent with compile-start updates, using a consistent lock order. Protecting
only the field read is insufficient.

## 5. Medium: malformed and overflowing numeric inputs are accepted

**Location:** `src/cli_argparser.c:24`, `src/cli_argparser.c:51`, and conversions
in `src/init.c:83`.

`is_valid_number()` skips a sign without requiring a following digit. Validation
also relies on the converted `int` from `atoi()` without checking its range.
These commands all returned exit status 0 instead of rejecting the input:

```sh
./codexion 2 + 1 1 1 1 0 fifo
./codexion 2 - 1 1 1 1 0 fifo
./codexion 4294967298 100 1 1 1 1 0 fifo
./codexion 2 -4294967196 1 1 1 1 0 fifo
./codexion 2 100 1 1 1 1 4294967296 fifo
```

On the tested platform, the oversized coder count became 2, the negative burnout
duration became 100, and the oversized cooldown became 0. Storing those results
in `long long` afterward does not restore the original values.

**Suggested fix:** Require at least one digit, reject negative values before
conversion, and perform checked digit-by-digit conversion with a defined upper
bound appropriate for each destination type.

## 6. Medium: startup consumes deadlines while the monitor is blocked

**Location:** `src/simulation.c:68`, `src/simulation.c:83`, and
`src/monitor.c:84`.

The simulation clock starts before thread creation, initial coder timestamps are
assigned at different times during creation, and the monitor cannot check any
deadline until every coder thread has been created. Slow startup can therefore
cause a burnout before the startup gate opens, with its report delayed beyond
the required 10 ms tolerance.

A controlled test inserted 10 ms into each `pthread_create()` call:

```sh
AUDIT_CREATE_DELAY_US=10000 ASAN_OPTIONS=detect_leaks=0 \
  /tmp/codexion-review-20261105/codexion-faults 4 20 1 1 1 1 0 edf
```

The captured run reported `53 1 burned out` for a 20 ms burnout interval. This
is a startup-delay injection test, not a claim that every ordinary run has this
latency.

**Suggested fix:** After creating all threads successfully, establish a common
simulation epoch and initialize every coder's initial deadline from it before
opening the startup gate.

## 7. Medium: source calls functions outside the subject's allowlist

The mandatory external-function list in `en.subject.txt` does not include:

- `fflush`: called in `src/coder.c:25`, `src/monitor.c:26`, and
  `src/cli_argparser.c:20`.
- `pthread_condattr_init` and `pthread_condattr_setclock`: called in
  `src/init.c:18`.
- `pthread_condattr_destroy`: called in `src/cleanup.c:44`.

These are direct source calls, not merely compiler-generated library symbols.
Norminette passing does not establish compliance with this function list.

**Suggested fix:** Adapt output and condition-variable setup to the permitted
APIs in the supplied subject.

## 8. Medium: header changes leave stale objects

**Location:** `Makefile:46`.

Object rules depend only on their `.c` files. `include/codexion.h` is not a
prerequisite, and compiler-generated dependency files are not used. After a
successful build:

```sh
make -n -W include/codexion.h
```

reported `make: 'codexion' is up to date.` The `-W` option simulates a changed
header without modifying it. Real changes to structures, constants, or
declarations can therefore leave the executable built against an older header.

**Suggested fix:** Add the header as an object prerequisite, or generate and
include dependency files with `-MMD -MP`.
