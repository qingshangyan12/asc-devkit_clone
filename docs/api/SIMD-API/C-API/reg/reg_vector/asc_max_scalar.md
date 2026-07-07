# asc_max_scalar

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明

头文件为：`#include "c_api/reg_compute/reg_vector.h"`

矢量 `src` 的逐个元素与标量 `value` 比较大小，按照掩码 `mask` 指示的比特位将最大值存入 `dst` 中。

$$
dst_i = max(src_i, value)
$$

## 函数原型
```cpp
__simd_callee__ inline void asc_max_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_max_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_max_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_max_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_max_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_max_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_max_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_max_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_max_scalar(vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);
```

## 参数说明

**表1**  模板参数说明


本接口为C API，通过函数重载支持多种数据类型，无模板参数。


**表2**  参数说明


| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器），存储比较后的最大值结果。 |
| `src` | 输入 | 源操作数（矢量数据寄存器），参与比较的矢量数据。 |
| `value` | 输入 | 源操作数（标量），与矢量每个元素比较的标量值，类型须与 `src` 元素类型一致。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask` 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义](../reg数据类型定义.md)。


## 数据类型支持


| 输入A类型 | 输入B类型/标量类型 | 输出类型 |
|---|---|---|
| vector_int8_t | int8_t | vector_int8_t |
| vector_int16_t | int16_t | vector_int16_t |
| vector_int32_t | int32_t | vector_int32_t |
| vector_uint8_t | uint8_t | vector_uint8_t |
| vector_uint16_t | uint16_t | vector_uint16_t |
| vector_uint32_t | uint32_t | vector_uint32_t |
| vector_half | half | vector_half |
| vector_float | float | vector_float |
| vector_bfloat16_t | bfloat16_t | vector_bfloat16_t |


## 返回值说明

无返回值。

## 约束说明

- 地址重叠约束：本接口操作矢量数据寄存器，不涉及内存地址，无地址重叠约束。
- 地址对齐约束：本接口操作矢量数据寄存器，不涉及内存地址，无地址对齐约束。
- 参数组合约束：`dst` 与 `src` 数据类型须一致；`value` 标量类型须与 `src` 元素类型一致。
- 环境影响约束：无。
- 特殊数据类型约束：输入 `src` 为 -0、`value` 为 +0的情况下，输出 `dst` 为 +0。
- 性能约束：无。
- 异常和边界值处理：浮点类型遵循IEEE 754规则；`mask` 未筛选位置的元素在输出中置零。

## 理论性能


| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| vector_int8_t | vector_int8_t | 256 |
| vector_uint8_t | vector_uint8_t | 256 |
| vector_int16_t | vector_int16_t | 128 |
| vector_uint16_t | vector_uint16_t | 128 |
| vector_half | vector_half | 128 |
| vector_bfloat16_t | vector_bfloat16_t | 128 |
| vector_int32_t | vector_int32_t | 64 |
| vector_uint32_t | vector_uint32_t | 64 |
| vector_float | vector_float | 64 |


asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_max_scalar.cpp`


## 调用示例


```cpp
vector_int8_t dst;
vector_int8_t src;
int8_t value = 0;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_max_scalar(dst, src, value, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_max_scalar.cpp`

