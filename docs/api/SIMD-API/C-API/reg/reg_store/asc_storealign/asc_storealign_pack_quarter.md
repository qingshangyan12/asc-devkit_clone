# asc_storealign_pack_quarter

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

Reg计算数据搬运存储接口，四分之一粒度压缩搬出模式（pack quarter），根据mask将src中有效元素的低四分之一bit数据（b8）连续存储于dst中。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst_align32b, vector_int32_t src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask);
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ float* dst_align32b, vector_float src, vector_bool mask);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst_align32b | 输出 | 目的操作数地址，需32B对齐 |
| src | 输入 | 源操作数 |
| mask | 输入 | 掩码控制 |

## 返回值说明

无

## 约束说明

- 目的地址需32B对齐
- 输入数据类型为b32，取低1/4存储

## 调用示例

```cpp
__ubuf__ int32_t* dst_addr;
vector_int32_t src;
vector_bool mask;
asc_storealign_pack_quarter(dst_addr, src, mask);
```
