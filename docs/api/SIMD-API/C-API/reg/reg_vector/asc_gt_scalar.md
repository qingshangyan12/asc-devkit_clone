# asc_gt_scalar

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
- 核心功能：对源操作数执行逐元素大于比较。对于 `src_i > value`，若条件成立则目的操作数对应结果位为 1，否则为 0，每个元素的比较结果占一个 bit。
- 实现原理/公式：

$$
dst_i = \begin{cases} 1, & src_i > value \\ 0, & src_i \leq value \end{cases}
$$

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask)
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask)
```

<!-- end id10 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950,x90,9030" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（掩码寄存器），存储逐元素比较结果，每个元素结果占一个 bit。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `value` | 输入 | 源操作数（标量），数据类型需与 `src` 一致。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。`mask` 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/C-API/reg/reg数据类型定义.md)。

<!-- end id11 -->

## 数据类型支持

<!-- npu="950,x90,9030" id12 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| `vector_uint8_t` | `uint8_t` | `vector_bool` |
| `vector_int8_t` | `int8_t` | `vector_bool` |
| `vector_bfloat16_t` | `bfloat16_t` | `vector_bool` |
| `vector_uint16_t` | `uint16_t` | `vector_bool` |
| `vector_int16_t` | `int16_t` | `vector_bool` |
| `vector_uint32_t` | `uint32_t` | `vector_bool` |
| `vector_int32_t` | `int32_t` | `vector_bool` |
| `vector_half` | `half` | `vector_bool` |
| `vector_float` | `float` | `vector_bool` |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

- `PIPE_V`

## 资源占用情况

- 临时空间占用：不占用 UB/临时空间，操作数均在矢量数据寄存器与掩码寄存器中完成计算。
- 同步资源占用：占用 `PIPE_V` 流水资源。
- 资源占用与参数/数据量关系：资源占用固定，不随参数或数据量变化（寄存器级计算，单次处理一个矢量寄存器长度的数据）。

## 约束说明

<!-- npu="950,x90,9030" id13 -->
- 数据类型约束：`src` 与 `value` 数据类型必须一致。
- mask 约束：`mask` 对应位置为 1 时元素参与比较，为 0 时不参与计算，未筛选元素在输出 `dst` 中置零。
- 浮点比较约束：对于 `half`/`float`/`bfloat16_t` 类型，涉及 `NaN` 时比较结果为 0；涉及 `inf` 时按 IEEE 754 规则比较。
<!-- end id13 -->

## 理论性能

<!-- npu="950,x90,9030" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| `vector_int8_t` | `vector_bool` | 256 |
| `vector_uint8_t` | `vector_bool` | 256 |
| `vector_int16_t` | `vector_bool` | 128 |
| `vector_uint16_t` | `vector_bool` | 128 |
| `vector_half` | `vector_bool` | 128 |
| `vector_bfloat16_t` | `vector_bool` | 128 |
| `vector_int32_t` | `vector_bool` | 64 |
| `vector_uint32_t` | `vector_bool` | 64 |
| `vector_float` | `vector_bool` | 64 |

<!-- end id14 -->

## 调用示例

<!-- npu="950,x90,9030" id15 -->

```cpp
// 对 half 类型的矢量数据寄存器中的元素逐个与标量 value 进行大于比较
// mask 使用 PAT_ALL 表示所有元素均参与比较
vector_bool dst;
vector_half src;
half value = 1.0;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr);  // src_addr 是外部输入的 UB 内存空间地址
asc_gt_scalar(dst, src, value, mask);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/blob/master/tests/api/c_api/npu_arch_3510/vector_compute/test_asc_gt_scalar.cpp`

<!-- end id15 -->