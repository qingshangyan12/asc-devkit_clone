# asc_ln

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

- 接口声明文件：`c_api/reg_compute/reg_vector.h`
- 核心功能：按元素取自然对数，根据 mask 指示对源操作数中参与计算的元素求自然对数，将结果写入目的操作数。
- 实现原理/公式：

$$
dst_i = ln(src_i)
$$

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/C-API/reg/reg数据类型定义.md)。

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
__simd_callee__ inline void asc_ln(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_ln(vector_float& dst, vector_float src, vector_bool mask);
```

<!-- end id10 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950,x90,9030" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。用于存储计算结果，数据类型需与 `src` 保持一致。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。参与自然对数计算的源数据。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。 |

<!-- end id11 -->

## 数据类型支持

<!-- npu="950,x90,9030" id12 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| half | - | half |
| float | - | float |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

PIPE_V

## 资源占用情况

- 临时空间占用：该接口基于矢量数据寄存器进行计算，不占用 UB 等临时空间。
- 同步资源占用：占用 PIPE_V 流水资源。
- 资源占用与参数/数据量关系：资源占用固定，不随参数或数据量变化。

## 约束说明

<!-- npu="950,x90,9030" id13 -->
- 该接口基于矢量数据寄存器和掩码寄存器进行计算，不涉及 UB 地址访问，无地址对齐和地址重叠约束。
- mask 采用 ZEROING 模式，mask 未筛选的元素在输出 `dst` 中置零。
- 目的操作数 `dst` 与源操作数 `src` 的数据类型必须保持一致，仅支持 half、float 数据类型。
- 源操作数 `src` 中元素取值应为正数；对非正数（含 0、负数、inf、nan）取自然对数的行为未定义。
<!-- end id13 -->

## 理论性能

<!-- npu="950,x90,9030" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| half | half | 16 |
| float | float | 16 |

<!-- end id14 -->

## 调用示例

<!-- npu="950,x90,9030" id15 -->

```cpp
// 对 half 类型的源数据按元素求自然对数
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL); // 所有元素参与计算
asc_loadalign(src, src_addr); // src_addr 是外部输入的 UB 内存空间地址
asc_ln(dst, src, mask);
```

- 完整样例链接：`<TODO>`

<!-- end id15 -->
