# asc_storealign_pack_quarter

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

四分之一压缩模式，根据mask将src中有效元素的低8bits数据连续存储于dst_align32b中，支持数据类型为b32。支持多种偏移/地址变体。Post Update模式请参见[asc_storealign_pack_quarter_postupdate](asc_storealign_pack_quarter_postupdate.md)。

## 函数原型

```cpp
// 不带偏移
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  int32_t* dst_align32b, vector_int32_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  float* dst_align32b, vector_float src, vector_bool mask)

// 通过iter_reg传入偏移
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  int32_t* dst_align32b, vector_int32_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  float* dst_align32b, vector_float src, iter_reg offset, vector_bool mask)

// 通过int32_t传入偏移
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__  float* dst_align32b, vector_float src, int32_t offset, vector_bool mask)

```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst_align32b | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |
| offset | 输入 | 当输入为reg_iter类型时，用户通过地址寄存器传入偏移；当输入为int32_t类型时，用户直接以数值的方式传入偏移。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义.md](../../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

无

## 调用示例

```cpp
// 不带偏移
__ubuf__ int32_t* dst_align32b = (__ubuf__ int32_t*)asc_get_phy_buf_addr(0);
vector_int32_t src;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_storealign_pack_quarter(dst_align32b, src, mask);

```
