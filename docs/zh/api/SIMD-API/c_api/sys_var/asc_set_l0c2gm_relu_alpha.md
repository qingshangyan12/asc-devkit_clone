# asc_set_l0c2gm_relu_alpha

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

本接口用于配置L0C Buffer搬出过程中，预处理阶段Scalar ReLU使用的alpha系数。配置写入后，由后续的[asc_copy_l0c2gm](../cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)等L0C Buffer搬出接口使用。

当搬出接口的`relu_pre_mode`设置为2（Scalar ReLU）时，本接口配置的alpha系数生效：输入大于等于0时，输出等于输入；输入小于0时，输出等于`alpha * 输入`。

## 函数原型

```c
__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| config | 输入 | Scalar ReLU的64 bit alpha系数配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。 |

**表2** config字段说明 <a id="table2"></a>

| bit范围 | 字段 | 描述 |
|---|---|---|
| 12:0 | - | 保留位，需设置为0。 |
| 31:13 | M2 | 预处理阶段Scalar ReLU的alpha系数。该字段为19 bit的(1, 8, 10)浮点格式，即1 bit符号位、8 bit指数位和10 bit尾数位。 |
| 44:32 | - | 保留位，需设置为0。 |
| 63:45 | M4 | 后处理阶段Scalar ReLU的alpha系数，格式与M2相同。当前L0C Buffer搬出C API的`relu_post`为无效参数，本字段需设置为0。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- 本接口需在对应的L0C Buffer搬出接口执行前调用，并将搬出接口的`relu_pre_mode`设置为2。其他ReLU模式不使用本接口配置的M2字段。
- M2系数不能为INF、NAN或非规格化数。系数为0时，指数位和尾数位均需为0。
- 由标准IEEE 754 FP32数值生成M2字段时，保留符号位、8 bit指数位和高10 bit尾数位，舍弃低13 bit尾数位。
- `config[12:0]`、`config[44:32]`以及当前不支持的M4字段需设置为0。接口会将`config`整体写入，不会检查字段值。
- 配置会持续生效，直至再次调用本接口覆盖。不同搬出任务使用的alpha系数不同时，需在对应搬出接口执行前重新配置。

## 调用示例

```cpp
// Build/run (examples/02_simd_c_api/04_c_api_call_examples/sys_var/asc_set_l0c2gm_relu_alpha.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang examples/02_simd_c_api/04_c_api_call_examples/sys_var/asc_set_l0c2gm_relu_alpha.asc -lm -o /tmp/c_api_sys_var_asc_set_l0c2gm_relu_alpha && /tmp/c_api_sys_var_asc_set_l0c2gm_relu_alpha
// Architecture: dav-3510

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t DIM = 16;
constexpr uint32_t ELEMENTS = DIM * DIM;
constexpr uint16_t HALF_ONE = 0x3c00;
constexpr uint16_t HALF_NEG_ONE = 0xbc00;
constexpr uint16_t HALF_NEG_HALF = 0xb800;

__global__ __cube__ void AscSetL0c2gmReluAlphaKernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ uint16_t* output)
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
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), DIM * sizeof(half), 0, DIM, DIM, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, 0, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);

    // Scalar ReLU uses the 19-bit mini-float in RELU_ALPHA[31:13].
    constexpr uint64_t alpha_half = static_cast<uint64_t>(126U << 10) << 13;
    asc_set_l0c2gm_relu_alpha(alpha_half);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(reinterpret_cast<__gm__ half*>(output), c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::F322F16), 2, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_ALL);
}

void PrintRow(const char* label, const std::vector<uint16_t>& data)
{
    std::cout << label << ':' << std::hex;
    for (uint32_t i = 0; i < 8; ++i) std::cout << " 0x" << data[i];
    std::cout << std::dec << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint16_t> a(ELEMENTS, 0);
    std::vector<uint16_t> b(ELEMENTS, 0);
    std::vector<uint16_t> output(ELEMENTS, 0);
    std::vector<uint16_t> golden(ELEMENTS, 0);
    for (uint32_t i = 0; i < DIM; ++i) {
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_NEG_ONE;
        golden[i * DIM + i] = HALF_NEG_HALF;
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscSetL0c2gmReluAlphaKernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint16_t), output_device, output.size() * sizeof(uint16_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintRow("Input B row 0 (FP16 bits)", b);
    PrintRow("Output row 0 (FP16 bits)", output);
    PrintRow("Golden row 0 (FP16 bits)", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_l0c2gm_relu_alpha passed."
                         : "[Failed] asc_set_l0c2gm_relu_alpha failed.") << std::endl;
    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
