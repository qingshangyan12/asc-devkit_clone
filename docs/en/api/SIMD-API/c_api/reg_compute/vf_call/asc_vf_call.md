# asc\_vf\_call

<!-- md-trans-meta sourceCommit=e430b2c74d537898700e8c0eb634eda72eb98460 translatedAt=2026-10-23T03:54:34.125966+00:00 pushedAt=2026-10-27T02:26:33.359Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"utils/base/helpers.h"`.

Used in SIMD C API programming scenarios. It is used to launch a Vector Function (VF) subtask in the Kernel. Reg vector computation C APIs can be called inside the VF function to complete register-level data movement and computation.

## Prototype

```cpp
template <auto funcPtr, typename... Args>
__aicore__ inline void asc_vf_call(Args &&...args)
```

## Parameters

**Table 1** Template parameters

| Parameter | Description |
| :-- | :------------ |
| funcPtr | Used to specify the Vector Function entrypoint function. |
| Args | Defines variable arguments used to pass actual arguments to the Vector Function entrypoint function. |

**Table 2** Parameters

| Parameter | Input/Output | Description |
| :-- | :------------ | :-- |
| args | Input | Variable arguments used to pass actual arguments to the Vector Function entrypoint function. |

## Return Value

None

## Constraints

- When `asc_vf_call` launches a Vector Function subtask, the subtask function cannot be a class member function. A regular function or a class static function is recommended, and the entrypoint function must use the `__simd_vf__` modifier macro.
- When `asc_vf_call` launches a Vector Function subtask, the input arguments support only Ascend C built-in data types and pointers, arrays, and structures composed of them. When passing source/destination addresses to Reg vector computation C APIs, the pointer type must point to Unified Buffer (UB) memory.
- The pipeline type of the Vector Function is `PIPE_V`. If UB address overlap or cross-pipeline dependencies exist inside the Vector Function, insert [asc_mem_bar](../reg_sync/asc_mem_bar.md) according to the constraints of the specific API.

## Examples

Use Reg vector computation C APIs to perform addition on UB data in the Vector Function.

```cpp
constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void add_vf(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1)
{
    vector_float dst_reg;
    vector_float src0_reg;
    vector_float src1_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);

    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}

__global__ __vector__ void asc_add_kernel(__gm__ float* dst, __gm__ float* src0, __gm__ float* src1)
{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src0_local[ELEMENT_COUNT];
    __ubuf__ float src1_local[ELEMENT_COUNT];

    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    asc_vf_call<add_vf>(dst_local, src0_local, src1_local);

    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}
```
