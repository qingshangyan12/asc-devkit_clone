# asc_cumulative_histogram

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

该接口用于对直方图数据进行累计统计。

如下图所示，在低位模式（BIN0）下，目的寄存器dst会统计源寄存器src中落在低位区间[0, 127]的数据分布情况；在高位模式（BIN1）下，目的寄存器dst则会统计src中落在高位区间[128, 255]的数据分布情况。dst中的第n个元素表示src中从0到n的所有数值在对应区间中出现的总频率。最终，统计结果会在目的寄存器原始数据的基础上进行累加。

![累计统计](../../../../figures/asc_cumulative_histogram.png)

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

```c
__simd_callee__ inline void asc_cumulative_histogram_bin0(vector_uint16_t& dst,
                                                          vector_uint8_t src,
                                                          vector_bool mask)
__simd_callee__ inline void asc_cumulative_histogram_bin1(vector_uint16_t& dst,
                                                          vector_uint8_t src,
                                                          vector_bool mask)
```

dtype支持的数据类型为`bool`、`uint8_t`、`uint16_t`。

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- dst的dtype必须为`uint16_t`，src的dtype必须为`uint8_t`，mask必须为`vector_bool`掩码寄存器；其他dtype触发数据类型异常。
- mask的第j个比特位控制`src[j]`是否参与统计：比特位为1时参与统计，为0时忽略该元素。
- 本次统计结果累加到dst对应元素的原值上，多次调用可累积跨批次数据。dst元素的最大值为65535，使用时需注意累加溢出。

### BIN0模式约束

对dst的第i个元素（i ∈ {0,1,...,127}），本次调用累加mask有效且满足`src[j] ≤ i`的源元素个数。

### BIN1模式约束

对dst的第i个元素（i ∈ {0,1,...,127}），本次调用累加mask有效且满足`128 <= src[j] ≤ 128 + i`的源元素个数。

## 调用示例

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"
namespace {
template <typename T>
void PrintData(const char* label, const std::vector<T>& values)
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

constexpr uint32_t SRC_COUNT = 256;
constexpr uint32_t DST_COUNT = 128;
__simd_vf__ inline void Histogram(__ubuf__ uint16_t* low, __ubuf__ uint16_t* high, __ubuf__ uint8_t* src)
{
    vector_uint16_t low_reg, high_reg;
    vector_uint8_t src_reg;
    uint32_t src_count = SRC_COUNT;
    vector_bool src_mask = asc_update_mask_b8(src_count);
    uint32_t dst_count = DST_COUNT;
    vector_bool dst_mask = asc_update_mask_b16(dst_count);
    asc_duplicate_scalar(low_reg, static_cast<uint16_t>(0), dst_mask);
    asc_duplicate_scalar(high_reg, static_cast<uint16_t>(0), dst_mask);
    asc_loadalign(src_reg, src);
    asc_cumulative_histogram_bin0(low_reg, src_reg, src_mask);
    asc_cumulative_histogram_bin1(high_reg, src_reg, src_mask);
    asc_storealign(low, low_reg, dst_mask);
    asc_storealign(high, high_reg, dst_mask);
}
__global__ __vector__ void AscCumulativeHistogramKernel(__gm__ uint16_t* low, __gm__ uint16_t* high, __gm__ uint8_t* src)
{
    asc_init();
    __ubuf__ uint16_t low_local[DST_COUNT], high_local[DST_COUNT];
    __ubuf__ uint8_t src_local[SRC_COUNT];
    asc_copy_gm2ub_align(src_local, src, SRC_COUNT);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Histogram(low_local, high_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(low, low_local, DST_COUNT * sizeof(uint16_t));
    asc_copy_ub2gm_align(high, high_local, DST_COUNT * sizeof(uint16_t));
    asc_sync();
}
} // namespace
int main()
{
    std::vector<uint8_t> src(SRC_COUNT);
    std::vector<uint16_t> low(DST_COUNT, 0), high(DST_COUNT, 0);
    std::vector<uint16_t> golden_low(DST_COUNT), golden_high(DST_COUNT);
    for (uint32_t i = 0; i < SRC_COUNT; ++i) src[i] = static_cast<uint8_t>(i);
    for (uint32_t i = 0; i < DST_COUNT; ++i) {
        golden_low[i] = i + 1;
        golden_high[i] = DST_COUNT + i + 1;
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), (SRC_COUNT) * sizeof(uint8_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint16_t* low_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&low_device), (DST_COUNT) * sizeof(uint16_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint16_t* high_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&high_device), (DST_COUNT) * sizeof(uint16_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(uint8_t), src.data(), src.size() * sizeof(uint8_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    AscCumulativeHistogramKernel<<<1, 0>>>(low_device, high_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(low.data(), low.size() * sizeof(uint16_t), low_device, low.size() * sizeof(uint16_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(high.data(), high.size() * sizeof(uint16_t), high_device, high.size() * sizeof(uint16_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", src);
    PrintData("BIN0 output", low);
    PrintData("BIN1 output", high);
    PrintData("BIN0 Golden", golden_low);
    PrintData("BIN1 Golden", golden_high);
    const bool passed = CompareData(low, golden_low) && CompareData(high, golden_high);
    std::cout << (passed ? "[Success] asc_cumulative_histogram_bin0/bin1 passed." : "[Failed] asc_cumulative_histogram_bin0/bin1 failed.") << std::endl;
    aclrtFree(high_device);
    aclrtFree(low_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
