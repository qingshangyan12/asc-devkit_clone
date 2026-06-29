# asc_storealign_pack

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

Reg计算数据搬运存储接口，PACK压缩搬出模式。根据mask将src中有效元素的低半部分bit数据连续存储于dst中，支持b16→b8、b32→b16、b64→b32。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int16_t* dst_align32b, vector_int16_t src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst_align32b, vector_uint16_t src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int32_t* dst_align32b, vector_int32_t src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int64_t* dst_align32b, vector_int64_t src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint64_t* dst_align32b, vector_uint64_t src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst_align32b, vector_half src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack(__ubuf__ float* dst_align32b, vector_float src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack(__ubuf__ bfloat16_t* dst_align32b, vector_bfloat16_t src, vector_bool mask);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst_align32b | 输出 | 目的操作数地址，需32B对齐 |
| src | 输入 | 源操作数 |
| mask | 输入 | 掩码控制 |

## 返回值说明

无

## 调用示例

```cpp
__ubuf__ int16_t* dst_addr;
vector_int16_t src;
vector_bool mask;
asc_storealign_pack(dst_addr, src, mask);
```
