# asc_dci

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T02:54:44.813Z pushedAt=2026-10-28T12:21:43.271Z nl=2 -->

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

The header file path is `"c_api/cache_ctrl/cache_ctrl.h"`.

This data cache invalidation API marks all cache lines in the entire DCache as invalid, removing them from the cache. This API does not check whether a cache line is dirty (dirty means the data has been modified but not yet written back to GM). If dirty data exists, it is deprecated directly without writeback. After the call, the next access to these memory addresses reloads data from GM instead of using potentially stale cached data.

This API takes no parameters. Calling it triggers a full invalidation of the entire DCache.

## Prototype

```c
__aicore__ inline void asc_dci()
```

## Parameters

None

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

Before calling `asc_dci`, you must call [asc_sync_data_barrier](../sync/intra_core_sync/asc_sync_data_barrier.md) with the parameter set to `mem_dsb_t::DSB_ALL` to wait for the preceding memory access instructions to complete, and then invalidate the entire data cache. The call sequence is as follows:

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
asc_dci();
```

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <array>
#include <cstdint>
#include <iostream>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr int32_t GM_VALUE = 11;
constexpr int32_t DIRTY_VALUE = 101;
constexpr uint32_t DATA_COUNT = 16;

__global__ __vector__ void AscDciKernel(__gm__ int32_t* data, __gm__ int32_t* output)
{
    asc_init();
    // This example verifies that asc_dci does not write dirty data in the DCache back to GM. Scalar first writes a dirty value to the GM address.
    // After invalidation, reading this address through Scalar again reloads the initial value from GM instead of returning the dirty value.
    volatile __gm__ int32_t* data_gm = data;
    data_gm[0] = DIRTY_VALUE;

    // After the preceding memory accesses complete, invalidate the entire DCache and directly deprecate the dirty data in it.
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();

    const int32_t value_after_dci = data_gm[0];
    asc_store_dev(output, value_after_dci);
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
}

bool CheckResult(int32_t output)
{
    return output == GM_VALUE;
}
} // namespace

int main()
{
    std::array<int32_t, DATA_COUNT> input{};
    input.fill(GM_VALUE);
    int32_t output = 0;
    aclInit(nullptr);
    aclrtSetDevice(0);

    int32_t* input_device = nullptr;
    int32_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), input.size() * sizeof(int32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), sizeof(int32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(int32_t), input.data(), input.size() * sizeof(int32_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscDciKernel<<<1, 0>>>(input_device, output_device);
    const aclError ret = aclrtSynchronizeDevice();
    aclrtMemcpy(&output, sizeof(int32_t), output_device, sizeof(int32_t), ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = ret == ACL_SUCCESS && CheckResult(output);
    std::cout << (passed ? "[Success] asc_dci passed." : "[Failed] asc_dci failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
