# L1到UB数据搬运

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

该接口用于将 L1 Buffer 中的数据搬运到 UB，数据搬运过程中格式和内容保持不变。支持 ND、DN、NZ 等布局的同布局搬运。

TensorAPI 中该搬运通过源 Tensor 和目的 Tensor 的 `Shape`、`Stride` 推导底层搬运参数：

- 连续搬运：使用连续布局描述源 Tensor 和目的 Tensor，调用 `Copy(dst, src)` 搬运完整 Tensor。
- 高维切分搬运：通过 Tensor 布局中的行数、列数和步长描述非连续数据块，调用 `Copy(dst, src)` 完成多块搬运；旧接口中的 `DataCopyParams` 不作为 TensorAPI 的运行时参数传入。

## 函数原型

- 自动根据张量位置分发。

    ```cpp
    template <typename T, typename U, Std::enable_if_t<IsAttrTensorV<T> && IsAttrTensorV<U>, int>,
        typename... Params>
    __aicore__ inline void
    Copy(const T& dst, const U& src, const Params& ...params)
    ```

- 显式指定 `CopyL12UB`。

    ```cpp
    template <typename T, typename... Params>
    __aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params)
    ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 目的张量，存储位置为 `Location::UB`。 |
| `src` | 输入 | 源张量，存储位置为 `Location::L1`。 |
| `params` | 输入 | 可变参数。当前 L1 到 UB 搬运不需要额外运行时参数，连续搬运和高维切分搬运均由 Tensor 布局描述。 |
| `atomCopy` | 输入 | 搬运原子对象，显式调用时使用 `CopyAtom<CopyTraits<CopyL12UB, CopyL12UBTraitDefault>>` 或 `MakeCopy(CopyL12UB{}, CopyL12UBTraitDefault{})`。 |

## 返回值说明

`Copy` 无返回值。

## 约束说明

- 目的张量必须位于 UB，源张量必须位于 L1 Buffer。
- 支持 `NDLayoutPtn` 到 `NDLayoutPtn`、`NDExtLayoutPtn` 到 `NDExtLayoutPtn`、`DNLayoutPtn` 到 `DNLayoutPtn`、`DNExtLayoutPtn` 到 `DNExtLayoutPtn`、`NZLayoutPtn` 到 `NZLayoutPtn`。
- 源张量和目的张量的数据类型需要保持一致。
- 搬运长度和步长由布局计算得到，调用者需要保证布局描述与实际内存空间匹配。
- ND 和 DN 场景下，L1 和 UB 上申请的空间以及 Tensor 的步长需要满足 32 字节对齐要求。
- 该接口仅在 AIC 侧执行，AIV 侧调用不会执行底层搬运指令。

## 搬运参数对应关系

TensorAPI 不直接暴露 `count` 或 `DataCopyParams`。当前 `CopyL12UB` 实现会根据布局类型映射到底层 `asc_copy_l12ub` 参数。

ND/NDExt 布局：

| 底层参数 | TensorAPI 来源 | 说明 |
| :--- | :--- | :--- |
| `blockCount` | `src` 的总行数 | 连续或高维切分搬运的数据块个数。 |
| `blockLen` | `src` 的总列数和源数据类型 | 当前实现为 `ceil_division(totalColumn, C0_ELEMENT<srcType>)`，单位为 32B。 |
| `srcStride` | `src` 的行步长和总列数 | 当前实现为 `ceil_division(srcRowStride - totalColumn, C0_ELEMENT<srcType>)`。 |
| `dstStride` | `dst` 的行步长和总列数 | 当前实现为 `ceil_division(dstRowStride - totalColumn, C0_ELEMENT<dstType>)`。 |

DN/DNExt 布局：

| 底层参数 | TensorAPI 来源 | 说明 |
| :--- | :--- | :--- |
| `blockCount` | `src` 的总列数 | 连续或高维切分搬运的数据块个数。 |
| `blockLen` | `src` 的总行数和源数据类型 | 当前实现为 `ceil_division(totalRow, C0_ELEMENT<srcType>)`，单位为 32B。 |
| `srcStride` | `src` 的列步长和总行数 | 当前实现为 `ceil_division(srcColumnStride - totalRow, C0_ELEMENT<srcType>)`。 |
| `dstStride` | `dst` 的列步长和总行数 | 当前实现为 `ceil_division(dstColumnStride - totalRow, C0_ELEMENT<dstType>)`。 |

NZ 布局：

| 底层参数 | TensorAPI 来源 | 说明 |
| :--- | :--- | :--- |
| `blockCount` | `src` 的 Column 外层 Shape | 对应 NZ 布局中按列方向分形后的块数。 |
| `blockLen` | `src` 的总行数 | 当前实现为 `GetTotalRowShape(srcLayout)`。 |
| `srcStride` | `src` 的 Column 外层步长和源数据类型 | 当前实现为 `srcColumnOuterStride / C0_ELEMENT<srcType> - blockLen`。 |
| `dstStride` | `dst` 的 Column 外层步长和目的数据类型 | 当前实现为 `dstColumnOuterStride / C0_ELEMENT<dstType> - blockLen`。 |

对于连续搬运，布局的行步长或列步长通常等于连续维度长度；对于高维切分搬运，可通过更大的行跨度或列跨度描述相邻数据块之间的地址间隔。这里的跨度是完整行或完整列占用的元素数，不是旧 `DataCopyParams` 中的块间隔字段。

## 调用示例

连续搬运示例：

```cpp
using namespace AscendC::Te;

auto l1 = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<int8_t>>(m, n));
auto ub = MakeTensor(MakeMemPtr<Location::UB>(ubAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<int8_t>>(m, n));

Copy(ub, l1);
```

高维切分搬运示例：

```cpp
using namespace AscendC::Te;

auto l1Full = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<int8_t>>(rowCount, srcRowStride));
auto ubFull = MakeTensor(MakeMemPtr<Location::UB>(ubAddr),
    MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<int8_t>>(rowCount, dstRowStride));

auto l1 = l1Full.Slice(MakeCoord(0, srcColOffset), MakeShape(rowCount, colCount));
auto ub = ubFull.Slice(MakeCoord(0, dstColOffset), MakeShape(rowCount, colCount));

Copy(ub, l1);
```
