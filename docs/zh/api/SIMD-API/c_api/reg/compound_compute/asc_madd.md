# asc_madd

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

## 功能说明

根据mask对目的操作数dst、源操作数src0和src1执行逐元素乘加运算：将dst中的原有元素与src0中的对应元素相乘，再与src1中的对应元素相加，并将结果写回dst，mask未筛选的元素在dst中置零。计算公式如下：

$$
dst_i = dst_i \times src0_i + src1_i
$$

本接口为Reg矢量计算接口，仅在AIV上生效。

## 函数原型

```c
__simd_callee__ inline void asc_madd(vector_half& dst,
                                     vector_half src0,
                                     vector_half src1,
                                     vector_bool mask)

__simd_callee__ inline void asc_madd(vector_float& dst,
                                     vector_float src0,
                                     vector_float src1,
                                     vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输入/输出 | 目的操作数（矢量数据寄存器）。输入时保存参与乘法运算的原始数据，输出时保存乘加结果。 |
| src0 | 输入 | 源操作数（矢量数据寄存器），参与乘法运算。 |
| src1 | 输入 | 源操作数（矢量数据寄存器），参与加法运算。 |
| mask | 输入 | 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素参与乘加运算，未筛选的元素在dst中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口为Reg矢量计算接口，只能在使用`__simd_vf__`标记的VF函数内调用，不支持在`__aicore__`函数中直接调用，仅在AIV上生效，在AIC上调用将直接返回。
- 同一寄存器的数据依赖由硬件保序，无需额外插入同步指令。本接口与前后Reg数据搬运接口之间，如果不同寄存器访问同一UB地址且存在写后读或写后写依赖，需要调用[asc_mem_bar](../sync_control/asc_mem_bar.md)进行同步。
- dst、src0和src1的数据类型需要保持一致，仅支持`half`和`float`。
- 使用mask前，需要通过掩码设置或搬入接口完成初始化；未初始化的掩码寄存器内容不确定。
- 输入包含nan或inf时，结果遵循浮点乘加规则。例如，`nan × 有限数 + 有限数`和`inf × 0 + 有限数`的结果均为nan。

<!-- npu="950" id8 -->
## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

以下调用示例代码仅Ascend 950PR/Ascend 950DT产品支持。

```cpp
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
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
bool CompareData(const std::vector<T>& actual, const std::vector<T>& expected, double tolerance = 0.0)
{
    if (actual.size() != expected.size()) return false;
    for (size_t i = 0; i < actual.size(); ++i) {
        if (actual[i] == expected[i]) continue;
        const double diff = static_cast<double>(actual[i]) - static_cast<double>(expected[i]);
        if (diff > tolerance || diff < -tolerance) return false;
    }
    return true;
}

template <typename T>
bool CompareRangeData(const std::vector<T>& actual, const std::vector<T>& expected,
    size_t begin, size_t count, double tolerance = 0.0)
{
    if (begin + count > actual.size() || begin + count > expected.size()) return false;
    for (size_t i = begin; i < begin + count; ++i) {
        if (actual[i] == expected[i]) continue;
        const double diff = static_cast<double>(actual[i]) - static_cast<double>(expected[i]);
        if (diff > tolerance || diff < -tolerance) return false;
    }
    return true;
}



constexpr uint32_t ELEMENT_COUNT = 64;
constexpr float SCALAR_VALUE = 0.5f;

__simd_vf__ inline void Compute(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1)
{
    vector_float dst_reg;
    vector_float src0_reg;
    vector_float src1_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(dst_reg, dst);
    asc_loadalign(src0_reg, src0);
    asc_loadalign(src1_reg, src1);
    asc_madd(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}

__global__ __vector__ void AscMaddKernel(__gm__ float* dst, __gm__ float* src0, __gm__ float* src1)
{
    asc_init();
    __ubuf__ float dst_local[ELEMENT_COUNT];
    __ubuf__ float src0_local[ELEMENT_COUNT];
    __ubuf__ float src1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(dst_local, dst, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src0_local, src0, ELEMENT_COUNT * sizeof(float));
    asc_copy_gm2ub_align(src1_local, src1, ELEMENT_COUNT * sizeof(float));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Compute(dst_local, src0_local, src1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(float));
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> initial(ELEMENT_COUNT);
    std::vector<float> src0(ELEMENT_COUNT);
    std::vector<float> src1(ELEMENT_COUNT);
    std::vector<float> output(ELEMENT_COUNT);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        initial[i] = 1.0f + static_cast<float>(i) * 0.125f;
        src0[i] = static_cast<float>(i % 8) * 0.25f;
        src1[i] = static_cast<float>((i + 3) % 8) * 0.25f;
        output[i] = initial[i];
        golden[i] = initial[i] * src0[i] + src1[i];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    float* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), (ELEMENT_COUNT) * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    float* src0_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src0_device), (ELEMENT_COUNT) * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    float* src1_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src1_device), (ELEMENT_COUNT) * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(dst_device, initial.size() * sizeof(float), initial.data(), initial.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(src0_device, src0.size() * sizeof(float), src0.data(), src0.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(src1_device, src1.size() * sizeof(float), src1.data(), src1.size() * sizeof(float),
        ACL_MEMCPY_HOST_TO_DEVICE);
    AscMaddKernel<<<1, 0>>>(dst_device, src0_device, src1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), dst_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Initial dst", initial);
    print_data("Input src0", src0);
    print_data("Input src1", src1);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = CompareData(output, golden, 1e-6);
    std::cout << (passed ? "[Success] asc_madd passed." : "[Failed] asc_madd failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src0_device);
    aclrtFree(src1_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
