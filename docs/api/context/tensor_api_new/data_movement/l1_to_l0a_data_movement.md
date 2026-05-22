# L1到L0A数据搬运

## 产品支持情况

|产品|是否支持|
|--------|--------|
|Ascend 950PR/Ascend 950DT|√|

## 头文件/库文件

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

## 功能说明

该接口用于将L1 Buffer中的矩阵分形数据搬运到L0A Buffer，作为矩阵计算的左矩阵输入。接口支持按布局自动选择普通搬运、转置搬运以及b8/b4转置搬运。传入坐标时，可以从源张量指定坐标处搬运一个子块。

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
|`dst`|输出|目的张量，存储位置为`Location::L0A`。|
|`src`|输入|源张量，存储位置为`Location::L1`。|
|`coord`|输入|可选坐标参数。传入`Coord`时，指定从源张量某个分形坐标开始搬运。|
|`atomCopy`|输入|搬运原子对象，可由`MakeCopy(CopyL12L0A{})`或`MakeCopy(CopyL12L0A{}, CopyL12L0ATraitDefault{})`构造。|

## 返回值说明

`Copy`无返回值。

## 约束说明

- 目的张量必须位于L0A Buffer，源张量必须位于L1 Buffer。
- 普通搬运支持`NZLayoutPtn`到`NZLayoutPtn`。
- 转置搬运支持`ZNLayoutPtn`到`NZLayoutPtn`，b8/b4数据类型会选择b8/b4专用转置路径。
- 普通数据类型支持`half`、`int16_t`、`uint16_t`、`bfloat16_t`、`uint32_t`、`int32_t`、`float`、`uint8_t`、`int8_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`。
- MX ScaleA搬运使用独立的`CopyL12L0ScaleA`，参见[L1到L0ScaleA数据搬运](l1_to_l0scalea_data_movement.md)。
- L0A通常作为`Mmad`的左矩阵输入，后续矩阵计算要求L0A地址和分形排布满足硬件矩阵计算约束。

## 调用示例

```cpp
using namespace AscendC::Te;

auto l1A = MakeTensor(MakeMemPtr<Location::L1, half>(l1Addr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));
auto l0A = MakeTensor(MakeMemPtr<Location::L0A, half>(l0aAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));

auto atom = MakeCopy(CopyL12L0A{});
Copy(atom, l0A, l1A);
atom.Call(l0A, l1A, MakeCoord(0, 0));
```
