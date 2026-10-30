# asc_store

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.080960+00:00 pushedAt=2026-10-27T02:26:33.205Z nl=2 -->

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
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/composite/storeunalign_composite.h"`.

Stores data from a vector data register to the Unified Buffer (UB). The destination address must be aligned according to `dtype`. The underlying implementation completes a single contiguous write through an unaligned store instruction, and the data format and content remain unchanged during the movement. For ultimate performance, [asc_storealign](asc_storealign.md) is recommended for aligned scenarios. This API provides two functional modes with different parameter lists:

- **Contiguous store mode**: Writes the entire vector data register content to the start address of `dst`, with a store size of one vector length (`VL`, 256 bytes by default).
- **Store with count mode**: Writes only the first `count` elements of the vector data register to the start address of `dst`, for partial store scenarios.

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

### Contiguous Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_store(__ubuf__ <dtype>* dst,
                                      vector_<dtype> src)
```

#### Data Types Supported by dtype

The data types supported by `dtype` are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e1m2_t`, `fp4x2_e2m1_t`, `hifloat8_t`, `fp8_e4m3fn_t`, `fp8_e5m2_t`, `fp8_e8m0_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, `float`, and `int64_t`. The corresponding vector register type is `vector_<dtype>`, and `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_store(__ubuf__ int8_t* dst,
                                      vector_int8_t src)
```

### Store with Count Mode

```c
__simd_callee__ inline void asc_store(__ubuf__ <dtype>* dst,
                                      vector_<dtype> src,
                                      uint32_t count)
```

#### Data Types Supported by dtype

The data types supported by `dtype` are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e1m2_t`, `fp4x2_e2m1_t`, `hifloat8_t`, `fp8_e4m3fn_t`, `fp8_e5m2_t`, `fp8_e8m0_t`, `int16_t`, `uint16_t`, `bfloat16_t`, `int32_t`, `uint32_t`, `float`, and `int64_t`. The corresponding vector register type is `vector_<dtype>`, and `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_store(__ubuf__ int8_t* dst,
                                      vector_int8_t src,
                                      uint32_t count)
```

## Parameters

### Contiguous Store Mode

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector), of type `__ubuf__ <dtype>*`. The address must be aligned according to `dtype`. |
| src | Input | Source vector data register. |

### Store with Count Mode

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector), of type `__ubuf__ <dtype>*`. The address must be aligned according to `dtype`. |
| src | Input | Source vector data register. `dtype` must be consistent with `dst`. Only the first `count` elements participate in the store, and the remaining elements are ignored. |
| count | Input | Number of elements actually stored. It must not exceed the maximum number of elements that the vector data register can hold. |

For details about vector data registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The source operand is a vector data register, and the destination operand is a UB address. Pointers outside the UB address space cannot be passed as `dst`.
- The start address of `dst` must be aligned according to `dtype`. This API writes the vector data register content to the UB in an unaligned manner, so alignment is not required and no error is reported.
- UB capacity limit: The UB capacity is capped at 256 KB. The capacity available to users varies with compilation options and programming scenarios (by default, 6 KB of SIMD VF stack and 2 KB of Ascend C reserved space are set aside, leaving 248 KB available; in SIMD+SIMT mixed programming, an additional 32 KB to 128 KB is partitioned as Data Cache, further reducing the available capacity). The destination address must not exceed the actual available capacity; otherwise, an error is reported.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### Constraints for Store with Count Mode

- The value range of `count` is `[0, VL/sizeof(dtype)]`, where `VL` is 256 bytes. Exceeding this range leads to undefined behavior. After the offset, it must not exceed the actual available UB capacity; otherwise, an error is reported.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```c
#include <cstdint>
#include <iostream>
#include <vector>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 128;
constexpr uint32_t PARTIAL_COUNT = 16;
constexpr uint32_t REPEAT_TIMES = 2;
constexpr uint32_t FULL_BASE = 1;
constexpr uint32_t FULL_STRIDE = 144;
constexpr uint32_t PARTIAL_BASE = 288;
constexpr uint32_t PARTIAL_OFFSET = 7;
constexpr uint32_t PARTIAL_STRIDE = 32;
constexpr uint32_t OUTPUT_COUNT = 384;
constexpr uint16_t SENTINEL = 0xffff;
constexpr uint32_t INPUT_BYTES = ELEMENT_COUNT * sizeof(uint16_t);
constexpr uint32_t OUTPUT_BYTES = OUTPUT_COUNT * sizeof(uint16_t);

__simd_vf__ inline void store_vf(__ubuf__ uint16_t* output, __ubuf__ uint16_t* input)
{
    vector_uint16_t src;
    asc_loadalign(src, input);
    for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
        asc_store(output + FULL_BASE + repeat * FULL_STRIDE, src);
        asc_store(output + PARTIAL_BASE + PARTIAL_OFFSET + repeat * PARTIAL_STRIDE,
                  src, PARTIAL_COUNT);
    }
}

__global__ __vector__ void store_kernel(__gm__ uint16_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint16_t input_local[ELEMENT_COUNT];
    __ubuf__ uint16_t output_local[OUTPUT_COUNT];
    asc_copy_gm2ub(input_local, input, INPUT_BYTES);
    asc_copy_gm2ub(output_local, output, OUTPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<store_vf>(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm(output, output_local, OUTPUT_BYTES);
    asc_sync_pipe(PIPE_ALL);
}
} // namespace

#define CHECK_ACL(expr)                                                                     \
    do {                                                                                    \
        if ((expr) != ACL_SUCCESS) {                                                        \
            std::cerr << "ACL call failed: " << #expr << std::endl;                        \
            return 1;                                                                       \
        }                                                                                   \
    } while (0)

int main()
{
    std::vector<uint16_t> input(ELEMENT_COUNT);
    std::vector<uint16_t> output(OUTPUT_COUNT, SENTINEL);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input[i] = static_cast<uint16_t>(i + 1);
    }

    CHECK_ACL(aclInit(nullptr));
    CHECK_ACL(aclrtSetDevice(0));
    aclrtStream stream = nullptr;
    uint16_t* input_device = nullptr;
    uint16_t* output_device = nullptr;
    CHECK_ACL(aclrtCreateStream(&stream));
    CHECK_ACL(aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST));
    CHECK_ACL(aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST));
    CHECK_ACL(aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE));
    CHECK_ACL(aclrtMemcpy(output_device, OUTPUT_BYTES, output.data(), OUTPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE));
    store_kernel<<<1, 0, stream>>>(output_device, input_device);
    CHECK_ACL(aclrtSynchronizeStream(stream));
    CHECK_ACL(aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST));

    std::vector<uint16_t> expected(OUTPUT_COUNT, SENTINEL);
    for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
        for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
            expected[FULL_BASE + repeat * FULL_STRIDE + i] = input[i];
        }
        for (uint32_t i = 0; i < PARTIAL_COUNT; ++i) {
            expected[PARTIAL_BASE + PARTIAL_OFFSET + repeat * PARTIAL_STRIDE + i] = input[i];
        }
    }

    bool passed = true;
    for (uint32_t i = 0; i < OUTPUT_COUNT; ++i) {
        if (output[i] != expected[i]) {
            std::cerr << "Mismatch at " << i << ", expected " << expected[i]
                      << ", actual " << output[i] << std::endl;
            passed = false;
            break;
        }
    }
    std::cout << (passed ? "[Success] asc_store completed."
                         : "[Failed] asc_store output mismatch.") << std::endl;

    CHECK_ACL(aclrtFree(output_device));
    CHECK_ACL(aclrtFree(input_device));
    CHECK_ACL(aclrtDestroyStream(stream));
    CHECK_ACL(aclrtResetDevice(0));
    CHECK_ACL(aclFinalize());
    return passed ? 0 : 1;
}
```
