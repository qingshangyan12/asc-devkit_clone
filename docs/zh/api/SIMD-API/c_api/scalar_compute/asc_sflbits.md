# asc_sflbits

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

计算一个int64_t类型数字的二进制中，从最高数值位开始与符号位相同的连续比特位的个数。

例：int64_t value = 0x0f00000000000000;

  符号位为0，从最高数值位开始往后（不包含符号位）与符号位相同的连续比特数为3，故返回3。

## 函数原型

```c
__aicore__ inline int64_t asc_sflbits(int64_t value)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| value | 输入 | 输入数据。 |

## 返回值说明

返回从最高数值位开始和符号位相同的连续比特位的个数，数据类型为int64_t。

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，非AIC调用直接返回。
- 当输入是-1（比特位全1）或者0（比特位全0）时，返回-1。
- 接口仅支持s64 dtype（64位有符号整数），传入其他位宽的值会按`int64_t`隐式转换后参与位运算，结果以`int64_t`返回。

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

constexpr uint32_t ELEMENTS = 8;

__global__ __vector__ void AscSflbitsKernel(__gm__ int64_t* output)
{
    asc_init();
    output[0] = asc_sflbits(0);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<int64_t> input = {0};
    std::vector<int64_t> golden = {-1};
    input.resize(ELEMENTS, 0);
    golden.resize(ELEMENTS, 0);
    std::vector<int64_t> output(ELEMENTS, -1);
    aclInit(nullptr);
    aclrtSetDevice(0);
    int64_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), (ELEMENTS) * sizeof(int64_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(output_device, input.size() * sizeof(int64_t), input.data(), input.size() * sizeof(int64_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    AscSflbitsKernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int64_t), output_device, output.size() * sizeof(int64_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", input);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = CompareData(output, golden);
    std::cout << (passed ? "[Success] asc_sflbits passed." : "[Failed] asc_sflbits failed.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
