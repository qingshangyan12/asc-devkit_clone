# asc_loadalign_deintlv

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

双搬入模式（DEINTLV），基于元素的交错搬运，从src中读取2*VL长度数据，将偶数索引的元素存入dst0，将奇数索引的元素存入dst1，数据类型为b8/b16/b32。支持多种偏移/地址变体。

## 函数原型

```cpp
// 不带偏移
__simd_callee__ inline void asc_loadalign_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1, __ubuf__ half* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0, vector_float& dst1, __ubuf__ float* src)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src)

// 通过iter_reg传入偏移
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1, __ubuf__ half* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0, vector_float& dst1, __ubuf__ float* src, iter_reg offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, iter_reg offset)

// 通过int32_t传入偏移
__simd_callee__ inline void asc_loadalign_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1, __ubuf__ half* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0, vector_float& dst1, __ubuf__ float* src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv(vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, int32_t offset)

// 硬件自动Post Update模式
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_half& dst0, vector_half& dst1, __ubuf__ half*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_float& dst0, vector_float& dst1, __ubuf__ float*& src, int32_t offset)
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t*& src, int32_t offset)
```

## 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| dst0       | 输出    | 目的操作数0（矢量数据寄存器）。            |
| dst1       | 输出    | 目的操作数1（矢量数据寄存器）。            |
| src | 输入    | 源操作数（矢量）的起始地址。PostUpdate模式下为输入/输出。          |
| offset | 输入    | 当输入为reg_iter类型时，用户通过地址寄存器传入偏移；当输入为int32_t类型时，用户直接以数值的方式传入偏移。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义.md](../../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- iter_reg方式传入偏移时，该接口中的src需要32B对齐。
- offset在缺省时默认偏移值offset=0。

## 调用示例

```cpp
// 不带偏移
vector_half dst0, dst1;
__ubuf__ half* src;
asc_loadalign_deintlv(dst0, dst1, src);

// 硬件自动Post Update
vector_half dst0, dst1;
__ubuf__ half* src;
int32_t offset;
asc_loadalign_deintlv_postupdate(dst0, dst1, src, offset);
```
