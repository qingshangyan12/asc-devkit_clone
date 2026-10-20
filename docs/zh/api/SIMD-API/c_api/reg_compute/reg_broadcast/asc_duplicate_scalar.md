# asc_duplicate_scalar

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/reg_compute/compute/reg_broadcast.h"`。

将标量`value`广播到目的操作数中。支持通过引用参数输出结果和通过函数返回值返回结果两种接口形式，每种形式均支持以下两类重载：

- 带`mask`：将`value`写入`dst`中被`mask`筛选的位置，未被`mask`筛选的位置置0。
- 不带`mask`：将`value`写入`dst`的全部元素位置。

本接口为Reg矢量计算接口，仅在AIV上生效。

## 函数原型

### 带mask

```cpp
// 通过函数返回值返回结果（占位符形式）
__simd_callee__ inline vector_<dtype> asc_duplicate_scalar(<dtype> value,
                                                           vector_bool mask)

// 通过引用参数输出结果（占位符形式）
__simd_callee__ inline void asc_duplicate_scalar(vector_<dtype>& dst,
                                                 <dtype> value,
                                                 vector_bool mask)
```

**占位符说明如下：**

- `<dtype>`取值为：`int8_t`、`uint8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

#### 函数原型典型示例

```cpp
// 通过函数返回值返回结果
__simd_callee__ inline vector_int8_t asc_duplicate_scalar(int8_t value,
                                                          vector_bool mask)
// 通过引用参数输出结果
__simd_callee__ inline void asc_duplicate_scalar(vector_int8_t& dst,
                                                 int8_t value,
                                                 vector_bool mask)
```

### 不带mask

```cpp
// 通过函数返回值返回结果（占位符形式）
__simd_callee__ inline vector_<dtype> asc_duplicate_scalar(<dtype> value)

// 通过引用参数输出结果（占位符形式）
__simd_callee__ inline void asc_duplicate_scalar(vector_<dtype>& dst,
                                                 <dtype> value)
```

**占位符说明如下：**

- `<dtype>`取值为：`int8_t`、`uint8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

#### 函数原型典型示例

```cpp
// 通过函数返回值返回结果
__simd_callee__ inline vector_int8_t asc_duplicate_scalar(int8_t value)

// 通过引用参数输出结果
__simd_callee__ inline void asc_duplicate_scalar(vector_int8_t& dst,
                                                 int8_t value)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器），保存广播结果。仅通过引用参数输出结果的接口包含该参数，`dst`的元素数据类型用于确定函数重载。 |
| value | 输入 | 源操作数（标量），作为待广播的数据。对于带返回值接口，`value`的数据类型用于确定函数重载和返回类型，建议传入类型明确的变量或使用显式类型转换。 |
| mask | 输入 | 目的操作数元素操作的有效指示（掩码寄存器），仅带`mask`的重载包含该参数。`mask`筛选的元素在`dst`中填充为`value`，未筛选的元素在`dst`中置0。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

- 通过引用参数输出结果的函数原型无返回值，广播结果写入`dst`。
- 通过函数返回值返回结果的函数原型返回广播结果，返回类型为与`dtype`对应的`vector_<dtype>`。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 同一寄存器的数据依赖由硬件保序，无需额外插入同步指令。本接口与前后Reg数据搬运接口之间，如果不同寄存器访问同一Unified Buffer（UB）地址且存在写后读或写后写依赖，需要调用[asc_mem_bar](../reg_sync/asc_mem_bar.md)进行同步。
- 调用带返回值接口时，应使用类型明确的变量或显式类型转换，例如`static_cast<uint8_t>(1)`，以匹配正确的函数原型。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

以下调用示例代码仅Ascend 950PR&950DT系列产品支持。

```cpp
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint8_t MUTEX_ID = 0;

__aicore__ constexpr uint32_t ceil_division(uint32_t numerator, uint32_t denominator)
{
    return (numerator + denominator - 1) / denominator;
}

constexpr uint32_t ELEMENTS_PER_REGISTER = 64;
constexpr uint32_t DATA_BYTES = 133 * sizeof(float);
constexpr uint32_t TOTAL_LENGTH = DATA_BYTES / sizeof(float);
constexpr uint32_t REPEAT_COUNT =
    (TOTAL_LENGTH + ELEMENTS_PER_REGISTER - 1) / ELEMENTS_PER_REGISTER;
constexpr uint32_t PADDED_ELEMENT_COUNT = REPEAT_COUNT * ELEMENTS_PER_REGISTER;
constexpr uint32_t ACTIVE_COUNT = 16;
constexpr uint32_t BUFFER_BYTES = TOTAL_LENGTH * sizeof(float);
constexpr float FILL_VALUE = 3.5f;

// one_repeat_size的单位为float元素；第i轮指针偏移i * one_repeat_size个元素，
// 对应i * one_repeat_size * sizeof(float)字节。
__simd_vf__ inline void duplicate_scalar(__ubuf__ float* dst, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_float dst_reg;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        const uint32_t offset = i * one_repeat_size;
        const uint32_t remaining = TOTAL_LENGTH - offset;
        uint32_t remaining_length = remaining < ELEMENTS_PER_REGISTER ? remaining : ELEMENTS_PER_REGISTER;
        uint32_t remaining_length_active = remaining_length < ACTIVE_COUNT ? remaining_length : ACTIVE_COUNT;
        vector_bool active_mask = asc_update_mask_b32(remaining_length_active);
        vector_bool store_mask = asc_update_mask_b32(remaining_length);
        asc_duplicate_scalar(dst_reg, FILL_VALUE, active_mask);
        asc_storealign(dst + offset, dst_reg, store_mask);
    }
}

__global__ __vector__ void asc_duplicate_scalar_kernel(__gm__ float* dst)
{
    asc_init();
    __ubuf__ float dst_local[PADDED_ELEMENT_COUNT];
    uint16_t one_repeat_size = asc_get_vf_len() / sizeof(float);
    uint16_t repeat_time = ceil_division(TOTAL_LENGTH, one_repeat_size);
    asc_lock(PIPE_V, MUTEX_ID);
    asc_vf_call<duplicate_scalar>(dst_local, one_repeat_size, repeat_time);
    asc_unlock(PIPE_V, MUTEX_ID);

    asc_lock(PIPE_MTE3, MUTEX_ID);
    asc_copy_ub2gm_align(dst, dst_local, BUFFER_BYTES);
    asc_unlock(PIPE_MTE3, MUTEX_ID);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<float> output(TOTAL_LENGTH);
    std::vector<float> golden(TOTAL_LENGTH, 0.0f);
    for (uint32_t repeat = 0; repeat < REPEAT_COUNT; ++repeat) {
        const uint32_t offset = repeat * ELEMENTS_PER_REGISTER;
        const uint32_t remaining = TOTAL_LENGTH - offset;
        const uint32_t remaining_length = remaining < ELEMENTS_PER_REGISTER ? remaining : ELEMENTS_PER_REGISTER;
        const uint32_t remaining_length_active = remaining_length < ACTIVE_COUNT ? remaining_length : ACTIVE_COUNT;
        for (uint32_t i = 0; i < remaining_length_active; ++i) golden[offset + i] = FILL_VALUE;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    float* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    asc_duplicate_scalar_kernel<<<1, 0>>>(dst_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), dst_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_duplicate_scalar passed."
                        : "[Failed] asc_duplicate_scalar failed.") << std::endl;
    aclrtFree(dst_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
