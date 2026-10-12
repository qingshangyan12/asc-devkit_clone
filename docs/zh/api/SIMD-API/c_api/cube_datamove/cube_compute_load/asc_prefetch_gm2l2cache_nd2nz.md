# asc_prefetch_gm2l2cache_nd2nz

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

将矩阵数据从Global Memory预加载到L2 Cache中，在此过程中执行ND->NZ/NCHW->NC1HWC0/NCHW->C1HWNC0操作。仅预加载到L2 Cache，不实际搬进L1 Buffer。仅Cube核支持。

GM中的数据以ND类型存储；预加载后在L2 Cache中以NZ形式缓存。
ND->NZ的搬运形式参考[asc_copy_gm2l1_nd2nz](asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz.md)。

## 函数原型

```c
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(__gm__ <dtype>* src,
                                                      uint64_t loop1_src_stride,
                                                      uint16_t n_value,
                                                      uint32_t d_value,
                                                      uint64_t loop4_src_stride)
```

### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：half类型数据的ND2NZ模式预取。
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(__gm__ half* src,
                                                      uint64_t loop1_src_stride,
                                                      uint16_t n_value,
                                                      uint32_t d_value,
                                                      uint64_t loop4_src_stride)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 含义 |
| --- | --- | --- |
| src | 输入 | 源操作数，存储位置为GM。起始地址需要按照1字节对齐。 |
| loop1_src_stride | 输入 | 源矩阵相邻行起始地址之间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 |
| n_value | 输入 | 源矩阵中ND矩阵的行数，单位为元素，取值范围：[1, 65535]。 |
| d_value | 输入 | 源矩阵中ND矩阵的列数，单位为元素，取值范围：[1, $2^{21}-1$]。 |
| loop4_src_stride | 输入 | 源矩阵相邻ND矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。<br>&nbsp;&nbsp;&nbsp;&bull; 源矩阵ND矩阵个数为1时，此参数无意义，设置为0即可。<br>&nbsp;&nbsp;&nbsp;&bull; 源矩阵ND矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复预取源矩阵的第一个ND矩阵。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 本接口仅Cube核支持。
- 本接口仅将GM数据预取到L2 Cache，不写入L1 Buffer等片上存储单元，也不改变GM中的数据内容。

## 调用示例

将代码保存为`examples.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[`__NPU_ARCH__`](../../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

本示例固定`M=128、K=128`（`int16_t`类型），源矩阵在GM中按ND格式存放（128×128（行×列）的行主序矩阵，每行128个元素连续存放）。先将源矩阵通过本接口预加载到L2 Cache（预加载过程中执行ND->NZ格式转换），随后发起GM到L1 Buffer的正常搬运（访问已预加载的GM地址时可命中L2 Cache），数据经UB回传GM后与Host侧原始输入比较，验证预加载不改变数据内容。

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

__global__ __mix__(1, 2) void PrefetchGm2l2cacheNd2nzKernel(__gm__ int16_t* a, __gm__ int16_t* output)
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
        // Prefetch: 将GM中的ND矩阵预加载到L2 Cache，预加载过程中执行ND->NZ格式转换，
        // 不实际搬进L1 Buffer
        // 源矩阵按ND格式存放：128×128（行×列）的行主序矩阵，每行128个元素连续存放
        uint64_t loop1SrcStride = K * sizeof(int16_t);  // ND矩阵相邻行起始地址间隔：256字节
        uint16_t nValue = M;                             // ND矩阵的行数：128
        uint32_t dValue = K;                             // ND矩阵的列数：128
        uint64_t loop4SrcStride = 0;                     // 仅预加载1个ND矩阵，设置为0
        asc_prefetch_gm2l2cache_nd2nz(srcGm, loop1SrcStride, nValue, dValue, loop4SrcStride);

        // CopyIn: GM -> L1，访问已预加载的GM地址时可命中L2 Cache中缓存的数据
        DataCopy(a_l1, srcGlobal, TOTAL);
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
    // 源矩阵按ND格式存放：N维共M行，每行K个D维元素
    for (uint32_t n = 0; n < M; ++n) {
        for (uint32_t d = 0; d < K; ++d) {
            a[n * K + d] = static_cast<int16_t>((n * 31 + d * 7) % 1000 - 500);
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    int16_t *ad = nullptr, *od = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&ad), a.size() * sizeof(int16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&od), output.size() * sizeof(int16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(ad, a.size() * sizeof(int16_t), a.data(), a.size() * sizeof(int16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    PrefetchGm2l2cacheNd2nzKernel<<<1, 0>>>(ad, od);
    const aclError ret = aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int16_t), od,
        output.size() * sizeof(int16_t), ACL_MEMCPY_DEVICE_TO_HOST);
    // 预加载仅将数据缓存到L2 Cache，不改变GM中的数据内容，输出应与原始输入完全一致
    const bool passed = ret == ACL_SUCCESS && output == a;
    std::printf("M=%u K=%u asc_prefetch_gm2l2cache_nd2nz=%s\n", M, K, passed ? "PASS" : "FAIL");
    aclrtFree(ad);
    aclrtFree(od);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
