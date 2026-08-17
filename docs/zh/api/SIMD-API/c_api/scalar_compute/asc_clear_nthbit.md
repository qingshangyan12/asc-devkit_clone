# asc_clear_nthbit

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

对一个64位无符号整数执行位清除操作，将`bits`中第`idx`位清零，其余位保持不变。返回修改后的64位无符号整数。

本接口为标量位运算，运行在标量流水上，AIC与AIV均可调用，行为一致。

## 函数原型

```c
__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits,
                                            int64_t idx)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `bits` | 输入 | 64位无符号整数。取值范围为[0，2^64−1]。 |
| `idx` | 输入 | 位索引。实际位索引按`(idx % 64 + 64) % 64`计算，范围为[0，63]。 |

## 返回值说明

返回位清零后的结果（uint64_t类型）。

## 流水类型

PIPE_S

## 约束说明

当`idx`大于63或小于0时，实际清除的位索引按`(idx % 64 + 64) % 64`计算。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->

在Ascend 950上，编译运行命令如下，NPU架构配置为`dav-3510`：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```
<!-- end id8 -->

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

constexpr uint32_t ELEMENTS = 8;

__global__ __vector__ void asc_clear_nthbit_kernel(__gm__ uint64_t* output)
{
    asc_init();
    output[0] = asc_clear_nthbit(15, 1);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint64_t> input = {0};
    std::vector<uint64_t> golden = {13};
    input.resize(ELEMENTS, 0);
    golden.resize(ELEMENTS, 0);
    std::vector<uint64_t> output(ELEMENTS, 0xffffffffffffffffULL);
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint64_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), (ELEMENTS) * sizeof(uint64_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(output_device, input.size() * sizeof(uint64_t), input.data(), input.size() * sizeof(uint64_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_clear_nthbit_kernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint64_t), output_device, output.size() * sizeof(uint64_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", input);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_clear_nthbit passed." : "[Failed] asc_clear_nthbit failed.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
