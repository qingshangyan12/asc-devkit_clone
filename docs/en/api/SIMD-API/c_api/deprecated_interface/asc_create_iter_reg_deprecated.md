# asc_create_iter_reg (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:25:45.750Z pushedAt=2026-10-28T12:55:18.867Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_addr_reg.h"`.

**This API is deprecated. Use [asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md) instead.**

The address register is initialized through this API and then used inside a loop to store address offsets. This overloaded API supports 1 to 4 offset parameters and initializes registers with b8/b16/b32 bit widths, adapting to 1- to 4-dimensional tensors. The underlying implementation automatically computes address offsets in multi-dimensional loops, eliminating scalar address computation overhead.

## Prototype

  ```cpp
  __simd_callee__ inline iter_reg asc_create_iter_reg_b32(uint32_t offset)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b16(uint32_t offset)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b8(uint32_t offset)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3)
  __simd_callee__ inline iter_reg asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3)
  ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --------- | ----- | ----------------- |
| offset       | Input | Address offset. |
| offset0      | Input | Address offset. |
| offset1     | Input | Address offset. |
| offset2     | Input | Address offset. |
| offset3     | Input | Address offset. |

## Return Value

Returns the address register.

## Pipeline Type

PIPE_V

## Constraints

- Do not use if/else inside a loop.
- iter_reg must be assigned and used inside the innermost loop body.
- For operands with different strides, use multiple iter_reg instances to avoid address overwriting.

## Examples

See [address register example](../defs/type/data_type_definition.md#example4).
