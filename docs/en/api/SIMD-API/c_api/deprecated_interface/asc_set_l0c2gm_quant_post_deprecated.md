# asc_set_l0c2gm_quant_post (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:35:33.312Z pushedAt=2026-10-28T12:55:49.486Z nl=2 -->

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

**The `asc_set_l0c2gm_quant_post` API is deprecated and has no replacement API.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Sets the value in the QUANT_POST register. QUANT_POST is a 64-bit register that stores the quantization information of the Fixpipe post-processing stage.
The meaning of each bit is as follows. For descriptions of the various quantization modes, see the descriptions of the quantization modes in [asc_copy_l0c2l1](../cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md):

**Table 1** QUANT_POST register bit description

| Bit range |                                                           Meaning                                                           |
| :-----------|:----------------------------------------------------------------------------------------------------------------------:|
| 4:0 |                                Indicated in the QS162S4_POST, QF162S4_POST, and SHIFT2S4_POST quantization modes as the offset in s5.                                |
| 8:0 | Indicated in the QS162B8_POST, QF162B8_POST, and SHIFT2B8_POST quantization modes as the offset in s9, or indicated in the QS162S16_POST, QF162S16_POST, and SHIFT2S16_POST quantization modes as the lower 9 bits of the s17 offset. |
| 9 |                             Indicated in the QS162B8_POST, QF162B8_POST, and SHIFT2B8_POST quantization modes to indicate whether the quantization result is signed.                              |
| 31:13 |         Indicated in the QS162B8_POST, QF162B8_POST, QS162S4_POST, QF162S4_POST, QS162S16_POST, and QF162S16_POST quantization modes as M3. It cannot be INF/NAN.          |
| 39:32 |                           Indicated in the QS162S16_POST, QF162S16_POST, SHIFT2S16_POST, and SHIFT2S32_POST quantization modes as the upper 8 bits of the s17 offset. |
| 44:40 | Indicated in the SHIFT2S4_POST, SHIFT2B8_POST, SHIFT2S16_POST, and SHIFT2S32_POST quantization modes as the shift value. |


## Prototype

```cpp
__aicore__ inline void asc_set_l0c2gm_quant_post(uint64_t config)
```

## Parameters

**Table 2** Parameter description

| Parameter | Input/Output | Description        |
| :------ | :---  |:----------|
|config   |Input   | Register value to be set. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

The quantization parameters cannot be inf/nan or denormal numbers.

## Examples
```cpp
const uint64_t MASK_8_0 = 0x1FF;
const uint64_t MASK_BIT9 = 0x200;
const uint64_t MASK_31_13 = 0xFFFFE000;

uint64_t field_8_0 = 0x1AB;
uint64_t bit_9 = 1;
uint64_t field_31_13 = 0x000EF;

uint64_t config = 0;

config = (config & ~MASK_8_0) | (field_8_0 & MASK_8_0);
config = (config & ~MASK_BIT9) | ((bit_9<<9) & MASK_BIT9);
config = (config & ~MASK_31_13) | (field_31_13 & MASK_31_13);
asc_set_l0c2gm_quant_post(config);
```
