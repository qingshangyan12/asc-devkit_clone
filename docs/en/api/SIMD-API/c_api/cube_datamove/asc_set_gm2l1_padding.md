# asc_set_gm2l1_padding

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T13:23:20.288Z pushedAt=2026-10-28T12:45:45.473Z nl=2 -->

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

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Sets the padding value used in the padding mode during the data transfer performed by [asc_copy_gm2l1_arch_3510 (high-dimensional split data transfer)](asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md).

This API takes effect only on the AIC.

## Prototype

```c
__aicore__ inline void asc_set_gm2l1_padding(<dtype> padding_value)
__aicore__ inline void asc_set_gm2l1_padding(uint64_t config)
```

### Data Types Supported by dtype

The data types supported by dtype are `int8_t`, `uint8_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

### Prototype Example

```c
// Example: Configure the padding value of the half type.
__aicore__ inline void asc_set_gm2l1_padding(half padding_value)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| padding_value | Input | Padding value used during the data transfer. |
| config | Input | Configuration register for the padding value used during the data transfer.<br>&nbsp;&nbsp;&bull; If the padding value type is b32, the value represented by `[0:31]` is used as the padding value. If the padding value type is b16, the value represented by `[0:15]` is used as the padding value. If the padding value type is b8, `[15:8]` and `[7:0]` must have the same value, and the value represented by `[7:0]` is used as the padding value. If the padding value type is b4, `[15:12]`, `[11:8]`, `[7:4]`, and `[3:0]` must have the same value, and the value represented by `[3:0]` is used as the padding value.<br>&nbsp;&nbsp;&bull; Set the unused bits `[32:63]` to `0`. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- This API returns directly when it is not called by the AIC.
- The register used by this API at the underlying layer is the same as that used by [asc_set_l12l0a_3d_padding](asc_set_l12l0a_3d_padding.md). Therefore, the value set here is also the padding value used during the data transfer performed by [asc_copy_l12l0a_arch_3510 (3D cube transfer)](asc_copy_l12l0a/asc_copy_l12l0a_3d_arch_3510.md).

## Examples

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

The following example transfers 16B input data to the L1 Buffer with `pad_func_mode=5`. It calls `asc_set_gm2l1_padding` to set the padding value to `0xA5`, then reads back the complete 32B L1 data block through L1→UB→GM, and verifies byte by byte on the host side that the first 16B is the input and the last 16B is the padding value.

```cpp
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#include "acl/acl.h"
#include "c_api/asc_simd.h"

namespace {
constexpr uint32_t INPUT_BYTES = 16;
constexpr uint32_t OUTPUT_BYTES = 32;
constexpr uint8_t PAD_VALUE = 0xA5;
constexpr int64_t CROSS_CORE_FLAG_ID = 0x8;
constexpr uint64_t AIV_TO_AIC_SYNC_ID = 0x1;
constexpr uint64_t AIC_TO_AIV_SYNC_ID = 0x2;

__global__ __mix__(1, 2) void asc_set_gm2l1_padding_kernel(  // __mix__: AIC configures and reads back L1, and AIV writes back to GM.
    __gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __cbuf__ uint8_t* l1 = (__cbuf__ uint8_t*)asc_get_phy_buf_addr(0);
    __ubuf__ uint8_t* staging_ub = (__ubuf__ uint8_t*)asc_get_phy_buf_addr(0);
    __ubuf__ uint8_t* result_ub = (__ubuf__ uint8_t*)asc_get_phy_buf_addr(OUTPUT_BYTES);

    if ASC_IS_AIV {
        const bool is_output_core = asc_get_sub_block_id() == 0;
        if (is_output_core) {
            // Initialize the physical UB on the AIV side before AIC returns the L1 result.
            asc_copy_gm2ub(staging_ub, input, INPUT_BYTES);
            asc_sync_notify(PIPE_MTE2, PIPE_MTE3, EVENT_ID0);
            asc_sync_wait(PIPE_MTE2, PIPE_MTE3, EVENT_ID0);
            asc_sync_intra_arrive(PIPE_MTE3, AIV_TO_AIC_SYNC_ID);
        }
        asc_sync_block_arrive(PIPE_MTE3, CROSS_CORE_FLAG_ID);
        if (is_output_core) {
            asc_sync_intra_wait(PIPE_MTE3, AIC_TO_AIV_SYNC_ID);
            asc_copy_ub2gm(output, result_ub, OUTPUT_BYTES);
        }
    }

    if ASC_IS_AIC {
        asc_sync_intra_wait(PIPE_MTE1, AIV_TO_AIC_SYNC_ID);
        asc_sync_block_wait(PIPE_MTE1, CROSS_CORE_FLAG_ID);
        // EXPAND_16B groups by 16B valid input, and the padding value is configured by this API.
        asc_set_gm2l1_padding(PAD_VALUE);
        asc_copy_gm2l1(l1, input, 1, 1, asc_channel_pad_mode::EXPAND_16B, 0, 0);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12ub(result_ub, l1, 0, 1, OUTPUT_BYTES / 32, 0, 0);
        asc_sync_pipe(PIPE_MTE1);
        asc_sync_intra_arrive(PIPE_MTE1, AIC_TO_AIV_SYNC_ID);
    }
    asc_sync_pipe(PIPE_ALL);
}
} // namespace

int main()
{
    std::vector<uint8_t> input(INPUT_BYTES), output(OUTPUT_BYTES);
    for (uint32_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i + 1);
    uint8_t *input_device = nullptr, *output_device = nullptr;
    if (aclInit(nullptr) != ACL_SUCCESS || aclrtSetDevice(0) != ACL_SUCCESS ||
        aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST) != ACL_SUCCESS ||
        aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST) != ACL_SUCCESS) return 1;
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_set_gm2l1_padding_kernel<<<1, 0>>>(output_device, input_device);
    const bool runtime_ok = aclrtSynchronizeDevice() == ACL_SUCCESS &&
        aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST) == ACL_SUCCESS;
    std::vector<uint8_t> golden(OUTPUT_BYTES, PAD_VALUE);
    std::memcpy(golden.data(), input.data(), INPUT_BYTES);
    const bool passed = runtime_ok && output == golden;
    std::cout << "RESULT status=" << (passed ? "PASS" : "FAIL")
              << " input_bytes=" << INPUT_BYTES << " output_bytes=" << OUTPUT_BYTES
              << " pad_value=" << static_cast<uint32_t>(PAD_VALUE) << std::endl;
    aclrtFree(output_device);
    aclrtFree(input_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
