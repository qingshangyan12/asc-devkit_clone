# asc_fill_l1_sync (Deprecated)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T01:42:05.136Z pushedAt=2026-10-28T12:55:18.895Z nl=2 -->

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

The header file path is `"c_api/composite/cube_datamove_composite.h"`.

**The `asc_fill_l1_sync` API is deprecated. Use the [corresponding asynchronous API](../cube_datamove/asc_fill_l1.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Writes a scalar fill value to the destination address in the L1 Buffer and broadcasts the fill value to multiple 32-byte fractal blocks. It can be used to initialize the L1 Buffer memory. The repeat count, the number of fractal blocks per repetition, and the gap between adjacent repetitions must be configured through [asc_fill_value_config](../defs/union/asc_fill_value_config.md).

This API takes effect only when executed on the AIC.

## Prototype

```cpp
__aicore__ inline void asc_fill_l1_sync(__cbuf__ <dst_dtype>* dst,
                                        <value_dtype> value,
                                        const asc_fill_value_config& config)
```

### Data Types Supported by dtype

The following combinations of dst dtype and value dtype are supported:

- When `dst_dtype` is `int16_t`, `uint16_t`, `half`, `int32_t`, `uint32_t`, or `float`, `value_dtype` supports `half` and `uint32_t`.
- When `dst_dtype` is `bfloat16_t`, `value_dtype` supports `half`, `bfloat16_t`, and `uint32_t`.

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | meaning |
|---|---|---|
| dst | Output | Destination operand, the start address of the destination matrix. The storage location is the L1 Buffer, and the start address must be 32-byte aligned. |
| value | Input | Source operand, the scalar fill value. |
| config | Input | Fill configuration structure [asc_fill_value_config](../defs/union/asc_fill_value_config.md). See [Table 2](#fill_value_config_fields) for the structure fields. |

**Table 2** config field description <a id="fill_value_config_fields"></a>

| field name | meaning |
|---|---|
| repeat | Repeat count, that is, the total number of times the fill value is written to the L1 Buffer in `blk_num` 32-byte blocks and then repeatedly written at intervals of `dst_gap`. Value range: [1, 32767]. |
| blk_num | Number of 32-byte blocks written in a single repetition, that is, the number of 32-byte blocks filled consecutively within one repetition. Value range: [1, 32767]. |
| dst_gap | Gap between two adjacent repetitions, in 32-byte blocks. The start address offset between two adjacent fills is $(\mathrm{blk}_{\mathrm{num}} + \mathrm{dst}_{\mathrm{gap}}) \times 32$ bytes. Value range: [0, 32767]. |

As shown in [Figure 1](#fill_l1_layout), `value = 0` means the fill value is 0, `repeat = 5` means five fills are performed, `blk_num = 3` means three 32-byte blocks are filled consecutively each time, and `dst_gap = 5` means the interval between two adjacent fills is five 32-byte blocks.

**Figure 1** Fill layout in L1 Buffer<a id="fill_l1_layout"></a>
![Fill data layout in L1 Buffer](../../../figures/Fill_L1.png "Fill layout in L1 Buffer")

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

- This API returns directly when it is not called by the AIC.
- The start address of `dst` must be 32-byte aligned (L1 Buffer alignment requirement). Otherwise, an address alignment exception is triggered.
- The total capacity of L1 Buffer is 512 KB. The sum of the `dst` offset and the actual occupied space must not exceed the L1 Buffer capacity. Otherwise, an address overflow exception is triggered.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data. When `asc_fill_l1_sync` is called, synchronization is completed inside the API.
- The data type of `value` and the data type of `dst` must satisfy the combination relationship in the Prototype section. When the two data types differ, convert the value of `value` according to the data type of `dst`. For details, see Examples.

## Examples

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id9 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id9 -->

The sample occupies 64 KB of L1 Buffer, 32 KB of L0A Buffer, 32 KB of L0B Buffer, and 64 KB of L0C Buffer on a single core. The sample reuses `EVENT_ID0` in sequence to complete synchronization from MTE2 to MTE1, MTE1 to M, and M to FIX.

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
constexpr uint32_t CUBE_BLOCK = 16;
constexpr uint32_t BLOCK_BYTES = 32;

__global__ __cube__ void AscFillL1Kernel(__gm__ float* output)
{
    asc_init();
    __cbuf__ half a_l1[M * K], b_l1[N * K];
    __ca__ half a_l0[M * K];
    __cb__ half b_l0[K * N];
    __cc__ float c_l0[ELEMENTS];

    // Fill matrices A and B in L1 with 1 using two value types.
    asc_fill_value_config config;
    config.repeat = M;
    config.blk_num = K * sizeof(half) / BLOCK_BYTES;
    config.dst_gap = 0;
    asc_fill_l1_sync(a_l1, static_cast<half>(1.0f), config);

    config.repeat = N;
    // 0x3C00 is 1.0 of the half type, with two such values packed in uint32_t.
    asc_fill_l1_sync(b_l1, 0x3C003C00U, config);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

    // Move the filled matrix into L0A and L0B.
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, M / CUBE_BLOCK, K / CUBE_BLOCK,
        M / CUBE_BLOCK, M / CUBE_BLOCK);
    asc_copy_l12l0b(b_l0, b_l1, 0, 0, N / CUBE_BLOCK, K / CUBE_BLOCK,
        N / CUBE_BLOCK, N / CUBE_BLOCK);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);

    // Compute the matrix multiplication. Both input matrices are 1, so each result element is K.
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, 0, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);

    asc_set_l0c_copy_nz_para(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_FIX);
}

void PrintRow(const char* label, const std::vector<float>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) {
        std::cout << ' ' << data[i];
    }
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<float> output(ELEMENTS);
    const std::vector<float> golden(ELEMENTS, static_cast<float>(K));

    // Allocate Device memory, launch the Kernel, and verify the result.
    aclInit(nullptr);
    aclrtSetDevice(0);
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float),
        ACL_MEM_MALLOC_HUGE_FIRST);
    AscFillL1Kernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device,
        output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);

    PrintRow("Output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_fill_l1 result is correct."
                         : "[Failed] asc_fill_l1 result mismatch.") << std::endl;

    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
