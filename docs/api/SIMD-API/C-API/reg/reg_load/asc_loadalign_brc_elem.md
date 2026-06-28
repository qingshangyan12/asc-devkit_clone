# asc_loadalign_brc_elem

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

对齐数据搬运接口，广播搬运模式（elem），搬入一个元素并广播至目的操作数所有元素位置，接口不传入偏移，需用户自行更新源操作数地址。

## 函数原型

```cpp
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst | 输出 | 目的操作数 |
| src | 输入 | 源操作数地址 |

## 返回值说明

无

## 约束说明

- 源操作数地址需32B对齐

## 调用示例

```cpp
vector_float dst;
__ubuf__ float* src_addr;
asc_loadalign_brc_elem(dst, src_addr);
```
