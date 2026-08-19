# operator!=

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

按元素判断src0_i != src1_i是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。

## 3. 函数原型

```cpp
namespace asc::te {

vector_bool operator!=(reg_tensor<T> src0, reg_tensor<T> src1);

}
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `src0` | 输入 | 左操作数；比较 mask 跟随 `src0.mask`。 |
| `src1` | 输入 | 右操作数；类型须与 `src0` 一致。 |

## 5. 返回值说明

返回 `vector_bool` 谓词；可交给 `select` 或 `with_mask`，**不是** `reg_tensor<T>`。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- `src0.mask` 须预先绑定；比较 mask 跟随左操作数。
- 返回值是谓词，不能直接当数值 `reg_tensor` 累加。

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
                                local_tensor<float> src0_ub,
                                local_tensor<float> src1_ub,
                                uint32_t total)
{
    using namespace asc::te;
    constexpr uint32_t VL = asc_get_vf_len<float>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<float>(total);
        auto a = src0_ub.load(make_coord(i * VL)).with_mask(m);
        auto b = src1_ub.load(make_coord(i * VL)).with_mask(m);
        auto cond = a != b;
        auto y = select(cond, fill(1.0f), fill(0.0f));
        dst_ub.store(make_coord(i * VL), y, m);
    }
}

__global__ __vector__ void opnegeq_kernel(__gm__ float* dst, __gm__ float* src0, __gm__ float* src1)
{
    asc_init();
    using namespace asc::te;
    auto dst_global = make_tensor(make_mem_ptr(dst), make_layout(make_shape(ELEMENT_COUNT)));
    auto src0_global = make_tensor(make_mem_ptr(src0), make_layout(make_shape(ELEMENT_COUNT)));
    auto src1_global = make_tensor(make_mem_ptr(src1), make_layout(make_shape(ELEMENT_COUNT)));
    __ubuf__ float dst_ptr[ELEMENT_COUNT];
    __ubuf__ float src0_ptr[ELEMENT_COUNT];
    __ubuf__ float src1_ptr[ELEMENT_COUNT];
    auto dst_local = make_tensor(make_mem_ptr(dst_ptr), make_layout(make_shape(ELEMENT_COUNT)));
    auto src0_local = make_tensor(make_mem_ptr(src0_ptr), make_layout(make_shape(ELEMENT_COUNT)));
    auto src1_local = make_tensor(make_mem_ptr(src1_ptr), make_layout(make_shape(ELEMENT_COUNT)));

    asc_lock(PIPE_MTE2, 0);
    copy(src0_local, src0_global);
    copy(src1_local, src1_global);
    asc_unlock(PIPE_MTE2, 0);

    asc_lock(PIPE_V, 0);
    compute(dst_local, src0_local, src1_local, ELEMENT_COUNT);
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
    std::vector<float> src0(ELEMENT_COUNT);
    std::vector<float> src1(ELEMENT_COUNT);
    std::vector<float> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src0[i] = static_cast<float>(i);
        src1[i] = static_cast<float>(i + 1);
        golden[i] = (src0[i] != src1[i]) ? 1.0f : 0.0f;
    }


    aclInit(nullptr);
    aclrtSetDevice(0);
    float* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), ELEMENT_COUNT * sizeof(float),
                ACL_MEM_MALLOC_HUGE_FIRST);
    float* src0_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src0_device), ELEMENT_COUNT * sizeof(float),
                ACL_MEM_MALLOC_HUGE_FIRST);
    float* src1_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src1_device), ELEMENT_COUNT * sizeof(float),
                ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src0_device, src0.size() * sizeof(float), src0.data(),
                src0.size() * sizeof(float), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(src1_device, src1.size() * sizeof(float), src1.data(),
                src1.size() * sizeof(float), ACL_MEMCPY_HOST_TO_DEVICE);

    opnegeq_kernel<<<1, 0>>>(dst_device, src0_device, src1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(dst.data(), dst.size() * sizeof(float), dst_device,
                dst.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input src0", src0);
    print_data("Input src1", src1);
    print_data("Output", dst);
    print_data("Golden", golden);
    const bool passed = compare_data(dst, golden);
    std::cout << (passed ? "[Success] operator!= passed." : "[Failed] operator!= failed.")
              << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src0_device);
    aclrtFree(src1_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
