# C++ 代码到 AscendC 代码：Tensor 操作转换示例

本文档提供标准 C++ 代码到 AscendC 算子代码的完整转换示例，涵盖常见的 Tensor 操作场景。

---

## 示例 1：向量加法 (Vector Add)

### 标准 C++ 参考实现

```cpp
void vector_add(const float* x, const float* y, float* z, int n) {
    for (int i = 0; i < n; i++) {
        z[i] = x[i] + y[i];
    }
}
```

### AscendC 语言扩展层 C API（指针风格）

```cpp
#include "c_api/asc_simd.h"

__vector__ __global__ __aicore__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    asc_init();

    const uint32_t TILE_LENGTH = 256;
    __ubuf__ float xLocal[TILE_LENGTH];
    __ubuf__ float yLocal[TILE_LENGTH];
    __ubuf__ float zLocal[TILE_LENGTH];

    uint32_t blockLength = TOTAL_LENGTH / asc_get_block_num();

    asc_copy_gm2ub_sync((__ubuf__ void*)xLocal,
        (__gm__ void*)(x + asc_get_block_idx() * blockLength),
        blockLength * sizeof(float));
    asc_copy_gm2ub_sync((__ubuf__ void*)yLocal,
        (__gm__ void*)(y + asc_get_block_idx() * blockLength),
        blockLength * sizeof(float));

    asc_add_sync(zLocal, xLocal, yLocal, blockLength);

    asc_copy_ub2gm_sync((__gm__ void*)(z + asc_get_block_idx() * blockLength),
        (__ubuf__ void*)zLocal,
        blockLength * sizeof(float));
}
```

### AscendC 基础API（C++ Tensor 风格）

```cpp
#include "kernel_operator.h"

class KernelAdd {
public:
    __aicore__ inline KernelAdd() = default;
    __aicore__ inline void Init(GM_ADDR x, GM_ADDR y, GM_ADDR z,
                                 uint32_t totalLength, uint32_t tileNum)
    {
        uint32_t blockLength = totalLength / AscendC::GetBlockNum();
        uint32_t tileLength = blockLength / tileNum;
        xGm.SetGlobalBuffer((__gm__ float*)x + AscendC::GetBlockIdx() * blockLength, blockLength);
        yGm.SetGlobalBuffer((__gm__ float*)y + AscendC::GetBlockIdx() * blockLength, blockLength);
        zGm.SetGlobalBuffer((__gm__ float*)z + AscendC::GetBlockIdx() * blockLength, blockLength);
        this->tileNum = tileNum;
        this->tileLength = tileLength;
    }

    __aicore__ inline void Process()
    {
        AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAlloc;
        AscendC::LocalTensor<float> xLocal = ubAlloc.Alloc<float, TILE_LENGTH>();
        AscendC::LocalTensor<float> yLocal = ubAlloc.Alloc<float, TILE_LENGTH>();
        AscendC::LocalTensor<float> zLocal = ubAlloc.Alloc<float, TILE_LENGTH>();

        for (uint32_t i = 0; i < tileNum; i++) {
            AscendC::DataCopy(xLocal, xGm[i * tileLength], tileLength);
            AscendC::DataCopy(yLocal, yGm[i * tileLength], tileLength);
            AscendC::Add(zLocal, xLocal, yLocal, tileLength);
            AscendC::DataCopy(zGm[i * tileLength], zLocal, tileLength);
        }
    }

private:
    AscendC::GlobalTensor<float> xGm, yGm, zGm;
    uint32_t tileLength, tileNum;
};

__global__ __vector__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z,
                                       AddCustomTilingData tiling)
{
    KernelAdd op;
    op.Init(x, y, z, tiling.totalLength, tiling.tileNum);
    op.Process();
}
```

### AscendC SIMT API（线程风格，Ascend 950）

```cpp
__global__ void add_custom(float* x, float* y, float* z, uint64_t totalLength)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= totalLength) return;
    z[idx] = x[idx] + y[idx];
}
```

---

## 示例 2：向量标量乘法 (Scalar Multiply)

### 标准 C++ 参考实现

```cpp
void scalar_mul(const float* x, float* y, float alpha, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = x[i] * alpha;
    }
}
```

### AscendC 基础API

```cpp
__aicore__ inline void ScalarMulKernel()
{
    // ... 初始化 xLocal, yLocal ...
    for (uint32_t i = 0; i < loopCount; i++) {
        AscendC::DataCopy(xLocal, xGm[i * tileLength], tileLength);
        AscendC::Muls(yLocal, xLocal, alpha, tileLength);   // 核心运算
        AscendC::DataCopy(yGm[i * tileLength], yLocal, tileLength);
    }
}
```

### AscendC C API

```cpp
asc_muls_sync(yLocal, xLocal, alpha, length);
```

---

## 示例 3：ReLU 激活函数

### 标准 C++ 参考实现

```cpp
void relu(const float* x, float* y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = (x[i] > 0.0f) ? x[i] : 0.0f;
    }
}
```

### AscendC 基础API（使用 Relu 专用接口）

```cpp
#include "kernel_operator.h"

__aicore__ inline void ReluKernel()
{
    // ... 初始化 ...
    for (uint32_t i = 0; i < loopCount; i++) {
        AscendC::DataCopy(xLocal, xGm[i * tileLength], tileLength);
        AscendC::Relu(yLocal, xLocal, tileLength);   // 专用 ReLU API
        AscendC::DataCopy(yGm[i * tileLength], yLocal, tileLength);
    }
}
```

### AscendC 基础API（使用通用条件选择 Where）

```cpp
// 等价实现：Where(condition ? a : b)
AscendC::LocalTensor<float> zeroLocal = ubAlloc.Alloc<float, TILE_LENGTH>();
AscendC::Muls(zeroLocal, zeroLocal, 0.0f, TILE_LENGTH);  // 全零Tensor
AscendC::Where(yLocal, xLocal, zeroLocal, xLocal, TILE_LENGTH);
```

### AscendC C API

```cpp
asc_relu_sync(yLocal, xLocal, length);
```

---

## 示例 4：矩阵乘法 (MatMul)

### 标准 C++ 参考实现

```cpp
void matmul(const float* A, const float* B, float* C, int M, int N, int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0f;
            for (int k = 0; k < K; k++) {
                sum += A[i * K + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}
```

### AscendC 基础API（使用 Cube 单元）

```cpp
#include "kernel_operator.h"

__aicore__ inline void MatmulKernel()
{
    // 使用 AscendC 的 Matmul 高阶 API
    // A[NZ格式在UB], B[NZ格式在UB], C[结果在UB]
    // 注意：矩阵乘需要将数据转换为 NZ 格式

    AscendC::LocalTensor<float> aNz, bNz, cNz;

    // 数据搬运 + NZ 格式转换
    AscendC::DataCopyPad(aNz, aNd);   // ND → NZ
    AscendC::DataCopyPad(bNz, bNd);   // ND → NZ

    // Cube 矩阵乘法
    AscendC::Matmul(cNz, aNz, bNz);

    // NZ → ND 格式还原
    AscendC::DataCopyPad(cNd, cNz);

    // 注意：实际使用中需要根据 M, N, K 进行分块（Tiling）
}
```

### NZ 格式要点

```
标准ND格式: A[M][K] 按行存储
NZ格式:     将矩阵按照硬件Cube单元要求重排分块
            需要 DataCopyPad 进行格式转换
            搬运过程中自动完成，高效无额外开销
```

---

## 示例 5：规约求和 (Reduce Sum)

### 标准 C++ 参考实现

```cpp
float reduce_sum(const float* x, int n) {
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += x[i];
    }
    return sum;
}
```

### AscendC 基础API

```cpp
__aicore__ inline void ReduceSumKernel()
{
    // ... 初始化 ...

    // 方案1：使用 Reduce API
    AscendC::ReduceSum<float>(sumScalar, xLocal, tileLength);

    // 方案2：分步规约（大尺寸数据）
    for (uint32_t i = 0; i < loopCount; i++) {
        AscendC::DataCopy(xLocal, xGm[i * tileLength], tileLength);
        AscendC::ReduceSum<float>(partialSum[i], xLocal, tileLength);
    }
    // 再对 partialSum[] 做最终规约
}
```

### 规约策略

```
大尺寸数据规约：
    1. 分块搬运到 UB
    2. 每块内做 ReduceSum → 得到各块部分和
    3. 对各块部分和做最终 ReduceSum → 得到总和
```

---

## 示例 6：Softmax

### 标准 C++ 参考实现

```cpp
void softmax(const float* x, float* y, int n) {
    // 1. 找最大值（数值稳定）
    float maxVal = x[0];
    for (int i = 1; i < n; i++) maxVal = std::max(maxVal, x[i]);

    // 2. exp(x - maxVal) 并求和
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        y[i] = std::exp(x[i] - maxVal);
        sum += y[i];
    }

    // 3. 归一化
    for (int i = 0; i < n; i++) {
        y[i] /= sum;
    }
}
```

### AscendC 基础API

```cpp
__aicore__ inline void SoftmaxKernel()
{
    AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAlloc;
    AscendC::LocalTensor<float> xLocal = ubAlloc.Alloc<float, TILE_LENGTH>();
    AscendC::LocalTensor<float> tmp = ubAlloc.Alloc<float, TILE_LENGTH>();
    AscendC::LocalTensor<float> yLocal = ubAlloc.Alloc<float, TILE_LENGTH>();

    // 1. 搬运数据
    AscendC::DataCopy(xLocal, xGm, tileLength);

    // 2. 找最大值
    float maxVal;
    AscendC::ReduceMax<float>(maxVal, xLocal, tileLength);

    // 3. x - maxVal → tmp
    AscendC::Adds(tmp, xLocal, -maxVal, tileLength);

    // 4. exp → yLocal
    AscendC::Exp(yLocal, tmp, tileLength);

    // 5. 求和
    float sum;
    AscendC::ReduceSum<float>(sum, yLocal, tileLength);

    // 6. 归一化：yLocal / sum
    float invSum = 1.0f / sum;
    AscendC::Muls(yLocal, yLocal, invSum, tileLength);

    // 7. 写回 GM
    AscendC::DataCopy(yGm, yLocal, tileLength);
}
```

---

## 示例 7：向量归一化 (LayerNorm)

### 标准 C++ 参考实现

```cpp
void layer_norm(const float* x, float* y, int n, float eps = 1e-5f) {
    // 1. 求均值
    float mean = 0.0f;
    for (int i = 0; i < n; i++) mean += x[i];
    mean /= n;

    // 2. 求方差
    float var = 0.0f;
    for (int i = 0; i < n; i++) {
        float diff = x[i] - mean;
        var += diff * diff;
    }
    var = var / n + eps;

    // 3. 归一化
    float invStd = 1.0f / std::sqrt(var);
    for (int i = 0; i < n; i++) {
        y[i] = (x[i] - mean) * invStd;
    }
}
```

### AscendC 基础API

```cpp
__aicore__ inline void LayerNormKernel()
{
    AscendC::LocalTensor<float> xLocal = ubAlloc.Alloc<float, TILE_LENGTH>();
    AscendC::LocalTensor<float> tmp1 = ubAlloc.Alloc<float, TILE_LENGTH>();
    AscendC::LocalTensor<float> tmp2 = ubAlloc.Alloc<float, TILE_LENGTH>();

    AscendC::DataCopy(xLocal, xGm, length);

    // 1. 求均值
    float mean;
    AscendC::ReduceSum<float>(mean, xLocal, length);
    mean /= (float)length;

    // 2. x - mean, 平方，求方差
    AscendC::Adds(tmp1, xLocal, -mean, length);     // tmp1 = x - mean
    AscendC::Mul(tmp2, tmp1, tmp1, length);          // tmp2 = (x-mean)^2
    float var;
    AscendC::ReduceSum<float>(var, tmp2, length);
    var = var / (float)length + eps;

    // 3. 归一化: y = (x - mean) / sqrt(var)
    float invStd = 1.0f / AscendC::Sqrt(var);
    AscendC::Muls(yLocal, tmp1, invStd, length);     // 复用 tmp1

    AscendC::DataCopy(yGm, yLocal, length);
}
```

---

## 示例 8：GELU 激活函数

### 标准 C++ 参考实现

```cpp
void gelu(const float* x, float* y, int n) {
    const float c = std::sqrt(2.0f / M_PI);
    for (int i = 0; i < n; i++) {
        float v = x[i];
        y[i] = 0.5f * v * (1.0f + std::tanh(c * (v + 0.044715f * v * v * v)));
    }
}
```

### AscendC 基础API

```cpp
__aicore__ inline void GeluKernel()
{
    const float C0 = 0.044715f;
    const float C1 = 0.7978845608028654f;  // sqrt(2/PI)
    const float C2 = 0.5f;

    AscendC::LocalTensor<float> xLocal = ubAlloc.Alloc<float, TILE_LENGTH>();
    AscendC::LocalTensor<float> t1 = ubAlloc.Alloc<float, TILE_LENGTH>();
    AscendC::LocalTensor<float> t2 = ubAlloc.Alloc<float, TILE_LENGTH>();

    AscendC::DataCopy(xLocal, xGm, length);

    // t1 = x^3
    AscendC::Mul(t1, xLocal, xLocal, length);
    AscendC::Mul(t1, t1, xLocal, length);
    // t1 = C0 * x^3
    AscendC::Muls(t1, t1, C0, length);
    // t1 = x + C0 * x^3
    AscendC::Add(t1, t1, xLocal, length);
    // t1 = C1 * (x + C0 * x^3)
    AscendC::Muls(t1, t1, C1, length);
    // t1 = tanh(...)
    AscendC::Tanh(t1, t1, length);
    // t1 = 1 + tanh(...)
    AscendC::Adds(t1, t1, 1.0f, length);
    // y = 0.5 * x * (1 + tanh(...))
    AscendC::Mul(yLocal, xLocal, t1, length);
    AscendC::Muls(yLocal, yLocal, C2, length);

    AscendC::DataCopy(yGm, yLocal, length);
}
```

---

## 关键转换规则总结

| C++ 模式 | AscendC 基础API | AscendC C API |
|----------|----------------|---------------|
| `float* arr = new float[n]` | `LocalTensor<float> arr = ubAlloc.Alloc<float, n>()` | `__ubuf__ float arr[n]` |
| `arr[i]` (标量访问) | `arr.GetValue(i)` / `arr.SetValue(i, val)` | `arr[i]` (C API 支持下标) |
| `for(i) { a[i] + b[i] }` | `AscendC::Add(dst, a, b, n)` | `asc_add_sync(dst, a, b, n)` |
| `memcpy(dst, src, n)` | `AscendC::DataCopy(dst, src, n)` | `asc_copy_gm2ub_sync(...)` |
| `sum += a[i]` | `AscendC::ReduceSum<T>(result, a, n)` | `asc_reduce_sum_sync(...)` |
| 需要同步控制 | `SetFlag`/`WaitFlag`/`PipeBarrier` | `_sync` 后缀函数自动同步 |

> **核心原则**：AscendC 中**所有操作都是向量化的**——一次 API 调用即完成整个 Tile 的计算。不需要编写逐元素的 for 循环（那是 C++ 宿主代码的模式）。这是从 C++ 到 AscendC 编程思维转换的关键。
