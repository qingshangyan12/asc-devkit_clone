# asc_min_scalar

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

- 接口声明文件：`c_api/reg_compute/reg_vector.h`
- 核心功能：源操作数矢量内每个元素与标量比较，取较小值写入目的操作数。当元素比标量大时取标量值，比标量小时取源操作数值。
- 实现原理/公式：

$$
dst_i = min(src_i, value)
$$

## 函数原型

<!-- npu="950" id10 -->

```cpp
__simd_callee__ inline void asc_min_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);
__simd_callee__ inline void asc_min_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
__simd_callee__ inline void asc_min_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);
__simd_callee__ inline void asc_min_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);
__simd_callee__ inline void asc_min_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);
__simd_callee__ inline void asc_min_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);
__simd_callee__ inline void asc_min_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);
__simd_callee__ inline void asc_min_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);
```

<!-- end id10 -->

## 参数说明

本接口无模板参数。

**表 1**  参数说明

<!-- npu="950" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器），存储逐元素取最小值的结果。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `value` | 输入 | 源操作数（标量），与矢量各元素逐个比较。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg数据类型定义.md)。

<!-- end id11 -->

## 数据类型支持

<!-- npu="950" id12 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| vector_int32_t | int32_t | vector_int32_t |
| vector_uint32_t | uint32_t | vector_uint32_t |
| vector_int16_t | int16_t | vector_int16_t |
| vector_uint16_t | uint16_t | vector_uint16_t |
| vector_int8_t | int8_t | vector_int8_t |
| vector_uint8_t | uint8_t | vector_uint8_t |
| vector_float | float | vector_float |
| vector_half | half | vector_half |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

PIPE_V

## 资源占用情况

- 临时空间占用：不占用 UB/临时空间，操作数均为矢量数据寄存器。
- 同步资源占用：占用矢量流水（PIPE_V）资源。
- 资源占用与参数/数据量关系：资源占用固定，不随参数或数据量变化。

## 约束说明

<!-- npu="950" id13 -->
- 地址重叠约束：无地址重叠约束，操作数均为矢量数据寄存器。
- 地址对齐约束：无地址对齐约束，操作数均为矢量数据寄存器。
- 参数组合约束：`dst`、`src`、`value` 的数据类型必须一致（同一重载）。
- 环境影响约束：无全局状态影响。
- 特殊数据类型约束：输入 `src` 为 -0、`value` 为 +0 的情况下，输出 `dst` 为 -0。
- 性能约束：无。
- 异常和边界值处理：浮点类型参与计算时遵循 IEEE 754 标准；`mask` 未选中的元素在输出中置零。
<!-- end id13 -->

## 理论性能

<!-- npu="950" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| vector_int8_t | vector_int8_t | 256 |
| vector_uint8_t | vector_uint8_t | 256 |
| vector_int16_t | vector_int16_t | 128 |
| vector_uint16_t | vector_uint16_t | 128 |
| vector_half | vector_half | 128 |
| vector_int32_t | vector_int32_t | 64 |
| vector_uint32_t | vector_uint32_t | 64 |
| vector_float | vector_float | 64 |

<!-- end id14 -->

## 调用示例

<!-- npu="950" id15 -->

```cpp
vector_half dst;
vector_half src;
half value = 0;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_min_scalar(dst, src, value, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_min_scalar.cpp`

<!-- end id15 -->
