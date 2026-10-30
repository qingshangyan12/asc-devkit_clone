# asc_copy_l12bt

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T12:34:55.820Z pushedAt=2026-10-28T12:45:45.398Z nl=2 -->

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

Transfers bias data from the L1 Buffer to the BiasTable Buffer for the cube compute unit (Mmad) to read as bias input. Supports transferring source operands of the `half`, `bfloat16_t`, `int32_t`, and `float` types. During the transfer, `half` and `bfloat16_t` undergo in-line format conversion before being written to the BiasTable Buffer: `bfloat16_t` is automatically converted to `float`; `half` is converted to `float` when conversion is enabled, that is, when `conv_control` is set to 1, and when conversion is disabled, that is, when `conv_control` is set to 0, each `half` element is only expanded to 32 bits (the upper 16 bits are placeholder data).

This API provides two functional modes with completely different parameter lists. Select the corresponding overload as needed:

- **High-Dimensional Split Transfer**: Transfers continuous and non-continuous data by configuring transfer parameters such as the number of data blocks, the length of a single data block, and the source/destination stride.
- **Continuous Data Transfer**: Specifies only `size` and transfers data as a single continuous data block, with both source and destination strides set to 0.

This API takes effect only when executed on the AIC.

## Prototype

### High-Dimensional Split Transfer Mode

```c
// Placeholder form.
__aicore__ inline void asc_copy_l12bt(uint64_t dst,
                                      __cbuf__ <dtype>* src,
                                      uint16_t conv_control,
                                      uint16_t n_burst,
                                      uint16_t len_burst,
                                      uint16_t source_gap,
                                      uint16_t dst_gap)
```

#### Data Types Supported by dtype

The data types supported by dtype are `half`, `bfloat16_t`, `int32_t`, and `float`.

#### Prototype Example

```c
// Example: high-dimensional split transfer of the bfloat16_t type.
__aicore__ inline void asc_copy_l12bt(uint64_t dst,
                                      __cbuf__ bfloat16_t* src,
                                      uint16_t conv_control,
                                      uint16_t n_burst,
                                      uint16_t len_burst,
                                      uint16_t source_gap,
                                      uint16_t dst_gap)
```

### Continuous Data Transfer Mode

```c
// Placeholder form
__aicore__ inline void asc_copy_l12bt(uint64_t dst,
                                      __cbuf__ <dtype>* src,
                                      uint32_t size)
```

#### Data Types Supported by dtype

The supported data types of dtype are `half`, `bfloat16_t`, `int32_t`, and `float`.

#### Prototype Example

```c
// Example: continuous data transfer of the float type.
__aicore__ inline void asc_copy_l12bt(uint64_t dst,
                                      __cbuf__ float* src,
                                      uint32_t size)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand, stored in the BiasTable Buffer. The destination address must be 64-byte aligned. |
| src | Input | Source operand, stored in the L1 Buffer. The source address must be 32-byte aligned. |
| conv_control | Input | When the source operand is of the `half` type, whether to enable on-the-fly format conversion.<br>&nbsp;&nbsp;&bull; `0`: Conversion is disabled. Each `half` element is expanded to 32 bits (the upper 16 bits are placeholder data) and then written to BT.<br>&nbsp;&nbsp;&bull; `1`: Conversion is enabled. Each `half` element is converted to `float` and then written to BT. This can be set only when dtype is `half`; for other dtypes, set it to 0. |
| n_burst | Input | Number of continuous data blocks to transfer. Value range: [1, 4095]. |
| len_burst | Input | Length of each continuous data block, in 32 bytes. Value range: [1, 65535]. When dtype is `float` or `int32_t`, it must be 2-aligned (value is an even number); otherwise, an exception is triggered. |
| source_gap | Input | Gap between two adjacent transfers on the source side (L1 Buffer), in 32 bytes, from the end of the previous transfer to the start of the next transfer. Value range: [0, 65535]. It is meaningless when `n_burst=1`; set it to 0. |
| dst_gap | Input | Gap between two adjacent transfers on the destination side (BiasTable Buffer), in 32 bytes, from the end of the previous transfer to the start of the next transfer. Value range: [0, 65535]. It is meaningless when `n_burst=1`; set it to 0. When dtype is `float` or `int32_t`, it must be 2-aligned (value is an even number); otherwise, an exception is triggered. |
| size | Input | Total size of the data to transfer, in bytes.<br>**Note: size must be 32-byte aligned. If it is not aligned, the transfer amount is rounded down to 32-byte alignment.** |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

### General Constraints

- This API returns directly when it is not called by the AIC.
- The dst start address must be 64-byte aligned (BiasTable Buffer alignment requirement), and the src start address must be 32-byte aligned (L1 Buffer alignment requirement); otherwise, an exception is triggered.
- The BiasTable Buffer capacity is capped at 4 KB. The sum of the dst offset and the transfer size must not exceed the boundary; otherwise, a write overflow exception is triggered.
- The L1 Buffer capacity is capped at 512 KB. The sum of the src offset and the transfer size must not exceed the boundary; otherwise, a read exception is triggered.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- When n_burst is 0, no transfer is performed, and this API is treated as a NOP (no operation).
- When dtype is `float` or `int32_t`, both len_burst and dst_gap must be 2-byte aligned (that is, an even number), and size/32 must be an even number. Otherwise, a transfer exception is triggered.
- When conv_control is enabled (non-zero), dtype must be `half`. Otherwise, a transfer exception is triggered. For values other than 0/1, only the least significant bit takes effect (for example, 2 is equivalent to 0, meaning conversion is disabled; 3 is equivalent to 1, meaning conversion is enabled).

## Examples

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <cstdio>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

constexpr uint32_t M = 128, N = 128, K = 128;
constexpr uint32_t BYTES = N * sizeof(half);

__global__ __cube__ void CopyL12btKernel(__gm__ half* input)
{
    asc_init();
    __cbuf__ half l1[N];
    asc_copy_gm2l1(l1, input, 1, BYTES / 32, asc_channel_pad_mode::NONE, 0, 0);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12bt(0, l1, BYTES);
    asc_sync_pipe(PIPE_ALL);
}

int main()
{
    aclInit(nullptr); aclrtSetDevice(0);
    half* input = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    CopyL12btKernel<<<1, 0>>>(input);
    const aclError ret = aclrtSynchronizeDevice();
    std::printf("M=%u N=%u K=%u asc_copy_l12bt=%s\n", M, N, K,
        ret == ACL_SUCCESS ? "PASS" : "FAIL");
    aclrtFree(input); aclrtResetDevice(0); aclFinalize();
    return ret == ACL_SUCCESS ? 0 : 1;
}
```
