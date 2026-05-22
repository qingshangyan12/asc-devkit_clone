# L1到L0ScaleB数据搬运

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 头文件/库文件

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

## 功能说明

该接口用于将 L1 Buffer 中的 MX ScaleB 系数矩阵搬运到 L0ScaleB。ScaleB 用于 MX 矩阵计算中右矩阵的缩放系数，通常与 `MmadTrait::mmadType = MmadType::MX` 的矩阵计算配合使用。

## 函数原型

```cpp
template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src)

template <typename CopyOperationType>
__aicore__ inline constexpr auto MakeCopy()
```

坐标搬运可通过 `CopyAtom::Call(dst, src, coord)` 调用。

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 目的张量，存储位置为 `Location::L0ScaleB`。 |
| `src` | 输入 | 源张量，存储位置为 `Location::L1`。 |
| `coord` | 输入 | 可选坐标参数。传入 `Coord` 时，从源张量指定坐标开始搬运。 |
| `atomCopy` | 输入 | 搬运原子对象，可由 `MakeCopy<CopyL12L0ScaleB>()` 或 `MakeCopy<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>()` 构造。 |

## 返回值说明

`Copy` 无返回值。`MakeCopy` 返回 `CopyAtom` 对象。

## 约束说明

- 目的张量必须位于 L0ScaleB，源张量必须位于 L1 Buffer。
- 支持的布局组合为 `NNLayoutPtn` 到 `NNLayoutPtn`。
- 数据类型支持 `fp8_e8m0_t`。
- L1 上 ScaleB 的 `NNLayoutPtn` 可由 GM 到 L1 搬运中的 `ScaleBNDLayoutPtn`、`ScaleBDNLayoutPtn` 或 `NNLayoutPtn` 输入准备。

## 调用示例

```cpp
using namespace AscendC::Te;

auto l1ScaleB = MakeTensor(MakeMemPtr<Location::L1, fp8_e8m0_t>(l1ScaleAddr),
    MakeFrameLayout<NNLayoutPtn, LayoutTraitDefault<fp8_e8m0_t>>(k / 32, n));
auto l0ScaleB = MakeTensor(MakeMemPtr<Location::L0ScaleB, fp8_e8m0_t>(l0ScaleAddr),
    MakeFrameLayout<NNLayoutPtn, LayoutTraitDefault<fp8_e8m0_t>>(k / 32, n));

auto atom = MakeCopy<CopyL12L0ScaleB>();
Copy(atom, l0ScaleB, l1ScaleB);
atom.Call(l0ScaleB, l1ScaleB, MakeCoord(0, 0));
```
