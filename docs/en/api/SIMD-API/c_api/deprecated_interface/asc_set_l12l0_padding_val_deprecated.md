# asc_set_l12l0_padding_val (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:36:51.286Z pushedAt=2026-10-28T12:55:49.487Z nl=2 -->

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

**The `asc_set_l12l0_padding_val` API is deprecated. Use the [asc_set_l12l0b_3d_padding](../cube_datamove/asc_set_l12l0b_3d_padding.md) API instead.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Sets the value in the PADDING_B register, a 64-bit register that stores the padding value used during data movement.
The meaning of each bit is as follows:

**Table 1** PADDING_B register bit description

| Bit range | Meaning |
| :-----------|:------------------------------------------------------------------------------------------------------------------------------------------------------:|
| 31:0 | Stores the value used for padding.<br>When the data bit width is 32, bits 31:0 are used directly.<br>When the data bit width is 16, bits 15:0 are used and bits 31:16 are ignored.<br>When the data bit width is 8, bits 15:8 must be the same as bits 7:0, and bits 31:16 are ignored. When the data bit width is 4, bits 15:12, 11:8, 7:4, and 3:0 must be the same, and bits 31:16 are ignored. |
| 33:32 | Indicates the padding mode used. Three modes are currently supported.<br>2'b00: fixed padding.<br>2'b01: per-channel padding.<br>2'b10/2'b11: currently reserved. |

## Prototype

```cpp
__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config)
```

## Parameters

**Table 2** Parameter description

| Parameter | Input/Output | Description |
| :------ | :---  |:----------|
|config   |Input   | Register value to be set. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

None

## Examples
```cpp
uint64_t config = 0;
asc_set_l12l0_padding_val(config);
```
