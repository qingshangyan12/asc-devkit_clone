# asc_mmad_mx

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:15:43.335Z pushedAt=2026-10-28T12:21:43.306Z nl=2 -->

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

The header file path is `"c_api/cube_compute/cube_compute.h"`.

This API performs cube multiply-accumulate with quantization coefficients (MmadMx). Both the left cube A and the right cube B have corresponding quantization coefficient matrices: the left quantization coefficient cube ScaleA and the right quantization coefficient cube ScaleB. In the MmadMx scenario, the left quantization coefficient cube is multiplied by the left cube, the right quantization coefficient cube is multiplied by the right cube, and then cube multiplication is performed on the two products. The calculation formula is as follows:

$$
C_{M \times N} = \left(ScaleA_{M \times (K/32)} \otimes A_{M \times K}\right) \times \left(ScaleB_{(K/32) \times N} \otimes B_{K \times N}\right) + C_{M \times N}
$$

Here, $\otimes$ denotes broadcast multiplication. Every 32 consecutive elements of matrices A and B along the K dimension share one quantization coefficient from the corresponding quantization coefficient matrices ScaleA and ScaleB. By configuring the parameters of this API, you can initialize the C cube to an all-zero cube, the cube in the L0C Buffer, or the Bias cube.

This API does not take the Scale cube addresses as separate parameters. Instead, it reads the corresponding Scale matrices from the L0A_MX Buffer and L0B_MX Buffer based on the start addresses of matrices A and B, following a fixed address mapping relationship. The information about each cube is described in the following table:

**Table 1** Cube information

| Cube | Storage Location | Shape (Rows × Columns) | Data Format | Fractal Size (Rows × Columns) |
| --- | --- | --- | --- | --- |
| A | L0A Buffer | M×K | Nz | 16×K0 |
| ScaleA | L0A_MX Buffer | M×(K / 32) | Zz | 16×2 |
| B | L0B Buffer | K×N | Zn | K0×16 |
| ScaleB | L0B_MX Buffer | (K / 32)×N | Nn | 2×16 |
| C | L0C Buffer | M×N | Nz | 16×16 |
| Bias (used for C cube initialization) | BiasTable Buffer | 1×N, broadcast-copied to M rows to initialize the C cube | ND | - |

When the cube data type is `fp8_e5m2_t` or `fp8_e4m3fn_t`, `K0 = 32`; when the cube data type is `fp4x2_e2m1_t` or `fp4x2_e1m2_t`, `K0 = 64`.

This API is a cube computation API and takes effect only on the AIC.

## Prototype

**Placeholder form:**

```c
// The Bias start address is not explicitly passed. The C cube initialization mode is controlled by parameters.
__aicore__ inline void asc_mmad_mx(__cc__ float* c_matrix,
                                    __ca__ <a_dtype>* a_matrix,
                                    __cb__ <b_dtype>* b_matrix,
                                    uint16_t m,
                                    uint16_t k,
                                    uint16_t n,
                                    asc_unit_flag_mode unit_flag_mode,
                                    bool disable_gemv,
                                    bool c_matrix_source,
                                    bool c_matrix_init_val)

// The Bias start address is explicitly passed. The C cube is initialized with the Bias cube.
__aicore__ inline void asc_mmad_mx(__cc__ float* c_matrix,
                                    __ca__ <a_dtype>* a_matrix,
                                    __cb__ <b_dtype>* b_matrix,
                                    uint64_t bias,
                                    uint16_t m,
                                    uint16_t k,
                                    uint16_t n,
                                    asc_unit_flag_mode unit_flag_mode,
                                    bool disable_gemv)
```

**Data types supported by dtype:**

The value combinations of `a_dtype` and `b_dtype` are as follows:

**Table 2** Supported data type combinations<a id="asc_mmad_mx_data_type"></a>

| a_dtype | b_dtype | ScaleA/ScaleB Data Type | Result Cube C Data Type | Bias Data Type |
| --- | --- | --- | --- | --- |
| `fp4x2_e1m2_t` | `fp4x2_e1m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e1m2_t` | `fp4x2_e2m1_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e2m1_t` | `fp4x2_e1m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp4x2_e2m1_t` | `fp4x2_e2m1_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e5m2_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e4m3fn_t` | `fp8_e8m0_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e5m2_t` | `fp8_e8m0_t` | `float` | `float` |

**Typical example:**

```c
// Do not explicitly pass the Bias start address.
__aicore__ inline void asc_mmad_mx(__cc__ float* c_matrix,
                                    __ca__ fp4x2_e1m2_t* a_matrix,
                                    __cb__ fp4x2_e2m1_t* b_matrix,
                                    uint16_t m,
                                    uint16_t k,
                                    uint16_t n,
                                    asc_unit_flag_mode unit_flag_mode,
                                    bool disable_gemv,
                                    bool c_matrix_source,
                                    bool c_matrix_init_val)


// Explicitly pass the Bias start address.
__aicore__ inline void asc_mmad_mx(__cc__ float* c_matrix,
                                    __ca__ fp4x2_e1m2_t* a_matrix,
                                    __cb__ fp4x2_e2m1_t* b_matrix,
                                    uint64_t bias,
                                    uint16_t m,
                                    uint16_t k,
                                    uint16_t n,
                                    asc_unit_flag_mode unit_flag_mode,
                                    bool disable_gemv)
```

## Parameters

**Table 3** Parameter description<a id="asc_mmad_mx_param_table"></a>

| Parameter | Input/Output | Description |
| --- | --- | --- |
| c_matrix | Output | Destination operand, the start address of result cube C in the L0C Buffer. The data type is `float`, and the address must be aligned to 1024 bytes. |
| a_matrix | Input | Source operand, the start address of left cube A in the L0A Buffer. The data type is determined by the API overload. For details, see [Table 2](#asc_mmad_mx_data_type).<br>When the cube data type is `fp4x2_e2m1_t` or `fp4x2_e1m2_t`, the address must be aligned to 512 bytes. When the cube data type is `fp8_e5m2_t` or `fp8_e4m3fn_t`, the address must be aligned to 1024 bytes.<br>The start address of the ScaleA cube in the L0A_MX Buffer is the start address of `a_matrix` divided by 16. |
| b_matrix | Input | Source operand, the start address of right cube B in the L0B Buffer. The data type is determined by the API overload. For details, see [Table 2](#asc_mmad_mx_data_type).<br>When the cube data type is `fp4x2_e2m1_t` or `fp4x2_e1m2_t`, the address must be aligned to 512 bytes. When the cube data type is `fp8_e5m2_t` or `fp8_e4m3fn_t`, the address must be aligned to 1024 bytes.<br>The start address of the ScaleB cube in the L0B_MX Buffer is the start address of `b_matrix` divided by 16. |
| bias | Input | Start address of the Bias cube in the BiasTable Buffer. The address must be aligned to 64 bytes and can be a non-zero address. This parameter is included only in the API overload that explicitly passes the Bias start address.<br>The supported data type of Bias is `float`. Before calling this API, move the Bias data to this address through the [asc_copy_l12bt](../cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_3510.md) API. The Bias occupies `n * sizeof(float)` bytes aligned up to 64 bytes. |
| m | Input | M dimension size of left cube A and result cube C, in elements. Value range: [0, 4095]. |
| k | Input | K dimension size of left cube A and right cube B, in elements. Value range: [0, 4095], and must be a multiple of 64. |
| n | Input | N dimension size of right cube B and result cube C, in elements. Value range: [0, 4095]. |
| unit_flag_mode | Input | Controls fine-grained parallelism between the cube multiply-accumulate instruction and the cube move-out instruction. When UnitFlag is enabled, the hardware moves out the computation result each time it finishes computing a fractal. The values are described as follows:<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`: UnitFlag is not enabled.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`: UnitFlag is enabled, and the hardware does not change the unit flag after executing the instruction.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`: UnitFlag is enabled, and the hardware changes the unit flag after executing the instruction.<br>The cube multiply-accumulate instruction and the corresponding cube move-out instruction must both enable or both disable UnitFlag. When UnitFlag is enabled, no synchronization instruction needs to be inserted between the instructions. |
| disable_gemv | Input | When M is 1, configures whether to disable GEMV mode.<br>&nbsp;&nbsp;&bull; false: GEMV mode is enabled.<br>&nbsp;&nbsp;&bull; true: GEMV mode is disabled.<br>When M is not 1, this parameter does not take effect. |
| c_matrix_source | Input | Included only in the overload that does not explicitly pass the Bias start address. When `c_matrix_init_val` is false, configures the source of the initial value of cube C.<br>&nbsp;&nbsp;&bull; false: The initial value of cube C comes from the L0C Buffer.<br>&nbsp;&nbsp;&bull; true: The initial value of cube C comes from the BiasTable Buffer and is always read starting from address 0. |
| c_matrix_init_val | Input | Included only in the overload that does not explicitly pass the Bias start address. Configures whether to set the initial value of cube C to 0.<br>&nbsp;&nbsp;&bull; true: The initial value of cube C is set to 0, and `c_matrix_source` does not take effect.<br>&nbsp;&nbsp;&bull; false: No zeroing is performed, and the initial value of cube C is configured by `c_matrix_source`. |

## Return Value

None

## Pipeline Type

PIPE_M

## Constraints

- This API takes effect only on the AIC. Calling it on the AIV returns immediately.
- If any of `m`, `k`, and `n` is 0, the API is treated as a NOP (no operation).
- Before calling this API, move the ScaleA and ScaleB matrices into the L0A_MX Buffer and L0B_MX Buffer through APIs such as [asc_copy_l12l0a_mx](../cube_datamove/asc_copy_l12l0a_mx.md) and [asc_copy_l12l0b_mx](../cube_datamove/asc_copy_l12l0b_mx.md). The data type of the ScaleA and ScaleB matrices must be `fp8_e8m0_t`, and each Scale element in the K direction corresponds to 32 input cube elements.

- Memory usage constraints:

  - The L0C Buffer size is 256 KB, the L0A Buffer and L0B Buffer sizes are both 64 KB, the L0A_MX Buffer and L0B_MX Buffer sizes are both 4 KB, and the BiasTable Buffer size is 4 KB. The start address and occupied space of a cube must not exceed the range of the corresponding Buffer.
  - The start address of each cube must meet the alignment requirements in [Table 3](#asc_mmad_mx_param_table). For other address constraints of the operands, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
  - The start address of the ScaleA cube in the L0A_MX Buffer is the start address of `a_matrix` divided by 16, and the start address of the ScaleB cube in the L0B_MX Buffer is the start address of `b_matrix` divided by 16. When moving in the Scale matrices, use destination addresses that match cube A and cube B.
  - `k` must be a multiple of 64. If the original K is not a multiple of 64, align K up to a multiple of 64 and set the cube A and cube B data in the padded region to 0. The `k` passed to the API, the cube storage space, and the K dimension size of the Scale matrices must all be calculated based on the aligned K.
  - The M and N dimensions of the cube storage space must be aligned up to multiples of 16 respectively, while `m` and `n` still pass the valid M and N values of the cube. The padded portion is invalid data and does not participate in the computation of the valid region of the result cube.
  - The Scale matrices must cover all data required for this cube multiply-accumulate operation. The M dimension of ScaleA must be aligned up to a multiple of 16, and its K dimension size is K/32. The K dimension size of ScaleB is K/32, and its N dimension must be aligned up to a multiple of 16. The corresponding start addresses and occupied space must not exceed the MX Buffer range.
  - When M is 1 and `disable_gemv` is false, GEMV mode is enabled by default. In this case, cube A is read from the L0A Buffer in ND format, so cube A must be laid out in ND format, and its start address must still meet the alignment requirements in [Table 3](#asc_mmad_mx_param_table).

- Sync constraint description:

  For the scenario where the input matrices are computed in blocks along the K axis and the results are accumulated into the same L0C Buffer, when `(m / 16) * (n / 16) < 10`, call [asc_sync_pipe](../sync/intra_core_sync/asc_sync_pipe.md) between two adjacent cube multiply-accumulate instructions and set the `pipe` parameter to `PIPE_M`.

- UnitFlag constraint description:

  - UnitFlag is enabled for both the cube multiply-accumulate instruction and the corresponding cube move-out instruction. When you want the same L0C Buffer memory space to be continuously operated on only by multiple cube multiply-accumulate instructions or multiple cube move-out instructions, all instructions except the last one must set `unit_flag_mode` to `asc_unit_flag_mode::ENABLE_KEEP` to keep the operated memory space continuously occupied, and the last instruction must set it to `asc_unit_flag_mode::ENABLE_UPDATE` to release the occupied state.
  - When UnitFlag is enabled, the cube computation direction must be consistent with the read order of cube move-out. When the cube move-out instruction enables Nz2ND in-path format conversion, or when in-path format conversion is not performed but B8/B4 quantization is enabled and the Channel Merge function is triggered, call [asc_set_mmad_direction_n](asc_set_mmad_direction_n.md); in other scenarios, call [asc_set_mmad_direction_m](asc_set_mmad_direction_m.md).
  - When UnitFlag is enabled, it is recommended that the computation data size of cube multiply-accumulate be consistent with the data size of cube move-out. Inconsistency between the two may cause execution exceptions. To clear the residual state generated by UnitFlag, call [asc_set_l0c_copy_config](../cube_datamove/asc_set_l0c_copy_config.md), set `enable_unit_flag` to true, set the unit flag bits of all memory blocks in the L0C Buffer to 0, and disable UnitFlag.

- Special value/boundary value constraint description:

  When the floating-point input or output contains inf/nan, you can configure the CTRL\[48\] bit of the CTRL register through the [asc_set_ctrl](../spr/asc_set_ctrl.md) API to control the computation mode:

  - When set to 0, saturation mode is used, where inf output is saturated to ±MAX and nan output is saturated to 0.
  - When set to 1, non-saturation mode is used, where inf/nan keeps the original output.

  Note that nan input should be avoided; otherwise, an execution error may occur.

  For boundary value computation results, see [MmadMx API Boundary Value Summary](../../../appendix/mmad_mx_interface_boundary_value_summary.md).

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

```cpp
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t M = 40;
constexpr uint32_t K = 70;
constexpr uint32_t N = 50;
constexpr uint32_t SCALE_K = 4;
constexpr uint32_t M_ALIGN = 48;
constexpr uint32_t K_ALIGN = 128;
constexpr uint32_t N_ALIGN = 64;
constexpr uint32_t CUBE_M = 16;
constexpr uint32_t CUBE_K = 32;
constexpr uint32_t GM2L1_K_ALIGN = 96;
constexpr uint8_t FP8_E4M3_ONE = 0x38;
constexpr uint8_t FP8_E4M3_NEG_ONE = 0xb8;
constexpr uint8_t FP8_E5M2_ONE = 0x3c;
constexpr uint8_t FP8_E5M2_NEG_ONE = 0xbc;

__global__ __cube__ void asc_mmad_mx_kernel(__gm__ uint8_t* a, __gm__ uint8_t* scale_a,
    __gm__ uint8_t* b, __gm__ uint8_t* scale_b, __gm__ float* output)
{
    asc_init();
    __cbuf__ fp8_e4m3fn_t a_l1[M_ALIGN * K_ALIGN];
    __cbuf__ fp8_e5m2_t b_l1[N_ALIGN * K_ALIGN];
    __cbuf__ fp8_e8m0_t scale_a_l1[M_ALIGN * SCALE_K], scale_b_l1[N_ALIGN * SCALE_K];
    __ca__ fp8_e4m3fn_t a_l0[M_ALIGN * K_ALIGN];
    __cb__ fp8_e5m2_t b_l0[N_ALIGN * K_ALIGN];
    __cc__ float c_l0[M_ALIGN * N_ALIGN];

    asc_set_gm2l1_nz_para(1, 1, M_ALIGN, 0);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ fp8_e4m3fn_t*>(a), K,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, M, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, SCALE_K / 2, 0);
    asc_copy_gm2l1_dn2nz(reinterpret_cast<__cbuf__ half*>(scale_a_l1),
        reinterpret_cast<__gm__ half*>(scale_a), SCALE_K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM,
        SCALE_K / 2, M, 0, false);
    asc_set_gm2l1_nz_para(1, 1, N_ALIGN, 0);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ fp8_e5m2_t*>(b), K,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, N, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, SCALE_K / 2, 0);
    asc_copy_gm2l1_dn2nz(reinterpret_cast<__cbuf__ half*>(scale_b_l1),
        reinterpret_cast<__gm__ half*>(scale_b), SCALE_K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM,
        SCALE_K / 2, N, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

    // ND2NZ move has aligned K to 96. Clear the [96, 128) region to zero.
    asc_fill_value_config fill_config;
    fill_config.repeat = M_ALIGN;
    fill_config.blk_num = 1;
    fill_config.dst_gap = 0;
    asc_fill_l1(reinterpret_cast<__cbuf__ uint16_t*>(a_l1) + M_ALIGN * GM2L1_K_ALIGN / 2, 0U, fill_config);
    fill_config.repeat = N_ALIGN;
    asc_fill_l1(reinterpret_cast<__cbuf__ uint16_t*>(b_l1) + N_ALIGN * GM2L1_K_ALIGN / 2, 0U, fill_config);
    asc_sync_pipe(PIPE_MTE1);

    asc_copy_l12l0a(a_l0, a_l1, 0, 0, M_ALIGN / CUBE_M, K_ALIGN / CUBE_K,
        M_ALIGN / CUBE_M, M_ALIGN / CUBE_M);
    asc_copy_l12l0a_mx(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(a_l0)) / 16,
        scale_a_l1, 0, 0, M_ALIGN / CUBE_M, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);
    asc_copy_l12l0b(b_l0, b_l1, 0, 0, N_ALIGN / CUBE_M, K_ALIGN / CUBE_K,
        N_ALIGN / CUBE_M, N_ALIGN / CUBE_M);
    asc_copy_l12l0b_mx(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(b_l0)) / 16,
        scale_b_l1, 0, 0, N_ALIGN / CUBE_M, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad_mx(c_l0, a_l0, b_l0, M, K_ALIGN, N, asc_unit_flag_mode::DISABLE, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M_ALIGN, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
        asc_unit_flag_mode::DISABLE, asc_quant_mode::NoQuant, asc_relu_pre_mode::NONE, false, true, false, false);
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
    std::vector<uint8_t> a(M * K), b(N * K), scale_a(M * SCALE_K, 127), scale_b(N * SCALE_K, 127);
    std::vector<float> output(M * N), golden(M * N);
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t k = 0; k < K; ++k) {
            a[row * K + k] = (row + 2 * k) % 5 < 2 ? FP8_E4M3_ONE : FP8_E4M3_NEG_ONE;
        }
    }
    for (uint32_t col = 0; col < N; ++col) {
        for (uint32_t k = 0; k < K; ++k) {
            b[col * K + k] = (3 * col + k) % 7 < 3 ? FP8_E5M2_ONE : FP8_E5M2_NEG_ONE;
        }
        for (uint32_t block = 0; block < SCALE_K; ++block) {
            scale_b[col * SCALE_K + block] = static_cast<uint8_t>(126 + (col + block) % 3);
        }
    }
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t col = 0; col < N; ++col) {
            for (uint32_t k = 0; k < K; ++k) {
                const float a_value = a[row * K + k] == FP8_E4M3_ONE ? 1.0f : -1.0f;
                const float b_value = b[col * K + k] == FP8_E5M2_ONE ? 1.0f : -1.0f;
                const float b_scale = std::ldexp(1.0f,
                    static_cast<int32_t>(scale_b[col * SCALE_K + k / CUBE_K]) - 127);
                golden[row * N + col] += a_value * b_value * b_scale;
            }
        }
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *a_device = nullptr, *b_device = nullptr, *scale_a_device = nullptr, *scale_b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&scale_a_device), scale_a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&scale_b_device), scale_b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size(), a.data(), a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size(), b.data(), b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(scale_a_device, scale_a.size(), scale_a.data(), scale_a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(scale_b_device, scale_b.size(), scale_b.data(), scale_b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_mmad_mx_kernel<<<1, 0>>>(a_device, scale_a_device, b_device, scale_b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_row("MX output row 0", output);
    print_row("Golden row 0", golden);
    std::cout << "Scale exponent bytes: " << +scale_a[0] << ' ' << +scale_b[0] << std::endl;
    bool passed = true;
    for (uint32_t i = 0; i < output.size(); ++i) {
        if (std::fabs(output[i] - golden[i]) >= 0.01f) {
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_mmad_mx applied MX scales."
                         : "[Failed] asc_mmad_mx result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(scale_a_device); aclrtFree(scale_b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```

For more scenario examples, see [asc_mmad_mx examples](../../../../../../examples/02_simd_c_api/03_c_api/03_matrix_compute/mmad_mx).
<!-- end id8 -->
