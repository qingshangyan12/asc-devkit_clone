# asc_atomic_max

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

对Global Memory中的数据与指定数据执行原子求最大值操作，即将address指向的数据元素与val中的最大值写回到该地址。

## 函数原型

### 模板原型（占位符形式）

```c
__aicore__ inline <dtype> asc_atomic_max(__gm__ <dtype>* address,
                                         <dtype> val)
```

### dtype支持数据类型

dtype支持的数据类型为`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t`。

### 典型示例

```c
// 示例：int32_t类型标量原子取大
__aicore__ inline int32_t asc_atomic_max(__gm__ int32_t* address,
                                         int32_t val)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| address | 输出 | Global Memory的地址。 |
| val | 输入 | 源操作数。 |

## 返回值说明

address地址中计算前的原始数据。

## 流水类型

PIPE_S

## 约束说明

- 本接口操作数固定为单个标量（不支持向量或连续多元素）。
- `address`必须落在Global Memory地址空间。传入UB/L1/L0C/栈等非GM地址空间时，硬件不保证原子语义且可能触发异常。
- `address`需按所访问dtype的字节数对齐。
- `val`的dtype必须与`address`指向元素的dtype一致，无需额外配置dtype。（本接口与数据搬运随路原子操作接口相互独立，不受`asc_set_atomic_max_*`等随路原子设置影响，也不会影响后续搬运指令）。
- 本接口通过原子锁对“读取—比较—写回”全过程串行化，多个AI Core或多线程并发调用同一`address`时不会相互覆盖。取最大值运算（max）满足结合律，多次并发取最大值的最终结果与执行顺序无关，dtype为`float`时同样为确定性计算（与浮点累加接口不同，浮点取最大值不存在非确定性问题）。
- 本接口运行在标量流水（PIPE_S）上，同一标量流水内的数据依赖由指令执行顺序保证。若本接口与PIPE_MTE2或PIPE_MTE3上的数据搬运指令访问同一GM地址，且执行顺序影响结果，编译器无法自动完成跨流水同步，调用方需按实际依赖插入`asc_sync_pipe`，或配合使用`asc_sync_notify`与`asc_sync_wait`保证执行顺序。
- 本接口访问GM时绕过DCache，不维护缓存一致性。若其他核或其他通路通过缓存访问同一GM地址，调用方需使用[asc_dcci](../cache_ctrl/asc_dcci.md)清理或失效对应Cache Line，并使用[asc_sync_data_barrier](../sync/asc_sync_data_barrier.md)保证相关访存操作的执行顺序和数据可见性。详情可参考[Scalar原子操作与DCache一致性](../../../../guide/编程指南/高级编程/内存模型/缓存一致性.md#scalar原子操作与dcache一致性)。

## 调用示例

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

template <typename T>
bool compare_range_data(const std::vector<T>& actual, const std::vector<T>& expected,
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

__global__ __vector__ void asc_atomic_max_kernel(__gm__ int64_t* output)
{
    asc_init();
    __gm__ uint32_t* address = reinterpret_cast<__gm__ uint32_t*>(output);
    asc_dcci_entire_all();
    const uint32_t old_value = asc_atomic_max(address, 15U);
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dcci_entire_all();
    output[1] = static_cast<int64_t>(old_value);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<int64_t> input = {10, 0};
    std::vector<int64_t> golden = {15, 10};
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
    asc_atomic_max_kernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int64_t), output_device, output.size() * sizeof(int64_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input", input);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_atomic_max passed." : "[Failed] asc_atomic_max failed.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
