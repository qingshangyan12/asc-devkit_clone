# Overview
<!-- md-trans-meta sourceCommit=10589b6e0347a8628628193466fcae403c924d16 translatedAt=2026-10-23T03:54:33.817072+00:00 pushedAt=2026-10-27T02:26:32.112Z nl=2 -->

The Reg vector computation C API provides register-level vector computation capabilities for the Reg vector computation architecture. Through this API, users can directly operate the registers involved in Vector computation on the chip, achieving greater flexibility and better performance. The Reg vector computation C API is similar in functionality to the Memory vector computation C API. However, unlike the Memory vector computation C API, whose input and output data are usually located in the Unified Buffer (UB), the input or output data of the Reg vector computation C API can use vector data registers `vector_<dtype>` (`dtype` is a placeholder representing data types supported by the C API, such as `half` and `float`) and mask registers `vector_bool`. For computation APIs, the function is to obtain data from given registers, perform computation, and save the result in given registers. For data movement APIs, the function is to implement data movement between the UB and registers. Therefore, compared with the Memory vector computation C API, the Reg vector computation C API hands the data movement and Reg computation processes to users for independent control, thereby achieving greater development freedom.

## How to Use the Reg Vector Computation API
The register-based programming model loads data from the UB into registers through Reg data movement instructions (Reg data load), performs complex mathematical computation, and then stores the data back to the UB through Reg data movement instructions (Reg data store). All computation logic is completed in registers, thereby reducing repeated reads and writes of intermediate data in the UB and greatly improving overall performance. The specific process is as follows:

**Figure 1** Reg vector computation

![](../../../figures/reg_vector_overview.png "Reg vector computation")

Taking [Reg Vector Computation Example](#reg-vector-computation-example) as an example, the complete Vector Function computation process consists of the following parts:
- Write and call Vector Functions. These functions are marked with `__simd_vf__` and can be called in kernel functions.
- Define vector data registers `vector_<dtype>` and mask registers `vector_bool`. For more Reg data types, see [Register Type Definition](../defs/type/data_type_definition.md).
- Write loops to process data of multiple VL lengths. The [asc_update_mask](./reg_mask/asc_update_mask.md) series APIs are used to update the mask involved in computation, and each loop iteration consumes elements of one VL length.
- In the loop, call the Reg data load API for contiguous aligned load ([asc_loadalign](./load/asc_loadalign.md)) to load data of a single VL length from the UB.
- In the loop, call the Reg computation API [asc_add](./reg_arith/asc_add.md) to complete a single Repeat computation.
- Inside the loop, call the Reg data store API for contiguous aligned store ([asc_storealign](./store/asc_storealign.md)) to store the computed data to the UB.

## Reg Vector Computation Example

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

    // Move GM data to UB addresses src0_local and src1_local.
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    // Call the vf function.
    asc_vf_call<add_vf>(dst_local, src0_local, src1_local);

    // Move data from UB address dst_local to GM.
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}
```

>[!NOTE]
>- Data movement between GM and UB must be performed through C API data movement APIs.
>- The pipeline type of a Vector Function is PIPE_V. If UB address overlap or cross-pipeline dependencies exist inside a Vector Function, insert [asc_mem_bar](./reg_sync/asc_mem_bar.md) according to the specific API constraints.

## General Constraints

- Reg Vector Computation C APIs that output results through reference parameters return directly when called on a non-AIV.
- Reg Vector Computation C APIs that output results through function return values return the default-constructed value of the corresponding vector type when called on a non-AIV.
- Reg Vector Computation C APIs must be called inside a Vector Function (a function marked with `__simd_vf__`). Direct calls in `__aicore__` functions are not supported. For the call relationship, see [SIMD BuiltIn Keywords](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md).
- For Reg Vector Computation C APIs that support the `mask` parameter, `mask` must be assigned in advance through the [mask setting API](./reg_mask/reg_mask.md) before being passed in. The content of an unassigned mask register is undefined, which may cause incorrect valid element positions.
