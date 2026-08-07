# asc_storealign_pack_quarter

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

从矢量数据寄存器压缩搬出到UB的接口，根据mask将src中有效元素的低8bits数据连续存储于dst_align32b中，支持数据类型为b32。

支持Post Update模式，接口调用后自动更新目的操作数地址。

## 函数原型

- 偏移固定传入0，由用户自行更新目的操作数的地址
  ```cpp
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst_align32b, vector_int32_t src, vector_bool mask)
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask)
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ float* dst_align32b, vector_float src, vector_bool mask)
  ```
- 通过int32_t传入偏移，用户可以选择更新偏移或者更新目的操作数的地址
  ```cpp
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ float* dst_align32b, vector_float src, int32_t offset, vector_bool mask)
  ```
- 通过addr_reg地址寄存器传入偏移，用户可以选择更新偏移或者更新目的操作数的地址
  ```cpp
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst_align32b, vector_int32_t src, addr_reg offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, addr_reg offset, vector_bool mask)
  __simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ float* dst_align32b, vector_float src, addr_reg offset, vector_bool mask)
  ```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst_align32b | 输出 | 目的操作数（矢量）的起始地址。 |
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

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，src为矢量数据寄存器，dst_align32b为UB地址，UB地址空间外的指针不可作为dst_align32b传入。
- dst_align32b起始地址需32字节对齐，否则会报错。
- mask必须为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- **UB容量上限**：UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈+2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。dst_align32b偏移与写出区域对应的UB范围不可超过实际可用容量，否则会报错。

### 带立即数偏移压缩搬出约束

- offset单位为元素个数。例如dtype为`int32_t`时offset=4表示实际访问地址向后偏移4×4=16字节；dtype为`float`时offset=2表示偏移8字节。
- offset取值偏移后需落在UB地址范围内，且仍需32字节对齐，否则会报错。
- offset入参为`int32_t`类型，需保证偏移后写出区域不超出UB可用容量范围。

### 带地址寄存器偏移压缩搬出约束

- offset必须由配套的地址寄存器构造接口预先构造。
- offset取值偏移后需落在UB地址范围内，且仍需32字节对齐，否则会报错。

## 调用示例

```cpp
__ubuf__ float* dst_align32b = (__ubuf__ float*)asc_get_phy_buf_addr(0);
vector_float src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_storealign_pack_quarter(dst_align32b, src, mask);
```
