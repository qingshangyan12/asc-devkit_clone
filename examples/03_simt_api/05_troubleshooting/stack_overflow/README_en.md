# SIMT Stack Overflow Scenarios and Optimization Example

## Overview

This example uses SIMT stack overflow caused by defining large array variables in a kernel and by recursive function calls to describe how to locate and optimize stack overflow issues in Ascend C SIMT programming.

This example contains two cases:

- Case 1: A large array in stack space causes stack overflow. You can increase the stack space through a configuration file, but using UB to store a large amount of temporary data is recommended.
- Case 2: When recursive functions are used, an excessively large recursion depth causes stack overflow. You need to control the recursion depth or use an iterative algorithm instead of a recursive algorithm.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Software Version

- \>=CANN 9.1.0

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

This example contains two typical stack overflow scenarios:

**Case description**:

| Case   | Core Issue                               | Corresponding Kernel                                                        | Solution                                      |
|--------|------------------------------------------|-----------------------------------------------------------------------------|-----------------------------------------------|
| Case 1 | Storing excessively large data on the stack | simt_stack_overflow                                                          | Increase the stack size through a configuration file |
| Case 2 | Excessive recursive function call depth  | simt_recursive_stack_overflow/simt_recursive_stack_overflow_optimized        | Use an iterative algorithm instead of recursion |

> **Stack overflow issue description**
>
> The default stack space of SIMT threads is limited. When a large thread-local array is defined in a kernel function, or when the recursive call depth is too large, the required SIMT stack space keeps increasing. If the runtime stack requirement exceeds the configured upper limit, SIMT stack overflow is triggered. For details about stack space, see [Memory Hierarchy](../../../../docs/zh/guide/%E7%BC%96%E7%A8%8B%E6%8C%87%E5%8D%97/%E7%BC%96%E7%A8%8B%E6%A8%A1%E5%9E%8B/AI-Core-SIMT%E7%BC%96%E7%A8%8B/%E5%86%85%E5%AD%98%E5%B1%82%E7%BA%A7.md).

**Example specifications**:

<table>
<tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="3" align="center">SimtStackOverflow</td></tr>
<tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">input</td><td align="center">[2048]</td><td align="center">int32_t</td></tr>
<tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[2048]</td><td align="center">float</td></tr>
<tr><td rowspan="1" align="center">Kernel Name</td><td colspan="3" align="center">simt_stack_overflow/simt_recursive_stack_overflow/simt_recursive_stack_overflow_optimized</td></tr>
</table>

## Example Implementation

This section analyzes two types of SIMT stack overflow issues: large local array variables and recursive calls. Each case includes a stack overflow reproduction scenario and an optimized scenario, and provides the corresponding core implementation, error locating method, and optimization suggestions.

---

### Case 1: Large Local Array Stack Overflow and Stack Size Configuration

**Example goal**: Reproduce SIMT stack overflow caused by using a large array in stack space, and verify the fix by configuring a larger SIMT stack space.

**Scenario configuration**:

| Scenario | Large Local Array          | Stack Requirement per Thread | SIMT Stack Size Configuration | Expected Result |
|:--:|:--------------------------|:-----------------------------|:------------------------------|:----------------|
| 1  | `float local_buffer[320]` | 1280B                        | Default, 1152B                | Stack overflow, kernel execution fails |
| 2  | `float local_buffer[320]` | 1280B                        | Configured, 1280B             | Kernel executes normally |

**Core implementation**: Use the kernel function `simt_stack_overflow` to allocate the large array `float local_buffer[320]` on the stack of each thread. The array occupies `320 * 4B = 1280B` per thread.

```cpp
__global__ void simt_stack_overflow(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= total_length) {
        return;
    }
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

**Obtaining failure logs**:

- Accuracy verification fails after the preceding implementation is executed. In this example, the message is `[Failed] Case accuracy verification failed!`.
- If the issue is confirmed not to be caused by the calculation logic, obtain the failure logs.
- To obtain the failure logs, add `aclGetRecentErrMsg()` after `aclrtSynchronizeStream` on the host side. The following error log is obtained:

```text
The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
For details, see the troubleshooting document on the Ascend official website. Search for the keyword "AI Core Error".[FUNC:ProcessDavidStarsCoreErrorInfo][FILE:device_error_proc_c.cc][LINE:749]
       An error occurred in the kernel task, retCode=0x31, [vector core exception].[FUNC:PreCheckTaskErr][FILE:davinci_kernel_task.cc][LINE:1262]
       Vector Core kernel execution failed, retCode=0x31.[FUNC:GetError][FILE:stream.cc][LINE:1750]
       [AIC_INFO] after execute:args print end[FUNC:GetError][FILE:stream.cc][LINE:1750]
       [DFX_INFO]AI Core kernel execution failed, device_id=0, stream_id=61, report_stream_id=61, task_id=0, flip_num=0, fault kernel_name=_Z19simt_stack_overflowPiPfj, fault kernel info ext=_Z19simt_stack_overflowPiPfj, program id=0, hash=136201840501799437.[FUNC:GetError][FILE:stream.cc][LINE:1750]
       rtStreamSynchronize execution failed, reason=vector core exception[FUNC:FuncErrorReason][FILE:error_message_manage.cc][LINE:69]
       synchronize stream failed, runtime result = 507035[FUNC:ReportCallError][FILE:log_inner.cpp][LINE:148]
```

Find `fault kernel_name` in the logs. It is the function related to the current issue. In addition, check the keyword `errorStr:` and find `The VEC SIMT stack overflows` in the logs. This indicates that the issue is stack overflow.

- Add `--cce-res-usage` to the compile options to obtain the stack space usage, as shown below:

```text
[BISHENG] Function properties for _Z19simt_stack_overflowPiPfj_simt_entry: Stack size: 1280 bytes, Used register number: 16
```

`Stack size: 1280 bytes` in the log indicates that the kernel variables require `1280B` of stack space, which exceeds the default value `1152B`.

**Log analysis**:

- Based on the preceding error log, this is a SIMT stack overflow error. The compile information shows that the required stack space is 1280B, which exceeds the default SIMT stack space of 1152B. The code defines `float local_buffer[320]`, which occupies 1280B. This confirms that the large array definition on the stack causes stack overflow.
- If a large array variable is suitable for UB space, move it to UB to reduce stack usage and allow sharing between threads.
- If a large amount of temporary data must be stored in stack space, increase the SIMT stack space to meet the stack space requirement of the kernel and resolve the stack overflow issue.

**Solution**: Increase the SIMT stack space to meet the stack space required by the kernel. For details, see the "SIMT operator stack space size configuration example" section in the [aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit) API.

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

**Conclusion**:

- Reduce temporary variables: use address references instead of temporary variables where possible to reduce register usage.
- Optimize large arrays: when stack overflow occurs, optimize the kernel code first. Avoid keeping large arrays, large structs, and similar data on the thread stack for a long time. If temporary data is suitable for UB, move it to UB to reduce stack usage.
- Increase SIMT stack space: configuring the SIMT stack size takes effect for all operators. Increasing the configuration value also increases GM usage, so evaluate it together with the service scale.

> The default SIMT stack space is 1152B. In this example, the SIMT stack space is configured to 1280, namely 1280B. The number of threads is 1024, so the additional GM space occupied is `(1280B - 1152B) * 1024 = 128KB`.

### Case 2: Recursive Stack Overflow and Iterative Optimization

**Example goal**: Reproduce SIMT stack overflow caused by recursive calls, and verify the fix with an iterative implementation.

**Scenario configuration**:

| Scenario | Implementation Method | Kernel                                      | Expected Result |
|:--:|-----------------------|:--------------------------------------------|:----------------|
| 3  | Recursive factorial   | `simt_recursive_stack_overflow`             | Recursive calls continuously accumulate stack frames, and kernel execution fails |
| 4  | Iterative factorial   | `simt_recursive_stack_overflow_optimized`   | No recursive stack frame accumulation, and the kernel executes normally |

> Recursion cannot be resolved only by increasing the divergence stack space. The recursion depth must be controlled or the implementation must be rewritten. Even if a larger divergence stack space is configured, stack overflow still occurs when the recursion depth is too large. Therefore, prefer an iterative implementation.

**Core implementation**: Use a recursive algorithm to calculate factorial.

```cpp
__aicore__ float recursive_stack_stage(int32_t value)
{
    if (value < 1) {
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

**Obtaining failure logs**:

- Accuracy verification fails after the preceding implementation is executed. In this example, the message is `[Failed] Case accuracy verification failed!`.
- If the issue is confirmed not to be caused by the calculation logic, obtain the failure logs.
- To obtain the failure logs, add `aclGetRecentErrMsg()` after `aclrtSynchronizeStream` on the host side. The following error log is obtained:

```text
The extend info: errcode:(356) errorStr: VEC SIMT DVG stack overflows, which may be caused by too many conditional branches or too manay nested loops. subErrType: 0x4.
For details, see the troubleshooting document on the Ascend official website. Search for the keyword "AI Core Error".[FUNC:ProcessDavidStarsCoreErrorInfo][FILE:device_error_proc_c.cc][LINE:749]
       An error occurred in the kernel task, retCode=0x31, [vector core exception].[FUNC:PreCheckTaskErr][FILE:davinci_kernel_task.cc][LINE:1262]
       Vector Core kernel execution failed, retCode=0x31.[FUNC:GetError][FILE:stream.cc][LINE:1750]
       [AIC_INFO] after execute:args print end[FUNC:GetError][FILE:stream.cc][LINE:1750]
       [DFX_INFO]AI Core kernel execution failed, device_id=0, stream_id=61, report_stream_id=61, task_id=0, flip_num=0, fault kernel_name=_Z29simt_recursive_stack_overflowPiPfj, fault kernel info ext=_Z29simt_recursive_stack_overflowPiPfj, program id=0, hash=136201840501799437.[FUNC:GetError][FILE:stream.cc][LINE:1750]
       rtStreamSynchronize execution failed, reason=vector core exception[FUNC:FuncErrorReason][FILE:error_message_manage.cc][LINE:69]
       synchronize stream failed, runtime result = 507035[FUNC:ReportCallError][FILE:log_inner.cpp][LINE:148]
```

Find `fault kernel_name` in the logs. It is the function related to the current issue. In addition, check the keyword `errorStr:` and find `VEC SIMT DVG stack overflows` in the logs. This indicates that the issue is SIMT divergence stack overflow.

**Log analysis**:

- The preceding error indicates SIMT divergence stack overflow. The code contains a recursive function call. Therefore, recursive calls continuously accumulate stack frames. A deeper recursion depth consumes more divergence stack space and finally causes divergence stack overflow.
- Simply increasing the divergence stack space can only postpone SIMT divergence stack overflow. It cannot fundamentally resolve the issue of uncontrolled recursion depth.
- For SIMT divergence stack overflow scenarios, prefer an iterative implementation or strictly control the recursion depth.

**Solution**: Change the recursive algorithm to an iterative algorithm for factorial calculation.

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

**Conclusion**:

- Optimize recursive algorithms: when recursive calls cause stack overflow, prefer changing recursion to an iterative method to reduce divergence stack usage.
- Limit recursion depth: set the maximum recursion depth and ensure that the recursive function has a clear termination condition.
- Increase SIMT divergence stack space: if recursive calls must be used, ensure that sufficient divergence stack space is available. You can configure the SIMT divergence stack space through [aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit).

## Optimization Suggestions

| Scenario | Root Cause | Optimization Method |
|----------|------------|---------------------|
| Large local array | A large thread-local array, such as `float buf[N]`, is defined in the kernel | Prefer moving it to UB or reducing the local array size. If a larger stack space is required, customize the SIMT stack size configuration |
| Recursive call | Excessive recursion depth causes stack frames to accumulate continuously | Prefer an iterative implementation. If recursion must be used, strictly control the recursion depth. Increasing the SIMT operator divergence stack size is not recommended |
| Excessive temporary variables | Too many temporary variables are defined | Reduce temporary variables. Use address references instead of temporary variables where possible to reduce register usage |
| Excessive branches | The function call hierarchy is too deep | Optimize branch processing logic and reduce branch jumps within a warp |
| Complex struct | A complex struct is passed in | For complex structs, plan UB or GM memory to store struct data and avoid passing excessive amounts of data |

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
  SCENARIO_NUM=1                       # Select the execution scenario, options 1-4
  mkdir -p build && cd build;          # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # Build the project
  ./demo                               # Run the example
  ```

**Build options**:

| Option                    | Valid Value | Description |
|---------------------------|-------------|-------------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510`  | NPU architecture. This example supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
| `SCENARIO_NUM`            | `1-4`       | Example scenario number. The default value is `1`. 1: overflow scenario of Case 1; 2: optimized scenario of Case 1; 3: overflow scenario of Case 2; 4: optimized scenario of Case 2 |

**Execution result example**:

For the stack overflow scenario of Case 1, the following error information appears in the logs:

```text
The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
```

For the stack overflow scenario of Case 2, the following error information appears in the logs:

```text
The extend info: errcode:(356) errorStr: VEC SIMT DVG stack overflows, which may be caused by too many conditional branches or too manay nested loops. subErrType: 0x4.
```

For the optimization verification scenarios of Case 1 and Case 2, the following information is displayed after successful execution:

```text
test pass!
```
