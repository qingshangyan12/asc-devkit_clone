# SIMD&SIMT BuiltIn Keywords<a name="EN-US_TOPIC_0000002593949295"></a>

## Function Execution Space Qualifiers<a name="section1074418132518"></a>

The function execution space qualifiers used in hybrid SIMD and SIMT programming are the same as those used in SIMD programming. For details, see [Function Execution Space Qualifiers in SIMD BuiltIn Keywords](SIMD-BuiltIn关键字.md#section1074418132518).

## VF Function Execution Qualifiers

### \_\_simt\_vf\_\_<a name="section1009183746521"></a>

\_\_simt\_vf\_\_ is used to mark a SIMT VF entry function. The function has no return value. An example of the function definition is as follows:

```cpp
__simt_vf__ inline void function_name(
    __gm__ uint32_t* gm_param,
    __ubuf__ float* ubuf_param,
    uint64_t scalar_param, ...);
```

The key qualifiers in the SIMT VF function definition are described as follows:

| Qualifier | Function |
| --- | --- |
| \_\_simt_vf\_\_ | Function identifier that identifies a SIMT VF function. |
| inline | Recommends inlining. Whether the function is actually inlined is determined by the compiler. |
| \_\_gm\_\_ | Memory space qualifier that indicates that the memory space is the GM. |
| \_\_ubuf\_\_ | Memory space qualifier that indicates that the memory space is the UB. |

A SIMT VF function can be called only in a kernel function or an \_\_aicore\_\_ function. The calling API is [asc\_vf\_call](../../../api/SIMT-API/SIMD与SIMT混合编程简介/扩展语法/核函数配置-147_en.md#asc_vf_call-call). An example is as follows:

```cpp
asc_vf_call<function_name>(dim3(blockDim), arg1, arg2, ...);
```

A SIMT VF function has the following constraints:

-   The input parameters support only the Ascend C [built-in data types](SIMT-BuiltIn关键字_en.md#section1835494915576) (int32\_t, uint32\_t, float, half, and so on) as well as pointer, array, and structure types composed of them. A pointer type must point to the GM or UB memory.
-   The function return type must be void.
-   Only \_\_simt\_callee\_\_ functions or \_\_callee\_\_ functions can be called within a SIMT VF.

### \_\_simt\_callee\_\_

A \_\_simt\_callee\_\_ subfunction is an auxiliary function called inside a SIMT VF function. SIMT built-in variables can be used inside the function. An example of the definition is as follows:

```cpp
__simt_callee__ uint32_t simt_helper(__gm__ uint32_t* gm_param, __ubuf__ float* ubuf_param, uint64_t scalar_param, ...);
```

An example of the call is as follows:

```cpp
uint32_t result = simt_helper(arg1, arg2, ...);
```

This function has the following constraints:

-   The input parameters support only the Ascend C [built-in data types](SIMT-BuiltIn关键字_en.md#section1835494915576) (int32\_t, uint32\_t, float, half, and so on) and the corresponding pointer types.
-   The function return value can only be an Ascend C [built-in data type](SIMT-BuiltIn关键字_en.md#section1835494915576) (int32\_t, uint32\_t, float, half, and so on) or a corresponding pointer type.
-   Only \_\_simt\_callee\_\_ functions or \_\_callee\_\_ functions can be called within the function.

### \_\_simd\_vf\_\_ and \_\_simd\_callee\_\_

\_\_simd\_vf\_\_ is used to mark a SIMD VF entry function, and \_\_simd\_callee\_\_ is a subfunction called inside a SIMD VF. For details, see [SIMD VF Function Identifiers in SIMD BuiltIn Keywords](SIMD-BuiltIn关键字.md#section192521344610).

## Address Space Qualifiers<a name="section1624210295308"></a>

The address space qualifiers used in hybrid SIMD and SIMT programming are the same as those used in SIMD programming. For details, see [Address Space Qualifiers in SIMD BuiltIn Keywords](SIMD-BuiltIn关键字.md#section1624210295308).

## Kernel Function Configuration

### Kernel Function Definition

A kernel function is the device-side entry function of hybrid SIMD and SIMT programming. It coordinates the execution process of the entire operator, including the scheduling and calling of VFs. In a hybrid programming scenario on the vector computing unit, the syntax of the function definition is as follows:

```cpp
__global__ __vector__ void kernel_name(__gm__ type* param1, __gm__ type* param2, ...);
```

The key qualifiers are described as follows:

-   \_\_global\_\_: mandatory qualifier that identifies a kernel function, indicating that the function can be called on the host side by using <<<...\>\>\>.
-   \_\_vector\_\_: mandatory qualifier that indicates that the function is executed on the AIV core on the device side.

The following constraints apply to the definition of a kernel function:

-   The return value type must be void.
-   The input parameters support pointer types (which must be modified by \_\_gm\_\_) and Ascend C built-in data types.
-   A pointer parameter must point to a memory address in the global memory and must be modified by \_\_gm\_\_.

### \_\_launch\_bounds\_\_\(N\)

During concurrent execution of multiple threads, using fewer registers per thread allows more threads and thread blocks to reside on the AI processor, thereby improving performance. Therefore, the compiler uses a heuristic algorithm to minimize register spilling and the number of instructions while reducing register usage as much as possible. An application can use the \_\_launch\_bounds\_\_\(\) qualifier in the definition of a \_\_global\_\_ function to restrict the launch bounds and provide additional information that helps the compiler optimize this process. This configuration is optional.

\_\_launch\_bounds\_\_\(N\): a function tag macro that can be optionally configured on a SIMT VF entry function to specify, at compile time, the maximum number of threads with which the SIMT VF is launched. The value of the parameter N ranges from 1 to 2048. If \_\_launch\_bounds\_\_ is not configured, the maximum number of threads defaults to 1024.

The maximum number of threads determines the number of registers that can be allocated to each thread. For details about the mapping, see the following table. Registers are used to store local variables in a thread. If the number of local variables exceeds the number of registers, problems such as stack overflow may occur. It is recommended that the maximum number of threads be consistent with the number of dim3 threads with which the VF task is launched.

**Table 1**  Mapping between \_\_launch\_bounds\_\_(N) and the number of registers available for each thread

| N | Number of Registers Available for Each Thread |
| --- | --- |
| 1025~2048 | 16 |
| 513~1024 | 32 |
| 257~512 | 64 |
| 1~256 | 127 |

### <<<\>\>\> Call

Hybrid SIMD and SIMT programming introduces SIMT VF (Vector Function) subtasks based on the kernel function execution process of the SIMD programming model. The kernel function is still launched on the host side by using <<<...\>\>\> in the SIMD programming mode. The syntax is as follows:

```cpp
kernel_name<<<block_num, dyn_ub_size, stream>>>(args...);
```

The configuration parameters in the kernel call operator are described as follows:

| Parameter | Type | Description | Constraint |
| --- | --- | --- | --- |
| block_num | uint32_t | Number of cores enabled by the kernel function | Value range: [1, 65535] |
| dyn_ub_size | uint32_t | Size of the dynamic memory, in bytes | Must not exceed the maximum configurable value: 256 KB - 8 KB - 32 KB - static memory |
| stream | aclrtStream | Used to maintain the execution sequence of asynchronous operations | None |

## asc\_vf\_call Call of VF Functions

### asc\_vf\_call Call of a SIMT VF
In hybrid SIMD and SIMT programming, a SIMD kernel function serves as the device-side entry. A SIMT VF subtask is launched by using `asc_vf_call` in a kernel function or an `__aicore__` function. Through parameter configuration, a specified number of threads are launched to execute the specified SIMT VF function. The function prototype is as follows:

```cpp
template <auto funcPtr, typename... Args>
__aicore__ inline void asc_vf_call(dim3 threadNums, Args &&...args)
```

The template parameters are the specified SIMT VF function name and the SIMT VF function parameters, which are described as follows:

**Table 2**  Template parameters

| Parameter | Description |
| --- | --- |
| funcPtr | Specifies the SIMT entry kernel function. |
| Args | Defines variable parameters that are used to pass actual arguments to the SIMT entry kernel function. |

**Table 3**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| threadNums | Input | A dim3 structure defined as {dimx, dimy, dimz} that specifies the number of threads in a SIMT thread block. The total number of threads is dimx * dimy * dimz, which must be less than or equal to 2048 and is recommended to be a multiple of 32. |
| args | Input | Variable parameters that are used to pass actual arguments to the SIMT entry kernel function. |

The following example shows how to call a \_\_simt\_vf\_\_ function by using asc\_vf\_call in a hybrid SIMD and SIMT programming scenario.

```cpp
#include "simt_api/common_functions.h"
__simt_vf__ inline void add_simt(
    __gm__ float* dst, __gm__ float* src0, __gm__ float* src1)
{
    // SIMT code
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    dst[idx] = src0[idx] + src1[idx];
}

__global__ __vector__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    // asc_vf_call launches a SIMT VF subtask. The function name is add_simt, and blockDim is configured as dim3{1024, 1, 1}.
    asc_vf_call<add_simt>(dim3{1024, 1, 1}, z, x, y);
}
```

### asc\_vf\_call Call of a SIMD VF

Use the `asc_vf_call` API to call a SIMD VF entry function to launch a VF subtask. For details about the API, see
[asc\_vf\_call API Description](../../../api/SIMD-API/基础API/Reg矢量计算/VF调用/asc_vf_call.md).

### Differences Between SIMT VF and SIMD VF Calls

Both SIMT VFs and SIMD VFs launch VF subtasks in a kernel function or an `__aicore__` function through the `asc_vf_call` API, but they target different parallel execution models: a SIMT VF uses threads as the basic execution unit to implement thread-level parallelism, whereas a SIMD VF implements data-level parallelism on contiguous data based on vector registers. The difference in execution models further leads to differences in their calling forms, as shown in the following table:

| Comparison Item | SIMT VF Call | SIMD VF Call |
| --- | --- | --- |
| VF entry function identifier | Modified by __simt_vf__. | Modified by __simd_vf__. |
| Calling form | asc_vf_call<simt_func>(dim3(threadNums), arg1, arg2, ...) | asc_vf_call<simd_func>(arg1, arg2, ...) |
| First runtime parameter | A thread configuration of the dim3 type must be passed in to specify the number of threads in the SIMT thread block. | There is no thread configuration parameter. The call parameters start from the first actual argument of the SIMD VF function. |
| Pointer input parameters | Pointers to the GM or UB are supported and must be modified by __gm__ or __ubuf__ based on the actual memory space. | Pointer input parameters must be modified by the __ubuf__ address space qualifier and cannot directly access the GM memory. |
| Typical usage | Processing irregular segments such as complex control flows, discrete memory access, and thread-level index mapping. | Processing contiguous and regular vector computation segments. |

## Built-in Variables<a name="en-us_topic_0000002571575581_section13165113520576"></a>

The following built-in variables are available only within a SIMT VF:
-   gridDim<a name="en-us_topic_0000002571575581_li20760123812911"></a>

    A built-in global variable that can be used only in a SIMT VF function. It indicates how many thread blocks the entire computing task consists of in each dimension. gridDim.x <= 65535; gridDim.y and gridDim.z must be 1.

-   blockDim<a name="en-us_topic_0000002571575581_li076017381191"></a>

    A built-in global variable that can be used directly in a SIMT VF function. It obtains the three-dimensional hierarchy of the threads configured in a thread block. blockDim.x, blockDim.y, and blockDim.z indicate the number of threads in each of the three dimensions of the thread block. The number of threads in each dimension must satisfy blockDim.x \* blockDim.y \*blockDim.z  <= 2048.

-   blockIdx

    A built-in global variable that can be used only in a SIMT VF function. It obtains the block index, that is, the position coordinates of the thread block to which the current thread belongs in the entire grid. The value of blockIdx.x ranges from 0 to gridDim.x - 1, and blockIdx.y and blockIdx.z can currently return only 0.

-   threadIdx

    A built-in global variable that can be used only in a SIMT VF function. It obtains the index of the current thread within the thread block. threadIdx.x, threadIdx.y, and threadIdx.z indicate the index of the current thread in each of the three dimensions. The value of threadIdx.x is in the range \[0, blockDim.x\), that of threadIdx.y is in the range \[0, blockDim.y\), and that of threadIdx.z is in the range \[0, blockDim.z\). The index of a thread within a thread block is calculated as follows:

    -   For a one-dimensional thread block, the index of a thread within the thread block is threadIdx.x.
    -   For a two-dimensional thread block, the index of a thread within the thread block is \(threadIdx.x + threadIdx.y \* blockDim.x\).
    -   For a three-dimensional thread block, the index of a thread within the thread block is \(threadIdx.x + threadIdx.y \* blockDim.x + threadIdx.z \* blockDim.x \* blockDim.y\).

-   warpSize

    A runtime variable that indicates the number of threads in a warp. Currently, the value is fixed at 32.

For the built-in variables at the SIMD kernel function level, see [Built-in Variables at the SIMD Kernel Function Level](SIMD-BuiltIn关键字.md#section199434523343).

### Restrictions on the Use of Built-in Variables at Different Levels

The <<<...\>\>\> call on the host side configures the number of logical cores of the outer kernel function, whereas the thread-architecture built-in variables inside a SIMT VF describe the thread hierarchy of the SIMT VF subtask. The two categories of concepts belong to different levels and cannot be mixed. The following table lists the differences between common concepts at the SIMD kernel function level and at the SIMT VF thread level.

| Built-in Variable or Method | Level | Usage Position | Meaning |
| --- | --- | --- | --- |
| `AscendC::GetBlockNum()` | SIMD kernel function level | Kernel function or `__aicore__` function; invisible inside a SIMT VF | Obtains the number of logical cores of the kernel function configured by the <<<...\>\>\> call on the host side, that is, `block_num`. |
| `AscendC::GetBlockIdx()` | SIMD kernel function level | Kernel function or `__aicore__` function; invisible inside a SIMT VF | Obtains the logical core index of the core that is currently executing the kernel function. |
| `gridDim` | SIMT VF thread level | `__simt_vf__` or `__simt_callee__` function; invisible outside a SIMT VF | Obtains the grid dimensions in the SIMT VF thread hierarchy, indicating the number of thread blocks of this SIMT VF subtask in each dimension. Its value is determined by the number of logical cores of the kernel function configured by the <<<...\>\>\> call on the host side. |
| `blockIdx` | SIMT VF thread level | `__simt_vf__` or `__simt_callee__` function; invisible outside a SIMT VF | Obtains the thread block index in the SIMT VF thread hierarchy, indicating the position of the SIMT thread block to which the current thread belongs in the VF execution grid. |
| `blockDim` | SIMT VF thread level | `__simt_vf__` or `__simt_callee__` function; invisible outside a SIMT VF | Obtains the three-dimensional thread structure within a SIMT VF thread block. Its value is specified by the first `dim3` parameter of `asc_vf_call` when the SIMT VF is called. |
| `threadIdx` | SIMT VF thread level | `__simt_vf__` or `__simt_callee__` function; invisible outside a SIMT VF | Obtains the index of the current SIMT thread within the thread block. |

Note the following during use:

-   A SIMD kernel function cannot use the built-in variables related to the SIMT VF thread level, and a SIMT VF cannot perceive the logical core number or the number of logical cores at the SIMD kernel function level.
-   If a SIMT VF needs to use the logical core number or the number of logical cores in the outer SIMD process, it is recommended that the corresponding values be obtained through `AscendC::GetBlockIdx()` and `AscendC::GetBlockNum()` in the kernel function or the `__aicore__` function and then passed to the SIMT VF as common parameters.

## Built-in Data Types<a name="en-us_topic_0000002571575581_section1880403364916"></a><a name="us_topic_0000002571575581_section1880403364916"></a>

The built-in data types used in hybrid SIMD and SIMT programming are the same as those used in SIMT programming. For details, see [Built-in Data Types in SIMT BuiltIn Keywords](SIMT-BuiltIn关键字_en.md#section1835494915576).

## Operators<a name="en-us_topic_0000002571575581_section186787252339"></a>

The operators supported by hybrid SIMD and SIMT programming are the same as those supported by SIMT programming. For details, see [Operators in SIMT BuiltIn Keywords](SIMT-BuiltIn关键字_en.md#section186787252339).
