# asc_reduce_sum

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

根据mask对源操作数src进行归约求和操作，用于将src中的所有参与计算的元素求和，得到的结果保存在dst中的0位置。参考伪代码：

```python
def asc_reduce_sum(dst, src, mask):
    sum_val = 0
    for i in range(128):
        if mask[i]:
            sum_val = sum_val + src[i]  # 底层为二叉树累加, 此处顺序展开仅作示意
    dst[0] = sum_val                    # 求和值
    for i in range(1, 128):
        dst[i] = 0                      # 其余位置置0
```

## 函数原型

### 模板原型（占位符形式）

本接口使用`<src_dtype>`和`<dst_dtype>`分别表示源操作数和目的操作数的数据类型，模板原型如下：

```c
__simd_callee__ inline void asc_reduce_sum(vector_<dst_dtype>& dst,
                                           vector_<src_dtype> src,
                                           vector_bool mask)
```

#### src_dtype与dst_dtype支持组合

| src_dtype | dst_dtype |
| --------- | --------- |
| `int16_t` | `int32_t` |
| `int32_t` | `int32_t` |
| `uint16_t` | `uint32_t` |
| `uint32_t` | `uint32_t` |
| `half` | `half` |
| `float` | `float` |

### 典型示例

```c
// 示例：float类型归约求和
__simd_callee__ inline void asc_reduce_sum(vector_float& dst,
                                           vector_float src,
                                           vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述                                     |
| ------ | --------- | ---------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。           |
| src    | 输入      | 源操作数（矢量数据寄存器）。             |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与归约。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- 指令内累加顺序采用二叉树累加方式，结果连续写入到目的操作数，目的操作数中的其它元素置0。
- 当所有元素均不参与计算（mask全为0）时，将0写入目的操作数对应位置（特别的，对于浮点数为+0）。
- 对于输入为uint16_t/int16_t类型的情况，会提升精度到uint32_t/int32_t进行计算。

## 关键特性

**asc_reduce_sum累加顺序**：

以二叉树累加的方式计算源操作数src内有效元素的数据总和。

以half类型的数据求和为例，在src内有128个数，通过二叉树的方式，两两相加，计算过程如下图所示：

1. data1和data2相加得到data01，data3和data4相加得到data02，……，data125和data126相加得到data63，data127和data128相加得到data64；
2. data01和data02相加得到data001，data03和data04相加得到data002，……，data63和data64相加得到data032；
3. 以此类推，得到目的操作数为1个half类型的数据sum。

**图1** reg_reduce_index
![reg_reduce_index](../../../../figures/reg_reduce_index.png "reg_reduce_index")

## 调用示例

```cpp
// Build/run (examples/02_simd_c_api/04_c_api_call_examples/reg/reduce_compute/asc_reduce_sum.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang examples/02_simd_c_api/04_c_api_call_examples/reg/reduce_compute/asc_reduce_sum.asc -lm -o /tmp/c_api_reg_reduce_compute_asc_reduce_sum && /tmp/c_api_reg_reduce_compute_asc_reduce_sum
// Architecture: dav-3510

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



constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void Reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src)
{
    vector_uint32_t dst_reg, src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    asc_reduce_sum(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}
__global__ __vector__ void AscReduceSumKernel(__gm__ uint32_t* dst, __gm__ uint32_t* src)
{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT], src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Reduce(dst_local, src_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync();
}
} // namespace
int main()
{
    std::vector<uint32_t> src(ELEMENT_COUNT), output(ELEMENT_COUNT, 0), golden(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) src[i] = i + 1;
    golden[0] = ELEMENT_COUNT * (ELEMENT_COUNT + 1) / 2;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint32_t* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), (ELEMENT_COUNT) * sizeof(uint32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(uint32_t), src.data(), src.size() * sizeof(uint32_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    AscReduceSumKernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint32_t), dst_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", src);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = CompareData(output, golden);
    std::cout << (passed ? "[Success] asc_reduce_sum passed." : "[Failed] asc_reduce_sum failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
