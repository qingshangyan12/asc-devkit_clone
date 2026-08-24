# asc_set_l3d_rpt_b

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

本接口用于配置右矩阵（B组）Feature Map的repeat参数。配置写入后，由[asc_copy_l12l0a](../cube_data_move/asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md)或[asc_copy_l12l0b](../cube_data_move/asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md)的3D格式搬运接口在执行Image to Column展开时使用。

仅当3D格式搬运接口的`f_matrix_ctrl`设置为true、选择右矩阵的Feature Map属性时，本接口的配置生效。`f_matrix_ctrl`设置为false时，需使用[asc_set_l13d_rpt](../cube_data_move/asc_set_l13d_rpt.md)配置左矩阵的repeat参数。

## 函数原型

```c
__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| config | 输入 | 右矩阵Feature Map的64 bit repeat配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。 |

**表2** config字段说明 <a id="table2"></a>

| bit范围 | 对应Basic API参数 | 描述 |
|---|---|---|
| 15:0 | repeatStride | 相邻两次迭代起始地址的偏移，取值范围为[0, 65535]。`repeatMode`为0时，单位为16个元素；`repeatMode`为1时，单位为`32 / sizeof(T)`个元素，T为源操作数的数据类型。 |
| 23:16 | repeatTime | 迭代次数，取值范围为[0, 255]，默认值为1。取值为0时，后续3D格式搬运接口被视为NOP（空操作）。 |
| 24 | repeatMode | 迭代方向。取值为0时，沿M（height）方向迭代；取值为1时，沿K（width）方向迭代。 |
| 31:25 | - | 保留位，需设置为0。 |
| 47:32 | dstStride | 输出矩阵K轴方向的偏移，取值范围为[0, 65535]，单位为512字节分形。Ascend 950PR/Ascend 950DT上必须根据目的矩阵排布配置本字段。 |
| 63:48 | dstMposition | 输出矩阵M轴方向的起始位置，取值范围为[0, 65535]，单位为分形。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- 本接口需在对应的3D格式搬运接口执行前调用，并将搬运接口的`f_matrix_ctrl`设置为true。配置会持续生效，直至再次调用本接口覆盖。
- 使用右矩阵Feature Map时，还需通过[asc_set_l13d_fmatrix_b](../cube_data_move/asc_set_l13d_fmatrix_b.md)配置Feature Map属性；需要padding时，通过[asc_set_l12l0_padding_val](./asc_set_l12l0_padding_val.md)配置填充值。
- `repeatTime`为0时不执行搬运。`repeatMode`只能为0或1，并按照对应的单位配置`repeatStride`。
- `dstStride`需与目的矩阵的K轴排布匹配，避免不同迭代写入的分形发生非预期重叠。
- `config[31:25]`为保留位，需设置为0。接口会将`config`整体写入，不会检查各字段的取值范围。

## 调用示例

```cpp
// Build/run (examples/02_simd_c_api/04_c_api_call_examples/sys_var/asc_set_l3d_rpt_b.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang examples/02_simd_c_api/04_c_api_call_examples/sys_var/asc_set_l3d_rpt_b.asc -lm -o /tmp/c_api_sys_var_asc_set_l3d_rpt_b && /tmp/c_api_sys_var_asc_set_l3d_rpt_b
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t M = 16;
constexpr uint32_t K = 16;
constexpr uint32_t N = 32;
constexpr uint32_t A_ELEMENTS = M * K;
constexpr uint32_t B_ELEMENTS = K * N;
constexpr uint32_t C_ELEMENTS = M * N;
constexpr uint16_t HALF_ONE = 0x3c00;
constexpr uint16_t HALF_NEG_ONE = 0xbc00;

__global__ __cube__ void AscSetL3dRptBKernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ float* output)
{
    asc_init();
    __cbuf__ half a_l1[A_ELEMENTS];
    __cbuf__ half b_l1[B_ELEMENTS];
    __ca__ half a_l0[A_ELEMENTS];
    __cb__ half b_l0[B_ELEMENTS];
    __cc__ float c_l0[C_ELEMENTS];
    constexpr uint64_t nz_config = (16ULL << 32) | (1ULL << 16) | 1ULL;

    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), K * sizeof(half), 0, M, K, 0, false);
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), N * sizeof(half), 0, K, N, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);

    // Repeat stride=1, count=2 in K, destination K stride=2 fragments.
    constexpr uint64_t repeat_config = 1ULL | (2ULL << 16) | (1ULL << 24) | (2ULL << 32);
    asc_set_l3d_rpt_b(repeat_config);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 2);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, 0, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M, 0, 0, 0,
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
    std::vector<uint16_t> a(A_ELEMENTS, 0);
    std::vector<uint16_t> b(B_ELEMENTS, 0);
    std::vector<float> input_b(B_ELEMENTS, 0.0f);
    std::vector<float> output(C_ELEMENTS, 0.0f);
    std::vector<float> golden(C_ELEMENTS, 0.0f);
    for (uint32_t i = 0; i < M; ++i) {
        a[i * K + i] = HALF_ONE;
        b[i * N + i] = HALF_ONE;
        b[i * N + 16 + i] = HALF_NEG_ONE;
        input_b[i * N + i] = 1.0f;
        input_b[i * N + 16 + i] = -1.0f;
        golden[i * N + i] = 1.0f;
        golden[i * N + 16 + i] = -1.0f;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscSetL3dRptBKernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintRow("Input B row 0", input_b, 0);
    PrintRow("Output row 0 first fragment", output, 0);
    PrintRow("Output row 0 second fragment", output, 16);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_l3d_rpt_b passed."
                         : "[Failed] asc_set_l3d_rpt_b failed.") << std::endl;
    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```



# asc_set_l3d_rpt_b

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

用于设置接口[asc_copy_l12l0a](../cube_data_move/asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md)、[asc_copy_l12l0b](../cube_data_move/asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md)的2D格式搬运的repeat参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| config | 输入 | 用于设置接口asc_copy_l12l0a、asc_copy_l12l0b的2D格式搬运repeat参数。比特位说明参考表2。 |

**表2** 常用重复控制寄存器比特位说明

|L3D_RPT_B比特位    |功能|
| :-------     | :---- |
| L3D_RPT_B[15:0]    | 表示重复步长    | 
| L3D_RPT_B[23:16]   | 表示在M或K方向的重复次数，默认值为1。      | 
| L3D_RPT_B[24]      | 表示重复模式 <br> - 1'b0: 在M方向重复。 <br> - 1'b1: 在K方向重复。     | 
| L3D_RPT_B[47:32]   | 表示输出矩阵在K方向上的步长，以分形为单位。     | 
| L3D_RPT_B[63:48]   | 表示输出矩阵在M方向上的起始位置，以分形为单位。      | 

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需要配合接口[asc_copy_l12l0a](../cube_data_move/asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md)、[asc_copy_l12l0b](../cube_data_move/asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md)使用。

## 调用示例

```cpp
uint64_t config = 0;
asc_set_l3d_rpt_b(config);
```
