# histograms

> **命名空间** `asc::te` · **头文件** `reg_vector/reg_compute.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

对输入数据进行频率统计，生成直方图。统计结果在dst原有数据基础上累加。

**工作原理：**
- 统计src中每个数值的出现频率
- dst的第n个元素存储数值n在src中出现的次数
- 结果累加到dst原有数据中，支持多次统计的累加计算

**分区间统计：**
由于src数据类型为uint8_t（取值范围0~255），而dst每个元素为uint16_t，且一个Vector Length可存储128个数据，因此将统计范围分为两个区间：
- **低位模式（BIN0）**：统计src中数值在[0, 127]范围内的出现频率
- **高位模式（BIN1）**：统计src中数值在[128, 255]范围内的出现频率，统计时将数值减去128，映射到dst的索引位置

**统计方式如下图所示：**
- dst0：低位模式统计结果，dst0[0]表示数值0的出现次数，dst0[127]表示数值127的出现次数
- dst1：高位模式统计结果，dst1[0]表示数值128的出现次数，dst1[127]表示数值255的出现次数

![频率统计](../../figures/频率统计.png)

## 3. 函数原型

```cpp
enum class histogram_half { unknow = -1, low, high };
enum class histogram_mode { unknow = -1, frequency, cumulative };

struct histogram_options {
    histogram_half half = histogram_half::unknow;
    histogram_mode mode = histogram_mode::unknow;
};
constexpr histogram_options default_histogram_opts = {
    histogram_half::low, histogram_mode::frequency
};

template <typename T, const histogram_options& opt = default_histogram_opts>
decltype(auto) histograms(reg_tensor<T> src);
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `src` | 输入 | 源操作数 `reg_tensor<T>`，内嵌 `mask` 决定有效 lane。 |

## 5. 返回值说明

返回 `reg_tensor<T>`：
- `value`：活跃 lane 为 `逐 lane 一元运算`；非活跃 lane 置零。
- `mask`：等于 `src.mask`。

## 6. 约束说明

- 当mask位数为0时，源操作数src对应位置的数数值将被忽略，dst对应位置数值为忽略该位置src后计算得到的值。
- dst的数据类型为uint16_t，最大值为65535，使用时需注意累加溢出问题。
- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- `src.mask` 须预先通过 `update_mask`、`make_mask` 等绑定（通常经 `load(...).with_mask(m)`）；未赋值 mask 内容不确定。
- 参与计算的元素个数 = VL ÷ sizeof(T)（`VL = asc_get_vf_len<T>`）；非活跃 lane 置零（ZEROING）。

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


__simd_vf__ inline void compute(local_tensor<uint8_t> dst_ub,
                                local_tensor<uint8_t> src_ub,
                                uint32_t total)
{
    using namespace asc::te;
    constexpr uint32_t VL = asc_get_vf_len<uint8_t>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<uint8_t>(total);
        auto px = src_ub.load(make_coord(i * VL)).with_mask(m);
        auto hist = histograms(px);
        dst_ub.store(make_coord(i * VL), hist, m);
    }
}

__global__ __vector__ void histograms_kernel(__gm__ uint8_t* dst, __gm__ uint8_t* src)
{
    asc_init();
    using namespace asc::te;
    auto dst_global = make_tensor(make_mem_ptr(dst), make_layout(make_shape(ELEMENT_COUNT)));
    auto src_global = make_tensor(make_mem_ptr(src), make_layout(make_shape(ELEMENT_COUNT)));
    __ubuf__ uint8_t dst_ptr[ELEMENT_COUNT];
    __ubuf__ uint8_t src_ptr[ELEMENT_COUNT];
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
    std::vector<uint8_t> dst(ELEMENT_COUNT, 0);
    std::vector<uint8_t> src(ELEMENT_COUNT);
    std::vector<uint8_t> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src[i] = static_cast<uint8_t>(i % 16);
        golden[i] = src[i];
    }


    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), ELEMENT_COUNT * sizeof(uint8_t),
                ACL_MEM_MALLOC_HUGE_FIRST);
    uint8_t* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), ELEMENT_COUNT * sizeof(uint8_t),
                ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(uint8_t), src.data(),
                src.size() * sizeof(uint8_t), ACL_MEMCPY_HOST_TO_DEVICE);

    histograms_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(dst.data(), dst.size() * sizeof(uint8_t), dst_device,
                dst.size() * sizeof(uint8_t), ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input src", src);
    print_data("Output", dst);
    print_data("Golden", golden);
    const bool passed = compare_data(dst, golden);
    std::cout << (passed ? "[Success] histograms passed." : "[Failed] histograms failed.")
              << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
