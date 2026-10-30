# asc_set_gm2ub_pad (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:51:48.154Z pushedAt=2026-10-28T12:55:35.781Z nl=2 -->

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

The header file path is `"c_api/vector_datamove/vector_datamove.h"`.

**This API is deprecated. To set the padding data value when moving source data from GM to the Unified Buffer (UB) in a non-aligned manner, use [asc_set_copy_pad_val](../vector_datamove/asc_set_copy_pad_val.md).**

When moving data from Global Memory (GM) to the UB in a non-aligned manner, call this API to set the data values to be padded on the left and right sides of the consecutively moved data blocks.

## Prototype

```cpp 
__aicore__ inline void asc_set_gm2ub_pad(uint32_t pad_val)
``` 

## Parameters

**Table 1** Parameter description

|Parameter|Input/Output|Description| 
|------------|------------|-----------| 
| pad_val    | Input       | Data values to be padded on the left and right sides.|

## Return Value

None 

## Pipeline Type

PIPE_S 

## Constraints

None 

## Examples

```cpp 
asc_set_gm2ub_loop_size(2, 2); 
asc_set_gm2ub_loop1_stride(96, 128); 
asc_set_gm2ub_loop2_stride(192, 288); 
// The source operand is non-aligned and requires padding. 
asc_set_gm2ub_pad(0); 
asc_copy_gm2ub_align(dst, src, 2, 46 * sizeof(int8_t), 1, 1, true, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 48, 48); 
asc_set_gm2ub_loop_size(1, 1); 
```
