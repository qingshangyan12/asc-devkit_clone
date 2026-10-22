# asc_pack_datablock

## 产品支持情况

<!-- npu="960" id8 -->
- Ascend 960PR&960DT系列产品：支持
<!-- end id8 -->
<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：不支持
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

头文件路径为：`"c_api/reg_compute/compute/reg_permute_sel.h"`。

按照DataBlock粒度对矢量数据寄存器中的数据进行打包，源操作数和目的操作数的数据类型一致。每个DataBlock长度为32B。

本接口为Reg矢量计算接口，仅在AIV上生效，仅支持Ascend 960PR&960DT系列产品的`dav-9201`架构。

## 函数原型

```cpp
// 通过引用参数输出结果。
__simd_callee__ inline void asc_pack_datablock(vector_<dtype>& dst, vector_<dtype> src)
```

```cpp
// 通过函数返回值返回结果。
__simd_callee__ inline vector_<dtype> asc_pack_datablock(vector_<dtype> src)
```

其中，`dtype`为占位符，`src`和`dst`的数据类型必须相同，支持的数据类型如下。

### dtype支持数据类型

| dtype |
| --- |
| uint16_t |
| int16_t |
| half |
| uint32_t |
| int32_t |
| float |
| uint64_t |
| int64_t |

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --------- | ----- | ---------------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

- 通过引用参数输出结果的函数原型无返回值，打包结果写入`dst`。
- 通过函数返回值返回结果的函数原型返回打包结果，类型与`src`一致。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 仅支持Ascend 960PR&960DT系列产品的`dav-9201`架构。
- 源操作数和目的操作数的数据类型必须一致。

## 调用示例

以下示例以`uint16_t`为例。输入寄存器包含8个32B的DataBlock，每个DataBlock的首个元素分别为`0`、`16`、…、`112`；打包后，这8个首元素连续写入结果寄存器的前8个元素。

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
constexpr uint32_t ELEMENT_COUNT = 128;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(uint16_t);
constexpr uint32_t DATABLOCK_BYTES = 32;
constexpr uint32_t VALID_RESULT_COUNT = BUFFER_BYTES / DATABLOCK_BYTES;
constexpr uint32_t ELEMENTS_PER_DATABLOCK = DATABLOCK_BYTES / sizeof(uint16_t);

__simd_vf__ inline void pack_datablock_vf(__ubuf__ uint16_t* dst_addr, __ubuf__ uint16_t* src_addr)
{
    vector_uint16_t src;
    vector_uint16_t dst;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    asc_loadalign(src, src_addr);
    asc_pack_datablock(dst, src);
    asc_storealign(dst_addr, dst, mask);
}

__global__ __vector__ void asc_pack_datablock_kernel(__gm__ uint16_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint16_t input_local[ELEMENT_COUNT];
    __ubuf__ uint16_t output_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    pack_datablock_vf(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint16_t> input(ELEMENT_COUNT);
    std::vector<uint16_t> output(ELEMENT_COUNT, 0);
    std::vector<uint16_t> golden(VALID_RESULT_COUNT);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input[i] = static_cast<uint16_t>(i);
    }
    for (uint32_t i = 0; i < VALID_RESULT_COUNT; ++i) {
        golden[i] = input[i * ELEMENTS_PER_DATABLOCK];
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t* input_device = nullptr;
    uint16_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BUFFER_BYTES, input.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_pack_datablock_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BUFFER_BYTES, output_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    for (uint32_t i = 0; i < VALID_RESULT_COUNT; ++i) {
        if (output[i] != golden[i]) {
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_pack_datablock passed."
                         : "[Failed] asc_pack_datablock failed.")
              << std::endl;
    aclrtFree(output_device);
    aclrtFree(input_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
