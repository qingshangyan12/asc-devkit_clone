# Ascend 3510 C API 示例

展示 3510 C API 使用模式的完整可运行示例。
所有示例假定项目已配置 `find_package(ASC REQUIRED)` 且引入了 `#include "c_api/asc_simd.h"`。

---

## RegBase编程模板

3510架构下的RegBase编程，其处理的对象为RegTensor,即数据从GM搬运到UB,再从UB搬运到寄存器RegTensor,在寄存器中完成运算后,再将结果按原路搬回，使用时应注意函数和变量声明的标识符：

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

constexpr uint32_t TILE_LENGTH = 2048;
constexpr uint32_t NUM_BLOCKS = 8;
constexpr uint32_t BLK_NUM = 1;
constexpr uint32_t MASK = 32;

__simd_vf__ inline void AddVF(uint16_t rep, uint16_t one_rep_size, uint32_t blockLength, __ubuf__ float* xLocal, __ubuf__ float* yLocal, __ubuf__ float* zLocal)
{
    vector_bool vmask;
    vector_float reg_src0;
    vector_float reg_src1;
    vector_float reg_dst;
    uint32_t remaining = blockLength;
    for (uint16_t i = 0; i < rep; ++i) {
        vmask = asc_update_mask_b32(remaining);
        asc_loadalign(reg_src0, xLocal + i * one_rep_size);
        asc_loadalign(reg_src1, yLocal + i * one_rep_size);    
        asc_add(reg_dst, reg_src0, reg_src1, vmask);
        asc_storealign(zLocal + i * one_rep_size, reg_dst, vmask);
    }
}

__vector__ __global__ __aicore__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    asc_init();

    uint32_t blockLength = TILE_LENGTH * NUM_BLOCKS / asc_get_block_num();

    __gm__ float* xGm = x + get_block_idx() * blockLength;
    __gm__ float* yGm = y + get_block_idx() * blockLength;
    __gm__ float* zGm = z + get_block_idx() * blockLength;

    __ubuf__ float xLocal[TILE_LENGTH];
    __ubuf__ float yLocal[TILE_LENGTH];
    __ubuf__ float zLocal[TILE_LENGTH];

    const uint8_t cacheMode0 = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
    const uint8_t cacheMode1 = static_cast<uint8_t>(((uint64_t)yGm) >> 60);
    const uint8_t cacheMode2 = static_cast<uint8_t>(((uint64_t)zGm) >> 60);
    uint32_t burstLength = blockLength * 4;
    uint64_t srcStride = 0;
    uint32_t dstStride = 0;

    asc_copy_gm2ub_align((__ubuf__ float*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode0, srcStride, dstStride);
    asc_copy_gm2ub_align((__ubuf__ float*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode1, srcStride, dstStride);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    uint16_t mask_bit_size = 256;
    uint16_t one_rep_size = mask_bit_size / sizeof(float);
    uint16_t rep = (blockLength + one_rep_size - 1) / one_rep_size;
    AddVF(rep, one_rep_size, blockLength, (__ubuf__ float*)xLocal, (__ubuf__ float*)yLocal, (__ubuf__ float*)zLocal );
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);

    asc_copy_ub2gm_align(zGm, (__ubuf__ float*)zLocal, BLK_NUM, burstLength, cacheMode2, srcStride, dstStride);
}

std::vector<float> kernel_add(std::vector<float> &x, std::vector<float> &y)
{
    constexpr uint32_t numBlocks = NUM_BLOCKS;
    uint32_t totalLength = x.size();
    size_t totalByteSize = totalLength * sizeof(float);
    int32_t deviceId = 0;
    aclrtStream stream = nullptr;
    uint8_t *xHost = reinterpret_cast<uint8_t *>(x.data());
    uint8_t *yHost = reinterpret_cast<uint8_t *>(y.data());
    uint8_t *zHost = nullptr;
    float *xDevice = nullptr;
    float *yDevice = nullptr;
    float *zDevice = nullptr;

    aclInit(nullptr);
    aclrtSetDevice(deviceId);
    aclrtCreateStream(&stream);

    aclrtMallocHost((void **)(&zHost), totalByteSize);
    aclrtMalloc((void **)&xDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&yDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&zDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);

    aclrtMemcpy((uint8_t*)xDevice, totalByteSize, xHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy((uint8_t*)yDevice, totalByteSize, yHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);

    add_custom<<<numBlocks, nullptr, stream>>>(xDevice, yDevice, zDevice);
    aclrtSynchronizeStream(stream);

    aclrtMemcpy(zHost, totalByteSize, (uint8_t*)zDevice, totalByteSize, ACL_MEMCPY_DEVICE_TO_HOST);
    std::vector<float> z((float *)zHost, (float *)(zHost + totalByteSize));

    aclrtFree(xDevice);
    aclrtFree(yDevice);
    aclrtFree(zDevice);
    aclrtFreeHost(zHost);

    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();

    return z;
}
```

---

## Host 侧启动模板

可复用的 Host 侧 kernel 启动模板：

```cpp
template<typename KernelFunc, typename... Args>
void launchKernel(KernelFunc kernel, uint32_t numBlocks,
                  std::vector<uint8_t*>& hostBuffers,
                  std::vector<size_t>& bufferSizes,
                  std::vector<uint8_t**>& deviceBuffers)
{
    int32_t deviceId = 0;
    aclrtStream stream = nullptr;

    aclInit(nullptr);
    aclrtSetDevice(deviceId);
    aclrtCreateStream(&stream);

    // 分配 device 内存
    for (size_t i = 0; i < hostBuffers.size(); ++i) {
        aclrtMalloc((void**)deviceBuffers[i], bufferSizes[i],
                    ACL_MEM_MALLOC_HUGE_FIRST);
        aclrtMemcpy(*deviceBuffers[i], bufferSizes[i],
                    hostBuffers[i], bufferSizes[i],
                    (i < 2) ? ACL_MEMCPY_HOST_TO_DEVICE
                            : ACL_MEMCPY_DEVICE_TO_HOST);
    }

    // 启动 kernel
    kernel<<<numBlocks, nullptr, stream>>>(/* device pointers */);
    aclrtSynchronizeStream(stream);

    // 读回输出
    // ... 对输出缓冲区执行 aclrtMemcpy DEVICE_TO_HOST ...

    // 清理
    for (auto* db : deviceBuffers) { aclrtFree(*db); }
    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();
}
```

---

## 编译注意事项

- Kernel 源文件使用 `.asc` 扩展名。
- 目标架构：`--npu-arch=dav-2201`（同时适用于 2201 和 3510 平台）。
- 项目语言必须包含 `ASC`。
- Host 侧代码为标准 C++，链接 `libascendcl.so`（ACL 运行时）。
- `asc_get_block_num()` 返回 2 的幂次（通常为 1、2、4 或 8）。
- 所有 UB 数组大小必须为编译期常量（`constexpr`）。
- 数据搬运大小应为 32 字节对齐。
