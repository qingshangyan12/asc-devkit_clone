# asc_float2e4m3

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/reg_compute/reg_convert.h"`。

根据掩码，将源操作数中参与计算的每个`float`类型元素转换为`fp8_e4m3fn_t`类型，并将结果写入目的操作数。由于源操作数与目的操作数的数据类型位宽比为4:1，写入数据时需将目的操作数中一个`VL`大小的数据划分为四等份，并根据位置选择标签将数据写入到第1、第2、第3或第4个四分之一位置。

关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式](rounding_mode.md)。

## 函数原型

```cpp
// 通过引用参数输出结果（占位符形式）
__simd_callee__ inline void asc_float2e4m3_rn<sat_mode>(vector_fp8_e4m3fn_t& dst,
                                                        vector_float src,
                                                        vector_bool mask,
                                                        std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_quarter_pos)
__simd_callee__ inline void asc_float2e4m3_rn<sat_mode>(vector_fp8_e4m3fn_t& dst,
                                                        vector_float src,
                                                        vector_bool mask,
                                                        std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_quarter_pos)
__simd_callee__ inline void asc_float2e4m3_rn<sat_mode>(vector_fp8_e4m3fn_t& dst,
                                                        vector_float src,
                                                        vector_bool mask,
                                                        std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_quarter_pos)
__simd_callee__ inline void asc_float2e4m3_rn<sat_mode>(vector_fp8_e4m3fn_t& dst,
                                                        vector_float src,
                                                        vector_bool mask,
                                                        std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_quarter_pos)

// 通过返回值输出结果（占位符形式）
__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn<sat_mode>(vector_float src,
                                                                       vector_bool mask,
                                                                       std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_quarter_pos)
__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn<sat_mode>(vector_float src,
                                                                        vector_bool mask,
                                                                        std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_quarter_pos)
__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn<sat_mode>(vector_float src,
                                                                        vector_bool mask,
                                                                        std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_quarter_pos)
__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn<sat_mode>(vector_float src,
                                                                        vector_bool mask,
                                                                        std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_quarter_pos)
```

- `<sat_mode>`表示饱和模式。取值为空时，表示非饱和模式；取值为`_sat`时，表示饱和模式。

### 函数原型典型示例

```cpp
__simd_callee__ inline void asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst,
                                               vector_float src,
                                               vector_bool mask,
                                               std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_quarter_pos)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |
| dst_quarter_pos | 输入 | 目的操作数写入位置。<br>取值为：<br>&bull; `ASC_DISPERSE_FIRST_QUARTER`。读取源操作数，并将转换结果写入目的操作数的第1/4位置。<br>&bull; `ASC_DISPERSE_SECOND_QUARTER`。读取源操作数，并将转换结果写入目的操作数的第2/4位置。<br>&bull; `ASC_DISPERSE_THIRD_QUARTER`。读取源操作数，并将转换结果写入目的操作数的第3/4位置。<br>&bull; `ASC_DISPERSE_FOURTH_QUARTER`。读取源操作数，并将转换结果写入目的操作数的第4/4位置。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

- 通过引用参数输出结果的函数原型无返回值。
- 通过函数返回值输出结果的函数原型返回计算结果，返回值类型与对应引用输出函数原型中`dst`参数的类型一致（去除引用）。

## 约束说明

- 通过引用参数输出结果的函数原型在非AIV上调用时直接返回。
- 通过函数返回值输出结果的函数原型在非AIV上调用时返回对应矢量类型的默认构造值。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- `mask`需通过掩码设置接口预先赋值后再传入，未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- `mask`掩码位为0时，`dst`对应元素置0。
- 参数`dst_quarter_pos`需要传入编译期常量，而不能传入变量。
- 目的操作数的位宽比源操作数小，当目的操作数的某部分被写入有效元素后，其余部分将置0。
- 使用饱和或非饱和模式时，需要配置`ctrl`寄存器，详细说明请参见[asc_set_ctrl](../../spr/asc_set_ctrl.md)。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t BUFFER_BYTES = 256;

__simd_vf__ inline void convert_vf(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_float src;
    vector_fp8_e4m3fn_t dst;
    asc_loadalign(src, reinterpret_cast<__ubuf__ float*>(input));
    asc_loadalign(dst, reinterpret_cast<__ubuf__ fp8_e4m3fn_t*>(output));
    asc_float2e4m3_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp8_e4m3fn_t*>(output), dst, mask);
}

__global__ __vector__ void asc_float2e4m3_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES];
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    convert_vf(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input(BUFFER_BYTES, 0);
    std::vector<uint8_t> output(BUFFER_BYTES, 0xff);
    std::vector<uint8_t> golden(BUFFER_BYTES, 0);
    constexpr uint8_t input_pattern[] = {0x00, 0x00, 0x80, 0x3f};
    constexpr uint8_t golden_pattern[] = {0x38, 0x00, 0x00, 0x00};
    for (size_t i = 0; i < BUFFER_BYTES; ++i) {
        input[i] = input_pattern[i % sizeof(input_pattern)];
        golden[i] = golden_pattern[i % sizeof(golden_pattern)];
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), input.size() * sizeof(uint8_t), ACL_MEM_MALLOC_HUGE_FIRST);
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(uint8_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(uint8_t), input.data(), input.size() * sizeof(uint8_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_float2e4m3_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint8_t), output_device, output.size() * sizeof(uint8_t),
        ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_float2e4m3_rn passed." : "[Failed] asc_float2e4m3_rn failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
