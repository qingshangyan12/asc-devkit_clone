# asc_half2float

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

头文件为：`#include "c_api/reg_compute/reg_convert.h"`

该接口根据 mask 将 src 中的每个参与计算的 half 类型元素转换为 float 类型，结果写入 dst。由于源操作数与目的操作数类型位宽比为 1:2，读取数据时需要将一个 VL 大小的数据分为两部分，根据参数 src_pos 选择读取 src 索引为偶数的位置或奇数的位置。本接口仅在 AIV 上生效。

$$
dst_i = cast\_f16\_to\_f32(src_i)
$$

## 函数原型

[适用版本：950]

```cpp
// 将half转换为float，返回结果矢量，从源操作数偶数位读取
__simd_callee__ inline vector_float asc_half2float(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

[适用版本：950]

```cpp
// 将half转换为float，返回结果矢量，从源操作数奇数位读取
__simd_callee__ inline vector_float asc_half2float(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

[适用版本：950]

```cpp
// 将half转换为float，结果写入dst，从源操作数偶数位读取
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

[适用版本：950]

```cpp
// 将half转换为float，结果写入dst，从源操作数奇数位读取
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

## 参数说明

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|----------|------|
| dst | 输出 | 目的操作数（矢量数据寄存器）。存储 half 转 float 的结果，类型为 vector_float。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。待转换的 half 类型数据。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。mask 中与元素对应的比特位为 1 时，该元素参与计算；为 0 时，该元素不参与计算。需通过掩码设置接口预先赋值后再传入。物理位置为掩码寄存器。 |
| src_pos | 输入 | 位置选择标签（编译器标签类型），类型为 std::integral_constant<asc_position_mode, asc_position_mode::EVEN> 或 std::integral_constant<asc_position_mode, asc_position_mode::ODD>。取 ASC_POSITION_EVEN 时选择读取源数据索引为偶数的位置；取 ASC_POSITION_ODD 时选择读取源数据索引为奇数的位置。 |

## 数据类型支持

[适用版本：950]

| 输入类型（src） | 位置参数 | 输出类型（dst） |
|-----------------|----------|-----------------|
| vector_half | ASC_POSITION_EVEN | vector_float |
| vector_half | ASC_POSITION_ODD | vector_float |

## 约束说明

- 本接口在非 AIV 上调用直接返回。
- 本接口在 Vector Function（`__simd_vf__` 标记的函数）内调用。
- mask 需通过掩码设置接口预先赋值后再传入，未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- mask 掩码位为 0 时，dst 对应元素置 0。
- 由于源操作数与目的操作数类型位宽比为 1:2，必须通过 src_pos 参数指定读取源操作数索引的偶数位或奇数位。

## 调用示例

以下示例演示 asc_half2float 将 half 类型数据转换为 float 类型，从偶数位读取源数据。

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

constexpr uint32_t BUFFER_BYTES = 256;

// Vector Function内执行half转float
__simd_vf__ inline void convert(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL); // 通过掩码设置接口预先赋值
    vector_float dst;
    vector_half src;
    asc_loadalign(dst, reinterpret_cast<__ubuf__ float*>(output)); // 从UB加载到矢量数据寄存器
    asc_loadalign(src, reinterpret_cast<__ubuf__ half*>(input));
    asc_half2float(dst, src, mask, ASC_POSITION_EVEN); // 从偶数位读取，执行half转float
    asc_storealign(reinterpret_cast<__ubuf__ float*>(output), dst, mask); // 将结果存回UB
}

__global__ __vector__ void asc_half2float_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES]; // Unified Buffer局部空间
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0); // MTE2到Vector流水同步
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    convert(output_local, input_local);
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
| asc_float2half | 将 float 类型转换为 half 类型 | 需要反向转换时使用 asc_float2half |
| asc_exp_sub_half2float | 融合了 half 转 float 与 exp(src0-src1) 运算 | 需要先转 float 再做指数减法时使用融合接口 |
