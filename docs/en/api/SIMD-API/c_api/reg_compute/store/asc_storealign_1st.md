# asc_storealign_1st

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.088900+00:00 pushedAt=2026-10-27T02:26:33.222Z nl=2 -->

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

The header file path is `"c_api/reg_compute/store/storealign.h"`.

Stores the first element from the Vector Data Register to the Unified Buffer (UB), with a single store size of one element. This API does not support mask configuration and always stores the first element of the Vector Data Register to the destination address. The destination address must be aligned to `sizeof(dtype)`, and the data format and content remain unchanged during the store.

This API provides three functional modes with different parameter lists:

- **Aligned store mode**: Stores the first element of the Vector Data Register to the start address of UB. For multiple calls, the user updates the destination address.
- **Immediate offset store mode**: Specifies the element offset relative to the destination start address through `int32_t offset`. The user can choose to update the offset or the destination address.
- **Address register offset store mode**: Specifies the offset relative to the destination start address through the address register `addr_reg offset`. It is commonly used in aligned store scenarios within a Hardware Loop where the offset changes with the loop count. It must be used together with [asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md).

Taking the **b16 bit-width single-point store** process as an example, the schematic diagram is as follows:

**Figure 1** b16 single-element store data

![](../../../../figures/reg_store_onept.png)

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

### Aligned Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign_1st(__ubuf__ <dtype>* dst,
                                               vector_<dtype> src)
```

#### Data Types Supported by dtype

The data types supported by `dtype` are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`, and `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst,
                                               vector_int8_t src)
```

### Immediate Offset Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign_1st(__ubuf__ <dtype>* dst,
                                               vector_<dtype> src,
                                               int32_t offset)
```

#### Data Types Supported by dtype

The data types supported by `dtype` are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`, and `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst,
                                               vector_int8_t src,
                                               int32_t offset)
```

### Address Register Offset Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign_1st(__ubuf__ <dtype>* dst,
                                               vector_<dtype> src,
                                               addr_reg offset)
```

#### Data Types Supported by dtype

The data types supported by `dtype` are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e2m1_t`, `fp4x2_e1m2_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`, and `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst,
                                               vector_int8_t src,
                                               addr_reg offset)
```

## Parameters

### Aligned Store Mode

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand, of type `__ubuf__ <dtype>*`. The start address must be dtype-aligned. |
| src | Input | Source operand (vector data register). Only the first element participates in the store; the remaining elements are ignored. |

### Immediate Offset Store Mode

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand, of type `__ubuf__ <dtype>*`. The start address must be dtype-aligned. |
| src | Input | Source operand (vector data register). Only the first element participates in the store; the remaining elements are ignored. |
| offset | Input | Offset relative to the start address of `dst`, of type `int32_t`, in elements. |

### Address Register Offset Store Mode

**Table 3** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand, of type `__ubuf__ <dtype>*`. The start address must be dtype-aligned. |
| src | Input | Source operand (vector data register). Only the first element participates in the store; the remaining elements are ignored. |
| offset | Input | Address register offset, of type `addr_reg`, pre-constructed by the corresponding address register construction API, in elements. |

For details about vector data registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The source operand is a vector data register, and the destination operand is a UB address. Pointers outside the UB address space cannot be passed as `dst`.
- UB capacity limit: The total UB capacity is 256 KB. The capacity available to users varies with compilation options and programming scenarios (by default, 6 KB of SIMD VF stack and 2 KB of Ascend C reserved space are set aside, leaving 248 KB available; in SIMD+SIMT mixed programming, an additional 32 KB to 128 KB is partitioned as Data Cache, further reducing the available capacity). `dst` after the offset must not exceed the actual available capacity.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### Instruction Constraints

- The start address of `dst` must be dtype-aligned, and the actual access address after the `offset` must be dtype-aligned and within the UB address range.

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
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 128;
constexpr uint32_t REPEAT_TIMES = 3;
constexpr uint32_t DIRECT_BASE = 0;
constexpr uint32_t IMMEDIATE_BASE = 16;
constexpr uint32_t IMMEDIATE_OFFSET = 17;
constexpr uint32_t ADDR_BASE = 64;
constexpr uint32_t ADDR_STRIDE = 7;
constexpr uint16_t SENTINEL = 0xffff;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(uint16_t);

__simd_vf__ inline void store_first_vf(__ubuf__ uint16_t* output, __ubuf__ uint16_t* input)
{
    vector_uint16_t src;
    asc_loadalign(src, input);
    addr_reg offset;
    for (uint32_t i = 0; i < REPEAT_TIMES; ++i) {
        asc_storealign_1st(output + DIRECT_BASE + i, src);
        asc_storealign_1st(output + IMMEDIATE_BASE + i, src,
                           static_cast<int32_t>(IMMEDIATE_OFFSET));
        offset = asc_update_addr_reg_b16(ADDR_STRIDE);
        asc_storealign_1st(output + ADDR_BASE, src, offset);
    }
}

__global__ __vector__ void store_first_kernel(__gm__ uint16_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint16_t input_local[ELEMENT_COUNT];
    __ubuf__ uint16_t output_local[ELEMENT_COUNT];
    asc_copy_gm2ub(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub(output_local, output, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<store_first_vf>(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm(output, output_local, BUFFER_BYTES);
    asc_sync_pipe(PIPE_ALL);
}
} // namespace

int main()
{
    std::vector<uint16_t> input(ELEMENT_COUNT);
    std::vector<uint16_t> output(ELEMENT_COUNT, SENTINEL);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) input[i] = static_cast<uint16_t>(i + 1);
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t* input_device = nullptr;
    uint16_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BUFFER_BYTES, input.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, BUFFER_BYTES, output.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    store_first_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BUFFER_BYTES, output_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    bool passed = true;
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        const bool written = (i >= DIRECT_BASE && i < DIRECT_BASE + REPEAT_TIMES) ||
            (i >= IMMEDIATE_BASE + IMMEDIATE_OFFSET &&
             i < IMMEDIATE_BASE + IMMEDIATE_OFFSET + REPEAT_TIMES) ||
            (i >= ADDR_BASE && i < ADDR_BASE + REPEAT_TIMES * ADDR_STRIDE &&
             (i - ADDR_BASE) % ADDR_STRIDE == 0);
        passed = passed && output[i] == (written ? input[0] : SENTINEL);
    }
    std::cout << (passed ? "[Success] asc_storealign_1st completed."
                         : "[Failed] asc_storealign_1st output mismatch.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
