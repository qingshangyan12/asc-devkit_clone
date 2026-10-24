# asc_loadalign_deintlv

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

头文件路径为：`"c_api/experimental/loadalign_exp.h"`。

本接口为试验接口，在后续版本中可能会调整或移除，不保证后续兼容性。编译时需定义`CANN_ASC_USE_EXPERIMENTAL`宏，并显式包含指定的试验头文件。

从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，通过返回结构体的`first`和`second`成员分别返回偶数下标和奇数下标元素。源地址由用户自行更新，仅提供无偏移的对齐搬入模式。

本接口为Reg矢量搬运接口，仅在AIV上生效。

通过引用参数输出结果的正式接口请参见[asc_loadalign_deintlv](../reg_compute/load/asc_loadalign_deintlv.md)。

## 函数原型

```c
// 通过函数返回值返回结果（占位符形式）
__simd_callee__ inline asc_<dtype>_<dtype>_pair asc_loadalign_deintlv(__ubuf__ <dtype>* src)
```

### dtype支持数据类型

`dtype`取值为：`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

当`dtype`为`int4b_t`时，返回类型为`asc_int4x2_t_int4x2_t_pair`，成员类型均为`vector_int4x2_t`。

### 函数原型典型示例

```c
// 通过函数返回值返回结果示例：对float数据执行解交织搬入。
__simd_callee__ inline asc_float_float_pair asc_loadalign_deintlv(__ubuf__ float* src)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| src | 输入 | 源UB地址，实际读取地址必须按32字节对齐。 |

## 返回值说明

返回[双返回值结构体](pair.md)`asc_<dtype>_<dtype>_pair`，两个成员的类型均为`vector_<dtype>`。使用变量`result`接收返回值时，`result.first`保存源数据中的偶数下标元素，`result.second`保存奇数下标元素。`dtype`为`int4b_t`时，返回类型为`asc_int4x2_t_int4x2_t_pair`，成员类型为`vector_int4x2_t`。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../reg_compute/overview.md#通用约束)。
- 实际读取地址必须按32字节对齐，且实际读取范围必须在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_compute/reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng -DCANN_ASC_USE_EXPERIMENTAL example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

本示例处理1024个`uint8_t`元素，`one_repeat_size`表示一个寄存器可容纳的元素个数。每次Repeat读取`2 * one_repeat_size`个元素，循环执行两次解交织搬入，Host按每次Repeat的范围生成预期结果。

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "c_api/experimental/loadalign_exp.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t INPUT_ELEMENTS = 1024;
constexpr uint32_t OUTPUT_ELEMENTS = 1024;
constexpr uint32_t INPUT_BYTES = INPUT_ELEMENTS * sizeof(uint8_t);
constexpr uint32_t OUTPUT_BYTES = OUTPUT_ELEMENTS * sizeof(uint8_t);

__simd_vf__ inline void asc_loadalign_deintlv_vf(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input,
                                                 uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    for (uint16_t i = 0; i < repeat_time; ++i) {
        auto result = asc_loadalign_deintlv(input + i * 2 * one_repeat_size);
        asc_storealign(output + i * 2 * one_repeat_size, result.first, mask);
        asc_storealign(output + i * 2 * one_repeat_size + one_repeat_size, result.second, mask);
    }
}

__global__ __vector__ void asc_loadalign_deintlv_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t input_local[INPUT_ELEMENTS];
    __ubuf__ uint8_t output_local[OUTPUT_ELEMENTS];
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    uint16_t one_repeat_size = asc_get_vf_len() / sizeof(uint8_t);
    uint16_t repeat_time = OUTPUT_ELEMENTS / (2 * one_repeat_size);
    asc_vf_call<asc_loadalign_deintlv_vf>(output_local, input_local, one_repeat_size, repeat_time);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input(INPUT_ELEMENTS);
    std::vector<uint8_t> output(OUTPUT_ELEMENTS, 0xff);
    std::vector<uint8_t> golden(OUTPUT_ELEMENTS, 0);
    for (uint16_t i = 0; i < INPUT_ELEMENTS; ++i) input[i] = static_cast<uint8_t>(i % 251 + 1);
    const uint32_t one_repeat_size = 256 / sizeof(uint8_t);
    for (uint32_t repeat = 0; repeat < OUTPUT_ELEMENTS / (2 * one_repeat_size); ++repeat) {
        const uint32_t base = repeat * 2 * one_repeat_size;
        for (uint32_t offset = 0; offset < one_repeat_size; ++offset) {
            golden[base + offset] = input[base + 2 * offset];
            golden[base + one_repeat_size + offset] = input[base + 2 * offset + 1];
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadalign_deintlv_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success]" : "[Failed]") << " asc_loadalign_deintlv example." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
