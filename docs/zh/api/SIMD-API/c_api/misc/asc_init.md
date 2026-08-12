# asc_init

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

本接口对AI Core的全局状态进行初始化，包括AIC（Cube Core）与AIV（Vector Core）的公共状态及各自特有状态。由于不同产品的实现存在差异，实际执行的初始化项也有所不同。

**各产品实际执行的初始化项如下：**

<!-- npu="950" id8 -->
**表1** 针对Ascend 950PR/Ascend 950DT本接口执行的初始化项

| 状态类别 | 初始化内容 |
| ---- | ---- |
| 公共状态 | 原子累加状态 |
| 公共状态 | Mask工作模式 |
| 公共状态 | CTRL寄存器 |
| 公共状态 | 原子操作开启位与原子操作类型 |
| AIC | asc_copy_l12l0a（3D格式搬运）接口调用时Pad填充的数值 |
| AIV | Mask |
| AIV | loop mode的参数 |
<!-- end id8 -->

<!-- npu="A3, 910b" id9 -->
**表2** 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品本接口执行的初始化项

| 状态类别 | 初始化内容 |
| ---- | ---- |
| 公共状态 | 原子累加状态 |
| 公共状态 | Mask工作模式 |
| AIV | Mask |
<!-- end id9 -->


## 函数原型

```cpp
__aicore__ inline void asc_init()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 在实际运行中，这些值可能被前序执行的算子修改，若不调用该接口进行初始化，非预期的值可能导致计算结果出现精度错误。

  例如前序算子使用Counter模式但未重置为Normal模式，当前算子以默认的Normal模式设置Mask时，会导致Mask设置不符合预期，进而引发精度错误。

## 调用示例

```cpp
// 将代码保存为example.asc后，使用以下命令编译并运行：
// bisheng example.asc -o main --npu-arch=dav-3510; ./main
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENTS = 16;
constexpr uint32_t BYTES = ELEMENTS * sizeof(uint64_t);

void print_data(const char* label, const std::vector<uint64_t>& data)
{
    std::cout << label << ":";
    const size_t count = data.size() < 8 ? data.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << data[i];
    if (data.size() > count) std::cout << " ...";
    std::cout << std::endl;
}

__global__ __vector__ void asc_init_kernel(__gm__ uint64_t* output, __gm__ uint64_t* input)
{
    asc_init();
    __ubuf__ uint32_t local[BYTES / sizeof(uint32_t)];
    asc_copy_gm2ub_align(local, reinterpret_cast<__gm__ uint32_t*>(input), BYTES);
    asc_sync_mte2(0);
    asc_set_atomic_add_int32();
    // Reinitialize the core and clear persistent state such as the atomic mode.
    asc_init();
    // asc_init resets the atomic mode, so this is an ordinary UB-to-GM copy.
    asc_copy_ub2gm(reinterpret_cast<__gm__ uint32_t*>(output), local, BYTES);
    asc_sync_mte3(0);
}

} // namespace

int main()
{
    std::vector<uint64_t> input(ELEMENTS), output(ELEMENTS, 0);
    for (uint32_t i = 0; i < ELEMENTS; ++i) { input[i] = i + 1; output[i] = 10; }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint64_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BYTES, input.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, BYTES, output.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_init_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", input);
    print_data("Output after asc_init reset", output);
    print_data("Golden ordinary copy", input);
    const bool passed = output == input;
    std::cout << (passed ? "[Success] asc_init passed." : "[Failed] asc_init failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
