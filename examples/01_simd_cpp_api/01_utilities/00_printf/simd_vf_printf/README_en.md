# SimdVF Printf API Description

## Overview

This example demonstrates FIFO transfer for the simd_vf printf API. The kernel prints more than 2 KB of data from a single `asc_vf_call`, verifying that output remains complete after crossing the reserved UB boundary.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---------|----------------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
├── simd_vf_printf
│   ├── CMakeLists.txt          // Build project file
│   ├── simd_vf_printf.asc      // Ascend C example implementation & invocation example
│   └── README.md               // Example description document
```

## Example Description

- Example features:

  Using the vector programming mode, one simd_vf function prints 32 fixed-format records with sequence numbers. Each record is at least 96 bytes, so the total text output is at least 3072 bytes and exceeds the 2 KB reserved UB space.

- Invocation implementation

  The `__global__ __vector__` kernel invokes `asc_vf_call<SimdVfPrintFifoData>(PRINT_RECORD_COUNT)` exactly once. The simd_vf function calls `printf` in a loop and emits a complete sequence from `record=0` through `record=31`. The output crosses the FIFO buffer boundary, so observing the complete first-to-last sequence verifies continued FIFO transfer.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Configure environment variables
  Configure environment variables based on the [installation method](../../../../../docs/zh/quick_start.md#prepare&install) of the CANN development kit on the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in the example directory.

  ```bash
  mkdir -p build && cd build;                                               # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # Build the project
  ./demo                                                                     # Run the example
  ```

- Build options

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` (default) | NPU architecture: dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution result
  The execution log should contain all 32 simd_vf records from `record=0` through `record=31`. Intermediate records are omitted below.

  ```bash
  [AIV Block 0/1] [aicore] simd_vf fifo printf demo start ...
  [simd_vf][fifo] record=0 payload=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
  [simd_vf][fifo] record=1 payload=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
  ...
  [simd_vf][fifo] record=31 payload=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
  [AIV Block 0/1] [aicore] simd_vf fifo printf demo end ...
  ```
