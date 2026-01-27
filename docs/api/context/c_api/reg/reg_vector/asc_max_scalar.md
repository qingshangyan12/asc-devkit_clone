# asc_max_scalar

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。计算公式如下：

$$
dst_i = max(src_i, value)
$$

## 函数原型

```cpp
__simd_callee__ inline void asc_max_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask)
__simd_callee__ inline void asc_max_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask)
__simd_callee__ inline void asc_max_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask)
__simd_callee__ inline void asc_max_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask)
__simd_callee__ inline void asc_max_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask)
__simd_callee__ inline void asc_max_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
__simd_callee__ inline void asc_max_scalar(vector_half& dst, vector_half src, half value, vector_bool mask)
__simd_callee__ inline void asc_max_scalar(vector_float& dst, vector_float src, float value, vector_bool mask)
__simd_callee__ inline void asc_max_scalar(vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)
  ```
## 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| dst       | 输出    | 目的操作数（矢量）的起始地址。            |
| src | 输入    | 源操作数（矢量）的起始地址。           |
| value | 输入    | 源操作数（标量）。       |
| mask | 输入 | 用于按位控制参与计算的元素，bit位值为1表示参与计算，0表示不参与。 |

## 返回值说明

无

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。

## 调用示例

```cpp
constexpr int total_length = 256; //total_length 指参与计算的数据长度
half value = 10;
__ubuf__ half src[total_length];
__ubuf__ half dst[total_length];
asc_max_scalar(dst, src, value, total_length);
// src : [1, 2, 3, 4, ... , 256]
// value : 3 
// dst : [3, 3, 3, 5, ... , 256]
```