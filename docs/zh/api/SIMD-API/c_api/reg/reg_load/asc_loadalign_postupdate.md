# asc_loadalign_postupdate

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

将Unified Buffer（UB）中起始地址32字节对齐的连续数据搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。搬运完成后启用Post Update模式，自动更新源地址指针。本接口提供两种功能模式：

- 连续对齐搬入：
    - 当dst为矢量数据寄存器，搬入VL长度数据。搬运完成后启用Post Update模式，自动将传入的源地址指针向后偏移`offset * sizeof(dtype)`字节，无需手动偏移地址。
    - 当dst为掩码寄存器，搬入VL/8长度数据。搬运完成后启用Post Update模式，自动将传入的源地址指针向后偏移`offset`字节，无需手动偏移地址。
- 非连续对齐搬入：从当前`src`地址开始，按照`block_stride`指定的DataBlock间隔读取8个DataBlock，搬入矢量数据寄存器。搬入完成后启用Post Update，将`src`地址增加`repeat_stride × 32`字节。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 连续对齐搬入

#### 模板原型（占位符形式）

```c
// 搬入矢量数据寄存器。
__simd_callee__ inline void asc_loadalign_postupdate(vector_<dtype>& dst,
                                                     __ubuf__ <dtype>*& src,
                                                     int32_t offset)
// 搬入掩码寄存器。
__simd_callee__ inline void asc_loadalign_postupdate(vector_bool& dst,
                                                     __ubuf__ uint32_t*& src,
                                                     int32_t offset)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t`。

#### 典型示例

```c
// 示例：float类型搬入。
__simd_callee__ inline void asc_loadalign_postupdate(vector_float& dst,
                                                     __ubuf__ float*& src,
                                                     int32_t offset)
```

### 非连续对齐搬入

#### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_loadalign_postupdate(vector_<dtype>& dst,
                                                     __ubuf__ <dtype>*& src,
                                                     uint16_t block_stride,
                                                     uint16_t repeat_stride,
                                                     vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

#### 典型示例

```c
// 示例：float类型搬入。
__simd_callee__ inline void asc_loadalign_postupdate(vector_float& dst,
                                                     __ubuf__ float*& src,
                                                     uint16_t block_stride,
                                                     uint16_t repeat_stride,
                                                     vector_bool mask)
```

## 参数说明

**表1** 参数说明（连续对齐搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器或掩码寄存器。<br>&bull; 当dst为矢量数据寄存器，dtype须与src一致，搬入VL长度数据。<br>&bull; 当dst为掩码寄存器，搬入VL/8长度数据。 |
| src | 输入/输出 | 源UB地址，实际读取地址需32字节对齐。 |
| offset | 输入 | Post Update步长。接口使用当前`src`地址完成搬入后更新`src`。<br>&bull; 当dst为矢量数据寄存器时，单位为元素个数，`src`指针增加`offset * sizeof(dtype)`字节。<br>&bull; 当dst为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。 |

**表2** 参数说明（非连续对齐搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype须与src一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。 |
| src | 输入/输出 | 源UB地址，实际读取地址需32字节对齐。 |
| block_stride | 输入 | 块间步长，控制一次搬入中相邻32字节数据块之间的源UB地址间隔，单位为32字节（DataBlock）。第i个块（i=0～7）的地址为`src`偏移`i × block_stride × 32`字节。取值范围为[0, 2^16−1]。 |
| repeat_stride | 输入 | Post Update步长，单位为32字节（DataBlock）。接口使用当前`src`地址完成搬入后，将`src`指针增加`repeat_stride × 32`字节。取值范围为[0, 2^16−1]。 |
| mask | 输入 | 掩码寄存器，用于指示在计算过程中哪些元素参与计算。<br>&bull; 因该接口以DataBlock为数据搬运单元，当DataBlock中的任意一个元素被mask筛选成有效元素时，该DataBlock中所有数据都会搬入至矢量数据寄存器。<br>&bull; 当DataBlock中所有元素都被mask筛选成无效元素时，该DataBlock中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- 当dst为矢量数据寄存器时，src与dst的dtype必须完全一致，dtype不一致触发异常。
- 实际读取的UB地址和偏移后的UB地址必须32字节对齐，并且在UB地址空间内且不越界，否则会报错。
- 针对连续对齐搬入：offset取0时，接口仍从当前src地址搬入数据，src指针保持不变，可重复读取同一组源数据。
- 针对非连续对齐搬入：repeat_stride取0时，接口仍从当前src地址搬入数据，src指针保持不变，可重复执行相同的非连续搬入。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。
- 针对非连续对齐搬入：因该接口以DataBlock为数据搬运单元，当DataBlock中的任意一个元素被mask筛选成有效元素时，该DataBlock中所有数据都会搬入至矢量数据寄存器。当DataBlock中所有元素都被mask筛选成无效元素时，该DataBlock中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。

## 调用示例

```cpp
// Build/run (save this example as asc_loadalign_postupdate.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang asc_loadalign_postupdate.asc -lm -o /tmp/asc_loadalign_postupdate && /tmp/asc_loadalign_postupdate
// Architecture: dav-3510

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

constexpr uint32_t BUFFER_BYTES = 256;
__simd_vf__ inline void Load(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    for (uint16_t repeat = 0; repeat < 1; ++repeat) {
    vector_bool arg_0_0;
    asc_loadalign(arg_0_0, reinterpret_cast<__ubuf__ uint32_t*>(input));
    __ubuf__ uint32_t* arg_0_1 = reinterpret_cast<__ubuf__ uint32_t*>(input);
    asc_loadalign_postupdate(arg_0_0, arg_0_1, 0);
    asc_storealign(reinterpret_cast<__ubuf__ uint8_t*>(output), arg_0_0);
    vector_bool arg_1_0;
    asc_loadalign(arg_1_0, reinterpret_cast<__ubuf__ uint32_t*>(input));
    __ubuf__ uint32_t* arg_1_1 = reinterpret_cast<__ubuf__ uint32_t*>(input);
    asc_loadalign_upsample_postupdate(arg_1_0, arg_1_1, 0);
    asc_storealign(reinterpret_cast<__ubuf__ uint8_t*>(output), arg_1_0);
    vector_bool arg_2_0;
    asc_loadalign(arg_2_0, reinterpret_cast<__ubuf__ uint32_t*>(input));
    __ubuf__ uint32_t* arg_2_1 = reinterpret_cast<__ubuf__ uint32_t*>(input);
    asc_loadalign_downsample_postupdate(arg_2_0, arg_2_1, 0);
    asc_storealign(reinterpret_cast<__ubuf__ uint8_t*>(output), arg_2_0);
    }
}
__global__ __vector__ void asc_loadalign_postupdate_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES], input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    Load(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace
int main()
{
    std::vector<uint8_t> input(BUFFER_BYTES), output(BUFFER_BYTES, 0xff);
    for (uint32_t i = 0; i < BUFFER_BYTES; ++i) input[i] = static_cast<uint8_t>(i % 251);
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
    asc_loadalign_postupdate_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint8_t), output_device, output.size() * sizeof(uint8_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input bytes", input);
    print_data("Output bytes", output);
    const bool passed = true;
    std::cout << "[Success] asc_loadalign_postupdate/asc_loadalign_upsample_postupdate/asc_loadalign_downsample_postupdate completed." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
