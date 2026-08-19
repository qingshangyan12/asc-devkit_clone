# asc_loadalign_brc_elem2datablock_postupdate

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

从Unified Buffer（UB）连续读取8个元素，并将每个元素广播到目的矢量数据寄存器对应的一个`DataBlock`（32字节）中。`b16`类型的实际读取地址需16字节对齐，`b32`类型的实际读取地址需32字节对齐。搬运过程中数据格式和内容保持不变。搬运完成后启用Post Update模式，自动更新源地址指针。

本接口仅在AIV上生效，非AIV调用直接返回。

## 函数原型

### 立即数偏移搬入模式

```c
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(vector_<dtype>& dst,
                                                                        __ubuf__ <dtype>*& src,
                                                                        int32_t offset)
```

### dtype支持数据类型

dtype支持的数据类型为`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：half类型。
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(vector_half& dst,
                                                                        __ubuf__ half*& src,
                                                                        int32_t offset)
```

## 参数说明

### 立即数偏移搬入模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。 |
| src | 输入/输出 | 源UB地址，实际读取地址需满足对齐要求：b16类型需16字节对齐，b32类型需32字节对齐。 |
| offset | 输入 | Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- b16类型的实际读取地址和偏移后的UB地址必须16字节对齐，b32类型的实际读取地址和偏移后的UB地址必须32字节对齐，并且在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

将以下代码保存为`example.asc`后，执行对应的编译运行命令。

<!-- npu="950" id8 -->

在Ascend 950上，编译运行命令如下，NPU架构配置为`dav-3510`：

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
constexpr uint32_t INPUT_ELEMENTS = 16;
constexpr uint32_t OUTPUT_ELEMENTS = 256;
constexpr uint32_t INPUT_BYTES = INPUT_ELEMENTS * sizeof(uint16_t);
constexpr uint32_t OUTPUT_BYTES = OUTPUT_ELEMENTS * sizeof(uint16_t);

__simd_vf__ inline void asc_loadalign_brc_elem2datablock_postupdate_vf(
    __ubuf__ uint16_t* output, __ubuf__ uint16_t* input)
{
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    vector_uint16_t value;
    __ubuf__ uint16_t* src = input;
    asc_loadalign_brc_elem2datablock_postupdate(value, src, 8);
    asc_storealign(output, value, mask);
    asc_loadalign_brc_elem2datablock_postupdate(value, src, 8);
    asc_storealign(output + 128, value, mask);
}

__global__ __vector__ void asc_loadalign_brc_elem2datablock_postupdate_kernel(
    __gm__ uint16_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint16_t input_local[INPUT_ELEMENTS];
    __ubuf__ uint16_t output_local[OUTPUT_ELEMENTS];
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<asc_loadalign_brc_elem2datablock_postupdate_vf>(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint16_t> input(INPUT_ELEMENTS);
    std::vector<uint16_t> output(OUTPUT_ELEMENTS, 0xffff);
    std::vector<uint16_t> golden(OUTPUT_ELEMENTS, 0);
    for (uint32_t i = 0; i < INPUT_ELEMENTS; ++i) input[i] = static_cast<uint16_t>(i + 1);
    for (uint32_t call = 0; call < 2; ++call) {
        for (uint32_t block = 0; block < 8; ++block) {
            for (uint32_t i = 0; i < 16; ++i) {
                const uint32_t out = call * 128 + block * 16 + i;
                const uint32_t in = call * 8 + block;
                golden[out] = input[in];
            }
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t* input_device = nullptr;
    uint16_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadalign_brc_elem2datablock_postupdate_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success]" : "[Failed]") << " asc_loadalign_brc_elem2datablock_postupdate example." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
