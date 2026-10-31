# asc_storealign

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.085956+00:00 pushedAt=2026-10-27T02:26:33.215Z nl=2 -->

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

Stores data from a vector data register or mask register to the Unified Buffer (UB) in an aligned manner. When the source operand is a vector data register, the store size per operation is VL (256 bytes), and a mask can be configured to indicate the elements that participate in the store: elements with a mask of 1 are written to the destination address, and elements with a mask of 0 retain the original data at the destination address. When the source operand is a mask register, the store size per operation is the mask register length (32 bytes), and the data in the mask register is stored directly without mask configuration. The data format and content remain unchanged during the movement. The destination operand is a UB address.

The vector data register store scenario provides the following four modes:

- **Contiguous aligned store mode**: Stores data from the vector data register to the UB destination address, with the destination address updated by the user.
- **Immediate offset store mode**: Specifies the offset relative to the destination start address through an immediate value, in elements. The user can choose to update the offset or the destination address.
- **Address register offset store mode**: Specifies the offset relative to the destination start address through an address register. It is commonly used in aligned store scenarios within a Hardware Loop where the offset changes with the loop count. It must be used together with [asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md).
- **Non-contiguous aligned store mode**: A single instruction stores 8 DataBlocks, with each DataBlock being 32 bytes. The stride between adjacent DataBlocks and the offset relative to the destination start address are configured through parameters.

The mask register store scenario provides the following three modes:

- **Contiguous aligned store mode**: Stores data from the mask register to the UB destination address, with the destination address updated by the user.
- **Immediate offset store mode**: Specifies the offset relative to the destination start address through an immediate value, in bytes. The user can choose to update the offset or the destination address.
- **Address register offset store mode**: Specifies the offset relative to the destination start address through an address register. It is commonly used in aligned store scenarios within a Hardware Loop where the offset changes with the loop count. It must be used together with [asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md).

This API is a Reg vector data movement API and takes effect only on AIV.

## Prototype

### Vector Data Register Contiguous Aligned Store Mode

```c
// Placeholder form.
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_<dtype> src,
                                           vector_bool mask)
```

#### Data Types Supported by dtype

The data types supported by dtype are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e1m2_t`, `fp4x2_e2m1_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`, and `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst,
                                           vector_int8_t src,
                                           vector_bool mask)
```

### Vector Data Register Immediate Offset Store Mode

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_<dtype> src,
                                           int32_t offset,
                                           vector_bool mask)
```

#### Data Types Supported by dtype

The data types supported by dtype are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e1m2_t`, `fp4x2_e2m1_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`, and `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst,
                                           vector_int8_t src,
                                           int32_t offset,
                                           vector_bool mask)
```

### Vector Data Register Address Register Offset Store Mode

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_<dtype> src,
                                           addr_reg offset,
                                           vector_bool mask)
```

#### Data Types Supported by dtype

The data types supported by dtype are `int4b_t`, `int8_t`, `uint8_t`, `fp4x2_e1m2_t`, `fp4x2_e2m1_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`. The corresponding vector register type is `vector_<dtype>`, and `int4b_t` corresponds to `vector_int4x2_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst,
                                           vector_int8_t src,
                                           addr_reg offset,
                                           vector_bool mask)
```

### Vector Data Register Non-Contiguous Aligned Store Mode

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_<dtype> src,
                                           uint16_t block_stride,
                                           uint16_t repeat_stride,
                                           vector_bool mask)
```

#### Data Types Supported by dtype

The data types supported by dtype are `int8_t`, `uint8_t`, `hifloat8_t`, `fp8_e8m0_t`, `fp8_e5m2_t`, `fp8_e4m3fn_t`, `int16_t`, `uint16_t`, `half`, `bfloat16_t`, `int32_t`, `uint32_t`, and `float`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst,
                                           vector_int8_t src,
                                           uint16_t block_stride,
                                           uint16_t repeat_stride,
                                           vector_bool mask)
```

### Mask Register Contiguous Aligned Store Mode

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_bool src)
```

#### Data Types Supported by dtype

The data types supported by dtype are `uint8_t`, `uint16_t`, and `uint32_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst,
                                           vector_bool src)
```

### Mask Register Immediate Offset Store Mode

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_bool src,
                                           int32_t offset)
```

#### Data Types Supported by dtype

The data types supported by dtype are `uint8_t`, `uint16_t`, and `uint32_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst,
                                           vector_bool src,
                                           int32_t offset)
```

### Mask Register Address Register Offset Store Mode

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_bool src,
                                           addr_reg offset)
```

#### Data Types Supported by dtype

The data types supported by dtype are `uint8_t`, `uint16_t`, and `uint32_t`.

#### Typical Prototype Example

```c
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst,
                                           vector_bool src,
                                           addr_reg offset)
```

## Parameters

### Vector Data Register Contiguous Aligned Store Mode

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector). The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (vector data register). dtype must be consistent with dst. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the data movement. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

### Vector Data Register Immediate Offset Store Mode

**Table 2** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector). The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (vector data register). dtype must be consistent with dst. |
| offset | Input | Offset relative to the start address of dst, of type `int32_t`, in units of elements. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the data movement. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

### Vector Data Register Address Register Offset Store Mode

**Table 3** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector). The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (vector data register). dtype must be consistent with dst. |
| offset | Input | Address register offset, of type `addr_reg`, which must be constructed in advance through the corresponding address register construction API. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the data movement. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

### Vector Data Register Non-Contiguous Aligned Store Mode

**Table 4** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector). The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (vector data register). |
| block_stride | Input | Stride between the start addresses of adjacent DataBlocks of the destination operand, of type `uint16_t`, in units of 32 bytes. |
| repeat_stride | Input | Offset of the start address of the destination operand, of type `uint16_t`, in units of 32 bytes. |
| mask | Input | Source operand mask (mask register), used to indicate the elements that participate in the data movement. A value of 1 at the corresponding position indicates participation, and 0 indicates non-participation. |

### Mask Register Contiguous Aligned Store Mode

**Table 5** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector). The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (mask register). |

### Mask Register Immediate Offset Store Mode

**Table 6** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector). The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (mask register). |
| offset | Input | Offset relative to the start address of dst, of type `int32_t`, in units of bytes. |

### Mask Register Address Register Offset Store Mode

**Table 7** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| dst | Output | Start address of the destination operand (vector). The type is `__ubuf__ <dtype>*`, and the start address must be 32-byte aligned. |
| src | Input | Source operand (mask register). |
| offset | Input | Address register offset, of type `addr_reg`, which must be constructed in advance through the corresponding address register construction API. |

For details about vector data registers and mask registers, see [Register Type Definition](../../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

### General Constraints

- For the general constraints on Reg vector computation C APIs, see [General Constraints](../overview.md#general-constraints).
- The start address of dst must be 32-byte aligned; otherwise, an error is reported.
- The UB capacity is capped at 256 KB. The capacity available to users varies with compilation options and programming scenarios (by default, 6 KB of SIMD VF stack and 2 KB of Ascend C reserved space are set aside, leaving 248 KB available; in SIMD+SIMT mixed programming, an additional 32 KB to 128 KB is partitioned as Data Cache, further reducing the available capacity). The destination operand address after applying the offset must not exceed the actual available capacity; otherwise, an error is reported.
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert the synchronization instruction [asc_mem_bar](../reg_sync/asc_mem_bar.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.

### Vector Data Register Store Scenario

- Positions where the mask bit is 0 use the hold mode: the corresponding positions of dst keep their original values, and src data is not written.
- The actual access address after applying parameters such as offset, block_stride, and repeat_stride must fall within the UB address range, and the actual access address must still be 32-byte aligned; otherwise, an error is reported.

### Mask Register Store Scenario

- The unit of `int32_t offset` is bytes. The actual access address after applying the offset parameter must fall within the UB address range and must still be 32-byte aligned; otherwise, an error is reported.

## Examples

Save the code as `examples.asc` and compile and run it with the `bisheng` command, where the `--npu-arch` parameter must specify the corresponding NPU architecture based on the actual product model. For the mapping between specific products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (corresponding to the NPU architecture `dav-3510`) as an example, the compilation and execution command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
template <typename T>
void print_data(const char* label, const std::vector<T>& values)
{
    std::cout << label << ":";
    const size_t count = values.size() < 8 ? values.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << +values[i];
    if (values.size() > count) std::cout << " ...";
    std::cout << std::endl;
}

constexpr uint32_t REGISTER_BYTES = 256;
constexpr uint32_t BUFFER_BYTES = 256;

__simd_vf__ inline void store_vf(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    for (uint16_t repeat = 0; repeat < 1; ++repeat) {
      vector_uint8_t src;
      asc_loadalign(src, input);
      asc_storealign(output, src, mask);
    }
}

__global__ __vector__ void asc_storealign_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES], input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    store_vf(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input(BUFFER_BYTES), output(BUFFER_BYTES, 0xff);
    for (uint32_t i = 0; i < BUFFER_BYTES; ++i) input[i] = static_cast<uint8_t>(i % 251);
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), (BUFFER_BYTES) * sizeof(uint8_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), (BUFFER_BYTES) * sizeof(uint8_t),
        ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(uint8_t), input.data(), input.size() * sizeof(uint8_t),
        ACL_MEMCPY_HOST_TO_DEVICE);
    asc_storealign_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(uint8_t), output_device, output.size() * sizeof(uint8_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input bytes", input);
    print_data("Output bytes", output);
    const bool passed = input == output;
    std::cout << (passed ? "[Success] asc_loadalign completed." : "[Failed] asc_loadalign output mismatch.")
              << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
