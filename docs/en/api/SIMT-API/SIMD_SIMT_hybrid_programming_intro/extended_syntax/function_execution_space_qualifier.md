# Function Execution Space Qualifier

<!-- md-trans-meta sourceCommit=57ffc0e98dc1ae7768d3eaadf217e1d94fdb6424 translatedAt=2026-10-22T07:35:10.330Z pushedAt=2026-10-22T08:00:16.411Z nl=2 -->

Mixed programming of SIMD and SIMT involves multiple types of functions that follow strict calling relationships and hierarchical constraints. This chapter introduces the overall function types and calling relationships, and details the definitions, calling syntax, and constraints of SIMT-related functions (\_\_simt\_vf\_\_ and \_\_simt\_callee\_\_). For other qualifiers, see the corresponding chapters.

The function types involved in SIMD and SIMT hybrid programming are shown in the following table:

| Function Execution Space Qualifier | Function Purpose | Calling Convention |
| --- | --- | --- |
| \_\_global\_\_ \_\_aicore\_\_ | Operator entry that coordinates VF execution. If only the SIMD and SIMT hybrid programming scenario executed on the AIV core is involved, __global__ __vector__ can be used to indicate that only the AIV core is launched. | Called from the Host side through <<<>>> |
| \_\_aicore\_\_ | Device-side helper function | Called by a kernel or a function at the same level |
| \_\_simt_vf\_\_ | Thread-level parallel computing task | Called through the asc_vf_call interface provided by SIMT |
| \_\_simd_vf\_\_ | Vector-level parallel computing task | Called through the asc_vf_call interface provided by SIMD |
| \_\_simt_callee\_\_ | Sub-function of a SIMT VF | Called inside the SIMT VF |
| \_\_simd_callee\_\_ | Sub-function of a SIMD VF | Called inside the SIMD VF |
| \_\_callee\_\_ | Device-side helper function executed at compile time | Can be called inside any Device-side function |

The function call relationships are shown in the following figure:

![](../../../figures/function_call_hierarchy_64.png)

## \_\_simt\_vf\_\_

\_\_simt\_vf\_\_ is used to mark the SIMT VF entry function, which has no return value. Use the asc\_vf\_call interface to call the SIMT VF entry function and start the VF subtask. The function can use the SIMT built-in variables: **threadIdx**, **blockIdx**, **blockDim**, and **gridDim**. The function definition example is as follows:

```cpp
__simt_vf__ inline void function_name(
    __gm__ uint32_t* gm_param,
    __ubuf__ float* ubuf_param,
    uint64_t scalar_param, ...);
```

The key modifiers in the SIMT VF function definition are described as follows:

| Modifier | Description |
| --- | --- |
| \_\_simt_vf\_\_ | Function identifier that identifies the SIMT VF function. |
| inline | Suggests inlining; whether inlining actually occurs is determined by the compiler. |
| \_\_gm\_\_ | Memory space modifier that identifies the memory space as GM. |
| \_\_ubuf\_\_ | Memory space modifier that identifies the memory space as UB. |

It is called through the SIMT [asc\_vf\_call](kernel_function_config.md#asc_vf_call) API in a kernel function (Kernel) or a `__aicore__` function. The calling example is as follows:

```cpp
asc_vf_call<function_name>(dim3(blockDim), arg1, arg2, ...);
```

The SIMT VF function has the following constraints:

-   Input parameters support only Ascend C [built-in data types](builtin_data_types.md) (int32\_t, uint32\_t, float, half, and so on) and pointer, array, and struct types composed of them. Pointer types must point to GM or Unified Buffer (UB) memory.
-   The function return type must be **void**.
-   Within a SIMT VF, only \_\_simt\_callee\_\_ functions or \_\_callee\_\_ functions can be called.

## \_\_simt\_callee\_\_

\_\_simt\_callee\_\_ sub-functions are helper functions called inside SIMT VF functions, and SIMT built-in variables can be used inside them. The following is an example of the definition:

```cpp
__simt_callee__ uint32_t simt_helper(__gm__ uint32_t* gm_param, __ubuf__ float* ubuf_param, uint64_t scalar_param, ...);
```

The following is an example of calling it:

```cpp
uint32_t result = simt_helper(arg1, arg2, ...);
```

This function has the following constraints:

-   Input parameters support only Ascend C [built-in data types](builtin_data_types.md) (int32\_t, uint32\_t, float, half, and so on) and their corresponding pointer types.
-   The function return value can only be an Ascend C [built-in data type](builtin_data_types.md) (int32\_t, uint32\_t, float, half, and so on) or its corresponding pointer type.
-   Only \_\_simt\_callee\_\_ functions or \_\_callee\_\_ functions can be called inside the function.
