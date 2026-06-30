# SIMT Stack Overflow Scenario and Optimization Example

## Overview

This example uses stack overflow caused by defining large array variables in a kernel and by recursive function calls to describe how to locate and optimize stack overflow issues in Ascend C SIMT programming.

This example contains two cases:

- Case 1: Defines a large array variable and reproduces stack overflow with the default stack size. The issue is resolved by increasing the SIMT stack space through a configuration file.
- Case 2: Uses a recursive function to calculate factorial and reproduce stack overflow. The issue is avoided by calculating factorial with an iterative method.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Software Version

- \>= CANN 9.1.0

## Directory Structure

```text
├── stack_overflow
│   ├── CMakeLists.txt          // Example build script
│   ├── stack_overflow.asc      // SIMT stack overflow reproduction and optimization example
│   ├── acl.json                // aclInit configuration file used to configure the SIMT stack size
│   ├── README_en.md
│   └── README.md
```

## Example Description

This example contains four scenarios that reproduce or optimize SIMT stack overflow issues. The example selects the scenario to test through `SCENARIO_NUM`.

**Two cases**:

|  Case  | Comparison Dimension                                      | Scenarios        |
|:------:|-----------------------------------------------------------|------------------|
| Case 1 | Large local array: default stack size -> larger SIMT stack space configured | Scenario 1 <-> Scenario 2 |
| Case 2 | Recursive call: recursive factorial -> iterative factorial | Scenario 3 <-> Scenario 4 |

**Example specifications**:

<table>
<tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="3" align="center">SimtStackOverflow</td></tr>
<tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">input</td><td align="center">[2048]</td><td align="center">int32_t</td></tr>
<tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[2048]</td><td align="center">float</td></tr>
<tr><td rowspan="1" align="center">Kernel Name</td><td colspan="3" align="center">simt_stack_overflow / simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized</td></tr>
</table>

## Example Implementation

This section analyzes two types of SIMT stack overflow issues: large local arrays and recursive calls. Each case includes a reproduction scenario and an optimization scenario, and describes the core implementation, error locating method, and optimization suggestion.

| Case   | Implementation Feature                                  | Kernel Used                                                                  | Solution          |
|--------|---------------------------------------------------------|-------------------------------------------------------------------------------|-------------------|
| Case 1 | First reproduces stack overflow caused by a large array with the default configuration, and then resolves it by configuring a larger stack space | simt_stack_overflow | Increase the stack space through a configuration file |
| Case 2 | First reproduces stack overflow by using a recursive function to calculate factorial, and then calculates factorial iteratively | simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized | Calculate factorial iteratively |

> **Stack overflow issue description**
>
> The default stack space of SIMT threads is limited. When a large thread-local array is defined in a kernel function, or when the recursive call depth is too large, the required SIMT stack space keeps increasing. If the runtime stack requirement exceeds the configured upper limit, SIMT stack overflow is triggered.

---

### Case 1: Large Local Array Stack Overflow and Stack Size Configuration

**Example goal**: Reproduce SIMT stack overflow caused by defining a large array in a kernel, and verify the fix by configuring a larger SIMT stack space.

**Scenario configuration**:

| Scenario | Local Array                 | Stack Requirement per Thread | SIMT Stack Size Configuration | GM Usage              | Expected Result |
|:--------:|:----------------------------|:-----------------------------|:------------------------------|:----------------------|:----------------|
| 1        | `float local_buffer[320]`   | 1280B                        | Default, 1152B                | `1152B*2*72=165888B`  | Stack overflow, kernel execution fails |
| 2        | `float local_buffer[320]`   | 1280B                        | Configured, 1280B             | `1280B*2*72=184320B`  | Kernel executes normally |

**Core implementation**: The two scenarios in Case 1 use the same kernel. `simt_stack_overflow` allocates `float local_buffer[320]` on the stack of each thread. The array occupies `320 * 4B = 1280B` per thread. Scenario 1 uses default initialization and does not load a configuration file. Scenario 2 loads a custom stack space size. For the configuration method, see the "SIMT operator stack space size configuration example" section in the [aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit) API.

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

**Custom stack space size configuration**:

```cpp
aclInit("acl.json");
```

```json
{
    "StackSize": {
        "simt_stack_size": 1280,
        "simt_divergence_stack_size": 512
    }
}
```

**Obtaining failure logs**

- Add `aclGetRecentErrMsg()` after `aclrtSynchronizeStream` on the host side to obtain error logs, and confirm that `The VEC SIMT stack overflows` appears in the logs.
- Search for `fault kernel_name` in the logs to confirm whether the faulty function is the function being analyzed.
- Add `--cce-res-usage` to the compile options and search for `Stack size` in the build logs. `Stack size: xxx bytes` indicates the stack space required by kernel variables as calculated by the compiler. In this example, the value is `1280bytes`, which exceeds the default value `1152bytes`.

**Analysis**:

- Scenario 1 uses the default configuration. The default SIMT stack space is 1152B, which is less than the 1280B required by this kernel. Therefore, stack overflow is triggered during execution.
- Scenario 2 configures the stack space to 1280B, which meets the single-thread stack requirement of this kernel. Therefore, the kernel can execute normally.
- For static-size local variables, `Stack size` in the build logs can be used to confirm the single-thread stack space requirement. In this example, `simt_stack_overflow` requires 1280B of stack space.

**Conclusion**:

- When stack overflow occurs, optimize the kernel code first. Avoid keeping large arrays, large structs, and similar data on the thread stack for a long time. If temporary data is suitable for Unified Buffer, move it to UB to reduce stack usage.
- If larger thread stack space is truly required, configure the SIMT stack size through [aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit). This example configures the value to 1280, namely 1280B. Increasing the configured value increases GM usage, so evaluate it together with the service scale.

### Case 2: Recursive Stack Overflow and Iterative Optimization

**Example goal**: Reproduce SIMT stack overflow caused by recursive calls, and verify the fix with an iterative implementation.

**Scenario configuration**:

| Scenario | Implementation Method | Kernel                                      | Expected Result |
|:--------:|-----------------------|:--------------------------------------------|:----------------|
| 3        | Recursive factorial   | `simt_recursive_stack_overflow`             | Recursive calls continuously accumulate stack frames, and kernel execution fails |
| 4        | Iterative factorial   | `simt_recursive_stack_overflow_optimized`   | No recursive stack frame accumulation, and the kernel executes normally |

> Recursion cannot be resolved only by increasing the stack space. The recursion depth must be controlled or the implementation must be rewritten. Even if a larger stack space is configured, prefer an iterative implementation when the recursion depth is too large.

**Core implementation of the recursive version**:

```cpp
__aicore__ float recursive_stack_stage(int32_t value)
{
    if (value < 1.0f) {
        return 1.0f;
    }
    return recursive_stack_stage(value - 1) * value;
}

__global__ void simt_recursive_stack_overflow(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= total_length) {
        return;
    }
    output[idx] = recursive_stack_stage(input[idx]);
}
```

**Core implementation of the iterative version**:

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
    if (idx >= total_length) {
        return;
    }
    output[idx] = recursive_stack_stage_optimized(input[idx]);
}
```

**Obtaining failure logs**:

- Add `aclGetRecentErrMsg()` after `aclrtSynchronizeStream` on the host side to obtain error logs, and confirm that `The VEC SIMT stack overflows` appears in the logs.
- Search for `fault kernel_name` in the logs to confirm whether the faulty function is the function being analyzed.
- Add `--cce-res-usage` to the compile options. Because recursive calls are used, the required stack space cannot be calculated at compile time, and `Stack size` is `0`.

**Analysis**:

- Recursive calls continuously accumulate stack frames at runtime. A deeper recursion depth consumes more stack space.
- The actual stack requirement of a recursive function depends on the runtime call depth, so the compiler cannot reliably provide complete stack usage. Therefore, this type of issue cannot be determined only by `Stack size` in the build logs.
- Simply increasing the stack size can only postpone stack overflow caused by recursion. It cannot fundamentally resolve the issue of uncontrolled recursion depth. In actual development, prefer an iterative implementation or strictly control the recursion depth.

**Conclusion**

- When recursive calls cause stack overflow, prefer changing the recursive calculation to an iterative method to reduce stack usage.
- If recursive calls must be used, do not allow the recursion depth to be too large, and ensure that enough stack space is available.

## Optimization Suggestions

| Scenario | Root Cause | Optimization Method |
|----------|------------|---------------------|
| Large local array | A large thread-local array, such as `float buf[N]`, is defined in the kernel | Prefer moving it to UB or reducing the local array size. If a larger stack space is required, customize the SIMT stack size configuration |
| Recursive call | Excessive recursion depth causes stack frames to accumulate continuously | Prefer an iterative implementation. If recursion must be used, strictly control the recursion depth |

## Build and Run

Run the following steps in the root directory of this example to build and execute the example.

- Configure environment variables.

  Configure environment variables based on the [installation method](../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note**: `${install_path}` is the CANN package installation directory. If no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Build and run the example.

  ```bash
  SCENARIO_NUM=1                                # 选择执行场景，可选1-4
  mkdir -p build && cd build                    # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..;make -j # 编译工程
  ./demo                                        # 执行样例
  ```

  The valid values of `SCENARIO_NUM` are `1-4`. Scenarios 1 and 2 belong to Case 1, and scenarios 3 and 4 belong to Case 2. After changing the scenario, run `cmake` and `make` again to ensure that the compile macro takes effect.

**Build options**:

| Option                    | Valid Value | Description |
|---------------------------|-------------|-------------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510`  | NPU architecture. This example supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
| `SCENARIO_NUM`            | `1-4`       | Example scenario number. The default value is `1`. Scenarios 1 and 2 correspond to Case 1, and scenarios 3 and 4 correspond to Case 2 |

**Execution result example**:

Scenario 1 of Case 1 and scenario 3 of Case 2 are stack overflow reproduction scenarios. The following error information appears in the logs:

```text
The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
```

Scenario 2 of Case 1 and scenario 4 of Case 2 are optimization verification scenarios. After successful execution, the following information is displayed:

```text
test pass!
```
