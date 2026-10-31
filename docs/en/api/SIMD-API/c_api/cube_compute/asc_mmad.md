# asc_mmad

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T03:17:32.632Z pushedAt=2026-10-28T12:21:43.310Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
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

This API is the core cube multiply-accumulate (Mmad) computation API of Ascend C for Ascend AI processors. It is designed for high-performance operator development and encapsulates the cube multiply-accumulate computation capability of Ascend NPU hardware. It is widely used in the development of neural network layers (such as fully connected layers and convolution layers) and numerical computation operators. Its computation formula is as follows:

$$
C_{M \times N} = A_{M \times K} \times B_{K \times N} + C_{M \times N}
$$

Here, A, B, and C are the left cube, right cube, and result cube, respectively. By configuring the parameters of this API, the C cube can be initialized to an all-zero cube, a cube in the L0C Buffer, or a Bias cube. The information about each cube is described in the following table:

<!-- npu="950" id8 -->
**Table** Cube information description ([NPU architecture version 3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

| Cube | Storage Location | Shape (Rows × Columns) | Data Format | Fractal Size (Rows × Columns) |
| --- | --- | --- | --- | --- |
| A | L0A Buffer | M×K | Nz | 16×K0 |
| B | L0B Buffer | K×N | Zn | K0×16 |
| C | L0C Buffer | M×N | Nz | 16×16 |
| Bias (used to initialize the C cube) | BiasTable Buffer | 1×N, where M rows are broadcast and copied to initialize the C cube during use | ND | - |

In the table, the value of K0 is `32B / sizeof(dtype)`, where `dtype` is the data type of the cube.
<!-- end id8 -->

<!-- npu="A3,910b" id9 -->
**Table** Cube information description ([NPU architecture version 2201](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

| Cube | Storage Location | Shape | Data Format | Fractal Size |
| --- | --- | --- | --- | --- |
| A | L0A Buffer | M×K | Zz | 16×K0 |
| B | L0B Buffer | K×N | Zn | K0×16 |
| C | L0C Buffer | M×N | Nz | 16×16 |
| Bias (used to initialize the C cube) | BiasTable Buffer | 1×N, where M rows are broadcast and copied to initialize the C cube during use | ND | - |

In the table, the value of K0 is `32B / sizeof(dtype)`, where `dtype` is the data type of the cube. In particular, when the data type is `int4b_t`, `K0 = 64`.
<!-- end id9 -->

This API is a cube computation API and takes effect only on the AIC.

## Prototype

<!-- npu="950" id12 -->
### Prototype ([NPU architecture version 3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

**Placeholder form:**

```c
// Do not explicitly pass the Bias start address. Control the C cube initialization mode through parameters.
__aicore__ inline void asc_mmad(__cc__ <c_dtype>* c_matrix,
                                __ca__ <a_dtype>* a_matrix,
                                __cb__ <b_dtype>* b_matrix,
                                uint16_t m,
                                uint16_t k,
                                uint16_t n,
                                asc_unit_flag_mode unit_flag_mode,
                                bool disable_gemv,
                                bool c_matrix_source,
                                bool c_matrix_init_val)

// Explicitly pass the Bias start address. Initialize the C cube using the Bias cube.
__aicore__ inline void asc_mmad(__cc__ <c_dtype>* c_matrix,
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

The value combinations of `c_dtype`, `a_dtype`, and `b_dtype` are as follows:

**Table** Supported data type combinations ([NPU architecture version 3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))<a id="asc_mmad_data_type"></a>

| a_dtype | b_dtype | c_dtype | Bias Data Type |
| --- | --- | --- | --- |
| `int8_t` | `int8_t` | `int32_t` | `int32_t` |
| `hifloat8_t` | `hifloat8_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e5m2_t` | `float` | `float` |
| `fp8_e5m2_t` | `fp8_e4m3fn_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e5m2_t` | `float` | `float` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` | `float` | `float` |
| `half` | `half` | `float` | `float` |
| `bfloat16_t` | `bfloat16_t` | `float` | `float` |
| `float` | `float` | `float` | `float` |

**Typical example:**

```c
// Do not explicitly pass the Bias start address.
__aicore__ inline void asc_mmad(__cc__ float* c_matrix,
                                __ca__ bfloat16_t* a_matrix,
                                __cb__ bfloat16_t* b_matrix,
                                uint16_t m,
                                uint16_t k,
                                uint16_t n,
                                asc_unit_flag_mode unit_flag_mode,
                                bool disable_gemv,
                                bool c_matrix_source,
                                bool c_matrix_init_val)

// Explicitly pass the Bias start address.
__aicore__ inline void asc_mmad(__cc__ float* c_matrix,
                                __ca__ bfloat16_t* a_matrix,
                                __cb__ bfloat16_t* b_matrix,
                                uint64_t bias,
                                uint16_t m,
                                uint16_t k,
                                uint16_t n,
                                asc_unit_flag_mode unit_flag_mode,
                                bool disable_gemv)
```
<!-- end id12 -->

<!-- npu="A3,910b" id13 -->
### Prototype ([NPU architecture version 2201](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

**Placeholder form:**

- asc_mmad API:

  ```c
    __aicore__ inline void asc_mmad(__cc__ <c_dtype>* c_matrix, __ca__ <a_dtype>* a_matrix, __cb__ <b_dtype>* b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  ```

  The value combinations of `c_dtype`, `a_dtype`, and `b_dtype` are listed in the following table:

  **Table** Supported data type combinations ([NPU architecture version 2201](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

  | a_dtype | b_dtype | c_dtype | Bias Data Type |
  | --- | --- | --- | --- |
  | `int8_t` | `int8_t` | `int32_t` | `int32_t` |
  | `half` | `half` | `float` | `float` |
  | `bfloat16_t` | `bfloat16_t` | `float` | `float` |
  | `float` | `float` | `float` | `float` |

- asc_mmad_s4 API, called when the input cube data type is `int4b_t`:

  ```c
__aicore__ inline void asc_mmad_s4(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t m, uint16_t k, uint16_t n, asc_unit_flag_mode unit_flag_mode, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val)
  ```
<!-- end id13 -->

## Parameters

<!-- npu="950" id14 -->
**Table** Parameters ([NPU architecture version 3510](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))<a id="asc_mmad_param_table"></a>

| Parameter | Input/Output | Description |
| --- | --- | --- |
| c_matrix | Output | Destination operand, the start address of result cube C in the L0C Buffer, which must be 1024-byte aligned. The data type is determined by the API overload. For details, see [Supported data type combinations](#asc_mmad_data_type). |
| a_matrix | Input | Source operand, the start address of left cube A in the L0A Buffer, which must be 512-byte aligned. The data type is determined by the API overload. For details, see [Supported data type combinations](#asc_mmad_data_type). |
| b_matrix | Input | Source operand, the start address of right cube B in the L0B Buffer, which must be 512-byte aligned. The data type is determined by the API overload. For details, see [Supported data type combinations](#asc_mmad_data_type). |
| bias | Input | Start address of the Bias cube in the BiasTable Buffer, which must be 64-byte aligned and can be a non-zero address. This parameter is included only in APIs that explicitly pass the Bias start address.<br>The data type of Bias must be consistent with that of cube C. Before calling this API, move the Bias data to this address through the [asc_copy_l12bt](../cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_3510.md) API. The Bias occupies a length of `n * sizeof(c_dtype)` rounded up to 64 bytes. |
| m | Input | M dimension size of left cube A and result cube C, in elements, with a value range of [0, 4095]. |
| k | Input | K dimension size of left cube A and right cube B, in elements, with a value range of [0, 4095]. |
| n | Input | N dimension size of right cube B and result cube C, in elements, with a value range of [0, 4095]. |
| unit_flag_mode | Input | Controls fine-grained parallelism between the cube multiply-accumulate instruction and the cube move-out instruction. When UnitFlag is enabled, the hardware moves out the computation result each time it finishes computing one fractal. The value description is as follows:<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`: UnitFlag is not enabled.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`: UnitFlag is enabled, and the hardware does not change the unit flag bit after executing the instruction.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`: UnitFlag is enabled, and the hardware changes the unit flag bit after executing the instruction.<br>UnitFlag is enabled or disabled for both the cube multiply-accumulate instruction and the corresponding cube move-out instruction. When UnitFlag is enabled, no synchronization instruction needs to be inserted between the instructions. |
| disable_gemv | Input | When M is 1, configures whether to disable GEMV mode.<br>&nbsp;&nbsp;&bull; false: GEMV mode is enabled.<br>&nbsp;&nbsp;&bull; true: GEMV mode is disabled.<br>When M is not 1, this parameter does not take effect. |
| c_matrix_source | Input | Included only in overloads that do not explicitly pass the Bias start address. When `c_matrix_init_val` is false, configures the source of the initial value of cube C.<br>&nbsp;&nbsp;&bull; false: The initial value of cube C comes from the L0C Buffer.<br>&nbsp;&nbsp;&bull; true: The initial value of cube C comes from the BiasTable Buffer and is read starting from address 0. |
| c_matrix_init_val | Input | Included only in overloads that do not explicitly pass the Bias start address. Configures whether to set the initial value of cube C to 0.<br>&nbsp;&nbsp;&bull; true: The initial value of cube C is set to 0, and `c_matrix_source` does not take effect.<br>&nbsp;&nbsp;&bull; false: No zeroing is performed, and the initial value of cube C is configured by `c_matrix_source`. |

<!-- end id14 -->

<!-- npu="A3,910b" id15 -->
**Table** Parameters ([NPU architecture version 2201](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md))

| Parameter | Input/Output | Description |
| --- | --- | --- |
| c_matrix | Output | Destination operand, the start address of result cube C in the L0C Buffer, which must be 1024-byte aligned. |
| a_matrix | Input | Source operand, the start address of left cube A in the L0A Buffer, which must be 512-byte aligned. |
| b_matrix | Input | Source operand, the start address of right cube B in the L0B Buffer, which must be 512-byte aligned. |
| m | Input | M dimension size of left cube A and result cube C, in elements, with a value range of [0, 4095]. |
| k | Input | K dimension size of left cube A and right cube B, in elements, with a value range of [0, 4095]. |
| n | Input | N dimension size of right cube B and result cube C, in elements, with a value range of [0, 4095]. |
| unit_flag_mode | Input | Controls fine-grained parallelism between the cube multiply-accumulate instruction and the cube move-out instruction. When UnitFlag is enabled, the hardware moves out the computation result each time it finishes computing a fractal. Value description:<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::DISABLE`: UnitFlag is disabled.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_KEEP`: UnitFlag is enabled, and the hardware does not change the unit flag bit after executing the instruction.<br>&nbsp;&nbsp;&bull; `asc_unit_flag_mode::ENABLE_UPDATE`: UnitFlag is enabled, and the hardware changes the unit flag bit after executing the instruction.<br>UnitFlag is enabled or disabled for both the cube multiply-accumulate instruction and the corresponding cube move-out instruction. When UnitFlag is enabled, no synchronization instruction needs to be inserted between them. |
| k_direction_align | Input | The core function of K direction alignment is to control, through the `k_direction_align` parameter, the alignment of the L0A Buffer and L0B Buffer matrices in the K direction when the float data type is used.<br>Value description:<br>&nbsp;&nbsp;&bull; false: The K direction is aligned to `ceil(k / 8) * 8`.<br>&nbsp;&nbsp;&bull; true: The K direction is aligned to `ceil(k / 16) * 16`. |
| c_matrix_source | Input | When the `c_matrix_init_val` parameter is false, configures the source of the initial value of cube C.<br>&nbsp;&nbsp;&bull; false: The initial value of cube C comes from the L0C Buffer.<br>&nbsp;&nbsp;&bull; true: The initial value of cube C comes from the BiasTable Buffer. |
| c_matrix_init_val | Input | Configures whether to set the initial value of cube C to 0.<br>&nbsp;&nbsp;&bull; true: The initial value of cube C is set to 0, and the `c_matrix_source` parameter does not take effect.<br>&nbsp;&nbsp;&bull; false: No zeroing operation is performed, and the initial value of cube C is configured by the `c_matrix_source` parameter. |

<!-- end id15 -->

## Return Value

None

## Pipeline Type

PIPE_M

## Constraints

- This API takes effect only on the AIC. Calling it on the AIV returns immediately.
- If any of `m`, `k`, and `n` is 0, the API is treated as a NOP (no operation).

- Memory usage constraints:
  <!-- npu="950" id10 -->
  - For Ascend 950PR&950DT products:

      - The L0C Buffer size is 256 KB, and both the L0A Buffer and L0B Buffer sizes are 64 KB. The BiasTable Buffer size is 4 KB. The start address and occupied space of a cube must not exceed the range of the corresponding buffer.
      - The start address of each cube must meet the alignment requirements in [Parameters](#asc_mmad_param_table). For other address constraints of operands, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
      - When applying for cube storage space, use the values padded to the fractal size: M and N are each padded up to a multiple of 16, and K is padded up to a multiple of K0, where K0 is `32B / sizeof(dtype)` and `dtype` is the data type of the cube. `m`, `k`, and `n` still pass the valid M, K, and N values of the cube. The padded part is invalid data and does not participate in the computation of the valid region of the result cube.
      - When M is 1 and `disable_gemv` is false, GEMV mode is enabled. In this case, cube A is read from the L0A Buffer in ND format, cube A must be arranged in ND format, and its start address must still be 512-byte aligned.
  <!-- end id10 -->

  <!-- npu="A3,910b" id11 -->
  - For the following product models:

 	  <!-- npu="A3" id16 -->
    Atlas A3 products
 	  <!-- end id16 -->
 	  <!-- npu="910b" id17 -->
 	  Atlas A2 products
 	  <!-- end id17 -->
 	  The L0C Buffer size is 128 KB, and both the L0A Buffer and L0B Buffer sizes are 64 KB. The BiasTable Buffer size is 1 KB. The start address and occupied space of the cube must not exceed the range of the corresponding buffer.
 <!-- end id11 -->

- Synchronization constraints:

  For the scenario where the input cube is computed in blocks along the K axis and the results are accumulated into the same L0C Buffer, when `(m / 16) * (n / 16) < 10`, call [asc_sync_pipe](../sync/intra_core_sync/asc_sync_pipe.md) between two adjacent cube multiply-accumulate instructions and set the input parameter `pipe` to `PIPE_M`.

- UnitFlag constraints:

  - UnitFlag is enabled for both the cube multiply-accumulate instruction and the corresponding cube move-out instruction. When you want the same L0C Buffer memory space to be continuously operated only by multiple cube multiply-accumulate instructions or multiple cube move-out instructions, set `unit_flag_mode` to `asc_unit_flag_mode::ENABLE_KEEP` for all instructions except the last one to keep the operated memory space continuously occupied, and set the last instruction to `asc_unit_flag_mode::ENABLE_UPDATE` to release the occupied state.
  - When UnitFlag is enabled, the cube computation direction must be consistent with the cube move-out read order. When the cube move-out instruction enables Nz2ND in-path format conversion, or when no in-path format conversion is performed but B8/B4 quantization is enabled and the Channel Merge function is triggered, call [asc_set_mmad_direction_n](asc_set_mmad_direction_n.md); in other scenarios, call [asc_set_mmad_direction_m](asc_set_mmad_direction_m.md).
  - When UnitFlag is enabled, it is recommended that the computation data size of the cube multiply-accumulate be consistent with the data size of the cube move-out. Inconsistency between the two may cause execution exceptions. To clear the residual state generated by UnitFlag, call [asc_set_l0c_copy_config](../cube_datamove/asc_set_l0c_copy_config.md), set `enable_unit_flag` to true, set the unit flag bits of all memory blocks in the L0C Buffer to 0, and disable UnitFlag.

- Special value/boundary value constraints:

  When the floating-point input or output contains inf/nan, you can configure the CTRL\[48\] bit of the CTRL register through the [asc_set_ctrl](../spr/asc_set_ctrl.md) API to control the computation mode:

  - When set to 0, saturation mode is used: inf output saturates to ±MAX, and nan output saturates to 0.
  - When set to 1, non-saturation mode is used: inf/nan are output as-is.

  Note that nan input should be avoided, as it may cause execution errors. Integer types support only saturation mode.

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id18 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

The following example code is supported only by the Ascend 950PR&950DT products.

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t M = 48;
constexpr uint32_t K = 96;
constexpr uint32_t N = 80;
constexpr uint32_t CUBE_M = 16;
constexpr uint32_t CUBE_K = 32;

__global__ __cube__ void asc_mmad_kernel(__gm__ int8_t* a, __gm__ int8_t* b, __gm__ int32_t* output)
{
    asc_init();
    __cbuf__ int8_t a_l1[M * K];
    __cbuf__ int8_t b_l1[N * K];
    __ca__ int8_t a_l0[M * K];
    __cb__ int8_t b_l0[N * K];
    __cc__ int32_t c_l0[M * N];

    asc_set_gm2l1_nz_para(1, 1, M, 0);
    asc_copy_gm2l1_nd2nz(
        a_l1, a, K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, M, K, 0, false);
    asc_set_gm2l1_nz_para(1, 1, N, 0);
    asc_copy_gm2l1_nd2nz(
        b_l1, b, K, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, N, K, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, M / CUBE_M, K / CUBE_K, M / CUBE_M, M / CUBE_M);
    asc_copy_l12l0b(b_l0, b_l1, 0, 0, N / CUBE_M, K / CUBE_K, N / CUBE_M, N / CUBE_M);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, asc_unit_flag_mode::DISABLE, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M, asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
        asc_unit_flag_mode::DISABLE, asc_quant_mode::NoQuant, asc_relu_pre_mode::NONE, false, true, false, false);
    asc_sync_pipe(PIPE_ALL);
}

template <typename T>
void print_row(const char* label, const std::vector<T>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << +data[i];
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    // B is stored on GM as N x K. After being moved into L0B, it forms the K x N cube required by MMAD.
    std::vector<int8_t> a(M * K), b(N * K);
    std::vector<int32_t> output(M * N), golden(M * N);
    for (uint32_t row = 0; row < M; ++row) {
        for (uint32_t k = 0; k < K; ++k) {
            a[row * K + k] = static_cast<int8_t>(static_cast<int32_t>((row + 2 * k) % 5) - 2);
        }
    }
    for (uint32_t col = 0; col < N; ++col) {
        for (uint32_t k = 0; k < K; ++k) {
            b[col * K + k] = static_cast<int8_t>(static_cast<int32_t>((3 * col + k) % 7) - 3);
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
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(int32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size(), a.data(), a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size(), b.data(), b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    asc_mmad_kernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int32_t), output_device, output.size() * sizeof(int32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_row("Input A row 0", a);
    print_row("Input B row 0", b);
    print_row("Output row 0", output);
    print_row("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_mmad passed." : "[Failed] asc_mmad failed.") << std::endl;
    aclrtFree(a_device);
    aclrtFree(b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```

For more usage examples, see [asc_mmad sample](../../../../../../examples/02_simd_c_api/03_c_api/03_matrix_compute/mmad).
<!-- end id18 -->
