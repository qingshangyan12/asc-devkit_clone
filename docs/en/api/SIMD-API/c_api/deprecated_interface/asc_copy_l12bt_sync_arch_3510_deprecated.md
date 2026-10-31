# asc_copy_l12bt_sync_arch_3510 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:54:00.674Z pushedAt=2026-10-28T12:55:18.828Z nl=2 -->

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

The header file path is `"c_api/composite/cube_datamove_composite.h"`.

**The `asc_copy_l12bt_sync` API is deprecated. Use the [corresponding non-synchronous API](../cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_3510.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Copies the Bias data of the MMAD instruction from the L1 Buffer to the BiasTable Buffer.

## Prototype

```c
// Placeholder form
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst,
                                           __cbuf__ <dtype>* src,
                                           uint32_t size)
```

### Data Types Supported by dtype

The data types supported by dtype are `half`, `bfloat16_t`, `int32_t`, and `float`.

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination operand. |
| src | Input | Start address of the source operand. |
| size| Input | Size of the data to copy (in bytes).|

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- When using the first n data copy APIs, the copied data size must be 32-byte aligned.

## Examples

```cpp
constexpr uint32_t size = 512;
__cbuf__ half src[256];
__biasbuf__ half dst[256];
uint64_t dst_addr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst)) / 16;
asc_copy_l12bt_sync(dst_addr, src, size);
```
