# asc_float2int32

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

将float类型的标量转换为int32_t类型并返回。接口后缀用于指定舍入模式，支持以下4种变体：

- `asc_float2int32_rd`：向负无穷方向舍入，即取不大于输入值的最大整数（如1.5→1、-1.5→-2）。
- `asc_float2int32_ru`：向正无穷方向舍入，即取不小于输入值的最小整数（如1.5→2、-1.5→-1）。
- `asc_float2int32_rn`：向最近偶数舍入，即四舍六入五凑偶（如0.5→0、1.5→2、2.5→2）。
- `asc_float2int32_rna`：向最近整数舍入，等值远离零舍入，即四舍五入（如0.5→1、1.5→2、-0.5→-1）。

本接口为标量运算，AIC与AIV均可调用，行为一致。

## 函数原型

### 舍入模式变体

#### 模板原型

```c
__aicore__ inline int32_t asc_float2int32_<round>(
    float value)
```

#### 变体支持

4个舍入模式变体分别为`asc_float2int32_rd`（向负无穷方向舍入）、`asc_float2int32_ru`（向正无穷方向舍入）、`asc_float2int32_rn`（向最近偶数舍入）、`asc_float2int32_rna`（向最近整数舍入，等值远离零）

#### 典型示例

```c
// 示例：向最近偶数舍入
__aicore__ inline int32_t asc_float2int32_rn(
    float value)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :------ | :----- | :----------------- |
| value | 输入 | 源操作数（标量）。 |

## 返回值说明

目的操作数（标量），value精度转换成int32_t的结果。

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- 本接口为标量运算，AIC与AIV均可调用，行为一致。
- 本接口为标量单目数据类型转换运算，输入dtype固定为`float`，返回dtype固定为`int32_t`，其他dtype触发数据类型异常。
- 输入为NaN或Inf时返回0，不触发非法指令异常。
- 有限输入值超出`int32_t`的表示范围[−2^31,2^31−1]时，饱和模式下，结果钳位到`int32_t`的最大值或最小值；非饱和模式下，结果截断为低32位。接口默认采用饱和模式，配置非饱和模式请参见[asc_set_ctrl](../sys_var/asc_set_ctrl.md)。
- 本接口为纯标量运算，不访问UB/GM/L1等存储单元，不涉及地址对齐与容量约束。
- 本接口运行在标量流水上，与后续依赖该返回值的指令之间存在标量数据依赖，由标量流水本身的顺序执行保证，无需额外插入同步指令。

### 舍入模式约束

- 4个接口分别通过`_rd`、`_ru`、`_rn`、`_rna`后缀指定舍入模式，调用方需按所需舍入语义选择对应接口。
- `asc_float2int32_rd`向负无穷舍入，对正数取floor、对负数取远离零方向舍入（如-1.5→-2）。
- `asc_float2int32_ru`向正无穷舍入，对正数取远离零方向舍入（如1.5→2）、对负数取ceil（如-1.5→-1）。
- `asc_float2int32_rn`向最近偶数舍入，0.5、2.5等恰为两整数中点的值取偶数邻值（如0.5→0、2.5→2）。
- `asc_float2int32_rna`向最近整数舍入，等值远离零舍入，0.5、1.5等恰为两整数中点的值取绝对值更大的邻值（如0.5→1、-0.5→-1）。

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

__global__ __vector__ void AscFloat2int32Kernel(__gm__ int64_t* output)
{
    asc_init();
    constexpr float value = 1.25f;
    output[0] = asc_float2int32_rn(value);
    output[1] = asc_float2int32_rna(value);
    output[2] = asc_float2int32_rd(value);
    output[3] = asc_float2int32_ru(value);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<int64_t> input = {0, 0, 0, 0};
    std::vector<int64_t> golden = {1, 1, 1, 2};
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
    AscFloat2int32Kernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int64_t), output_device, output.size() * sizeof(int64_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", input);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = CompareData(output, golden);
    std::cout << (passed ? "[Success] asc_float2int32_rn/asc_float2int32_rna/asc_float2int32_rd/asc_float2int32_ru passed." : "[Failed] asc_float2int32_rn/asc_float2int32_rna/asc_float2int32_rd/asc_float2int32_ru failed.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
