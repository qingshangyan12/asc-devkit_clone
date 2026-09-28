# asc_storealign_mask_pack

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

头文件路径为：`"c_api/reg_compute/store/storealign.h"`。

将掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。硬件每间隔1bit丢弃1bit，将保留的bit连续压缩为16字节数据写入目的地址。本接口不支持配置额外的执行掩码，提供以下三种模式：

- **连续对齐搬出模式**：将压缩后的数据搬出到UB目的地址，由用户自行更新目的地址。
- **立即数偏移搬出模式**：搬出到相对目的起始地址偏移指定距离的位置，偏移单位为字节。
- **地址寄存器偏移搬出模式**：通过地址寄存器指定相对目的起始地址的偏移。需要与[asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md)配合使用。

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

### 连续对齐搬出模式

```c
__simd_callee__ inline void asc_storealign_pack(__ubuf__ <dtype>* dst,
                                                vector_bool src)
```

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst,
                                                vector_bool src)
```

### 立即数偏移搬出模式

```c
__simd_callee__ inline void asc_storealign_pack(__ubuf__ <dtype>* dst,
                                                vector_bool src,
                                                int32_t offset)
```

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst,
                                                vector_bool src,
                                                int32_t offset)
```

### 地址寄存器偏移搬出模式

```c
__simd_callee__ inline void asc_storealign_pack(__ubuf__ <dtype>* dst,
                                                vector_bool src,
                                                addr_reg offset)
```

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst,
                                                vector_bool src,
                                                addr_reg offset)
```

### dtype支持数据类型

`dtype`支持的数据类型为`uint8_t`、`uint16_t`、`uint32_t`。

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的UB地址，类型为`__ubuf__ <dtype>*`，实际写入地址必须按16字节对齐。 |
| src | 输入 | 源掩码寄存器，类型为`vector_bool`。 |
| offset | 输入 | 仅偏移搬出模式包含该参数。`int32_t`类型的偏移单位为字节；`addr_reg`类型的偏移由配套的地址寄存器构造接口预先构造。 |

掩码寄存器和地址寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 各功能模式下的实际写入地址必须按16字节对齐，且写入范围必须在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化。目的地址偏移后的写入范围不可超过实际可用容量，否则会报错。详细说明请参见[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 使用地址寄存器偏移搬出模式前，必须先调用`asc_update_addr_reg`接口生成地址寄存器。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

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
constexpr uint32_t PACKED_BYTES = 16;
constexpr uint32_t REPEAT_TIMES = 2;
constexpr uint32_t STEP_BYTES = 32;
constexpr uint32_t SECTION_BYTES = REPEAT_TIMES * STEP_BYTES;
constexpr uint32_t OUTPUT_BYTES = 3 * SECTION_BYTES;
constexpr uint8_t SENTINEL = 0xa5;

__simd_vf__ inline void pack_vf(__ubuf__ uint8_t* output, __ubuf__ uint32_t* mask_input)
{
    vector_bool src = asc_loadalign_mask(mask_input);
    for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
        // 三种目的指针类型均将32字节掩码数据压缩为16字节后搬出。
        asc_storealign_pack(output + repeat * STEP_BYTES, src);
        asc_storealign_pack(reinterpret_cast<__ubuf__ uint16_t*>(
            output + SECTION_BYTES + repeat * STEP_BYTES), src);
        asc_storealign_pack(reinterpret_cast<__ubuf__ uint32_t*>(
            output + 2 * SECTION_BYTES + repeat * STEP_BYTES), src);
    }
}

__global__ __vector__ void asc_storealign_mask_pack_kernel(
    __gm__ uint8_t* output, __gm__ uint32_t* mask_input)
{
    asc_init();
    __ubuf__ uint8_t output_local[OUTPUT_BYTES];
    __ubuf__ uint32_t mask_local[MASK_BYTES / sizeof(uint32_t)];
    asc_copy_gm2ub(output_local, output, OUTPUT_BYTES);
    asc_copy_gm2ub(mask_local, mask_input, MASK_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    pack_vf(output_local, mask_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint32_t> mask_input(MASK_BYTES / sizeof(uint32_t), 0x55555555u);
    std::vector<uint8_t> output(OUTPUT_BYTES, SENTINEL);
    std::vector<uint8_t> expected(OUTPUT_BYTES, SENTINEL);
    for (uint32_t section = 0; section < 3; ++section) {
        for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
            for (uint32_t i = 0; i < PACKED_BYTES; ++i) {
                expected[section * SECTION_BYTES + repeat * STEP_BYTES + i] = 0xff;
            }
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* mask_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&mask_device), MASK_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(mask_device, MASK_BYTES, mask_input.data(), MASK_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, OUTPUT_BYTES, output.data(), OUTPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_storealign_mask_pack_kernel<<<1, 0>>>(output_device, mask_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == expected;
    std::cout << (passed ? "[Success] asc_storealign_pack passed."
                         : "[Failed] asc_storealign_pack output mismatch.")
              << std::endl;
    aclrtFree(mask_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
