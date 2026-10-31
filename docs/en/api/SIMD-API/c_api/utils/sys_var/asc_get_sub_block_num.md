# asc_get_sub_block_num

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-23T03:54:34.201178+00:00 pushedAt=2026-10-27T02:26:33.784Z nl=2 -->

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
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/utils/sys_var.h"`.

Reads the number of Cube Cores (AICs) or Vector Cores (AIVs) on the current AI Core in [separation mode](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md). The value is configured before the kernel starts and cannot be changed during execution.

## Prototype

```c
__aicore__ inline int64_t asc_get_sub_block_num()
```

## Parameters

None

## Return Value


For different kernel function (Kernel) types, the return values of this API when called on AIC and AIV are as follows:

**Table 1** Return values for different Kernel function (Kernel) types

|Kernel Type|KERNEL_TYPE_AIV_ONLY|KERNEL_TYPE_AIC_ONLY|KERNEL_TYPE_MIX_AIC_1_2|KERNEL_TYPE_MIX_AIC_1_1|KERNEL_TYPE_MIX_AIC_1_0|KERNEL_TYPE_MIX_AIV_1_0|
| :------ | :------------------ | :----------------- | :-------------------- | :--------------------- | :-------------------- | :-------------------- |
|AIV      |1                    |-                   |2                      |1                       |-                      |1                      |
|AIC      |-                    |1                   |1                      |1                       |1                      |-                      |

## Pipeline Type

PIPE_S

## Constraints

- The subblock dimension read by this API is stored in a read-only special register. It is configured by the System Controller (SC) before the kernel starts and cannot be changed during kernel execution. Two consecutive calls return the same value.
- The return value reflects the number of subblocks on the current AI Core and is meaningful only in [separate mode](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md). In non-separate mode, the semantics of the return value are subject to the SC configuration.

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
#include <iomanip>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENTS = 16;
constexpr uint32_t BYTES = ELEMENTS * sizeof(uint64_t);

void print_data(const char* label, const std::vector<uint64_t>& data)
{
    std::cout << label << ":";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}

void print_hex(const char* label, const std::vector<uint64_t>& data)
{
    std::cout << label << ":" << std::hex;
    for (uint32_t i = 0; i < 4; ++i) std::cout << " 0x" << data[i];
    std::cout << std::dec << std::endl;
}

__global__ __vector__ void asc_get_sub_block_num_kernel(__gm__ uint64_t* output)
{
    asc_init();
    const uint64_t sub_id = static_cast<uint64_t>(asc_get_sub_block_id());
    const uint64_t sub_num = static_cast<uint64_t>(asc_get_sub_block_num());
    output[0] = sub_num;
    output[1] = sub_id;
    output[2] = sub_num;
    asc_dcci_single(output);
}

} // namespace

int main()
{
    std::vector<uint64_t> output(ELEMENTS, ~0ULL);
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint64_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(output_device, BYTES, output.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_get_sub_block_num_kernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Observed", output);
    print_hex("Observed hex", output);
    const bool passed = output[2] > 0 && output[1] < output[2];
    std::cout << (passed ? "[Success] asc_get_sub_block_num passed." : "[Failed] asc_get_sub_block_num failed.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
