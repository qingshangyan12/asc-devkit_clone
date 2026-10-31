# asc_storeunalign_postupdate (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:45:23.118Z pushedAt=2026-10-28T12:55:49.505Z nl=2 -->

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
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/reg_compute/store/storeunalign.h"`.

**The prototype of this API that does not specify a storage offset is deprecated. Use [asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md) to implement this function instead. The b8 data type prototype of the mask register eviction mode is deprecated. Use the b16 or b32 data type prototype instead.**

A reg compute data movement API, applicable to the scenario of continuously moving data from a vector data register or mask register with a non-32B-aligned start address to the Unified Buffer (UB).

Each call to this API updates the address of the destination operand in the UB.

## Prototype

- Without specifying a storage offset

    ```cpp
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ half* dst, vector_store_unalign& src0, vector_half src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ float* dst, vector_store_unalign& src0, vector_float src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1)
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1)
    ```

- Mask register eviction mode (b8 data type)

    ```cpp
    __simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_bool src1)
    ```

## Parameters

- Without specifying a storage offset

    **Table 1** Parameter description

    | Parameter | Input/Output | Description |
    | :----- | :------- | :------- |
    | dst | Output | Start address of the destination operand (vector). |
    | src0 | Input/Output | Non-aligned register, used to store non-aligned data, with a length of 32B. |
    | src1 | Input | Source operand (vector data register or mask register). |

- Mask register eviction mode (b8 data type)

    **Table 2** Parameter description

    | Parameter | Input/Output | Description |
    | :----- | :------- | :------- |
    | dst | Input/Output | Start address of the destination operand in UB, with the data type `uint8_t`. After the eviction, `dst` is automatically updated to the end address of this eviction. |
    | src0 | Input/Output | Non-aligned register used to store non-aligned data, with a length of 32B. |
    | src1 | Input | Source operand (mask register), with the type `vector_bool`. |

For details about vector data registers, non-aligned registers, and address registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- dst in this API does not need to be 32B-aligned.
- This API should be used together with [asc_storeunalign_post](asc_storeunalign_post_deprecated.md).
- The b8 data type prototype of the mask register eviction mode is deprecated. Use the b16 or b32 data type prototype instead.

## Examples

- No storage offset specified

    ```cpp
    // dst is the UB address, with a start address of 8, not 32B-aligned
    vector_store_unalign ureg;
    vector_uint32_t src;
    asc_storeunalign_postupdate(dst, ureg, src);
    asc_storeunalign_post(dst, ureg);
    ```
