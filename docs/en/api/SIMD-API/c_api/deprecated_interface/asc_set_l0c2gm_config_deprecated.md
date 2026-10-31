# asc_set_l0c2gm_config (Deprecated)
<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:33:40.495Z pushedAt=2026-10-28T12:55:49.477Z nl=2 -->

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

**The `asc_set_l0c2gm_config` API is deprecated. Use the [asc_set_l0c_copy_config](../cube_datamove/asc_set_l0c_copy_config.md) API instead.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

When in-line quantization is performed during data movement, call this API to set the vector quantization parameters in the quantization process.

## Prototype

```cpp
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag)
```

## Parameters

**Table 1** Parameter description

|Parameter|Input/Output|Description|
|------------|------------|-----------|
| relu_pre | Input     | Start address of the vector before the ReLU operation.|
| quant_pre | Input     | Start address of the vector before the quantization operation.|
| enable_unit_flag | Input     | Whether to enable unit_flag. unit_flag is a fine-grained parallelization between matrix computation instructions and matrix movement instructions. When this feature is enabled, the hardware moves out the computation result each time it finishes computing a fractal. This feature does not apply to L0C Buffer accumulation scenarios.|
## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

Quantization parameters cannot be inf/nan or denormalized numbers.

The following three parameter passing forms are supported:
- Set both relu_pre and quant_pre.
- Pass only relu_pre, with quant_pre set to 0.
- Pass only quant_pre, with relu_pre set to 0.

## Examples

```cpp
constexpr uint64_t relu_pre = 0;
constexpr uint64_t quant_pre = 0x1000;// Assume the effective address of the quantization operation is 0x1000.
asc_set_l0c2gm_config(relu_pre, quant_pre, true);
```
