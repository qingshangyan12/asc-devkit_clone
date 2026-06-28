# asc_loadalign_unpack4

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

对齐数据搬运接口，解压缩搬入模式（unpack4），按u8类型加载VL/4长度数据，unpack到VL长度u32类型，中间位置补0。

## 函数原型

```cpp
__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src);
__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src);
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);
__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src);
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);
__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst | 输出 | 目的操作数，4倍解压 |
| src | 输入 | 源操作数地址 |

## 返回值说明

无

## 约束说明

- 源操作数地址需32B对齐
- 输入数据类型为b8/b8类型，输出为b32类型

## 调用示例

```cpp
vector_uint32_t dst;
__ubuf__ uint8_t* src_addr;
asc_loadalign_unpack4(dst, src_addr);
```
