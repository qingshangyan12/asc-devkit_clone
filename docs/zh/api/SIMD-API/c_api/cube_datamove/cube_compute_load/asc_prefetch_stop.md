# asc_prefetch_stop

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR&960DT系列产品：支持
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR&950DT系列产品：不支持
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3系列产品：不支持
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2系列产品：不支持
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas训练系列产品：不支持
<!-- end id8 -->

## 功能说明

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

将没有执行的Prefetch指令删除，用于在预加载流程发生变化时清理尚未生效的预加载指令,Cube核与Vector核均支持。

本接口需要与[asc_prefetch_gm2l2cache](asc_prefetch_gm2l2cache.md)、[asc_prefetch_gm2l2cache_dn2nz](asc_prefetch_gm2l2cache_dn2nz.md)、[asc_prefetch_gm2l2cache_nd2nz](asc_prefetch_gm2l2cache_nd2nz.md)等预取接口配合使用。

## 函数原型

```c
__aicore__ inline void asc_prefetch_stop()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 仅删除尚未执行的Prefetch指令，已在执行中的Prefetch指令不受影响。

## 调用示例

将代码保存为`examples.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[`__NPU_ARCH__`](../../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

本示例固定`M=128、K=128`（`int16_t`类型），模拟分tile迭代场景：先预取当前tile数据，随后下发下一轮tile的预取指令（此处用相同地址模拟）；当前轮数据完成GM到L1 Buffer的搬运（命中L2 Cache）后，通过本接口删除尚未执行的下一轮预取指令，避免无效预加载占用搬运带宽和L2 Cache空间。数据经UB回传GM后与Host侧原始输入比较，验证已执行完成的预加载数据不受影响。

<!-- npu="960" id9 -->
以Ascend 960PR&960DT系列产品（对应NPU架构为`dav-9201`）为例，编译运行命令如下：

```bash
bisheng examples.asc -o main --npu-arch=dav-9201 && ./main
```
<!-- end id9 -->

```cpp
#include <cstdint>
#include <cstdio>
#include <vector>
#include "acl/acl.h"
#include "kernel_operator.h"
#include "c_api/cube_datamove/cube_datamove.h"
using namespace AscendC;

namespace {
constexpr uint32_t M = 128, K = 128;
constexpr uint32_t TOTAL = M * K;

__global__ __mix__(1, 2) void PrefetchStopKernel(__gm__ int16_t* a, __gm__ int16_t* output)
{
    __gm__ int16_t* srcGm = a;
    GlobalTensor<int16_t> srcGlobal, cGlobal;
    srcGlobal.SetGlobalBuffer(a, TOTAL);
    cGlobal.SetGlobalBuffer(output, TOTAL);

    TPipe pipe;
    TQue<TPosition::A1, 1> qidA1;
    TQue<TPosition::VECOUT, 1> qidUB;
    pipe.InitBuffer(qidA1, 1, TOTAL * sizeof(int16_t));
    pipe.InitBuffer(qidUB, 1, TOTAL * sizeof(int16_t));

    LocalTensor<int16_t> a_l1 = qidA1.template AllocTensor<int16_t>();
    LocalTensor<int16_t> output_ub = qidUB.template AllocTensor<int16_t>();

    if ASCEND_IS_AIC {
        // Prefetch: 预取当前tile数据后，再下发下一轮tile的预取指令（用相同地址模拟），
        // 每行为1个连续传输数据块（128 * 2 = 256字节），矩阵共128行，即128个数据块，
        // 相邻数据块首地址的间隔为256字节
        uint32_t burstNum = M;                      // 数据块个数：128
        uint32_t burstLen = K * sizeof(int16_t);    // 每个数据块长度：256字节
        uint64_t burstSrcStride = K * sizeof(int16_t);  // 相邻数据块间隔：256字节
        // 预取当前tile数据
        asc_prefetch_gm2l2cache(srcGm, burstNum, burstLen, burstSrcStride);
        // 预加载流程发生变化（如满足终止条件提前退出迭代），
        // 已下发的下一轮tile预取指令不再需要（此处用相同地址模拟下一轮tile的预取）
        asc_prefetch_gm2l2cache(srcGm, burstNum, burstLen, burstSrcStride);

        // CopyIn: GM -> L1，访问已预加载的GM地址时可命中L2 Cache中缓存的数据
        DataCopy(a_l1, srcGlobal, TOTAL);
        pipe_barrier(PIPE_ALL);

        // Stop: 删除尚未执行（未生效）的Prefetch指令，避免无效预加载占用搬运带宽和
        // L2 Cache空间；已在执行中及已执行完成的Prefetch指令不受影响
        asc_prefetch_stop();
        pipe_barrier(PIPE_ALL);

        // CopyOut: L1 -> UB
        copy_cbuf_to_ubuf((__ubuf__ void*)output_ub.GetPhyAddr(), (__cbuf__ void*)a_l1.GetPhyAddr(), 0, 1,
            static_cast<uint16_t>(TOTAL * sizeof(int16_t) / 32), 0, 0);
    }
    SyncAll<false>();
    if (GetSubBlockIdx() == 0) {
        DataCopy(cGlobal, output_ub, TOTAL);
    }
}
} // namespace

int main()
{
    std::vector<int16_t> a(TOTAL), output(TOTAL);
    for (uint32_t r = 0; r < M; ++r) {
        for (uint32_t k = 0; k < K; ++k) {
            a[r * K + k] = static_cast<int16_t>((r * 31 + k * 7) % 1000 - 500);
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    int16_t *ad = nullptr, *od = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&ad), a.size() * sizeof(int16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&od), output.size() * sizeof(int16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(ad, a.size() * sizeof(int16_t), a.data(), a.size() * sizeof(int16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    PrefetchStopKernel<<<1, 0>>>(ad, od);
    const aclError ret = aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int16_t), od,
        output.size() * sizeof(int16_t), ACL_MEMCPY_DEVICE_TO_HOST);
    // 已执行完成的预加载数据不受asc_prefetch_stop影响，输出应与原始输入完全一致
    const bool passed = ret == ACL_SUCCESS && output == a;
    std::printf("M=%u K=%u asc_prefetch_stop=%s\n", M, K, passed ? "PASS" : "FAIL");
    aclrtFree(ad);
    aclrtFree(od);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
