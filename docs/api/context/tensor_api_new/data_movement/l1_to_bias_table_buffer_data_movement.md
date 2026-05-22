# L1 Buffer到BiasTable Buffer数据搬运

## 产品支持情况

|产品|是否支持|
|--------|--------|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

头文件为：#include "tensor_api/tensor.h"

该接口用于将L1 Buffer中的数据搬运到BiasTable Buffer。BiasTable Buffer可作为矩阵计算中bias输入的物理位置，用于对L0C Buffer的初始矩阵或偏置项进行处理。

## 函数原型

```cpp
template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src)

template <typename CopyOperationType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation)

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation, const CopyTraitType& copyTrait)
```

## 参数说明

|参数名|输入/输出|描述|
|--------|--------|--------|
|`dst`|输出|目的张量，存储位置为`Location::BIAS`。|
|`src`|输入|源张量，存储位置为`Location::L1`。|
|`atomCopy`|输入|搬运原子对象，可由`MakeCopy(CopyL12BT{})`或`MakeCopy(CopyL12BT{}, CopyL12BTTraitDefault{})`构造。|

## 返回值说明

`Copy`无返回值。

## 约束说明

- 目的张量必须位于BiasTable Buffer，源张量必须位于L1 Buffer。
- 支持的数据类型组合包括`float`目的和`bfloat16_t`源、`float`目的和`half`源、`float`目的和`float`源、`int32_t`目的和`int32_t`源。
- 常用布局为`NDExtLayoutPtn`到`NDExtLayoutPtn`。
- 作为`Mmad` bias输入时，bias张量维度需要与矩阵计算的N方向匹配。

## 调用示例

```cpp
using namespace AscendC::Te;

auto l1Bias = MakeTensor(MakeMemPtr<Location::L1, float>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(1, n));
auto bias = MakeTensor(MakeMemPtr<Location::BIAS, float>(biasAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(1, n));

auto atom = MakeCopy(CopyL12BT{});
Copy(atom, bias, l1Bias);
```
