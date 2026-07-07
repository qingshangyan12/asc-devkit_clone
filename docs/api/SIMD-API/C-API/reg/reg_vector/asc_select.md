# asc_select

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
- 核心功能：根据 mask 的比特位值，从源操作数 src0、src1 中选择元素，得到目的操作数 dst。当 mask 的比特位为 1 时，从 src0 中选取对应位置的元素；当 mask 的比特位为 0 时，从 src1 中选取对应位置的元素。
- 实现原理/公式：

$$
dst_i = (mask_i == 1)\ ?\ src0_i : src1_i
$$

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
__simd_callee__ inline void asc_select(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_hifloat8_t& dst, vector_hifloat8_t src0, vector_hifloat8_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)

__simd_callee__ inline void asc_select(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
```

<!-- end id10 -->

## 参数说明

本接口为 C 语言重载接口，无模板参数。

**表 1**  参数说明

<!-- npu="950,x90,9030" id11 -->

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| `dst` | 输出 | 目的操作数（掩码寄存器或矢量数据寄存器）。 |
| `src0` | 输入 | 源操作数 0（掩码寄存器或矢量数据寄存器）。 |
| `src1` | 输入 | 源操作数 1（掩码寄存器或矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器）。指定选择 src0 或 src1 为有效数据。mask 的比特位为 1 时，选取 src0；mask 的比特位为 0 时，选取 src1。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义.md](../reg数据类型定义.md)。

<!-- end id11 -->

## 数据类型支持

mask 参数类型固定为 `vector_bool`，src0、src1、dst 三者数据类型一致。

<!-- npu="950,x90,9030" id12 -->

| 输入 A 类型 (src0) | 输入 B 类型 (src1) | 输出类型 (dst) |
| :----------------- | :----------------- | :------------- |
| vector_bool | vector_bool | vector_bool |
| vector_uint8_t | vector_uint8_t | vector_uint8_t |
| vector_int8_t | vector_int8_t | vector_int8_t |
| vector_fp8_e4m3fn_t | vector_fp8_e4m3fn_t | vector_fp8_e4m3fn_t |
| vector_hifloat8_t | vector_hifloat8_t | vector_hifloat8_t |
| vector_fp8_e5m2_t | vector_fp8_e5m2_t | vector_fp8_e5m2_t |
| vector_uint16_t | vector_uint16_t | vector_uint16_t |
| vector_int16_t | vector_int16_t | vector_int16_t |
| vector_half | vector_half | vector_half |
| vector_bfloat16_t | vector_bfloat16_t | vector_bfloat16_t |
| vector_uint32_t | vector_uint32_t | vector_uint32_t |
| vector_int32_t | vector_int32_t | vector_int32_t |
| vector_float | vector_float | vector_float |

<!-- end id12 -->

## 返回值说明

无返回值。

## 流水类型

PIPE_V

## 资源占用情况

- 临时空间占用：不占用 UB 临时空间，操作直接在矢量数据寄存器或掩码寄存器上完成。
- 同步资源占用：占用 PIPE_V（矢量计算流水）资源。
- 资源占用与参数/数据量关系：资源占用固定，与参与计算的数据量无关。

## 约束说明

<!-- npu="950,x90,9030" id13 -->
- 无特殊约束。
<!-- end id13 -->

## 理论性能

<!-- npu="950,x90,9030" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| vector_bool | vector_bool | 256 |
| vector_int8_t | vector_int8_t | 256 |
| vector_uint8_t | vector_uint8_t | 256 |
| vector_fp8_e4m3fn_t | vector_fp8_e4m3fn_t | 256 |
| vector_hifloat8_t | vector_hifloat8_t | 256 |
| vector_fp8_e5m2_t | vector_fp8_e5m2_t | 256 |
| vector_int16_t | vector_int16_t | 128 |
| vector_uint16_t | vector_uint16_t | 128 |
| vector_half | vector_half | 128 |
| vector_bfloat16_t | vector_bfloat16_t | 128 |
| vector_int32_t | vector_int32_t | 64 |
| vector_uint32_t | vector_uint32_t | 64 |
| vector_float | vector_float | 64 |

<!-- end id14 -->

## 调用示例

<!-- npu="950,x90,9030" id15 -->

```cpp
vector_half dst;
vector_half src0;
vector_half src1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src0, src0_addr); // src0_addr 是外部输入的 UB 内存空间地址。
asc_loadalign(src1, src1_addr); // src1_addr 是外部输入的 UB 内存空间地址。
// mask 为源操作数掩码，指定选择 src0 或 src1 为有效数据
asc_select(dst, src0, src1, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_select.cpp`

<!-- end id15 -->
