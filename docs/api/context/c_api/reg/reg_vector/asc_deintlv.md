
# asc_deintlv

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |
## 功能说明

给定源操作数src0和src1，将src0和src1中的元素解交织存入结果操作数dst0和dst1中，解交织排列方式如下图所示，其中每个方格代表1个元素：


## 函数原型

```cpp
__simd_callee__ inline void asc_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1)
__simd_callee__ inline void asc_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1)
__simd_callee__ inline void asc_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, vector_int16_t src0, vector_int16_t src1)
__simd_callee__ inline void asc_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, vector_uint16_t src0, vector_uint16_t src1)
__simd_callee__ inline void asc_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, vector_int8_t src0, vector_int8_t src1)
__simd_callee__ inline void asc_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, vector_uint8_t src0, vector_uint8_t src1)
__simd_callee__ inline void asc_deintlv(vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1)
__simd_callee__ inline void asc_deintlv(vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1)
__simd_callee__ inline void asc_deintlv(vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1)
__simd_callee__ inline void asc_deintlv(vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, vector_bfloat16_t src0, vector_bfloat16_t src1)
__simd_callee__ inline void asc_deintlv(vector_float& dst0, vector_float& dst1, vector_float src0, vector_float src1)
__simd_callee__ inline void asc_deintlv(vector_half& dst0, vector_half& dst1, vector_half src0, vector_half src1)

```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst0 | 输出 | 目的操作数（矢量）的起始地址。 |
| dst1 | 输出 | 目的操作数（矢量）的起始地址。 |
| src0 | 输入 | 源操作数（矢量）的起始地址。 |
| src1 | 输入 | 源操作数（矢量）的起始地址。 |

## 返回值说明

无

## 约束说明

无

## 调用示例

```cpp
vector_int32_t dst0;
vector_int32_t dst1;
vector_int32_t src0;
vector_int32_t src1;
asc_deintlv(dst0, dst1, src0, src1);
```