# asc_storealign

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

正常模式（NORM），从矢量数据寄存器或掩码寄存器连续对齐搬出到UB，搬运VL数据。支持多种偏移/地址变体：不传入偏移由用户自行更新目的地址、通过地址寄存器iter_reg传入偏移、通过int32_t传入偏移、repeat stride模式。Post Update模式请参见[asc_storealign_postupdate](asc_storealign_postupdate.md)。

## 函数原型

```cpp
// 从矢量数据寄存器搬出到UB

// 不带偏移
__simd_callee__ inline void asc_storealign(__ubuf__  int8_t* dst_align32b, vector_int8_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  uint8_t* dst_align32b, vector_uint8_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  int16_t* dst_align32b, vector_int16_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  uint16_t* dst_align32b, vector_uint16_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  int32_t* dst_align32b, vector_int32_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  half* dst_align32b, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  float* dst_align32b, vector_float src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  bfloat16_t* dst_align32b, vector_bfloat16_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  hifloat8_t* dst_align32b, vector_hifloat8_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp4x2_e2m1_t* dst_align32b, vector_fp4x2_e2m1_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp4x2_e1m2_t* dst_align32b, vector_fp4x2_e1m2_t src, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  int4b_t* dst_align32b, vector_int4x2_t src, vector_bool mask)

// 通过iter_reg传入偏移
__simd_callee__ inline void asc_storealign(__ubuf__  int8_t* dst_align32b, vector_int8_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  uint8_t* dst_align32b, vector_uint8_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  int16_t* dst_align32b, vector_int16_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  uint16_t* dst_align32b, vector_uint16_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  int32_t* dst_align32b, vector_int32_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  half* dst_align32b, vector_half src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  float* dst_align32b, vector_float src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  bfloat16_t* dst_align32b, vector_bfloat16_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  hifloat8_t* dst_align32b, vector_hifloat8_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp4x2_e2m1_t* dst_align32b, vector_fp4x2_e2m1_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  fp4x2_e1m2_t* dst_align32b, vector_fp4x2_e1m2_t src, iter_reg offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__  int4b_t* dst_align32b, vector_int4x2_t src, iter_reg offset, vector_bool mask)

// 通过int32_t传入偏移
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst_align32b, vector_int8_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst_align32b, vector_uint8_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ int16_t* dst_align32b, vector_int16_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst_align32b, vector_half src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ float* dst_align32b, vector_float src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ hifloat8_t* dst_align32b, vector_hifloat8_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ fp4x2_e2m1_t* dst_align32b, vector_fp4x2_e2m1_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ fp4x2_e1m2_t* dst_align32b, vector_fp4x2_e1m2_t src, int32_t offset, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ int4b_t* dst_align32b, vector_int4x2_t src, int32_t offset, vector_bool mask)

// repeat stride模式
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst_align32b, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst_align32b, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ hifloat8_t* dst_align32b, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ int16_t* dst_align32b, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst_align32b, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ int32_t* dst_align32b, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
__simd_callee__ inline void asc_storealign(__ubuf__ float* dst_align32b, vector_float src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)

// 从掩码寄存器搬出到UB

// 不带偏移
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_bool src)
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_bool src)
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src)

// 通过iter_reg传入偏移
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_bool src, iter_reg offset)
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_bool src, iter_reg offset)
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src, iter_reg offset)

// 通过int32_t传入偏移
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst,  vector_bool src, int32_t offset)
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst,  vector_bool src, int32_t offset)
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src, int32_t offset)
```

## 参数说明

- 从矢量数据寄存器连续对齐搬出到UB的场景

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | dst_align32b | 输出 | 目的操作数（矢量）的起始地址。 |
  | src | 输入 | 源操作数（矢量数据寄存器）。 |
  | mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |
  | offset | 输入 | 当输入为reg_iter类型时，用户通过地址寄存器传入偏移；当输入为int32_t类型时，用户直接以数值的方式传入偏移。 |
  | block_stride | 输入 | 源操作数单次迭代内不同DataBlock间地址步长。 |
  | repeat_stride | 输入 | 源操作数相邻迭代间相同DataBlock的地址步长。 |

- 从掩码寄存器连续对齐搬出到UB的场景

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | dst | 输出 | 目的操作数（矢量）的起始地址。PostUpdate模式下为输入/输出。 |
  | src | 输入 | 源操作数（掩码寄存器）。 |
  | offset | 输入 | 当输入为reg_iter类型时，用户通过地址寄存器传入偏移；当输入为int32_t类型时，用户直接以数值的方式传入偏移。 |

矢量数据寄存器、掩码寄存器和地址寄存器的详细说明请参见[reg数据类型定义.md](../../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

无

## 调用示例

```cpp
// 从矢量数据寄存器搬出 - 不带偏移
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_storealign(dst_align32b, src, mask);

// 从矢量数据寄存器搬出 - 通过iter_reg传入偏移
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
uint32_t offset = 128;
iter_reg addr_reg = asc_create_iter_reg_b16(offset);
asc_storealign(dst_align32b, src, addr_reg, mask);

// 从矢量数据寄存器搬出 - 通过int32_t传入偏移
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
int32_t offset = 128;
asc_storealign(dst_align32b, src, offset, mask);

// repeat stride模式
__ubuf__ int8_t* dst_align32b;
vector_int8_t src;
uint16_t block_stride;
uint16_t repeat_stride;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_storealign(dst_align32b, src, block_stride, repeat_stride, mask);

// 从掩码寄存器搬出
__ubuf__ uint32_t* dst = (__ubuf__ uint32_t*)asc_get_phy_buf_addr(0);
vector_bool src;
asc_storealign(dst, src);

// 从掩码寄存器搬出 - 通过int32_t传入偏移
__ubuf__ uint32_t* dst = (__ubuf__ uint32_t*)asc_get_phy_buf_addr(0);
vector_bool src;
int32_t offset = 0;
asc_storealign(dst, src, offset);

## 其他搬出模式

- **[asc_storealign_first](asc_storealign_first.md)** - FIRST搬出模式
- **[asc_storealign_pack](asc_storealign_pack.md)** - PACK搬出模式
- **[asc_storealign_pack_quarter](asc_storealign_pack_quarter.md)** - PACK_QUARTER搬出模式
- **[asc_storealign_intlv](asc_storealign_intlv.md)** - INTLV搬出模式
- **[asc_storealign_postupdate](asc_storealign_postupdate.md)** - PostUpdate搬出模式
