# asc_int322int64

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
- Kirin X90：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：支持
<!-- end id9 -->

## 功能说明

- 接口声明文件：`c_api/reg_compute/reg_convert.h`
- 核心功能：将 vector_int32_t 类型的源操作数以 256B 为单位分为两部分，读取其中一部分元素，将其转换成 vector_int64_t 类型并写入目的操作数。
  - asc_int322int64：将源操作数的上半部分写入目的操作数。
  - asc_int322int64_v2：将源操作数的下半部分写入目的操作数。
- 实现原理/公式：int32_t 到 int64_t 为符号扩展的位宽放大转换，无精度损失；mask 未筛选的元素在输出中置零。

$$
dst_i = (int64\_t)src_i
$$

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
__simd_callee__ inline void asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask);
__simd_callee__ inline void asc_int322int64_v2(vector_int64_t& dst, vector_int32_t src, vector_bool mask);
```

<!-- end id10 -->

## 参数说明

本接口无模板参数。

**表 1**  参数说明

<!-- npu="950,x90,9030" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。 |

<!-- end id11 -->

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义.md](../reg数据类型定义.md)。

## 数据类型支持

<!-- npu="950,x90,9030" id12 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| int32_t | 不涉及 | int64_t |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

- PIPE_V

## 资源占用情况

- 临时空间占用：不占用 UB/临时空间，源和目的操作数均为矢量数据寄存器。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：资源占用不随参数或数据量变化。

## 约束说明

<!-- npu="950,x90,9030" id13 -->
- 地址重叠约束：源和目的操作数为矢量数据寄存器，不涉及地址重叠问题。
- 地址对齐约束：不涉及地址对齐要求。
- 参数组合约束：无。
- 环境影响约束：不影响全局状态。
- 特殊数据类型约束：无。
- 性能约束：无。
- 异常和边界值处理：mask 未筛选的元素在输出中置零。
<!-- end id13 -->

## 理论性能

<!-- npu="950,x90,9030" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| int32_t | int64_t | 32 |

<!-- end id14 -->

## 调用示例

<!-- npu="950,x90,9030" id15 -->

```cpp
// 将 vector_int32_t 源操作数的上半部分/下半部分转换为 vector_int64_t 类型并写入 dst
vector_int64_t dst;
vector_int32_t src;
vector_bool mask;
asc_int322int64(dst, src, mask);     // 将 src 的上半部分转换成 vector_int64_t 类型并写入 dst
asc_int322int64_v2(dst, src, mask);  // 将 src 的下半部分转换成 vector_int64_t 类型并写入 dst
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/blob/master/tests/api/c_api/npu_arch_3510/vector_compute/test_vconv_instr.cpp`

<!-- end id15 -->
