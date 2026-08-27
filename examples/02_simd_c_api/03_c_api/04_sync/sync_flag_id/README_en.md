# sync_flag_id Example

## Overview

This example verifies the `uint16_t flag_id` signatures of the inter-core synchronization APIs. Matching constants are shared by each `arrive/wait` pair. The `dav-2201` path covers block, subblock, and inter-group synchronization. The `dav-3510` path also covers intra-group synchronization.

The kernel launches two AI Cores. Each synchronization mode runs in a separate process. Participating AICs or AIVs write a completion marker to GM after synchronization, and the Host verifies that the kernel returns within the timeout and writes at least one marker. Separate processes isolate hardware synchronization counters and make a mismatched `flag_id` easy to locate.

## Run

```bash
export ASCEND_HOME_PATH=/path/to/cann
bash run.sh dav-2201
```

Use `bash run.sh dav-3510` on an Ascend 950 environment. The script temporarily links the CANN C API include and implementation directories to the current repository and restores the original paths on exit. Each mode has a 60-second timeout by default; set `SYNC_TEST_TIMEOUT` to override the limit. Set `SYNC_BUILD_ONLY=1` to compile the 3510 path without running it on a non-3510 system.

The 2201 block, subblock, and inter modes passed on an NPU. The 3510 block, subblock, inter, and intra paths compile successfully; their NPU run is deferred to a 3510 environment and is not a merge blocker for the current 2201 environment.
