# asc_addc

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

头文件为：`#include "c_api/reg_compute/compute/reg_arith.h"`

该接口根据 mask，对源操作数 src0、src1 及输入进位 carry_src 进行按元素求和操作，将结果写入目的操作数 dst，同时将每个元素的进位结果写入 carry（存放进位标志的掩码寄存器）。Carry flag（进位/借位标志）用于表示加法进位或者减法无借位。若 src0、src1、carry_src 输入按位相加后最高位有进位，在 carry 中对应位置每 4bit 的最低位写 1，否则写 0。硬件计算时按照 32 位无符号数处理，矢量数据寄存器写入加法和的低 32 位，目的掩码寄存器写入每个元素加法产生的进位输出。本接口仅在 AIV 上生效。

$$
\{carry_i, dst_i\} = \{1'b0, src0_i\} + \{1'b0, src1_i\} + \{32'b0, carry\_src_i\}
$$

## 函数原型

[适用版本：950]

```cpp
// 对uint32_t类型源操作数进行带进位的加法运算，结果写入dst，进位写入carry
__simd_callee__ inline void asc_addc(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src,
    vector_bool mask)
```

[适用版本：950]

```cpp
// 对int32_t类型源操作数进行带进位的加法运算，结果写入dst，进位写入carry
__simd_callee__ inline void asc_addc(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool carry_src,
    vector_bool mask)
```

## 参数说明

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|----------|------|
| carry | 输出 | 目的操作数（掩码寄存器）。存储加法计算后的进位数据。物理位置为掩码寄存器，起始地址需按照 32 字节对齐。 |
| dst | 输出 | 目的操作数（矢量数据寄存器）。存储加法计算结果。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。加法运算的第一个输入。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。加法运算的第二个输入。物理位置为矢量数据寄存器（TPosition: VREG）。 |
| carry_src | 输入 | 源操作数（掩码寄存器）。输入的进位标志。物理位置为掩码寄存器。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为 1 时参与计算，为 0 时不参与计算。mask 未筛选的元素在输出中置零。需通过掩码设置接口预先赋值后再传入。 |

## 数据类型支持

[适用版本：950]

| 输入 A 类型（src0） | 输入 B 类型（src1） | 进位输入类型（carry_src） | 输出类型（dst） | 进位输出类型（carry） |
|---------------------|---------------------|--------------------------|-----------------|----------------------|
| vector_uint32_t | vector_uint32_t | vector_bool | vector_uint32_t | vector_bool |
| vector_int32_t | vector_int32_t | vector_bool | vector_int32_t | vector_bool |

## 约束说明

- 本接口在非 AIV 上调用直接返回。
- `mask` 需通过掩码设置接口预先赋值后再传入，未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- 运算输出完整计算结果（包含进位位），不受 asc_set_ctrl 影响，硬件不会对输出进行饱和或截断。
- 本接口在 Vector Function（`__simd_vf__` 标记的函数）内调用。

## 调用示例

以下示例演示 asc_addc 对 64 个 uint32_t 元素进行带进位加法运算，src0 取 `UINT32_MAX - i`，src1 取 `i + 2`，每个元素均产生进位。

```cpp
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t CARRY_WORDS = 8;

// Vector Function内执行带进位加法
__simd_vf__ inline void addc_vf(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* carry_out,
    __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1)
{
    vector_uint32_t dst_reg;
    vector_uint32_t src0_reg;
    vector_uint32_t src1_reg;
    vector_bool carry;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count); // 通过掩码设置接口预先赋值
    vector_bool carry_src = mask;
    asc_loadalign(src0_reg, src0);  // 从UB加载数据到矢量数据寄存器
    asc_loadalign(src1_reg, src1);
    asc_addc(carry, dst_reg, src0_reg, src1_reg, carry_src, mask); // 执行带进位加法
    asc_storealign(dst, dst_reg, mask);  // 将结果存回UB
    asc_storealign(carry_out, carry);
}

__global__ __vector__ void asc_addc_kernel(__gm__ uint32_t* dst, __gm__ uint32_t* carry,
    __gm__ uint32_t* src0, __gm__ uint32_t* src1)
{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT];       // Unified Buffer局部空间
    __ubuf__ uint32_t carry_local[CARRY_WORDS];
    __ubuf__ uint32_t src0_local[ELEMENT_COUNT];
    __ubuf__ uint32_t src1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(uint32_t));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);    // MTE2到Vector流水同步
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    addc_vf(dst_local, carry_local, src0_local, src1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);    // Vector到MTE3流水同步
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_copy_ub2gm_align(carry, carry_local, CARRY_WORDS * sizeof(uint32_t));
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
| asc_add | 不带进位输入，仅做两数加法，可选输出进位标志 | 无需进位输入时使用 asc_add |
| asc_subc | 带借位的减法运算 | 需要减法运算时使用 asc_subc |
