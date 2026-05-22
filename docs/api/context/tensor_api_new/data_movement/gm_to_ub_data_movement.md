# GM到UB数据搬运

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

该接口用于将 Global Memory 中的数据搬运到 UB。接口根据源张量和目的张量布局推导搬运参数，支持 ND、DN、NZ 等同布局搬运。

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

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 目的张量，存储位置为 `Location::UB`。 |
| `src` | 输入 | 源张量，存储位置为 `Location::GM`。 |
| `atomCopy` | 输入 | 搬运原子对象，可由 `MakeCopy(CopyGM2UB{})` 或 `MakeCopy(CopyGM2UB{}, CopyGM2UBTraitDefault{})` 构造。 |

## 约束说明

- 目的张量必须位于 UB，源张量必须位于 Global Memory。
- 支持 `NDExtLayoutPtn` 到 `NDExtLayoutPtn`、`DNExtLayoutPtn` 到 `DNExtLayoutPtn`、`NZLayoutPtn` 到 `NZLayoutPtn`。
- 源张量和目的张量的数据类型需要保持一致。
- 支持 fp4 数据搬运，底层按 b8 搬运并按代码逻辑折算搬运长度。
- UB 空间和布局 stride 需要满足 32 字节对齐要求。

## 调用示例

```cpp
using namespace AscendC::Te;

auto gm = MakeTensor(MakeMemPtr<Location::GM, half>(gmAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(m, n));
auto ub = MakeTensor(MakeMemPtr<Location::UB, half>(ubAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(m, n));

auto atom = MakeCopy(CopyGM2UB{});
Copy(atom, ub, gm);
```
