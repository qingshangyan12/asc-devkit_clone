# asc_storealign_first

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

FIRST搬出模式，忽略mask，向dst_align32b中搬运src第一个元素，支持数据类型为b8、b16、b32。支持多种偏移/地址变体。

## 函数原型

```cpp
// 不带偏移
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int8_t* dst_align32b, vector_int8_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint8_t* dst_align32b, vector_uint8_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int16_t* dst_align32b, vector_int16_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint16_t* dst_align32b, vector_uint16_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int32_t* dst_align32b, vector_int32_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  half* dst_align32b, vector_half src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  float* dst_align32b, vector_float src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  bfloat16_t* dst_align32b, vector_bfloat16_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  hifloat8_t* dst_align32b, vector_hifloat8_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp4x2_e2m1_t* dst_align32b, vector_fp4x2_e2m1_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp4x2_e1m2_t* dst_align32b, vector_fp4x2_e1m2_t src)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int4b_t* dst_align32b, vector_int4x2_t src)

// 通过iter_reg传入偏移
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int8_t* dst_align32b, vector_int8_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint8_t* dst_align32b, vector_uint8_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int16_t* dst_align32b, vector_int16_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint16_t* dst_align32b, vector_uint16_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int32_t* dst_align32b, vector_int32_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  half* dst_align32b, vector_half src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  float* dst_align32b, vector_float src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  bfloat16_t* dst_align32b, vector_bfloat16_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  hifloat8_t* dst_align32b, vector_hifloat8_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp4x2_e2m1_t* dst_align32b, vector_fp4x2_e2m1_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp4x2_e1m2_t* dst_align32b, vector_fp4x2_e1m2_t src, iter_reg offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int4b_t* dst_align32b, vector_int4x2_t src, iter_reg offset)

// 通过int32_t传入偏移
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int8_t* dst_align32b, vector_int8_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint8_t* dst_align32b, vector_uint8_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int16_t* dst_align32b, vector_int16_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint16_t* dst_align32b, vector_uint16_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int32_t* dst_align32b, vector_int32_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  half* dst_align32b, vector_half src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  float* dst_align32b, vector_float src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  bfloat16_t* dst_align32b, vector_bfloat16_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e4m3fn_t* dst_align32b, vector_fp8_e4m3fn_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  hifloat8_t* dst_align32b, vector_hifloat8_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e5m2_t* dst_align32b, vector_fp8_e5m2_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp8_e8m0_t* dst_align32b, vector_fp8_e8m0_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp4x2_e2m1_t* dst_align32b, vector_fp4x2_e2m1_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  fp4x2_e1m2_t* dst_align32b, vector_fp4x2_e1m2_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st(__ubuf__  int4b_t* dst_align32b, vector_int4x2_t src, int32_t offset)

// 硬件自动Post Update模式
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  int8_t* dst_align32b, vector_int8_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  uint8_t* dst_align32b, vector_uint8_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  int16_t* dst_align32b, vector_int16_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  uint16_t* dst_align32b, vector_uint16_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  int32_t* dst_align32b, vector_int32_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  uint32_t* dst_align32b, vector_uint32_t src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  half* dst_align32b, vector_half src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  float* dst_align32b, vector_float src, int32_t offset)
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__  bfloat16_t* dst_align32b, vector_bfloat16_t src, int32_t offset)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst_align32b | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
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
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
asc_storealign_1st(dst_align32b, src);

// 硬件自动Post Update
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
int32_t offset = 128;
asc_storealign_1st_postupdate(dst_align32b, src, offset);
```
