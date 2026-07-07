# asc_int322float

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
- 核心功能：将 int32_t 类型数据转换为 float 类型，并支持多种舍入模式。当 int32_t 的值超出 float 可精确表示的整数范围时，按所选舍入模式进行舍入。
- 实现原理/公式：

$$
dst_i = (float)src_i
$$

关于舍入模式的详细说明，请参见 [舍入模式](./rounding_mode.md)。

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
// RINT 舍入模式（四舍六入五成双）
__simd_callee__ inline void asc_int322float_rn(vector_float& dst, vector_int32_t src, vector_bool mask);
// ROUND 舍入模式（四舍五入）
__simd_callee__ inline void asc_int322float_rna(vector_float& dst, vector_int32_t src, vector_bool mask);
// FLOOR 舍入模式（向负无穷方向舍入）
__simd_callee__ inline void asc_int322float_rd(vector_float& dst, vector_int32_t src, vector_bool mask);
// CEIL 舍入模式（向正无穷方向舍入）
__simd_callee__ inline void asc_int322float_ru(vector_float& dst, vector_int32_t src, vector_bool mask);
// TRUNC 舍入模式（向零方向舍入）
__simd_callee__ inline void asc_int322float_rz(vector_float& dst, vector_int32_t src, vector_bool mask);
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
| int32_t | 不涉及 | float |

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
- 特殊数据类型约束：当 int32_t 的值超出 float 可精确表示的整数范围（绝对值大于 2^24）时，按函数后缀指定的舍入模式进行舍入。
- 性能约束：无。
- 异常和边界值处理：mask 未筛选的元素在输出中置零。
<!-- end id13 -->

## 理论性能

<!-- npu="950,x90,9030" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| int32_t | float | 32 |

<!-- end id14 -->

## 调用示例

<!-- npu="950,x90,9030" id15 -->

```cpp
// 将 int32_t 类型的矢量数据寄存器转换为 float 类型（RINT 舍入模式）
vector_int32_t src;
vector_float dst;
vector_bool mask = asc_create_mask_b32(PAT_ALL); // 全部元素参与计算
asc_loadalign(src, src_addr);                    // src_addr 是外部输入的 UB 内存空间地址
asc_int322float_rn(dst, src, mask);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/blob/master/tests/api/c_api/npu_arch_3510/vector_compute/test_asc_int322float.cpp`

<!-- end id15 -->

