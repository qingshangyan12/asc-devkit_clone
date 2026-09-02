# asc_prelu

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

头文件为：`#include "c_api/reg_compute/compute/reg_fused.h"`

该接口根据掩码对源操作数矢量数据寄存器 src0 和 src1 中的对应元素执行 PReLU（Parametric ReLU）运算，并将结果写入目的操作数矢量数据寄存器 dst。src0 大于 0 时直接写入 src0，否则将 src0 与 src1 相乘后写入 dst。掩码对应位置为 1 的元素参与计算，为 0 的元素在目的矢量数据寄存器中置零。本接口仅在 AIV 上生效。

$$
dst_i = \begin{cases}
src0_i, & src0_i > 0 \\
src0_i \times src1_i, & src0_i \leq 0
\end{cases}
$$

## 函数原型

[适用版本：950]

```cpp
// float类型PReLU运算，结果写入dst
__simd_callee__ inline void asc_prelu(vector_float& dst, vector_float src0, vector_float alpha, vector_bool mask)
```

[适用版本：950]

```cpp
// half类型PReLU运算，结果写入dst
__simd_callee__ inline void asc_prelu(vector_half& dst, vector_half src0, vector_half alpha, vector_bool mask)
```

[适用版本：950]

```cpp
// float类型PReLU运算，返回结果矢量
__simd_callee__ inline vector_float asc_prelu(vector_float src0, vector_float alpha, vector_bool mask)
```

[适用版本：950]

```cpp
// half类型PReLU运算，返回结果矢量
__simd_callee__ inline vector_half asc_prelu(vector_half src0, vector_half alpha, vector_bool mask)
```

## 参数说明

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|----------|------|
| dst | 输出 | 目的操作数（矢量数据寄存器）。存储 PReLU 计算结果。数据类型须与 src0、alpha 一致。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。PReLU 的输入数据。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| alpha | 输入 | 源操作数（矢量数据寄存器）。PReLU 的斜率参数，当 src0 小于等于 0 时与 src0 相乘。数据类型须与 src0 一致。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| mask | 输入 | 掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。物理位置为掩码寄存器。 |

## 数据类型支持

[适用版本：950]

| 输入 A 类型（src0） | 输入 B 类型（alpha） | 输出类型（dst） |
|---------------------|----------------------|-----------------|
| vector_half | vector_half | vector_half |
| vector_float | vector_float | vector_float |

## 约束说明

- 非 AIV 调用直接返回。
- 本接口在 Vector Function（`__simd_vf__` 标记的函数）内调用。
- `mask` 需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- 掩码位为 0 的元素位置不参与 PReLU 运算，dst 对应位置写 0。
- 源矢量数据寄存器中数据为负零（-0）时按负数处理，乘法满足 IEEE 754 浮点乘法规则。

## 调用示例

以下示例演示 asc_prelu 对 64 个 float 元素执行 PReLU 运算，斜率参数 slope 取 0.1。

```cpp
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(float);

// Vector Function内执行PReLU运算
__simd_vf__ inline void prelu_vf(__ubuf__ float* output,
                                 __ubuf__ float* input,
                                 __ubuf__ float* slope)
{
    vector_float src0;
    vector_float src1;
    vector_float dst;
    vector_bool mask = asc_create_mask_b32(PAT_ALL); // 通过掩码设置接口预先赋值
    asc_loadalign(src0, input);   // 从UB加载数据到矢量数据寄存器
    asc_loadalign(src1, slope);
    asc_prelu(dst, src0, src1, mask); // 执行PReLU运算
    asc_storealign(output, dst, mask); // 将结果存回UB
}

__global__ __vector__ void asc_prelu_kernel(__gm__ float* output,
                                           __gm__ float* input,
                                           __gm__ float* slope)
{
    asc_init();
    __ubuf__ float output_local[ELEMENT_COUNT]; // Unified Buffer局部空间
    __ubuf__ float input_local[ELEMENT_COUNT];
    __ubuf__ float slope_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(slope_local, slope, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0); // MTE2到Vector流水同步
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    prelu_vf(output_local, input_local, slope_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0); // Vector到MTE3流水同步
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}

int main()
{
    // ... 初始化ACL、分配设备内存、拷贝数据、启动kernel、校验结果
    // 完整示例请参考 asc-devkit 仓库
}
```

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 `dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

## 关联API

| 相似API | 与当前API区别 | 选择建议 |
|---------|-------------|---------|
| asc_relu | 无斜率参数，负数直接置零 | 不需要可学习斜率参数时使用 asc_relu |
| asc_leakyrelu | 使用标量斜率而非矢量斜率 | 斜率为标量常数时使用 asc_leakyrelu |
