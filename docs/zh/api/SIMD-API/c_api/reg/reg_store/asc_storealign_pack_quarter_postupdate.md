# asc_storealign_pack_quarter_postupdate

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

从矢量数据寄存器压缩搬出到UB的接口，根据mask将src中有效元素的低8bits数据连续存储于dst_align32b中，支持数据类型为b32，接口调用后自动更新目的操作数地址。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(__ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask)
```

## 参数说明

**表 1**  参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst_align32b | 输入/输出 | 目的操作数（矢量）的起始地址，接口调用后自动更新。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| offset | 输入 | 偏移量。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，`src`为矢量数据寄存器，`dst_align32b`为UB地址，UB地址空间外的指针不可作为`dst_align32b`传入。
- `dst_align32b`起始地址需32字节对齐。
- mask必须为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。`dst_align32b`起始地址、`offset × sizeof(dtype)`偏移与写出区域对应的UB范围三者之和不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### Quarter Pack压缩与Post Update约束

- `offset`入参为`int32_t`类型，需保证偏移后写出区域不超出UB可用容量范围。
- `offset`取0时写入位置与`dst_align32b`当前指向地址重合，且Post Update后`dst_align32b`指针不变，可重复向同一地址写入同一`src`内容。
- `offset`取负值时Post Update后`dst_align32b`指针向后回退，需确保回退后的地址仍32字节对齐且不低于UB起始地址，否则会报错。
- Post Update后的`dst_align32b`新地址仍需满足32字节对齐，否则下一次调用本接口会报错；通常取`offset`使偏移量为32字节的整数倍以保证对齐（dtype字节大小为4时取`offset`为8的整数倍）。
- Post Update仅更新`dst_align32b`指针，不影响`src`寄存器内容与`mask`寄存器内容；多次调用时每次都按当前`mask`压缩当前`src`写出，若需改变写出内容应先更新`src`或`mask`。

## 调用示例

```cpp
__ubuf__ float* dst_align32b = (__ubuf__ float*)asc_get_phy_buf_addr(0);
vector_float src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_storealign_pack_quarter_postupdate(dst_align32b, src, 0, mask);
```
