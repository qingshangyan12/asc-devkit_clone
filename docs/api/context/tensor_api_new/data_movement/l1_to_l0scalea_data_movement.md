# L1 Buffer到L0ScaleA Buffer数据搬运

## 产品支持情况

|产品|是否支持|
|--------|--------|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

头文件为：#include "tensor_api/tensor.h"

该接口用于将L1 Buffer中的MX ScaleA系数矩阵搬运到L0ScaleA Buffer。ScaleA用于MX矩阵计算中左矩阵的缩放系数，通常与`MmadTrait::mmadType = MmadType::MX`的矩阵计算配合使用。

## 函数原型

```cpp
template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src)

template <typename CopyOperationType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation)

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation, const CopyTraitType& copyTrait)
```

坐标搬运可通过`CopyAtom::Call(dst, src, coord)`调用。

## 参数说明

|参数名|输入/输出|描述|
|--------|--------|--------|
|`dst`|输出|目的张量，存储位置为`Location::L0ScaleA`。|
|`src`|输入|源张量，存储位置为`Location::L1`。|
|`coord`|输入|可选坐标参数。传入`Coord`时，从源张量指定坐标开始搬运。|
|`atomCopy`|输入|搬运原子对象，可由`MakeCopy(CopyL12L0ScaleA{})`或`MakeCopy(CopyL12L0ScaleA{}, CopyL12L0ScaleATraitDefault{})`构造。|

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 约束说明

- 目的张量必须位于L0ScaleA Buffer，源张量必须位于L1 Buffer。
- 支持的布局组合为`ZZLayoutPtn`到`ZZLayoutPtn`。
- 数据类型支持`fp8_e8m0_t`。
- L1 Buffer上ScaleA的`ZZLayoutPtn`可由Global Memory到L1 Buffer搬运中的`ScaleANDLayoutPtn`、`ScaleADNLayoutPtn`或`ZZLayoutPtn`输入准备。

## 调用示例

```cpp
using namespace AscendC::Te;

auto l1ScaleA = MakeTensor(MakeMemPtr<Location::L1, fp8_e8m0_t>(l1ScaleAddr),
    MakeFrameLayout<ZZLayoutPtn, LayoutTraitDefault<fp8_e8m0_t>>(m, k / 32));
auto l0ScaleA = MakeTensor(MakeMemPtr<Location::L0ScaleA, fp8_e8m0_t>(l0ScaleAddr),
    MakeFrameLayout<ZZLayoutPtn, LayoutTraitDefault<fp8_e8m0_t>>(m, k / 32));

auto atom = MakeCopy(CopyL12L0ScaleA{});
Copy(atom, l0ScaleA, l1ScaleA);
atom.Call(l0ScaleA, l1ScaleA, MakeCoord(0, 0));
```
