# asc_set_gm2l1_pad

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T13:22:19.954Z pushedAt=2026-10-28T12:45:45.468Z nl=2 -->

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

Sets the constant fill value for the [asc_copy_gm2l1_align](asc_copy_gm2l1_align.md) API. When `asc_copy_gm2l1_align` is called, the value set by this API is used in the following scenarios:

- When `enable_data_select` is set to `true` and both `left_padding_count` and `right_padding_count` are `0`, the fill data in Normal fill mode or Compact fill mode uses the value set by this API.
- When `left_padding_count` or `right_padding_count` is not `0`, the left/right fill mode is entered. In this case, the hardware forcibly uses the value set by this API, and the `enable_data_select` setting is invalid.

When `enable_data_select` is set to `false` and the left/right fill mode is not enabled, the fill data takes the first element of each consecutive data block, and the value set by this API does not take effect.

This API takes effect only on the AIC.

## Prototype

```cpp
__aicore__ inline void asc_set_gm2l1_pad(uint32_t pad_val)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| pad_val | Input | Constant fill value. The API parameter is `uint32_t`, and `asc_copy_gm2l1_align` uses the corresponding low bits as the value of a single fill element based on `dtype`. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- This API returns directly when it is not called by the AIC.
- Call this API to configure the fill value before calling [asc_copy_gm2l1_align](asc_copy_gm2l1_align.md).

## Examples

For complete examples of Normal fill, Compact fill, and left/right fill scenarios, see [asc_copy_gm2l1_align Examples](asc_copy_gm2l1_align.md#examples).

After saving the code as `examples.asc`, you can compile it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510
```
<!-- end id8 -->

The following example moves three 48B `uint8_t` data blocks and uses the constant `0xA5` to fill the trailing 16B of each data block, so that each data block is written to the L1 Buffer with a 64B stride.

```cpp
#include <cstdint>
#include "c_api/asc_simd.h"

// This kernel function performs GM-to-L1 data movement on the AIC.
__global__ __cube__ void Gm2L1AlignPadExample(__gm__ uint8_t* src)
{
    asc_init();
    __cbuf__ uint8_t dst[192];

    uint8_t PAD_VALUE = 0xA5;
    constexpr uint32_t N_BURST = 3;
    constexpr uint32_t LEN_BURST = 48;
    constexpr uint32_t DST_STRIDE = 64;
    constexpr asc_load_l2_cache_mode L2_CACHE_MODE = asc_load_l2_cache_mode::NOTALLOC_KEEP;
    // Configure the constant fill value. The uint8_t fill byte is 0xA5, and the actual argument is promoted to the uint32_t type required by the API.
    asc_set_gm2l1_pad(*reinterpret_cast<uint8_t*>(&PAD_VALUE));
    // enable_data_select=true. In Normal mode, the trailing 16B of each 48B data block is filled with the constant value.
    asc_copy_gm2l1_align(dst, src, N_BURST, LEN_BURST, 0, 0, true, L2_CACHE_MODE, LEN_BURST, DST_STRIDE);
}
```
