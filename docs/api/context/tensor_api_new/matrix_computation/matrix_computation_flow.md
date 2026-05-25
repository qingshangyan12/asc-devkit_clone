# 矩阵计算流程

## 功能说明

Tensor API矩阵计算流程由数据搬入、矩阵乘加和结果搬出三部分组成。典型流程为：先将Global Memory中的输入矩阵搬运到L1 Buffer，再将左矩阵搬运到L0A Buffer、右矩阵搬运到L0B Buffer，然后调用`Mmad`将结果写入L0C Buffer，最后将L0C Buffer中的结果搬运到Global Memory或Unified Buffer。

矩阵计算中的A、B、C含义如下：

| 矩阵 | 物理位置 | 逻辑维度 | 说明 |
| :--- | :--- | :--- | :--- |
| A | L0A Buffer | M * K | 左矩阵输入。 |
| B | L0B Buffer | K * N | 右矩阵输入。 |
| C | L0C Buffer | M * N | 矩阵乘加结果。 |

## 典型流程

1. 构造Global Memory、L1 Buffer、L0A Buffer、L0B Buffer、L0C Buffer对应的Tensor API张量。
2. 调用`Copy`将A矩阵和B矩阵从Global Memory搬运到L1 Buffer。
3. 调用`Copy`将A矩阵从L1 Buffer搬运到L0A Buffer，将B矩阵从L1 Buffer搬运到L0B Buffer。
4. 调用`Mmad`完成`C = A * B`，结果写入L0C Buffer。
5. 调用`Copy`将L0C Buffer中的结果搬出到Global Memory或Unified Buffer。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void MatrixComputationFlowExample(
    __gm__ half* gmAAddr, __gm__ half* gmBAddr, __gm__ float* gmCAddr)
{
    using namespace AscendC::Te;

    constexpr uint16_t m = 16;
    constexpr uint16_t n = 16;
    constexpr uint16_t k = 16;

    __cbuf__ half l1ABuf[m * k];
    __cbuf__ half l1BBuf[k * n];
    __ca__ half l0ABuf[m * k];
    __cb__ half l0BBuf[k * n];
    __cc__ float l0CBuf[m * n];

    auto gmA = MakeTensor(
        MakeMemPtr(gmAAddr),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(m, k));
    auto gmB = MakeTensor(
        MakeMemPtr(gmBAddr),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(k, n));
    auto gmC = MakeTensor(
        MakeMemPtr(gmCAddr),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(m, n));

    auto l1A = MakeTensor(
        MakeMemPtr(l1ABuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));
    auto l1B = MakeTensor(
        MakeMemPtr(l1BBuf),
        MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(k, n));
    auto l0A = MakeTensor(
        MakeMemPtr(l0ABuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));
    auto l0B = MakeTensor(
        MakeMemPtr(l0BBuf),
        MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(k, n));
    auto l0C = MakeTensor(
        MakeMemPtr(l0CBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));

    auto gm2L1Atom = MakeCopy(CopyGM2L1{}, CopyGM2L1TraitDefault{});
    auto l12L0AAtom = MakeCopy(CopyL12L0A{}, CopyL12L0ATraitDefault{});
    auto l12L0BAtom = MakeCopy(CopyL12L0B{}, CopyL12L0BTraitDefault{});
    auto mmadAtom = MakeMmad(MmadOperation{}, MmadTraitDefault{});
    auto l0C2GMAtom = MakeCopy(CopyL0C2GM{}, CopyL0C2GMTraitDefault{});

    Copy(gm2L1Atom, l1A, gmA);
    Copy(gm2L1Atom, l1B, gmB);
    Copy(l12L0AAtom, l0A, l1A);
    Copy(l12L0BAtom, l0B, l1B);

    Mmad(mmadAtom.with(MmadParams(m, n, k, 0, true)), l0C, l0A, l0B);
    Copy(l0C2GMAtom.with(FixpipeParams{}), gmC, l0C);
}
```
