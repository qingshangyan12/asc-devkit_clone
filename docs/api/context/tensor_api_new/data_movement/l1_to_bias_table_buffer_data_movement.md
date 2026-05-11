# L1到BiasTable Buffer数据搬运

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

该接口用于将 L1 Buffer 中的数据搬运到 BiasTable Buffer，数据搬运后可作为矩阵计算中的 bias 输入，用于对 L0C 的初始矩阵或偏置项进行处理。

TensorAPI 中该搬运通过源 Tensor 和目的 Tensor 的 `Shape`、`Stride` 推导底层搬运参数：

- 连续搬运：使用连续布局描述源 Tensor 和目的 Tensor，调用 `Copy(dst, src)` 搬运完整 Tensor。
- 高维切分搬运：通过 Tensor 布局中的行数、列数和行步长描述非连续数据块，调用 `Copy(dst, src)` 完成多块搬运；旧接口中的 `DataCopyParams` 不作为 TensorAPI 的运行时参数传入。
- 类型转换搬运：源 Tensor 为 `half`、目的 Tensor 为 `float` 时，底层搬运会开启转换控制，将 `half` 数据搬运到 BiasTable Buffer 后按 `float` 使用。

## 函数原型

- 自动根据张量位置分发。

    ```cpp
    template <typename T, typename U, Std::enable_if_t<IsAttrTensorV<T> && IsAttrTensorV<U>, int>,
        typename... Params>
    __aicore__ inline void
    Copy(const T& dst, const U& src, const Params& ...params)
    ```

- 显式指定 `CopyL12BT`。

    ```cpp
    template <typename T, typename... Params>
    __aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params)
    ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 目的张量，存储位置为 `Location::BIAS`。 |
| `src` | 输入 | 源张量，存储位置为 `Location::L1`。 |
| `params` | 输入 | 可变参数。当前 L1 到 BiasTable Buffer 搬运不需要额外运行时参数，连续搬运和高维切分搬运均由 Tensor 布局描述。 |
| `atomCopy` | 输入 | 搬运原子对象，显式调用时使用 `CopyAtom<CopyTraits<CopyL12BT, CopyL12BTTraitDefault>>` 或 `MakeCopy(CopyL12BT{}, CopyL12BTTraitDefault{})`。 |

## 返回值说明

`Copy` 无返回值。

## 约束说明

- 目的张量必须位于 BiasTable Buffer，源张量必须位于 L1 Buffer。
- 支持的数据类型组合包括 `float` 目的和 `bfloat16_t` 源、`float` 目的和 `half` 源、`float` 目的和 `float` 源、`int32_t` 目的和 `int32_t` 源。
- 支持 `NDLayoutPtn` 或 `NDExtLayoutPtn`，源张量和目的张量均需使用受支持的布局模式。
- 作为 `Mmad` bias 输入时，bias 张量维度需要与矩阵计算的 N 方向匹配。
- 该接口仅在 AIC 侧执行，AIV 侧调用不会执行底层搬运指令。

## 搬运参数对应关系

TensorAPI 不直接暴露 `count` 或 `DataCopyParams`。当前 `CopyL12BT` 实现会从 Tensor 布局中提取源 Tensor 的行数、列数和源/目的行步长，并映射到底层 `asc_copy_l12bt` 参数。

| 底层参数 | TensorAPI 来源 | 说明 |
| :--- | :--- | :--- |
| `convControl` | 源/目的数据类型 | `half` 源搬运到 `float` 目的时为 `true`，其他支持类型组合为 `false`。 |
| `blockCount` | `src` 的行数 | 连续或高维切分搬运的数据块个数。 |
| `blockLen` | `src` 的列数和源数据类型 | 当前实现为 `ceil_division(srcCol, C0_ELEMENT<srcType>)`；源类型为 `float` 或 `int32_t` 时，该值再按 2 对齐。 |
| `srcStride` | `src` 的行步长、列数和源数据类型 | 当前实现为 `(srcRowStride - srcCol) / C0_ELEMENT<srcType>`。 |
| `dstStride` | `dst` 的行步长、列数和目的数据类型 | 当前实现为 `ceil_align((dstRowStride - srcCol) / C0_ELEMENT<dstType>, 2)`。 |

对于连续搬运，布局的行步长通常等于列数；对于高维切分搬运，可通过更大的行步长描述相邻数据块之间的地址间隔。

## 调用示例

连续搬运示例：

```cpp
using namespace AscendC::Te;

auto l1Bias = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(1, n));
auto bias = MakeTensor(MakeMemPtr<Location::BIAS>(biasAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(1, n));

Copy(bias, l1Bias);
```

源操作数和目的操作数数据类型不一致示例：

```cpp
using namespace AscendC::Te;

auto l1Bias = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(1, n));
auto bias = MakeTensor(MakeMemPtr<Location::BIAS>(biasAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(1, n));

Copy(bias, l1Bias);
```

高维切分搬运示例：

```cpp
using namespace AscendC::Te;

auto l1Full = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<float>>(rowCount, srcRowStride));
auto biasFull = MakeTensor(MakeMemPtr<Location::BIAS>(biasAddr),
    MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<float>>(rowCount, dstRowStride));

auto l1Bias = l1Full.Slice(MakeCoord(0, srcColOffset), MakeShape(rowCount, colCount));
auto bias = biasFull.Slice(MakeCoord(0, dstColOffset), MakeShape(rowCount, colCount));

Copy(bias, l1Bias);
```
