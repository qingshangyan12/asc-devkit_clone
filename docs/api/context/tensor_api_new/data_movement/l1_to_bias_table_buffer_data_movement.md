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
- 高维切分搬运：通过 Tensor.Slice的`Coord`和`Shape`描述非连续数据块，调用 `Copy(dst, src)` 完成多块搬运。
- 类型转换搬运：源 Tensor 为 `half`、目的 Tensor 为 `float` 时，底层搬运会开启转换控制，将 `half` 数据搬运到 BiasTable Buffer 后按 `float` 使用。

## 函数原型

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

## 约束说明

- 目的张量必须位于 BiasTable Buffer，源张量必须位于 L1 Buffer。
- 支持的数据类型组合包括 `float` 目的和 `bfloat16_t` 源、`float` 目的和 `half` 源、`float` 目的和 `float` 源、`int32_t` 目的和 `int32_t` 源。
- 支持 `NDLayoutPtn` 或 `NDExtLayoutPtn`，源张量和目的张量均需使用受支持的布局模式。
- 作为 `Mmad` bias 输入时，bias 张量维度需要与矩阵计算的 N 方向匹配。
- 该接口仅在 AIC 侧执行，AIV 侧调用不会执行底层搬运指令。

## 调用示例

连续搬运示例：

```cpp
using namespace AscendC::Te;

auto l1Bias = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(1, n));
auto bias = MakeTensor(MakeMemPtr<Location::BIAS>(biasAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(1, n));

auto copyL12BT = MakeCopy<CopyL12BT, CopyL12BTTraitDefault>();
Copy(copyL12BT, bias, l1Bias);
```

源操作数和目的操作数数据类型不一致示例：

```cpp
using namespace AscendC::Te;

auto l1Bias = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(1, n));
auto bias = MakeTensor(MakeMemPtr<Location::BIAS>(biasAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(1, n));

auto copyL12BT = MakeCopy<CopyL12BT, CopyL12BTTraitDefault>();
Copy(copyL12BT, bias, l1Bias);
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

auto copyL12BT = MakeCopy<CopyL12BT, CopyL12BTTraitDefault>();
Copy(copyL12BT, bias, l1Bias);
```
