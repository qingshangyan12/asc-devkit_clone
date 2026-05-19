# Ascend 3510 Cube核 C API 示例

展示 3510 Cube核 C API 使用模式的完整可运行示例。
所有示例假定项目已配置 `find_package(ASC REQUIRED)` 且引入了 `#include "c_api/asc_simd.h"`。

---

## Cube核编程模板（单核，单分形）

### Kernel函数

Cube核的 kernel 入口使用 `__cube__ __global__ __aicore__` 标识符。

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

constexpr uint32_t NUM_BLOCKS = 1;   // 单核
constexpr uint32_t M_SIZE = 16;
constexpr uint32_t K_SIZE = 16;
constexpr uint32_t N_SIZE = 16;

/* --------------------------------------------------------------------------
 * Cube核 kernel: 单分形 matrix multiply (half * half -> float)
 * 数据流: GM -> L1 -> L0A/L0B -> MMAD -> L0C -> GM
 * -------------------------------------------------------------------------- */
__cube__ __global__ __aicore__ void cube_mmad_kernel(
    __gm__ half* gm_a, __gm__ half* gm_b, __gm__ float* gm_c)
{
    asc_init();

    // L1 Buffer: 存放从GM搬来的A和B矩阵
    // 对于16x16的half矩阵，需要256个元素 = 512B
    __cbuf__ half l1_a[M_SIZE * K_SIZE];
    __cbuf__ half l1_b[K_SIZE * N_SIZE];

    // L0 Buffers: 计算缓冲
    __ca__ half l0a_a[M_SIZE * K_SIZE];
    __cb__ half l0b_b[K_SIZE * N_SIZE];
    __cc__ float l0c_c[M_SIZE * N_SIZE];

    // 步骤1: GM -> L1 (2D搬运)
    constexpr uint8_t l2_cache_ctl = 0;    // DISABLE模式
    constexpr uint8_t decomp_mode = 0;
    asc_copy_gm2l1_sync(l1_a, gm_a,
                        0, 0, 0, 1, 1,     // m_start=0,k_start=0,dst_stride=0,m_step=1,k_step=1
                        decomp_mode, l2_cache_ctl);

    asc_copy_gm2l1_sync(l1_b, gm_b,
                        0, 0, 0, 1, 1,
                        decomp_mode, l2_cache_ctl);

    // 步骤2: L1 -> L0A, L0B (分形搬运)
    // m_step=1: 搬运1个分形的M方向(16个元素), k_step=1: K方向1个分形(32B=16个half)
    asc_copy_l12l0a_sync(l0a_a, l1_a, 0, 0, 1, 1, 0, 0);
    asc_copy_l12l0b_sync(l0b_b, l1_b, 0, 0, 1, 1, 0, 0);

    // 步骤3: 矩阵乘加 c = a * b
    constexpr uint16_t left_height = M_SIZE;
    constexpr uint16_t n_dim = K_SIZE;
    constexpr uint16_t right_width = N_SIZE;
    constexpr uint8_t unit_flag = 0;       // 关闭unit_flag
    constexpr bool disable_gemv = false;   // 开启GEMV模式
    constexpr bool c_matrix_source = false;
    constexpr bool c_matrix_init_val = true; // C初始化为0

    asc_mmad_sync(l0c_c, l0a_a, l0b_b,
                  left_height, n_dim, right_width,
                  unit_flag, disable_gemv,
                  c_matrix_source, c_matrix_init_val);

    // 步骤4: L0C -> GM (无量化, 直接搬运float结果)
    constexpr uint16_t n_size = 1;         // N方向分形数
    constexpr uint16_t m_size = 1;         // M方向分形数
    constexpr uint32_t loop_dst_stride = 0;
    constexpr uint16_t loop_src_stride = 0;
    constexpr uint8_t clip_relu_pre = 0;
    constexpr uint8_t unit_flag_ctl = 0;
    constexpr uint64_t quant_pre = 0;      // NoQuant
    constexpr uint8_t relu_pre = 0;
    constexpr bool split_en = false;
    constexpr bool NZ2ND_en = false;
    constexpr uint64_t quant_post = 0;     // NoConv
    constexpr uint8_t relu_post = 0;
    constexpr bool clip_relu_post = false;
    constexpr uint8_t eltwise_op = 0;
    constexpr bool eltwise_antq_en = false;
    constexpr bool C0_pad_en = false;
    constexpr bool broadcast_en = false;
    constexpr bool NZ2DN_en = false;

    asc_copy_l0c2gm_sync(
        (__gm__ float*)gm_c, l0c_c,
        n_size, m_size,
        loop_dst_stride, loop_src_stride, l2_cache_ctl,
        clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre,
        split_en, NZ2ND_en, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en,
        C0_pad_en, broadcast_en, NZ2DN_en);
}
```

### Host 侧调用流程

```cpp
std::vector<float> kernel_cube_mmad(
    std::vector<half> &a, std::vector<half> &b)
{
    constexpr uint32_t numBlocks = NUM_BLOCKS;
    uint32_t totalLengthA = a.size();
    uint32_t totalLengthB = b.size();
    uint32_t totalLengthC = M_SIZE * N_SIZE;

    size_t byteSizeA = totalLengthA * sizeof(half);
    size_t byteSizeB = totalLengthB * sizeof(half);
    size_t byteSizeC = totalLengthC * sizeof(float);

    int32_t deviceId = 0;
    aclrtStream stream = nullptr;

    uint8_t *aHost = reinterpret_cast<uint8_t *>(a.data());
    uint8_t *bHost = reinterpret_cast<uint8_t *>(b.data());
    uint8_t *cHost = nullptr;

    half *aDevice = nullptr;
    half *bDevice = nullptr;
    float *cDevice = nullptr;

    aclInit(nullptr);
    aclrtSetDevice(deviceId);
    aclrtCreateStream(&stream);

    aclrtMallocHost((void **)(&cHost), byteSizeC);
    aclrtMalloc((void **)&aDevice, byteSizeA, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&bDevice, byteSizeB, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&cDevice, byteSizeC, ACL_MEM_MALLOC_HUGE_FIRST);

    aclrtMemcpy((uint8_t*)aDevice, byteSizeA, aHost, byteSizeA,
                ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy((uint8_t*)bDevice, byteSizeB, bHost, byteSizeB,
                ACL_MEMCPY_HOST_TO_DEVICE);

    cube_mmad_kernel<<<numBlocks, nullptr, stream>>>(aDevice, bDevice, cDevice);

    aclrtSynchronizeStream(stream);
    aclrtMemcpy(cHost, byteSizeC, (uint8_t*)cDevice, byteSizeC,
                ACL_MEMCPY_DEVICE_TO_HOST);

    std::vector<float> c((float *)cHost,
                         (float *)(cHost + byteSizeC));

    aclrtFree(aDevice);
    aclrtFree(bDevice);
    aclrtFree(cDevice);
    aclrtFreeHost(cHost);

    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();

    return c;
}
```

---

## Host 侧验证与启动模板

```cpp
uint32_t VerifyResult(std::vector<float> &output,
                      std::vector<float> &golden,
                      const char *caseName)
{
    auto printTensor = [](std::vector<float> &tensor, const char *name) {
        constexpr size_t maxPrintSize = 16;
        std::cout << name << ": ";
        for (size_t i = 0; i < std::min(tensor.size(), maxPrintSize); ++i) {
            std::cout << tensor[i] << " ";
        }
        if (tensor.size() > maxPrintSize) {
            std::cout << "...";
        }
        std::cout << std::endl;
    };

    printTensor(output, "Output");
    printTensor(golden, "Golden");

    float maxError = 0.0f;
    for (size_t i = 0; i < golden.size(); ++i) {
        float error = std::abs(output[i] - golden[i]);
        if (error > maxError) {
            maxError = error;
        }
    }

    constexpr float epsilon = 0.001f;
    bool pass = (maxError < epsilon);
    if (pass) {
        std::cout << "[Success] " << caseName
                  << " accuracy verification passed."
                  << " max_error=" << maxError << std::endl;
        return 0;
    } else {
        std::cout << "[Failed] " << caseName
                  << " accuracy verification failed!"
                  << " max_error=" << maxError << std::endl;
        return 1;
    }
}

int32_t main(int32_t argc, char *argv[])
{
    constexpr uint32_t M = M_SIZE;
    constexpr uint32_t K = K_SIZE;
    constexpr uint32_t N = N_SIZE;

    // 初始化输入矩阵 A (M x K) 和 B (K x N)
    std::vector<half> a(M * K);
    std::vector<half> b(K * N);
    for (uint32_t i = 0; i < M * K; ++i) {
        a[i] = half((i % K) * 0.1f);
    }
    for (uint32_t i = 0; i < K * N; ++i) {
        b[i] = half((i % N) * 0.1f);
    }

    // 调用kernel
    std::vector<float> output = kernel_cube_mmad(a, b);

    // 计算CPU真值
    std::vector<float> golden(M * N, 0.0f);
    for (uint32_t i = 0; i < M; ++i) {
        for (uint32_t j = 0; j < N; ++j) {
            float sum = 0.0f;
            for (uint32_t k = 0; k < K; ++k) {
                sum += float(a[i * K + k]) * float(b[k * N + j]);
            }
            golden[i * N + j] = sum;
        }
    }

    return VerifyResult(output, golden, "cube_mmad_half_half_float");
}
```

---

## 编译注意事项

- Kernel 源文件使用 `.asc` 扩展名。
- 目标架构：`--npu-arch=dav-3510`（仅适用于 3510 平台）。
- 项目语言必须包含 `ASC`。
- 仿真模式编译运行命令：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
  ```
- 分形（fractal）概念：每个分形在L0A/L0B中为 512B，half 类型下对应 16x16 个元素。
- Cube核没有UB，所有数据必须经过 L0A/L0B/L0C 缓冲区。

---

## 关键约束汇总

1. **分形对齐**: GM中的数据在2D搬运时，M方向按16元素对齐，K方向按32B对齐
2. **m_step/k_step单位**: m_step 单位为 16 个元素（半个分形高），k_step 单位为 32B
3. **L0C结果大小**: `n_size=1, m_size=1` 表示结果矩阵在N和M方向各1个分形
4. **同步命令替代**: 使用 `_sync` 后缀API可省略显式 `asc_sync_notify/wait` 调用
5. **无UB限制**: Cube核只能使用 `__ca__`, `__cb__`, `__cc__`, `__cbuf__`，不能使用 `__ubuf__`
6. **单核模式**: 测试样例仅考虑单核场景（NUM_BLOCKS=1），无需 `asc_get_block_num()` 分块
