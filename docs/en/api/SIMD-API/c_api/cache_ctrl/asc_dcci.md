# asc_dcci

<!-- md-trans-meta sourceCommit=a63d5261c7c8077a0081e867f43a0e884fec066b translatedAt=2026-10-21T02:54:32.311Z pushedAt=2026-10-28T12:21:43.267Z nl=2 -->

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

The header file path is `"c_api/cache_ctrl/cache_ctrl.h"`.

As shown in Figure 1, inside the AI Core, Global Memory (GM) and Unified Buffer (UB) are accessed in the following two ways:

- The DMA unit reads from and writes to GM. Data is exchanged between local memory such as UB and GM through APIs such as [asc_copy_ub2gm](../vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm.md) and [asc_copy_gm2ub](../vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md), without going through the Data Cache (DCache). Therefore, cache coherence is not a concern.
- When the Scalar unit accesses GM, if it goes through the DCache, it first accesses the DCache in each core. Therefore, a cache coherence issue exists between the DCache and GM. For details, see [Cache Coherence](../../../../guide/programming_guide/advanced_programming/memory_model/cache_coherence.md).

**Figure 1**  DCache memory hierarchy diagram

![](../../../figures/dcci.png "DCache memory hierarchy diagram")

The asc_dcci API is used to resolve the coherence issue described above. Its functionality can be divided into two parts:

- Clean: Writes the modified data (dirty data) in the cache back to GM to prevent data loss.
- Invalid:
    - Marks the cache lines in the specified address range as "invalid" so that they are removed from the cache.
    - Ensures that the next access to these memory addresses reloads data from GM instead of using potentially stale cached data.

Typical use cases of this API are as follows (using GM read/write as an example):

- When reading data from GM that may have been modified externally by other cores, use the asc_dcci API to access GM directly and obtain the latest data.
- When you write data to GM through the Scalar unit and want it to be written out immediately, you also need to use the asc_dcci API.

> [!NOTE]Note
> - When the Scalar unit reads or writes GM, if you call the [asc_load_dev](../scalar_compute/scalar_load/asc_load_dev.md) and [asc_store_dev](../scalar_compute/scalar_store/asc_store_dev.md) API pair, the read/write data does not pass through the DCache, so you do not need to call the asc_dcci API.
> - Whether the Scalar unit passes through the DCache when reading or writing UB is controlled by the CTRL[49] bit configured by the [asc_set_ctrl](../spr/asc_set_ctrl.md) API. The default value of this bit is 1'b0, meaning that it does not pass through the DCache by default. For details about CTRL[49], see the description of common CTRL register bits in [asc_set_ctrl](../spr/asc_set_ctrl.md#ctrl_bit_desc).

## Prototype

```c
// Flush the single cache line where the passed GM address resides.
__aicore__ inline void asc_dcci_single(__gm__ void* dst)

// Flush the single cache line where the passed UB address resides.
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst)

// Flush all valid cache lines in the DCache based on the target domain. The target domain has the following four values:

// Flush all valid cache lines in the entire DCache.
__aicore__ inline void asc_dcci_entire_all()

// Flush all valid cache lines in the entire DCache that contain UB addresses.
__aicore__ inline void asc_dcci_entire_ub()

// Flush all valid cache lines in the entire DCache that contain GM addresses.
__aicore__ inline void asc_dcci_entire_out()

// Flush all valid cache lines in the entire DCache that contain GM addresses related to scalar atomic operations.
__aicore__ inline void asc_dcci_entire_atomic()
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Input | Address where the cache lines to be cleaned and invalidated reside.<br>The hardware aligns the target line to the cache line boundary, so the passed address does not need to be cache line aligned. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

<!-- npu="950" id8 -->
- For Ascend 950PR&950DT products, the asc_dcci_entire_ub API is not supported.
<!-- end id8 -->
- The prototype with `dst` of type `__ubuf__ void*` can be called only in the AIV.

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id9 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id9 -->

```cpp
// This example verifies the writeback effect of asc_dcci_entire_out on GM dirty data. The Scalar first writes data in two different cache lines.
// Write 101, and then call asc_dcci_entire_out to write the dirty data in the DCache back to GM. After the Host reads it back, it should get 101.
#include <array>
#include <cstdint>
#include <iostream>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr int32_t GM_VALUE = 11;
constexpr int32_t DIRTY_VALUE = 101;
constexpr uint32_t SECOND_LINE_OFFSET = 32;
constexpr uint32_t DATA_COUNT = 64;

__global__ __vector__ void AscDcciEntireOutKernel(__gm__ int32_t* data)
{
    asc_init();
    volatile __gm__ int32_t* data_gm = data;
    // The two elements are 128B apart and located in different cache lines. A normal Scalar write to GM goes through the DCache.
    data_gm[0] = DIRTY_VALUE;
    data_gm[SECOND_LINE_OFFSET] = DIRTY_VALUE;

    // Flush the cache lines in the DCache that contain the GM address, and wait for the dirty data to be written back to GM.
    asc_dcci_entire_out();
    asc_sync_data_barrier(mem_dsb_t::DSB_DDR);
}

bool CheckResult(const std::array<int32_t, DATA_COUNT>& output)
{
    return output[0] == DIRTY_VALUE && output[SECOND_LINE_OFFSET] == DIRTY_VALUE;
}
} // namespace

int main()
{
    std::array<int32_t, DATA_COUNT> input{};
    input.fill(GM_VALUE);
    aclInit(nullptr);
    aclrtSetDevice(0);

    int32_t* input_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), input.size() * sizeof(int32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, input.size() * sizeof(int32_t), input.data(), input.size() * sizeof(int32_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscDcciEntireOutKernel<<<1, 0>>>(input_device);
    const aclError ret = aclrtSynchronizeDevice();
    aclrtMemcpy(input.data(), input.size() * sizeof(int32_t), input_device, input.size() * sizeof(int32_t), ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = ret == ACL_SUCCESS && CheckResult(input);
    std::cout << (passed ? "[Success] asc_dcci passed." : "[Failed] asc_dcci failed.") << std::endl;
    aclrtFree(input_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
