# asc_reduce_add

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | √ |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | √ |

## 功能说明

归约求和功能，用于将src中的所有参与计算的元素求和，得到的结果保存在dst中，计算公式如下：
$$
dst = ∑_{i=0}^{N-1} (src_i × mask_i)
$$

## 函数原型

```cpp
__simd_callee__ inline void asc_reduce_add(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
__simd_callee__ inline void asc_reduce_add(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
__simd_callee__ inline void asc_reduce_add(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
__simd_callee__ inline void asc_reduce_add(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
__simd_callee__ inline void asc_reduce_add(vector_half& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_reduce_add(vector_float& dst, vector_float src, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量）的起始地址。 |
| mask | 输入 | 用于按位控制参与计算的元素，bit位值为1表示参与计算，0表示不参与。 |

## 返回值说明

无

## 约束说明

- 操作数地址对齐要求请参考
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
vector_half dst;
vector_half src;
vector_bool mask;
asc_reduce_add(dst, src, mask);
```