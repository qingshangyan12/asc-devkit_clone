# asc_loadalign_mask

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

头文件路径为：`"c_api/reg_compute/load/loadalign.h"`。

从Unified Buffer（UB）中32字节对齐的起始地址读取VL/8长度数据，并通过函数返回值返回或写入目的掩码寄存器。搬运过程中数据格式和内容保持不变。本接口提供三种功能模式：

- **连续对齐搬入模式**：从UB源地址搬入数据，由用户自行更新源地址。支持通过函数返回值返回或通过`dst`参数写入结果。
- **立即数偏移搬入模式**：从相对源起始地址偏移指定距离的位置搬入数据，偏移单位为字节。
- **地址寄存器偏移搬入模式**：通过地址寄存器指定相对源起始地址的偏移。需要与[asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md)配合使用。

**图1** 连续搬入掩码寄存器

![](../../figures/capi_loadalign_mask_norm.png)

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

### 连续对齐搬入模式

```c
__simd_callee__ inline vector_bool asc_loadalign_mask(__ubuf__ uint32_t* src)

__simd_callee__ inline void asc_loadalign(vector_bool& dst,
                                          __ubuf__ uint32_t* src)
```

### 立即数偏移搬入模式

```c
__simd_callee__ inline void asc_loadalign(vector_bool& dst,
                                          __ubuf__ uint32_t* src,
                                          int32_t offset)
```

### 地址寄存器偏移搬入模式

```c
__simd_callee__ inline void asc_loadalign(vector_bool& dst,
                                          __ubuf__ uint32_t* src,
                                          addr_reg offset)
```

## 参数说明

### 连续对齐搬入模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的掩码寄存器，类型为`vector_bool`，仅无返回值类型接口包含该参数。 |
| src | 输入 | 源UB地址，实际读取地址必须按32字节对齐，搬入VL/8长度数据。 |

### 立即数偏移搬入模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的掩码寄存器，类型为`vector_bool`。 |
| src | 输入 | 源UB地址，实际读取地址必须按32字节对齐，搬入VL/8长度数据。 |
| offset | 输入 | 相对`src`起始地址的偏移，单位为字节。 |

### 地址寄存器偏移搬入模式

**表3** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的掩码寄存器，类型为`vector_bool`。 |
| src | 输入 | 源UB地址，实际读取地址必须按32字节对齐，搬入VL/8长度数据。 |
| offset | 输入 | 地址寄存器，类型为`addr_reg`，必须通过`asc_update_addr_reg`生成。该寄存器记录相对`src`起始地址的偏移，使用b32模式。 |

## 返回值说明

`asc_loadalign_mask`返回保存连续对齐搬入结果的掩码寄存器，类型为`vector_bool`。其余原型无返回值。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 各功能模式下的实际读取地址必须按32字节对齐，且实际读取范围必须在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化。源地址偏移后的读取范围不可超过实际可用容量，否则会报错。详细说明请参见[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化。
- 使用地址寄存器偏移搬入模式前，必须先调用`asc_update_addr_reg`接口生成地址寄存器，并使用b32模式。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```c
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t MASK_BYTES = 32;

__simd_vf__ inline void asc_loadalign_mask_vf(
    __ubuf__ uint32_t* output, __ubuf__ uint32_t* mask_input)
{
    vector_bool mask = asc_loadalign_mask(mask_input);
    asc_storealign(output, mask);
}

__global__ __vector__ void asc_loadalign_mask_kernel(
    __gm__ uint32_t* output, __gm__ uint32_t* mask_input)
{
    asc_init();
    __ubuf__ uint32_t output_local[MASK_BYTES / sizeof(uint32_t)];
    __ubuf__ uint32_t mask_local[MASK_BYTES / sizeof(uint32_t)];
    asc_copy_gm2ub_align(mask_local, mask_input, MASK_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<asc_loadalign_mask_vf>(output_local, mask_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, MASK_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint32_t> output(MASK_BYTES / sizeof(uint32_t), 0);
    std::vector<uint32_t> mask_input(MASK_BYTES / sizeof(uint32_t), 0x55555555u);

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* output_device = nullptr;
    uint32_t* mask_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), MASK_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&mask_device), MASK_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(mask_device, MASK_BYTES, mask_input.data(), MASK_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadalign_mask_kernel<<<1, 0>>>(output_device, mask_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), MASK_BYTES, output_device, MASK_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == mask_input;
    std::cout << (passed ? "[Success]" : "[Failed]") << " asc_loadalign_mask example." << std::endl;
    aclrtFree(output_device);
    aclrtFree(mask_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
