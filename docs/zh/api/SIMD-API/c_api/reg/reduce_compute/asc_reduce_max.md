# asc_reduce_max

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

根据`mask`对源操作数`src`进行归约最大值操作，将最大值写入目的操作数`dst`的第0个元素，将最大值在`src`中的索引原始位模式写入`dst`的第1个元素，并将`dst`中的其他元素置0。如果存在多个最大值，则写入最小的索引。计算公式如下：

$$
dst_0 = \max\{src_i \mid mask_i = 1\} \\
dst_1 = \argmax\{src_i \mid mask_i = 1\}
$$

## 函数原型

```c
__simd_callee__ inline void asc_reduce_max(vector_<dtype>& dst,
                                           vector_<dtype> src,
                                           vector_bool mask)
```

### dtype取值

`dtype`支持的取值为`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：对int16_t类型的矢量数据寄存器执行归约最大值操作。
__simd_callee__ inline void asc_reduce_max(vector_int16_t& dst,
                                           vector_int16_t src,
                                           vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述                                     |
| ------ | --------- | ---------------------------------------- |
| dst  | 输出      | 目的操作数（矢量数据寄存器）。           |
| src  | 输入      | 源操作数（矢量数据寄存器）。             |
| mask | 输入      | 掩码寄存器，用于控制各元素是否参与归约。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 未被`mask`选中的元素被视为对应数据类型的最小值，浮点数类型的最小值为`-inf`。如果`src`中的所有元素均未被`mask`选中，则将该最小值写入`dst`的第0个元素，并将其余元素置0。
- 比较时遵循$max(-0, +0) = +0$。
- 如果输入数据中存在nan，则将nan写入`dst`的第0个元素，并将第一个nan的索引写入`dst`的第1个元素。

## 关键特性

**索引值需要强制类型转换**：

`dst`的索引按照`dst`的数据类型存储，比如`dst`为half类型时，索引按照half类型存储，因此读取索引需要使用  reinterpret\_cast方法转换到整数类型。若数据类型是half，需要使用reinterpret\_cast\<uint16_t\*\>；若数据类型是float，需要使用reinterpret\_cast\<uint32\_t\*\>。

```cpp
__global__ __vector__ void reduce_max_kernel(
    __gm__ uint8_t* src, __gm__ uint8_t* value_result, __gm__ uint8_t* index_result)
{
    asc_init();
    __ubuf__ half src_local[KEY_FEATURE_COUNT];
    __ubuf__ half dst_local[KEY_FEATURE_COUNT];
    __ubuf__ half value_local[KEY_FEATURE_RESULT_COUNT];
    __ubuf__ uint16_t index_local[KEY_FEATURE_RESULT_COUNT];
    asc_copy_gm2ub(src_local, (__gm__ half*)src, KEY_FEATURE_BYTES);
    asc_sync();
    uint32_t count = KEY_FEATURE_COUNT;
    reduce_max_vf(dst_local, src_local, count);
    asc_sync();
    half max_value = dst_local[0];
    half index_bits = dst_local[1];
    uint16_t max_index = *reinterpret_cast<uint16_t*>(&index_bits);
}
```

## 调用示例

<!-- npu="950" id8 -->
将以下代码保存为`example.asc`后，可通过`bisheng`命令编译运行。其中，`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

```cpp
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

constexpr uint32_t ELEMENT_COUNT = 64;
__simd_vf__ inline void reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src)
{
    vector_uint32_t dst_reg, src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);
    asc_reduce_max(dst_reg, src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}
__global__ __vector__ void asc_reduce_max_kernel(__gm__ uint32_t* dst, __gm__ uint32_t* src)
{
    asc_init();
    __ubuf__ uint32_t dst_local[ELEMENT_COUNT], src_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(src_local, src, ELEMENT_COUNT * sizeof(uint32_t));
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    reduce(dst_local, src_local);
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
    golden[0] = src.back(); golden[1] = ELEMENT_COUNT - 1;
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
    asc_reduce_max_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint32_t), dst_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", src);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_reduce_max passed." : "[Failed] asc_reduce_max failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```

<!-- end id8 -->
