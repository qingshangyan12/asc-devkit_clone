# asc_set_gm2l1_nz_para

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
<!-- npu="910" id4 -->
- Atlas 训练系列产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="310b" id7 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id7 -->

## 功能说明

本接口用于配置从Global Memory搬运至L1 Buffer过程中，ND2Nz或DN2Nz格式转换所需的目的地址布局，包括矩阵数量、目的Nz矩阵中相邻行的步长、相邻Z矩阵的步长以及相邻Nz矩阵的步长。配置完成后，由后续调用的[asc_copy_gm2l1_nd2nz](../cube_data_move/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md)或[asc_copy_gm2l1_dn2nz](../cube_data_move/asc_copy_gm2l1_dn2nz.md)接口使用。

本接口只完成参数配置，不执行数据搬运。源地址步长、矩阵行数和矩阵列数等搬运参数由后续搬运接口配置。

SmallC0模式由后续搬运接口的`smallc0_en`参数控制，具体说明请参见[asc_copy_gm2l1_nd2nz的smallc0_en参数](../cube_data_move/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md#参数说明)和[asc_copy_gm2l1_dn2nz的smallc0_en参数](../cube_data_move/asc_copy_gm2l1_dn2nz.md#参数说明)。

本接口为矩阵搬入相关配置接口，仅在AIC上生效。

## 函数原型

```c
__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| ---------- | ---------- | ---------- |
| config | 输入 | ND2Nz或DN2Nz搬运的64位配置参数。接口不检查或转换字段值，需按照[表 2](#table2)完成拼装。 |

**表 2**  config参数字段说明<a id="table2"></a>

| 比特位范围 | 字段名称 | 描述 |
| ---------- | ---------- | ---------- |
| 15:0 | `matrix_num` | 源操作数中ND矩阵或DN矩阵的数量，取值范围为[0, 4095]。取值为0时，后续搬运接口将被视为NOP（空操作）。 |
| 31:16 | `dst_nz_n_stride` | 目的Nz矩阵中相邻行起始地址间的偏移。未开启SmallC0模式时，取值范围为[1, 16384]，单位为32B。 |
| 47:32 | `dst_nz_c0_stride` | 目的Nz矩阵中相邻Z矩阵起始地址间的偏移。未开启SmallC0模式时，取值范围为[1, 16384]，单位为32B。 |
| 63:48 | `dst_nz_matrix_stride` | 目的操作数中相邻Nz矩阵起始地址间的偏移，取值范围为[0, 65535]，单位为32B。矩阵数量为1时，本字段无意义，设置为0即可。矩阵数量大于1且本字段为0时，后写入的Nz矩阵会覆盖第一个Nz矩阵。 |

三个步长字段均以32B为单位，字段值表示相应地址偏移包含的32B块数。例如，字段值为2时，表示地址偏移为64B。待配置的字节偏移必须为32B的整数倍，字段值可按`字节偏移/32`计算。

`config`可按如下方式拼装：

```cpp
uint64_t config = (static_cast<uint64_t>(dst_nz_matrix_stride) << 48) |
                  (static_cast<uint64_t>(dst_nz_c0_stride) << 32) |
                  (static_cast<uint64_t>(dst_nz_n_stride) << 16) |
                  static_cast<uint64_t>(matrix_num);
```

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用将直接返回。
- 本接口需在对应的ND2Nz或DN2Nz搬运接口执行前调用，调用后配置会持续生效，直至再次调用本接口覆盖。
- `config`由4个16位字段组成，接口会将`config`整体写入，不会检查字段范围。拼装前需确保各字段满足[表 2](#table2)中的取值要求，不得将超出16位的值直接左移，否则高位会写入相邻字段。
- 后续搬运接口开启SmallC0模式时，`config[31:16]`和`config[47:32]`不生效，设置为0即可。
- 搬运至L1 Buffer的数据不能重叠。如果步长配置导致重叠写入，硬件不会产生告警或错误，并且不保证重叠数据的写入顺序。

<!-- npu="950" id8 -->
## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

以下调用示例代码仅Ascend 950PR/Ascend 950DT产品支持。

示例中每次搬运一个16×16的`half`类型ND矩阵。目的Nz矩阵的相邻行起始地址偏移为1个32B块，相邻Z矩阵起始地址偏移为16个32B块；由于每次只搬运一个矩阵，相邻Nz矩阵起始地址偏移配置为0。

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

__global__ __cube__ void asc_set_gm2l1_nz_para_kernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ float* output)
{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS];
    __cbuf__ half b_l1[ELEMENTS];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];

    // 三个目的地址步长字段的单位均为32B。
    constexpr uint64_t matrix_num = 1;
    constexpr uint64_t dst_nz_n_stride = 1;
    constexpr uint64_t dst_nz_c0_stride = 16;
    constexpr uint64_t dst_nz_matrix_stride = 0;
    constexpr uint64_t nz_config = (dst_nz_matrix_stride << 48) | (dst_nz_c0_stride << 32) |
                                   (dst_nz_n_stride << 16) | matrix_num;
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
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_ALL);
}

void print_row(const char* label, const std::vector<float>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<uint16_t> a(ELEMENTS, 0);
    std::vector<uint16_t> b(ELEMENTS, 0);
    std::vector<float> input_row(DIM, 0.0f);
    std::vector<float> output(ELEMENTS, 0.0f);
    std::vector<float> golden(ELEMENTS, 0.0f);
    for (uint32_t i = 0; i < DIM; ++i) {
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_ONE;
        golden[i * DIM + i] = 1.0f;
    }
    input_row[0] = 1.0f;

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_set_gm2l1_nz_para_kernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);

    print_row("Input A row 0", input_row);
    print_row("Output row 0", output);
    print_row("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_gm2l1_nz_para passed."
                         : "[Failed] asc_set_gm2l1_nz_para failed.") << std::endl;
    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
<!-- end id8 -->
