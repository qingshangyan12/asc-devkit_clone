# asc_intlv

<!-- md-trans-meta sourceCommit=20f3bc2ba9d24ddc30f5d7c0cf577fe8f93ec36c translatedAt=2026-10-27T01:34:54.937402+00:00 pushedAt=2026-10-27T02:26:33.052Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_permute_sel.h"`.

Interleaves the data in two source operand vector data registers and writes the results into two destination vector data registers. Depending on the operand type, this API provides two functional modes:

- **Operands are vector data registers:** Interleaving is performed element by element. The elements in the first half of `src0` and `src1` are alternately written to `dst0`, and the elements in the second half are alternately written to `dst1`. Taking the `int8_t` type as an example, the interleaving process is shown in the following figure.
- **Operands are mask registers:** Interleaving is performed at the granularity specified by the API suffix. `asc_intlv_b8` uses 1-bit groups, `asc_intlv_b16` uses 2-bit groups, and `asc_intlv_b32` uses 4-bit groups.

**Figure 1** Interleaving process for the int8_t type

![](../../../../figures/asc_intlv.png)

This API is a Reg vector computation API and takes effect only on AIV.

## Prototype

### Vector Data Register Interleaving Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_intlv(vector_<dtype>& dst0,
                                      vector_<dtype>& dst1,
                                      vector_<dtype> src0,
                                      vector_<dtype> src1)
```

#### Data Types Supported by dtype

`dtype` can be: `int8_t`, `uint8_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_intlv(vector_int8_t& dst0,
                                      vector_int8_t& dst1,
                                      vector_int8_t src0,
                                      vector_int8_t src1)
```

### Mask Register Interleaving Mode

```c
__simd_callee__ inline void asc_intlv_b8(vector_bool& dst0,
                                         vector_bool& dst1,
                                         vector_bool src0,
                                         vector_bool src1)
__simd_callee__ inline void asc_intlv_b16(vector_bool& dst0,
                                          vector_bool& dst1,
                                          vector_bool src0,
                                          vector_bool src1)
__simd_callee__ inline void asc_intlv_b32(vector_bool& dst0,
                                          vector_bool& dst1,
                                          vector_bool src0,
                                          vector_bool src1)
```

## Parameters

### Vector Data Register Interleaving Mode

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst0 | Output | Destination operand (vector data register), used to store the lower half of the interleaved data. The data type must be the same as that of `src0` and `src1`. |
| dst1 | Output | Destination operand (vector data register), used to store the upper half of the interleaved data. The data type must be the same as that of `src0` and `src1`. |
| src0 | Input | Source operand (vector data register). |
| src1 | Input | Source operand (vector data register). The data type must be the same as `src0`. |

### Mask Register Interleaving Mode

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst0 | Output | Destination operand (mask register), used to store the lower half of the interleaved data. |
| dst1 | Output | Destination operand (mask register), used to store the upper half of the interleaved data. |
| src0 | Input | Source operand (mask register). |
| src1 | Input | Source operand (mask register). |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The source and destination operands are vector data registers or mask registers.

### Register Constraints

- `src0` and `src1` can be the same vector data register or mask register.
- `dst0` and `dst1` cannot be the same vector data register or mask register.
- The source and destination operands can use the same vector data register or mask register, for example, `asc_intlv(src0, src1, src0, src1)`.

## Examples

Save the code as `example.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENT_COUNT = 256;
constexpr uint32_t HALF_COUNT = ELEMENT_COUNT / 2;
constexpr uint32_t BUFFER_BYTES = ELEMENT_COUNT * sizeof(uint8_t);

__simd_vf__ inline void intlv_vf(__ubuf__ uint8_t* output0, __ubuf__ uint8_t* output1,
                                 __ubuf__ uint8_t* input0, __ubuf__ uint8_t* input1)
{
    vector_uint8_t src0;
    vector_uint8_t src1;
    vector_uint8_t dst0;
    vector_uint8_t dst1;
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    asc_loadalign(src0, input0);
    asc_loadalign(src1, input1);
    asc_intlv(dst0, dst1, src0, src1);
    asc_storealign(output0, dst0, mask);
    asc_storealign(output1, dst1, mask);
}

__global__ __vector__ void asc_intlv_kernel(__gm__ uint8_t* output0, __gm__ uint8_t* output1,
                                            __gm__ uint8_t* input0, __gm__ uint8_t* input1)
{
    asc_init();
    __ubuf__ uint8_t output0_local[ELEMENT_COUNT];
    __ubuf__ uint8_t output1_local[ELEMENT_COUNT];
    __ubuf__ uint8_t input0_local[ELEMENT_COUNT];
    __ubuf__ uint8_t input1_local[ELEMENT_COUNT];
    asc_copy_gm2ub_align(input0_local, input0, BUFFER_BYTES);
    asc_copy_gm2ub_align(input1_local, input1, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    intlv_vf(output0_local, output1_local, input0_local, input1_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output0, output0_local, BUFFER_BYTES);
    asc_copy_ub2gm_align(output1, output1_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input0(ELEMENT_COUNT);
    std::vector<uint8_t> input1(ELEMENT_COUNT);
    std::vector<uint8_t> output0(ELEMENT_COUNT, 0);
    std::vector<uint8_t> output1(ELEMENT_COUNT, 0);
    for (uint32_t i = 0; i < ELEMENT_COUNT; ++i) {
        input0[i] = static_cast<uint8_t>(i);
        input1[i] = static_cast<uint8_t>(255 - i);
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input0_device = nullptr;
    uint8_t* input1_device = nullptr;
    uint8_t* output0_device = nullptr;
    uint8_t* output1_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input0_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&input1_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output0_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output1_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input0_device, BUFFER_BYTES, input0.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(input1_device, BUFFER_BYTES, input1.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_intlv_kernel<<<1, 0>>>(output0_device, output1_device, input0_device, input1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output0.data(), BUFFER_BYTES, output0_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(output1.data(), BUFFER_BYTES, output1_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    bool passed = true;
    for (uint32_t i = 0; i < HALF_COUNT; ++i) {
        passed = passed && output0[2 * i] == input0[i] && output0[2 * i + 1] == input1[i];
        passed = passed && output1[2 * i] == input0[i + HALF_COUNT]
                 && output1[2 * i + 1] == input1[i + HALF_COUNT];
    }
    std::cout << (passed ? "[Success] asc_intlv completed." : "[Failed] asc_intlv output mismatch.")
              << std::endl;
    aclrtFree(input0_device);
    aclrtFree(input1_device);
    aclrtFree(output0_device);
    aclrtFree(output1_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
