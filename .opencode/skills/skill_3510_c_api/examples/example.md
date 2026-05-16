# Ascend 3510 C API 示例

展示 3510 C API 使用模式的完整可运行示例。
所有示例假定项目已配置 `find_package(ASC REQUIRED)` 且引入了 `#include "c_api/asc_simd.h"`。

---

## RegBase编程模板

3510架构下的RegBase编程，其处理的对象为RegTensor,即数据从GM搬运到UB,再从UB搬运到寄存器RegTensor,在寄存器中完成运算后,再将结果按原路搬回，使用时应注意函数和变量声明的标识符。

Regbase下的变量说明：
- 所有的`vector_xxx`类型的变量声明只能在`__simd_vf__`函数内
- 所有的`vector_xxx`类型的变量，长度固定为256B, 根据后面的实际类型对该vector进行解释
- `vector_xxx`类型变量的示例，如`vector_float`类型的变量，长度为256B, 即64个float类型的数据
- `vector_xxx`类型变量的示例，如`vector_int8_t`类型的变量，长度为256B, 即256个int8_t类型的数据
- 特殊数据类型为`vector_bool`，其长度固定为32B, 其中每个bit位控制`vector_xxx`类型的1B长度是否参与计算
- 对于数据类型大于1B的`vector_xxx`类型的变量，vector_bool类型的变量中多个bit位对应一个元素，只有最低位生效
- vector_bool类型的变量的示例，如`vector_bool`中前16 bit值为`0001 0001 0001 0000`，其控制`vector_float`类型的变量为前3个元素参与运算，第4个不参与
- vector_bool类型的变量的示例，如`vector_bool`中前16 bit值为`00 01 00 01 00 01 00 00`，其控制`vector_half`类型的变量为前6个元素间隔参与运算，第7, 8个不参与
- `asc_update_mask_b32(uint32_t remaining)` 函数用于根据剩余元素数量`remaining`生成一个`vector_bool`类型的变量，其前`remaining`个b32元素的控制位为`1`, 其余为`0`
- `asc_update_mask_b32(uint32_t remaining)` 函数执行完成后，剩余元素数量`remaining`会减少`256 / sizeof(b32)`
- `asc_update_mask_b32(uint32_t remaining)` 函数中，当remaining小于`256 / sizeof(b32)`时，控制前`remaining`个元素参与运算，剩余元素数量`remaining`会减少到`0`
- `asc_update_mask_b32(uint32_t remaining)` 函数需被赋值给`vector_bool`类型的变量
- `asc_update_mask_b32(uint32_t remaining)` 通常用于处理连续存储数据计算的场景，能适应不同长度的剩余元素数量

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

Host 侧直接调用kernel_add函数，并执行精度比对

```cpp
uint32_t VerifyResult(std::vector<float> &output, std::vector<float> &golden)
{
    auto printTensor = [](std::vector<float> &tensor, const char *name) {
        constexpr size_t maxPrintSize = 20;
        std::cout << name << ": ";
        std::copy(tensor.begin(), tensor.begin() + std::min(tensor.size(), maxPrintSize),
            std::ostream_iterator<float>(std::cout, " "));
        if (tensor.size() > maxPrintSize) {
            std::cout << "...";
        }
        std::cout << std::endl;
    };
    printTensor(output, "Output");
    printTensor(golden, "Golden");
    if (std::equal(golden.begin(), golden.end(), output.begin())) {
        std::cout << "[Success] Case accuracy is verification passed." << std::endl;
        return 0;
    } else {
        std::cout << "[Failed] Case accuracy is verification failed!" << std::endl;
        return 1;
    }
}

int32_t main(int32_t argc, char *argv[])
{
    constexpr uint32_t totalLength = NUM_BLOCKS * TILE_LENGTH;
    std::vector<float> x(totalLength);
    std::vector<float> y(totalLength);

    for (uint32_t i = 0; i < totalLength; ++i) {
        x[i] = i * 0.1f;
        y[i] = i * 0.1f;
    }
    std::vector<float> output = kernel_add(x, y);
    std::vector<float> golden(totalLength);
    for (uint32_t i = 0; i < totalLength; ++i) {
            golden[i] = x[i] + y[i];
    }
    return VerifyResult(output, golden);
}
```

---

## 编译注意事项

- Kernel 源文件使用 `.asc` 扩展名。
- 目标架构：`--npu-arch=dav-3510`（仅适用于 3510 平台）。
- 项目语言必须包含 `ASC`。
- UB上数据搬运大小应为 32 字节对齐。
