# asc_loadalign_unpack4_postupdate

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

对齐数据搬运接口，解压缩搬入模式（unpack4），使能postupdate自动更新UB源地址。按b8加载VL/4长度数据，unpack到VL长度b32类型。

## 函数原型

```cpp
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst | 输出 | 目的操作数，4倍解压 |
| src | 输入/输出 | 源操作数地址（postupdate后自动偏移） |
| offset | 输入 | 偏移量 |

## 返回值说明

无

## 调用示例

```cpp
vector_uint32_t dst;
__ubuf__ uint8_t* src_addr;
int32_t offset = 128;
asc_loadalign_unpack4_postupdate(dst, src_addr, offset);
```
