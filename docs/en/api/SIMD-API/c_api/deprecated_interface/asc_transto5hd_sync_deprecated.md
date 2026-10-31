# asc_transto5hd_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:49:55.100Z pushedAt=2026-10-28T12:55:49.515Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
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

**The `asc_transto5hd_b8_sync`, `asc_transto5hd_b16_sync`, and `asc_transto5hd_b32_sync` APIs are deprecated. Use the [asc_transto5hd](../vector_compute/vector_permute_sel/asc_transto5hd.md) API and the [asc_sync](../sync/intra_core_sync/asc_sync.md) API instead.**

Performs data format conversion, typically converting the NCHW format to the NC1HWC0 format. In particular, it can also be used to transpose a two-dimensional matrix data block. This API can process 512 bytes of data (16 DataBlocks) within a single Repeat. Depending on the data type, it supports matrix transposition of different shapes (for example, when the data type is half, a single Repeat can complete the transposition of a 16*16 matrix). It also supports multiple Repeat operations.

The conversion rules within a single repeat are as follows:

- When the input data type bit width is 16 bits, each DataBlock contains 16 numbers. The instruction internal loops 16 times, and each iteration reads values from the corresponding positions of the specified 16 DataBlocks to form a new DataBlock unit and places it in the destination address. As shown in the following figure, srcList[0]-srcList[15] in the figure represent the 16 DataBlocks of the source operand. ![](../../../figures/asc_transto5hd_b16.png "Figure 1 Conversion rules when the input data type bit width is 16 bits")

- When the data type bit width is 32 bits, each DataBlock contains 8 numbers. The instruction internal loops 8 times, and each iteration reads values from the corresponding positions of the specified 16 DataBlocks to form 2 new DataBlocks and places them in the destination address. As shown in the following figure: ![](../../../figures/asc_transto5hd_b32.png "Figure 2 Conversion rules when the input data type bit width is 32 bits")

- When the data type bit width is 8 bits, each DataBlock contains 32 numbers. The instruction internal loops 16 times, and each iteration reads values from the corresponding positions of the specified 16 DataBlocks to form half a DataBlock and places it in the destination address. Whether the read and write occur in the high half or low half of the DataBlock is determined by the srcHighHalf and dstHighHalf parameters. As shown in the following figure: ![](../../../figures/asc_transto5hd_b8.png "Figure 3 Conversion rules when the input data type bit width is 8 bits")

Based on the conversion rules above, this API is used for NC1HWC0 format conversion or matrix transposition. NC1HWC0 format conversion is relatively complex, and its specific conversion method is described as follows:

When converting the NCHW format to the NC1HWC0 format, if the data type bit width is 32 bits or 16 bits, C0=16; if the data type bit width is 8 bits, C0=32. The following figure uses C0=16 as an example: ![](../../../figures/asc_transto5hd_NCHW2NC1HWC0.png)

## Prototype

- Synchronous computation.
```cpp
__aicore__ inline void asc_transto5hd_b32_sync(ub_addr8_t dst, ub_addr8_t src, uint8_t repeat,
    uint16_t dst_stride, uint16_t src_stride)
__aicore__ inline void asc_transto5hd_b16_sync(ub_addr8_t dst, ub_addr8_t src, uint8_t repeat,
    uint16_t dst_stride, uint16_t src_stride)
__aicore__ inline void asc_transto5hd_b8_sync(ub_addr8_t dst, ub_addr8_t src, uint8_t repeat,
    uint16_t dst_stride, uint16_t src_stride, bool dst_high_half, bool src_high_half)
```

## Parameters

**Table 1** Parameter description

|Parameter|Input/Output|Description|
| ------------ | ------------ | ------------ |
|dst|Output|Destination address register.|
|src|Input|Source address register.|
|repeat|Input|Number of repeated iterations, repeat∈[0,255].<br>Note:<br>&bull;When repeat is 1, the effective start position of the destination operand/source operand is the start position of the dst/src sequence input plus dst_stride/src_stride. To make the effective start position of the destination operand/source operand equal to the start position of the dst/src sequence input, set dst_stride/src_stride to 0.<br>&bull;When repeat is greater than 1, the effective start position of the destination operand/source operand in the first iteration is the start position of the dst/src sequence input, and dst_stride/src_stride is added in the second iteration, and so on.|
|dst_stride|Input|Address stride of the same DataBlock of the destination operand between adjacent iterations, in DataBlock.<br>For details about the address stride parameter of the same DataBlock between adjacent iterations, see [repeatStride](../general_description_and_constraints.md#repeatStride).|
|src_stride|Input|Address stride of the same DataBlock of the source operand between adjacent iterations, in DataBlock.<br>For details about the address stride parameter of the same DataBlock between adjacent iterations, see [repeatStride](../general_description_and_constraints.md#repeatStride).|
|dst_high_half|Input|Specifies whether the data at each dst address is stored in the high half or low half of the DataBlock. This configuration applies only to the int8_t/uint8_t data types.<br>The supported data type is bool, with the following two values:<br>&bull; true: stored in the high half of the DataBlock.<br>&bull; false: stored in the low half of the DataBlock.|
|src_high_half|Input|Specifies whether the data at each src address is stored in the high half or low half of the DataBlock. This configuration applies only to the int8_t/uint8_t data types.<br>The supported data type is bool, with the following two values:<br>&bull; true: stored in the high half of the DataBlock.<br>&bull;false: stored in the low half of the DataBlock.|

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For operand address alignment constraints, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- For operand address overlap constraints, see [General Address Overlap Constraints](../../general_description_and_constraints.md#general-address-overlap-constraints).
- When converting from the NCHW format to the NC1HWC0 format, the common practice is to configure each element in src/dst as the start point of each HW plane.
- For better performance, when the type bit width is 8 bits, fix dst_high_half/src_high_half first, then repeat in the HW direction, and then change dst_high_half/src_high_half.

## Examples

```cpp
// Example 1:
constexpr uint64_t total_length = 256;    // total_length is the data length involved in the computation.
__ubuf__ half src[total_length];
__ubuf__ half dst[total_length];
const uint32_t STEP = 16;
// src_list and dst_list are arrays of 16 DataBlock addresses.
__ubuf__ half* src_list[16] = {
    (__ubuf__ half*)(src + 0 * STEP),
    (__ubuf__ half*)(src + 1 * STEP),
    (__ubuf__ half*)(src + 2 * STEP),
    (__ubuf__ half*)(src + 3 * STEP),
    (__ubuf__ half*)(src + 4 * STEP),
    (__ubuf__ half*)(src + 5 * STEP),
    (__ubuf__ half*)(src + 6 * STEP),
    (__ubuf__ half*)(src + 7 * STEP),
    (__ubuf__ half*)(src + 8 * STEP),
    (__ubuf__ half*)(src + 9 * STEP),
    (__ubuf__ half*)(src + 10 * STEP),
    (__ubuf__ half*)(src + 11 * STEP),
    (__ubuf__ half*)(src + 12 * STEP),
    (__ubuf__ half*)(src + 13 * STEP),
    (__ubuf__ half*)(src + 14 * STEP),
    (__ubuf__ half*)(src + 15 * STEP)
};
__ubuf__ half* dst_list[16] = {
    (__ubuf__ half*)(dst + 0 * STEP),
    (__ubuf__ half*)(dst + 1 * STEP),
    (__ubuf__ half*)(dst + 2 * STEP),
    (__ubuf__ half*)(dst + 3 * STEP),
    (__ubuf__ half*)(dst + 4 * STEP),
    (__ubuf__ half*)(dst + 5 * STEP),
    (__ubuf__ half*)(dst + 6 * STEP),
    (__ubuf__ half*)(dst + 7 * STEP),
    (__ubuf__ half*)(dst + 8 * STEP),
    (__ubuf__ half*)(dst + 9 * STEP),
    (__ubuf__ half*)(dst + 10 * STEP),
    (__ubuf__ half*)(dst + 11 * STEP),
    (__ubuf__ half*)(dst + 12 * STEP),
    (__ubuf__ half*)(dst + 13 * STEP),
    (__ubuf__ half*)(dst + 14 * STEP),
    (__ubuf__ half*)(dst + 15 * STEP)
};
const int32_t VA_REG_ARRAY_LEN = 8;
uint8_t repeat = 1;
uint16_t dst_stride = 0;
uint16_t src_stride = 0;

// The asc_set_va_reg API requires the first eight and last eight address sequences to be associated with the address registers respectively.
asc_set_va_reg(VA0, dst_list);
asc_set_va_reg(VA1, dst_list + VA_REG_ARRAY_LEN);
asc_set_va_reg(VA2, src_list);
asc_set_va_reg(VA3, src_list + VA_REG_ARRAY_LEN);

asc_transto5hd_b16_sync(VA0, VA2, repeat, dst_stride, src_stride);

// Example 2:
constexpr uint64_t total_length = 2 * 32 * 16 * 16;    // total_length indicates the length of the data involved in the computation.
__ubuf__ half src[total_length];
__ubuf__ half dst[total_length];
const uint32_t DST_STEP = 16;
const uint32_t SRC_STEP = 16 * 16;
const uint32_t OFFSET = 16 * 16 * 16;
const int32_t VA_REG_ARRAY_LEN = 8;
uint8_t repeat = 16;
uint16_t dst_stride = 16;
uint16_t src_stride = 1;

// src_list and dst_list are defined outside the loop.
__ubuf__ half* src_list[16];
__ubuf__ half* dst_list[16];

for (int j = 0; j < 4; ++j) {
    // Update the address lists inside the loop.
    for (int i = 0; i < 16; ++i) {
        src_list[i] = (__ubuf__ half*)(src + OFFSET * j + i * SRC_STEP);
        dst_list[i] = (__ubuf__ half*)(dst + OFFSET * j + i * DST_STEP);
    }

    // Set the registers and perform the conversion in each iteration.
    asc_set_va_reg(VA0, dst_list);
    asc_set_va_reg(VA1, dst_list + VA_REG_ARRAY_LEN);
    asc_set_va_reg(VA2, src_list);
    asc_set_va_reg(VA3, src_list + VA_REG_ARRAY_LEN);

    asc_transto5hd_b16_sync(VA0, VA2, repeat, dst_stride, src_stride);
}
```
