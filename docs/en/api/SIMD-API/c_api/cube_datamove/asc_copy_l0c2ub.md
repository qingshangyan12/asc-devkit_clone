# asc_copy_l0c2ub

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T13:04:50.593Z pushedAt=2026-10-28T12:45:45.442Z nl=2 -->

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

Copies cube computation results from the L0C Buffer to the Unified Buffer. During the copy, it can simultaneously support combinations of inline quantization, inline activation, and inline format conversion (Nz2ND/Nz2DN).

The following figure shows the valid combinations, intermediate data types, and data paths of inline quantization, inline ReLU, inline format conversion, inline channel splitting, and inline channel merging. The line colors distinguish the supported scenario combinations. In the figure, F32-\>F16 and F32-\>BF16 are Cast operations, and the rest are inline scalar/tensor quantization modes.

**Figure 1** Inline function combinations of asc_copy_l0c2ub

![](../../../figures/C_API_L0C2UB_Function_Combination_950.png)

This API supports combinations of multiple inline capabilities. You need to pre-configure registers such as quantization parameters, activation parameters, and channel parameters through the companion APIs, and then call this API to complete the copy.

- In the Nz2ND format conversion scenario, pre-configure the format conversion parameters through [asc_set_l0c_copy_nz_para](asc_set_l0c_copy_nz_para.md), and use them together with the `enable_nz2nd` parameter of this API.
- In the Nz2DN format conversion scenario, pre-configure the format conversion parameters through [asc_set_l0c_copy_nz_para](asc_set_l0c_copy_nz_para.md) and [asc_set_l0c_copy_channel_para](asc_set_l0c_copy_channel_para.md), and use them together with the `enable_nz2dn` parameter of this API.
- In the inline scalar quantization mode, set the inline scalar quantization parameters through [asc_set_l0c_copy_prequant](asc_set_l0c_copy_prequant.md), and use them together with the `quant_pre_mode` parameter of this API.
- In the inline tensor quantization mode, set the start address of the tensor used for inline tensor quantization through [asc_set_l0c_copy_config](asc_set_l0c_copy_config.md), where each element of the quantization tensor represents a quantization parameter, and use them together with the `quant_pre_mode` parameter of this API.
- In inline activation mode, pre-configure the ReLU/Leaky ReLU activation parameters through [asc_set_l0c_copy_relu_alpha](asc_set_l0c_copy_relu_alpha.md) and [asc_set_l0c_copy_lrelu_alpha](asc_set_l0c_copy_lrelu_alpha.md), and use them together with the `enable_clip_relu_pre` and `relu_pre_mode` parameters of this API.

The supported enumeration values of the `quant_pre_mode` quantization mode parameter are as follows:

- NoQuant: Quantization is disabled.
- DEQF16: Quantize int32_t to half, scalar quantization.
- VDEQF16: Quantize int32_t to half, tensor quantization.
- REQ4: Quantize int32_t to int4b_t, scalar quantization.
- VREQ4: Quantize int32_t to int4b_t, tensor quantization.
- REQ8: Quantize int32_t to int8_t/uint8_t, scalar quantization.
- VREQ8: Quantize int32_t to int8_t/uint8_t, tensor quantization.
- QS322BF16_PRE: Quantize int32_t to bfloat16_t, scalar quantization.
- VQS322BF16_PRE: Quantize int32_t to bfloat16_t with tensor quantization.
- QF322F16_PRE: Quantize float to half with scalar quantization.
- VQF322F16_PRE: Quantize float to half with tensor quantization.
- QF322BF16_PRE: Quantize float to bfloat16_t with scalar quantization.
- VQF322BF16_PRE: Quantize float to bfloat16_t with tensor quantization.
- F322F16: Cast float to half with the cast mode set to CAST_RINT.
- F322BF16: Cast float to bfloat16_t with the cast mode set to CAST_RINT.
- QF322S4_PRE: Quantize float to int4b_t with scalar quantization.
- VQF322S4_PRE: Quantize float to int4b_t with tensor quantization.
- QF322B8_PRE: Quantize float to int8_t/uint8_t with scalar quantization.
- VQF322B8_PRE: Quantize float to int8_t/uint8_t with scalar quantization.
- QF322FP8_PRE: Quantize float to fp8_e4m3fn_t with scalar quantization.
- VQF322FP8_PRE: Quantize float to fp8_e4m3fn_t with tensor quantization.
- QF322HIF8_PRE: Quantize float to hifloat8_t (Half to Away Round) with scalar quantization.
- VQF322HIF8_PRE: Quantize float to hifloat8_t (Half to Away Round) with tensor quantization.
- QF322HIF8_PRE_HYBRID: Quantize float to hifloat8_t (Hybrid Round) with scalar quantization.
- VQF322HIF8_PRE_HYBRID: Quantize float to hifloat8_t (Hybrid Round) with tensor quantization.
- QF322F32_PRE: Quantize float to float with scalar quantization. The precision can reach two thousandths but cannot reach two ten-thousandths.
- VQF322F32_PRE: Quantize float to float with tensor quantization. The precision can reach two thousandths but cannot reach two ten-thousandths.

This API takes effect only when executed on the AIC.

## Prototype

### Template Prototype (Placeholder Form)

```cpp
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       int8_t sub_blockid,
                                       asc_dual_dst_mode dual_dst_ctrl,
                                       asc_unit_flag_mode unit_flag_mode,
                                       asc_quant_mode quant_pre_mode,
                                       asc_relu_pre_mode relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       bool enable_nz2dn,
                                       bool enable_clip_relu_pre)
```

### Data Types Supported by dtype

The following combinations of src dtype and dst dtype are supported:

- When `src_dtype` is `int32_t`, `dst_dtype` supports `int4b_t`, `int8_t`, `uint8_t`, `half`, `bfloat16_t`, and `int32_t`.
- When `src_dtype` is `float`, `dst_dtype` supports `int4b_t`, `int8_t`, `uint8_t`, `hifloat8_t`, `fp8_e4m3fn_t`, `half`, `bfloat16_t`, and `float`.

### Prototype Example

```cpp
// Example: Convert float data to half and then move it.
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ half* dst,
                                       __cc__ float* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       int8_t sub_blockid,
                                       asc_dual_dst_mode dual_dst_ctrl,
                                       asc_unit_flag_mode unit_flag_mode,
                                       asc_quant_mode quant_pre_mode,
                                       asc_relu_pre_mode relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       bool enable_nz2dn,
                                       bool enable_clip_relu_pre)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Meaning |
|---|---|---|
| dst | Output | Destination operand, stored in UB. The start address must be 32-byte aligned. |
| src | Input | Source operand, stored in the L0C Buffer. The start address must be 64-byte aligned. |
| n_size | Input | Size of the source Nz cube in the N direction, with a value range of $[1, 2^{12}-1]$.<br>For the Nz output scenario:<br>&nbsp;&nbsp;&bull; If `enable_channel_split` is set to `true` to enable Channel Split, `n_size` must be a multiple of 8.<br>&nbsp;&nbsp;&bull; If Channel Split is not enabled, `n_size` must be a multiple of 16.<br>For the ND output scenario:<br>&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$ must be a multiple of 32; if `dst_dtype` is set to `int4b_t`, `n_size` must be a multiple of 64.<br> |
| m_size | Input | Size of the source Nz cube in the M direction, with a value range of $[1, 2^{16}-1]$.<br>For the DN output scenario:<br>&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$ must be a multiple of 32; if `dst_dtype` is set to `int4b_t`, `m_size` must be a multiple of 64.<br> |
| dst_stride | Input | Destination cube stride, with a value range of $[1, 2^{32}-1]$. The corresponding byte length must be 32-byte aligned.<br>&nbsp;&nbsp;&bull; If Nz2ND is not enabled, `dst_stride` indicates the start address offset between adjacent Z layouts in the destination Nz cube, in elements.<br>&nbsp;&nbsp;&bull; If Nz2ND/Nz2DN is enabled, `dst_stride` indicates the number of elements in each row of the destination ND/DN cube, in elements. The alignment constraints for different `dst_dtype` values are as follows: `int4b_t` output must be a multiple of 64, 8-bit output must be a multiple of 32, 16-bit output must be a multiple of 16, and 32-bit output must be a multiple of 8.<br> |
| src_stride | Input | Start address offset between adjacent Z layouts in the source Nz cube, in units of 64 bytes, that is, $16\times\operatorname{sizeof}(src\_dtype)$. The value range is $[0, 2^{16}-1]$. |
| sub_blockid | Input | SUB BLOCK ID of the target UB when single-target mode is enabled. The value is 0 or 1: 0 writes to SUB BLOCK0, and 1 writes to SUB BLOCK1. |
| dual_dst_ctrl | Input | Dual-target mode control parameter. When an AI Core contains one Cube Core and two Vector Cores, if dual-target mode control is enabled, the M×N cube in the L0C Buffer is split into two halves and written simultaneously to the UBs of the two Vector Cores, with the first half written to SUB BLOCK0 and the second half to SUB BLOCK1. The parameter values are described as follows:<br>&nbsp;&nbsp;&bull; `asc_dual_dst_mode::DUAL_DST_DISABLE`: Single-target mode. The entire cube is written to the target UB configured by the `sub_blockid` parameter.<br>&nbsp;&nbsp;&bull; `asc_dual_dst_mode::DUAL_DST_SPLIT_M`: Dual-target mode. The cube is split along the M dimension into two cubes of shape $M/2\times N$, which are written to the two UBs respectively. M must be a multiple of 2.<br>&nbsp;&nbsp;&bull; `asc_dual_dst_mode::DUAL_DST_SPLIT_N`: Dual-target mode. The cube is split along the N dimension into two cubes of shape $M\times N/2$, which are written to the two UBs respectively. N must be a multiple of 32.<br>This parameter supports only the normal move mode (Nz2Nz) or the Nz2ND move scenario, and does not support other inline function scenarios. |
| unit_flag_mode | Input | UnitFlag is a fine-grained parallel feature for MMAD instructions and cube move-out instructions. When enabled, the hardware moves out the computation result as soon as each fractal is computed. The values are described as follows:<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`: UnitFlag is not enabled.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`: UnitFlag is enabled. After the hardware finishes executing the instruction, the unit flag bit is not reset.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`: UnitFlag is enabled. After the hardware finishes executing the instruction, the unit flag bit is reset.<br>When this feature is enabled, set the UnitFlag value of the MMAD instructions and cube move-out instructions to `asc_unit_flag_mode::ENABLE_KEEP` or `asc_unit_flag_mode::ENABLE_UPDATE`. |
| quant_pre_mode | Input | Quantization mode in the pre-processing stage. For values, see [Description](#description). |
| relu_pre_mode | Input | ReLU mode control in the pre-processing stage. The values are as follows:<br>&nbsp;&nbsp;&bull; `asc_relu_pre_mode::NONE`: ReLU is not enabled.<br>&nbsp;&nbsp;&bull; `asc_relu_pre_mode::NORMAL`: Normal ReLU is enabled.<br>&nbsp;&nbsp;&bull; `asc_relu_pre_mode::SCALAR`: Scalar ReLU is enabled.<br>&nbsp;&nbsp;&bull; `asc_relu_pre_mode::VECTOR`: Vector ReLU is enabled.<br> |
| enable_channel_split | Input | Whether to enable Channel Split. It can be enabled only when both `src_dtype` and `dst_dtype` are `float` and the output is in Nz format.<br>&nbsp;&nbsp;&bull; `false`: Not enabled.<br>&nbsp;&nbsp;&bull; `true`: Enabled.<br> |
| enable_nz2nd | Input | Nz2ND format conversion enable.<br>&nbsp;&nbsp;&bull; `false`: Nz2ND conversion is disabled.<br>&nbsp;&nbsp;&bull; `true`: Nz2ND conversion is enabled.<br> |
| enable_nz2dn | Input | Nz2DN format conversion enable.<br>&nbsp;&nbsp;&bull; `false`: Nz2DN conversion is disabled.<br>&nbsp;&nbsp;&bull; `true`: Nz2DN conversion is enabled.<br> |
| enable_clip_relu_pre | Input | Whether to enable Clip ReLU. It must be used together with Normal ReLU and requires quantization to be enabled. The values are as follows:<br>&nbsp;&nbsp;&bull; `false`: Clip ReLU is not enabled.<br>&nbsp;&nbsp;&bull; `true`: Clip ReLU is enabled (scalar mode).<br> |

## Return Value

None

## Pipeline Type

PIPE_FIX

## Constraints

### General Constraints

- This API returns directly when it is not called by the AIC.
- The `dst` start address must be 32-byte aligned; otherwise, an exception is triggered.
- The `src` start address must be 64-byte aligned; otherwise, an exception is triggered.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- UB capacity limit: The total UB capacity is 256 KB. After reserving 6 KB for the SIMD VF stack and 2 KB for Ascend C reserved space by default, 248 KB is available. When SIMD and SIMT are mixed, an additional 32 KB to 128 KB is allocated as Data Cache, further reducing the available capacity. The sum of the dst offset and the move size must not exceed the actual available capacity; otherwise, a write overflow exception is triggered.
- L0C Buffer capacity limit: The total L0C Buffer capacity is 256 KB. The sum of the src offset and the move size must not exceed the L0C Buffer capacity; otherwise, a read overflow exception is triggered.
- An Nz cube uses a 16×16 element block as the basic fractal. In boundary fractals, data beyond the range specified by `n_size` or `m_size` is not part of the valid copy result.

### Inline Conversion Constraints

- `n_size`, `m_size`, and `dst_stride` must satisfy the alignment constraints determined by the dtype and function mode. For details, see the parameter description. Failing to meet the alignment constraints leads to unexpected copy results.
- The src and dst dtype combination must match the `quant_pre_mode` quantization mode; otherwise, the copy result is unexpected.
- `dual_dst_ctrl` is supported only in the normal copy mode (Nz2Nz) or the Nz2ND copy scenario, and is not supported in other inline function scenarios.
- `enable_channel_split` can be set to true only when the output dtype is `float` and the output is in Nz format.
- The quantization coefficients used in quantization and activation modes must not be INF, NAN, or denormalized numbers; otherwise, the quantization and activation result is incorrect.
- When Nz2DN conversion is enabled, pre-configure the source cube stride through [asc_set_l0c_copy_channel_para](asc_set_l0c_copy_channel_para.md). The source cube stride must not be 0; otherwise, the copy is abnormal.
- When Nz2DN conversion is enabled, the UnitFlag function can be enabled simultaneously only when the source cube stride configured through [asc_set_l0c_copy_channel_para](asc_set_l0c_copy_channel_para.md) is 1.
- When `enable_clip_relu_pre` is set to Clip ReLU (scalar mode), it must be used together with `relu_pre_mode` and the quantization function.

## Examples

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

This example uses 32 KB of L1 Buffer, 16 KB of L0A Buffer, 16 KB of L0B Buffer, 64 KB of L0C Buffer, and 64 KB of UB, and uses `EVENT_ID0` and synchronization resource ID `0x8` to complete pipeline and inter-core synchronization.

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t M = 128;
constexpr uint32_t K = 128;
constexpr uint32_t N = 128;
constexpr uint32_t ELEMENTS = M * N;
constexpr uint32_t CUBE_M = 16;
constexpr uint32_t CUBE_K = 32;

__global__ __mix__(1, 2) void AscCopyL0c2ubKernel(__gm__ int8_t* a, __gm__ int8_t* b,
    __gm__ int32_t* output)
{
    asc_init();
    __cbuf__ int8_t a_l1[M * K], b_l1[N * K];
    __ca__ int8_t a_l0[M * K];
    __cb__ int8_t b_l0[K * N];
    __cc__ int32_t c_l0[ELEMENTS];
    __ubuf__ int32_t* output_ub = reinterpret_cast<__ubuf__ int32_t*>(0);

    if ASC_IS_AIC {
        // Convert the two 128x128 ND inputs to the Nz layout required by MMAD.
        asc_set_gm2l1_nz_para(1, 1, 128, 0);
        asc_copy_gm2l1_nd2nz(a_l1, a, K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, M, K, 0, false);
        asc_set_gm2l1_nz_para(1, 1, 128, 0);
        asc_copy_gm2l1_nd2nz(b_l1, b, K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, N, K, 0, false);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

        // Move cubes A and B from L1 Buffer to L0A Buffer and L0B Buffer.
        asc_copy_l12l0a(a_l0, a_l1, 0, 0, M / CUBE_M, K / CUBE_K, M / CUBE_M, M / CUBE_M);
        asc_copy_l12l0b(b_l0, b_l1, 0, 0, N / CUBE_M, K / CUBE_K, N / CUBE_M, N / CUBE_M);
        asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);

        // Perform 128x128x128 cube multiplication and write the result to L0C Buffer.
        asc_mmad(c_l0, a_l0, b_l0, M, K, N, asc_unit_flag_mode::DISABLE, true, false, true);
        asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
        asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);

        // Convert the Nz result in L0C Buffer to the contiguous ND format and move it to the UB on the AIV side.
        asc_set_l0c_copy_nz_para(1, 0, ELEMENTS);
        asc_copy_l0c2ub(output_ub, c_l0, N, M, N, M, 0, asc_dual_dst_mode::DUAL_DST_DISABLE,
            asc_unit_flag_mode::DISABLE, asc_quant_mode::NoQuant, asc_relu_pre_mode::NONE,
            false, true, false, false);
        asc_sync_block_arrive(PIPE_FIX, 0x8);
    }
    if ASC_IS_AIV {
        if (asc_get_sub_block_id() != 0) {
            return;
        }
        // Wait for the AI Core to finish moving data from L0C to UB, then AIV0 moves the result to GM.
        asc_sync_block_wait(PIPE_MTE3, 0x8);
        asc_copy_ub2gm(output, output_ub, M, N * sizeof(int32_t),
            N * sizeof(int32_t), N * sizeof(int32_t));
        asc_sync_pipe(PIPE_ALL);
    }
}

template <typename T>
void PrintRow(const char* label, const std::vector<T>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) {
        std::cout << ' ' << +data[i];
    }
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<int8_t> a(M * K), b(N * K);
    std::vector<int32_t> output(ELEMENTS), golden(ELEMENTS);
    // Construct the asymmetric input and compute the golden result on the Host side.
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t k = 0; k < K; ++k) {
            a[row * K + k] = static_cast<int8_t>(static_cast<int32_t>((row + 2 * k) % 5) - 2);
        }
    }
    for (uint32_t col = 0; col < N; ++col) {
        for (uint32_t k = 0; k < K; ++k) {
            b[col * K + k] = static_cast<int8_t>(static_cast<int32_t>((3 * col + k) % 5) - 2);
        }
    }
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t col = 0; col < N; ++col) {
            for (uint32_t k = 0; k < K; ++k) {
                golden[row * N + col] += a[row * K + k] * b[col * K + k];
            }
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    int8_t *a_device = nullptr, *b_device = nullptr;
    int32_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(int32_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size(), a.data(), a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size(), b.data(), b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    AscCopyL0c2ubKernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int32_t), output_device,
        output.size() * sizeof(int32_t), ACL_MEMCPY_DEVICE_TO_HOST);

    PrintRow("Output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_l0c2ub result is correct."
                         : "[Failed] asc_copy_l0c2ub result mismatch.") << std::endl;

    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
