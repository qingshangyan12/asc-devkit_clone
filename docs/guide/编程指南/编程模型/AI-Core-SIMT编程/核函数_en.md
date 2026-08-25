# Kernel Function<a name="EN-US_TOPIC_0000002554438935"></a>

As described in [Overview](概述.md), a function that can be executed in parallel on the AI processor (NPU) and is called from the host is called a kernel function (Kernel). A kernel function is designed to be run in parallel by a large number of threads. Each thread has its own registers and stack, and the threads work together to complete massive data computation tasks.

## Kernel Function Definition<a name="section962384613331"></a>

Ascend C allows developers to define custom kernel functions to extend C++. When a kernel function is defined, specific function qualifiers enable the compiler to recognize that the function needs to be compiled on the device side and allow it to be called from the host side.

An example of a kernel function definition is as follows:

```cpp
// Example of a kernel function definition
__global__ void vec_add(float* x, float* y, float* z) 
{
    // Specific computation logic
}
```

The following rules must be observed when a kernel function is defined:

-   Use the function type qualifier \_\_global\_\_ to identify it as a kernel function.
-   A kernel function must have the void return type.
-   The parameter list of a kernel function must comply with the [restrictions on the function parameter list](../../../技术附录/CPP标准支持/语法限制/函数.md#section_kernel_function_argument_list_constraint).

## Kernel Function Call<a name="section33493239410"></a>

Functions in an operator program can be classified into three types: host-side execution functions, kernel functions (executed on the device side), and device-side execution functions (other than kernel functions). The following figure uses the kernel direct call operator development mode as an example to describe the call relationship among the three types of functions:

-   A host-side execution function can call other host-side execution functions, which is the function call in general C/C++ programming. It can also call a kernel function by using <<<...\>\>\>.
-   A kernel function can call device-side execution functions other than kernel functions.
-   A device-side execution function (other than a kernel function) is identified by the type qualifier \_\_aicore\_\_ and can call other device-side execution functions of the same kind.

**Figure 1**  Call relationship among kernel functions, host-side execution functions, and device-side execution functions<a name="fig14484835135913"></a>  
![](../../../figures/算子程序中三种函数间的调用关系.png "Call relationship among the three types of functions in an operator program")

On the host side, a kernel function is called by using the <<<...>>> kernel call operator, as shown below:

```cpp
kernel_name<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>(argument list)
```

The parameters in <<<...\>\>\> constitute the execution configuration of the kernel function, which consists of four parameters. For details about the usage, see [Kernel Function Configuration](../../语言扩展层/SIMT-BuiltIn关键字_en.md#section97005415463):

-   blocks\_per\_grid: a variable of the [dim3](../../语言扩展层/SIMT-BuiltIn关键字_en.md#section13165113520576) type that specifies the dimensions and scale of the grid. blocks\_per\_grid.x \* blocks\_per\_grid.y \* blocks\_per\_grid.z equals the total number of launched thread blocks, and the product must not be greater than 65535.
-   threads\_per\_block: a variable of the [dim3](../../语言扩展层/SIMT-BuiltIn关键字_en.md#section13165113520576) type that specifies the dimensions and scale of each thread block. threads\_per\_block.x \* threads\_per\_block.y \* threads\_per\_block.z equals the number of threads contained in each thread block, and the product must be less than or equal to the value configured by \_\_launch\_bounds\_\_.
-   <a name="li12421238101815"></a>dyn\_ubuf\_size: a variable of the size_t type that specifies the number of bytes of shared memory dynamically allocated to each thread block in this call, in addition to the statically allocated memory. The unit is bytes, and the default value is 0. For details about the usage, see the "Dynamic Allocation" method in [Shared Memory](内存层级_en.md#section66329146410).
-   stream: a pointer of the aclrtStream type that specifies the associated stream, which is used to maintain the execution sequence of asynchronous operations.

## Built-in Variables for Grid and Thread Indexes

In a kernel function, Ascend C provides [built-in variables](../../语言扩展层/SIMT-BuiltIn关键字_en.md#section_builtin_variables) to obtain the parameters of the execution configuration as well as the indexes of threads and thread blocks.

The commonly used built-in index variables are described as follows:
-   threadIdx: obtains the index of the current thread within the thread block to which it belongs. threadIdx.x, threadIdx.y, and threadIdx.z indicate the index of the current thread in each of the three dimensions. The value of threadIdx.x is in the range [0, blockDim.x), that of threadIdx.y is in the range [0, blockDim.y), and that of threadIdx.z is in the range [0, blockDim.z).
-   blockDim: obtains the three-dimensional hierarchy configured in the thread block, that is, the value of the dim3 structure instance configured when the kernel function is launched. blockDim.x, blockDim.y, and blockDim.z indicate the number of threads in each of the three dimensions of the thread block. It is specified in the execution configuration when the kernel function is launched and corresponds to the threads_per_block parameter in the kernel function configuration.
-   blockIdx: obtains the index of the current thread block within the grid to which it belongs, that is, the position coordinates of the thread block to which the current thread belongs in the entire grid.
-   gridDim: indicates how many thread blocks the entire computing task consists of in each dimension. It is specified in the execution configuration when the kernel function is launched and corresponds to the blocks_per_grid parameter in the kernel function configuration. The thread blocks in each dimension must satisfy gridDim.x * gridDim.y * gridDim.z <= 65535.

By combining the preceding variables, developers can calculate the globally unique data index (global index) of the current thread in the entire huge computing grid space. This enables tens of thousands of threads that execute the same code to accurately locate and process the specific data slice that belongs to each of them.

### Boundary Check

In actual one-dimensional parallel processing development, the total length of the data to be processed often cannot be exactly divided by the size of a single thread block. Therefore, extra physical threads usually need to be allocated, and a boundary check mechanism needs to be introduced inside the kernel function to prevent memory out-of-bounds access.

The standard formula for calculating a one-dimensional data index is as follows:

```cpp
int global_index = blockIdx.x * blockDim.x + threadIdx.x;
```

The following code snippet shows how to implement vector addition of a dynamic length by combining the index mechanism with a boundary check:

```cpp
// 1. Kernel implementation
__global__ void vec_add(float* x, float* y, float* z, int vector_length) 
{
    // Calculate the global element index corresponding to the current thread
    int global_index = blockIdx.x * blockDim.x + threadIdx.x;

    // Boundary check: Constrain the computation range to prevent excess threads from accessing out-of-bounds memory
    if (global_index < vector_length) 
    {
        // Only threads with indices within the valid range will execute the specific addition operation
        z[global_index] = x[global_index] + y[global_index];
    }
}

// 2. Host-side invocation example
int main() 
{
    // Assume memory allocation and initialization on both the Host and Device sides are complete
    int vector_length = 1000;
    
    // Empirical configuration: Specify that each block contains 256 threads
    int threads = 256; 
    
    // Calculate parameters: Calculate the required number of blocks based on the ceiling principle
    // Algorithm: (length + threads - 1) / threads
    int blocks = (vector_length + threads - 1) / threads; // The result is 4

    // Execute invocation: Launch 4 blocks, with 256 threads per block (a total of 1024 physical threads dispatched)
    vec_add<<<blocks, threads, 0, stream>>>(dev_x, dev_y, dev_z, vector_length);
    
    // ... Subsequent stream synchronization and resource release logic ...
    return 0;
}
```

In this example, the total number of target elements to be processed is 1000. Based on the strategy of 256 threads per block, the host side actually dispatches four thread blocks to the hardware, launching a total of $4 \times 256 = 1024$ physical threads. When the program runs to the last thread block, that is, when blockIdx.x is 3, the global_index values derived for the last 24 threads in the block (from index 1000 to 1023) fall in the range [1000, 1023]. At this point, the boundary check if (global_index < vector_length) causes these "extra" threads beyond the valid data range to end their tasks and exit safely. While ensuring the completeness of the computation result, this mechanism effectively avoids the risk of out-of-bounds memory access.
