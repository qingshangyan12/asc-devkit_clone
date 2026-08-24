# asc_set_l0c2gm_config

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

本接口用于设置L0C Buffer搬出场景下随路tensor量化、随路tensor激活、UnitFlag功能的相关配置，具体功能说明如下：

- 功能一：设置随路tensor量化参数的起始地址索引。当L0C Buffer搬出接口启用tensor量化模式时，硬件将从Fixpipe Buffer中按本接口设置的起始地址索引读取N维量化参数，N代表搬出矩阵的列数。
- 功能二：设置随路tensor激活参数的起始地址索引。当L0C Buffer搬出接口启用tensor激活模式时，硬件将从Fixpipe Buffer中按本接口设置的起始地址索引读取N维激活参数，N代表搬出矩阵的列数。
- 功能三：UnitFlag全局状态初始化：当需要关闭L0C Buffer搬出接口的UnitFlag功能，或矩阵乘加计算与搬出的数据量不一致时，本接口可以将UnitFlag功能的全局状态设置为关闭。

本接口设置的配置写入后对后续[asc_copy_l0c2gm](../cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)等所有L0C Buffer搬出接口生效，直至再次调用本接口修改配置。

## 函数原型

```c
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre,
                                             uint64_t quant_pre,
                                             bool enable_unit_flag)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| relu_pre | 输入 | tensor激活参数在Fixpipe Buffer中的起始地址索引，单位为64字节，取值范围为[0, 255]。该参数仅低8 bit有效。tensor激活参数的地址需64字节对齐，地址索引的计算方式为：Fixpipe Buffer物理地址/64。仅当后续L0C Buffer搬出接口选择tensor激活模式时，本参数生效。 |
| quant_pre | 输入 | tensor量化参数在Fixpipe Buffer中的起始地址索引，单位为128字节，取值范围为[0, 255]。该参数仅低8 bit有效。tensor量化参数的地址需128字节对齐，地址索引的计算方式为：Fixpipe Buffer物理地址/128。仅当后续L0C Buffer搬出接口选择tensor量化模式时，本参数生效。 |
| enable_unit_flag | 输入 | 是否清零L0C Buffer所有存储块的unitFlag。取值为true时清零，取值为false时保持当前状态。本参数是一次性的清零控制，不用于开启后续搬出接口的unitFlag功能。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- tensor激活参数需先搬运到Fixpipe Buffer。调用本接口后，需在后续搬出接口读取激活参数前完成PIPE_FIX流水同步。`relu_pre`超出取值范围时仅低8 bit生效，将导致硬件从错误的地址读取量化参数。
- tensor量化参数需先搬运到Fixpipe Buffer。调用本接口后，需在后续搬出接口读取量化参数前完成PIPE_FIX流水同步。`quant_pre`超出取值范围时仅低8 bit生效，将导致硬件从错误的地址读取量化参数。
- 本接口的配置会持续生效，直至再次调用本接口覆盖。不同搬出任务使用的配置不同时，需在对应搬出接口执行前重新配置；每次调用均会同时覆盖三个参数。
- `enable_unit_flag`用于消除L0C Buffer中残留unitFlag对后续任务的影响。是否使用unitFlag搬出数据，由后续搬出接口的unitFlag相关参数控制。

## 调用示例

```cpp
// Build/run (examples/02_simd_c_api/04_c_api_call_examples/cube_compute/asc_set_l0c2gm_config.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang examples/02_simd_c_api/04_c_api_call_examples/cube_compute/asc_set_l0c2gm_config.asc -lm -o /tmp/c_api_cube_compute_asc_set_l0c2gm_config && /tmp/c_api_cube_compute_asc_set_l0c2gm_config
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t DIM = 16, ELEMENTS = DIM * DIM;
constexpr uint16_t HALF_ONE = 0x3c00, HALF_THREE = 0x4200;
constexpr uint64_t SIGNED_SCALE_TWO = 0x40000000ULL | (1ULL << 46);

__global__ __cube__ void AscSetL0c2gmConfigKernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ uint64_t* scales, __gm__ int8_t* output)
{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS], b_l1[ELEMENTS];
    __cbuf__ uint64_t scale_l1[DIM];
    __fbuf__ uint64_t scale_fb[DIM];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];
    constexpr uint64_t nz_config = (16ULL << 32) | (1ULL << 16) | 1ULL;
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), DIM * sizeof(half), 0, DIM, DIM, 0, false);
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), DIM * sizeof(half), 0, DIM, DIM, 0, false);
    asc_copy_gm2l1(reinterpret_cast<__cbuf__ void*>(scale_l1), reinterpret_cast<__gm__ void*>(scales), 1, 4, 0, 0, 0);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, 0, true, false, true);
    asc_sync_notify(PIPE_MTE2, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_FIX, EVENT_ID0);
    asc_copy_l12fb(reinterpret_cast<__fbuf__ void*>(scale_fb), reinterpret_cast<__cbuf__ void*>(scale_l1), 1, 2, 0, 0);
    const uint64_t scale_address = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(scale_fb)) / 128;
    asc_set_l0c2gm_config(0, scale_address, false);
    asc_sync_pipe(PIPE_FIX);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::VQF322B8_PRE), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_ALL);
}

void PrintRow(const char* label, const std::vector<int8_t>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << +data[i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint16_t> a(ELEMENTS), b(ELEMENTS);
    std::vector<uint64_t> scales(DIM, SIGNED_SCALE_TWO);
    std::vector<int8_t> output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t i = 0; i < DIM; ++i) {
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_THREE;
        golden[i * DIM + i] = 6;
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    uint64_t* scales_device = nullptr;
    int8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&scales_device), scales.size() * sizeof(uint64_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(scales_device, scales.size() * sizeof(uint64_t), scales.data(), scales.size() * sizeof(uint64_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    AscSetL0c2gmConfigKernel<<<1, 0>>>(a_device, b_device, scales_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size(), output_device, output.size(), ACL_MEMCPY_DEVICE_TO_HOST);
    PrintRow("Quantized output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_l0c2gm_config applied per-channel scale 2."
                         : "[Failed] asc_set_l0c2gm_config result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(scales_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```


# asc_set_l0c2gm_config
## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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
待修改

本接口用于设置L0C Buffer搬出场景下随路tensor量化、随路tensor激活、UnitFlag功能的相关配置，具体功能说明如下：

- 功能一：设置随路tensor量化参数的起始地址索引。当L0C Buffer搬出接口启用tensor量化模式时，硬件将从Fixpipe Buffer中按本接口设置的起始地址索引读取N维量化参数，N代表搬出矩阵的列数。
- 功能二：设置随路tensor激活参数的起始地址索引。当L0C Buffer搬出接口启用tensor激活模式时，硬件将从Fixpipe Buffer中按本接口设置的起始地址索引读取N维激活参数，N代表搬出矩阵的列数。
- 功能三：UnitFlag全局状态初始化：当需要关闭L0C Buffer搬出接口的UnitFlag功能，或矩阵乘加计算与搬出的数据量不一致时，本接口可以将UnitFlag功能的全局状态设置为关闭。

本接口设置的配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口修改配置。

## 函数原型

```c
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre,
                                             uint64_t quant_pre,
                                             bool enable_unit_flag)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| relu_pre | 输入 | tensor激活参数在Fixpipe Buffer中的起始地址索引，单位64字节。当L0C Buffer搬出接口启用tensor激活模式时，硬件按`本参数×64字节+激活偏移+列索引×4×元素数`从Fixpipe Buffer读取各列ReLU参数。取值范围 [0, 7]。传入 > 7的值会按位宽截断处理（如8→0、9→1）。截断后实际生效值偏小，导致实际读取的向量激活参数表起始地址偏小，读到错误位置的激活参数，随路激活结果错误。 |
| quant_pre | 输入 | 向量随路量化参数表在Fixpipe Buffer中的起始地址索引，单位128字节。当L0C搬出接口的随路量化模式选为向量量化模式（如VDEQF16、VREQ8、VQF322B8_PRE等）时，硬件按 `本参数 × 128字节 + 列索引 × 8 × 元素数` 从Fixpipe Buffer读取各列量化参数。取值范围 [0, 15]。传入 > 15的值会按位宽截断处理（如16→0、17→1）。截断后实际生效值偏小，导致实际读取的向量量化参数表起始地址偏小，读到错误位置的量化参数，随路量化结果错误。 |
| enable_unit_flag | 输入 | L0C UnitFlag清除控制位。• false：不清除L0C UnitFlag，保留各存储块的UnitFlag当前状态；• true：清除L0C Buffer全部存储块的UnitFlag，用于矩阵乘加计算与L0C搬出数据量不一致或需要重新配置L0C Buffer搬出状态时复位UnitFlag。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

支持以下三种传参形式：
- 同时设置relu_pre和quant_pre。
- 仅传入relu_pre，quant_pre传入0。
- 仅传入quant_pre，relu_pre传入0。


### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- 本接口仅写入Fixpipe控制寄存器配置，实际随路量化与随路激活计算由后续调用的L0C搬出接口执行，须先调用本接口写入配置，再调用对应搬出接口。
- 配置写入后对后续所有L0C搬出接口生效，直至再次调用本接口覆盖配置；不同搬出任务间若需不同向量量化/激活参数表地址或不同UnitFlag状态，应在调用搬出接口前重新调用本接口设置。
- 本接口与配套setter接口协同配置L0C搬出接口的完整随路能力参数集，调用搬出接口前须完成全部相关配套setter的配置（配套setter名见功能说明章节）。
- 向量随路量化参数须先搬运至Fixpipe Buffer，再调用本接口设置参数表地址索引；向量随路激活参数同理须先搬运至Fixpipe Buffer，否则硬件按本接口设置的地址读到未初始化数据，随路量化/激活结果错误。
- M1/M2/M3/M4量化乘子不可为INF/NAN或非规格化数，否则随路量化结果错误。

### 字段截断约束

- `relu_pre` 传入 > 7的值会按位宽截断处理（如8→0、9→1）。截断后实际生效值偏小，导致实际读取的向量激活参数表起始地址偏小，读到错误位置的激活参数，随路激活结果错误。参数类型为 `uint64_t`（0~2^64−1）。
- `quant_pre` 传入 > 15的值会按位宽截断处理（如16→0、17→1）。截断后实际生效值偏小，导致实际读取的向量量化参数表起始地址偏小，读到错误位置的量化参数，随路量化结果错误。参数类型为 `uint64_t`（0~2^64−1）。

### UnitFlag清除约束

- `enable_unit_flag` 置为true会清除L0C Buffer全部存储块的UnitFlag，调用前须确认无其他在途L0C搬出指令依赖原有UnitFlag状态，否则可能丢失UnitFlag同步信息导致数据竞争。
- 当L0C搬出接口开启UnitFlag机制时，本接口设置的UnitFlag状态影响硬件读阻塞与置位行为；当L0C搬出接口关闭UnitFlag机制时，本接口的UnitFlag清除不产生可观察效果。


## 调用示例

```cpp
constexpr uint64_t relu_pre = 0;
constexpr uint64_t quant_pre = 0x1000;// 假设量化操作有效地址为0x1000
asc_set_l0c2gm_config(relu_pre, quant_pre, true);
```
