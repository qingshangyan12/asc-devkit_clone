
# asc_deInterleave

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | √ |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | √ |

## 功能说明

给定源操作数src0和src1，将src0和src1中的元素解交织存入结果操作数dst0和dst1中，解交织排列方式如下图所示，其中每个方格代表1个元素：


## 函数原型

```cpp
__simd_callee__ inline void asc_ln(vector_half &dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_ln(vector_float &dst, vector_float src, vector_bool mask)

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
//total_length 指参与计算的数据长度
constexpr uint32_t total_length = 128;
__ubuf__ half src[total_length];
__ubuf__ half dst[total_length];
asc_abs(dst, src, total_length);
```