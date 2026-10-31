# asc_set_gm2l1_nz_para

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T13:21:36.320Z pushedAt=2026-10-28T12:45:45.467Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

This API configures the destination address layout required for ND2Nz or DN2Nz format conversion during data transfer from Global Memory to the L1 Buffer, including the number of cubes, the stride between adjacent rows in the destination Nz cube, the stride between adjacent Z cubes, and the stride between adjacent Nz cubes. After configuration, the layout is used by the subsequently called [asc_copy_gm2l1_nd2nz](./asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md) or [asc_copy_gm2l1_dn2nz](./asc_copy_gm2l1_dn2nz.md) API.

This API only completes parameter configuration and does not perform data transfer. Transfer parameters such as the source address stride, the number of cube rows, and the number of cube columns are configured by the subsequent transfer API.

This API is a cube load-in related configuration API and takes effect only on the AIC.

## Prototype

```c
__aicore__ inline void asc_set_gm2l1_nz_para(uint16_t matrix_num, uint16_t dst_nz_n_stride, uint16_t dst_nz_c0_stride, uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| matrix_num | Input | Number of ND or DN cubes in the source operand. Value range: [0, 4095]. When set to 0, the subsequent transfer API is treated as a NOP (no operation). |
| dst_nz_n_stride | Input | Offset between the start addresses of adjacent rows in the destination Nz cube. Value range: [1, 16384], in units of 32 bytes. |
| dst_nz_c0_stride | Input | Offset between the start addresses of adjacent Z fractals in the destination Nz cube. Value range: [1, 16384], in units of 32 bytes. |
| dst_nz_matrix_stride | Input | Offset between the start addresses of adjacent Nz cubes in the destination operand. Value range: [0, 65535], in units of 32 bytes.<br>&nbsp;&nbsp;&bull; When the number of cubes is 1, that is, when `matrix_num` is set to 1, this parameter is meaningless and can be set to 0.<br>&nbsp;&nbsp;&bull; When `matrix_num` is set to a value greater than 1 and `dst_nz_matrix_stride = 0`, the subsequently written Nz cube overwrites the first Nz cube. |
| config | Input | 64-bit configuration parameter for ND2Nz or DN2Nz transfer. It is assembled from `matrix_num`, `dst_nz_n_stride`, `dst_nz_c0_stride`, and `dst_nz_matrix_stride`. For the meaning of each bit range, see [Table 2](#table2). |

**Table 2** config parameter field description <a id="table2"></a>

| Bit Range | Field Name | Description |
| ---------- | ---------- | ---------- |
| 15:0 | matrix_num | Number of ND or DN cubes in the source operand. Value range: [0, 4095]. When set to 0, the subsequent transfer API is treated as a NOP (no operation). |
| 31:16 | dst_nz_n_stride | Offset between the start addresses of adjacent rows in the destination Nz cube. Value range: [1, 16384], in units of 32 bytes. |
| 47:32 | dst_nz_c0_stride | Offset between the start addresses of adjacent Z fractals in the destination Nz cube. Value range: [1, 16384], in units of 32 bytes. |
| 63:48 | dst_nz_matrix_stride | Offset between the start addresses of adjacent Nz cubes in the destination operand. Value range: [0, 65535], in units of 32 bytes.<br>&nbsp;&nbsp;&bull; When the number of cubes is 1, that is, when `matrix_num` is set to 1, this parameter is meaningless and can be set to 0.<br>&nbsp;&nbsp;&bull; When `matrix_num` is set to a value greater than 1 and `dst_nz_matrix_stride = 0`, the subsequently written Nz cube overwrites the first Nz cube. |

All three stride fields are in units of 32 bytes. The field value indicates the number of 32-byte blocks contained in the corresponding address offset. For example, a field value of 2 indicates an address offset of 64 bytes.

`config` can be assembled as follows:

```cpp
uint64_t config = (static_cast<uint64_t>(dst_nz_matrix_stride) << 48) |
                  (static_cast<uint64_t>(dst_nz_c0_stride) << 32) |
                  (static_cast<uint64_t>(dst_nz_n_stride) << 16) |
                  static_cast<uint64_t>(matrix_num);
```

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- This API returns directly when it is not called by the AIC.
- This API must be called before the corresponding ND2Nz or DN2Nz copy API is executed.
- Data copied to the L1 Buffer must not overlap. If the stride configuration causes overlapping writes, the hardware does not generate an alarm or error, and the write order of the overlapping data is not guaranteed.
- All four fields of `config` are 16-bit. The API writes `config` as a whole and does not check the field ranges individually. Before assembling, ensure that each field meets the value requirements in [Table 2](#table2). Do not directly left-shift a value that exceeds 16 bits, otherwise the high bits will be written into the adjacent field.

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

The following example code is supported only by Ascend 950PR&950DT products.

In the example, one 16×16 `half` ND cube is copied at a time. The adjacent row start address offset of the destination Nz cube is one 32B block, and the adjacent Z cube start address offset is 16 32B blocks. Since only one cube is copied at a time, the adjacent Nz cube start address offset is configured to 0.


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

    // The unit of the three destination address stride fields is 32B.
    constexpr uint16_t matrix_num = 1;
    constexpr uint16_t dst_nz_n_stride = 1;
    constexpr uint16_t dst_nz_c0_stride = 16;
    constexpr uint16_t dst_nz_matrix_stride = 0;
    asc_set_gm2l1_nz_para(matrix_num, dst_nz_n_stride, dst_nz_c0_stride, dst_nz_matrix_stride);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), DIM * sizeof(half), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, DIM, DIM, 0, false);
    asc_set_gm2l1_nz_para(matrix_num, dst_nz_n_stride, dst_nz_c0_stride, dst_nz_matrix_stride);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), DIM * sizeof(half), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, DIM, DIM, 0, false);

    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, asc_unit_flag_mode::DISABLE, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
        asc_unit_flag_mode::DISABLE, QuantMode_t::NoQuant, asc_relu_pre_mode::NONE, false, true, false, false);
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
