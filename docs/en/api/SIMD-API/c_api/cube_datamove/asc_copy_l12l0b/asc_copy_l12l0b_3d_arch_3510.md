# asc_copy_l12l0b_arch_3510 (3D Matrix Copy)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T12:47:12.049Z pushedAt=2026-10-29T05:01:35.398Z nl=2 -->

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

3D convolution data copy is used to perform Image to Column expansion on a Feature Map in NC1HWC0 format, generating an img2col two-dimensional cube based on the convolution stride, convolution kernel size, and expansion coefficient. It then selects a specified data block from the expanded cube and copies it to the L0B Buffer for cube computation data loading in 3D convolution scenarios.

The copy is performed in units of 512-byte data fractals. This API can also be used to copy 2D formatted data required for ordinary cube computation. For details on how to use this API to copy 2D formatted data, see [Key Feature Description](#l12l0b_3d_key_features). Data copy from the L1 Buffer to the L0B Buffer is supported.

In 3D img2col copy mode, the attribute description of the Feature Map (fm_w, fm_h, and padding on all sides) must be preconfigured through [asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md), and the fill value and fill mode must be preconfigured through [asc_set_l12l0b_3d_padding](../asc_set_l12l0b_3d_padding.md). If repeat mode is required, the repeat direction, count, and stride must be preconfigured through [asc_set_l13d_rpt_b](../asc_set_l13d_rpt_b.md).

This API takes effect only when executed on the AIC.

## Prototype

```cpp
__aicore__ inline void asc_copy_l12l0b(__cb__ <dtype>* dst,
                                       __cbuf__ <dtype>* src,
                                       uint16_t k_extension,
                                       uint16_t m_extension,
                                       uint16_t k_start_pt,
                                       uint16_t m_start_pt,
                                       uint8_t stride_w,
                                       uint8_t stride_h,
                                       uint8_t filter_w,
                                       uint8_t filter_h,
                                       uint8_t dilation_filter_w,
                                       uint8_t dilation_filter_h,
                                       bool filter_size_w,
                                       bool filter_size_h,
                                       bool transpose,
                                       bool f_matrix_ctrl,
                                       uint16_t channel_size)
```

### Data Types Supported by dtype

The data types supported by dtype are `int8_t`, `uint8_t`, `hifloat8_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Prototype Example

```cpp
// Example: 3D copy of the half type.
__aicore__ inline void asc_copy_l12l0b(__cb__ half* dst,
                                       __cbuf__ half* src,
                                       uint16_t k_extension,
                                       uint16_t m_extension,
                                       uint16_t k_start_pt,
                                       uint16_t m_start_pt,
                                       uint8_t stride_w,
                                       uint8_t stride_h,
                                       uint8_t filter_w,
                                       uint8_t filter_h,
                                       uint8_t dilation_filter_w,
                                       uint8_t dilation_filter_h,
                                       bool filter_size_w,
                                       bool filter_size_h,
                                       bool transpose,
                                       bool f_matrix_ctrl,
                                       uint16_t channel_size)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|---|---|---|
| `dst` | Output | Destination operand, stored in the L0B Buffer. The destination address must be 512-byte aligned. |
| `src` | Input | Source operand, stored in the L1 Buffer. The source address must be 32-byte aligned. |
| `k_extension` | Input | Transfer length of the destination operand in the width dimension. If the rightmost fractal is not covered, it must be a multiple of 8 for the b32 type, a multiple of 16 for the b16 type, and a multiple of 32 for the b8 type. If the fractal is covered, no multiple requirement applies to any data type. Value range: [1, 65535]. |
| `m_extension` | Input | Transfer length of the destination operand in the height dimension. If the bottommost fractal is not covered, it must be a multiple of 16 for the b8 and b16 types, with no requirement for the b32 type. If the fractal is covered, no multiple requirement applies to any data type. Value range: [0, 65535]. |
| `k_start_pt` | Input | Start point of the destination operand in the width dimension. It must be a multiple of 8 for the b32 type, a multiple of 16 for the b16 type, and a multiple of 32 for the b8 type. Value range: [0, 65535]. |
| `m_start_pt` | Input | Start point of the destination operand in the height dimension. Value range: [0, 32767]. |
| `stride_w` | Input | Stride of the convolution kernel sliding in the width dimension of the source operand. Value range: [1, 63]. |
| `stride_h` | Input | Stride of the convolution kernel sliding in the height dimension of the source operand. Value range: [1, 63]. |
| `filter_w` | Input | Convolution kernel width. Value range: [1, 255]. |
| `filter_h` | Input | Convolution kernel height. Value range: [1, 255]. |
| `dilation_filter_w` | Input | Expansion coefficient of the convolution kernel width. Value range: [1, 255]. |
| `dilation_filter_h` | Input | Expansion coefficient of the convolution kernel height. Value range: [1, 255]. |
| `filter_size_w` | Input | Whether to increase the convolution kernel width by 256 elements on top of `filter_w`.<br>&nbsp;&nbsp;&bull; `true`: Increase.<br>&nbsp;&nbsp;&bull; `false`: Do not increase. |
| `filter_size_h` | Input | Whether to increase the convolution kernel height by 256 elements on top of `filter_h`.<br>&nbsp;&nbsp;&bull; `true`: Increase.<br>&nbsp;&nbsp;&bull; `false`: Do not increase. |
| `transpose` | Input | Whether to enable the transpose function. This API automatically transposes the fractal cube written to the L0B Buffer. This parameter is ignored by the hardware, and passing true or false does not affect the transpose behavior. |
| `f_matrix_ctrl` | Input | Register group selection bit for the attribute description of the Feature Map in 3D data copy.<br>&nbsp;&nbsp;&bull; `true`: Obtain the attribute description of the Feature Map from the right cube, used together with [asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md), [asc_set_l12l0b_3d_padding](../asc_set_l12l0b_3d_padding.md), and [asc_set_l13d_rpt_b](../asc_set_l13d_rpt_b.md).<br>&nbsp;&nbsp;&bull; `false`: Obtain the attribute description of the Feature Map from the left cube, used together with [asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md), [asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md), and [asc_set_l13d_rpt](../asc_set_l13d_rpt.md). |
| `channel_size` | Input | Number of channels N of the source operand.<br>For the b32 type, the remainder of `channel_size` divided by 8 must be 0 or 4, and the maximum value is 65532.<br>For the b16 type, the remainder of `channel_size` divided by 16 must be 0, 4, or 8, and the maximum value is 65528.<br>For the b8 type, the remainder of `channel_size` divided by 32 must be 0, 4, 8, or 16, and the maximum value is 65520. |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

### General Constraints

- This API returns directly when it is not called by the AIC.
- The dst start address must be 512-byte aligned (L0B Buffer alignment requirement); otherwise, the copy will be abnormal.
- The src start address must be 32-byte aligned (L1 Buffer alignment requirement); otherwise, the copy will be abnormal.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- L0B Buffer capacity limit: The total L0B Buffer capacity is 64 KB. The sum of the dst offset and the copy size must not exceed the boundary; otherwise, an exception is triggered.
- L1 Buffer capacity limit: The total L1 Buffer capacity is 512 KB. The sum of the src offset and the source cube size must not exceed the boundary; otherwise, an exception is triggered.

### Constraints on 3D img2col Transfer Mode

- Before calling this API, you must call [asc_set_l13d_rpt_b](../asc_set_l13d_rpt_b.md) to configure the dst_stride parameter, which is the size of the output cube after alignment in the height dimension.
- Before calling this API, you must call the matching register configuration APIs [asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md) and [asc_set_l12l0b_3d_padding](../asc_set_l12l0b_3d_padding.md) to preconfigure the feature map description, padding value, and padding mode. Otherwise, the transfer result may not meet expectations. To use the repeat mode, you must first call [asc_set_l13d_rpt_b](../asc_set_l13d_rpt_b.md) to configure the repeat direction, count, and stride.
- When 3D formatted data is transferred to the L0B Buffer, it is automatically transposed. The transpose parameter is invalid, and non-transposed scenarios are not supported.
- The transfer is not performed and this API is treated as a NOP (no operation) when `k_extension`, `m_extension`, or `channel_size` is `0`; when `filter_w` is `0` and `filter_size_w` is `false`; or when `filter_h` is `0` and `filter_size_h` is `false`.
- For the b32 type, the remainder of `channel_size` divided by 8 must be 0 or 4, and the maximum value is 65532. For the b16 type, the remainder of `channel_size` divided by 16 must be 0, 4, or 8, and the maximum value is 65528. For the b8 type, the remainder of `channel_size` divided by 32 must be 0, 4, 8, or 16, and the maximum value is 65520. Otherwise, the transfer result may not meet expectations.
- `stride_w` and `stride_h` cannot be 0. If 0 is passed, the hardware treats it as 1.
- The value range of `stride_w` is [1, 63]. Values outside this range are truncated, causing the transfer result to not meet expectations.
- The value range of `stride_h` is [1, 63]. Values outside this range are truncated, causing the transfer result to not meet expectations.

## Key Features<a id="l12l0b_3d_key_features"></a>

### Using 3D Convolution Data Transfer to Move the 2D Formatted Data Required for Matrix Computation

3D data transfer supports transpose for the b8, b16, and b32 data types. The L1 Buffer -> L0B Buffer path automatically enables transpose transfer. The following describes each type in detail.

- For the b8 scenario: Each fractal is 16×32. The transfer diagram is as follows:

    ![](../../../../figures/load3d_l1tol0b_b8_trans_950.png)

- For the b16 scenario: Each fractal is 16×16. The transfer diagram is as follows:

    ![](../../../../figures/load3d_l1tol0b_b16_trans_950.png)

- For the b32 scenario: Each fractal is 16×8. The transfer diagram is as follows:

    ![](../../../../figures/load3d_l1tol0b_b32_trans_950.png)

## Examples

The following example uses `asc_copy_l12l0b` to expand a 2×2, 16-channel all-zero Feature Map into a 4×4 cube with one ring of padding, and automatically transposes it when writing to the L0B Buffer. The padding value on the B side is set to 2, and `f_matrix_ctrl` is set to `true`. After performing MMAD with an all-one L0A cube, each output accumulates the 12 padding positions in the 4×4 region, with an expected value of `12 × 2 = 24`.

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#include "acl/acl.h"
#include "c_api/asc_simd.h"

namespace {
constexpr uint32_t H = 2;
constexpr uint32_t W = 2;
constexpr uint32_t C0 = 16;
constexpr uint32_t CIN = C0;
constexpr uint32_t COUT = C0;
constexpr uint32_t PAD = 1;
constexpr uint32_t HOUT = H + 2 * PAD;
constexpr uint32_t WOUT = W + 2 * PAD;
constexpr uint32_t M = HOUT * WOUT;
constexpr uint32_t M_ALIGN = 16;
constexpr uint32_t K = CIN;
constexpr uint32_t K_ALIGN = 16;
constexpr half PAD_VALUE = static_cast<half>(2);
constexpr uint32_t FMAP_ELEMENTS = H * W * CIN;
constexpr uint32_t WEIGHT_ELEMENTS = COUT * K;
constexpr uint32_t OUTPUT_ELEMENTS = COUT * M;

__global__ __cube__ void asc_copy_l12l0b_3d_bside_kernel(
    __gm__ half* fmap, __gm__ half* weight, __gm__ half* output)
{
    asc_init();
    __cbuf__ half fmap_l1[FMAP_ELEMENTS];
    __cbuf__ half weight_l1[WEIGHT_ELEMENTS];
    __ca__ half weight_l0[COUT * K_ALIGN];
    __cb__ half fmap_l0[K_ALIGN * M_ALIGN];
    __cc__ float output_l0[COUT * M_ALIGN];

    asc_set_gm2l1_nz_para(1, 1, H * W, 0);
    asc_copy_gm2l1_nd2nz(fmap_l1, fmap, CIN * sizeof(half), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, H * W, CIN, 0, false);
    asc_set_gm2l1_nz_para(1, 1, K_ALIGN, 0);
    asc_copy_gm2l1_nd2nz(weight_l1, weight, K * sizeof(half), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, COUT, K, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

    asc_copy_l12l0a(weight_l0, weight_l1, 0, 0, COUT / 16, K_ALIGN / 16, COUT / 16, COUT / 16);

    asc_l13d_fmatrix_config fmatrix_config;
    fmatrix_config.l1_width = W;
    fmatrix_config.l1_height = H;
    fmatrix_config.padding_left_size = PAD;
    fmatrix_config.padding_right_size = PAD;
    fmatrix_config.padding_top_size = PAD;
    fmatrix_config.padding_bottom_size = PAD;
    // When f_matrix_ctrl is true, 3D transfer reads the Feature Map descriptor and padding value from the B-side register group.
    asc_set_l13d_fmatrix_b(fmatrix_config);
    asc_set_l12l0b_3d_padding(PAD_VALUE);
    asc_set_l13d_rpt_b(0, 1, asc_l13d_repeat_direction::M_DIRECTION, M_ALIGN / 16, 0);
    // The destination address is L0B Buffer; f_matrix_ctrl=true selects the B-side register group configured above.
    asc_copy_l12l0b(
        fmap_l0, fmap_l1, K_ALIGN, M, 0, 0, 1, 1, 1, 1, 1, 1, false, false, false, true, CIN);

    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(output_l0, weight_l0, fmap_l0, COUT, K_ALIGN, M, asc_unit_flag_mode::DISABLE, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(
        output, output_l0, M, COUT, M, COUT, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
        asc_unit_flag_mode::DISABLE, QuantMode_t::F322F16, asc_relu_pre_mode::NONE, false, true, false, false);
    asc_sync_pipe(PIPE_ALL);
}

} // namespace

int main()
{
    // The valid Feature Map is all zeros; the L0A cube is all ones, so that the MMAD result reflects only the padding values in L0B.
    std::vector<half> fmap(FMAP_ELEMENTS, static_cast<half>(0));
    std::vector<half> weight(WEIGHT_ELEMENTS, static_cast<half>(0));
    std::vector<half> output(OUTPUT_ELEMENTS);
    for (uint32_t i = 0; i < WEIGHT_ELEMENTS; ++i) weight[i] = static_cast<half>(1);

    half *fmap_device = nullptr, *weight_device = nullptr, *output_device = nullptr;
    if (aclInit(nullptr) != ACL_SUCCESS || aclrtSetDevice(0) != ACL_SUCCESS ||
        aclrtMalloc(reinterpret_cast<void**>(&fmap_device), fmap.size() * sizeof(half), ACL_MEM_MALLOC_HUGE_FIRST) != ACL_SUCCESS ||
        aclrtMalloc(reinterpret_cast<void**>(&weight_device), weight.size() * sizeof(half), ACL_MEM_MALLOC_HUGE_FIRST) != ACL_SUCCESS ||
        aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(half), ACL_MEM_MALLOC_HUGE_FIRST) != ACL_SUCCESS) {
        return 1;
    }
    aclrtMemcpy(fmap_device, fmap.size() * sizeof(half), fmap.data(), fmap.size() * sizeof(half), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(weight_device, weight.size() * sizeof(half), weight.data(), weight.size() * sizeof(half), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_copy_l12l0b_3d_bside_kernel<<<1, 0>>>(fmap_device, weight_device, output_device);
    const bool runtime_ok = aclrtSynchronizeDevice() == ACL_SUCCESS &&
        aclrtMemcpy(output.data(), output.size() * sizeof(half), output_device, output.size() * sizeof(half), ACL_MEMCPY_DEVICE_TO_HOST) == ACL_SUCCESS;
    uint32_t match_count = 0;
    uint32_t unexpected_count = 0;
    constexpr float EXPECTED_OUTPUT = 12.0f * static_cast<float>(PAD_VALUE);
    for (uint32_t i = 0; i < OUTPUT_ELEMENTS; ++i) {
        const float value = static_cast<float>(output[i]);
        if (std::fabs(value - EXPECTED_OUTPUT) <= 1e-3f) ++match_count;
        else {
            ++unexpected_count;
            if (unexpected_count <= 12) std::cout << "UNEXPECTED index=" << i << " value=" << value << std::endl;
        }
    }
    const bool passed = runtime_ok && match_count == OUTPUT_ELEMENTS && unexpected_count == 0;
    std::cout << "RESULT status=" << (passed ? "PASS" : "FAIL") << " expected=" << EXPECTED_OUTPUT
              << " match_count=" << match_count << '/' << OUTPUT_ELEMENTS
              << " unexpected=" << unexpected_count << std::endl;
    aclrtFree(output_device);
    aclrtFree(weight_device);
    aclrtFree(fmap_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
