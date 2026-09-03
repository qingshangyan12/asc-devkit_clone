# asc_icache_preload Invalid-Input Reproducer

## Overview

This standalone Ascend C and ACL example reproduces a delayed device error after a Vector Core executes `asc_icache_preload` with an out-of-range length. The first kernel and synchronization may complete successfully, while synchronization of a subsequent unrelated kernel reports a device error. The example does not depend on Python, PyTorch, or torch_npu.

> **Notice:** This example intentionally exercises invalid input and is not a valid usage example for `asc_icache_preload`. `prefetch_len` is measured in 2 KB units. A Vector Core has a 16 KB ICache, and the API requires `prefetch_len < 8`; this reproducer deliberately passes `8`.

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```text
├── icache_preload_issue
│   ├── CMakeLists.txt
│   ├── icache_probe_kernel.asc
│   ├── unrelated_probe_kernel.asc
│   ├── main.cpp
│   ├── README.md
│   └── README_en.md
```

## Execution Flow

1. Dynamically load the first kernel library and launch its kernel, with or without `asc_icache_preload` according to the command-line option.
2. Call `aclrtSynchronizeStream` and print `FIRST_SYNC_OK` or `FIRST_SYNC_FAIL`.
3. Dynamically load a separate second kernel library and launch an unrelated kernel that writes 128 bytes to GM.
4. Synchronize again. Print `PROBE_FAIL acl_error=<code>` on failure, or validate both kernel outputs and print `PROBE_OK` on success.

## Build and Run

Configure the CANN environment, then run the following commands in this example directory:

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
```

Run the control case without preload:

```bash
./demo --no-preload
```

Expected output:

```text
FIRST_SYNC_OK
PROBE_OK
```

Run the out-of-range preload case:

```bash
./demo --preload
```

The issue environment typically completes the first synchronization and reports `507035` at the second synchronization:

```text
FIRST_SYNC_OK
PROBE_FAIL acl_error=507035
```

The exact failure stage and error code depend on the runtime environment.
