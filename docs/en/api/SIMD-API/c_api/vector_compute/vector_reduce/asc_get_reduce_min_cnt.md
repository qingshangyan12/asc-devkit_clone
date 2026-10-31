# asc_get_reduce_min_cnt

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:54:34.314712+00:00 pushedAt=2026-10-27T02:26:34.549Z nl=2 -->

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
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/vector_compute/compute/vector_reduce.h"`.

This API is used to obtain the minimum value after the [asc_repeat_reduce_min](asc_repeat_reduce_min.md) operation, as well as the index of the first minimum value.

## Prototype

```cpp
__aicore__ inline void asc_get_reduce_min_cnt(half& val, uint32_t& index)
__aicore__ inline void asc_get_reduce_min_cnt(float& val, uint32_t& index)
```

## Parameters

**Table 1** Parameters

| Parameter  | Input/Output | Description |
| :----- | :------- | :------- |
| val | Output | Minimum value. |
| index | Output | Index of the first minimum value. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- Must be used together with the [asc_repeat_reduce_min](asc_repeat_reduce_min.md) operation.
- A synchronization operation is required to ensure that [asc_repeat_reduce_min](asc_repeat_reduce_min.md) has completed before this API is called to obtain the result.

## Examples

```cpp
// Example: Compute the minimum value and index of 256 half values in 2 iterations, producing 2 groups of value_index.
constexpr uint32_t total_length = 256;
__ubuf__ half src[total_length];
constexpr uint32_t dst_length = 4;
__ubuf__ half dst[dst_length];

asc_repeat_reduce_min_value_index(dst, src, total_length);
asc_sync();

half minValue;
uint32_t minIndex;
asc_get_reduce_min_cnt(minValue, minIndex);
```

Result example:

```
Input data src: [256, 255, 254, ..., 129, 128, ..., 2, 1]
minValue: 1.0
minIndex: 255
```
