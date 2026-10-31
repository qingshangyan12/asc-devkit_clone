# asc_repeat_reduce_max_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:44:45.267Z pushedAt=2026-10-28T12:55:35.765Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Not supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
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

The header file path is `"c_api/composite/vector_compute_composite.h"`.

**The `asc_repeat_reduce_max_index_value_sync`, `asc_repeat_reduce_max_value_index_sync`, `asc_repeat_reduce_max_only_value_sync`, and `asc_repeat_reduce_max_only_index_sync` APIs are deprecated. Use [the corresponding non-synchronous API](../vector_compute/vector_reduce/asc_repeat_reduce_max.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Computes the maximum value of all elements in each Repeat, and the index of the first maximum element in the current Repeat. One Repeat computes 256 bytes.

index_value API: Indicates that index is in the lower half, and the return result storage order is [index, value]. When the input data type is half, the index data type is uint16; when the input data type is float, the index data type is uint32.
value_index API: Indicates that value is in the lower half, and the return result storage order is [value, index]. When the input data type is half, the index data type is uint16; when the input data type is float, the index data type is uint32.
only_value API: Indicates that only the maximum value is returned, and the return result storage order is [value].
only_index API: Indicates that only the index of the maximum value is returned, and the return result storage order is [index]. The index data type is fixed to uint32.

After this API is executed, you can continue to execute [asc_get_reduce_max_cnt](../vector_compute/vector_reduce/asc_get_reduce_max_cnt.md) to obtain the maximum values in all repeats and the index of the first maximum value.

## Prototype

- Synchronous computation

    ```cpp
    __aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
    __aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
    __aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
    __aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
    __aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
    __aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
    __aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
    __aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|----|-----|-----|
| dst | Output | Start address of the destination operand (vector). |
| src  | Input | Start address of the source operand (vector). |
| count | Input | Number of elements involved in the computation. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- The index data in the output is stored according to the data type of dst. For example, if the output is float, the index data should be uint32, so you need to reinterpret float as uint32 when reading.
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- The starting address of dst requires 4-byte alignment when the input type is half, and 8-byte alignment when the input type is float.
- The starting address of src requires 32-byte alignment.


## Examples

```cpp
// Example: Compute the maximum values and indices of 256 half elements in two iterations, producing two groups of value_index.
constexpr uint32_t total_length = 256;
__ubuf__ half src[total_length];
constexpr uint32_t dst_length = 4;
__ubuf__ half dst[dst_length];

// Use the high-dimensional split computation API, which requires setting the mask.
asc_set_mask_norm();
asc_set_vector_mask(0xffffffffffffffff, 0xffffffffffffffff);    // Set all elements in each iteration to participate in the computation.

uint8_t repeat = 2;
// The unit is sizeof(half)*2 = 4 bytes, stored contiguously.
uint16_t dst_repeat_stride = 1;
uint16_t src_block_stride = 1;
uint16_t src_repeat_stride = 8;
asc_repeat_reduce_max_value_index_sync(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
```

Result example:

```
Input data src: [1, 2, 3, ..., 128, 129, ..., 256]
Output data dst: [128, 7.6e-6, 256, 7.6e-6]
Output description: The binary representation of the half value 7.6e-6 is 0x007F, which, when reinterpreted as uint32, is the maximum value index 127.
```
