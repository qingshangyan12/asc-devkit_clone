# asc_add_carry

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/experimental/reg_arith_exp.h"`。

本接口为试验接口，在后续版本中可能会调整或移除，不保证后续兼容性。编译时需定义`CANN_ASC_USE_EXPERIMENTAL`宏，并显式包含指定的试验头文件。

该接口根据`mask`，对`src0`和`src1`进行按元素加法运算，并通过函数返回值同时返回32位加法结果和进位标志。

接口支持不带输入进位和带输入进位两种模式。Carry flag（进位标志）用于表示加法是否产生最高位向外的进位，不表示有符号溢出。接口通过`result.second`（掩码寄存器）保存各元素的输出进位标志，每个32位数据元素对应一组4bit，该组的最低位保存进位标志：有进位时为1，否则为0。带输入进位时，输出进位由`src0`、`src1`及对应元素的输入进位共同相加后产生。

不带输入进位时，计算公式如下：

$$
\{carry_i, result_i\} = \{1'b0, src0_i\} + \{1'b0, src1_i\}
$$

带输入进位时，额外加入对应元素的输入进位（0或1），计算公式如下：

$$
\{carry_i, result_i\} = \{1'b0, src0_i\} + \{1'b0, src1_i\} + \{32'b0, carry\_src_i\}
$$

本接口为Reg矢量计算接口，仅在AIV上生效。

## 函数原型

```c
// 通过函数返回值返回结果（占位符形式），不带输入进位。
__simd_callee__ inline asc_<dtype>_bool_pair asc_add_carry(vector_<dtype> src0,
                                                           vector_<dtype> src1,
                                                           vector_bool mask)

// 通过函数返回值返回结果（占位符形式），带输入进位。
__simd_callee__ inline asc_<dtype>_bool_pair asc_add_carry(vector_<dtype> src0,
                                                           vector_<dtype> src1,
                                                           vector_bool carry_src,
                                                           vector_bool mask)
```

### dtype支持数据类型

`dtype`取值为：`int32_t`、`uint32_t`。

### 函数原型典型示例

```c
// 通过函数返回值返回结果示例：不带输入进位的uint32_t类型加法。
__simd_callee__ inline asc_uint32_t_bool_pair asc_add_carry(vector_uint32_t src0,
                                                            vector_uint32_t src1,
                                                            vector_bool mask)

// 通过函数返回值返回结果示例：带输入进位的uint32_t类型加法。
__simd_callee__ inline asc_uint32_t_bool_pair asc_add_carry(vector_uint32_t src0,
                                                            vector_uint32_t src1,
                                                            vector_bool carry_src,
                                                            vector_bool mask)
```

## 参数说明

**表1** 不带输入进位版本参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| src0 | 输入 | 源操作数（矢量数据寄存器）。数据类型为`int32_t`或`uint32_t`。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算，未选中的元素在输出中置零。 |

**表2** 带输入进位版本参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| src0 | 输入 | 源操作数（矢量数据寄存器）。数据类型为`int32_t`或`uint32_t`。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。 |
| carry_src | 输入 | 输入进位标志（掩码寄存器），每个32位元素对应4bit中的最低位。该位为1时额外加1，为0时不增加输入进位。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算，未选中的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../defs/type/data_type_definition.md)。

## 返回值说明

返回结构体的公共定义请参见[双返回值结构体](pair.md)。

通过函数返回值输出结果的函数原型返回公共结果结构体，返回类型为`asc_<dtype>_bool_pair`。例如使用变量`result`接收函数返回值，`result.first`数据类型与输入矢量数据寄存器一致，为`vector_int32_t`或`vector_uint32_t`；`result.second`数据类型为`vector_bool`。相同dtype的加法和减法进位接口共用该结构体类型。

## 约束说明

- 使用本接口前，需要包含头文件：`#include "c_api/experimental/reg_arith_exp.h"`。
- 使用前需要增加编译选项`-DCANN_ASC_USE_EXPERIMENTAL`开启试验特性。
- Reg矢量计算C API通用约束请参见[通用约束](../reg_compute/overview.md#通用约束)。
- 运算输出包含低32位加法结果及进位标志，不受[asc_set_ctrl](../spr/asc_set_ctrl.md)影响，硬件不会对结果进行饱和处理。
- 进位输出按32位无符号加法产生，不表示有符号溢出。不带输入进位时，将`src0`和`src1`的对应元素按无符号数相加；带输入进位时，还需加上`carry_src`中对应元素的输入进位（0或1）。若和值超过`0xFFFFFFFF`，则输出进位标志为1，否则为0。
- `result.first`仅保存和值的低32位，超出32位的部分通过`result.second`的进位输出体现。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng -DCANN_ASC_USE_EXPERIMENTAL example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include "c_api/asc_simd.h"
#include "c_api/experimental/reg_arith_exp.h"
#include "acl/acl.h"

namespace {
template <typename T>
void print_data(const char* label, const std::vector<T>& values)
{
    std::cout << label << ":";
    const size_t count = values.size() < 8 ? values.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << +values[i];
    if (values.size() > count) std::cout << " ...";
    std::cout << std::endl;
}

template <typename T>
bool compare_data(const std::vector<T>& actual, const std::vector<T>& expected)
{
    return actual == expected;
}

constexpr uint32_t ELEMENT_COUNT = 128;
constexpr uint32_t CARRY_WORDS = ELEMENT_COUNT / 8;

__simd_vf__ inline void add_carry_vf(__ubuf__ uint32_t* dst,
                                     __ubuf__ uint32_t* carry_out,
                                     __ubuf__ uint32_t* src0,
                                     __ubuf__ uint32_t* src1,
                                     uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_uint32_t src0_reg;
    vector_uint32_t src1_reg;
    uint32_t count = ELEMENT_COUNT;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        vector_bool mask = asc_update_mask_b32(count);
        asc_loadalign(src0_reg, src0 + i * one_repeat_size);
        asc_loadalign(src1_reg, src1 + i * one_repeat_size);
        auto result = asc_add_carry(src0_reg, src1_reg, mask);
        asc_storealign(dst + i * one_repeat_size, result.first, mask);
        asc_storealign(carry_out + i * (one_repeat_size / 8), result.second);
    }
}

__global__ __vector__ void asc_add_carry_kernel(__gm__ uint32_t* dst,
                                                __gm__ uint32_t* carry,
                                                __gm__ uint32_t* src0,
                                                __gm__ uint32_t* src1)
{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT];
    __ubuf__ uint32_t carry_local[CARRY_WORDS];
    __ubuf__ uint32_t src0_local[ELEMENT_COUNT];
    __ubuf__ uint32_t src1_local[ELEMENT_COUNT];

    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(uint32_t));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    uint16_t one_repeat_size = asc_get_vf_len() / sizeof(uint32_t);
    uint16_t repeat_time = ELEMENT_COUNT / one_repeat_size;
    asc_vf_call<add_carry_vf>(dst_local, carry_local, src0_local, src1_local,
                            one_repeat_size, repeat_time);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_copy_ub2gm_align(carry, carry_local, CARRY_WORDS * sizeof(uint32_t));
    asc_sync();
}

} // namespace

int main()
{
    std::vector<uint32_t> src0(ELEMENT_COUNT);
    std::vector<uint32_t> src1(ELEMENT_COUNT);
    std::vector<uint32_t> output(ELEMENT_COUNT, 0);
    std::vector<uint32_t> carry(CARRY_WORDS, 0);
    std::vector<uint32_t> golden(ELEMENT_COUNT, 0);
    std::vector<uint32_t> golden_carry(CARRY_WORDS, 0);

    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src0[i] = i % 2 == 0 ? std::numeric_limits<uint32_t>::max() - i : i;
        src1[i] = i + 2;
        const uint64_t sum = static_cast<uint64_t>(src0[i]) + src1[i];
        golden[i] = static_cast<uint32_t>(sum);
        golden_carry[i / 8] |= static_cast<uint32_t>(sum >> 32) << (4 * (i % 8));
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* src0_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src0_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint32_t* src1_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src1_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint32_t* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint32_t* carry_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&carry_device), (CARRY_WORDS) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src0_device, src0.size() * sizeof(uint32_t), src0.data(), src0.size() * sizeof(uint32_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(src1_device, src1.size() * sizeof(uint32_t), src1.data(), src1.size() * sizeof(uint32_t),
        ACL_MEMCPY_HOST_TO_DEVICE);

    asc_add_carry_kernel<<<1, 0>>>(dst_device, carry_device, src0_device, src1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint32_t), dst_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(carry.data(), carry.size() * sizeof(uint32_t), carry_device, carry.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);

    print_data("Input src0", src0);
    print_data("Input src1", src1);
    print_data("Output", output);
    print_data("Carry words", carry);
    print_data("Golden output", golden);
    print_data("Golden carry words", golden_carry);
    const bool passed = compare_data(output, golden) && compare_data(carry, golden_carry);
    std::cout << (passed ? "[Success] asc_add_carry passed." : "[Failed] asc_add_carry failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(carry_device);
    aclrtFree(src0_device);
    aclrtFree(src1_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
