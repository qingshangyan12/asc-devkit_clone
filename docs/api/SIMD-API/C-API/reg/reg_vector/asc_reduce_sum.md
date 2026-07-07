# asc_reduce_sum

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
- 核心功能：归约求和，用于将 `src` 中所有参与计算的元素求和，得到的结果保存在 `dst` 中。
- 实现原理/公式：在掩码 `mask` 控制下，对源操作数 `src` 中对应比特位为 1 的元素执行累加求和，累加结果写入目的操作数 `dst`；`mask` 未筛选（对应比特位为 0）的元素在输出中置零。

## 函数原型

<!-- npu="950" id10 -->

```cpp
__simd_callee__ inline void asc_reduce_sum(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_float& dst, vector_float src, vector_bool mask);
```

<!-- end id10 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器），用于保存归约求和结果。 |
| `src` | 输入 | 源操作数（矢量数据寄存器），参与求和的元素来源。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。`mask` 未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义](../reg数据类型定义.md)。

<!-- end id11 -->

## 数据类型支持

<!-- npu="950" id12 -->

| 源操作数 src 类型 | 目的操作数 dst 类型 |
|---|---|
| vector_int16_t | vector_int32_t |
| vector_int32_t | vector_int32_t |
| vector_uint16_t | vector_uint32_t |
| vector_uint32_t | vector_uint32_t |
| vector_half | vector_half |
| vector_float | vector_float |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

- PIPE_V

## 资源占用情况

- 临时空间占用：不占用 UB/临时空间，源操作数与目的操作数均为矢量数据寄存器，计算在寄存器内完成。
- 同步资源占用：占用 PIPE_V 流水资源。
- 资源占用与参数/数据量关系：资源占用固定，不随参数或数据量变化。

## 约束说明

<!-- npu="950" id13 -->
- 操作数类型约束：`dst` 与 `src` 的数据类型组合须满足数据类型支持表中的对应关系，`int16_t`/`uint16_t` 类型的源操作数归约结果分别存入 `int32_t`/`uint32_t` 类型的目的操作数，其余类型源操作数与目的操作数类型一致。
- mask 行为约束：`mask` 对应比特位为 1 的元素参与求和，为 0 的元素不参与计算且在输出中置零。
- 累加顺序约束：指令内累加顺序采用二叉树累加方式，两两相加逐层归约。
- 异常和边界值处理：当所有元素均不参与计算（mask 全为 0）时，将 0 写入 dst。
- 寄存器约束：`dst`、`src` 必须为矢量数据寄存器，`mask` 必须为掩码寄存器，不可传入 UB 等存储空间地址。
<!-- end id13 -->

## 理论性能

<!-- npu="950" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| vector_int16_t | vector_int32_t | 64 |
| vector_uint16_t | vector_uint32_t | 64 |
| vector_half | vector_half | 128 |
| vector_int32_t | vector_int32_t | 64 |
| vector_uint32_t | vector_uint32_t | 64 |
| vector_float | vector_float | 64 |

<!-- end id14 -->

## 调用示例

<!-- npu="950" id15 -->

```cpp
// 对 half 类型的源操作数执行归约求和
// mask 选用 PAT_ALL，表示所有元素均参与计算
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr 是外部输入的 UB 内存空间地址
asc_reduce_sum(dst, src, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_reduce_sum.cpp`

<!-- end id15 -->
