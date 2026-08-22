# asc_loadunalign_pre

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

本接口根据Unified Buffer（UB）中按dtype对齐的起始地址，初始化[非对齐寄存器](../reg_data_types/data_type_definition.md#非对齐寄存器)，作为后续非对齐搬入的前置缓存。该阶段仅完成非对齐寄存器初始化，不会将数据搬入矢量数据寄存器。

设本次初始化对应的有效起始字节地址为`src_start_addr`，将`src_start_addr`向低地址方向对齐到32字节边界，得到`aligned_src_start_addr`。本接口将UB中字节地址范围`[aligned_src_start_addr, aligned_src_start_addr + 32)`的32字节数据搬入非对齐寄存器。

本接口输出的非对齐寄存器作为[asc_loadunalign](asc_loadunalign.md)或[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md)的输入非对齐寄存器。

本接口提供两种功能模式：

- **源地址预处理模式**：以UB源地址作为起始地址，为[asc_loadunalign](asc_loadunalign.md)或[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md)的连续非对齐搬入模式准备前置数据缓存。
- **地址寄存器偏移预处理模式**：以UB源地址与地址寄存器记录的元素偏移量共同确定起始地址，为`asc_loadunalign_postupdate`的地址寄存器偏移搬入模式准备前置数据缓存。需要与[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)配合使用。

## 函数原型

### 源地址预处理模式

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ <dtype>* src)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

#### 函数原型典型示例

```c
// 示例：float类型。
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ float* src)
```

### 地址寄存器偏移预处理模式

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ <dtype>* src,
                                                addr_reg offset)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

#### 函数原型典型示例

```c
// 示例：float类型。
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ float* src,
                                                addr_reg offset)
```

## 参数说明

### 源地址预处理模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。 |
| src | 输入 | 源UB地址，必须按dtype对齐。 |

### 地址寄存器偏移预处理模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。 |
| src | 输入 | 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。 |
| offset | 输入 | 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。 |

## 返回值说明

无

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- 实际访问地址在接口内部向低地址方向对齐到32字节边界后读取数据。传入的`src`或`src`与`offset`确定的地址必须按dtype对齐，对齐后的32字节读取范围必须在UB地址空间内，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，必须插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 源地址预处理模式

- 本接口和紧随其后的搬入必须使用相同的`src`地址。
- 与[asc_loadunalign](asc_loadunalign.md)配合使用时，每次搬入前都必须调用本接口重新初始化。
- 与[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md)的连续非对齐搬入模式配合使用时，只有继续使用上次调用自动更新后的`src`时才可以复用；如果另外修改了`src`，必须重新初始化。

### 地址寄存器偏移预处理模式

- 每次循环迭代中，必须先调用`asc_update_addr_reg`生成该次迭代的起始偏移，再调用本接口初始化非对齐寄存器。
- 当dtype为`int64_t`时，由于地址寄存器更新接口不支持b64模式，需要使用[asc_update_addr_reg_b32](../reg_data_types/asc_update_addr_reg.md)生成地址寄存器。一个b64元素占用两个b32寻址单元，因此传入`asc_update_addr_reg_b32`的各维偏移量必须设置为期望的b64元素偏移量的2倍。
- 本接口和紧随其后的首次搬入必须使用相同的`src`和`offset`。
- 同一循环迭代内，后续搬入继续使用[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md)按`inc`更新后的`offset`时，可以复用更新后的`src0`；如果`src`或`offset`被另行修改，必须根据修改后的实际读取地址重新初始化`src0`。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

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
__aicore__ constexpr uint32_t ceil_division(uint32_t numerator, uint32_t denominator)
{
    return (numerator + denominator - 1) / denominator;
}

constexpr uint32_t one_repeat_size = 256;
constexpr uint32_t repeat_time = 2;
constexpr uint32_t total_length = repeat_time * one_repeat_size;
constexpr uint32_t INPUT_BYTES = repeat_time * one_repeat_size + 32;
constexpr uint32_t OUTPUT_BYTES = 2 * repeat_time * one_repeat_size;

__simd_vf__ inline void asc_loadunalign_pre_vf(__ubuf__ uint8_t* output,
                                               __ubuf__ uint8_t* input,
                                               uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_load_unalign unalign;
    __ubuf__ uint8_t* src = input + 1;
    vector_uint8_t value;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadunalign_pre(unalign, src);
        asc_loadunalign(value, unalign, src);
        asc_storealign(output + i * one_repeat_size, value, mask);
        src += one_repeat_size;
    }

    for (uint16_t i = 0; i < repeat_time; ++i) {
        addr_reg offset = asc_update_addr_reg_b8(one_repeat_size);
        vector_load_unalign offset_unalign;
        __ubuf__ uint8_t* base = input + 1;
        asc_loadunalign_pre(offset_unalign, base, offset);
        asc_loadunalign_postupdate(value, offset_unalign, base, offset, 0);
        asc_storealign(output + (repeat_time + i) * one_repeat_size, value, mask);
    }
}

__global__ __vector__ void asc_loadunalign_pre_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t input_local[INPUT_BYTES];
    __ubuf__ uint8_t output_local[OUTPUT_BYTES];
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    uint16_t one_repeat_size = asc_get_vf_len() / sizeof(uint8_t);
    uint16_t repeat_time = ceil_division(total_length, one_repeat_size);
    asc_vf_call<asc_loadunalign_pre_vf>(output_local, input_local, one_repeat_size, repeat_time);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input(INPUT_BYTES);
    std::vector<uint8_t> output(OUTPUT_BYTES, 0xff);
    std::vector<uint8_t> golden(OUTPUT_BYTES, 0);
    for (uint16_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i % 251 + 1);
    for (uint16_t i = 0; i < repeat_time * one_repeat_size; ++i) {
        golden[i] = input[i + 1];
        golden[repeat_time * one_repeat_size + i] = input[i + 1];
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadunalign_pre_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success]" : "[Failed]") << " asc_loadunalign_pre example." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
