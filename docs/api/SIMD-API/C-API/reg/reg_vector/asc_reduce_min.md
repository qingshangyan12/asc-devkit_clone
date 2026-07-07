# asc_reduce_min

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

根据 mask 对源操作数 src 进行归约最小值操作，将结果写入目的操作数 dst。 对 src 中 mask 为 1 的元素求最小值，结果广播写入 dst 的所有有效元素；mask 为 0 的位置在输出中置零。底层对应 `vcmin` 指令（`MODE_ZEROING` 模式）。

$$
\text{result} = \min\{src_i \mid mask_i = 1\}
$$

## 函数原型
```cpp
__simd_callee__ inline void asc_reduce_min(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_reduce_min(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_reduce_min(vector_half& dst, vector_half src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_reduce_min(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_reduce_min(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_reduce_min(vector_float& dst, vector_float src, vector_bool mask)
```

## 参数说明
**表 1**  模板参数说明

无模板参数。

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg数据类型定义.md)。

## 数据类型支持
| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| uint16_t | 不涉及 | uint16_t |
| int16_t | 不涉及 | int16_t |
| half | 不涉及 | half |
| uint32_t | 不涉及 | uint32_t |
| int32_t | 不涉及 | int32_t |
| float | 不涉及 | float |

## 返回值说明
无返回值。

## 约束说明
- 地址重叠约束：操作在矢量数据寄存器上完成，不涉及内存地址重叠。
- 地址对齐约束：不涉及地址对齐（矢量数据寄存器操作）。
- 参数组合约束：无。
- 环境影响约束：无。
- 特殊数据类型约束：min(-0, +0) = -0；如果输入数据存在 nan，将该数据类型的 nan 写入 dst。
- 性能约束：无。
- 异常和边界值处理：当所有元素均不参与计算（mask 全为 0）时，将该数据类型的最大值写入 dst；当存在多个最小值时，将第一个最小值保存在 dst 中。

## 理论性能

本接口的理论性能数据请参考附录：
- Vector指令理论性能汇总：详见 [Vector指令理论性能汇总](../../../../附录/Vector指令理论性能汇总.md)

## 调用示例
```cpp
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr 是外部输入的 UB 内存空间地址。
asc_reduce_min(dst, src, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_reduce_min.cpp`

