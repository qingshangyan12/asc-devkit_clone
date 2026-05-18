# 矩阵计算流程

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：`#include "tensor_api/tensor.h"`

Tensor API矩阵计算流程由数据搬运、矩阵乘加和结果搬出三部分组成。典型流程为：先将Global Memory中的输入矩阵搬运到L1 Buffer，再将左矩阵搬运到L0A Buffer、右矩阵搬运到L0B Buffer，然后调用`Mmad`将结果写入L0C Buffer，最后将L0C结果搬运到GM或UB。

矩阵计算中的A、B、C含义如下：

| 矩阵 | Tensor API张量 | 物理位置 | 逻辑维度 | 常用布局 |
| :--- | :--- | :--- | :--- | :--- |
| A | `fm` | `Location::L0A` | M * K | `NZLayoutPtn` |
| B | `filter` | `Location::L0B` | K * N | `ZNLayoutPtn` |
| C | `dst` | `Location::L0C` | M * N | `NZLayoutPtn` |
| Bias | `bias` | `Location::BIAS`或`Location::L0C` | 1 * N或M * N | `NDLayoutPtn` / `NZLayoutPtn` |

矩阵A、B、C在计算中的逻辑关系和物理位置如下图所示：

![Mmad矩阵计算示意图](../../figures/mmad_matrix_layout.png)

## 典型流程

1. 使用`MakeMemPtr`标识GM、L1、L0A、L0B、L0C等存储位置。
2. 使用`MakeFrameLayout`构造ND、DN、NZ、ZN等布局。
3. 使用`MakeTensor`构造Tensor API张量。
4. 使用`MakeCopy`构造搬运原子对象，并调用`Copy`完成GM -> L1、L1 -> L0A、L1 -> L0B等数据搬运。
5. 可选：使用`Copy`完成L1 -> BiasTable Buffer的bias搬运。
6. 使用`MakeMmad`构造矩阵计算原子对象，并通过`.with(MmadParams)`配置矩阵计算参数。
7. 使用`Mmad`完成矩阵乘加计算。
8. 使用`Copy`完成L0C -> GM或L0C -> UB的结果搬出。

## 相关接口原型

**矩阵计算接口：**

```cpp
template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor>
__aicore__ inline void Mmad(
    const MmadAtom<AtomType>& atomMmad, const DstTensor& dst, const FmTensor& fm, const FilterTensor& filter);

template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor, typename BiasTensor,
    Std::enable_if_t<IsAttrTensorV<BiasTensor>, int> Enable>
__aicore__ inline void Mmad(
    const MmadAtom<AtomType>& atomMmad, const DstTensor& dst, const FmTensor& fm, const FilterTensor& filter,
    const BiasTensor& bias);

template <typename MmadOperationType>
__aicore__ inline constexpr auto MakeMmad(const MmadOperationType& mmadOperation);

template <typename MmadOperationType, typename MmadTraitType>
__aicore__ inline constexpr auto MakeMmad(
    const MmadOperationType& mmadOperation, const MmadTraitType& mmadTrait);
```

**数据搬运接口：**

```cpp
template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src);

template <typename AtomType, typename DstTensor, typename SrcTensor, typename QuantParam,
    Std::enable_if_t<IsCopyQuantParamV<QuantParam>, int> Enable>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src,
    const QuantParam& quant);

template <typename CopyOperationType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation);

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto MakeCopy(
    const CopyOperationType& copyOperation, const CopyTraitType& copyTrait);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomMmad` | 输入 | 矩阵计算原子对象，由`MakeMmad`构造。需要配置`MmadParams`时，通过`.with(params)`生成携带参数的原子对象。 |
| `atomCopy` | 输入 | 搬运原子对象，由`MakeCopy`构造。 |
| `mmadOperation` | 输入 | 矩阵计算操作对象，普通矩阵计算使用`MmadOperation{}`。 |
| `mmadTrait` | 输入 | 矩阵计算Trait对象，用于指定矩阵计算特性，普通矩阵计算默认使用`MmadTraitDefault{}`。 |
| `copyOperation` | 输入 | 搬运操作对象，例如L1 -> L0A使用`CopyL12L0A{}`，L1 -> L0B使用`CopyL12L0B{}`。 |
| `copyTrait` | 输入 | 搬运Trait对象，用于指定搬运特性，例如`CopyL12L0ATraitDefault{}`、`CopyL12L0BTraitDefault{}`。 |
| `dst` | 输出 | `Mmad`的目的张量位于L0C；`Copy`的目的张量由搬运方向决定。 |
| `fm` | 输入 | 左矩阵A，存储位置必须为L0A。 |
| `filter` | 输入 | 右矩阵B，存储位置必须为L0B。 |
| `bias` | 输入 | 可选bias张量，存储位置为BiasTable Buffer或L0C。 |
| `src` | 输入 | `Copy`的源张量。 |
| `quant` | 输入 | 可选量化参数，可为标量或Tensor API张量。 |

## 返回值说明

`Copy`和`Mmad`无返回值。`MakeCopy`返回`CopyAtom`对象，`MakeMmad`返回`MmadAtom`对象。

## 数据类型

普通矩阵计算和MX矩阵计算支持的数据类型组合不同：

- 普通矩阵计算支持`int32_t = int8_t * int8_t`，以及`float = half/float/bfloat16_t/fp8/hifloat8_t`相关组合。
- MX矩阵计算支持`float = fp4/fp8`相关组合。
- 具体类型组合参考[普通矩阵计算](normal_matrix_computation.md)和[MX矩阵计算](mx_matrix_computation.md)。

## 约束说明

- 矩阵计算的左矩阵必须位于L0A，右矩阵必须位于L0B，结果矩阵必须位于L0C。
- 调用`Copy`和`Mmad`时需要显式传入对应的Atom对象。
- 普通矩阵计算中，左矩阵常用`NZLayoutPtn`，右矩阵常用`ZNLayoutPtn`，结果矩阵常用`NZLayoutPtn`。
- `MmadParams`中的`m`、`n`、`k`需要与参与计算的矩阵尺寸一致。
- 如果需要bias输入，bias张量需要位于`Location::BIAS`或`Location::L0C`。
- L1 -> L0A、L1 -> L0B搬运需要满足分形、地址对齐和转置约束。
- L0C结果搬出时，应根据目标数据类型选择L0C -> GM或L0C -> UB搬运，并传入符合要求的`FixpipeParams`。
- 使用`unitFlag`时，需要同时考虑`MmadParams::unitFlag`和结果搬出`FixpipeParams::unitFlag`的配合。

## 流水类型

典型矩阵计算流程涉及如下流水：

| 阶段 | 常用接口 | 流水类型 |
| :--- | :--- | :--- |
| GM -> L1 | `Copy` | `PIPE_MTE2` |
| L1 -> L0A/L0B | `Copy` | `PIPE_MTE1` |
| L0A/L0B -> L0C | `Mmad` | `PIPE_M` |
| L0C -> GM/UB | `Copy` | `PIPE_FIX` |

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void MatrixComputationFlowExample()
{
    using namespace AscendC::Te;

    constexpr uint16_t m = 16;
    constexpr uint16_t n = 16;
    constexpr uint16_t k = 16;

    __ca__ half l0aBuf[m * k];
    __cb__ half l0bBuf[k * n];
    __cc__ float l0cBuf[m * n];

    auto l0A = MakeTensor(
        MakeMemPtr(l0aBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));
    auto l0B = MakeTensor(
        MakeMemPtr(l0bBuf),
        MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(k, n));
    auto l0C = MakeTensor(
        MakeMemPtr(l0cBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));

    MmadParams params(m, n, k, 0, true);
    auto mmadAtom = MakeMmad(MmadOperation{}, MmadTraitDefault{}).with(params);
    Mmad(mmadAtom, l0C, l0A, l0B);
}
```

## 关键特性说明

### K方向分块累加

矩阵乘通常会沿K方向分块计算。第一次K分块计算时，可将`MmadParams::cmatrixInitVal`设置为`true`；后续K分块需要累加到同一个C矩阵时，将其设置为`false`。

### Bias初始化

需要bias时，可先将bias搬运到BiasTable Buffer，再调用带bias的`Mmad`；也可以使用L0C张量作为bias输入。带bias调用中，Tensor API根据bias的位置确定C矩阵初始值来源。

### 结果搬出

`Mmad`的结果保存在L0C Buffer中。写回GM或UB时，需要使用L0C -> GM或L0C -> UB搬运接口；如果需要类型转换、量化、Relu或channel split，则通过对应`FixpipeTrait`、`FixpipeParams`或量化参数配置。
