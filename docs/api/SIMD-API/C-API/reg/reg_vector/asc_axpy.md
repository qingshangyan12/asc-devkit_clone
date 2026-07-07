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
- Kirin X90：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：支持
<!-- end id9 -->

## 功能说明

- 接口声明文件：`c_api/reg_compute/reg_vector.h`
- 核心功能：根据 mask 对源操作数 src、value 进行按元素做乘加操作，将结果写入目的操作数 dst。
- 实现原理/公式：

$$
dst_i = src_i \times value + dst_i
$$

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
__simd_callee__ inline void asc_axpy(vector_half& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_axpy(vector_float& dst, vector_float src, float value, vector_bool mask);
```

<!-- end id10 -->

## 参数说明

**表 1**  模板参数说明

无模板参数。

**表 2**  参数说明

<!-- npu="950,x90,9030" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `value` | 输入 | 源操作数（标量）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义.md](../reg数据类型定义.md)。

<!-- end id11 -->

## 数据类型支持

<!-- npu="950,x90,9030" id12 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| `half` | `half` | `half` |
| `float` | `float` | `float` |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

- PIPE_V

## 资源占用情况

- 临时空间占用：不占用 UB/临时空间，运算在矢量数据寄存器上完成。
- 同步资源占用：占用 PIPE_V 流水资源。
- 资源占用与参数/数据量关系：寄存器级运算，资源占用不随参数或数据量变化。

## 约束说明

<!-- npu="950,x90,9030" id13 -->
- 掩码模式约束：仅支持 ZEROING 模式，mask 未筛选的元素在输出中置零。
- 数据类型约束：仅支持 `half` 和 `float` 类型，`dst`、`src` 与 `value` 数据类型须一致。
- 标量类型约束：`value` 的数据类型须与 `dst`/`src` 的数据类型可互转。
- 异常和边界值处理：`inf`、`nan`、`0`、`MAX`、`MIN` 等特殊值行为遵循 IEEE 754 标准。
<!-- end id13 -->

## 理论性能

<!-- npu="950,x90,9030" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| `half` | `half` | 128 |
| `float` | `float` | 64 |

<!-- end id14 -->

## 调用示例

<!-- npu="950,x90,9030" id15 -->

```cpp
vector_half dst;
vector_half src;
half value = 0.0;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr 是外部输入的 UB 内存空间地址。
asc_axpy(dst, src, value, mask);
```

- 完整样例链接：`<TODO>`

<!-- end id15 -->
