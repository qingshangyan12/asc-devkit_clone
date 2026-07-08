# asc_uint82uint32

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

将uint8_t类型转化为uint32_t类型。

由于源操作数与目的操作数类型位宽比为1:4，读取数据时需要将一个VL大小的数据分为四部分，根据不同接口选取索引0、索引1、索引2或者索引3。

## 函数原型

```cpp
// 读取索引0的位置
__simd_callee__ inline void asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask, ASC_DISPERSE_FIRST_QUARTER)
// 读取索引1的位置
__simd_callee__ inline void asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask, ASC_DISPERSE_SECOND_QUARTER)
// 读取索引2的位置
__simd_callee__ inline void asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask, ASC_DISPERSE_THIRD_QUARTER)
// 读取索引3的位置
__simd_callee__ inline void asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask, ASC_DISPERSE_FOURTH_QUARTER)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |
| quarter | 输入 | 位置参数，指定数据读取的位置，可选ASC_DISPERSE_FIRST_QUARTER（索引0）、ASC_DISPERSE_SECOND_QUARTER（索引1）、ASC_DISPERSE_THIRD_QUARTER（索引2）或ASC_DISPERSE_FOURTH_QUARTER（索引3）。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

无

## 调用示例

```cpp
vector_uint8_t src;
vector_uint32_t dst;
vector_bool mask;
asc_uint82uint32(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
```
