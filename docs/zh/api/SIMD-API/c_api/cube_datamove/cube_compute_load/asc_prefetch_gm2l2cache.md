# asc_prefetch_gm2l2cache

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

将Global Memory中的数据预加载到L2 Cache中，不实际搬进内核（L1 Buffer/Unified Buffer）。预加载的数据后续被核间指令访问时可命中L2 Cache，从而降低访问延迟。

## 函数原型

```c
__aicore__ inline void asc_prefetch_gm2l2cache(__gm__ <dtype>* src,
                                                uint32_t burst_num,
                                                uint32_t burst_len,
                                                uint64_t burst_src_stride)
```

### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：half类型数据的burst模式预取。
__aicore__ inline void asc_prefetch_gm2l2cache(__gm__ half* src,
                                                uint32_t burst_num,
                                                uint32_t burst_len,
                                                uint64_t burst_src_stride)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 含义 |
| --- | --- | --- |
| src | 输入 | 源操作数，存储位置为GM。起始地址需要按照32字节对齐。 |
| burst_num | 输入 | 预取的连续传输数据块个数。 |
| burst_len | 输入 | 预取的每个连续传输数据块的长度，单位为字节。需满足dtype字节对齐：dtype为`int16_t`、`uint16_t`、`half`、`bfloat16_t`时，burst_len必须为2的倍数；dtype为`int32_t`、`uint32_t`、`float`时，burst_len必须为4的倍数。 |
| burst_src_stride | 输入 | 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。<br>只预取1个数据块，即burst_num=1时，可以将此参数设置为0。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 本接口Cube及Vector核均支持。
- 本接口仅将GM数据预取到L2 Cache，不写入L1 Buffer、Unified Buffer等片上存储单元，也不改变GM中的数据内容。

## 调用示例

将代码保存为`examples.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[`__NPU_ARCH__`](../../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

本示例固定`M=128、K=128`（`int16_t`类型，GM中按行连续存放），先将源矩阵通过本接口预加载到L2 Cache，随后发起GM到L1 Buffer的正常搬运（访问已预加载的GM地址时可命中L2 Cache），数据经UB回传GM后与Host侧原始输入比较，验证预加载不改变数据内容。

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
#include "c_api/asc_simd.h"

namespace {
constexpr uint32_t M = 128, K = 128;
constexpr uint32_t TOTAL = M * K;
constexpr int64_t READY_FLAG = 11;

__global__ __mix__(1, 2) void PrefetchGm2l2cacheKernel(__gm__ int16_t* a, __gm__ int16_t* output)
{
    asc_init();
    __cbuf__ int16_t a_l1[TOTAL];
    __ubuf__ int16_t* output_ub = reinterpret_cast<__ubuf__ int16_t*>(0);
    if ASC_IS_AIC {
        // Prefetch: 将GM中的源矩阵预加载到L2 Cache，不实际搬进L1 Buffer/Unified Buffer。
        // 每行为1个连续传输数据块（128 * 2 = 256字节），矩阵共128行，即128个数据块，
        // 相邻数据块首地址的间隔为256字节
        uint32_t burstNum = M;                          // 数据块个数：128
        uint32_t burstLen = K * sizeof(int16_t);        // 每个数据块长度：256字节
        uint64_t burstSrcStride = K * sizeof(int16_t);  // 相邻数据块间隔：256字节
        asc_prefetch_gm2l2cache(a, burstNum, burstLen, burstSrcStride);

        // CopyIn: GM -> L1，访问已预加载的GM地址时可命中L2 Cache中缓存的数据
        asc_copy_gm2l1_align(a_l1, a, M, K * sizeof(int16_t), 0, 0, false,
            asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, K * sizeof(int16_t), K * sizeof(int16_t));
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

        // CopyOut: L1 -> UB
        asc_copy_l12ub(output_ub, a_l1, 0, 1, TOTAL * sizeof(int16_t) / 32, 0, 0);
        asc_sync_pipe(PIPE_MTE1);
        asc_sync_block_arrive(PIPE_MTE1, READY_FLAG);
    }
    if ASC_IS_AIV {
        asc_sync_block_wait(PIPE_MTE3, READY_FLAG);
        if (asc_get_sub_block_id() == 0)
            asc_copy_ub2gm_align(output, output_ub, TOTAL * sizeof(int16_t));
    }
    asc_sync_pipe(PIPE_ALL);
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
    aclrtMalloc(reinterpret_cast<void**>(&ad), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&od), output.size() * sizeof(int16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(ad, a.size() * sizeof(int16_t), a.data(), a.size() * sizeof(int16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    PrefetchGm2l2cacheKernel<<<1, 0>>>(ad, od);
    const aclError ret = aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int16_t), od,
        output.size() * sizeof(int16_t), ACL_MEMCPY_DEVICE_TO_HOST);
    // 预加载仅将数据缓存到L2 Cache，不改变GM中的数据内容，输出应与原始输入完全一致
    const bool passed = ret == ACL_SUCCESS && output == a;
    std::printf("M=%u K=%u asc_prefetch_gm2l2cache=%s\n", M, K, passed ? "PASS" : "FAIL");
    aclrtFree(ad);
    aclrtFree(od);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
