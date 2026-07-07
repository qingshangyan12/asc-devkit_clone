# asc_add_scalar

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
- 核心功能：对源矢量操作数 `src` 与标量 `value` 执行逐元素加法运算，将结果写入目的矢量操作数 `dst`。计算过程固定采用 ZEROING 模式：未被 `mask` 选中（`mask` 对应位为 0）的元素在目的操作数中置 0。
- 实现原理/公式：

$$
dst_i = src_i + value
$$

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
__simd_callee__ inline void asc_add_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);
__simd_callee__ inline void asc_add_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);
__simd_callee__ inline void asc_add_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);
__simd_callee__ inline void asc_add_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);
__simd_callee__ inline void asc_add_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);
__simd_callee__ inline void asc_add_scalar(vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);
__simd_callee__ inline void asc_add_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);
__simd_callee__ inline void asc_add_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
__simd_callee__ inline void asc_add_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);
```

<!-- end id10 -->

## 参数说明

**表 1**  模板参数说明

<!-- npu="950,x90,9030" id11 -->
本接口无模板参数，通过函数重载支持 int8_t/uint8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float 数据类型。
<!-- end id11 -->

**表 2**  参数说明

<!-- npu="950,x90,9030" id12 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器），存储加法结果。 |
| `src` | 输入 | 源操作数（矢量数据寄存器），参与加法运算的矢量。 |
| `value` | 输入 | 源操作数（标量），与 `src` 逐元素相加，数据类型需与 `src`/`dst` 一致。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。`mask` 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

<!-- end id12 -->

## 数据类型支持

<!-- npu="950,x90,9030" id13 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| int8_t | int8_t | int8_t |
| uint8_t | uint8_t | uint8_t |
| int16_t | int16_t | int16_t |
| uint16_t | uint16_t | uint16_t |
| half | half | half |
| bfloat16_t | bfloat16_t | bfloat16_t |
| int32_t | int32_t | int32_t |
| uint32_t | uint32_t | uint32_t |
| float | float | float |

<!-- end id13 -->

## 返回值说明

无返回值。

## 流水类型

PIPE_V

## 资源占用情况

- 临时空间占用：不占用额外 UB/临时空间，计算在矢量数据寄存器中完成。
- 同步资源占用：占用 Vector 流水（PIPE_V）执行资源。
- 资源占用与参数/数据量关系：资源占用与参与计算的元素数量（由 `mask` 决定）相关，不随标量 `value` 取值变化。

## 约束说明

<!-- npu="950,x90,9030" id14 -->
- mask 模式约束：本接口固定采用 ZEROING 模式，未被 `mask` 选中的元素在目的操作数 `dst` 中置 0，不支持保留原值。
- 数据类型约束：标量 `value` 的数据类型必须与 `src`、`dst` 的矢量数据类型一致，由函数重载匹配。
- 执行位置约束：接口仅在 AIV（Vector Core）上生效。
<!-- end id14 -->

## 理论性能

<!-- npu="950,x90,9030" id15 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| int8_t | int8_t | 128 |
| uint8_t | uint8_t | 128 |
| int16_t | int16_t | 64 |
| uint16_t | uint16_t | 64 |
| half | half | 64 |
| bfloat16_t | bfloat16_t | 64 |
| int32_t | int32_t | 32 |
| uint32_t | uint32_t | 32 |
| float | float | 32 |

<!-- end id15 -->

## 调用示例

<!-- npu="950,x90,9030" id16 -->

```cpp
// 对 half 类型的矢量 src 逐元素加上标量 scalar_val，结果写入 dst
vector_half dst;
vector_half src;
half scalar_val = 0.0f;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr);  // src_addr 是外部输入的 UB 内存空间地址
asc_add_scalar(dst, src, scalar_val, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_add_scalar.cpp`

<!-- end id16 -->
