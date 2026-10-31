# asc_datacache_preload

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-21T02:54:36.976Z pushedAt=2026-10-28T12:21:43.269Z nl=2 -->

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

Preloads one cache line (64 bytes) of data from the specified Global Memory address into the DCache, filling the cache in advance for subsequent reads of that address by the Scalar unit to reduce first-access latency.

The prefetch address is the sum of `address` and `offset`, in bytes.

This API must be used together with subsequent instructions that read Global Memory through the Scalar unit for the prefetch to yield a cache-hit benefit.

## Prototype

```c
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)
```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
|---|---|---|
| address | Input | Address of the source operand in Global Memory, unit: byte. |
| offset | Input | Signed byte offset relative to `address`. The prefetch start address = `address` + `offset`, unit: byte. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

Calling this API frequently may cause congestion in the reservation station (the hardware queue that holds instructions pending execution). In this case, the instruction is treated as a NOP instruction, which in turn blocks the Scalar pipeline. Therefore, do not call this API more than four times consecutively.

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
// This example is for functional verification only and does not represent an actual operator implementation.
// It simulates the Scalar unit reading scattered data from GM through indirect indexing, and prefetches the Cache Line of the next access into DCache in advance,
// to reduce the memory access latency of subsequent Scalar reads.
#include <cstdint>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t DATA_SIZE = 256;
constexpr uint32_t INDEX_COUNT = 4;
constexpr uint32_t DATA_BYTES = DATA_SIZE * sizeof(uint8_t);
constexpr uint32_t INDEX_BYTES = INDEX_COUNT * sizeof(uint32_t);

__global__ __vector__ void AscDatacachePreloadKernel(__gm__ uint8_t* data, __gm__ uint32_t* indices,
    __gm__ uint32_t* result, uint32_t count)
{
    asc_init();
    if (count == 0) {
        result[0] = 0;
        return;
    }

    // indices stores the byte offsets of the data to be accessed. This example first prefetches the 64B Cache Line where the first index resides,
    // and then prefetches the Cache Line of the next index while reading the current data. This makes the next round of Scalar reads
    // more likely to hit DCache. The offset is aligned down to 64B to ensure that the prefetch range covers the target data.
    uint32_t current_index = indices[0];
    int64_t cache_offset = static_cast<int64_t>(current_index & ~63ULL);
    // The first round prefetches the Cache Line where the current index resides, and subsequent loops prefetch the data to be read in the next round in advance.
    asc_datacache_preload(reinterpret_cast<__gm__ uint64_t*>(data), cache_offset);

    uint32_t sum = 0;
    for (uint32_t i = 0; i < count; ++i) {
        sum += data[current_index];
        if (i + 1 < count) {
            current_index = indices[i + 1];
            cache_offset = static_cast<int64_t>(current_index & ~63ULL);
            asc_datacache_preload(reinterpret_cast<__gm__ uint64_t*>(data), cache_offset);
        }
    }
    result[0] = sum;
}
} // namespace

int main()
{
    std::vector<uint8_t> data(DATA_SIZE);
    for (uint32_t i = 0; i < DATA_SIZE; ++i) {
        data[i] = static_cast<uint8_t>(i);
    }
    std::vector<uint32_t> indices = {0, 64, 128, 192};
    uint32_t result = 0;
    constexpr uint32_t GOLDEN = 384;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* data_device = nullptr;
    uint32_t* indices_device = nullptr;
    uint32_t* result_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&data_device), DATA_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&indices_device), INDEX_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&result_device), sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(data_device, DATA_BYTES, data.data(), DATA_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(indices_device, INDEX_BYTES, indices.data(), INDEX_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    AscDatacachePreloadKernel<<<1, 0>>>(data_device, indices_device, result_device, INDEX_COUNT);
    const aclError ret = aclrtSynchronizeDevice();
    aclrtMemcpy(&result, sizeof(uint32_t), result_device, sizeof(uint32_t), ACL_MEMCPY_DEVICE_TO_HOST);
    const bool passed = ret == ACL_SUCCESS && result == GOLDEN;
    std::cout << (passed ? "[Success] asc_datacache_preload passed."
                         : "[Failed] asc_datacache_preload failed.") << std::endl;
    aclrtFree(data_device);
    aclrtFree(indices_device);
    aclrtFree(result_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
