# 矩阵计算流程

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

矩阵计算流程由数据搬运和矩阵乘加两部分组成。通常先通过 GM 到 L1 的 `Copy` 将 Global Memory 中的输入矩阵搬运到 L1，再将左矩阵搬运到 L0A、右矩阵搬运到 L0B，然后调用 `Mmad` 将结果写入 L0C，最后将 L0C 结果搬运到 GM 或 UB。

GM 到 L1 搬运是矩阵计算输入准备的第一段 Copy 通路，支持格式保持的数据连续搬入、ND/DN 到 NZ/ZN 的随路格式转换搬入以及 Scale 数据搬入。详细接口约束和 Layout 组合可参考 [GM到L1数据搬运](../data_movement/gm_to_l1_data_movement.md)。

典型流程如下。

1. 使用 `MakeMemPtr` 标识 GM、L1、L0A、L0B、L0C 等存储位置。
2. 使用 `MakeFrameLayout` 构造 ND、NZ、ZN 等布局。
3. 使用 `MakeTensor` 构造张量。
4. 使用 `Copy` 完成 GM 到 L1、L1 到 L0A、L1 到 L0B 的数据搬运。GM 到 L1 阶段需要先构造对应 Copy 原子对象，再根据 GM/L1 Tensor 的 Layout 自动选择连续搬运或随路格式转换。
5. 使用 `Mmad` 完成矩阵乘加计算。
6. 使用 `Copy` 完成 L0C 到 GM 或 L0C 到 UB 的结果搬运。

## 函数原型

```cpp
template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor>
__aicore__ inline void
Mmad(const MmadAtom<AtomType>& atomMmad, const DstTensor& dst, const FmTensor& fm, const FilterTensor& filter)
```

```cpp
template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void
Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomMmad` | 输入 | 矩阵乘加原子对象。 |
| `atomCopy` | 输入 | 数据搬运原子对象。 |
| `dst` | 输出 | `Mmad` 或 `Copy` 的目的张量。 |
| `fm` | 输入 | 左矩阵张量，存储位置为 L0A。 |
| `filter` | 输入 | 右矩阵张量，存储位置为 L0B。 |
| `src` | 输入 | `Copy` 的源张量。 |

## 返回值说明

`Copy` 和 `Mmad` 无返回值。

## 约束说明

- 矩阵计算的左矩阵必须位于 L0A，右矩阵必须位于 L0B，结果矩阵必须位于 L0C。
- 输入矩阵位于 Global Memory 时，需要先通过 GM 到 L1 的 `Copy` 搬入 L1 Buffer，再从 L1 搬入 L0A 或 L0B。
- 普通矩阵计算中，左矩阵常用 `NZLayoutPtn`，右矩阵常用 `ZNLayoutPtn`，结果矩阵常用 `NZLayoutPtn`。
- GM 到 L1 搬入时，A 矩阵可通过 `NDExtLayoutPtn/NDLayoutPtn -> NZLayoutPtn` 直接搬入并转换为 L1 上的 NZ 布局，B 矩阵可通过 `NDExtLayoutPtn/NDLayoutPtn -> ZNLayoutPtn` 直接搬入并转换为 L1 上的 ZN 布局。
- `MmadParams` 中的 `m`、`n`、`k` 需要与参与计算的矩阵尺寸一致。
- 如果需要 bias 输入，bias 张量需要位于 BiasTable Buffer 或 L0C。
- 计算结果搬出时，应根据目标数据类型选择 L0C 到 GM 或 L0C 到 UB 搬运，并传入符合要求的 `FixpipeParams`。

## 调用示例

```cpp
using namespace AscendC::Te;

auto gmA = MakeTensor(MakeMemPtr(gmAAddr),
    MakeFrameLayout<NDExtLayoutPtn>(m, k));
auto gmB = MakeTensor(MakeMemPtr(gmBAddr),
    MakeFrameLayout<NDExtLayoutPtn>(k, n));

auto l1A = MakeTensor(MakeMemPtr(l1AAddr),
    MakeFrameLayout<NZLayoutPtn>(m, k));
auto l1B = MakeTensor(MakeMemPtr(l1BAddr),
    MakeFrameLayout<ZNLayoutPtn>(k, n));

auto l0A = MakeTensor(MakeMemPtr(l0aAddr),
    MakeFrameLayout<NZLayoutPtn>(m, k));
auto l0B = MakeTensor(MakeMemPtr(l0bAddr),
    MakeFrameLayout<ZNLayoutPtn>(k, n));
auto l0C = MakeTensor(MakeMemPtr(l0cAddr),
    MakeFrameLayout<NZLayoutPtn>(m, n));

auto copyGm2L1 = MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>();
auto copyL12L0A = MakeCopy<CopyL12L0A, CopyL12L0ATraitDefault>();
auto copyL12L0B = MakeCopy<CopyL12L0B, CopyL12L0BTraitDefault>();

Copy(copyGm2L1, l1A, gmA);
Copy(copyGm2L1, l1B, gmB);
Copy(copyL12L0A, l0A, l1A);
Copy(copyL12L0B, l0B, l1B);

MmadParams params(m, n, k, 0, true);
auto mmadAtom = MakeMmad<MmadOperation, MmadTraitDefault>().with(params);
Mmad(mmadAtom, l0C, l0A, l0B);
```
