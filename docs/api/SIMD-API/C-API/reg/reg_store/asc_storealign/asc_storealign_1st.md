# asc_storealign_1st

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

Reg计算数据搬运存储接口，FIRST搬出模式。忽略mask，向dst中搬运src第一个元素。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst_align32b, vector_int8_t src);
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst_align32b, vector_uint8_t src);
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst_align32b, vector_int16_t src);
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src);
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst_align32b, vector_int32_t src);
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src);
__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst_align32b, vector_half src);
__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst_align32b, vector_float src);
__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst_align32b | 输出 | 目的操作数地址，需32B对齐 |
| src | 输入 | 源操作数 |

## 返回值说明

无

## 调用示例

```cpp
__ubuf__ float* dst_addr;
vector_float src;
asc_storealign_1st(dst_addr, src);
```
