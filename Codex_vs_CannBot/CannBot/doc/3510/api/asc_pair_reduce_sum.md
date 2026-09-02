# asc_pair_reduce_sum

<!-- 标签说明 -->
> - ![待补充](https://img.shields.io/badge/待补充-red)：该章节内容待后续补充

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

头文件为：`#include "c_api/reg_compute/compute/reg_reduce.h"`

该接口将传入的源操作数 src 中每两个相邻数值相加，并将相加的结果连续保存在目的操作数 dst 中的低半部分。以 half 数据类型为例，相加示意图如下：源操作数中相邻的两个元素 a1 和 a2 相加得到 b1，a3 和 a4 相加得到 b2，以此类推，结果连续保存在 dst 的低半部分，高半部分填充为 0。本接口仅在 AIV 上生效。

$$
dst_i = src_{2i} + src_{2i+1}
$$

## 函数原型

[适用版本：950]

```cpp
// half类型相邻元素求和，结果写入dst
__simd_callee__ inline void asc_pair_reduce_sum(vector_half& dst, vector_half src, vector_bool mask)
```

[适用版本：950]

```cpp
// float类型相邻元素求和，结果写入dst
__simd_callee__ inline void asc_pair_reduce_sum(vector_float& dst, vector_float src, vector_bool mask)
```

[适用版本：950]

```cpp
// half类型相邻元素求和，返回结果矢量
__simd_callee__ inline vector_half asc_pair_reduce_sum(vector_half src, vector_bool mask)
```

[适用版本：950]

```cpp
// float类型相邻元素求和，返回结果矢量
__simd_callee__ inline vector_float asc_pair_reduce_sum(vector_float src, vector_bool mask)
```

## 参数说明

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|----------|------|
| dst | 输出 | 目的操作数（矢量数据寄存器）。存储相邻元素求和结果。求和后仅前一半元素为有效数据，后一半填充为 0。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。待求和的输入数据。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算，不参与计算的元素被当作 0 处理。需通过掩码设置接口预先赋值后再传入。物理位置为掩码寄存器。 |

## 数据类型支持

[适用版本：950]

| 输入类型（src） | 输出类型（dst） |
|-----------------|-----------------|
| vector_half | vector_half |
| vector_float | vector_float |

## 约束说明

- mask 指定源操作数是否参与计算，不参与计算的元素被当作 0 进行累加。例如：若连续两个元素 a、b 均不参与计算，目的操作数结果为 0；若连续两个元素 a、b 仅元素 a 参与计算，目的操作数结果为 a；若连续两个元素 a、b 仅元素 b 参与计算，目的操作数结果为 b。
- 求和后，dst 中仅前一半元素为有效数据，后一半填充为 0。搬出至 Unified Buffer（UB）时需要避免数据踩踏。
- 本接口在非 AIV 上调用直接返回。
- 本接口在 Vector Function（`__simd_vf__` 标记的函数）内调用。
- mask 需通过掩码设置接口预先赋值后再传入，未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。

## 调用示例

以下示例演示 asc_pair_reduce_sum 对 half 类型矢量数据寄存器执行相邻元素求和。

```cpp
#include "c_api/asc_simd.h"

// Vector Function内执行相邻元素求和
__simd_vf__ inline void pair_reduce_sum_vf(__ubuf__ half* output, __ubuf__ half* input)
{
    vector_half dst;
    vector_half src;
    vector_bool mask = asc_create_mask_b16(PAT_ALL); // 通过掩码设置接口预先赋值
    asc_loadalign(src, input);  // 从UB加载数据到矢量数据寄存器
    asc_pair_reduce_sum(dst, src, mask); // 执行相邻元素求和
    asc_storealign(output, dst, mask);    // 将结果存回UB
}
```

以下示例演示使用返回值版本进行相邻元素求和。

```cpp
#include "c_api/asc_simd.h"

__simd_vf__ inline void pair_reduce_sum_ret_vf(__ubuf__ half* output, __ubuf__ half* input)
{
    vector_half src;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    asc_loadalign(src, input);
    vector_half dst = asc_pair_reduce_sum(src, mask); // 返回值版本
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
| asc_reduce_sum | 对整个矢量数据寄存器所有元素求和，结果为标量 | 需要全量归约时使用 asc_reduce_sum |
| asc_reduce_max | 对所有元素求最大值 | 需要求最大值时使用 asc_reduce_max |
