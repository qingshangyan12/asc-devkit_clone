# asc_storealign_intlv

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

Reg计算数据搬运存储接口，INTLV双搬出模式。忽略mask，将src0、src1中的元素交错存储于dst中，存储长度为VL*2。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int8_t* dst_align32b, vector_int8_t src0, vector_int8_t src1);
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint8_t* dst_align32b, vector_uint8_t src0, vector_uint8_t src1);
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int16_t* dst_align32b, vector_int16_t src0, vector_int16_t src1);
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src0, vector_uint16_t src1);
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ half* dst_align32b, vector_half src0, vector_half src1);
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src0, vector_bfloat16_t src1);
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int32_t* dst_align32b, vector_int32_t src0, vector_int32_t src1);
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src0, vector_uint32_t src1);
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ float* dst_align32b, vector_float src0, vector_float src1);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst_align32b | 输出 | 目的操作数地址，需32B对齐 |
| src0 | 输入 | 源操作数0 |
| src1 | 输入 | 源操作数1 |

## 返回值说明

无

## 调用示例

```cpp
__ubuf__ float* dst_addr;
vector_float src0;
vector_float src1;
asc_storealign_intlv(dst_addr, src0, src1);
```
