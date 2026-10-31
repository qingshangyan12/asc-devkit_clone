# asc_get_l0c2gm_prequant

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T13:16:41.850Z pushedAt=2026-10-28T12:45:45.454Z nl=2 -->

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

When in-transit quantization is performed during data transfer, call this API to obtain the start address of the vector before the quantization operation.

## Prototype

```cpp
__aicore__ inline uint64_t asc_get_l0c2gm_prequant()
```

## Parameters

None

## Return Value

Returns the start address of the vector before the quantization operation.

## Pipeline Type

PIPE_S

## Constraints

None

## Examples

```cpp
uint64_t prequant_addr = asc_get_l0c2gm_prequant();
```
