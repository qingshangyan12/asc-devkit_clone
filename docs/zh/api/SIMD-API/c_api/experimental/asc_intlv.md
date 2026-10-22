# asc_intlv

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

头文件路径为：`"c_api/experimental/reg_permute_sel_exp.h"`。

本接口为试验接口，在后续版本中可能会调整或移除，不保证后续兼容性。编译时需定义`CANN_ASC_USE_EXPERIMENTAL`宏，并显式包含指定的试验头文件。

将两个源操作数矢量数据寄存器中的数据交织后分别写入两个目的矢量数据寄存器。根据操作数类型，本接口提供两种功能模式：

- **操作数为矢量数据寄存器：** 以元素为单位进行交织。`src0`和`src1`前半部分的元素依次交替写入`result.first`，后半部分的元素依次交替写入`result.second`。以`int8_t`类型为例，交织过程如下图所示。
- **操作数为掩码寄存器：** 根据接口后缀指定的粒度进行交织。`asc_intlv_b8`以1bit为一组，`asc_intlv_b16`以2bit为一组，`asc_intlv_b32`以4bit为一组。

**图 1** int8_t类型交织过程

![](../figures/asc_intlv.png)

本接口为Reg矢量计算接口，仅在AIV上生效。

通过引用参数输出结果的正式接口请参见[asc_intlv](../reg_compute/reg_permute_sel/asc_intlv.md)。

## 函数原型

### 矢量数据寄存器交织模式

```c
// 通过函数返回值返回结果（占位符形式）
__simd_callee__ inline asc_<dtype>_<dtype>_pair asc_intlv(vector_<dtype> src0,
                                                       vector_<dtype> src1)
```

#### dtype支持数据类型

`dtype`取值为：`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

#### 函数原型典型示例

```c
// 通过函数返回值返回结果示例：对int8_t矢量数据寄存器执行交织。
__simd_callee__ inline asc_int8_t_int8_t_pair asc_intlv(vector_int8_t src0,
                                                         vector_int8_t src1)
```

### 掩码寄存器交织模式

```c
// 通过函数返回值返回结果
__simd_callee__ inline asc_bool_bool_pair asc_intlv_b8(vector_bool src0,
                                                        vector_bool src1)
__simd_callee__ inline asc_bool_bool_pair asc_intlv_b16(vector_bool src0,
                                                          vector_bool src1)
__simd_callee__ inline asc_bool_bool_pair asc_intlv_b32(vector_bool src0,
                                                          vector_bool src1)
```

## 参数说明

### 矢量数据寄存器交织模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| src0 | 输入 | 源矢量数据寄存器，参与交织的操作数。前半部分元素与`src1`前半部分元素交替写入`result.first`，后半部分元素与`src1`后半部分元素交替写入`result.second`，每对元素中`src0`的元素在前。 |
| src1 | 输入 | 源矢量数据寄存器，参与交织的操作数。数据类型须与`src0`一致，每对元素中`src1`的元素在后。 |

### 掩码寄存器交织模式

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| src0 | 输入 | 源操作数（掩码寄存器）。 |
| src1 | 输入 | 源操作数（掩码寄存器）。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../defs/type/data_type_definition.md)。

## 返回值说明

返回[双返回值结构体](pair.md)，使用变量`result`接收返回值。

- 操作数为矢量数据寄存器时，返回类型为`asc_<dtype>_<dtype>_pair`，两个成员的类型均为`vector_<dtype>`。`result.first`保存交织后的低半部分数据，`result.second`保存高半部分数据。
- 操作数为掩码寄存器时，b8、b16和b32模式的返回类型均为`asc_bool_bool_pair`，两个成员的类型均为`vector_bool`，分别保存对应粒度下交织后的低半部分和高半部分掩码。

## 约束说明

### 通用约束

- Reg矢量计算C API通用约束请参见[通用约束](../reg_compute/overview.md#通用约束)。
- 源操作数和目的操作数为矢量数据寄存器或掩码寄存器。

### 寄存器约束

- `src0`和`src1`可以为同一个矢量数据寄存器或掩码寄存器。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng -DCANN_ASC_USE_EXPERIMENTAL example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

本示例处理两组各512个`uint8_t`元素，单次Repeat分别读取两个源寄存器各256个元素。Kernel计算`one_repeat_size`和`repeat_time`后，使用`asc_vf_call`启动Vector Function，循环执行两次交织，Host按每次Repeat的范围校验结果。

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "c_api/experimental/reg_permute_sel_exp.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 512;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(uint8_t);

__simd_vf__ inline void intlv_vf(__ubuf__ uint8_t* output0, __ubuf__ uint8_t* output1,
                                 __ubuf__ uint8_t* input0, __ubuf__ uint8_t* input1,
                                 uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_uint8_t src0;
    vector_uint8_t src1;
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadalign(src0, input0 + i * one_repeat_size);
        asc_loadalign(src1, input1 + i * one_repeat_size);
        auto result = asc_intlv(src0, src1);
        asc_storealign(output0 + i * one_repeat_size, result.first, mask);
        asc_storealign(output1 + i * one_repeat_size, result.second, mask);
    }
}

__global__ __vector__ void asc_intlv_kernel(__gm__ uint8_t* output0, __gm__ uint8_t* output1,
                                            __gm__ uint8_t* input0, __gm__ uint8_t* input1)
{
    asc_init();
    __ubuf__ uint8_t output0_local[ELEMENT_COUNT];
    __ubuf__ uint8_t output1_local[ELEMENT_COUNT];
    __ubuf__ uint8_t input0_local[ELEMENT_COUNT];
    __ubuf__ uint8_t input1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input0_local, input0, BUFFER_BYTES);
    asc_copy_gm2ub_align(input1_local, input1, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    uint16_t one_repeat_size = asc_get_vf_len() / sizeof(uint8_t);
    uint16_t repeat_time = ELEMENT_COUNT / one_repeat_size;
    asc_vf_call<intlv_vf>(output0_local, output1_local, input0_local, input1_local,
                          one_repeat_size, repeat_time);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output0, output0_local, BUFFER_BYTES);
    asc_copy_ub2gm_align(output1, output1_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input0(ELEMENT_COUNT);
    std::vector<uint8_t> input1(ELEMENT_COUNT);
    std::vector<uint8_t> output0(ELEMENT_COUNT, 0);
    std::vector<uint8_t> output1(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input0[i] = static_cast<uint8_t>(i % 251);
        input1[i] = static_cast<uint8_t>(250 - i % 251);
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input0_device = nullptr;
    uint8_t* input1_device = nullptr;
    uint8_t* output0_device = nullptr;
    uint8_t* output1_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input0_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&input1_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output0_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output1_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input0_device, BUFFER_BYTES, input0.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(input1_device, BUFFER_BYTES, input1.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_intlv_kernel<<<1, 0>>>(output0_device, output1_device, input0_device, input1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output0.data(), BUFFER_BYTES, output0_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(output1.data(), BUFFER_BYTES, output1_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    const uint32_t one_repeat_size = 256 / sizeof(uint8_t);
    for (uint32_t repeat = 0; repeat < ELEMENT_COUNT / one_repeat_size; ++repeat) {
        const uint32_t base = repeat * one_repeat_size;
        for (uint32_t offset = 0; offset < one_repeat_size / 2; ++offset) {
            passed = passed && output0[base + 2 * offset] == input0[base + offset]
                     && output0[base + 2 * offset + 1] == input1[base + offset];
            passed = passed && output1[base + 2 * offset] == input0[base + one_repeat_size / 2 + offset]
                     && output1[base + 2 * offset + 1] == input1[base + one_repeat_size / 2 + offset];
        }
    }
    std::cout << (passed ? "[Success] asc_intlv completed." : "[Failed] asc_intlv output mismatch.")
              << std::endl;
    aclrtFree(input0_device);
    aclrtFree(input1_device);
    aclrtFree(output0_device);
    aclrtFree(output1_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
