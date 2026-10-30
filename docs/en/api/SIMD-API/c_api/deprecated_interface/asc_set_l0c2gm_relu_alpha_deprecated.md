# asc_set_l0c2gm_relu_alpha (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:35:46.195Z pushedAt=2026-10-28T12:55:49.484Z nl=2 -->

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

**The `asc_set_l0c2gm_relu_alpha` API is deprecated. Use the [asc_set_l0c_copy_relu_alpha](../cube_datamove/asc_set_l0c_copy_relu_alpha.md) API instead.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Sets the value in the RELU_ALPHA register. This is a 64-bit register that stores the alpha value used for Scalar ReLU in fixpipe or cube instructions.

The meaning of each bit is as follows:

**Table 1** RELU_ALPHA register bit description

| Bit range |                                            Meaning                                           |
|:------|:--------------------------------------------------------------------------------------:|
| 31:13 | Indicates the M2 value of Scalar ReLU in ReLU_PRE, which takes effect only for Scalar ReLU. The hardware uses the format of 1 sign bit, 8 exponent bits, and 10 mantissa bits for computation. The value cannot be INF/NAN.  |
| 63:45 | Indicates the M2 value of Scalar ReLU in ReLU_POST, which takes effect only for Scalar ReLU. The hardware uses the format of 1 sign bit, 8 exponent bits, and 10 mantissa bits for computation. The value cannot be INF/NAN. |

## Prototype

```cpp
__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config)
```

## Parameters

**Table 2** Parameter description

|Parameter|Input/Output| Description        |
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
uint32_t pre_m2 = (0<<18)|(127<<10)|0;
uint32_t post_m2 = (0<<18)|(124<<10)|0;
uint64_t config = ((uint64_t)pre_m2<<13)|((uint64_t)post_m2<<45);
asc_set_l0c2gm_relu_alpha(config);
```
