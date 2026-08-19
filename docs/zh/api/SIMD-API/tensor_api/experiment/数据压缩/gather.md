# gather

> **命名空间** `asc::te` · **头文件** `reg_vector/reg_compute.h`

> **状态**：函数体为空，未实现

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"c_api/reg_compute/reg_load.h"`。

根据索引位置index将源操作数src按元素收集到目的操作数dst中。

![asc_gather](../../figures/asc_gather.png)

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T, typename U>
decltype(auto) gather(reg_tensor<T> src, reg_tensor<U> index);
// dst[i] = src[index[i]]；函数体为空。

}
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `src` | 输入 | 被索引的源表 `reg_tensor<T>`。 |
| `index` | 输入 | 索引 `reg_tensor<U>`。 |

## 5. 返回值说明

返回 `reg_tensor<T>`：
- `value`：活跃 lane 为 `逐 lane 二元运算`；非活跃 lane 置零。
- `mask`：等于 `src0.mask`。

## 6. 约束说明

- 收集UB中的元素
- 当src的数据类型为b8数据类型时，dst为b16数据类型，这种情况下目的操作数的低8位与源操作数相同，高8位自动补0。例如src为int8_t数据类型：
- 40 = 0b00101000 -> 0b0000000000101000,扩充至16位后等于40；
- 40 = 0b11011000 -> 0b0000000011011000，扩充至16位后等于216。
- 收集矢量数据寄存器中的元素
- src为矢量数据寄存器类型，位宽是固定的VL,存储的元素个数固定。如果index中索引值超出当前矢量数据寄存器中能存储的最大元素个数时，按照如下方式处理：设定当前矢量数据寄存器所能存储的最大数据元素个数为vl_length, index中索引值为i，索引值更新为i % vl_length。
- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- `src0.mask` 须预先通过 `update_mask`、`make_mask` 等绑定（通常经 `load(...).with_mask(m)`）；未赋值 mask 内容不确定。
- 二元运算 mask 跟随**左操作数** `src0`。
- 参与计算的元素个数 = VL ÷ sizeof(T)；非活跃 lane 置零（ZEROING）。

## 7. 调用示例

将代码保存为 `example.asc` 后，可通过 bisheng 命令编译运行，其中 `--npu-arch` 参数需根据实际产品型号指定对应的 NPU 架构，具体产品与 NPU 架构的映射关系请参考 `__NPU_ARCH__`。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include <cmath>

#include "reg_tensor.h"
#include "local_tensor.h"
#include "reg_compute.h"
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
using namespace asc::te;

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


__simd_vf__ inline void compute(local_tensor<half> dst_ub,
                                local_tensor<half> src_ub,
                                uint32_t total)
{
    using namespace asc::te;
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);
        auto lut = src_ub.load(make_coord(i * VL)).with_mask(m);
        auto idx = iota(fill(static_cast<uint32_t>(0))).with_mask(m);
        auto y = gather(lut, idx);  // TODO：当前未实现
        dst_ub.store(make_coord(i * VL), y, m);
    }
}

__global__ __vector__ void gather_kernel(__gm__ half* dst, __gm__ half* src)
{
    asc_init();
    using namespace asc::te;
    auto dst_global = make_tensor(make_mem_ptr(dst), make_layout(make_shape(ELEMENT_COUNT)));
    auto src_global = make_tensor(make_mem_ptr(src), make_layout(make_shape(ELEMENT_COUNT)));
    __ubuf__ half dst_ptr[ELEMENT_COUNT];
    __ubuf__ half src_ptr[ELEMENT_COUNT];
    auto dst_local = make_tensor(make_mem_ptr(dst_ptr), make_layout(make_shape(ELEMENT_COUNT)));
    auto src_local = make_tensor(make_mem_ptr(src_ptr), make_layout(make_shape(ELEMENT_COUNT)));

    asc_lock(PIPE_MTE2, 0);
    copy(src_local, src_global);
    asc_unlock(PIPE_MTE2, 0);

    asc_lock(PIPE_V, 0);
    compute(dst_local, src_local, ELEMENT_COUNT);
    asc_unlock(PIPE_V, 0);

    asc_lock(PIPE_MTE3, 0);
    copy(dst_global, dst_local);
    asc_unlock(PIPE_MTE3, 0);

    asc_sync();
}
} // namespace

int main()
{
    std::vector<half> dst(ELEMENT_COUNT, 0);
    std::vector<half> src(ELEMENT_COUNT);
    std::vector<half> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src[i] = static_cast<half>(i * 0.01f);
        golden[i] = src[i];
    }


    aclInit(nullptr);
    aclrtSetDevice(0);
    half* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), ELEMENT_COUNT * sizeof(half),
                ACL_MEM_MALLOC_HUGE_FIRST);
    half* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), ELEMENT_COUNT * sizeof(half),
                ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(half), src.data(),
                src.size() * sizeof(half), ACL_MEMCPY_HOST_TO_DEVICE);

    gather_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(dst.data(), dst.size() * sizeof(half), dst_device,
                dst.size() * sizeof(half), ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input src", src);
    print_data("Output", dst);
    print_data("Golden", golden);
    const bool passed = compare_data(dst, golden, 0.01);
    std::cout << (passed ? "[Success] gather passed." : "[Failed] gather failed.")
              << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
