# asc_storealign_pack_quarter

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.101573+00:00 pushedAt=2026-10-27T02:26:33.259Z nl=2 -->

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

Packs and stores data from a vector data register to the Unified Buffer (UB). `mask` indicates the elements that participate in the store. When the corresponding `mask` bit is 1, the low 8 bits of each valid 32-bit element in `src` are written to the corresponding packed destination position; when the corresponding `mask` bit is 0, the corresponding packed destination position retains its original value. The destination operand is a UB address.

This API provides three functional modes with different parameter lists:

- **Aligned store mode**: The offset is fixed at 0. The packed data is stored to the UB start address, and the user updates the destination address.
- **Immediate offset store mode**: An `int32_t offset` specifies the offset relative to the destination start address, in elements. The user can choose to update the offset or the destination address.
- **Address register offset store mode**: Uses the address register `addr_reg offset` to specify the offset relative to the destination start address. It is commonly used in compressed store scenarios where the offset changes with the loop count inside a Hardware Loop. It must be used together with [asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md).

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

### Aligned Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ <dtype>* dst,
                                                        vector_<dtype> src,
                                                        vector_bool mask)
```

#### Data Types Supported by dtype

`dtype` supports `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst,
                                                        vector_int32_t src,
                                                        vector_bool mask)
```

### Immediate Offset Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ <dtype>* dst,
                                                        vector_<dtype> src,
                                                        int32_t offset,
                                                        vector_bool mask)
```

#### Data Types Supported by dtype

`dtype` supports `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst,
                                                        vector_int32_t src,
                                                        int32_t offset,
                                                        vector_bool mask)
```

### Address Register Offset Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ <dtype>* dst,
                                                        vector_<dtype> src,
                                                        addr_reg offset,
                                                        vector_bool mask)
```

#### Data Types Supported by dtype

`dtype` supports `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst,
                                                        vector_int32_t src,
                                                        addr_reg offset,
                                                        vector_bool mask)
```


## Parameters

### Aligned Store Mode

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand. The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (vector data register). `dtype` must be the same as `dst`. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the data movement. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

### Immediate Offset Store Mode

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand. The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (vector data register). `dtype` must be the same as `dst`. |
| offset | Input | Offset relative to the start address of `dst`, of type `int32_t`, in elements. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the data movement. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

### Address Register Offset Store Mode

**Table 3** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand. The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (vector data register). `dtype` must be the same as `dst`. |
| offset | Input | Address register offset, of type `addr_reg`, which must be constructed in advance through the corresponding address register construction API. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the data movement. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

For details about vector data registers, mask registers, and address registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The start address of `dst` must be 32-byte aligned; otherwise, an error is reported.
- `mask` must be a `vector_bool` mask register. The content of an unassigned mask register is indeterminate, which may cause incorrect valid element positions.
- The total UB capacity is 256 KB. The capacity available to users varies with compilation options and programming scenarios (by default, 6 KB of SIMD VF stack and 2 KB of Ascend C reserved space are set aside, leaving 248 KB available; in SIMD+SIMT mixed programming, an additional 32 KB to 128 KB is partitioned as Data Cache, further reducing the available capacity). The UB range corresponding to the `dst` offset and the write area must not exceed the actual available capacity; otherwise, an error is reported.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### int32_t Offset Packed Store Scenario

- The unit of `offset` is elements. The actual access address after applying the `offset` parameter must fall within the UB address range and must still be 32-byte aligned; otherwise, an error is reported.
- Source operand elements whose `mask` bit is 1 participate in the store; elements whose bit is 0 do not participate, and the corresponding packed destination positions retain their original values.

### addr_reg Offset Packed Store Scenario

- `offset` must be constructed in advance by the corresponding address register construction API.
- The actual access address after applying the `offset` parameter must fall within the UB address range and must still be 32-byte aligned; otherwise, an error is reported.
- Source operand elements whose `mask` bit is 1 participate in the store; elements whose bit is 0 do not participate, and the corresponding packed destination positions retain their original values.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

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
constexpr uint32_t ELEMENT_COUNT = 64;
constexpr uint32_t INPUT_BYTES = 256;
constexpr uint32_t PACKED_BYTES = 64;
constexpr uint32_t REPEAT_TIMES = 2;
constexpr uint32_t STEP_BYTES = 128;
constexpr uint32_t SECTION_BYTES = REPEAT_TIMES * STEP_BYTES;
constexpr uint32_t OUTPUT_BYTES = 3 * SECTION_BYTES;
constexpr uint8_t SENTINEL = 0xa5;

__simd_vf__ inline void pack_quarter_vf(__ubuf__ uint8_t* output,
                                        __ubuf__ uint32_t* input)
{
    vector_bool mask = asc_create_mask_b32(PAT_ALL);
    vector_int32_t src_i32;
    vector_uint32_t src_u32;
    vector_float src_f32;
    asc_loadalign(src_i32, reinterpret_cast<__ubuf__ int32_t*>(input));
    asc_loadalign(src_u32, input);
    asc_loadalign(src_f32, reinterpret_cast<__ubuf__ float*>(input));
    for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
        asc_storealign_pack_quarter(reinterpret_cast<__ubuf__ int32_t*>(
            output + repeat * STEP_BYTES), src_i32, mask);
        asc_storealign_pack_quarter(reinterpret_cast<__ubuf__ uint32_t*>(
            output + SECTION_BYTES + repeat * STEP_BYTES), src_u32, mask);
        asc_storealign_pack_quarter(reinterpret_cast<__ubuf__ float*>(
            output + 2 * SECTION_BYTES + repeat * STEP_BYTES), src_f32, mask);
    }
}

__global__ __vector__ void asc_storealign_pack_quarter_kernel(
    __gm__ uint8_t* output, __gm__ uint32_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[OUTPUT_BYTES];
    __ubuf__ uint32_t input_local[ELEMENT_COUNT];
    asc_copy_gm2ub(output_local, output, OUTPUT_BYTES);
    asc_copy_gm2ub(input_local, input, INPUT_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    pack_quarter_vf(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm(output, output_local, OUTPUT_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint32_t> input(ELEMENT_COUNT);
    std::vector<uint8_t> output(OUTPUT_BYTES, SENTINEL);
    std::vector<uint8_t> expected(OUTPUT_BYTES, SENTINEL);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input[i] = 0x5a000000u | i;
        const uint8_t value = static_cast<uint8_t>(input[i]);
        for (uint32_t section = 0; section < 3; ++section) {
            for (uint32_t repeat = 0; repeat < REPEAT_TIMES; ++repeat) {
                expected[section * SECTION_BYTES + repeat * STEP_BYTES + i] = value;
            }
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(output_device, OUTPUT_BYTES, output.data(), OUTPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_storealign_pack_quarter_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output == expected;
    std::cout << (passed ? "[Success] asc_storealign_pack_quarter passed."
                         : "[Failed] asc_storealign_pack_quarter output mismatch.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
