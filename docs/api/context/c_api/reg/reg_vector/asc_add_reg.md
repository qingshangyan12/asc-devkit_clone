# asc_add

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| <term>Ascend 950PR/Ascend 950DT</term> | √ |

## 功能说明

按照元素对应位置执行矢量加法运算，计算过程包含ZEROING和MERGING两种模式。计算公式如下：

$$
dst_i = src0_i + src1_i
$$

## 函数原型

```cpp
__simd_callee__ inline void asc_add_impl(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_bool& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
__simd_callee__ inline void asc_add_impl(vector_bool& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。 |
| dst0 | 输出 | 目的操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。vector_bool类型。 |
| src0 | 输入 | 源操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。 |
| src1 | 输入 | 源操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。 |
| mask | 输入 | 源操作数掩码。对应位置为1时参与计算，为0时不参与计算。<br>&bull; 模式：mask未筛选的元素在输出中置零。<br>&bull; 模式：暂不支持。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- dst、src0、src1的需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../../../docs/api/context/c_api/general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
//total_length 指参与计算的数据长度
constexpr uint32_t total_length = 128;
vector_half src0[total_length];
vector_half src1[total_length];
vector_half dst[total_length];
// mask为源操作数掩码
asc_add(dst, src0, src1, mask);
```