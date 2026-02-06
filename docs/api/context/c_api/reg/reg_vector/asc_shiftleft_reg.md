# asc_shiftleft

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| <term>Atlas A5 训练系列产品/Atlas A5 推理系列产品</term> | √ |

## 功能说明

根据掩码mask对输入数据src0，按照src1对应元素进行左移操作，完成后将结果写入dst中。

## 函数原型

```cpp
__simd_callee__ inline void asc_shiftleft(vector_uint8_t& dst, vector_uint8_t src0, vector_int8 src1, vector_bool mask)
__simd_callee__ inline void asc_shiftleft(vector_int8& dst, vector_int8 src0, vector_int8 src1, vector_bool mask)
__simd_callee__ inline void asc_shiftleft(vector_uint16_t& dst, vector_uint16_t src0, vector_int16 src1, vector_bool mask)
__simd_callee__ inline void asc_shiftleft(vector_int16& dst, vector_int16 src0, vector_int16 src1, vector_bool mask)
__simd_callee__ inline void asc_shiftleft(vector_uint32_t& dst, vector_uint32_t src0, vector_int32 src1, vector_bool mask)
__simd_callee__ inline void asc_shiftleft(vector_int32& dst, vector_int32 src0, vector_int32 src1, vector_bool mask)
__simd_callee__ inline void asc_shiftleft(vector_uint64_t& dst, vector_uint64_t src0, vector_int64 src1, vector_bool mask)
__simd_callee__ inline void asc_shiftleft(vector_int64& dst, vector_int64 src0, vector_int64 src1, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。|
| src0 | 输入 | 源操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。|
| src1 | 输入 | 源操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。|
| mask | 输入 | 源操作数掩码。对应位置为1时参与计算，为0时不参与计算。<br>&bull; ZEROING模式：mask未筛选的元素在输出中置零。<br>&bull; MERGING模式：暂不支持。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- dst与src0的数据类型应该保持一致，src1不支持负数。
- dst1、src0、src1的需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../../../docs/api/context/c_api/general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
// total_length 指参与计算的数据长度
```