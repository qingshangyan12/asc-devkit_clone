# asc_arange

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

以传入的value为起始值，生成递增/递减的索引，并将生成的索引保存在dst中。算法逻辑表示如下：
```
// 递增
{value, value + 1, value + 2, ... value + VL_T - 2, value + VL_T - 1}
// 递减
{value + VL_T - 1, value + VL_T - 2, value + VL_T - 3, ... value + 1, value}
```

以int16_t数据类型，起始值value=10为例：递增索引为{10, 11, 12, 13, ... 135, 136, 137}，递减索引为{137, 136, 135, 134, ... 12, 11, 10}。

## 函数原型

<!-- npu="950" id2 -->
```cpp
// 递增模式
__simd_callee__ inline void asc_arange(vector_int8_t& dst, int8_t value)
__simd_callee__ inline void asc_arange(vector_int16_t& dst, int16_t value)
__simd_callee__ inline void asc_arange(vector_half& dst, half value)
__simd_callee__ inline void asc_arange(vector_int32_t& dst, int32_t value)
__simd_callee__ inline void asc_arange(vector_float& dst, float value)
// 递减模式
__simd_callee__ inline void asc_arange_descend(vector_int8_t& dst, int8_t value)
__simd_callee__ inline void asc_arange_descend(vector_int16_t& dst, int16_t value)
__simd_callee__ inline void asc_arange_descend(vector_half& dst, half value)
__simd_callee__ inline void asc_arange_descend(vector_int32_t& dst, int32_t value)
__simd_callee__ inline void asc_arange_descend(vector_float& dst, float value)
```
<!-- end id2 -->

## 参数说明

**表 2**  参数说明

<!-- npu="950" id3 -->
| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `value` | 输入 | 源操作数（标量）。 |
<!-- end id3 -->

矢量数据寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 数据类型支持

<!-- npu="950" id4 -->
目的操作数与源操作数的数据类型需要保持一致。支持的数据类型为：`vector_int8_t`、`vector_int16_t`、`vector_half`、`vector_int32_t`、`vector_float`。
<!-- end id4 -->

## 返回值说明

无

## 流水类型

PIPE_V

## 理论性能

| 数据类型 | 理论并行度（element/cycle） | 
|---|---|
| `vector_int8_t` | 128 |
| `vector_int16_t` | 64 |
| `vector_half` | 128 |
| `vector_int32_t` | 32 |
| `vector_float` | 64 |

## 约束说明

<!-- npu="950" id5 -->
对于整型数据类型，如果生成的索引发生溢出，结果将环绕。
<!-- end id5 -->

## 调用示例

<!-- npu="950" id6 -->
```cpp
vector_int8_t dst;
int8_t value;
asc_arange(dst, value);
```
<!-- end id6 -->
