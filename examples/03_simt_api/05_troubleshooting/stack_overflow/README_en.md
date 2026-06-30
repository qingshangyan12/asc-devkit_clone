# SIMT Stack Overflow Scenario and Optimization Example

## Overview

This example uses large local arrays and recursive calls inside kernels to introduce stack overflow issues and optimization approaches in Ascend C SIMT programming.
- Case 1: Defines a large local array and uses the default stack size. The large stack usage causes stack overflow.
- Case 2: Defines a large local array and loads `acl.json` through `aclInit("acl.json")` to configure a larger SIMT stack size and resolve stack overflow.
- Case 3: Uses recursive calls to calculate factorial and reproduce stack overflow caused by recursion.
- Case 4: Uses an iterative method to calculate factorial and avoid stack overflow caused by recursive calls.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Software Version

- \>= CANN 9.1.0

## Directory Structure

```text
├── stack_overflow
│   ├── CMakeLists.txt          // CMake build file
│   ├── stack_overflow.asc      // Ascend C operator stack overflow and optimization example
│   ├── acl.json                // aclInit configuration file, configures SIMT stack size
│   ├── README_en.md
│   └── README.md
```

## Example Description

- Example Function

  This example constructs stack overflow scenarios by building large arrays and calculating factorial recursively, then guides users to resolve stack overflow by increasing stack size and optimizing recursion.

- Example Specifications:

  <table>
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">SimtStackOverflow</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[2048]</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">simt_stack_overflow / simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized</td></tr>
  </table>

## Example Implementation

### Stack Overflow Issue Description

In SIMT development, the default stack size is limited. When large arrays are defined in a function or recursive calls are used, stack usage keeps increasing. If stack usage far exceeds the stack space limit, **stack overflow** occurs.

### Case Implementation Description

This example selects different initialization configurations and kernel implementations through the `SCENARIO_NUM` compile-time macro. Each Case corresponds to a stack overflow scenario or an explanation version.

| Case   | Implementation Features                                      | Kernel Used                                    | Optimization Feature                |
|--------|-------------------------------------------------------------|------------------------------------------------|---------------------|
| Case 1 | Use `aclInit(nullptr)`. The kernel defines a large local array `local_buffer[320]`, and the default stack size is insufficient, causing stack overflow | simt_stack_overflow                            | Large local array version with stack overflow issue |
| Case 2 | Use `aclInit("acl.json")`. The same kernel configures a larger `simt_stack_size` through `acl.json` to avoid stack overflow | simt_stack_overflow                            | Stack overflow optimization by expanding stack space via configuration file |
| Case 3 | Call a recursive function. The recursion depth grows and triggers stack overflow | simt_recursive_stack_overflow                  | Recursion stack overflow example |
| Case 4 | Use an iterative method to calculate factorial | simt_recursive_stack_overflow_optimized        | Iterative factorial calculation |

#### Stack Space Analysis

| Configuration          | Case 1 (Baseline)      | Case 2 (Configured Stack Size)       |
|--------------|--------------------|--------------------|
| Stack usage per thread       | `320 * 4B = 1280B`  | `320 * 4B = 1280B`  |
| Stack size config        | Default (small)            | `2048B` (`acl.json` config)  |
| **Result**       | **Stack overflow, kernel execution fails** | **Kernel executes successfully**     |

### acl.json Configuration Description

By loading the configuration file via `aclInit`, you can configure SIMT stack size and other parameters in `acl.json`. CMake copies `acl.json` from the example root directory to the build directory, so the source code uses `aclInit("acl.json")` to load the configuration file from the current run directory. The `acl.json` configuration example is as follows:

```json
{
    "StackSize": {
        "simt_stack_size": 2048,
        "simt_divergence_stack_size": 512
    }
}
```

- `simt_stack_size`: SIMT stack size for each thread, in bytes. It is configured to 2048, which is larger than the stack requirement of the large-local-array scenario and leaves stack space for the recursive scenario. For recursive calculation, increasing stack space can only delay failure and cannot fundamentally avoid stack overflow.
- `simt_divergence_stack_size`: SIMT thread divergence stack size, in bytes.

> **Reference:** [aclInit API Documentation](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900/API/runtimeapi/aclcppdevg_03_0022.html)

---

#### Recursive Calls and Iterative Factorial Calculation Scenario Description

| Configuration          | Case 3 (Recursive Version)      | Case 4 (Iterative Version)       |
|--------------|--------------------|--------------------|
| Stack usage       | Recursive calls exhaust stack space  | Iterative factorial calculation  |
| **Result**       | **Stack overflow, kernel execution fails** | **Kernel executes successfully**     |

> Recursion cannot be resolved by only increasing `simt_stack_size`. Recursion depth must be controlled or the implementation must be rewritten. Even when a larger `simt_stack_size` is configured, prefer an iterative implementation when recursion depth is too large.

---

### Case 1: Large-Array Version

**Example Goal**: Reproduce the stack overflow issue caused by defining large local arrays inside the kernel

**Core Implementation**:
- Use `aclInit(nullptr)` for default initialization, without loading configuration file
- Define `float local_buffer[320]` (1280B) inside the kernel, allocated on the stack
- The default stack space is insufficient, causing stack overflow
- Call `aclGetRecentErrMsg()` after `aclrtSynchronizeStream` on the host side to obtain stack overflow details from kernel execution

**Key Code**:

```cpp
// Large local array version: initialize with default config and do not load acl.json.
aclInit(nullptr);
```

```cpp
__global__ void simt_stack_overflow(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    float local_buffer[BUFFER_SIZE];

    for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
        local_buffer[i] = i + 1;
    }

    float sum = 1.0f;
    for (uint32_t i = 0; i < input[idx]; i++) {
        sum *= local_buffer[i];
    }
    output[idx] = sum;
}
```

**Result**: During compilation, the required stack size is printed as `_Z19simt_stack_overflowPfj_simt_entry: Stack size: 1280 bytes, Used register number: 16`. Execution fails, and `aclGetRecentErrMsg()` returns stack overflow related error information.

**Data Analysis**:

- The large array inside the function is statically defined. During compilation, `--cce-res-usage` can calculate the required stack size, so the log shows that the required stack space is `1280 bytes`.
- During runtime, `aclGetRecentErrMsg()` obtains the real error information. If the log contains `The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables`, the issue can be further identified as stack overflow.

**Optimization Suggestions**:

> **Define large local arrays in UB space**
>
> - For stack space insufficiency caused by too many variables or arrays, prefer optimizing the code by defining local variables in `UB` space and reducing stack definitions.
>
> **Expand the SIMT stack space through custom configuration**
>
> - If a large array must use stack space that exceeds the default configuration, use `aclInit("acl.json")` to load a custom configuration file and adjust the SIMT stack size.

---

### Case 2: Custom Configured Stack Size Version

**Example Goal**: Define a large array and configure a larger SIMT stack space through `acl.json` to verify the stack overflow fix for the large-array scenario.

**Core Implementation**:
- Use `aclInit("acl.json")` to load the configuration file
- Use the same kernel implementation as Case 1, still defining `float local_buffer[320]` on the stack
- Configure `simt_stack_size=2048` in `acl.json`, providing 2048B SIMT stack space
- The 2048B stack space is larger than this example's `1280B` stack requirement, so the function can execute normally

**Key Code**:

```cpp
// Optimized version: load acl.json through aclInit to configure SIMT stack size.
aclInit("acl.json");
```

```json
{
    "StackSize": {
        "simt_stack_size": 2048,
        "simt_divergence_stack_size": 512
    }
}
```

**Result**: During compilation, the required stack size is printed as `_Z19simt_stack_overflowPfj_simt_entry: Stack size: 1280 bytes, Used register number: 16`. Execution succeeds, and `The VEC SIMT stack overflows` no longer appears.

**Data Analysis**:

- The large array inside the function is statically defined. During compilation, `--cce-res-usage` can calculate the required stack size, so the log shows that the required stack space is `1280 bytes`.
- During runtime, `aclInit("acl.json")` loads the custom stack size, meeting the function's `1280 bytes` requirement. The kernel code is unchanged and can still run normally.

---

### Case 3: Recursive Stack Overflow Version

**Example Goal**: Reproduce SIMT stack overflow caused by recursive calls inside the SIMT function.

**Core Implementation**:
- Use `aclInit(nullptr)` for default initialization, without loading configuration file
- Launch the `simt_recursive_stack_overflow` kernel
- Call the `recursive_stack_stage` recursive function, triggering SIMT stack overflow
- Call `aclGetRecentErrMsg()` after `aclrtSynchronizeStream` on the host side to obtain stack overflow details from kernel execution

**Key Code**:

```cpp
__aicore__ float recursive_stack_stage(float value)
{
    if (value < 1.0f) {
        return 1.0f;
    }
    return recursive_stack_stage(value - 1) * value;
}

__global__ void simt_recursive_stack_overflow(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx > total_length) {
        return;
    }
    output[idx] = recursive_stack_stage(input[idx]);
}
```

**Result**: The compiler prints the stack size as 0, `_Z29simt_recursive_stack_overflowPfj_simt_entry: Stack size: 0 bytes, Used register number: 10`. Kernel execution fails, and `aclGetRecentErrMsg()` returns error information related to `The VEC SIMT stack overflows`.

**Data Analysis**:

- Because the compiler cannot directly verify the required stack space of a recursive function during compilation, the required stack space is printed as 0.
- Recursion fills the stack space at runtime. Therefore, recursion cannot be completely fixed by only increasing `simt_stack_size`; reduce recursion depth or rewrite it as an iterative implementation.

**Optimization Suggestion**:

> **Rewrite recursion as iteration**
>
> Rewrite recursion as an iterative calculation. If recursion must be used, ensure the call stack is not too deep and enough stack space is available.

---

### Case 4: Iterative Factorial Calculation Version

**Example Goal**: Resolve SIMT stack overflow caused by recursive calls by using an iterative method to calculate factorial.

**Core Implementation**:
- Use `aclInit(nullptr)` for default initialization, without loading a configuration file
- Launch the `simt_recursive_stack_overflow_optimized` kernel
- Call `recursive_stack_stage_optimized` to calculate factorial iteratively

**Key Code**:

```cpp
__aicore__ float recursive_stack_stage_optimized(int32_t value)
{
    float result = 1.0f;
    for (int32_t i = 2; i <= value; ++i) {
        result *= i;
    }
    return result;
}

__global__ void simt_recursive_stack_overflow_optimized(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx > total_length) {
        return;
    }
    output[idx] = recursive_stack_stage_optimized(input[idx]);
}
```

**Result**: Kernel execution succeeds, and the log prints `[Host] kernel completed.`.

**Data Analysis**:

- Case 4 shows that iteration can reduce stack usage. In real development, when recursion is used, prefer an iterative implementation.

---

### Stack Overflow Comparison Summary

For static variables defined in a kernel, use the `--cce-res-usage` compile option to obtain the stack size. For recursive functions, the compiler cannot provide the correct stack usage. On the host side, use `aclGetRecentErrMsg()` to obtain the latest runtime error details. If the log contains keywords related to `The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables`, the issue can be identified as a stack overflow scenario. If the size of local variables exceeds the stack size, consider moving local variables to `UB` space or increasing the SIMT stack size.

> **Note:** This example demonstrates the troubleshooting flow. In Case 1, the process still exits with code 0 after printing the kernel error. Use the `The VEC SIMT stack overflows` log message to determine whether the stack overflow is reproduced.

---

### Troubleshooting Steps

When a SIMT kernel stack overflow occurs, handle it as follows:

1. Reproduce the issue: run Case 1 or Case 3 first and confirm that the log contains `The VEC SIMT stack overflows`.
2. Confirm the fault location: find `fault kernel_name` in the log and check whether the faulting kernel is the target kernel.
3. Check stack usage: find `Stack size` in the build log. This value indicates the stack usage of one SIMT thread. If `Stack size` is 0, the stack usage may need to be determined at runtime and cannot be determined during compilation.
4. Optimize stack usage first: avoid defining large temporary variables, arrays, or intermediate buffers as thread-local stack variables. When `UB` can hold the data, prefer moving the buffer to `UB`.
5. Update the configuration file when large stack space is required: if larger stack space is still required, configure a larger `simt_stack_size` in `acl.json`. This example uses `2048`, which is 2048B.
6. Load the configuration file: call `aclInit("acl.json")` on the host side. If running from another directory, adjust the configuration file path based on the actual working directory.
7. Verify the fix: rebuild and run the corresponding configured-stack-size version.
  - Case 1 maps to Case 2. Confirm that the log prints `[Host] kernel completed.` and that the SIMT stack overflow error no longer appears.
  - For the recursion issue in Case 3, reduce recursive calls or rewrite the implementation as iteration. Case 4 is used to show that there is no stack overflow after changing to iteration.

> If the configured-stack-size version still fails, first check whether larger local arrays or recursive calls have been added to the kernel.

## Optimization Summary

| Scenario     | Root Cause                                         | Solution                                   |
|--------|----------------------------------------------|----------------------------------------|
| Stack overflow    | Large local arrays defined in the kernel (for example, `float buf[N]`)     | Prefer moving large temporary variables or intermediate buffers to UB, or configure a larger `simt_stack_size` through `acl.json` |
| Stack overflow    | Excessive recursion depth                                     | Reduce recursion depth, avoid recursion in kernels, and use iterative implementation                   |

> **Recommendation:** `simt_stack_size` is useful when larger stack space is truly required, but large arrays or large structs should not be kept on the stack long term. For large temporary variables or intermediate buffers, first evaluate whether they can be placed in UB to reduce stack usage.

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
  SCENARIO_NUM=1                                                    # Select the scenario. Valid values are 1-4.
  mkdir -p build && cd build                                        # Create and enter the build directory.
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
  make -j                                                           # Build the project.
  ./demo                                                            # Run the example.
  ```

  > **Note:** The `--cce-res-usage` compile option is enabled by default. During compilation, stack usage is printed. Case 1/Case 2 are array allocation scenarios and output `Stack size: 1280 bytes`. Case 3/Case 4 are recursive call scenarios, and the stack usage cannot be obtained during compilation. Use the runtime result for analysis.

- Build Options Description

  | Option                        | Values        | Description                                                |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example only supports dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM`            | `1`-`4` | Example type, default is 1. 1: large-array baseline version, 2: large-array configured-stack-size version, 3: recursive stack-overflow version, 4: iterative factorial version |

  Execution results:

  Case 1/Case 3 expected output:

  ```text
  ...
  The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
  ...
  ```

  Case 2/Case 4 expected output:

  ```text
  ...
  [Success] Case accuracy verification passed.
  ```
