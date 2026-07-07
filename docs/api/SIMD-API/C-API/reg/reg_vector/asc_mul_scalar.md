# asc_mul_scalar

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
- 核心功能：对源操作数 src 与标量 value 进行按元素乘法操作，将结果写入目的操作数 dst。
- 实现原理/公式：

$$
dst = src * value
$$

## 函数原型

<!-- npu="950" id10 -->

```cpp
__simd_callee__ inline void asc_mul_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);
```

<!-- end id10 -->

## 参数说明

无模板参数。

**表 1**  参数说明

<!-- npu="950" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器），用于存放计算结果。 |
| `src` | 输入 | 源操作数（矢量数据寄存器），参与乘法计算的矢量数据。 |
| `value` | 输入 | 源操作数（标量），与 src 中每个元素相乘的标量值。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义.md](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/C-API/reg/reg数据类型定义.md)。

<!-- end id11 -->

## 数据类型支持

<!-- npu="950" id12 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| vector_int16_t | int16_t | vector_int16_t |
| vector_uint16_t | uint16_t | vector_uint16_t |
| vector_half | half | vector_half |
| vector_int32_t | int32_t | vector_int32_t |
| vector_uint32_t | uint32_t | vector_uint32_t |
| vector_float | float | vector_float |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

- `PIPE_V`

## 资源占用情况

- 临时空间占用：不占用 UB/临时空间，计算直接在矢量数据寄存器上完成。
- 同步资源占用：占用 PIPE_V 矢量计算流水线资源。
- 资源占用与参数/数据量关系：寄存器资源占用固定，不随数据量变化。

## 约束说明

<!-- npu="950" id13 -->
- 地址重叠约束：源操作数与目的操作数为不同的矢量数据寄存器，不涉及地址重叠。
- 地址对齐约束：不涉及内存地址，无对齐要求。
- 参数组合约束：无。
- 环境影响约束：无。
- 特殊数据类型约束：无。
- 性能约束：无。
- 异常和边界值处理：对于浮点类型（half/float），遵循 IEEE 754 标准处理 `inf`、`nan`、`0` 等特殊值；对于整型类型，遵循整数溢出回绕语义。
- mask 行为约束：mask 对应位置为 0 的元素不参与计算，且在输出中置零。
<!-- end id13 -->

## 理论性能

<!-- npu="950" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| vector_int16_t | vector_int16_t | 64 |
| vector_uint16_t | vector_uint16_t | 64 |
| vector_half | vector_half | 128 |
| vector_int32_t | vector_int32_t | 32 |
| vector_uint32_t | vector_uint32_t | 32 |
| vector_float | vector_float | 64 |

<!-- end id14 -->

## 调用示例

<!-- npu="950" id15 -->

```cpp
// 对 vector_half 类型的矢量数据与标量进行按元素乘法
vector_half src;
vector_half dst;
half value = 1.0;
vector_bool mask = asc_create_mask_b16(PAT_ALL);  // 全部元素参与计算
asc_loadalign(src, src_addr);  // src_addr 是外部输入的 UB 内存空间地址
asc_mul_scalar(dst, src, value, mask);
```

- 完整样例链接：`https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/02_reg_vector_compute/muls`

<!-- end id15 -->
