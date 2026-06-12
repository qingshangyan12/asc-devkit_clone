# asc_storealign_postupdate

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

对齐数据搬出Post Update接口，与asc_storealign的区别在于该接口支持硬件操作的目的操作数地址自动更新，用户在编程时不用关心目的操作数地址更新逻辑。支持基本模式与repeat stride模式，同时支持矢量数据寄存器和掩码寄存器搬出。

## 函数原型

```cpp
// 从矢量数据寄存器搬出到UB - 基本Post Update模式
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int8_t* dst_align32b, vector_int8_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t* dst_align32b, vector_uint8_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int16_t* dst_align32b, vector_int16_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ half* dst_align32b, vector_half src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ float* dst_align32b, vector_float src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src, int32_t offset, vector_bool mask)

// 从矢量数据寄存器搬出到UB - repeat stride + PostUpdate模式
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int8_t* dst_align32b, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t* dst_align32b, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ hifloat8_t* dst_align32b, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int16_t* dst_align32b, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ half* dst_align32b, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int32_t* dst_align32b, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ float* dst_align32b, vector_float src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

// 从掩码寄存器搬出到UB - 基本Post Update模式
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint32_t*& dst, vector_bool src, int32_t offset)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint16_t*& dst, vector_bool src, int32_t offset)
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t*& dst, vector_bool src, int32_t offset)
```

## 参数说明

- 从矢量数据寄存器搬出的场景

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | dst_align32b | 输出 | 目的操作数（矢量）的起始地址。 |
  | src | 输入 | 源操作数（矢量数据寄存器）。 |
  | mask | 输入 | 源操作数掩码（掩码寄存器）。 |
  | offset | 输入 | 地址偏移量。 |
  | block_stride | 输入 | 源操作数单次迭代内不同DataBlock间地址步长。 |
  | repeat_stride | 输入 | 源操作数相邻迭代间相同DataBlock的地址步长。 |

- 从掩码寄存器搬出的场景

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | dst | 输入/输出 | 目的操作数（矢量）的起始地址，硬件自动更新。 |
  | src | 输入 | 源操作数（掩码寄存器）。 |
  | offset | 输入 | 地址偏移量。 |

矢量数据寄存器、掩码寄存器和地址寄存器的详细说明请参见[reg数据类型定义.md](../../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

无

## 调用示例

```cpp
// 从矢量数据寄存器搬出 - 基本Post Update
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
int32_t offset = 128;
asc_storealign_postupdate(dst_align32b, src, offset, mask);

// 从掩码寄存器搬出 - PostUpdate
__ubuf__ uint32_t*& dst = (__ubuf__ uint32_t*&)asc_get_phy_buf_addr(0);
vector_bool src;
int32_t offset = 0;
asc_storealign_postupdate(dst, src, offset);
```
