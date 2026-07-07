# asc_ge_scalar

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
- 核心功能：ge（greater than or equal to），对源矢量操作数 `src` 与标量 `value` 执行逐元素比较。对于 `src_i >= value`，若条件成立则目的操作数 `dst` 对应比特位为 1，否则为 0，每个元素的比较结果占一个 bit。`dst` 为掩码寄存器，按掩码 `mask` 指示参与计算的元素。
- 实现原理/公式：

$$
dst_i = (src_i \geq value)
$$

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);
```

<!-- end id10 -->

## 参数说明

**表 1**  模板参数说明

<!-- npu="950,x90,9030" id11 -->
本接口无模板参数，通过函数重载支持 uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float 数据类型。
<!-- end id11 -->

**表 2**  参数说明

<!-- npu="950,x90,9030" id12 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（掩码寄存器），存储逐元素比较结果，每个元素的比较结果占一个 bit。 |
| `src` | 输入 | 源操作数（矢量数据寄存器），参与比较的矢量数据。 |
| `value` | 输入 | 源操作数（标量），与矢量每个元素比较的标量值，类型须与 `src` 元素类型一致。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。`mask` 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

<!-- end id12 -->

## 数据类型支持

<!-- npu="950,x90,9030" id13 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| vector_uint8_t | uint8_t | vector_bool |
| vector_int8_t | int8_t | vector_bool |
| vector_uint16_t | uint16_t | vector_bool |
| vector_int16_t | int16_t | vector_bool |
| vector_half | half | vector_bool |
| vector_bfloat16_t | bfloat16_t | vector_bool |
| vector_uint32_t | uint32_t | vector_bool |
| vector_int32_t | int32_t | vector_bool |
| vector_float | float | vector_bool |

<!-- end id13 -->

## 返回值说明

无返回值。

## 流水类型

PIPE_V

## 资源占用情况

- 临时空间占用：不占用额外 UB/临时空间，计算在矢量数据寄存器与掩码寄存器上完成。
- 同步资源占用：占用 Vector 流水（PIPE_V）执行资源。
- 资源占用与参数/数据量关系：固定开销，单条指令对整个矢量寄存器操作，资源占用不随数据量变化。

## 约束说明

<!-- npu="950,x90,9030" id14 -->
- mask 模式约束：本接口固定采用 ZEROING 模式，未被 `mask` 选中的元素在目的操作数 `dst` 中置 0，不支持保留原值。
- 数据类型约束：标量 `value` 的数据类型必须与 `src` 的矢量数据类型一致，由函数重载匹配；`dst` 与 `mask` 均为掩码寄存器类型 `vector_bool`。
- 执行位置约束：接口仅在 AIV（Vector Core）上生效。
- 异常和边界值处理：浮点类型（`half`/`float`/`bfloat16_t`）比较遵循 IEEE 754 规则，当 `src_i` 为 NaN 时 `src_i >= value` 结果为 0（false）。
<!-- end id14 -->

## 理论性能

<!-- npu="950,x90,9030" id15 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| vector_int8_t | vector_bool | 256 |
| vector_uint8_t | vector_bool | 256 |
| vector_int16_t | vector_bool | 128 |
| vector_uint16_t | vector_bool | 128 |
| vector_half | vector_bool | 128 |
| vector_bfloat16_t | vector_bool | 128 |
| vector_int32_t | vector_bool | 64 |
| vector_uint32_t | vector_bool | 64 |
| vector_float | vector_bool | 64 |

<!-- end id15 -->

## 调用示例

<!-- npu="950,x90,9030" id16 -->

```cpp
// 对 half 类型的矢量 src 逐元素比较是否大于等于标量 value，结果写入掩码寄存器 dst
vector_bool dst;
vector_half src;
half value = 0.0;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr);  // src_addr 是外部输入的 UB 内存空间地址
asc_ge_scalar(dst, src, value, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_ge_scalar.cpp`

<!-- end id16 -->
