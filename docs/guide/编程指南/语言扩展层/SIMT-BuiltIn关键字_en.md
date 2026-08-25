# SIMT BuiltIn Keywords<a name="EN-US_TOPIC_0000002477663934"></a>

## Function Execution Space Qualifiers<a name="section204112391232"></a>

A function execution space qualifier indicates whether a function is executed on the host side or the device side, as well as the space scope from which the function can be called.

**Table 1**  Overview of function execution space qualifiers

<a name="table121121062614"></a>
| Function Execution Space Qualifier | Execution Space (Host) | Execution Space (Device) | Space Allowed to Call the Function (Host) | Space Allowed to Call the Function (Device) |
| --- | --- | --- | --- | --- |
| \_\_host\_\_, no qualifier | √ | x | √ | x |
| \_\_aicore\_\_ | x | √ | x | √ |
| \_\_global\_\_ | x | √ | √ | x |

A function modified by \_\_global\_\_ is a kernel function entry and has the following usage constraints:

-   The function return type must be void, and the function cannot be a member function of a class, struct, or union.
-   Recursive calls are not supported.
-   Calls to a \_\_global\_\_ function are asynchronous. The call returns to the host thread on the host side immediately after it is issued.
-   The function can be called only by host-side functions and is executed on the device.

A function modified by \_\_aicore\_\_ can be executed only on the device side and can be called only by a \_\_global\_\_ function or another \_\_aicore\_\_ function.

A function modified by \_\_host\_\_ can be called and executed only on the host side.

## Memory Space Qualifiers<a name="section20787845172517"></a>

The memory space qualifier \_\_ubuf\_\_ is used to indicate the dynamic and static memory. The size of the static memory is determined at compile time, and the size of the dynamic memory is determined when the kernel function is executed.

-   The static memory is allocated through an array:

    ```cpp
    __ubuf__ half staticBuf[1024];
    ```

-   The dynamic memory is allocated and used in the following way:

    ```cpp
    extern __ubuf__ half dynamicBuf[]; 
    ```

    The actual size of the dynamic memory must be configured when the kernel function is launched. For details, see [Kernel Function Configuration](#section97005415463).

## Built-in Constants<a name="section784531219338"></a>

<a name="table878553753515"></a>
| Constant Name | Value | Function |
| --- | --- | --- |
| constexpr uint64_t ASC_UB_SIZE | The value is determined by the current AI processor. If this space does not exist on the AI processor, the value defaults to 0. | Indicates the capacity of the Unified Buffer (UB) under the current AI processor architecture. It can be used to obtain the UB resource size at compile time. |

## Built-in Structures<a name="section13165113520576"></a>

-   dim3<a name="li1136665405"></a>

    dim3 is a built-in structure type that consists of three unsigned integers. The structure is defined as follows:

    ```cpp
    struct dim3{
        unsigned int x, y, z;
        dim3(unsigned int x, unsigned int y = 1, unsigned int z = 1)
            :x(x), y(y), z(z) {}
    };
    ```

    It is used to specify the sizes of three different dimensions. The total number of the three dimensions is x \* y \* z. Developers can create a dim3 structure in the following ways.

    ```cpp
    dim3(x); // Creates a one-dimensional structure. dimy and dimz use the default value 1.
    dim3(x, y); // Creates a two-dimensional structure. dimz uses the default value 1.
    dim3(x, y, z); // Creates a three-dimensional structure.
    ```

## Built-in Variables<a name="section_builtin_variables"></a>


The following built-in variables of the dim3 structure are available only on the device:

-   gridDim<a name="li20760123812911"></a>

    <a id="griddim"></a>
    A built-in global variable that can be used only in a kernel function. It indicates how many thread blocks the entire computing task consists of in each dimension.

-   blockDim<a name="li076017381191"></a>

    <a id="blockdim"></a>
    A built-in global variable that can be used directly in a kernel function. It obtains the three-dimensional hierarchy of the threads configured in a thread block, that is, the value of the dim3 structure instance configured when the kernel function is launched. blockDim.x, blockDim.y, and blockDim.z indicate the number of threads in each of the three dimensions of the thread block.

-   blockIdx<a name="li1676053814914"></a>

    A built-in global variable that can be used only in a kernel function. It obtains the block index, that is, the position coordinates of the thread block to which the current thread belongs in the entire grid.

    -   The value of blockIdx.x is in the range [0, gridDim.x - 1].
    -   The value of blockIdx.y is in the range [0, gridDim.y - 1].
    -   The value of blockIdx.z is in the range [0, gridDim.z - 1].

-   threadIdx<a name="li7760123814919"></a>

    A built-in global variable that can be used directly in a kernel function. It obtains the index of the current thread within the thread block. threadIdx.x, threadIdx.y, and threadIdx.z indicate the index of the current thread in each of the three dimensions. The value of threadIdx.x is in the range \[0, blockDim.x\), that of threadIdx.y is in the range \[0, blockDim.y\), and that of threadIdx.z is in the range \[0, blockDim.z\). The mapping between the index of a thread within a thread block and the thread ID is as follows:

    -   For a one-dimensional thread block, the thread ID is blockIdx.x \* blockDim.x + threadIdx.x.

        ![](../../figures/线程ID图示-1.png)

    -   For a two-dimensional thread block, the thread ID is a two-dimensional structure, which is calculated as follows:

        ```cpp
        thread_id_x = blockIdx.x * blockDim.x + threadIdx.x;
        thread_id_y = blockIdx.y * blockDim.y + threadIdx.y;
        ```

    -   For a three-dimensional thread block, the thread ID is a three-dimensional structure, which is calculated as follows:

        ```cpp
        thread_id_x = blockIdx.x * blockDim.x + threadIdx.x;
        thread_id_y = blockIdx.y * blockDim.y + threadIdx.y;
        thread_id_z = blockIdx.z * blockDim.z + threadIdx.z;
        ```

The following built-in variable of the int type is available only on the device:

-   warpSize

    A variable of the const int32_t type that indicates the number of threads in a warp. Currently, the value is fixed at 32.

## Built-in Data Types<a name="section1835494915576"></a>

A series of data types applicable to the device side are provided, including scalars and short vectors. A short vector is a simple vector consisting of multiple elements.

**Table 2**  Scalar data types

<a name="table114921244133211"></a>
| Category | Data Type | Description | Size (bit) | Value Range |
| --- | --- | --- | --- | --- |
| Boolean type | bool | All zeros indicate false; otherwise, true. | 8 | true, false |
| Integer type | uint8_t | unsigned char | 8 | [0, 255] |
| Integer type | int8_t | signed char | 8 | [-128, 127] |
| Integer type | uint16_t | unsigned short | 16 | [0, 65535] |
| Integer type | int16_t | signed short | 16 | [-32768, 32767] |
| Integer type | uint32_t | unsigned int | 32 | [0, 4294967295] |
| Integer type | int32_t | signed int | 32 | [-2147483648, 2147483647] |
| Integer type | uint64_t | unsigned long | 64 | [0,18446744073709551615] |
| Integer type | int64_t | signed long | 64 | [-9223372036854775808, 9223372036854775807] |
| Floating-point type | float8_e4m3_t | 1 sign bit, 4 exponent bits, and 3 mantissa bits | 8 | [2<sup>6</sup> - 2<sup>9</sup>, 2<sup>9</sup> - 2<sup>6</sup>] |
| Floating-point type | float8_e5m2_t | 1 sign bit, 5 exponent bits, and 2 mantissa bits | 8 | [2<sup>13</sup> - 2<sup>16</sup>, 2<sup>16</sup> - 2<sup>13</sup>] |
| Floating-point type | hifloat8_t | 1 sign bit and 2 dot-field bits, with the exponent and mantissa bit widths determined by the dot-field encoding | 8 | The dot-field encoding determines the data precision and value range. |
| Floating-point type | half | 1 sign bit, 5 exponent bits, and 10 mantissa bits | 16 | [2<sup>5</sup> - 2<sup>16</sup>, 2<sup>16</sup> - 2<sup>5</sup>] |
| Floating-point type | bfloat16_t | 1 sign bit, 8 exponent bits, and 7 mantissa bits | 16 | [2<sup>120</sup> - 2<sup>128</sup>, 2<sup>128</sup> - 2<sup>120</sup>] |
| Floating-point type | float | 1 sign bit, 8 exponent bits, and 23 mantissa bits | 32 | [2<sup>104</sup> - 2<sup>128</sup>, 2<sup>128</sup> - 2<sup>104</sup>] |

Short vector data types are classified into Vector X2, Vector X3, and Vector X4, indicating that a short vector variable has 2, 3, or 4 elements. The currently supported types are distributed as follows:

<a name="table49986503175"></a>
| Element Data Type | Vector X2 | Vector X3 | Vector X4 |
| --- | --- | --- | --- |
| unsigned char | uchar2 | uchar3 | uchar4 |
| signed char | char2 | char3 | char4 |
| unsigned short (16bit) | ushort2 | ushort3 | ushort4 |
| signed short (16bit) | short2 | short3 | short4 |
| unsigned int | uint2 | uint3 | uint4 |
| signed int | int2 | int3 | int4 |
| Unsigned long integer (64bit) | ulonglong2 | ulonglong3 | ulonglong4 |
| Signed long integer (64bit) | longlong2 | longlong3 | longlong4 |
| Unsigned long integer (64bit) | ulong2 | ulong3 | ulong4 |
| Signed long integer (64bit) | long2 | long3 | long4 |
| Floating-point type: 1 sign bit, 2 exponent bits, and 1 mantissa bit | float4_e2m1x2_t | - | - |
| Floating-point type: 1 sign bit, 1 exponent bit, and 2 mantissa bits | float4_e1m2x2_t | - | - |
| Floating-point type: 1 sign bit, 4 exponent bits, and 3 mantissa bits | float8_e4m3x2_t | - | - |
| Floating-point type: 1 sign bit, 5 exponent bits, and 2 mantissa bits | float8_e5m2x2_t | - | - |
| Floating-point type hif8 | hifloat8x2_t | - | - |
| Floating-point type: 1 sign bit, 5 exponent bits, and 10 mantissa bits | half2 | - | - |
| Floating-point type: 1 sign bit, 8 exponent bits, and 7 mantissa bits | bfloat16x2_t | - | - |
| Floating-point type: 1 sign bit, 8 exponent bits, and 23 mantissa bits | float2 | float3 | float4 |

**Table 3**  Short vector data types

<a name="table13856144241"></a>
| Data Type | Memory Size (Bytes) | Address Alignment (Bytes) |
| --- | --- | --- |
| char2, uchar2 | 2 | 2 |
| char3, uchar3, char4, uchar4 | 4 | 4 |
| short2, ushort2 | 4 | 4 |
| short3, ushort3, short4, ushort4 | 8 | 8 |
| int2, uint2 | 8 | 8 |
| int3, uint3, int4, uint4 | 16 | 16 |
| long2, ulong2 | 16 | 16 |
| long3, ulong3, long4, ulong4 | 32 | 32 |
| longlong2, ulonglong2 | 16 | 16 |
| longlong3, ulonglong3, longlong4, ulonglong4 | 32 | 32 |
| float2 | 8 | 8 |
| float3, float4 | 16 | 16 |
| float4_e2m1x2_t, float4_e1m2x2_t | 1 | 1 |
| float8_e4m3x2_t, float8_e5m2x2_t, hifloat8x2_t | 2 | 2 |
| half2, bfloat16x2_t | 4 | 4 |

## Operators<a name="section186787252339"></a>

SIMT programming provides a series of operators for performing mathematical operations. The following is the list of supported operators.

**Table 4**  Operators supported by SIMT programming

<a name="table11745172843710"></a>
| Category | Operator | bool | int8_t/uint8_t/int16_t/uint16_t/int32_t/uint32_t/int64_t/uint64_t | half/bfloat16_t/float | half2/bfloat16x2_t | hifloat8_t |
| --- | --- | --- | --- | --- | --- | --- |
| Arithmetic operator | + | x | √ | √ | √ | x |
| Arithmetic operator | - | x | √ | √ | √ | x |
| Arithmetic operator | * | x | √ | √ | √ | x |
| Arithmetic operator | / | x | √ | √ | √ | x |
| Arithmetic operator | % | x | √ | x | x | x |
| Arithmetic operator | ++ | x | √ | √ | √ | x |
| Arithmetic operator | -- | x | √ | √ | √ | x |
| Arithmetic operator | - (negation) | x | √ | √ | √ | x |
| Comparison operator | &lt; | x | √ | √ | x | x |
| Comparison operator | &lt;= | x | √ | √ | x | x |
| Comparison operator | &gt; | x | √ | √ | x | x |
| Comparison operator | &gt;= | x | √ | √ | x | x |
| Comparison operator | == | x | √ | √ | x | x |
| Comparison operator | != | x | √ | √ | x | x |
| Bitwise operator | & | x | √ | x | x | x |
| Bitwise operator | \| | x | √ | x | x | x |
| Bitwise operator | ^ | x | √ | x | x | x |
| Bitwise operator | ~ | x | √ | x | x | x |
| Bitwise operator | &lt;&lt; | x | √ | x | x | x |
| Bitwise operator | &gt;&gt; | x | √ | x | x | x |
| Logical operator | && | √ | √ | √ | x | x |
| Logical operator | \|\| | √ | √ | √ | x | x |
| Logical operator | ! | √ | √ | √ | x | x |
| Conditional operator | a ? b : c | √ | √ | √ | √ | x |

> **Constraints**: The division operator (/) currently does not support subnormal inputs (all exponent bits are 0 and the mantissa bits are not all 0, representing an extremely small value close to zero). A subnormal input value is flushed to 0.

Examples of using the operators are as follows:

```cpp
// Addition
res[idx] = x[idx] + y[idx]; 

// Negation
x[idx] = (-x[idx]);

// Comparison
if (x[idx] > y[idx]) {
    res[idx] = x[idx];
} else {
    res[idx] = y[idx];
}

// Bitwise AND
res[idx] = x[idx] & y[idx];

// Logical OR
if (x[idx] || y[idx]) {
    res[idx] = 1;
}

// Conditional operation
res[idx] = x[idx] > y[idx] ? x[idx] : y[idx];
```

## Kernel Function Configuration<a name="section97005415463"></a>

When a function modified by the \_\_global\_\_ qualifier is called, the execution configuration must be specified. The execution configuration is specified by inserting an expression of the following form between the function name and the parenthesized argument list:

```cpp
<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>
```

Where:

-   blocks\_per\_grid: a variable of the dim3 type that specifies the dimensions and scale of the grid. blocks\_per\_grid.x \* blocks\_per\_grid.y \* blocks\_per\_grid.z equals the total number of launched thread blocks, and the product cannot exceed 65535.
-   threads\_per\_block: a variable of the dim3 type that specifies the dimensions and scale of each thread block. threads\_per\_block.x \* threads\_per\_block.y \* threads\_per\_block.z equals the number of threads contained in each thread block, and the product must be less than or equal to the value configured by \_\_launch\_bounds\_\_.
-   dyn\_ubuf\_size: a variable of the size\_t type that specifies the size of the shared memory dynamically allocated to each thread block, in bytes. This part of memory is used by arrays. For details about the usage, see the "Dynamic Allocation" method in [Shared Memory](../编程模型/AI-Core-SIMT编程/内存层级_en.md#section66329146410).
-   stream: a variable of the aclrtStream type that specifies the associated stream, which is used to maintain the execution sequence of asynchronous operations.

The following example shows how to declare and call a kernel function.

```cpp
// Declaration
__global__ void add_custom(float* x, float* y, float* z, uint64_t total_length);
// Call
uint32_t blocks_per_grid = 48; // Number of thread blocks (Grid size)
uint32_t threads_per_block = 256; // Number of threads per block (Block size)
size_t dyn_ubuf_size = 1024; // need 1024 Byte dynamic memory
add_custom<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>(x, y, z, 1024); // blocks_per_grid and threads_per_block are implicitly converted to the dim3 type
```

Before the function is executed, the preceding configuration parameters are verified. If blocks\_per\_grid or threads\_per\_block exceeds the maximum scale allowed by the device, or if dyn\_ubuf\_size exceeds the shared memory available after the static memory is allocated, the function fails to be executed.

The number of registers used by a kernel function significantly affects the number of resident warps. The number of registers used by a kernel function is specified by the \_\_launch\_bounds\_\_\(\) qualifier or the \_\_maxnreg\_\_\(\) qualifier.

Note the following constraints when using these two optional qualifiers:
-   \_\_launch\_bounds\_\_ or \_\_maxnreg\_\_ can be used only in a \_\_global\_\_ function.
-   \_\_launch\_bounds\_\_ and \_\_maxnreg\_\_ cannot be configured for the same function at the same time.

During concurrent execution of multiple threads, using fewer registers per thread allows more threads to reside on the AI processor. Therefore, the compiler uses a heuristic algorithm to minimize register spilling and the number of instructions while reducing register usage as much as possible. An application can use the \_\_launch\_bounds\_\_\(\) qualifier in the definition of a \_\_global\_\_ function to restrict the launch bounds and provide additional information that helps the compiler optimize this process. This configuration is optional.
-   \_\_launch\_bounds\_\_\(N\) <a name="li23861114618"></a>
  
    A function tag macro that can be optionally configured on a kernel function to specify the maximum number of threads with which the kernel function is launched. The maximum number of threads determines the number of registers that can be allocated to each thread. For details about the mapping, see the following table. Registers are used to store local variables in a thread. If the number of local variables exceeds the number of registers, problems such as stack overflow may occur. It is recommended that the maximum number of threads be consistent with the number of dim3 threads with which the kernel function is launched.
    
    **Table 5**  Mapping between \_\_launch\_bounds\_\_(N) and the number of registers available for each thread

    <a name="table1715318510594"></a>
    | N | Number of Registers Available for Each Thread |
    | --- | --- |
    | 1025~2048 | 16 |
    | 513~1024 | 32 |
    | 257~512 | 64 |
    | 1~256 | 127 |

    The following example shows how to configure the maximum number of threads of a SIMT function as 512, in which case the number of registers available for each thread is 64:

    ```cpp
    __global__ __launch_bounds__(512) inline void add(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
    ```

    The parameter N of \_\_launch\_bounds\_\_\(N\) must meet the following requirements:
    -   N \>= dimx \* dimy \* dimz; dimx, dimy, and dimz belong to the dim3 structure that represents the threads.
    -   The value of N ranges from 1 to 2048.
    -   If \_\_launch\_bounds\_\_ is not configured, the maximum number of threads defaults to 1024.

To support low-level performance tuning, an application can use the \_\_maxnreg\_\_\(\) qualifier in the definition of a \_\_global\_\_ function to pass the performance tuning intent to the compiler. This qualifier directly limits the maximum number of registers that can be allocated to a single thread within a thread block.

-   \_\_maxnreg\_\_\(N\) <a name="section_maxnreg"></a>

    A function tag macro that can be optionally configured on a kernel function to specify, at compile time, the maximum number of registers that can be allocated to a single thread within a thread block.

    The parameter N of \_\_maxnreg\_\_\(N\) must meet the following requirements:
    -   The value of N is an integer in the interval (0, 128].
    -   If the input value N is in the range (0, 16], each thread can use a maximum of 16 registers; if the input value N is in the range (16, 32], each thread can use a maximum of 32 registers; if the input value N is in the range (32, 64\], each thread can use a maximum of 64 registers; if the input value N is in the range (64, 128\], each thread can use a maximum of 128 registers.
    -   If \_\_maxnreg\_\_ is not configured, the maximum number of registers that can be allocated to a single thread defaults to 32.

    The maximum number of registers available for each thread limits the number of threads that can be actually launched for each block. For details about the mapping, see the following table.

    **Table 6**  Mapping between the maximum number of registers that can be allocated to each thread by \_\_maxnreg\_\_ and the number of threads that can be actually launched for each block

    | Maximum Number of Registers That Can Be Allocated to a Single Thread | Number of Threads That Can Be Actually Launched for Each Block |
    | --- | --- |
    | 16 | 1~2048 |
    | 32 | 1~1024 |
    | 64 | 1~512 |
    | 128 | 1~256 |

    The following example shows how to configure the maximum number of registers that can be allocated to a single thread of a SIMT function as 64:

    ```cpp
    __global__ __maxnreg__(64) void add(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
    ```
