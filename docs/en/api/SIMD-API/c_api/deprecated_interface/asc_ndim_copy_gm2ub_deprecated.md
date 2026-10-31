# asc_ndim_copy_gm2ub (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:35:37.791Z pushedAt=2026-10-28T12:55:35.748Z nl=2 -->

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

**The following function prototype is deprecated. Use the `asc_load_l2_cache_mode` type enumeration to configure the L2 Cache management policy. For the new API, see [asc_ndim_copy_gm2ub](../vector_datamove/asc_ndim_copy_gm2ub.md)**

Multi-dimensional data movement API that moves data from Global Memory (GM) to Unified Buffer (UB). Compared with the basic data movement API, it allows more flexible configuration of the dimensions to be moved in.
This API can process up to five dimensions. Each dimension can be configured with an independent Stride, and it supports on-the-fly Padding. There are many related configuration parameters, which need to be passed through different APIs.

**Table 1** All parameters involved in Stride configuration and the APIs through which they are passed, where i indicates the i-th dimension and can be [0, 4]
| Parameter | Passing API | Description |
| :--- | :------- | :--- |
| loopi_size | This API | Indicates the number of elements processed within each dimension (does not include Padding elements).<br> The unit is the number of elements, and the value range is [1, 2^20-1]. |
| loopi_src_stride | [asc_set_ndim_loopi_stride](../vector_datamove/asc_set_ndim_loop_stride.md) | Indicates the interval between this source operand element and the next element within each dimension.<br> The unit is the number of elements, and the value range is [0, 2^40-1]. |
| loopi_dst_stride | [asc_set_ndim_loopi_stride](../vector_datamove/asc_set_ndim_loop_stride.md) | Indicates the interval between this destination operand element and the next element within each dimension.<br> The unit is the number of elements, and the value range is [0, 2^20-1]. |

**Table 2** All parameters involved in Padding configuration and the APIs through which they are passed, where i indicates the i-th dimension and can be [0, 4]
| Parameter | Passing API | Description |
| :--- | :------- | :--- |
| padding_mode | This API | Indicates the Padding value filling method, of the bool type.<br>&bull; true: Uses the constant filling method, that is, all Padding values are the fixed value pad_value.<br>&bull; false: Uses the most recent value filling method, that is, the left and right Padding values select the leftmost or rightmost value of the current dimension for filling. |
| pad_value | [asc_set_ndim_pad_value](../vector_datamove/asc_set_ndim_pad_value.md) | When padding_mode is true, this value is used as the fixed Padding value. When padding_mode is false, this value does not take effect. |
| loopi_lp_count | <br>&bull; Dimension 0: This API <br>&bull; Dimensions 1 to 4: [asc_set_ndim_pad_count](../vector_datamove/asc_set_ndim_pad_count.md) | Indicates the number of elements to be padded on the left within each dimension.<br> The unit is the number of elements, and the value range is [0, 255]. |
| loopi_rp_count | <br>&bull; Dimension 0: This API <br>&bull; Dimensions 1 to 4: [asc_set_ndim_pad_count](../vector_datamove/asc_set_ndim_pad_count.md) | Indicates the number of elements to be padded on the right within each dimension.<br> The unit is the number of elements, and the value range is [0, 255]. |

The following uses a 2D example to introduce several typical usage scenarios.

- 2D Padding scenario, using the fixed value filling method
![ ](../../../figures/asc_ndim_copy_gm2ub_2d_padding_constant.png)

- 2D Padding scenario, using the most recent value filling method
![ ](../../../figures/asc_ndim_copy_gm2ub_2d_padding_nearest.png)

- 2D Transpose scenario
![ ](../../../figures/asc_ndim_copy_gm2ub_2d_transpose.png)

- 2D Slice scenario
![ ](../../../figures/asc_ndim_copy_gm2ub_2d_slice.png)

- 2D Broadcast scenario
![ ](../../../figures/asc_ndim_copy_gm2ub_2d_broadcast.png)

## Prototype

```c
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
```

## Parameters

**Table 3** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Destination UB address. |
| src | Input | Source GM address. |
| loop0_size | Input | Indicates the number of elements processed in dimension 0 (does not include Padding elements).<br> Unit is number of elements, value range is [1, 2^20-1]. |
| loop1_size | Input | Indicates the number of elements processed in dimension 1 (does not include Padding elements).<br> Unit is number of elements, value range is [1, 2^20-1]. |
| loop2_size | Input | Indicates the number of elements processed in dimension 2 (does not include Padding elements).<br> Unit is number of elements, value range is [1, 2^20-1]. |
| loop3_size | Input | Indicates the number of elements processed in dimension 3 (does not include Padding elements).<br> Unit is number of elements, value range is [1, 2^20-1]. |
| loop4_size | Input | Indicates the number of elements processed in dimension 4 (does not include Padding elements).<br> Unit is number of elements, value range is [1, 2^20-1]. |
| loop0_lp_count | Input | Indicates the number of elements to be padded on the left side of dimension 0.<br> Unit is number of elements, value range is [0, 255]. |
| loop0_rp_count | Input | Indicates the number of elements to be padded on the right side of dimension 0.<br> Unit is number of elements, value range is [0, 255]. |
| padding_mode | Input | Indicates the Padding value filling method.<br>&bull; true: Uses the constant filling method, where all Padding values are the fixed value pad_value.<br>&bull; false: Uses the most recent value filling method, where the left and right Padding values select the leftmost or rightmost value of the current dimension for filling. |
| l2_cache_mode | Input | Enumeration value of the [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md) type, configuring the data management policy in the L2 Cache. |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- If the UB address of this instruction overlaps with that of other instructions, insert synchronization instructions [asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md) to serialize the instructions and prevent abnormal data.
- The address range width of all data that a single instruction can obtain must not exceed 40 bits (1 TB). That is, the i-dimension size of the source operand is (loopi_lp_size + loopi_size + loopi_rp_size - 1) * loop0_src_stride, and the i-dimension size of the destination operand is (loopi_lp_size + loopi_size + loopi_rp_size - 1) * loop0_dst_stride. The sum of the sizes of all dimensions must not exceed 2^40 bits.
- When loopi_dst_stride of each dimension is an ascending sequence, the address spaces of different loops must not interleave or overlap.

## Examples

2D Padding scenario, using the fixed-value filling method
![ ](../../../figures/asc_ndim_copy_gm2ub_2d_padding_constant.png)

```cpp
// total_length indicates the total length of the data to be moved. src is the half-type GM memory input from outside.
constexpr uint64_t total_length = 54;
__ubuf__ half dst[total_length];

uint32_t loop0_size = 3;
uint32_t loop1_size = 4;
uint32_t loop2_size = 1;
uint32_t loop3_size = 1;
uint32_t loop4_size = 1;
uint64_t loop0_src_stride = 1;
uint64_t loop1_src_stride = 3;
uint64_t loop2_src_stride = 0;
uint64_t loop3_src_stride = 0;
uint64_t loop4_src_stride = 0;
uint64_t loop0_dst_stride = 1;
uint64_t loop1_dst_stride = 6;
uint64_t loop2_dst_stride = 0;
uint64_t loop3_dst_stride = 0;
uint64_t loop4_dst_stride = 0;

bool padding_mode = true;
half pad_value = 88.0;
uint8_t loop0_lp_count = 2;
uint8_t loop1_lp_count = 2;
uint8_t loop0_rp_count = 1;
uint8_t loop1_rp_count = 3;

asc_set_ndim_loop0_stride(loop0_dst_stride, loop0_src_stride);
asc_set_ndim_loop1_stride(loop1_dst_stride, loop1_src_stride);
asc_set_ndim_loop2_stride(loop2_dst_stride, loop2_src_stride);
asc_set_ndim_loop3_stride(loop3_dst_stride, loop3_src_stride);
asc_set_ndim_loop4_stride(loop4_dst_stride, loop4_src_stride);

asc_set_ndim_pad_value(pad_value);

asc_ndim_pad_count_config pad_count_config;
pad_count_config.loop1_lp_count = loop1_lp_count;
pad_count_config.loop1_rp_count = loop1_rp_count;
asc_set_ndim_pad_count(pad_count_config);

asc_ndim_copy_gm2ub(dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
    loop0_lp_count, loop0_rp_count, padding_mode, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
```
