# sync_flag_id Example

## Overview

This example verifies the `uint16_t flag_id` signatures of the inter-core synchronization APIs. Each `arrive/wait` pair receives the same ordinary `int64_t` local variable whose value exceeds the `uint16_t` range. Separate modes use distinct low-bit IDs to isolate their hardware counters: block maps 65537 to 1, subblock maps 65538 to 2, inter maps 65544 to 8, and the `dav-3510` intra path maps 65539 to 3.

The kernel launches two AI Cores. Each synchronization mode runs in a separate process. Participating AICs or AIVs write a completion marker to GM after synchronization, and the Host verifies that the kernel returns within the timeout and writes at least one marker. Separate processes isolate hardware synchronization counters and make a mismatched `flag_id` easy to locate.

## Run

```bash
export ASCEND_HOME_PATH=/path/to/cann
bash run.sh dav-2201
```

Use `bash run.sh dav-3510` on an Ascend 950 environment. The script temporarily links the CANN C API include and implementation directories to the current repository and restores the original paths on exit. Each mode has a 60-second timeout by default; set `SYNC_TEST_TIMEOUT` to override the limit. Set `SYNC_BUILD_ONLY=1` to compile the 3510 path without running it on a non-3510 system.

The 2201 block, subblock, and inter modes passed on an NPU with the out-of-range signed 64-bit values 65537, 65538, and 65544 respectively. The 3510 code and script are provided for compilation and NPU verification in a 3510 environment; they are not executed in the current 2201 environment.
