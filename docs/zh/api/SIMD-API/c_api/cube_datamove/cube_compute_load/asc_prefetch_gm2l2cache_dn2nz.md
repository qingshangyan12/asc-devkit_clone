# asc_prefetch_gm2l2cache_dn2nz

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

将矩阵数据从Global Memory预加载到L2 Cache中，在此过程中执行DN->NZ/NCHW->NC1HWC0/NCHW->C1HWNC0操作。仅预加载到L2 Cache，不实际搬进L1 Buffer。仅Cube核支持。

GM中的数据以DN类型存储；预加载后在L2 Cache中以NZ形式缓存。DN、NZ等矩阵分形存储格式的定义及转换说明请参见[关键分形格式详解](../../cube_compute/cube_compute_fractal_intro/key_fractal_format_details.md)。

DN->NZ的搬运形式参考[asc_copy_gm2l1_dn2nz](asc_copy_gm2l1_dn2nz.md)。

## 函数原型

```c
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(__gm__ <dtype>* src,
                                                      uint64_t loop1_src_stride,
                                                      uint16_t n_value,
                                                      uint32_t d_value,
                                                      uint64_t loop4_src_stride)
```

### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：half类型数据的DN2NZ模式预取。
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(__gm__ half* src,
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
| loop1_src_stride | 输入 | 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。 |
| n_value | 输入 | 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。 |
| d_value | 输入 | 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。 |
| loop4_src_stride | 输入 | 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。<br>&nbsp;&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。<br>&nbsp;&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复预取源矩阵的第一个DN矩阵。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 本接口仅Cube核支持。
- 本接口仅将GM数据预取到L2 Cache，不写入L1 Buffer等片上存储单元，也不改变GM中的数据内容。

## 调用示例

将代码保存为`examples.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[`__NPU_ARCH__`](../../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

本示例固定`M=128、K=128`（`int16_t`类型），源矩阵在GM中按DN格式存放（D维共128行，每行128个N维元素连续存放，即1个128×128的DN矩阵）。先将源矩阵通过本接口预加载到L2 Cache（预加载过程中执行DN->NZ格式转换，以NZ分形形式缓存），随后通过[asc_copy_gm2l1_dn2nz](asc_copy_gm2l1_dn2nz.md)接口将源矩阵搬入L1 Buffer并转换为NZ格式（读取模式与预取一致，可命中L2 Cache中以NZ分形缓存的数据），数据经UB回传GM后与Host侧按NZ排布生成的参考结果比较，验证预取与搬入的格式转换结果正确。

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

__global__ __mix__(1, 2) void PrefetchGm2l2cacheDn2nzKernel(__gm__ int16_t* a, __gm__ int16_t* output)
{
    asc_init();
    __cbuf__ int16_t a_l1[TOTAL];
    __ubuf__ int16_t* output_ub = reinterpret_cast<__ubuf__ int16_t*>(0);
    if ASC_IS_AIC {
        // Prefetch: 将GM中的DN矩阵预加载到L2 Cache，预加载过程中执行DN->NZ格式转换，
        // 以NZ分形形式缓存，不实际搬进L1 Buffer
        // 源矩阵按DN格式存放：共128行（D维），每行128个N维元素连续存放
        uint64_t loop1SrcStride = M * sizeof(int16_t);  // 同一DN矩阵相邻行起始地址间隔：256字节
        uint16_t nValue = M;                             // DN矩阵的列数：128
        uint32_t dValue = K;                             // DN矩阵的行数：128
        uint64_t loop4SrcStride = 0;                     // 仅预加载1个DN矩阵，设置为0
        asc_prefetch_gm2l2cache_dn2nz(a, loop1SrcStride, nValue, dValue, loop4SrcStride);

        // CopyIn: GM -> L1，通过配套的asc_copy_gm2l1_dn2nz接口执行相同的DN->NZ格式转换搬运，
        // 读取模式与预取一致，可命中L2 Cache中以NZ分形缓存的数据
        asc_set_gm2l1_nz_para(1, 1, M, 0);
        asc_copy_gm2l1_dn2nz(a_l1, a, M * sizeof(int16_t), asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM,
            M, K, 0, false);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

        // CopyOut: L1 -> UB，L1中为NZ格式数据
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
    std::vector<int16_t> a(TOTAL), output(TOTAL), goldenNz(TOTAL);
    // 源矩阵按DN格式存放：D维共K行，每行M个N维元素
    for (uint32_t d = 0; d < K; ++d) {
        for (uint32_t n = 0; n < M; ++n) {
            a[d * M + n] = static_cast<int16_t>((d * 31 + n * 7) % 1000 - 500);
        }
    }
    // 按NZ排布生成参考结果：共(K/16)*(M/16)个16x16分形，分形间按D方向（列方向）外层排列，
    // 分形内为N方向16行 x D方向16列的行主序；逻辑矩阵元素(n, d)在DN存储中位于a[d * M + n]
    for (uint32_t df = 0; df < K / 16; ++df) {
        for (uint32_t nf = 0; nf < M / 16; ++nf) {
            for (uint32_t ni = 0; ni < 16; ++ni) {
                for (uint32_t di = 0; di < 16; ++di) {
                    goldenNz[((df * (M / 16) + nf) * 16 + ni) * 16 + di] =
                        a[(df * 16 + di) * M + nf * 16 + ni];
                }
            }
        }
    }

    aclInit(nullptr);
    aclrtSetDevice(0);
    int16_t *ad = nullptr, *od = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&ad), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&od), output.size() * sizeof(int16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(ad, a.size() * sizeof(int16_t), a.data(), a.size() * sizeof(int16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    PrefetchGm2l2cacheDn2nzKernel<<<1, 0>>>(ad, od);
    const aclError ret = aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int16_t), od,
        output.size() * sizeof(int16_t), ACL_MEMCPY_DEVICE_TO_HOST);
    // 搬入后L1中为NZ格式数据，输出应与按NZ排布生成的参考结果完全一致
    const bool passed = ret == ACL_SUCCESS && output == goldenNz;
    std::printf("M=%u K=%u asc_prefetch_gm2l2cache_dn2nz=%s\n", M, K, passed ? "PASS" : "FAIL");
    aclrtFree(ad);
    aclrtFree(od);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
