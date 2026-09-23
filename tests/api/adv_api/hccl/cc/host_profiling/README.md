# MC2 Host Mc2CommInfo experiment

## Scope

The Host reporter supplements HCOMM's original `KFC=0` relation with the actual
unfold runtime stream ID and all communication physical SQ IDs. HCOMM still
reports KfcInfo, including CONDITION tasks. Existing DFX registration and
message-level DeviceOp reporting are unchanged.

- A5 AICPU_TS, one communication domain, one capture per process/device.
- Resources may be created before START; only value snapshots are retained.
- `usrRankId` temporarily uses domain rank. Subcommunicator parent-rank semantics
  are not guaranteed by this experiment.
- No extra HEAD/TAIL, reserved marker, public API or serialization change.
- Registration/collection/report failure only warns; communication continues.
- STOP prevents further reports, including late-registration START races. A
  second full capture in the same process is intentionally unsupported.
- Callback state and its DSOs remain alive for the process lifetime. This
  experiment does not support resource destruction/recreation across domains.
- A toolkit whose HCCL module supports only a single callback is incompatible.
  Run the compatibility probe below before testing a different toolkit; do not
  replace HCOMM's callback to bypass this requirement.

## Portable state tests

From the repository root:

```sh
cmake -S tests/api/adv_api/hccl/cc/host_profiling -B /tmp/mc2-host-tests
cmake --build /tmp/mc2-host-tests
ctest --test-dir /tmp/mc2-host-tests --output-on-failure
```

Covers 1/8/9/17 SQs, both START/resource orders, duplicate notifications, partial
batch retry, per-device state, STOP and 200 concurrent registration/START races.
These tests do not mock or validate HCOMM resource queries or five algorithms.

## Installed Host profapi compatibility

This probe uses pure CPU profiling only; it does not launch NPU work or install
packages. Two callbacks register on HCCL module 3 before INIT; a third registers
after START. All three must receive INIT and START.

```sh
source "$CANN_PREFIX/set_env.sh"
cmake -S tests/api/adv_api/hccl/cc/host_profiling -B /tmp/mc2-host-tests \
  -DCANN_PREFIX="$CANN_PREFIX"
cmake --build /tmp/mc2-host-tests
mkdir -p /tmp/mc2-callback-output
/tmp/mc2-host-tests/prof_callback_probe /tmp/mc2-callback-output
```

The callbacks are probe functions, not a running HCOMM communicator. The test
checks the installed manager's coexistence/replay contract without board tasks.

## Installed parser relation test

```sh
python3 tests/api/adv_api/hccl/cc/host_profiling/parser_relation_probe.py \
  "$CANN_PREFIX/tools/profiler/profiler_tool/analysis"
```

Uses the installed parser methods and synthetic model rows. Tests both orders
of `KFC=0` plus `KFC=59`, 8+1 batching, duplicate relations and parent-rank
fallback. It checks mapping and task grouping, not complete raw-PROF export.

## Signed-package acceptance still required

With matching signed HCOMM/asc-devkit packages, run AG/RS/AR/A2A/A2AV at L1 and
ordinary HCCL controls. Check all main/sub SQs, Communication tasks, CONDITION,
no duplicate tasks, and resource-before-START. L0 should retain relation data.
Resource-query failure injection, complete raw-PROF replay and NPU execution
are not covered by the standalone Host tests above.
