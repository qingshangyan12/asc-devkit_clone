# 6.1 快速上手

本章引导你完成第一个基于 TensorAPI 的 Ascend C 算子程序：将 Global Memory 上的输入数据通过 L1 中转，经由 Cube 通路写回到 Global Memory。

## 6.1.1 准备工作

确保开发环境已配置好 Ascend C 工具链，能够编译 `.asc` 后缀的异构源文件。

### 头文件引入

```cpp
#include "include/tensor_api/tensor.h"
using namespace AscendC::Te;
```

引入 `tensor.h` 即引入了所有 TensorAPI 的公开接口。

## 6.1.2 数据搬运程序

以下是一个最简的 GM → L1 → GM 数据搬运示例：

```cpp
__global__ void simple_copy_kernel(
    __gm__ half* input,   // GM 输入指针
    __gm__ half* output,  // GM 输出指针
    int rows, int cols
) {
    // ========== 1. 构造阶段 ==========

    // 1a. 创建内存指针
    auto gmInPtr  = MakeMemPtr<Location::GM>(input);
    auto gmOutPtr = MakeMemPtr<Location::GM>(output);

    // 1b. 创建 Layout (ND 平坦格式)
    auto layout = MakeFrameLayout<NDLayoutPtn>(rows, cols);

    // 1c. 创建 Tensor
    auto gmInTensor  = MakeTensor(gmInPtr,  layout);
    auto gmOutTensor = MakeTensor(gmOutPtr, layout);

    // 1d. 设定 L2 缓存策略
    gmInTensor.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);

    // 1e. 在 L1 上分配缓冲区 (此处假设已由外部分配)
    __cbuf__ half* l1Buffer = /* 已分配的 L1 缓冲区 */;
    auto l1Ptr    = MakeMemPtr<Location::L1>(l1Buffer);
    // L1 上的数据用 ND 格式存储
    auto l1Layout = MakeFrameLayout<NDLayoutPtn>(rows, cols);
    auto l1Tensor = MakeTensor(l1Ptr, l1Layout);

    // ========== 2. 执行阶段 ==========

    // GM → L1 (Cube 通路入口)
    auto gm2l1Atom = MakeCopy(CopyGM2L1{});
    Copy(gm2l1Atom, l1Tensor, gmInTensor);

    // L1 → GM (经 Vector 通路: L1 → UB → GM)
    auto l12ubAtom = MakeCopy(CopyL12UB{});
    // 此处需要 UB 缓冲区... (见完整示例)

    // 简化：直接 UB → GM 搬出
    // auto ub2gmAtom = MakeCopy(CopyUB2GM{});
    // Copy(ub2gmAtom, gmOutTensor, ubTensor);
}
```

## 6.1.3 一个更完整的示例

以下示例展示从 GM 读取数据，经 L1 → UB 进行 Vector 计算，再写回 GM 的完整流程：

```cpp
__global__ void vector_add_kernel(
    __gm__  half*  inputA,
    __gm__  half*  inputB,
    __gm__  half*  output,
    int rows, int cols
) {
    int totalElements = rows * cols;

    // ---- 1. 构造 GM Tensors ----
    auto gmA     = MakeTensor(
        MakeMemPtr<Location::GM>(inputA),
        MakeFrameLayout<NDLayoutPtn>(rows, cols));
    auto gmB     = MakeTensor(
        MakeMemPtr<Location::GM>(inputB),
        MakeFrameLayout<NDLayoutPtn>(rows, cols));
    auto gmOut   = MakeTensor(
        MakeMemPtr<Location::GM>(output),
        MakeFrameLayout<NDLayoutPtn>(rows, cols));

    gmA.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);
    gmB.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);
    gmOut.SetL2CacheHint(CacheMode::CACHE_MODE_LAST);

    // ---- 2. 构造 L1 Tensor ----
    __cbuf__ half* l1BufA = /* 分配 */;
    __cbuf__ half* l1BufB = /* 分配 */;
    auto l1A = MakeTensor(
        MakeMemPtr<Location::L1>(l1BufA),
        MakeFrameLayout<NDLayoutPtn>(rows, cols));
    auto l1B = MakeTensor(
        MakeMemPtr<Location::L1>(l1BufB),
        MakeFrameLayout<NDLayoutPtn>(rows, cols));

    // ---- 3. 数据搬入: GM → L1 ----
    auto gm2l1 = MakeCopy(CopyGM2L1{});
    Copy(gm2l1, l1A, gmA);
    Copy(gm2l1, l1B, gmB);

    // ---- 4. L1 → UB ----
    __ubuf__ half* ubBufA = /* 分配 */;
    __ubuf__ half* ubBufB = /* 分配 */;
    __ubuf__ half* ubBufC = /* 分配 */;
    auto ubA = MakeTensor(
        MakeMemPtr<Location::UB>(ubBufA),
        MakeFrameLayout<NDLayoutPtn>(rows, cols));
    auto ubB = MakeTensor(
        MakeMemPtr<Location::UB>(ubBufB),
        MakeFrameLayout<NDLayoutPtn>(rows, cols));
    auto ubC = MakeTensor(
        MakeMemPtr<Location::UB>(ubBufC),
        MakeFrameLayout<NDLayoutPtn>(rows, cols));

    auto l12ub = MakeCopy(CopyL12UB{});
    Copy(l12ub, ubA, l1A);
    Copy(l12ub, ubB, l1B);

    // ---- 5. Vector 计算 (逐元素加) ----
    // 此处需要调用 Vector 指令 (超出 TensorAPI 范围，由基础 API 提供)
    // 伪代码: VecAdd(ubC, ubA, ubB, totalElements);

    // ---- 6. 数据搬出: UB → GM ----
    auto ub2gm = MakeCopy(CopyUB2GM{});
    Copy(ub2gm, gmOut, ubC);
}
```

## 6.1.4 关键步骤速记

```
GM ──CopyGM2L1──▶ L1 ──CopyL12UB──▶ UB ──[Vector计算]──▶ UB ──CopyUB2GM──▶ GM
│                    │                                   │
│  Cube 通路入口      │  数据中转                        │  Vector 通路出口
└────────────────────┴───────────────────────────────────┘
```

## 6.1.5 常见问题

**Q: 为什么 GM → L1 用 `CopyGM2L1` 而非直接用 `CopyGM2UB`？**

A: `CopyGM2L1` 走 Cube 通路，`CopyGM2UB` 走 Vector 通路。两条通路的硬件资源独立，可以并行。当后续要在 L1 上做 Cube 计算（如 MMAD）时，必须走 Cube 通路；当后续仅做 Vector 计算时，可以直接用 `CopyGM2UB`。

**Q: Layout 应该选 ND 还是 NZ？**

A: GM 上的数据通常用 ND（平坦格式）。L1 上如果要给 Cube 计算用（L0A/L0B），必须用 NZ/ZN 等分形格式。UB 上的数据既可以 ND 也可以 NZ。详见 [第四章 §4.1.9 选择 Layout Pattern 的指南](../04-layout-reference/01-fractal-formats.md#419-选择-layout-pattern-的指南)。

**Q: 运行时报错"Shape 不匹配"？**

A: 检查 Shape 和 Stride 的维度数是否一致。对于分形格式，行和列必须对齐到分形块边界。使用 `Std::ceil_division()` 和 `Std::ceil_align()` 确保对齐。
