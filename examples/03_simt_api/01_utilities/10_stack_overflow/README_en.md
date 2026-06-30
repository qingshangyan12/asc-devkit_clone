# SIMT Stack Overflow Scenario and Optimization Example

## Overview

This example uses large local array definitions inside kernels causing stack overflow to introduce stack overflow issues and optimization approaches in Ascend C SIMT programming. The example includes 1 baseline version and 1 optimized version, both using the same kernel code but with different initialization configurations: the baseline version uses `aclInit(nullptr)` with default stack size, causing stack overflow; the optimized version loads a configuration file via `aclInit("acl.json")` to configure a larger SIMT stack size, avoiding stack overflow.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Software Version

- \> CANN 9.0.0

## Directory Structure

```text
├── 10_stack_overflow
│   ├── CMakeLists.txt          // CMake build file
│   ├── stack_overflow.asc      // Ascend C operator stack overflow and optimization example
│   ├── acl.json                // aclInit configuration file, configures SIMT stack size
│   └── README.md
```

## Example Description

- Example Function

  Define large local arrays inside the kernel for data filling and sum computation, comparing execution results under default stack size and configured stack size.

- Example Specifications:

  <table>
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">SimtStackOverflow</td></tr>
  <tr><td rowspan="1" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[128]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[128]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">simt_stack_overflow</td></tr>
  </table>

## Example Implementation

### Stack Overflow Issue Description

In SIMT kernels, each thread has its own stack space, but the stack size is limited. When large local arrays are defined inside a kernel, they are allocated on the stack. If there are many threads and each thread uses a large amount of stack, the total stack demand will exceed the stack space limit, causing **stack overflow**.

This example selects different initialization configurations through the `SCENARIO_NUM` compile-time macro to compare the stack overflow issue with the optimization solution.

| Case   | Initialization          | Stack Size     | Implementation Features                                      | Optimization Feature                |
|--------|-------------------------|---------------|-------------------------------------------------------------|---------------------|
| Case 1 | `aclInit(nullptr)`      | Default (small) | Define large local arrays inside kernel (float local_buffer[320]), default stack size insufficient causing stack overflow    | Baseline version with stack overflow issue        |
| Case 2 | `aclInit("acl.json")`   | Configured (1MB) | Same kernel, configure larger simt_stack_size via acl.json to avoid stack overflow              | Stack overflow optimization by expanding stack space via configuration file    |

#### 1. Stack Space Analysis

| Configuration          | Case 1 (Baseline)      | Case 2 (Optimized)       |
|--------------|--------------------|--------------------|
| Stack usage per thread       | `320 * 4B = 1280B`  | `320 * 4B = 1280B`  |
| Total threads         | `2 * 32 = 64`      | `2 * 32 = 64`      |
| Total stack demand         | `1280B * 64 = 80KB` | `1280B * 64 = 80KB` |
| Stack size config        | Default (small)            | `1MB` (acl.json config)  |
| **Result**       | **Stack overflow, kernel execution fails** | **Kernel executes successfully**     |

---

### acl.json Configuration Description

By loading the configuration file via `aclInit("acl.json")`, you can configure SIMT stack size and other parameters in `acl.json`. The `acl.json` configuration example is as follows:

```json
{
    "StackSize": {
        "simt_stack_size": 1048576,
        "simt_divergence_stack_size": 512
    }
}
```

- `simt_stack_size`: SIMT thread stack size, in bytes. Configured to 1048576 (1MB) to ensure that large local arrays in the kernel do not cause stack overflow.
- `simt_divergence_stack_size`: SIMT thread divergence stack size, in bytes.

> **Reference: ** [aclInit API Documentation](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900/API/runtimeapi/aclcppdevg_03_0022.html)

---

### Case 1: Baseline Version

**Example Goal**: Reproduce the stack overflow issue caused by defining large local arrays inside the kernel

**Core Implementation**:
- Use `aclInit(nullptr)` for default initialization, without loading configuration file
- Define `float local_buffer[320]` (1280B/thread) inside the kernel, allocated on the stack
- Default stack size insufficient, 64 threads with total stack demand of 80KB, causing stack overflow

**Key Code**:

```cpp
// Baseline version: initialize with default config, do not load acl.json
aclInit(nullptr);
```

```cpp
__global__ void simt_stack_overflow(float* input, uint32_t in_shape)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    float local_buffer[BUFFER_SIZE]; // Large local array allocated on stack
    // ...
}
```

**Expected Result**: Kernel execution fails, `aclGetRecentErrMsg()` returns stack overflow related error message.

---

### Case 2: Optimized Version

**Optimization Goal**: Expand stack space through configuration file to eliminate stack overflow risk

**Core Optimization**:
- Use `aclInit("acl.json")` to load configuration file
- Configure `simt_stack_size` to 1MB in `acl.json`, far exceeding the 80KB total stack demand
- Kernel code is identical to the baseline version, no modification needed

**Key Code**:

```cpp
// Optimized version: load acl.json configuration file via aclInit to configure SIMT stack size
aclInit("acl.json");
```

**Expected Result**: Kernel executes successfully.

---

## Optimization Summary

| Scenario     | Root Cause                                         | Solution                                   |
|--------|----------------------------------------------|----------------------------------------|
| Stack overflow    | Large local arrays defined in kernel (e.g., `float buf[N]`), stack demand exceeds limit with multiple threads     | Configure larger `simt_stack_size` via `acl.json`, or migrate to global memory |
| Stack overflow    | Excessive recursion depth                                     | Avoid recursion in kernels, use iterative implementation                   |
| Stack overflow    | Deep call chains, too many parameters                                | Simplify call chain, use structs/global memory to pass large parameters                 |

## Build and Run

Run the following steps in the root directory of this example to build and execute the example.

- Configure Environment Variables  
  Configure environment variables based on the [installation method](../../../../docs/quick_start.md#prepare&install) of the CANN development kit in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the Example

  Run the following commands in this example directory.

  ```bash
  SCENARIO_NUM=1                       # Select scenario, options 1-2
  mkdir -p build && cd build;          # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # Build project
  ./demo                               # Run example
  ```

  > **Note:** The `--cce-res-usage` compile option is enabled by default. During compilation, it outputs hardware resource usage such as registers and Unified Buffer, making it easy to compare resource overhead between problematic and optimized code.

- Build Options Description

  | Option                        | Values        | Description                                                |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example only supports dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM`            | `1`-`2`    | Example type, default is 1, 1: baseline (stack overflow), 2: optimized (configured stack size)              |

  Execution results:

  Case 1 (Baseline) expected output:
  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=1)
  Running case 1: baseline (aclInit(nullptr), stack overflow)...
  ========================================
  [Host] Running kernel (SCENARIO_NUM=1)...
  ========================================
  [Host] kernel error: ... (stack overflow related error message)
  ```

  Case 2 (Optimized) expected output:
  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=2)
  Running case 2: optimized (aclInit(acl.json), stack configured)...
  ========================================
  [Host] Running kernel (SCENARIO_NUM=2)...
  ========================================
  [SIMT] thread 0: local_buffer addr = 0x...
  [SIMT] thread 1: local_buffer addr = 0x...
  ...
  [Host] kernel completed.
  ```