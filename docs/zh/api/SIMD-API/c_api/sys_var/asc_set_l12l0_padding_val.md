# asc_set_l12l0_padding_val

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

本接口用于配置右矩阵（B组）Feature Map的Pad填充值。配置写入后，由[asc_copy_l12l0a](../cube_data_move/asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md)或[asc_copy_l12l0b](../cube_data_move/asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md)的3D格式搬运接口在执行Image to Column展开时使用，对Feature Map边界外的数据进行定值填充。

仅当3D格式搬运接口的`f_matrix_ctrl`设置为true、选择右矩阵的Feature Map属性时，本接口的配置生效。`f_matrix_ctrl`设置为false时，本接口的配置不生效。

## 函数原型

```c
__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| config | 输入 | 右矩阵Feature Map的64 bit Pad填充值配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。 |

**表2** config参数说明 <a id="table2"></a>

| 搬运数据位宽 | `config[31:0]`的配置方法 |
| :--- | :--- |
| 8 bit | `config[7:0]`为Pad填充值的原始位模式，`config[15:8]`必须与`config[7:0]`相同，`config[31:16]`设置为0。 |
| 16 bit | `config[15:0]`为Pad填充值的原始位模式，`config[31:16]`设置为0。 |
| 32 bit | `config[31:0]`为Pad填充值的原始位模式。 |

`config[63:32]`为保留位，必须设置为0。

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- 本接口需在对应的3D格式搬运接口执行前调用，并将搬运接口的`f_matrix_ctrl`设置为true。配置会持续生效，直至再次调用本接口覆盖。
- 使用右矩阵Feature Map时，还需通过[asc_set_l13d_fmatrix_b](../cube_data_move/asc_set_l13d_fmatrix_b.md)配置Feature Map属性，并通过[asc_set_l3d_rpt_b](./asc_set_l3d_rpt_b.md)配置repeat参数。
- 配置8 bit Pad填充值时，必须将同一个8 bit原始位模式分别写入`config[7:0]`和`config[15:8]`，否则搬运结果未定义。本接口不会自动复制填充值。
- 本接口直接写入完整配置，不检查字段取值。除[表2](#table2)中使用的字段外，其余位必须设置为0。

## 调用示例

```cpp
// Build/run (examples/02_simd_c_api/04_c_api_call_examples/sys_var/asc_set_l12l0_padding_val.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang examples/02_simd_c_api/04_c_api_call_examples/sys_var/asc_set_l12l0_padding_val.asc -lm -o /tmp/c_api_sys_var_asc_set_l12l0_padding_val && /tmp/c_api_sys_var_asc_set_l12l0_padding_val
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t DIM = 16;
constexpr uint32_t ELEMENTS = DIM * DIM;
constexpr uint16_t HALF_ONE = 0x3c00;

__global__ __cube__ void AscSetL12l0PaddingValKernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ float* output)
{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS];
    __cbuf__ half b_l1[ELEMENTS];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];
    constexpr uint64_t nz_config = (16ULL << 32) | (1ULL << 16) | 1ULL;

    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), DIM * sizeof(half), 0, DIM, DIM, 0, false);
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), DIM * sizeof(half), 0, DIM - 1, DIM, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);

    asc_l13d_fmatrix_config feature_map;
    feature_map.l1_width = 1;
    feature_map.l1_height = DIM - 1;
    feature_map.padding_left_size = 0;
    feature_map.padding_right_size = 0;
    feature_map.padding_top_size = 1;
    feature_map.padding_bottom_size = 0;
    asc_set_l13d_fmatrix_b(feature_map);
    asc_load3d_v2_config repeat;
    asc_set_l13d_rpt(repeat);
    asc_set_l12l0_padding_val(HALF_ONE);
    asc_copy_l12l0b(b_l0, b_l1, DIM, DIM, 0, 0, 1, 1, 1, 1, 1, 1,
        false, false, true, true, DIM);

    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, 0, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_ALL);
}

void PrintRow(const char* label, const std::vector<float>& data, uint32_t offset)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[offset + i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint16_t> a(ELEMENTS, 0);
    std::vector<uint16_t> b(ELEMENTS, 0);
    std::vector<float> output(ELEMENTS, 0.0f);
    std::vector<float> golden(ELEMENTS, 0.0f);
    for (uint32_t i = 0; i < DIM; ++i) a[i * DIM + i] = HALF_ONE;
    for (uint32_t i = 0; i < DIM; ++i) golden[i] = 1.0f;

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscSetL12l0PaddingValKernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintRow("Input B row 0", std::vector<float>(ELEMENTS, 0.0f), 0);
    PrintRow("Output row 0", output, 0);
    PrintRow("Output row 1", output, DIM);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_l12l0_padding_val passed."
                         : "[Failed] asc_set_l12l0_padding_val failed.") << std::endl;
    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```



# asc_set_l12l0_padding_val

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

对PADDING_B寄存器中的值进行设置，这是一个64bit的寄存器，用于存储搬运过程中padding的值。
其中各bit含义如下：

**表1** PADDING_B寄存器比特位说明

| bit范围 |                                                                           含义                                                                           | 
| :-----------|:------------------------------------------------------------------------------------------------------------------------------------------------------:|
| 31:0 | 存储进行padding时的值。<br>数据位宽为32时，直接使用31:0位；<br> 数据位宽为16时，使用15:0位，31:16位被忽略； <br> 数据位宽为8时，15:8位应与7:0位相同，31:16位被忽略；数据位宽为4时，15:12，11:8，7:4和3:0位应该相同，31:16位被忽略。 |
| 33:32 | 表示所使用的padding模式，当前支持三种模式。<br>2'b00：固定padding；<br>2'b01：按通道padding；<br>2'b10/2'b11：当前保留。 |

## 函数原型

```cpp
__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config)
```

## 参数说明

**表2** 参数说明

|参数名|输入/输出| 描述        |
| :------ | :---  |:----------|
|config   |输入   | 待设置的寄存器值。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例
```cpp
uint64_t config = 0;
asc_set_l12l0_padding_val(config);
```
