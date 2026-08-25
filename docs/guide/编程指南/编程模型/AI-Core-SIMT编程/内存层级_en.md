# Memory Hierarchy<a name="EN-US_TOPIC_0000002523359048"></a>

SIMT threads can access multiple memory spaces. The following table summarizes the scope and lifetime of the memory types commonly used in SIMT programming.

| Memory Type | Thread Scope | Lifetime | Physical Location |
|---------|----------|---------|---------|
| Global memory | Grid | Application | Device |
| Shared memory | Block | Kernel function | Vector Core |
| Register | Thread | Kernel function | Vector Core |

-   The global memory is the memory resource that all threads can directly access, that is, the Global Memory.
-   The shared memory is the memory shared by all threads within a thread block. It is located in the Unified Buffer, and its lifetime is the same as that of the thread block.
-   Each thread has its own registers, which are used to store local variables.
-   In terms of access performance, registers provide the highest access speed, and the access efficiency of the shared memory is higher than that of the global memory.

The memory hierarchy is shown in the following figure.  
<img src="../../../figures/内存.png" width="50%">

## Global Memory<a name="section8946131492119"></a>

The global memory on the device side is the memory space that all threads in the entire grid can access. The global memory is persistent: the space allocated in the global memory and the data stored in it are retained until the memory space is released or the application terminates. Before the kernel function is launched, you need to allocate and initialize the global memory by using the Runtime APIs. During the execution of the kernel function, each SIMT thread can read from and write to the global memory. After the execution is complete, the result can be copied back to the host. For more information and details about the Runtime APIs, see [Runtime API Reference](https://hiascend.com/document/redirect/CannCommunityRuntimeApi).

A kernel function running on the device side can directly access the global memory by using pointers. The following code shows a simple example of the global memory. The arrays x, y, and z are all stored in the global memory. The following kernel function implements the access to and storage in the global memory by each thread.

```cpp
__global__ void add_custom(float* x, float* y, float* z, uint64_t total_length)
{
    // Calculate global thread ID
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    // Maps to the row index of output tensor
    if (idx >= total_length) {
        return;
    }
    z[idx] = x[idx] + y[idx];
}
```

## Shared Memory<a name="section66329146410"></a>

The shared memory is the memory space that all threads within the same thread block can access. It is located in the Unified Buffer inside each Vector Core (AIV). Compared with the global memory, the shared memory has a smaller capacity but provides higher bandwidth and lower access latency. It can be regarded as a high-speed cache resource managed by the user during kernel execution.

You can allocate the shared memory dynamically or statically.

1.  Static allocation: A memory space of a specified size is allocated. The size is determined at compile time and cannot be modified dynamically. Developers allocate and use it through array allocation.

    ```cpp
    __global__ void add_custom(...)
    {
        __ubuf__ half static_buf[1024];
        ...
    }
    ```

    By default, the start address of the allocated static memory is aligned based on the data type. You can also use `__align__(N)` to manually specify the alignment.

2.  Dynamic allocation: You need to specify the size of the dynamic memory space by using the [dyn_ubuf_size](核函数_en.md#li12421238101815) parameter in <<<\>\>\>. The size is determined at runtime. In SIMT programming, the dynamic memory can be allocated and used in the following way.

    ```cpp
    // Device side: declare the dynamic shared memory
    __global__ void add_custom(...)
    {
        extern __ubuf__ char dynamic_buf[];
        ...
    }

    // Host side: specify the size of the dynamic shared memory by using the third parameter in <<<>>>
    int32_t main(int argc, char const *argv[])
    {
        ...
        uint32_t dyn_ubuf_size = 1024 * sizeof(char);
        add_custom<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>(...);
        ...
    }
    ```

### Restrictions on the Shared Memory Size

As shown in the following figure, the total size of the Unified Buffer memory space is 256 KB. In the default compilation mode, the UB is divided into four main areas by function. From the low address to the high address, they are the static memory, dynamic memory, reserved space, and Data Cache. Because these four areas share the same Unified Buffer memory, when configuring the shared memory size, you should ensure that sufficient space (at least 32 KB) is reserved for the Data Cache to avoid a verification error caused by insufficient available space.

![](../../../figures/UB内存分配.png)

The specific structure is as follows:

1.  The static memory and dynamic memory correspond to the memory allocated in the static and dynamic allocation modes.
2.  Reserved space: space reserved by the compiler and Ascend C. The size is fixed at 8 KB.
3.  Data Cache: the Data Cache space dedicated to SIMT, which is used to cache data when SIMT threads access the global memory. The configurable range of the Data Cache space is **32 KB at minimum and 128 KB at maximum**. The actual memory size is affected by the static and dynamic memory sizes configured by the user. The calculation formula is as follows:

    ```text
    Data Cache space size = min(total UB size (256 KB) - static memory - dynamic memory - reserved space (8 KB), 128 KB)
    ```

    The upper limit of the Data Cache space is **128 KB**. Even if a small amount of static and dynamic memory is allocated, the actual allocated Data Cache size does not exceed this upper limit.

Because the Data Cache, the reserved space, and the UB memory allocated by the user share the same UB, developers cannot use the entire UB space and should ensure that the actual access range does not exceed the allocated static and dynamic memory sizes.

The declaration or configuration of the static memory and dynamic memory defines the valid range in which the user can use the UB memory. If the configured memory size is insufficient, or if a UB address is accessed directly without any memory allocation, the accessed address may cross the boundary of the user's dynamic and static memory and fall into the Data Cache area. Because the Data Cache capacity is determined by subtracting the static memory, dynamic memory, and reserved space from the total UB size, a smaller shared memory configuration enlarges the remaining space that can be allocated to the Data Cache. When the program crosses the boundary of the allocated dynamic and static memory and an out-of-bounds write occurs, memory allocation failure may not be reported immediately, but the data cache for SIMT global memory access may be corrupted, resulting in unstable computation results or abnormal operator precision.

> [!NOTE]NOTE
>
> You can disable the reserved space by adding the [--cce-disable-vf-stack-reserved-ubuf](../../编译与运行/算子编译/AI-Core算子编译基本用法.md#ZH-CN_TOPIC_0000002462746461) option during compilation. After this option is enabled, the compiler no longer reserves this part of the UB space, and the space can be used as common UB space. The Data Cache space can be estimated by using the following formula:
> ```text
> Data Cache space size = min(total UB size (256 KB) - static memory - dynamic memory - reserved space (8 KB), 128 KB)
> ```
> When this option is used, the Data Cache space must still be no less than 32 KB, and the upper limit of the Data Cache space is still 128 KB. After this option is enabled, the compiler cannot use the reserved UB space as the buffer space for register spilling, so developers must ensure that register spilling does not occur.

## Register<a name="section1137715201010"></a>

In SIMT programming, each thread has its own registers, whose lifetime is the same as that of the kernel function. The use of registers is managed by the compiler, and the registers are used for local storage of a thread during the execution of the kernel function. The number of registers available during thread execution is related to the maximum number of threads configured when the kernel function is defined, as described in the following table.

**Table 1**  Mapping between the maximum number of threads and the number of registers available for each thread

| Maximum Number of Threads | Number of Registers Available for Each Thread |
| --- | --- |
| 1025~2048 | 16 |
| 513~1024 | 32 |
| 257~512 | 64 |
| 1~256 | 127 |

As shown in the preceding table, the larger the maximum number of threads configured when the kernel function is defined, the fewer registers each thread can use. If the number of threads set by the user is too large and the computation complexity of each thread is high, the compiler may temporarily store data in the stack space because there are not enough registers to store local variables. This easily leads to register spilling, which affects operator performance. Therefore, you should configure the maximum number of threads properly based on the actual operator complexity.

The maximum number of threads and the number of registers available during thread execution can be configured by using [\_\_launch\_bounds\_\_](../../语言扩展层/SIMT-BuiltIn关键字_en.md#li23861114618) or [\_\_maxnreg\_\_](../../语言扩展层/SIMT-BuiltIn关键字_en.md#section_maxnreg).
