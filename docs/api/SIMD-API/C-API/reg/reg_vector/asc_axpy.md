# asc_axpy

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

根据 mask 对源操作数 src、value 进行按元素做乘加操作，将结果写入目的操作数 dst。

$$
dst_i = src_i \times value + dst_i
$$

## 函数原型
```cpp
__simd_callee__ inline void asc_axpy(vector_half& dst, vector_half src, half value, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_axpy(vector_float& dst, vector_float src, float value, vector_bool mask);
```

## 参数说明
**表 1**  模板参数说明

无模板参数。

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `value` | 输入 | 源操作数（标量）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义](../reg数据类型定义.md)。

## 数据类型支持
| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| `half` | `half` | `half` |
| `float` | `float` | `float` |

## 返回值说明
无返回值。

## 约束说明
- 掩码模式约束：仅支持 ZEROING 模式，mask 未筛选的元素在输出中置零。
- 数据类型约束：仅支持 `half` 和 `float` 类型，`dst`、`src` 与 `value` 数据类型须一致。
- 标量类型约束：`value` 的数据类型须与 `dst`/`src` 的数据类型可互转。
- 异常和边界值处理：`inf`、`nan`、`0`、`MAX`、`MIN` 等特殊值行为遵循 IEEE 754 标准。

## 理论性能

本接口的理论性能数据请参考附录：
- Vector指令理论性能汇总：详见 [Vector指令理论性能汇总](../../../../附录/Vector指令理论性能汇总.md)

## 调用示例
```cpp
vector_half dst;
vector_half src;
half value = 0.0;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr 是外部输入的 UB 内存空间地址。
asc_axpy(dst, src, value, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_axpy.cpp`

