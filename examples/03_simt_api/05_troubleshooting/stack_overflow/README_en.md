# SIMT Stack Overflow Scenario and Optimization Example

## Overview

This example uses large local arrays and recursive calls inside kernels to introduce stack overflow issues and optimization approaches in Ascend C SIMT programming. The large-local-array scenario provides a baseline version and a configured-stack-size version: the baseline version uses `aclInit(nullptr)` with default stack size, causing stack overflow when stack usage is high; the configured-stack-size version loads a configuration file via `aclInit("acl.json")` to configure a larger SIMT stack size, avoiding stack overflow. In the recursive scenario, Case 3 launches a recursive kernel with no termination condition to reproduce stack overflow caused by infinite recursion; Case 4 launches a finite-recursion kernel with a termination condition to show how recursion affects SIMT thread stack usage.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Software Version

- \> CANN 9.0.0

## Directory Structure

```text
├── stack_overflow
│   ├── CMakeLists.txt          // CMake build file
│   ├── stack_overflow.asc      // Ascend C operator stack overflow and optimization example
│   ├── acl.json                // aclInit configuration file, configures SIMT stack size
│   └── README.md
```

## Example Description

- Example Function

  Define large local arrays inside the kernel, or accumulate multiple small stack frames through recursive calls. The kernel launches 64 resident threads to process 128 input elements, comparing execution results under default stack size and configured stack size, and showing the impact of infinite and finite recursion on SIMT thread stacks.

- Example Specifications:

  <table>
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">SimtStackOverflow</td></tr>
  <tr><td rowspan="1" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[128]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[128]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">simt_stack_overflow / simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized</td></tr>
  </table>

## Example Implementation

### Stack Overflow Issue Description

In SIMT kernels, each thread has its own stack space, but the stack size is limited. When large local arrays are defined inside a kernel, or when recursive calls keep accumulating active stack frames, each thread uses more stack. If there are many threads and each thread uses a large amount of stack, the total stack demand will exceed the stack space limit, causing **stack overflow**.

### Case Implementation Description

This example selects different initialization configurations and kernel implementations through the `SCENARIO_NUM` compile-time macro. Each Case corresponds to a stack overflow scenario or an explanation version.

| Case   | Implementation Features                                      | Kernel Used                                    | Optimization Feature                |
|--------|-------------------------------------------------------------|------------------------------------------------|---------------------|
| Case 1 | Use `aclInit(nullptr)`. The kernel defines a large local array (float local_buffer[320]), and the default stack size is insufficient, causing stack overflow | simt_stack_overflow                            | Baseline version with stack overflow issue        |
| Case 2 | Use `aclInit("acl.json")`. The same kernel configures a larger `simt_stack_size` through acl.json to avoid stack overflow | simt_stack_overflow                            | Stack overflow optimization by expanding stack space via configuration file    |
| Case 3 | Use `aclInit(nullptr)`. The kernel calls a recursive function with no termination condition, so recursion depth grows indefinitely and causes stack overflow | simt_recursive_stack_overflow                  | Infinite-recursion issue example |
| Case 4 | Use `aclInit("acl.json")`. The kernel calls a finite recursive function with a termination condition to observe stack frame accumulation and SIMT stack impact | simt_recursive_stack_overflow_optimized        | Recursion impact example |

#### 1. Stack Space Analysis

| Configuration          | Case 1 (Baseline)      | Case 2 (Configured Stack Size)       |
|--------------|--------------------|--------------------|
| Stack usage per thread       | `320 * 4B = 1280B`  | `320 * 4B = 1280B`  |
| Total threads         | `2 * 32 = 64`      | `2 * 32 = 64`      |
| Total stack demand         | `1280B * 64 = 80KB` | `1280B * 64 = 80KB` |
| Stack size config        | Default (small)            | `128KB` (acl.json config)  |
| **Result**       | **Stack overflow, kernel execution fails** | **Kernel executes successfully**     |

> **Note:** The input shape is `[128]`, and the kernel launches `2 * 32 = 64` resident threads. Each thread allocates `local_buffer` on the stack once and uses a grid-stride loop to cover all 128 input elements, so the stack demand is calculated by resident thread count.

#### 2. Recursive Call Scenario

Case 3/Case 4 use recursive device functions to show the impact of recursion on SIMT thread stacks. Recursive calls keep accumulating active stack frames, and deep recursion quickly increases the stack demand of each SIMT thread. In the current host dispatch logic, Case 3 launches `simt_recursive_stack_overflow`, whose `recursive_stack_stage` has no termination condition. This is infinite recursion and keeps consuming stack space until stack overflow occurs. Case 4 launches `simt_recursive_stack_overflow_optimized`, whose `recursive_stack_stage_optimized` adds a termination condition and loads the stack-size configuration through `aclInit("acl.json")`. It is used to show that finite recursion still accumulates stack frames and consumes stack space. This scenario shows why recursion should be avoided in SIMT kernels; prefer iterative implementations or explicit stack structures. Infinite recursion cannot be fixed by only increasing `simt_stack_size`; it must be given a termination condition or rewritten.

#### 3. Case 3 Infinite Recursion Analysis

Case 3 uses `aclInit(nullptr)` and launches `simt_recursive_stack_overflow`. This kernel calls `recursive_stack_stage`, which has no recursion termination condition:

```cpp
__aicore__ float recursive_stack_stage(float value) {
    return recursive_stack_stage(value - 1) * value;
}
```

Each recursive call creates another active stack frame, and the call chain never returns by itself. As recursion depth keeps increasing, each SIMT thread keeps consuming stack space until `The VEC SIMT stack overflows` is reported. This Case shows that infinite recursion is a logic issue and cannot be fixed by only increasing `simt_stack_size`; add a clear termination condition or rewrite the implementation as an iteration.

#### 4. Case 4 Finite Recursion Impact Analysis

Case 4 uses `aclInit("acl.json")` to load the stack-size configuration and launches `simt_recursive_stack_overflow_optimized`. This kernel calls `recursive_stack_stage_optimized`, which adds the `value < 1` termination condition:

```cpp
__aicore__ float recursive_stack_stage_optimized(float value)
{
    if (value < 1) {
        return 1.0f;
    }
    return recursive_stack_stage_optimized(value - 1) * value;
}
```

Finite recursion can return normally, but before the termination condition is reached, every recursive level still keeps an active stack frame. Therefore, deeper recursion increases per-thread stack usage. This Case shows that recursion itself affects SIMT thread stack usage; even with a larger `simt_stack_size`, keep recursion depth controlled and prefer iterative implementations.

---

### acl.json Configuration Description

By loading the configuration file via `aclInit`, you can configure SIMT stack size and other parameters in `acl.json`. CMake copies `acl.json` from the example root directory to the build directory, so the source code uses `aclInit("acl.json")` to load the configuration file from the current run directory. The `acl.json` configuration example is as follows:

```json
{
    "StackSize": {
        "simt_stack_size": 131072,
        "simt_divergence_stack_size": 512
    }
}
```

- `simt_stack_size`: SIMT thread stack size, in bytes. Configured to 131072 (128KB), which is larger than the stack demand in the large-local-array scenario and also leaves stack space for the finite-recursion scenario. For infinite recursion without a termination condition, increasing stack space can only delay failure and cannot fundamentally avoid stack overflow.
- `simt_divergence_stack_size`: SIMT thread divergence stack size, in bytes.

> **Reference:** [aclInit API Documentation](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900/API/runtimeapi/aclcppdevg_03_0022.html)

---

### Case 1: Baseline Version

**Example Goal**: Reproduce the stack overflow issue caused by defining large local arrays inside the kernel

**Core Implementation**:
- Use `aclInit(nullptr)` for default initialization, without loading configuration file
- Define `float local_buffer[320]` (1280B/thread) inside the kernel, allocated on the stack
- Default stack size insufficient, 64 threads with total stack demand of 80KB, causing stack overflow
- Call `aclGetRecentErrMsg()` after `aclrtSynchronizeStream` on the host side to obtain stack overflow details from kernel execution

**Key Code**:

```cpp
// Baseline version: initialize with default config, do not load acl.json
aclInit(nullptr);
```

```cpp
__global__ void simt_stack_overflow(float* input, uint32_t in_shape)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    uint32_t stride = blockDim.x * gridDim.x;
    float local_buffer[BUFFER_SIZE]; // Large local array allocated on stack
    for (; idx < in_shape; idx += stride) {
        // ...
    }
}
```

**Expected Result**: Kernel execution fails, `aclGetRecentErrMsg()` returns stack overflow related error message.

---

### Case 2: Configured Stack Size Version

**Example Goal**: Configure a larger SIMT thread stack through `acl.json` and verify the stack overflow fix for the large-local-array scenario.

**Core Implementation**:
- Use `aclInit("acl.json")` to load the configuration file
- Use the same kernel implementation as Case 1, still defining `float local_buffer[320]` on the stack
- Configure `simt_stack_size=131072` in `acl.json`, providing 128KB SIMT thread stack space
- The 128KB stack space is larger than this example's total stack demand, `1280B * 64 = 81920B`, so the kernel can complete normally

**Key Code**:

```cpp
// Optimized version: load acl.json through aclInit to configure SIMT stack size
aclInit("acl.json");
```

```json
{
    "StackSize": {
        "simt_stack_size": 131072,
        "simt_divergence_stack_size": 512
    }
}
```

**Expected Result**: Kernel execution succeeds, the log prints `[Host] kernel completed.`, and `The VEC SIMT stack overflows` no longer appears.

---

### Case 3: Infinite-Recursion Stack Overflow Version

**Example Goal**: Reproduce SIMT thread stack overflow caused by infinite recursion inside the kernel.

**Core Implementation**:
- Use `aclInit(nullptr)` for default initialization, without loading configuration file
- Launch the `simt_recursive_stack_overflow` kernel
- The kernel calls `recursive_stack_stage`, which has no termination condition
- Recursive calls keep accumulating active stack frames until the SIMT thread stack is exhausted and stack overflow occurs

**Key Code**:

```cpp
__aicore__ float recursive_stack_stage(float value) {
    return recursive_stack_stage(value - 1) * value;
}

__global__ void simt_recursive_stack_overflow(float* input, uint32_t in_shape)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx > in_shape) {
        return;
    }
    input[idx] = recursive_stack_stage(input[idx]);
}
```

**Expected Result**: Kernel execution fails, and `aclGetRecentErrMsg()` returns error information related to `The VEC SIMT stack overflows`.

> **Note:** Case 3 is a recursion logic issue example. Infinite recursion cannot be fundamentally fixed by only increasing `simt_stack_size`; add a termination condition or rewrite it as an iterative implementation.

---

### Case 4: Finite-Recursion Impact Version

**Example Goal**: Use a recursive function with a termination condition to show that finite recursion still accumulates stack frames and affects SIMT thread stack usage.

**Core Implementation**:
- Use `aclInit("acl.json")` to load the stack-size configuration
- Launch the `simt_recursive_stack_overflow_optimized` kernel
- `recursive_stack_stage_optimized` uses `value < 1` as the recursion termination condition
- Recursion can return normally, but recursion depth still determines the accumulated stack frames of each thread

**Key Code**:

```cpp
__aicore__ float recursive_stack_stage_optimized(float value)
{
    if (value < 1) {
        return 1.0f;
    }
    return recursive_stack_stage_optimized(value - 1) * value;
}

__global__ void simt_recursive_stack_overflow_optimized(float* input, uint32_t in_shape)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx > in_shape) {
        return;
    }
    input[idx] = recursive_stack_stage_optimized(input[idx]);
}
```

**Expected Result**: Kernel execution succeeds, and the log prints `[Host] kernel completed.`.

> **Note:** Case 4 explains the stack cost of recursion. It is not a configuration-only fix for the infinite-recursion issue in Case 3. In real development, avoid recursion in SIMT kernels where possible, and strictly control recursion depth when recursion is required.

---

### Error Collection and Analysis

The host calls `aclrtSynchronizeStream(stream)` to wait for kernel execution to finish, and then calls `aclGetRecentErrMsg()` to obtain the latest runtime error details. In this example, the stack overflow happens during kernel execution, so the recent error message contains the device-side stack overflow reason.

When running the example, save both standard output and standard error to keep the complete device-side error information:

```bash
./demo 2>&1 | tee case1_stack_overflow.log
```

> **Note:** This example demonstrates the troubleshooting flow. In Case 1, the process still exits with code 0 after printing the kernel error. Use the `The VEC SIMT stack overflows` log message to determine whether the stack overflow is reproduced.

Following the style of other SIMT examples, this example calls initialization, memory allocation, memory copy, and synchronization APIs linearly. After synchronizing kernel execution, it calls `aclGetRecentErrMsg()` to obtain the latest runtime error details:

```cpp
aclrtSynchronizeStream(stream);
const char* err = aclGetRecentErrMsg();
if (err != nullptr) {
    fprintf(stderr, "%s\n", err);
}
aclrtFree(input_device);
aclrtDestroyStream(stream);
```

Case 1 or Case 3 prints key error information similar to the following:

```text
EZ9999: Inner Error!
The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
An error occurred in the kernel task, retCode=0x31, [vector core exception].
AI Core kernel execution failed, device_id=0, stream_id=..., task_id=..., fault kernel_name=_Z19simt_stack_overflowPfj
rtStreamSynchronize execution failed, reason=vector core exception
```

Analyze it in the following order:

1. Confirm the error stage: after `aclrtSynchronizeStream` waits for kernel completion, `aclGetRecentErrMsg()` returns a device-side execution error, which means the failure happened during kernel execution.
2. Confirm the error type: `The VEC SIMT stack overflows` indicates VEC SIMT stack overflow. The hint `The local variable is too large or there are too many local variables` points to excessive stack usage by local variables.
3. Locate the faulting kernel: `fault kernel_name=_Z19simt_stack_overflowPfj` corresponds to the `simt_stack_overflow` kernel in this example.
4. Combine compiler resource information: `--cce-res-usage` prints `Stack size: 1280 bytes`, which means each SIMT thread uses 1280B of stack.
5. Calculate stack demand: the kernel launches `2` blocks with `32` threads per block, for `64` resident threads. The total stack demand is `1280B * 64 = 81920B`, about `80KB`.
6. Identify the root cause: Case 1 uses `aclInit(nullptr)` and does not configure a larger SIMT stack. The default stack size is insufficient, and `float local_buffer[320]` in the kernel causes stack overflow.
7. Verify the fix: Case 2 uses `aclInit("acl.json")` to load `simt_stack_size=131072`, configuring 128KB stack space. This exceeds the example's total stack demand, and the run prints `[Host] kernel completed.`. Case 4 is not a pure configuration fix for the infinite recursion in Case 3; it uses finite recursion with a termination condition to show recursion's stack impact.

---

### Troubleshooting Steps

When a SIMT kernel stack overflow occurs, guide the user through the following steps:

1. Reproduce the issue: run Case 1 or Case 3 first and confirm that the log contains `The VEC SIMT stack overflows`.
2. Confirm the fault location: find `fault kernel_name` in the log and check whether the faulting kernel is the target kernel.
3. Check per-thread stack usage: find `Stack size` in the build log. This value is the stack usage of one SIMT thread.
4. Calculate total stack demand: estimate it as "per-thread stack usage * resident thread count". Case 1/Case 2 use `1280B * 64 = 81920B`, about 80KB. Case 3/Case 4 should use the actual build output and runtime log.
5. Optimize stack usage first: avoid defining large temporary variables, arrays, or intermediate buffers as thread-local stack variables. When Unified Buffer (UB) can hold the data, prefer moving the buffer to UB. Plan UB usage based on operator concurrency, alignment, and available UB capacity.
6. Update the configuration file: if a larger thread stack is still required, configure a larger `simt_stack_size` in `acl.json`. This example uses `131072`, which is 128KB.
7. Load the configuration file: call `aclInit("acl.json")` on the host side. If running from another directory, adjust the configuration file path based on the actual working directory.
8. Verify the fix: rebuild and run the corresponding configured-stack-size version: Case 1 maps to Case 2. Confirm that the log prints `[Host] kernel completed.`, and confirm that the SIMT stack overflow error no longer appears. For the infinite recursion in Case 3, add a recursion termination condition or rewrite it as an iterative implementation. Case 4 is used to show the stack impact after changing to finite recursion.

If the configured-stack-size version still fails, check the following items first:

- Whether the `acl.json` path is correct and matches the working directory used to run `./demo`.
- Whether `simt_stack_size` is larger than the actual total stack demand, with some extra margin.
- Whether the kernel has added larger local arrays or recursion.
- Whether the example is running on the supported `dav-3510` architecture and Ascend 950PR/Ascend 950DT products.

## Optimization Summary

| Scenario     | Root Cause                                         | Solution                                   |
|--------|----------------------------------------------|----------------------------------------|
| Stack overflow    | Large local arrays defined in kernel (e.g., `float buf[N]`), stack demand exceeds limit with multiple threads     | Prefer moving large temporary variables or intermediate buffers to UB. If UB is not suitable, migrate them to global memory or configure a larger `simt_stack_size` via `acl.json` |
| Stack overflow    | Infinite recursion or excessive recursion depth                                     | Add a clear recursion termination condition. Prefer avoiding recursion in kernels and use iterative implementation                   |

> **Recommendation:** `simt_stack_size` is useful when a larger thread stack is truly required, but large arrays or large structs should not be kept on the thread stack long term. For large temporary variables or intermediate buffers, first evaluate whether they can be placed in UB to reduce per-thread stack usage and total stack demand under concurrent threads.

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

  Run Case 1 first to reproduce the issue:

  ```bash
  SCENARIO_NUM=1
  mkdir -p build_case1 && cd build_case1
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
  make -j
  ./demo 2>&1 | tee case1_stack_overflow.log
  ```

  Then return to the example directory and run Case 2 to verify the fix:

  ```bash
  cd ..
  SCENARIO_NUM=2
  mkdir -p build_case2 && cd build_case2
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
  make -j
  ./demo
  ```

  You can also run Case 3 to reproduce stack overflow caused by infinite recursion:

  ```bash
  cd ..
  SCENARIO_NUM=3
  mkdir -p build_case3 && cd build_case3
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
  make -j
  ./demo 2>&1 | tee case3_stack_overflow.log
  ```

  Then return to the example directory and run Case 4 to observe the impact of finite recursion with a termination condition:

  ```bash
  cd ..
  SCENARIO_NUM=4
  mkdir -p build_case4 && cd build_case4
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
  make -j
  ./demo
  ```

  > **Note:** The `--cce-res-usage` compile option is enabled by default. During compilation, it outputs hardware resource usage such as registers and Unified Buffer. Case 1/Case 2 output includes `Stack size: 1280 bytes`; Case 3/Case 4 should use the actual build output and runtime log to analyze per-thread stack usage.

- Build Options Description

  | Option                        | Values        | Description                                                |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example only supports dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM`            | `1`-`4` | Example type, default is 1. 1: large-array baseline, 2: large-array configured-stack-size version, 3: infinite-recursion stack-overflow version, 4: finite-recursion impact version |

  Execution results:

  Case 1 (Baseline) expected output:
  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=1)
  Running case 1: large local array baseline (aclInit(nullptr), stack overflow)...
  ========================================
  [Host] Running kernel (SCENARIO_NUM=1)...
  ========================================
  [Host] kernel error: EZ9999: Inner Error!
  The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
  ...
  rtStreamSynchronize execution failed, reason=vector core exception
  ```

  Case 2 (Configured stack size) expected output:
  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=2)
  Running case 2: large local array optimized (aclInit(acl.json), stack configured)...
  ========================================
  [Host] Running kernel (SCENARIO_NUM=2)...
  ========================================
  [Host] kernel completed.
  ```

  Case 3 (Infinite-recursion stack-overflow version) expected output:
  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=3)
  Running case 3: recursive call baseline (aclInit(nullptr), stack overflow)...
  ========================================
  [Host] Running kernel (SCENARIO_NUM=3)...
  ========================================
  [Host] kernel error: EZ9999: Inner Error!
  The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
  ...
  rtStreamSynchronize execution failed, reason=vector core exception
  ```

  Case 4 (Finite-recursion impact version) expected output:
  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=4)
  Running case 4: recursive call optimized (aclInit(acl.json), stack configured)...
  ========================================
  [Host] Running kernel (SCENARIO_NUM=4)...
  ========================================
  [Host] kernel completed.
  ```
