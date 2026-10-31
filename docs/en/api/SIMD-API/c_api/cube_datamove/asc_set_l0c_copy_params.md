# asc_set_l0c_copy_params

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T13:26:42.547Z pushedAt=2026-10-28T12:45:45.478Z nl=2 -->

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

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

When DataCopy moves data from the L0C Buffer to Global Memory or the L1 Buffer and performs in-transit format conversion (converting the NZ format to the ND format), call this API to set the configuration related to the format conversion.

## Prototype

```cpp
__aicore__ inline void asc_set_l0c_copy_params(uint16_t nd_num, uint16_t src_nd_stride, uint16_t dst_nd_stride)
```


## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --------- | ----- | ---------------- |
| nd_num | Input | Number of ND cubes. Value range: nd_num ∈ [1, 65535]. |
| src_nd_stride | Input | Source stride in units of fractal size, which is the offset between the start addresses of adjacent NZ cubes in the source.<br> src_nd_stride ∈ [1, 512], unit: fractal_size 1024B. |
| dst_nd_stride | Input | Offset between the start addresses of adjacent ND cubes in the destination, in units of elements.<br> dst_nd_stride ∈ [1, 65535]. |




## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

None

## Examples

```cpp
uint16_t nd_num = 2;
uint16_t src_nd_stride = 2;
uint16_t dst_nd_stride = 1;
asc_set_l0c_copy_params(nd_num, src_nd_stride, dst_nd_stride);
```
