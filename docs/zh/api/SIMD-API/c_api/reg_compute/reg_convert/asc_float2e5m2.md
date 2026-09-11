# asc_float2e5m2

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR/Ascend 960DT：支持
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->

## 功能说明

头文件路径为：`"c_api/reg_compute/reg_convert.h"`。

根据`mask`将`src`中的每个`float`类型数据转换成`fp8_e5m2_t`类型，支持RINT舍入模式、饱和/非饱和模式与置零/合并模式，结果写入`dst`或通过函数返回值返回。
由于源操作数与目的操作数类型位宽比为4:1，写入数据时需要将一个`VL`大小的数据分为四部分，根据位置参数选择数据写入每4个元素中第0个、第1个、第2个或第3个的位置。以置零模式为例，伪代码如下：

```python
# 位置判断仅用于说明接口语义，实际位置参数为编译期常量。
# VL表示矢量数据寄存器位宽，取值256字节。
def asc_float2e5m2_rn(dst, src, mask, dst_quarter_pos):
    # ASC_DISPERSE_FIRST_QUARTER、ASC_DISPERSE_SECOND_QUARTER、ASC_DISPERSE_THIRD_QUARTER和
    # ASC_DISPERSE_FOURTH_QUARTER分别对应offset取0、1、2、3
    offset = {ASC_DISPERSE_FIRST_QUARTER: 0, ASC_DISPERSE_SECOND_QUARTER: 1,
              ASC_DISPERSE_THIRD_QUARTER: 2, ASC_DISPERSE_FOURTH_QUARTER: 3}[dst_quarter_pos]
    for i in range(VL // 4):
        dst[4 * i] = 0
        dst[4 * i + 1] = 0
        dst[4 * i + 2] = 0
        dst[4 * i + 3] = 0
        if mask[i]:
            dst[4 * i + offset] = fp8_e5m2(src[i])  # float -> fp8_e5m2_t，RINT舍入
```

合并模式下，上述伪代码中置0的元素保留`dst`中原值。

关于舍入模式的详细说明，请参见[舍入模式与饱和模式](rounding_mode.md)。

本接口为`reg`矢量计算接口，仅在AIV上生效。

## 函数原型

```c
// 占位符形式：通过引用参数输出结果
__simd_callee__ inline void asc_float2e5m2_rn<sat_mode><merge_mode>(vector_fp8_e5m2_t& dst,
                                                                    vector_float src,
                                                                    vector_bool mask,
                                                                    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
                                                                        dst_quarter_pos)

// 占位符形式：通过引用参数输出结果
__simd_callee__ inline void asc_float2e5m2_rn<sat_mode><merge_mode>(vector_fp8_e5m2_t& dst,
                                                                    vector_float src,
                                                                    vector_bool mask,
                                                                    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
                                                                        dst_quarter_pos)

// 占位符形式：通过引用参数输出结果
__simd_callee__ inline void asc_float2e5m2_rn<sat_mode><merge_mode>(vector_fp8_e5m2_t& dst,
                                                                    vector_float src,
                                                                    vector_bool mask,
                                                                    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
                                                                        dst_quarter_pos)

// 占位符形式：通过引用参数输出结果
__simd_callee__ inline void asc_float2e5m2_rn<sat_mode><merge_mode>(vector_fp8_e5m2_t& dst,
                                                                    vector_float src,
                                                                    vector_bool mask,
                                                                    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
                                                                        dst_quarter_pos)

// 占位符形式：通过函数返回值返回结果
__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn<sat_mode><merge_mode>(vector_float src,
                                                                                 vector_bool mask,
                                                                                 std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
                                                                                     dst_quarter_pos)

// 占位符形式：通过函数返回值返回结果
__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn<sat_mode><merge_mode>(vector_float src,
                                                                                 vector_bool mask,
                                                                                 std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
                                                                                     dst_quarter_pos)

// 占位符形式：通过函数返回值返回结果
__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn<sat_mode><merge_mode>(vector_float src,
                                                                                 vector_bool mask,
                                                                                 std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
                                                                                     dst_quarter_pos)

// 占位符形式：通过函数返回值返回结果
__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn<sat_mode><merge_mode>(vector_float src,
                                                                                 vector_bool mask,
                                                                                 std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
                                                                                     dst_quarter_pos)
```

- `<sat_mode>`表示饱和/非饱和模式，为空时表示非饱和模式，取值为`_sat`时表示饱和模式。
- `<merge_mode>`表示掩码未选中元素的处理模式：为空时表示置零模式，`mask`未选中的元素和组内未写入结果的位置置0；取值为`_m`时表示合并模式，`mask`未选中的元素和组内未写入结果的位置保留`dst`中原值。
<!-- npu="960" id9 -->
- 合并模式（`<merge_mode>`取值为`_m`）仅在Ascend 960PR/Ascend 960DT产品上支持。
<!-- end id9 -->

### 函数原型典型示例

```c
// RINT舍入模式，非饱和模式，置零模式，数据写入每4个元素中第0个的位置
__simd_callee__ inline void asc_float2e5m2_rn(vector_fp8_e5m2_t& dst,
                                              vector_float src,
                                              vector_bool mask,
                                              std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
                                                  dst_quarter_pos)
```

<!-- npu="960" id10 -->

```c
// RINT舍入模式，饱和模式，合并模式，数据写入每4个元素中第3个的位置，结果通过函数返回值返回
__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat_m(vector_float src,
                                                                 vector_bool mask,
                                                                 std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
                                                                     dst_quarter_pos)
```

<!-- end id10 -->

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。合并模式下，未写入结果的位置保留`dst`原值。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |
| dst_quarter_pos | 输入 | 位置选择标签（编译器标签类型），类型为`std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>`、`std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>`、`std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>`或`std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>`。取`ASC_DISPERSE_FIRST_QUARTER`时选择将结果写入目的操作数每4个元素中第0个的位置；取`ASC_DISPERSE_SECOND_QUARTER`时选择写入第1个的位置；取`ASC_DISPERSE_THIRD_QUARTER`时选择写入第2个的位置；取`ASC_DISPERSE_FOURTH_QUARTER`时选择写入第3个的位置。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

- 通过引用参数输出结果的函数原型无返回值。
- 通过函数返回值返回结果的函数原型返回转换结果，返回值类型与对应引用输出函数原型中`dst`参数的类型一致（去除引用），即`vector_fp8_e5m2_t`。

## 约束说明

- 通过引用参数输出结果的函数原型在非AIV上调用时直接返回；通过函数返回值返回结果的函数原型在非AIV上调用时返回对应矢量类型的默认构造值。
- `src`与`dst`的数据类型需要与函数原型匹配。
- 置零模式下，`mask`掩码位为0的元素和组内未写入结果的位置置0；合并模式下保留`dst`原值。
- 使用饱和或非饱和模式时，需要配置`ctrl`寄存器，详细说明请参见[asc_set_ctrl](../../spr/asc_set_ctrl.md)。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id11 -->
以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

<!-- end id11 -->

```c
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

constexpr uint32_t BUFFER_BYTES = 256;
__simd_vf__ inline void convert(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_fp8_e5m2_t dst_0;
    vector_float src_0;
    asc_loadalign(dst_0, reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output));
    asc_loadalign(src_0, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2e5m2_rn_sat(dst_0, src_0, mask, ASC_DISPERSE_FIRST_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_0, mask);
    vector_fp8_e5m2_t dst_1;
    vector_float src_1;
    asc_loadalign(dst_1, reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output));
    asc_loadalign(src_1, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2e5m2_rn(dst_1, src_1, mask, ASC_DISPERSE_SECOND_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_1, mask);
    vector_fp8_e5m2_t dst_2;
    vector_float src_2;
    asc_loadalign(dst_2, reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output));
    asc_loadalign(src_2, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2e5m2_rn_sat(dst_2, src_2, mask, ASC_DISPERSE_SECOND_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_2, mask);
    vector_fp8_e5m2_t dst_3;
    vector_float src_3;
    asc_loadalign(dst_3, reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output));
    asc_loadalign(src_3, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2e5m2_rn(dst_3, src_3, mask, ASC_DISPERSE_THIRD_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_3, mask);
    vector_fp8_e5m2_t dst_4;
    vector_float src_4;
    asc_loadalign(dst_4, reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output));
    asc_loadalign(src_4, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2e5m2_rn_sat(dst_4, src_4, mask, ASC_DISPERSE_THIRD_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_4, mask);
    vector_fp8_e5m2_t dst_5;
    vector_float src_5;
    asc_loadalign(dst_5, reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output));
    asc_loadalign(src_5, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2e5m2_rn(dst_5, src_5, mask, ASC_DISPERSE_FOURTH_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_5, mask);
    vector_fp8_e5m2_t dst_6;
    vector_float src_6;
    asc_loadalign(dst_6, reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output));
    asc_loadalign(src_6, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2e5m2_rn_sat(dst_6, src_6, mask, ASC_DISPERSE_FOURTH_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_6, mask);
    vector_fp8_e5m2_t dst_7;
    vector_float src_7;
    asc_loadalign(dst_7, reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output));
    asc_loadalign(src_7, reinterpret_cast<__ubuf__ float*>(input));
    asc_float2e5m2_rn(dst_7, src_7, mask, ASC_DISPERSE_FIRST_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_7, mask);
    vector_float src_8;
    asc_loadalign(src_8, reinterpret_cast<__ubuf__ float*>(input));
    vector_fp8_e5m2_t dst_8 = asc_float2e5m2_rn_sat(src_8, mask, ASC_DISPERSE_SECOND_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_8, mask);
    vector_float src_9;
    asc_loadalign(src_9, reinterpret_cast<__ubuf__ float*>(input));
    vector_fp8_e5m2_t dst_9 = asc_float2e5m2_rn(src_9, mask, ASC_DISPERSE_FIRST_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e5m2_t*>(output), dst_9, mask);
}
__global__ __vector__ void asc_float2e5m2_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES];
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
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
    constexpr uint8_t input_pattern[] = {0x00, 0x00, 0x80, 0x3f};
    constexpr uint8_t golden_pattern[] = {0x3C, 0x00, 0x00, 0x00};
    std::vector<uint8_t> input(BUFFER_BYTES);
    std::vector<uint8_t> output(BUFFER_BYTES, 0xff);
    std::vector<uint8_t> golden(BUFFER_BYTES);
    for (size_t i = 0; i < BUFFER_BYTES; ++i) {
        input[i] = input_pattern[i % sizeof(input_pattern)];
        golden[i] = golden_pattern[i % sizeof(golden_pattern)];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), (BUFFER_BYTES) * sizeof(uint8_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), (BUFFER_BYTES) * sizeof(uint8_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(uint8_t), input.data(), input.size() * sizeof(uint8_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_float2e5m2_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint8_t), output_device, output.size() * sizeof(uint8_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Conversion: vector_float -> vector_fp8_e5m2_t" << std::endl;
    print_data("Input 1.0 (raw bytes)", input);
    print_data("Output (raw bytes)", output);
    print_data("Golden 1.0 (raw bytes)", golden);
    const bool passed = compare_data(output, golden);
    std::cout << (passed ? "[Success] asc_float2e5m2_rn/asc_float2e5m2_rn_sat/"
                            "ASC_DISPERSE_FIRST_QUARTER/ASC_DISPERSE_SECOND_QUARTER/"
                            "ASC_DISPERSE_THIRD_QUARTER/ASC_DISPERSE_FOURTH_QUARTER passed."
                          : "[Failed] asc_float2e5m2_rn/asc_float2e5m2_rn_sat/"
                            "ASC_DISPERSE_FIRST_QUARTER/ASC_DISPERSE_SECOND_QUARTER/"
                            "ASC_DISPERSE_THIRD_QUARTER/ASC_DISPERSE_FOURTH_QUARTER failed.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
