# asc_loadalign_upsample

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

从Unified Buffer（UB）读取连续数据并进行2倍上采样。

- 当`dst`为矢量数据寄存器时，实际读取地址需32字节对齐，读取VL/2长度数据，将每个源元素重复两次后写入VL长度数据。
- 当`dst`为掩码寄存器时，实际读取地址需16字节对齐，读取VL/16长度数据，将每个bit重复两次后写入VL/8长度数据。

本接口提供三种功能模式：

- **连续对齐搬入模式**：将UB源地址的数据上采样后搬入到矢量数据寄存器或掩码寄存器，由用户自行更新源地址。
- **立即数偏移搬入模式**：从相对源起始地址偏移指定距离的位置搬入数据。本接口不会自动更新源地址。
- **地址寄存器偏移搬入模式**：通过地址寄存器指定相对源起始地址的偏移，常用于Hardware Loop内偏移随循环计数变化的对齐搬入场景。需要与[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)配合使用。

本接口仅在AIV上生效，非AIV调用直接返回。

## 函数原型

### 连续对齐搬入模式

```c
// 搬入矢量数据寄存器。
__simd_callee__ inline void asc_loadalign_upsample(vector_<dtype>& dst,
                                                   __ubuf__ <dtype>* src)
// 搬入掩码寄存器。
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst,
                                                   __ubuf__ uint32_t* src)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`。当dtype为`int4b_t`时，dst的实际类型为`vector_int4x2_t`。

#### 典型示例

```c
// 示例：从UB起始地址搬入half数据并按上采样模式扩位填充half矢量寄存器。
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst,
                                                   __ubuf__ half* src)
```

### 立即数偏移搬入模式

```c
// 搬入矢量数据寄存器。
__simd_callee__ inline void asc_loadalign_upsample(vector_<dtype>& dst,
                                                   __ubuf__ <dtype>* src,
                                                   int32_t offset)
// 搬入掩码寄存器。
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst,
                                                   __ubuf__ uint32_t* src,
                                                   int32_t offset)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`。当dtype为`int4b_t`时，dst的实际类型为`vector_int4x2_t`。

#### 典型示例

```c
// 示例：带偏移从UB搬入half数据并按上采样模式扩位填充。
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst,
                                                   __ubuf__ half* src,
                                                   int32_t offset)
```

### 地址寄存器偏移搬入模式

```c
// 搬入矢量数据寄存器。
__simd_callee__ inline void asc_loadalign_upsample(vector_<dtype>& dst,
                                                   __ubuf__ <dtype>* src,
                                                   addr_reg offset)
// 搬入掩码寄存器。
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst,
                                                   __ubuf__ uint32_t* src,
                                                   addr_reg offset)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`。当dtype为`int4b_t`时，dst的实际类型为`vector_int4x2_t`。

#### 典型示例

```c
// 示例：按地址寄存器偏移从UB搬入half数据并按上采样模式扩位填充。
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst,
                                                   __ubuf__ half* src,
                                                   addr_reg offset)
```

## 参数说明

### 连续对齐搬入模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器或掩码寄存器。<br>&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。<br>&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。 |
| src | 输入 | 源UB地址。<br>&bull; 当`dst`为矢量数据寄存器，实际读取地址需32字节对齐。<br>&bull; 当`dst`为掩码寄存器，实际读取地址需16字节对齐。 |

### 立即数偏移搬入模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器或掩码寄存器。<br>&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。<br>&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。 |
| src | 输入 | 源UB地址。<br>&bull; 当`dst`为矢量数据寄存器，实际读取地址需32字节对齐。<br>&bull; 当`dst`为掩码寄存器，实际读取地址需16字节对齐。 |
| offset | 输入 | 相对`src`起始地址的偏移。<br>&bull; 当`dst`为矢量数据寄存器，单位为元素个数。<br>&bull; 当`dst`为掩码寄存器，单位为字节。 |

### 地址寄存器偏移搬入模式

**表3** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器或掩码寄存器。<br>&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。<br>&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。 |
| src | 输入 | 源UB地址。<br>&bull; 当`dst`为矢量数据寄存器，实际读取地址需32字节对齐。<br>&bull; 当`dst`为掩码寄存器，实际读取地址需16字节对齐。 |
| offset | 输入 | 地址寄存器，类型为`addr_reg`，必须通过`asc_update_addr_reg`生成。该寄存器记录的实际字节偏移由创建地址寄存器时选择的位宽模式决定：b8、b16、b32模式分别以1字节、2字节、4字节为偏移单位。当`dst`为`vector_bool`时，应使用b32模式。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- 当`dst`为矢量数据寄存器时，实际读取地址和偏移后的UB地址必须32字节对齐；当`dst`为掩码寄存器时，实际读取地址和偏移后的UB地址必须按16字节对齐。实际访问范围必须在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 地址寄存器偏移搬入模式

必须先调用`asc_update_addr_reg`接口。

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
constexpr uint32_t INPUT_BYTES = 128;
constexpr uint32_t OUTPUT_BYTES = 256;

__simd_vf__ inline void asc_loadalign_upsample_vf(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_uint8_t value;
    asc_loadalign_upsample(value, input);
    asc_storealign(output, value, mask);
}

__global__ __vector__ void asc_loadalign_upsample_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t input_local[INPUT_BYTES];
    __ubuf__ uint8_t output_local[OUTPUT_BYTES];
    asc_copy_gm2ub_align(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<asc_loadalign_upsample_vf>(output_local, input_local);
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
    for (uint32_t i = 0; i < OUTPUT_BYTES; ++i) golden[i] = input[i / 2];

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_loadalign_upsample_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == golden;
    std::cout << (passed ? "[Success]" : "[Failed]") << " asc_loadalign_upsample example." << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
