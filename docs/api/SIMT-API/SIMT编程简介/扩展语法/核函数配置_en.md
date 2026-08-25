# Kernel Function Configuration

## Kernel Function Definition

A kernel function is the device-side entry function of SIMT programming. It coordinates the execution process of the entire operator. The syntax of the function definition is as follows:

```cpp
__global__ void kernel_name(uint32_t* param1, float* param2, ...);
```

The key qualifier is described as follows:

-   \_\_global\_\_: mandatory qualifier that identifies a kernel function, indicating that the function can be called on the host side by using <<<...\>\>\>.

The following constraints apply to the definition of a kernel function:

-   The return value type must be void.
-   The input parameters support basic data types and pointers to basic data types, such as int32\_t, float, int32\_t\*, and float\*.
-   A pointer parameter must point to a memory address in the global memory.

## \_\_launch\_bounds\_\_\(N\)

During concurrent execution of multiple threads, using fewer registers per thread allows more threads and thread blocks to reside on the AI processor, thereby improving performance. Therefore, the compiler uses a heuristic algorithm to minimize register spilling and the number of instructions while reducing register usage as much as possible. An application can use the \_\_launch\_bounds\_\_\(\) qualifier in the definition of a \_\_global\_\_ function to restrict the launch bounds and provide additional information that helps the compiler optimize this process. This configuration is optional.

\_\_launch\_bounds\_\_\(\): a function tag macro that can be optionally configured on a kernel function to specify, at compile time, the maximum number of threads with which the kernel function is launched. If \_\_launch\_bounds\_\_ is not configured, the maximum number of threads defaults to 1024. The parameter N must meet the following requirements:

-   N \>= blockDim.x \* blockDim.y \*blockDim.z
-   The value of N ranges from 1 to 2048.

    The maximum number of threads determines the number of registers that can be allocated to each thread. For details about the mapping, see the following table. Registers are used to store local variables in a thread. If the number of local variables exceeds the number of registers, problems such as register spilling may occur. It is recommended that the maximum number of threads be consistent with the number of threads configured when the kernel function is launched.

    **Table 1**  Mapping between \_\_launch\_bounds\_\_(N) and the number of registers available for each thread

    | N | Number of Registers Available for Each Thread |
    | --- | --- |
    | 1025~2048 | 16 |
    | 513~1024 | 32 |
    | 257~512 | 64 |
    | 1~256 | 127 |

## <<<\>\>\> Call

In SIMT programming, when a function modified by the \_\_global\_\_ qualifier is called by using <<<\>\>\>, the execution configuration must be specified in the following form:

```cpp
<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>
```

The execution configuration consists of four parameters. For details about the usage, see [Kernel Function Configuration](https://gitcode.com/cann/asc-devkit/blob/9.1.0/docs/guide/编程指南/语言扩展层/SIMT-BuiltIn关键字.md#核函数配置):

-   blocks\_per\_grid: a variable of the [dim3](内置数据类型-139_en.md#dim3) type that specifies the dimensions and scale of the grid. blocks\_per\_grid.x \* blocks\_per\_grid.y \* blocks\_per\_grid.z equals the total number of launched thread blocks, and the product cannot exceed 65535.
-   threads\_per\_block: a variable of the [dim3](内置数据类型-139_en.md#dim3) type that specifies the dimensions and scale of each thread block. threads\_per\_block.x \* threads\_per\_block.y \* threads\_per\_block.z equals the number of threads contained in each thread block, and the product must be less than or equal to the value configured by \_\_launch\_bounds\_\_.
-   dyn\_ubuf\_size: a variable of the size\_t type that specifies the size of the shared memory dynamically allocated to each thread block, in bytes. This part of memory is used by arrays. For details about the usage, see the "Dynamic Allocation" method in [Shared Memory](https://gitcode.com/cann/asc-devkit/blob/9.1.0/docs/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存).
-   stream: a pointer of the aclrtStream type that specifies the associated stream, which is used to maintain the execution sequence of asynchronous operations.

The following example shows how to declare and call a kernel function.

```cpp
// Declaration
__global__ void add_custom(float* x, float* y, float* z, uint64_t total_length);
// Call
add_custom<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>(x, y, z, 1024);
```
