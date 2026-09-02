# asc_exp_sub

<!-- 标签说明 -->
> - ![待补充](https://img.shields.io/badge/待补充-red)：该章节内容待后续补充

> **已废弃**，替代方案请使用 asc_exp_sub_half2float（带 src_pos 参数版本）或 asc_exp_sub（float 输入版本）。

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持
- Kirin X90：不支持
- Kirin 9030：不支持

## 功能说明

头文件为：`#include "c_api/reg_compute/compute/reg_fused.h"`

该接口将 src0 与 src1 相减，差值作为 e 的指数计算，结果写入目的操作数矢量数据寄存器 dst。i 为元素索引。当 src0 和 src1 为 float 类型时，直接对 float 进行减法和指数运算；当 src0 和 src1 为 half 类型时，先将 half 转换为 float 再进行计算。

- 当 src0 和 src1 为 float 类型时：

$$
dst_i = e^{src0_i - src1_i}
$$

- 当 src0 和 src1 为 half 类型时：

$$
dst_i = e^{cast\_f16\_to\_f32(src0_i) - cast\_f16\_to\_f32(src1_i)}
$$

## 函数原型

[适用版本：950]

```cpp
// float类型输入，执行 exp(src0 - src1) 计算，结果写入dst
__simd_callee__ inline void asc_exp_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
```

[适用版本：950]

```cpp
// float类型输入，执行 exp(src0 - src1) 计算，返回结果矢量
__simd_callee__ inline vector_float asc_exp_sub(vector_float src0, vector_float src1, vector_bool mask)
```

[适用版本：950]

```cpp
// half类型输入，先转换为float再执行 exp(src0 - src1) 计算，从源操作数偶数位读取，结果写入dst
__simd_callee__ inline void asc_exp_sub_half2float(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

[适用版本：950]

```cpp
// half类型输入，先转换为float再执行 exp(src0 - src1) 计算，从源操作数奇数位读取，结果写入dst
__simd_callee__ inline void asc_exp_sub_half2float(
    vector_float& dst, vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

[适用版本：950]

```cpp
// half类型输入，先转换为float再执行 exp(src0 - src1) 计算，从源操作数偶数位读取，返回结果矢量
__simd_callee__ inline vector_float asc_exp_sub_half2float(
    vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

[适用版本：950]

```cpp
// half类型输入，先转换为float再执行 exp(src0 - src1) 计算，从源操作数奇数位读取，返回结果矢量
__simd_callee__ inline vector_float asc_exp_sub_half2float(
    vector_half src0, vector_half src1, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

以下原型已废弃，请使用带 src_pos 参数的 asc_exp_sub_half2float 替代：

```cpp
// 已废弃：half类型输入不带src_pos参数，默认从偶数位读取
[[deprecated]] __simd_callee__ inline void asc_exp_sub(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask)

// 已废弃：half类型输入，从奇数位读取
[[deprecated]] __simd_callee__ inline void asc_exp_sub_v2(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask)

// 已废弃：float类型输入，从奇数位读取
[[deprecated]] __simd_callee__ inline void asc_exp_sub_v2(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
```

## 参数说明

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|----------|------|
| dst | 输出 | 目的操作数（矢量数据寄存器）。存储 exp(src0 - src1) 计算结果，类型为 vector_float。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。指数运算的被减数输入。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。指数运算的减数输入。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。物理位置为掩码寄存器。 |
| src_pos | 输入 | 位置选择标签（编译器标签类型），类型为 std::integral_constant<asc_position_mode, asc_position_mode::EVEN> 或 std::integral_constant<asc_position_mode, asc_position_mode::ODD>。取 ASC_POSITION_EVEN 时选择读取源数据索引为偶数的位置；取 ASC_POSITION_ODD 时选择读取源数据索引为奇数的位置。仅 half 输入的 asc_exp_sub_half2float 版本需要此参数。 |

## 数据类型支持

[适用版本：950]

| 输入 A 类型（src0） | 输入 B 类型（src1） | 位置参数 | 输出类型（dst） |
|---------------------|---------------------|----------|-----------------|
| vector_float | vector_float | 无 | vector_float |
| vector_half | vector_half | ASC_POSITION_EVEN | vector_float |
| vector_half | vector_half | ASC_POSITION_ODD | vector_float |

## 约束说明

- 本接口在非 AIV 上调用直接返回。
- 本接口在 Vector Function（`__simd_vf__` 标记的函数）内调用。
- `mask` 需通过掩码设置接口预先赋值后再传入，未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- 使用 half 输入时，由于源操作数与目的操作数类型位宽比为 1:2，需通过 src_pos 参数指定读取源操作数索引的偶数位或奇数位。
- 已废弃的 asc_exp_sub（half 输入不带 src_pos）和 asc_exp_sub_v2 接口请避免在新代码中使用，替代方案为 asc_exp_sub_half2float（带 src_pos 参数）。

## 调用示例

以下示例演示 asc_exp_sub 对 float 类型矢量数据寄存器执行 exp(src0 - src1) 计算。

```cpp
#include "c_api/asc_simd.h"

// Vector Function内执行exp(src0 - src1)运算
__simd_vf__ inline void exp_sub_vf(__ubuf__ float* output,
                                   __ubuf__ float* src0_buf,
                                   __ubuf__ float* src1_buf)
{
    vector_float src0;
    vector_float src1;
    vector_float dst;
    vector_bool mask = asc_create_mask_b32(PAT_ALL); // 通过掩码设置接口预先赋值
    asc_loadalign(src0, src0_buf);  // 从UB加载数据到矢量数据寄存器
    asc_loadalign(src1, src1_buf);
    asc_exp_sub(dst, src0, src1, mask); // 执行exp(src0 - src1)运算
    asc_storealign(output, dst, mask);   // 将结果存回UB
}
```

以下示例演示使用 half 输入的 asc_exp_sub_half2float，从偶数位读取源数据。

```cpp
#include "c_api/asc_simd.h"

// half输入，先转float再执行exp(src0 - src1)，从偶数位读取
__simd_vf__ inline void exp_sub_half2float_vf(__ubuf__ float* output,
                                              __ubuf__ half* src0_buf,
                                              __ubuf__ half* src1_buf)
{
    vector_half src0;
    vector_half src1;
    vector_float dst;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);
    asc_loadalign(src0, src0_buf);
    asc_loadalign(src1, src1_buf);
    asc_exp_sub_half2float(dst, src0, src1, mask, ASC_POSITION_EVEN); // 从偶数位读取
    asc_storealign(output, dst, mask);
}
```

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 `dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

## 关联API

| 相似API | 与当前API区别 | 选择建议 |
|---------|-------------|---------|
| asc_exp | 仅计算 e 的 src 次方，无减法操作 | 不需要减法时使用 asc_exp |
| asc_sub | 仅执行减法，无指数运算 | 仅需减法结果时使用 asc_sub |
| asc_exp_sub_half2float | 带 src_pos 参数的 half 转 float 版本 | half 输入时推荐使用此接口替代废弃的 asc_exp_sub |
