# asc_uint322uint16

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| <term>Ascend 950PR/Ascend 950DT</term> | √ |

## 功能说明

将uint32_t类型转化为uint16_t类型。

由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个VL大小的数据分为两部分，根据不同接口选取索引0或者索引1。

## 函数原型

```cpp
// 非饱和模式，数据写入索引为0的位置
__simd_callee__ inline void asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
// 饱和模式，数据写入索引为0的位置
__simd_callee__ inline void asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
// 非饱和模式，数据写入索引为1的位置
__simd_callee__ inline void asc_uint322uint16_v2(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
// 饱和模式，数据写入索引为1的位置
__simd_callee__ inline void asc_uint322uint16_sat_v2(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 非饱和模式：
    - 输入数据超过输出类型最值时，结果被截断为目标格式的数据宽度，例如输入half值为257，输出int8_t值为1。
    - 输入数据为+/-inf时，返回输出类型的最值。
    - 输入数据为nan时，返回0。
- 饱和模式：
    - 输入数据超过输出类型最值时，返回输出类型的对应最值，例如输入half值为257，输出int8_t值为127。
    - 输入数据为+/-inf时，返回输出类型的最值。
    - 输入数据为nan时，返回0。

## 调用示例

```cpp
vector_uint32_t src;
vector_uint16_t dst;
vector_bool mask;
asc_uint322uint16(dst, src, mask);
```
