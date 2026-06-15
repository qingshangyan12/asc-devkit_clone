# asc_uint322uint16

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

将uint32_t类型转化为uint16_t类型。

根据输入数据的大小分为非饱和模式和饱和模式。
- 非饱和模式：结果被截断至目标格式的数据宽度，且保留最低有效位（LSB）。
- 饱和模式： 结果被限制在目标格式的可表示范围内。

由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个VL大小的数据分为两部分，根据不同接口选取索引偶数的位置或者索引奇数的位置。

## 函数原型

```cpp
// 非饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position, asc_position::EVEN> pos)

// 饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position, asc_position::EVEN> pos)

// 非饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position, asc_position::ODD> pos)

// 饱和模式，数据写入索引为奇数的位置
__simd_callee__ inline void asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position, asc_position::ODD> pos)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |
| pos | 输入 | 目的操作数写入位置。取值范围：<br>ASC_POSITION_EVEN：写入索引为偶数的位置；<br>ASC_POSITION_ODD：写入索引为奇数的位置。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 开启饱和模式和非饱和模式时，需配置ctrl寄存器，ctrl寄存器的详细说明请参见[asc_set_ctrl.md](../../系统变量/asc_set_ctrl.md)。

## 调用示例

```cpp
vector_uint32_t src;
vector_uint16_t dst;
vector_bool mask;
asc_uint322uint16(dst, src, mask, ASC_POSITION_ODD);
```
