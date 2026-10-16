# asc_e8m02bfloat16

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR&960DT系列产品：Ascend 960DT支持，Ascend 960PR不支持
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

本接口仅支持Ascend 960DT产品的`dav-9201`架构，且要求Ascend C DevKit 9.2.0及以上版本。

根据`mask`将`src`中参与计算的`fp8_e8m0_t`类型元素转换为`bfloat16_t`类型，结果写入`dst`。由于源操作数与目的操作数的类型位宽比为1:2，参数`src_pos`选择从源操作数的偶数或奇数位置读取数据，未选中的源位置不参与计算。

本接口提供引用输出、返回值和`_v2`变体。`_v2`变体固定读取源操作数的奇数位置。`asc_cast`提供与该转换对应的`EVEN/ODD`位置重载。

本接口为Reg矢量计算接口，仅在AIV上生效。

## 函数原型

以下原型中的`<src_pos>`为占位符，用于概括一组编译期重载；实际调用时必须使用下文所示的具体枚举值。

```cpp
__simd_callee__ inline void asc_e8m02bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, <src_pos>> src_pos)
```

```cpp
__simd_callee__ inline vector_bfloat16_t asc_e8m02bfloat16(
    vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, <src_pos>> src_pos)
```

```cpp
__simd_callee__ inline void asc_e8m02bfloat16_v2(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, <src_pos>> src_pos)
```

**接口说明如下：**

- `src_pos`为占位符，支持`EVEN`和`ODD`。
- `asc_e8m02bfloat16_v2`等价于使用`ODD`位置的引用输出版本。
- `asc_cast`提供`EVEN`和`ODD`两个位置重载，不支持饱和模式和合并写入版本。

### 函数原型典型示例

```cpp
__simd_callee__ inline void asc_e8m02bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

```cpp
__simd_callee__ inline vector_bfloat16_t asc_e8m02bfloat16(
    vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

```cpp
__simd_callee__ inline void asc_e8m02bfloat16_v2(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。 |
| src_pos | 输入 | 源数据位置选择标签，选择从源操作数的偶数或奇数位置读取数据。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 数据类型

| 源数据类型 | 目的数据类型 |
| --- | --- |
| `vector_fp8_e8m0_t` | `vector_bfloat16_t` |

## 返回值说明

- 引用输出版本和`_v2`版本无返回值。
- 返回值版本返回`vector_bfloat16_t`类型的转换结果。
- `asc_cast`为引用输出接口，无返回值。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 位置选择标签仅能使用编译期常量。
- `mask`掩码位为0时，`dst`对应元素置0。

## 调用示例

以下示例是一个可执行的完整测试用例。示例先将可精确表示的`bfloat16_t`类型`1.0`分别转换到`vector_fp8_e8m0_t`的偶数和奇数位置，再测试具名接口的引用输出、返回值、`_v2`变体，以及对应的`asc_cast`偶数/奇数位置重载。主机侧逐段比较输出与`bfloat16_t`类型`1.0`的原始字节`0x3f80`，验证每个重载的转换结果。

示例以256B缓冲区演示一次矢量寄存器转换；实际缓冲区大小应覆盖目标产品的一个矢量寄存器数据，256B不是本接口的通用约束。

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需指定为`dav-9201`。

```bash
bisheng example.asc -o main --npu-arch=dav-9201 && ./main
```

```cpp
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

namespace {
constexpr uint32_t BUFFER_BYTES = 256;
constexpr uint32_t RESULT_BYTES = BUFFER_BYTES;
constexpr uint32_t RESULT_COUNT = 7;
constexpr uint32_t OUTPUT_BYTES = RESULT_BYTES * RESULT_COUNT;

__simd_vf__ inline void convert(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    vector_bfloat16_t bfloat_src;
    vector_fp8_e8m0_t src;
    vector_bfloat16_t dst;

    asc_loadalign(bfloat_src, reinterpret_cast<__ubuf__ bfloat16_t*>(input));
    asc_bfloat162e8m0_rz(src, bfloat_src, mask, ASC_POSITION_EVEN);
    asc_e8m02bfloat16(dst, src, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output), dst, mask);
    dst = asc_e8m02bfloat16(src, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output + RESULT_BYTES), dst, mask);
    asc_cast(dst, src, mask, ASC_POSITION_EVEN);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output + RESULT_BYTES * 5), dst, mask);

    asc_bfloat162e8m0_rz(src, bfloat_src, mask, ASC_POSITION_ODD);
    asc_e8m02bfloat16(dst, src, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output + RESULT_BYTES * 2), dst, mask);
    dst = asc_e8m02bfloat16(src, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output + RESULT_BYTES * 3), dst, mask);
    asc_e8m02bfloat16_v2(dst, src, mask);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output + RESULT_BYTES * 4), dst, mask);
    asc_cast(dst, src, mask, ASC_POSITION_ODD);
    asc_storealign(reinterpret_cast<__ubuf__ bfloat16_t*>(output + RESULT_BYTES * 6), dst, mask);
}

__global__ __vector__ void asc_e8m02bfloat16_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    __ubuf__ uint8_t output_local[OUTPUT_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    convert(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}

bool all_results_equal(const std::vector<uint8_t>& output, const std::vector<uint8_t>& golden)
{
    for (uint32_t i = 0; i < RESULT_COUNT; ++i) {
        if (!std::equal(output.begin() + i * RESULT_BYTES,
                output.begin() + (i + 1) * RESULT_BYTES, golden.begin())) {
            return false;
        }
    }
    return true;
}
} // namespace

int main()
{
    constexpr uint8_t bfloat_one[] = {0x80, 0x3f};
    std::vector<uint8_t> input(BUFFER_BYTES);
    std::vector<uint8_t> output(OUTPUT_BYTES, 0xff);
    std::vector<uint8_t> golden(BUFFER_BYTES);
    for (uint32_t i = 0; i < BUFFER_BYTES; ++i) {
        input[i] = bfloat_one[i % sizeof(bfloat_one)];
        golden[i] = bfloat_one[i % sizeof(bfloat_one)];
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BUFFER_BYTES, input.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_e8m02bfloat16_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = all_results_equal(output, golden);
    std::cout << (passed ? "[Success] asc_e8m02bfloat16 test passed."
                         : "[Failed] asc_e8m02bfloat16 test failed.") << std::endl;
    aclrtFree(output_device);
    aclrtFree(input_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
