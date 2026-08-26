# asc_set_l0c_copy_prequant

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

本接口用于在L0C Buffer搬出过程中进行随路Scalar量化时，配置量化计算所需的缩放系数、偏移量和量化结果符号。配置完成后，由后续调用的[asc_copy_l0c2gm](../cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)、[asc_copy_l0c2l1](../cube_data_move/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md)或[asc_copy_l0c2ub](../cube_data_move/asc_copy_l0c2ub.md)接口使用。

本接口为矩阵搬出相关配置接口，仅在AIC上生效。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config);

__aicore__ inline void asc_set_l0c_copy_prequant(float scale, uint16_t offset, bool is_signed);
```

第二个重载仅Ascend 950PR/Ascend 950DT产品支持。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| config | 输入 | Scalar量化参数，取值范围为[0, 2^64-1]。各比特位的含义与NPU架构和量化模式相关，参见表2和表3。 |
| scale | 输入 | 量化计算所需的缩放系数。接口将float数据的第13~31位写入config的第13~31位，其中第31位为符号位，第23~30位为指数位，第13~22位为尾数位，硬件按(1, 8, 10)格式进行计算。取值不能为inf或nan。仅Ascend 950PR/Ascend 950DT产品支持。 |
| offset | 输入 | 量化计算所需的偏移量，取值范围为[0, 511]。接口将offset的低9位写入config的第37~45位。源数据乘以scale后的结果与offset相加。不使用偏移量时，配置为0。仅在REQ8和QF322B8_PRE模式下生效。仅Ascend 950PR/Ascend 950DT产品支持。 |
| is_signed | 输入 | 量化结果是否为有符号数据。取值为`true`时，量化结果为int8_t；取值为`false`时，量化结果为uint8_t。接口将该参数写入config的第46位。仅在REQ8和QF322B8_PRE模式下生效。仅Ascend 950PR/Ascend 950DT产品支持。 |

各量化模式的功能说明请参考[随路量化](../../basic_api/cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md)。

**表2** config比特位含义（NPU架构版本2201）

| 量化模式 | 比特位数 | 变量名 | 描述 |
|---|---|---|---|
| DEQF16、REQ8、QF322B8_PRE | 0~31 | M1 | 32位数据视为float，硬件以(1, 8, 10)格式进行计算。其中第31位为符号位，第23~30位为指数位，第13~22位为尾数位。 |
| DEQF16、REQ8、QF322B8_PRE | 32~35 | N | 4位数据表示范围[1, 16]，`0b0000`表示1，`0b1111`表示16。REQ8模式下，MCB标志位置为1时，输入值右移N位；QF322B8_PRE模式下该字段无效。 |
| DEQF16、REQ8、QF322B8_PRE | 36 | MCB标志位 | 取值为0时，输入的int32_t数据直接转换为float，N字段无效；取值为1时，输入的int32_t数据先右移N位并转换为int16_t，再转换为float。QF322B8_PRE模式下该字段无效。 |
| DEQF16、REQ8、QF322B8_PRE | 37~45 | Offset | 9位整型数据。源数据乘以量化系数后的结果与Offset相加。不使用偏移量时，配置为0。DEQF16模式下该字段无效。 |
| DEQF16、REQ8、QF322B8_PRE | 46 | Sign标志位 | 取值为1时，量化结果为int8_t；取值为0时，量化结果为uint8_t。仅在REQ8和QF322B8_PRE模式下生效。 |
| DEQF16、REQ8、QF322B8_PRE | 47~63 | - | 无效比特位。 |

**表3** config比特位含义（NPU架构版本3510）

| 量化模式 | 比特位数 | 变量名 | 描述 |
|---|---|---|---|
| DEQF16、REQ8、QF322B8_PRE、QF322FP8_PRE、QF322HIF8_PRE、QF322HIF8_PRE_HYBRID、QS322BF16_PRE、QF322F16_PRE、QF322BF16_PRE、QF322F32_PRE | 0~12 | - | 无效比特位。 |
| 同第一行 | 13~31 | M1 | 数据类型视为float，硬件以(1, 8, 10)格式进行计算。其中第31位为符号位，第23~30位为指数位，第13~22位为尾数位。取值不能为inf或nan。 |
| 同第一行 | 32~36 | - | 无效比特位。 |
| 同第一行 | 37~45 | Offset | 9位整型数据。源数据乘以量化系数后的结果与Offset相加。不使用偏移量时，配置为0。仅在REQ8和QF322B8_PRE模式下生效。 |
| 同第一行 | 46 | Sign标志位 | 取值为1时，量化结果为int8_t；取值为0时，量化结果为uint8_t。仅在REQ8和QF322B8_PRE模式下生效。 |
| 同第一行 | 47~63 | - | 无效比特位。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口非AIC调用直接返回。
- 本接口需在对应的L0C Buffer搬出接口执行前调用，并在搬出接口中配置对应的Scalar量化模式。
- 三参数重载中，scale的第0~12位不写入量化参数，offset仅保留低9位。

<!-- npu="950" id8 -->
## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

以下调用示例代码仅Ascend 950PR/Ascend 950DT产品支持。

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t DIM = 16;
constexpr uint32_t ELEMENTS = DIM * DIM;
constexpr uint16_t HALF_ONE = 0x3c00;
constexpr float SCALE = 2.0f;
constexpr uint16_t OFFSET = 3;

__global__ __cube__ void asc_set_l0c_copy_prequant_kernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ int8_t* output)
{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS], b_l1[ELEMENTS];
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

    asc_set_l0c_copy_prequant(SCALE, OFFSET, true);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::QF322B8_PRE), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_ALL);
}

void print_row(const char* label, const std::vector<int8_t>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << static_cast<int32_t>(data[i]);
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint16_t> a(ELEMENTS), b(ELEMENTS);
    std::vector<int8_t> output(ELEMENTS), golden(ELEMENTS, static_cast<int8_t>(OFFSET));
    for (uint32_t i = 0; i < DIM; ++i) {
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_ONE;
        golden[i * DIM + i] = static_cast<int8_t>(SCALE + OFFSET);
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    int8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(int8_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_set_l0c_copy_prequant_kernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int8_t), output_device, output.size() * sizeof(int8_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_row("Quantized output row 0", output);
    print_row("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_l0c_copy_prequant scalar quantization passed."
                         : "[Failed] asc_set_l0c_copy_prequant result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
