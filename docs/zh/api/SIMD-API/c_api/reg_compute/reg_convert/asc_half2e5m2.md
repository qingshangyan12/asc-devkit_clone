# asc_half2e5m2

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR&960DT系列产品：支持
<!-- end id1 -->
<!-- npu="950" id8 -->
- Ascend 950PR&950DT系列产品：不支持
<!-- end id8 -->
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

头文件路径为：`"c_api/reg_compute/reg_convert.h"`。

本接口仅在Ascend 960PR&960DT系列产品的`dav-9201`架构上支持。

根据`mask`将`src`中参与计算的`half`类型元素转换为`fp8_e5m2_t`类型，结果写入`dst`。由于源操作数与目的操作数的类型位宽比为2:1，`dst`中的元素按索引分为偶数位置和奇数位置，参数`dst_pos`选择转换结果的写入位置。非合并写入版本将`mask`对应位为0或未选中的位置置零，合并写入版本保留这些位置的原值。

本接口使用RN舍入模式，支持饱和和非饱和模式，并提供合并写入版本。`asc_cast`和`asc_cast_merge`提供与本接口对应的编译期重载：前者将未选中位置清零，后者保留未选中位置的原值。

本接口为Reg矢量计算接口，仅在AIV上生效。

## 函数原型

```cpp
__simd_callee__ inline void asc_half2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, <dst_pos>> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, <dst_pos>> dst_pos)
```

```cpp
__simd_callee__ inline vector_fp8_e5m2_t asc_half2e5m2_rn(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, <dst_pos>> dst_pos)
```

```cpp
__simd_callee__ inline vector_fp8_e5m2_t asc_half2e5m2_rn_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, <dst_pos>> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e5m2_rn_v2(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_half2e5m2_rn_sat_v2(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_half2e5m2_rn_merge(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, <dst_pos>> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e5m2_rn_sat_merge(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, <dst_pos>> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, <saturation_mode>> saturation_mode,
    std::integral_constant<asc_position_mode, <dst_pos>> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast_merge(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, <saturation_mode>> saturation_mode,
    std::integral_constant<asc_position_mode, <dst_pos>> dst_pos)
```

**接口说明如下：**

- `dst_pos`为占位符，支持`EVEN`和`ODD`。
- `saturation_mode`为占位符，支持`NO_SAT`和`SAT`。
- `asc_half2e5m2_rn`和`asc_half2e5m2_rn_sat`分别为非饱和和饱和版本。
- `asc_half2e5m2_rn_merge`和`asc_half2e5m2_rn_sat_merge`各包含`EVEN/ODD`两个位置重载。
- `asc_cast`和`asc_cast_merge`各包含`NO_SAT/SAT`与`EVEN/ODD`的全部组合重载。
- `asc_half2e5m2_rn_v2`和`asc_half2e5m2_rn_sat_v2`等价于使用`ODD`位置的引用输出版本。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。 |
| round_mode | 输入 | 舍入模式，固定为`ASC_CAST_RN`。具名接口固定为RN舍入。 |
| saturation_mode | 输入 | 饱和模式，取`ASC_CAST_NO_SAT`或`ASC_CAST_SAT`。 |
| dst_pos | 输入 | 位置选择标签。选择将结果写入目的操作数的偶数或奇数位置。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 数据类型

| 源数据类型 | 目的数据类型 |
| --- | --- |
| `vector_half` | `vector_fp8_e5m2_t` |

## 返回值说明

- 引用输出版本无返回值。
- 返回值版本返回`vector_fp8_e5m2_t`类型的转换结果。
- `asc_cast`和`asc_cast_merge`为引用输出接口，无返回值。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 位置选择标签仅能使用编译期常量。
- `asc_half2e5m2_rn`、`asc_half2e5m2_rn_sat`及其`_v2`变体和`asc_cast`在`mask`对应位为0或未选中位置时将结果置零；`asc_half2e5m2_rn_merge`、`asc_half2e5m2_rn_sat_merge`和`asc_cast_merge`保留这些位置的原值。

## 调用示例

以下示例将`half`类型的`1.0`转换为`fp8_e5m2_t`。先通过具名接口写入偶数位置，再通过`asc_cast_merge`写入奇数位置，因此输出的每个字节均为`fp8_e5m2_t`类型`1.0`的编码`0x3c`。

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需指定为`dav-9201`。

```bash
bisheng example.asc -o main --npu-arch=dav-9201 && ./main
```

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

namespace {
constexpr uint32_t BUFFER_BYTES = 256;

__simd_vf__ inline void convert(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool half_mask = asc_create_mask_b16(PAT_ALL);
    vector_bool fp8_mask = asc_create_mask_b8(PAT_ALL);
    vector_half src;
    vector_fp8_e5m2_t dst;
    asc_loadalign(src, reinterpret_cast<__ubuf__ half*>(input));
    // 写入偶数位置，未写入的奇数位置清零。
    asc_half2e5m2_rn_sat(dst, src, half_mask, ASC_POSITION_EVEN);
    // 合并写入奇数位置，保留上一步已经写入的偶数位置。
    asc_cast_merge(dst, src, half_mask, ASC_CAST_RN, ASC_CAST_SAT, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst, fp8_mask);
}

__global__ __vector__ void asc_half2e5m2_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    __ubuf__ uint8_t output_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    convert(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    constexpr uint8_t half_one[] = {0x00, 0x3c};
    constexpr uint8_t fp8_one[] = {0x3c, 0x3c};
    std::vector<uint8_t> input(BUFFER_BYTES);
    std::vector<uint8_t> output(BUFFER_BYTES, 0xff);
    std::vector<uint8_t> golden(BUFFER_BYTES);
    for (uint32_t i = 0; i < BUFFER_BYTES; ++i) {
        input[i] = half_one[i % sizeof(half_one)];
        golden[i] = fp8_one[i % sizeof(fp8_one)];
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BUFFER_BYTES, input.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_half2e5m2_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BUFFER_BYTES, output_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_half2e5m2 passed." : "[Failed] asc_half2e5m2 failed.") << std::endl;
    aclrtFree(output_device);
    aclrtFree(input_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
