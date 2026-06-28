# asc_loadalign_brc_datablock_postupdate

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

对齐数据搬运接口，广播搬运模式（datablock），使能postupdate自动更新UB源地址。搬入一个DataBlock（32B）并广播到VL。

## 函数原型

```cpp
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst | 输出 | 目的操作数 |
| src | 输入/输出 | 源操作数地址（postupdate后自动偏移） |
| offset | 输入 | 偏移量 |

## 返回值说明

无

## 调用示例

```cpp
vector_float dst;
__ubuf__ float* src_addr;
int32_t offset = 128;
asc_loadalign_brc_datablock_postupdate(dst, src_addr, offset);
```
