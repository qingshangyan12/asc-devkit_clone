# cast

> **命名空间** `asc::te` · **头文件** `reg_vector/reg_compute.h`

> **状态**：函数体 TODO

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品 AI Core：不支持
- Atlas 推理系列产品 Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

设计为元素类型转换 `reg_tensor<From> → reg_tensor<To>`，可带舍入/饱和/`cast_layout`。**函数体 TODO，不可用。**

计划对齐 `ct::element_cast<To>`。合入前请在 load 侧用匹配类型的 UB，或使用已实现的 `trunc`（同类型取整，不是换类型）。

- `cast_options`：round / sat / layout 组合，见枚举表。

## 3. 函数原型

```cpp
enum class cast_layout { unknow = -1, zero, one, two, three };
enum class cast_round_mode { unknow = -1, rint, round, floor, ceil, trunc };
enum class cast_sat_mode { unknow = -1, not_sat, sat };

struct cast_options {
    cast_layout     layout = cast_layout::unknow;
    cast_round_mode round  = cast_round_mode::unknow;
    cast_sat_mode   sat    = cast_sat_mode::unknow;
};
constexpr cast_options default_cast_ops = {
    cast_layout::zero, cast_round_mode::rint, cast_sat_mode::not_sat
};

template <typename To,
          const cast_options& opt = default_cast_ops,
          typename From>
reg_tensor<To> cast(reg_tensor<From> src);   // 函数体 TODO
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


__simd_vf__ inline void compute(local_tensor<half> dst_ub,
                                local_tensor<half> src_ub,
                                uint32_t total)
{
    using namespace asc::te;
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);
        auto x = src_ub.load(make_coord(i * VL)).with_mask(m);
        auto y = cast<half>(x);
        dst_ub.store(make_coord(i * VL), y, m);
    }
}

__global__ __vector__ void cast_kernel(__gm__ half* dst, __gm__ half* src)
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
        src[i] = static_cast<float>(i) * 0.1f;
        golden[i] = static_cast<half>(src[i]);
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

    cast_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(dst.data(), dst.size() * sizeof(half), dst_device,
                dst.size() * sizeof(half), ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input src", src);
    print_data("Output", dst);
    print_data("Golden", golden);
    const bool passed = compare_data(dst, golden, 0.01);
    std::cout << (passed ? "[Success] cast passed." : "[Failed] cast failed.")
              << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
