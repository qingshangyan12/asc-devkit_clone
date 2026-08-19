# reduce_min

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

根据mask对源操作数src进行归约最小值操作：将src中的最小值写入目的操作数dst的0位置，最小值在src中的索引写入目的操作数dst的1位置，dst的其他位置将被置为0。

## 3. 函数原型

```cpp
namespace asc::te {

// reduce_scope 定义见 §7.2
template <typename To,
          reduce_scope scope = reduce_scope::vector,
          typename From>
decltype(auto) reduce_min(reg_tensor<From> src);
// pair 范围未实现。

}
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `src` | 输入 | 源 `reg_tensor<From>`，内嵌 `mask` 决定参与归约的 lane。 |
| `To` | 模板参数 | 归约结果元素类型。 |
| `scope` | 模板参数 | `reduce_scope::vector`（默认）/ `datablock` / `pair`（pair 对 max/min 未实现）。 |

## 5. 返回值说明

返回 `reg_tensor<To>` 窄结果；Softmax 等场景用 `fill(reduce_min<T>(x), m)` 广播后再逐 lane 运算。

## 6. 约束说明

- 当所有元素均不参与计算时，将该数据类型的最大值写入dst。
- 当存在多个最小值时，会将第一个最小值及其索引保存在dst中。
- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 不是跨拍 NumPy axis 归约；跨多拍需自行累加。
- 归约结果为窄向量，须 `fill(..., m)` 后再参与逐 lane 运算。

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


__simd_vf__ inline void compute(local_tensor<float> dst_ub,
                                local_tensor<float> src_ub,
                                uint32_t total)
{
    using namespace asc::te;
    constexpr uint32_t VL = asc_get_vf_len<float>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<float>(total);
        auto x = src_ub.load(make_coord(i * VL)).with_mask(m);
        auto y = x - fill(reduce_min<float>(x), m);
        dst_ub.store(make_coord(i * VL), y, m);
    }
}

__global__ __vector__ void reduce_min_kernel(__gm__ float* dst, __gm__ float* src)
{
    asc_init();
    using namespace asc::te;
    auto dst_global = make_tensor(make_mem_ptr(dst), make_layout(make_shape(ELEMENT_COUNT)));
    auto src_global = make_tensor(make_mem_ptr(src), make_layout(make_shape(ELEMENT_COUNT)));
    __ubuf__ float dst_ptr[ELEMENT_COUNT];
    __ubuf__ float src_ptr[ELEMENT_COUNT];
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
    std::vector<float> dst(ELEMENT_COUNT, 0);
    std::vector<float> src(ELEMENT_COUNT);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src[i] = static_cast<float>(i) * 0.1f;

    }

    uint32_t rem = ELEMENT_COUNT;
    for (uint32_t off = 0; off < ELEMENT_COUNT; ) {
        uint32_t chunk = rem > 64 ? 64 : rem;
        float rv = src[off];
        for (uint32_t j = 1; j < chunk; ++j) rv = std::min(rv, src[off + j]);
        for (uint32_t j = 0; j < chunk; ++j) golden[off + j] = src[off + j] - rv;
        off += chunk;
        rem -= chunk;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    float* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), ELEMENT_COUNT * sizeof(float),
                ACL_MEM_MALLOC_HUGE_FIRST);
    float* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), ELEMENT_COUNT * sizeof(float),
                ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(float), src.data(),
                src.size() * sizeof(float), ACL_MEMCPY_HOST_TO_DEVICE);

    reduce_min_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(dst.data(), dst.size() * sizeof(float), dst_device,
                dst.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input src", src);
    print_data("Output", dst);
    print_data("Golden", golden);
    const bool passed = compare_data(dst, golden, 0.01);
    std::cout << (passed ? "[Success] reduce_min passed." : "[Failed] reduce_min failed.")
              << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
