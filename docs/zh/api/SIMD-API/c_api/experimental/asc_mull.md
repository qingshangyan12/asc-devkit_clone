# asc_mull

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

根据掩码将两个源矢量数据寄存器中的有效元素相乘，数据类型位宽为32位的元素相乘得到64位乘积，拆分为低32位与高32位分别写入两个目的矢量数据寄存器。掩码对应位置为1的元素参与计算，为0的元素在目的矢量数据寄存器中置零。计算公式如下：

$$
dst0_i = (src0_i \times src1_i) \& ((1 \ll bit) - 1)
$$

$$
dst1_i = (src0_i \times src1_i) \gg bit
$$

其中，`bit`表示`src0`和`src1`的数据类型位宽，`dst0_i`和`dst1_i`分别表示返回结构体`first`和`second`成员中的第`i`个元素。

**图 1** asc_mull计算示意图

![](../../../figures/asc_mull.png)

本接口为Reg矢量计算接口，仅在AIV上生效。

通过引用参数输出结果的正式接口请参见[asc_mull](../reg_compute/reg_arith/asc_mull.md)。

## 函数原型

```c
// 通过函数返回值返回结果（占位符形式）
__simd_callee__ inline asc_<dtype>_<dtype>_pair asc_mull(vector_<dtype> src0,
                                                         vector_<dtype> src1,
                                                         vector_bool mask)
```

### dtype支持数据类型

`dtype`取值为：`int32_t`、`uint32_t`。

### 函数原型典型示例

```c
// 通过函数返回值返回结果示例：uint32_t类型乘法。
__simd_callee__ inline asc_uint32_t_uint32_t_pair asc_mull(vector_uint32_t src0,
                                                           vector_uint32_t src1,
                                                           vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| src0 | 输入 | 源操作数（矢量数据寄存器）。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。数据类型须与`src0`一致。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示参与计算的元素。对应位置为1时参与计算，为0时不参与计算且`result.first`、`result.second`对应元素置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../defs/type/data_type_definition.md)。

## 返回值说明

返回[双返回值结构体](pair.md)`asc_int32_t_int32_t_pair`或`asc_uint32_t_uint32_t_pair`。使用变量`result`接收返回值时，`result.first`保存64位乘积的低32位，`result.second`保存高32位，两个成员的数据类型均与输入矢量数据寄存器一致。

## 约束说明

### 通用约束

- 使用本接口前，需要包含头文件：`#include "c_api/experimental/reg_arith_exp.h"`。
- 使用前需要增加编译选项`-DCANN_ASC_USE_EXPERIMENTAL`开启试验特性。
- Reg矢量计算C API通用约束请参见[通用约束](../reg_compute/overview.md#通用约束)。
- 掩码位为0的元素位置不参与乘法运算，`result.first`和`result.second`对应位置写0。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng -DCANN_ASC_USE_EXPERIMENTAL example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

本示例处理128个`uint32_t`元素，单次Repeat处理64个元素。Kernel计算`one_repeat_size`和`repeat_time`后，使用`asc_vf_call`启动Vector Function，通过`for`循环完成两次长乘计算。

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "c_api/experimental/reg_arith_exp.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 128;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(uint32_t);

__simd_vf__ inline void mull_vf(__ubuf__ uint32_t* output_low,
                                __ubuf__ uint32_t* output_high,
                                __ubuf__ uint32_t* input0,
                                __ubuf__ uint32_t* input1,
                                uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_uint32_t src0;
    vector_uint32_t src1;
    uint32_t count = ELEMENT_COUNT;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        vector_bool mask = asc_update_mask_b32(count);
        asc_loadalign(src0, input0 + i * one_repeat_size);
        asc_loadalign(src1, input1 + i * one_repeat_size);
        auto result = asc_mull(src0, src1, mask);
        asc_storealign(output_low + i * one_repeat_size, result.first, mask);
        asc_storealign(output_high + i * one_repeat_size, result.second, mask);
    }
}

__global__ __vector__ void asc_mull_kernel(__gm__ uint32_t* output_low,
                                           __gm__ uint32_t* output_high,
                                           __gm__ uint32_t* input0,
                                           __gm__ uint32_t* input1)
{
    asc_init();
    __ubuf__ uint32_t output_low_local[ELEMENT_COUNT];
    __ubuf__ uint32_t output_high_local[ELEMENT_COUNT];
    __ubuf__ uint32_t input0_local[ELEMENT_COUNT];
    __ubuf__ uint32_t input1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input0_local, input0, BUFFER_BYTES);
    asc_copy_gm2ub_align(input1_local, input1, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    uint16_t one_repeat_size = asc_get_vf_len() / sizeof(uint32_t);
    uint16_t repeat_time = ELEMENT_COUNT / one_repeat_size;
    asc_vf_call<mull_vf>(output_low_local, output_high_local, input0_local, input1_local,
                         one_repeat_size, repeat_time);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output_low, output_low_local, BUFFER_BYTES);
    asc_copy_ub2gm_align(output_high, output_high_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint32_t> input0(ELEMENT_COUNT);
    std::vector<uint32_t> input1(ELEMENT_COUNT);
    std::vector<uint32_t> output_low(ELEMENT_COUNT, 0);
    std::vector<uint32_t> output_high(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input0[i] = i + 1;
        input1[i] = 0x10000000U + i;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* input0_device = nullptr;
    uint32_t* input1_device = nullptr;
    uint32_t* output_low_device = nullptr;
    uint32_t* output_high_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input0_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&input1_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_low_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_high_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input0_device, BUFFER_BYTES, input0.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(input1_device, BUFFER_BYTES, input1.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_mull_kernel<<<1, 0>>>(output_low_device, output_high_device, input0_device, input1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output_low.data(), BUFFER_BYTES, output_low_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(output_high.data(), BUFFER_BYTES, output_high_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        const uint64_t expected = static_cast<uint64_t>(input0[i]) * input1[i];
        if (output_low[i] != static_cast<uint32_t>(expected) ||
            output_high[i] != static_cast<uint32_t>(expected >> 32)) {
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_mull completed."
                         : "[Failed] asc_mull output mismatch.")
              << std::endl;
    aclrtFree(input0_device);
    aclrtFree(input1_device);
    aclrtFree(output_low_device);
    aclrtFree(output_high_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
