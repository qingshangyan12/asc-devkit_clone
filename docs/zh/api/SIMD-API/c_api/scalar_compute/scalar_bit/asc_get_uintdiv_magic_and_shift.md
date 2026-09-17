# asc_get_uintdiv_magic_and_shift

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

头文件路径为：`"c_api/scalar_compute/compute/scalar_bit.h"`。

给定除数`divisor`，计算快速除法乘法因子`magic`和移位量`shift`。整数快除算法分为两步计算商。本接口用于第一步计算，生成的参数供SIMT侧快除接口`asc_uintdiv`使用。本接口的计算过程如下：

$$
\begin{aligned}
shift &= \left\lceil \log_2 divisor \right\rceil \\
magic &= \left\lfloor 2^N \times \frac{2^{shift} - divisor}{divisor} \right\rfloor + 1
\end{aligned}
$$

根据除数数据类型的不同，公式中$N$的取值不同：当除数为`uint32_t`数据类型时，$N$为32；当除数为`uint64_t`数据类型时，$N$为64。

> [!NOTE]说明
>
> 整数快除算法使用方式：
> 1. 在标量计算逻辑中，调用[asc_get_uintdiv_magic_and_shift](asc_get_uintdiv_magic_and_shift.md)接口，计算除数的`magic`和`shift`值。
> 2. 在SIMT VF计算逻辑中，调用`asc_uintdiv`接口，完成指定被除数的除法运算。

## 函数原型

```c
__aicore__ inline void asc_get_uintdiv_magic_and_shift(<dtype>* magic,
                                                       <dtype>* shift,
                                                       <dtype> divisor)
```

### dtype支持数据类型

dtype支持的数据类型为`uint32_t`、`uint64_t`。

### 函数原型典型示例

```c
// 示例：uint32_t类型除数。
__aicore__ inline void asc_get_uintdiv_magic_and_shift(uint32_t* magic,
                                                       uint32_t* shift,
                                                       uint32_t divisor)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :------ | :--- | :------------ |
| magic | 输出 | 根据指定除数计算得到的快速除法乘法因子。 |
| shift | 输出 | 根据指定除数计算得到的快速除法移位量。 |
| divisor | 输入 | 除数。 |

## 返回值说明

无

## 约束说明

- 当除数为`uint32_t`数据类型时，支持的除数最大值为`INT32_MAX`，即2的31次方减1；当除数为`uint64_t`数据类型时，支持的除数最大值为`INT64_MAX`，即2的63次方减1。
- 除数不支持为0。
- 整数快除算法可以用于性能优化，适用于一个除数被用于多次除法计算的场景。
- 本接口运行在标量流水（`PIPE_S`）上，同一流水内的数据依赖由指令执行顺序保证，无需额外同步。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
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
    for (const T value : values) std::cout << ' ' << value;
    std::cout << std::endl;
}

template <typename T>
bool compare_data(const std::vector<T>& actual, const std::vector<T>& expected)
{
    return actual == expected;
}

constexpr uint32_t ELEMENTS = 4;

__global__ __vector__ void asc_get_uintdiv_magic_and_shift_kernel(__gm__ uint64_t* output)
{
    asc_init();
    uint32_t magic32 = 0;
    uint32_t shift32 = 0;
    uint64_t magic64 = 0;
    uint64_t shift64 = 0;
    asc_get_uintdiv_magic_and_shift(&magic32, &shift32, static_cast<uint32_t>(7));
    asc_get_uintdiv_magic_and_shift(&magic64, &shift64, static_cast<uint64_t>(7));
    output[0] = magic32;
    output[1] = shift32;
    output[2] = magic64;
    output[3] = shift64;
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint64_t> output(ELEMENTS, 0);
    const std::vector<uint64_t> golden = {613566757, 3, 2635249153387078803ULL, 3};
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint64_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), ELEMENTS * sizeof(uint64_t), ACL_MEM_MALLOC_HUGE_FIRST);
    asc_get_uintdiv_magic_and_shift_kernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint64_t), output_device, output.size() * sizeof(uint64_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_get_uintdiv_magic_and_shift passed."
                         : "[Failed] asc_get_uintdiv_magic_and_shift failed.")
              << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```

完整算子样例请参考[SIMD与SIMT混合编程实现快速除法算子样例](../../../../../../../examples/05_simd_simt_hybrid/02_best_practices/simd_simt_integer_fast_div/README.md)。
