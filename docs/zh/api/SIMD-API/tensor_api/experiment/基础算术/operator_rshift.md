# operator>>

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

根据掩码mask对输入数据src0，按照src1对应元素进行右移操作，完成后将结果写入dst中。

## 3. 函数原型

```cpp
namespace asc::te {

decltype(auto) operator>>(reg_tensor<T> src0, reg_tensor<T> src1);
decltype(auto) operator>>(reg_tensor<T> src0, T src1);

}
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `src0` | 输入 | 左源操作数；内嵌 `mask` 决定有效 lane，结果 mask 跟随 `src0`。 |
| `src1` | 输入 | 右源操作数；类型 `T` 须与 `src0` 一致。 |

## 5. 返回值说明

返回 `reg_tensor<T>`：
- `value`：活跃 lane 为 `dst_i = src0_i >> src1_i`；非活跃 lane 置零。
- `mask`：等于 `src0.mask`。

## 6. 约束说明

- 数据类型为无符号类型：执行逻辑右移。逻辑右移会将二进制数整体向右移动指定的位数，最低位被丢弃，最高位用0填充。
- 对于逻辑移位（无符号数据类型），如果位移量大于等于数据类型位宽，则输出为0。
- 数据类型为有符号类型：执行算术右移。算术右移会将二进制数整体向右移动指定的位数，最低位被丢弃，最高位用原符号位填充。
- 对于算术移位（有符号数据类型），如果src0小于0，src1大于0，且src1的位移量大于数据类型位宽，则输出为-1。
- 对于算术移位（有符号数据类型），如果src0大于0，且src1的位移量大于数据类型位宽，则输出为0。
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


__simd_vf__ inline void compute(local_tensor<int32_t> dst_ub,
                                local_tensor<int32_t> src_ub,
                                uint32_t total)
{
    using namespace asc::te;
    constexpr uint32_t VL = asc_get_vf_len<int32_t>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<int32_t>(total);
        auto x = src_ub.load(make_coord(i * VL)).with_mask(m);
        auto y = x >> int32_t(1);
        dst_ub.store(make_coord(i * VL), y, m);
    }
}

__global__ __vector__ void opgtgt_kernel(__gm__ int32_t* dst, __gm__ int32_t* src)
{
    asc_init();
    using namespace asc::te;
    auto dst_global = make_tensor(make_mem_ptr(dst), make_layout(make_shape(ELEMENT_COUNT)));
    auto src_global = make_tensor(make_mem_ptr(src), make_layout(make_shape(ELEMENT_COUNT)));
    __ubuf__ int32_t dst_ptr[ELEMENT_COUNT];
    __ubuf__ int32_t src_ptr[ELEMENT_COUNT];
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
    std::vector<int32_t> dst(ELEMENT_COUNT, 0);
    std::vector<int32_t> src(ELEMENT_COUNT);
    std::vector<int32_t> golden(ELEMENT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        src[i] = static_cast<int32_t>(8 + i);
        golden[i] = src[i] >> 1;
    }


    aclInit(nullptr);
    aclrtSetDevice(0);
    int32_t* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), ELEMENT_COUNT * sizeof(int32_t),
                ACL_MEM_MALLOC_HUGE_FIRST);
    int32_t* src_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src_device), ELEMENT_COUNT * sizeof(int32_t),
                ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(src_device, src.size() * sizeof(int32_t), src.data(),
                src.size() * sizeof(int32_t), ACL_MEMCPY_HOST_TO_DEVICE);

    opgtgt_kernel<<<1, 0>>>(dst_device, src_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(dst.data(), dst.size() * sizeof(int32_t), dst_device,
                dst.size() * sizeof(int32_t), ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input src", src);
    print_data("Output", dst);
    print_data("Golden", golden);
    const bool passed = compare_data(dst, golden);
    std::cout << (passed ? "[Success] operator>> passed." : "[Failed] operator>> failed.")
              << std::endl;
    aclrtFree(dst_device);
    aclrtFree(src_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
