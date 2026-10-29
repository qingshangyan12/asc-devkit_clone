# asc_deintlv

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

头文件路径为：`"c_api/experimental/reg_permute_sel_exp.h"`。

本接口为试验接口，在后续版本中可能会调整或移除，不保证后续兼容性。编译时需定义`CANN_ASC_USE_EXPERIMENTAL`宏，并显式包含指定的试验头文件。

将src0和src1中的元素解交织存入`result.first`和`result.second`中，本接口提供矢量数据解交织和掩码解交织两种功能模式：

- **矢量数据解交织**：对两个源矢量数据寄存器中的有效元素逐对解交织。src0的偶数索引元素写入`result.first`前半部分，奇数索引元素写入`result.second`前半部分。src1的偶数索引元素写入`result.first`后半部分，奇数索引元素写入`result.second`后半部分。
- **掩码解交织**：根据接口后缀指定的粒度，对两个源掩码寄存器中的比特位分组后进行解交织。`asc_deintlv_b8`以1bit为一组，`asc_deintlv_b16`以2bit为一组，`asc_deintlv_b32`以4bit为一组，组内比特位的顺序保持不变。分组从低位到高位按0开始编号。src0的偶数索引分组写入`result.first`前半部分，奇数索引分组写入`result.second`前半部分。src1的偶数索引分组写入`result.first`后半部分，奇数索引分组写入`result.second`后半部分。

以int8_t数据类型为例，asc_deintlv的实现流程如图1所示：

**图1** 解交织实现流程

![解交织实现流程](../figures/capi_deintlv_reg.png)

本接口为Reg矢量计算接口，仅在AIV上生效。

通过引用参数输出结果的正式接口请参见[asc_deintlv](../reg_compute/reg_permute_sel/asc_deintlv.md)。

## 函数原型

### 矢量数据寄存器解交织模式

```c
// 通过函数返回值返回结果（占位符形式）
__simd_callee__ inline asc_<dtype>_<dtype>_pair asc_deintlv(vector_<dtype> src0,
                                                            vector_<dtype> src1)
```

#### dtype支持数据类型

`dtype`取值为：`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

#### 函数原型典型示例

```c
// 通过函数返回值返回结果示例：对half矢量数据寄存器执行解交织。
__simd_callee__ inline asc_half_half_pair asc_deintlv(vector_half src0,
                                                      vector_half src1)
```

### 掩码寄存器解交织模式

```c
// 通过函数返回值返回结果
__simd_callee__ inline asc_bool_bool_pair asc_deintlv_b8(vector_bool src0,
                                                         vector_bool src1)

__simd_callee__ inline asc_bool_bool_pair asc_deintlv_b16(vector_bool src0,
                                                          vector_bool src1)

__simd_callee__ inline asc_bool_bool_pair asc_deintlv_b32(vector_bool src0,
                                                          vector_bool src1)
```

## 参数说明

**表1** 参数说明（操作数为矢量数据寄存器）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| src0 | 输入 | 源矢量数据寄存器，参与解交织的操作数。`dtype`须与`src1`一致。其偶数索引元素写入`result.first`前半部分，奇数索引元素写入`result.second`前半部分。 |
| src1 | 输入 | 源矢量数据寄存器，参与解交织的操作数。`dtype`须与`src0`一致。其偶数索引元素写入`result.first`后半部分，奇数索引元素写入`result.second`后半部分。 |

**表2** 参数说明（操作数为掩码寄存器）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| src0 | 输入 | 源掩码寄存器，参与解交织的操作数。其偶数索引分组写入`result.first`前半部分，奇数索引分组写入`result.second`前半部分。 |
| src1 | 输入 | 源掩码寄存器，参与解交织的操作数。其偶数索引分组写入`result.first`后半部分，奇数索引分组写入`result.second`后半部分。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../defs/type/data_type_definition.md)。

## 返回值说明

返回[双返回值结构体](pair.md)，使用变量`result`接收返回值。

- 操作数为矢量数据寄存器时，返回类型为`asc_<dtype>_<dtype>_pair`，两个成员的类型均为`vector_<dtype>`。`result.first`保存解交织后的偶数下标元素集合，`result.second`保存奇数下标元素集合。
- 操作数为掩码寄存器时，b8、b16和b32模式的返回类型均为`asc_bool_bool_pair`，两个成员的类型均为`vector_bool`，分别保存对应粒度下解交织后的偶数索引分组和奇数索引分组。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../reg_compute/overview.md#通用约束)。
- `src0`和`src1`的数据类型必须一致，可以为同一个矢量数据寄存器或掩码寄存器。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id10 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng -DCANN_ASC_USE_EXPERIMENTAL example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id10 -->

本示例处理两组各128个`float`元素，单次Repeat分别读取两个源寄存器各64个元素。Kernel计算`one_repeat_size`和`repeat_time`后，使用`asc_vf_call`启动Vector Function，循环执行两次解交织，Host按每次Repeat的范围生成预期结果。

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "c_api/experimental/reg_permute_sel_exp.h"
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
bool compare_data(const std::vector<T>& actual, const std::vector<T>& expected, double tolerance = 0.0)
{
    if (actual.size() != expected.size()) return false;
    for (size_t i = 0; i < actual.size(); ++i) {
        if (actual[i] == expected[i]) continue;
        const double diff = static_cast<double>(actual[i]) - static_cast<double>(expected[i]);
        if (diff > tolerance || diff < -tolerance) return false;
    }
    return true;
}

constexpr uint32_t ELEMENT_COUNT = 128;
__simd_vf__ inline void rearrange(__ubuf__ float* dst0, __ubuf__ float* dst1,
    __ubuf__ float* src0, __ubuf__ float* src1,
    uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_float src0_reg;
    vector_float src1_reg;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);
    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadalign(src0_reg, src0 + i * one_repeat_size);
        asc_loadalign(src1_reg, src1 + i * one_repeat_size);
        auto result = asc_deintlv(src0_reg, src1_reg);
        asc_storealign(dst0 + i * one_repeat_size, result.first, mask);
        asc_storealign(dst1 + i * one_repeat_size, result.second, mask);
    }
}
__global__ __vector__ void asc_deintlv_kernel(__gm__ float* dst0, __gm__ float* dst1,
    __gm__ float* src0, __gm__ float* src1)
{
    asc_init();
    __ubuf__ float dst0_local[ELEMENT_COUNT], dst1_local[ELEMENT_COUNT];
    __ubuf__ float src0_local[ELEMENT_COUNT], src1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    uint16_t one_repeat_size = asc_get_vf_len() / sizeof(float);
    uint16_t repeat_time = ELEMENT_COUNT / one_repeat_size;
    asc_vf_call<rearrange>(dst0_local, dst1_local, src0_local, src1_local,
                           one_repeat_size, repeat_time);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst0, dst0_local, ELEMENT_COUNT * sizeof(float));
    asc_copy_ub2gm_align(dst1, dst1_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> src0(ELEMENT_COUNT), src1(ELEMENT_COUNT);
    std::vector<float> output0(ELEMENT_COUNT, 0), output1(ELEMENT_COUNT, 0);
    std::vector<float> golden0(ELEMENT_COUNT), golden1(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src0[i] = static_cast<float>(i);
        src1[i] = 1000.0f + i;
    }
    const uint32_t one_repeat_size = 256 / sizeof(float);
    for (uint32_t repeat = 0; repeat < ELEMENT_COUNT / one_repeat_size; ++repeat) {
        const uint32_t base = repeat * one_repeat_size;
        for (uint32_t offset = 0; offset < one_repeat_size / 2; ++offset) {
            golden0[base + offset] = src0[base + 2 * offset];
            golden1[base + offset] = src0[base + 2 * offset + 1];
            golden0[base + one_repeat_size / 2 + offset] = src1[base + 2 * offset];
            golden1[base + one_repeat_size / 2 + offset] = src1[base + 2 * offset + 1];
        }
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    float* src0_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src0_device), (ELEMENT_COUNT) * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    float* src1_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src1_device), (ELEMENT_COUNT) * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    float* dst0_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst0_device), (ELEMENT_COUNT) * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    float* dst1_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst1_device), (ELEMENT_COUNT) * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src0_device, src0.size() * sizeof(float), src0.data(), src0.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(src1_device, src1.size() * sizeof(float), src1.data(), src1.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_deintlv_kernel<<<1, 0>>>(dst0_device, dst1_device, src0_device, src1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output0.data(), output0.size() * sizeof(float), dst0_device, output0.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(output1.data(), output1.size() * sizeof(float), dst1_device, output1.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input 0", src0);
    print_data("Input 1", src1);
    print_data("Output 0", output0);
    print_data("Golden 0", golden0);
    print_data("Output 1", output1);
    print_data("Golden 1", golden1);
    const bool passed = compare_data(output0, golden0) && compare_data(output1, golden1);
    std::cout << (passed ? "[Success] asc_deintlv passed." : "[Failed] asc_deintlv failed.") << std::endl;
    aclrtFree(dst0_device);
    aclrtFree(dst1_device);
    aclrtFree(src0_device);
    aclrtFree(src1_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
