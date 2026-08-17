# asc_loadunalign_postupdate

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

将Unified Buffer（UB）中起始地址无需32字节对齐的VL长度连续数据搬入矢量数据寄存器。接口将非对齐寄存器中缓存的前置数据与从UB读取的后续数据拼接，得到从本次有效起始地址开始的VL长度数据。搬入完成后，接口自动更新源地址指针或地址寄存器记录的地址偏移，并将非对齐寄存器更新为包含下一次有效起始地址所需前置数据的缓存。本接口提供两种功能模式：

- **源地址Post Update搬入模式**：以当前源地址作为本次有效起始地址。搬入完成后，将源地址增加`count × sizeof(dtype)`字节，并同步维护下一次搬入所需的非对齐寄存器状态。

- **地址寄存器Post Update搬入模式**：以UB源基地址与地址寄存器记录的偏移之和作为有效起始地址。搬入完成后，地址偏移增加`inc × sizeof(dtype)`字节，源基地址保持不变，并同步更新非对齐寄存器。

`asc_update_addr_reg`根据当前循环迭代的索引生成该次迭代的起始偏移；`inc`只更新当前循环迭代内的地址偏移，不影响下一次循环迭代的起始偏移。

## 函数原型

### 源地址Post Update搬入模式

```c
__simd_callee__ inline void asc_loadunalign_postupdate(vector_<dtype>& dst,
                                                       vector_load_unalign& src0,
                                                       __ubuf__ <dtype>*& src1,
                                                       uint32_t count)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。当dtype为`int4b_t`时，`dst`的实际类型为`vector_int4x2_t`。

#### 典型示例

```c
__simd_callee__ inline void asc_loadunalign_postupdate(vector_int8_t& dst,
                                                       vector_load_unalign& src0,
                                                       __ubuf__ int8_t*& src1,
                                                       uint32_t count)
```

### 地址寄存器Post Update搬入模式

```c
__simd_callee__ inline void asc_loadunalign_postupdate(vector_<dtype>& dst,
                                                       vector_load_unalign& src0,
                                                       __ubuf__ <dtype>* src1,
                                                       addr_reg& offset,
                                                       uint32_t inc)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。当dtype为`int4b_t`时，`dst`的实际类型为`vector_int4x2_t`。

#### 典型示例

```c
__simd_callee__ inline void asc_loadunalign_postupdate(vector_int8_t& dst,
                                                       vector_load_unalign& src0,
                                                       __ubuf__ int8_t* src1,
                                                       addr_reg& offset,
                                                       uint32_t inc)
```

## 参数说明

### 源地址Post Update搬入模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。 |
| src0 | 输入/输出 | 非对齐寄存器，大小为32字节。本接口调用后，`src0`更新为包含下一次有效起始地址所需前置数据的缓存。 |
| src1 | 输入/输出 | 源UB地址，实际读取地址无需32字节对齐。 |
| count | 输入 | Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`指针增加`count × sizeof(dtype)`字节。 |

### 地址寄存器Post Update搬入模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。 |
| src0 | 输入/输出 | 非对齐寄存器，大小为32字节。本接口调用后，`src0`更新为包含当前循环迭代内下一次有效起始地址所需前置数据的缓存。 |
| src1 | 输入 | 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址无需32字节对齐。 |
| offset | 输入/输出 | 地址寄存器，类型为`addr_reg`。该寄存器记录的实际字节偏移由创建地址寄存器时选择的位宽模式决定：`b8`、`b16`、`b32`模式分别以1字节、2字节、4字节为偏移单位。本接口完成搬入后会根据`inc`更新`offset`。 |
| inc | 输入 | Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的地址偏移量增加`inc × sizeof(dtype)`字节。 |

## 返回值说明

无

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- `src1`起始地址或`src1`与`offset`确定的实际访问地址无需32字节对齐，但实际访问范围必须在UB地址空间内，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，必须插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 源地址Post Update搬入模式

- 首次搬入前必须调用[asc_loadunalign_pre](asc_loadunalign_pre.md)，且初始化和紧随其后的首次搬入必须使用相同的`src1`地址。
- 后续调用继续使用上一次调用更新后的`src1`时，可以复用更新后的`src0`；如果用户另外修改了`src1`，必须根据新地址重新初始化`src0`。

### 地址寄存器Post Update搬入模式

- 必须先调用`asc_update_addr_reg`接口。
- `asc_update_addr_reg`生成当前循环迭代的起始偏移后，必须在该次迭代首次搬入前使用相同的`src1`和`offset`调用[asc_loadunalign_pre](asc_loadunalign_pre.md)重新初始化`src0`。
- 同一次循环迭代内的后续调用继续使用接口按`inc`更新后的`offset`时，可以复用更新后的`src0`。
- 当前循环迭代中最后一次使用`offset`时，可将`inc`设置为0。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->

在Ascend 950上，编译运行命令如下，NPU架构配置为`dav-3510`：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t INPUT_BYTES = 544;
constexpr uint32_t OUTPUT_BYTES = 1024;

__simd_vf__ inline void asc_loadunalign_postupdate_vf(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_load_unalign unalign;
    __ubuf__ uint8_t* src = input + 1;
    asc_loadunalign_pre(unalign, src);
    vector_uint8_t value;
    asc_loadunalign_postupdate(value, unalign, src, 256);
    asc_storealign(output, value, mask);
    asc_loadunalign_postupdate(value, unalign, src, 256);
    asc_storealign(output + 256, value, mask);

    __ubuf__ uint8_t* base = input + 1;
    for (uint16_t i = 0; i < 1; ++i) {
        addr_reg offset = asc_update_addr_reg_b8(256);
        vector_load_unalign offset_unalign;
        asc_loadunalign_pre(offset_unalign, base, offset);
        asc_loadunalign_postupdate(value, offset_unalign, base, offset, 256);
        asc_storealign(output + 512, value, mask);
        asc_loadunalign_postupdate(value, offset_unalign, base, offset, 0);
        asc_storealign(output + 768, value, mask);
    }
}

__global__ __vector__ void asc_loadunalign_postupdate_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t input_local[INPUT_BYTES];
    __ubuf__ uint8_t output_local[OUTPUT_BYTES];
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<asc_loadunalign_postupdate_vf>(output_local, input_local);
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
    for (uint32_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i % 251 + 1);
    for (uint32_t i = 0; i < 512; ++i) {
        golden[i] = input[i + 1];
        golden[i + 512] = input[i + 1];
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadunalign_postupdate_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success]" : "[Failed]") << " asc_loadunalign_postupdate example." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
