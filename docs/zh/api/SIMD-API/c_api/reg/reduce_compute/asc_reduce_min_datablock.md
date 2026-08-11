# asc_reduce_min_datablock

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


根据mask将每个DataBlock(32B)中的最小值依次保存在dst中的最低位。计算公式如下：

$$
dst_j = \min\{src_i \mid jN \le i < (j+1)N,\ mask_i = 1\}, \quad j = 0,1,\ldots,7
$$

其中，N表示每个DataBlock包含的元素个数；$j$表示DataBlock的索引。

## 函数原型

### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_reduce_min_datablock(vector_<dtype>& dst,
                                                     vector_<dtype> src,
                                                     vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`

### 典型示例

```c
// 示例：对half矢量数据寄存器按数据块归约取最小值
__simd_callee__ inline void asc_reduce_min_datablock(vector_half& dst,
                                                     vector_half src,
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

- 在比较中，遵循min(-0, +0) = -0。
- 找到源操作数中每个DataBlock内的最小值，连续写入到目的操作数，目的操作数中的其它元素置0。
- 未被mask选中的元素被视为最大值（浮点数为+inf），如果一个DataBlock中所有元素都未被mask选中，最大值（浮点数为+inf）将被填充到目的操作数的指定位置。
- 仅输出最值，不输出index下标。

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



constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void Reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src)
{
    vector_uint32_t dst_reg, src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    asc_reduce_min_datablock(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}
__global__ __vector__ void AscReduceMinDatablockKernel(__gm__ uint32_t* dst, __gm__ uint32_t* src)
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
    for (uint32_t block = 0; block < 8; ++block) golden[block] = src[block * 8];
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
    AscReduceMinDatablockKernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint32_t), dst_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", src);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = CompareData(output, golden);
    std::cout << (passed ? "[Success] asc_reduce_min_datablock passed." : "[Failed] asc_reduce_min_datablock failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
