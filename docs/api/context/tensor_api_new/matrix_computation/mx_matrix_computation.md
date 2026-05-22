# MX矩阵计算

## 产品支持情况

|产品|是否支持|
|--------|--------|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

头文件为：#include "tensor_api/tensor.h"

MX矩阵计算是Tensor API中通过`MmadTrait::mmadType`选择的矩阵计算模式，用于低精度MX数据类型的矩阵乘加。该模式仍以L0A Buffer为左矩阵、L0B Buffer为右矩阵、L0ScaleA Buffer/L0ScaleB Buffer为缩放系数矩阵、L0C Buffer为结果矩阵，调用入口与普通`Mmad`一致。

## 函数原型

```cpp
template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor>
__aicore__ inline void Mmad(
    const MmadAtom<AtomType>& atomMmad, const DstTensor& dst, const FmTensor& fm, const FilterTensor& filter)

template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor, typename BiasTensor,
    Std::enable_if_t<IsAttrTensorV<BiasTensor>, int> Enable>
__aicore__ inline void Mmad(
    const MmadAtom<AtomType>& atomMmad, const DstTensor& dst, const FmTensor& fm, const FilterTensor& filter,
    const BiasTensor& bias)

template <typename MmadOperationType>
__aicore__ inline constexpr auto MakeMmad(const MmadOperationType& operation)

template <typename MmadOperationType, typename MmadTraitType>
__aicore__ inline constexpr auto MakeMmad(const MmadOperationType& operation, const MmadTraitType& trait)
```

## 参数说明

|参数名|输入/输出|描述|
|--------|--------|--------|
|`dst`|输出|结果矩阵张量，存储位置为`Location::L0C`，数据类型通常为`float`。|
|`fm`|输入|左矩阵张量，存储位置为`Location::L0A`，数据类型为MX支持的低精度类型。|
|`filter`|输入|右矩阵张量，存储位置为`Location::L0B`，数据类型为MX支持的低精度类型。|
|`params`|输入|矩阵计算参数，类型为`MmadParams`，通过`MakeMmad(MmadOperation{}).with(params)`或`MakeMmad(MmadOperation{}, MmadTraitMX{}).with(params)`绑定到atom。|
|`atomMmad`|输入|使用`MmadTrait`指定`MmadType::MX`的矩阵计算原子对象。|
|`traits`|输入|`MakeMmad`的第二个构造参数，为trait wrapper对象。例如包含`TraitType = MmadTrait`和`value = mxTrait`的`MmadTraitMX`，用于将`mmadType`设置为`MmadType::MX`。|

## 返回值说明

`Mmad`无返回值。`MakeMmad`返回`MmadAtom`对象。

## 约束说明

- 目的张量必须位于L0C Buffer，左矩阵必须位于L0A Buffer，右矩阵必须位于L0B Buffer。
- 支持的数据类型组合包括`float = fp4x2_e2m1_t * fp4x2_e2m1_t`、`float = fp4x2_e2m1_t * fp4x2_e1m2_t`、`float = fp4x2_e1m2_t * fp4x2_e2m1_t`、`float = fp4x2_e1m2_t * fp4x2_e1m2_t`、`float = fp8_e4m3fn_t * fp8_e4m3fn_t`、`float = fp8_e4m3fn_t * fp8_e5m2_t`、`float = fp8_e5m2_t * fp8_e4m3fn_t`、`float = fp8_e5m2_t * fp8_e5m2_t`。
- 带bias场景中，bias位于BiasTable Buffer或L0C Buffer时，bias数据类型需要为`float`。
- MX计算通常需要配合ScaleA和ScaleB数据搬运流程准备scale数据，ScaleA搬运可参考[L1 Buffer到L0ScaleA Buffer数据搬运](../data_movement/l1_to_l0scalea_data_movement.md)，ScaleB搬运可参考[L1 Buffer到L0ScaleB Buffer数据搬运](../data_movement/l1_to_l0scaleb_data_movement.md)。
- `m`、`n`、`k`的取值和张量布局需要满足底层MX矩阵计算指令约束。

## 调用示例

```cpp
using namespace AscendC::Te;

auto l0A = MakeTensor(MakeMemPtr<Location::L0A, fp8_e4m3fn_t>(aAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<fp8_e4m3fn_t>>(m, k));
auto l0B = MakeTensor(MakeMemPtr<Location::L0B, fp8_e5m2_t>(bAddr),
    MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<fp8_e5m2_t>>(k, n));
auto l0C = MakeTensor(MakeMemPtr<Location::L0C, float>(cAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));

MmadParams params(m, n, k, 0, true);
constexpr MmadTrait mxTrait(0, false, false, true, MmadType::MX);
struct MmadTraitMX {
    using TraitType = MmadTrait;
    static constexpr const TraitType value = mxTrait;
};

auto atom = MakeMmad(MmadOperation{}, MmadTraitMX{}).with(params);
Mmad(atom, l0C, l0A, l0B);
```
