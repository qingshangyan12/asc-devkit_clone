# asc_storealign_postupdate

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

从矢量数据寄存器或掩码寄存器连续对齐搬出到UB的接口，实现NORM搬出模式并启用Post Update，接口调用后自动更新目的操作数地址。支持数据类型为b8、b16、b32。

## 函数原型

- 从矢量数据寄存器连续对齐搬出到UB的场景
  ```cpp
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int8_t*& dst_align32b, vector_int8_t src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t*& dst_align32b, vector_uint8_t src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int16_t*& dst_align32b, vector_int16_t src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint16_t*& dst_align32b, vector_uint16_t src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ half*& dst_align32b, vector_half src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ bfloat16_t*& dst_align32b, vector_bfloat16_t src, int32_t offset, vector_bool mask)
  ```
- 从掩码寄存器连续对齐搬出到UB的场景
  ```cpp
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint32_t*& dst, vector_bool src, int32_t offset)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint16_t*& dst, vector_bool src, int32_t offset)
  __simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t*& dst, vector_bool src, int32_t offset)
  ```

## 参数说明

- 从矢量数据寄存器连续对齐搬出到UB的场景

  **表 1**  参数说明

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | dst_align32b | 输入/输出 | 目的操作数（矢量）的起始地址，搬运完成后该地址会被硬件自动Post Update。 |
  | src | 输入 | 源操作数（矢量数据寄存器）。 |
  | offset | 输入 | 地址偏移量。 |
  | mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。 |

- 从掩码寄存器连续对齐搬出到UB的场景

  **表 2**  参数说明

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | dst | 输入/输出 | 目的操作数（矢量）的起始地址，搬运完成后该地址会被硬件自动Post Update。 |
  | src | 输入 | 源操作数（掩码寄存器）。 |
  | offset | 输入 | 地址偏移量。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为矢量数据寄存器或掩码寄存器，目的操作数为UB地址。
- dst_align32b起始地址需32字节对齐，否则会报错。
- UB容量上限：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。dst_align32b起始地址与Post Update累计偏移量之和不可超过实际可用容量，否则会报错。
- mask必须为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- mask比特位为0的位置采用保持模式：dst_align32b对应位置保持原值不变，不写入src数据。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 连续对齐搬出约束

- offset取0时dst_align32b指针不变，仍按当前dst_align32b起始位置写入数据，可重复写入同一组目的地址。
- offset取负值时dst_align32b指针向后回退，需确保回退后的地址仍32字节对齐且不低于UB起始地址，否则会报错。
- Post Update后的dst_align32b新地址仍需满足32字节对齐，否则下一次调用本接口会报错。
- Post Update仅更新dst_align32b指针，不影响src寄存器内容；多次调用时每次都从同一src寄存器搬出数据，如需搬出不同数据应先更新src寄存器内容。
- 从矢量数据寄存器连续对齐搬出到UB的场景offset单位为元素，从掩码寄存器连续对齐搬出到UB的场景单位为字节。

## 调用示例

```cpp
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
int32_t offset = 128;
asc_storealign_postupdate(dst_align32b, src, offset, mask);
```
