# asc_uint162uint8

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

将uint16_t类型转化为uint8_t类型，无舍入模式。

## 函数原型

```cpp
// 非饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
// 饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
// 非饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_uint162uint8_v2(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
// 饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_uint162uint8_sat_v2(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
```

## 参数说明

| 参数名    | 输入/输出 | 描述                |
| :------ | :----- | :----------------- |
| dst    | 输出    | 目的操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md/#矢量数据寄存器)。            |
| src    | 输入    | 源操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md/#矢量数据寄存器)。             |
| mask | 输入 | 源操作数掩码（矢量），类型为[掩码寄存器](../reg数据类型定义.md/#掩码寄存器)，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 非饱和模式：输入数据超出目标数据范围时，会截断为目标数据格式，例如输入uint16_t值为256，输出uint8_t值为0。
- 饱和模式：输入数据超出目标数据范围时，返回输出类型的对应最值。

## 调用示例

```cpp
vector_uint16_t src;
vector_uint8_t dst;
vector_bool mask;
asc_uint162uint8(dst, src, mask);
```
