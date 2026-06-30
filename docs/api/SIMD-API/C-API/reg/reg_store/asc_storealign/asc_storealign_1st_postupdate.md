# asc_storealign_1st_postupdate

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

Reg计算数据搬运存储接口，FIRST搬出模式，使能postupdate自动更新UB目的地址。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int8_t*& dst_align32b, vector_int8_t src, int32_t offset);
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint8_t*& dst_align32b, vector_uint8_t src, int32_t offset);
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int16_t*& dst_align32b, vector_int16_t src, int32_t offset);
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint16_t*& dst_align32b, vector_uint16_t src, int32_t offset);
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset);
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset);
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ half*& dst_align32b, vector_half src, int32_t offset);
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ float*& dst_align32b, vector_float src, int32_t offset);
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ bfloat16_t*& dst_align32b, vector_bfloat16_t src, int32_t offset);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst_align32b | 输入/输出 | 目的操作数地址（postupdate后自动偏移） |
| src | 输入 | 源操作数 |
| offset | 输入 | 偏移量 |

## 返回值说明

无

## 调用示例

```cpp
__ubuf__ float* dst_addr;
vector_float src;
int32_t offset = 128;
asc_storealign_1st_postupdate(dst_addr, src, offset);
```
