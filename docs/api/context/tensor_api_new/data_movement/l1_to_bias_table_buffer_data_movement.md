# L1到BiasTable Buffer数据搬运

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：`#include "tensor_api/tensor.h"`

该接口用于将L1 Buffer中的数据搬运到BiasTable Buffer，数据搬运后可作为矩阵计算中的bias输入，用于对L0C的初始矩阵或偏置项进行处理。

TensorAPI 中该搬运通过源Tensor和目的Tensor的`Shape`、`Stride`推导底层搬运参数：

- 连续搬运：使用连续布局描述源Tensor和目的Tensor，调用`Copy(dst, src)`搬运完整Tensor。
- 高维切分搬运：通过Tensor.Slice的`Coord`和`Shape`描述非连续数据块，调用`Copy(dst, src)`完成多块搬运。
- 类型转换搬运：源Tensor为`half`、目的Tensor为`float`时，底层搬运会开启转换控制，将`half`数据搬运到BiasTable Buffer后按`float`使用。

## 函数原型

- 执行搬运。

    ```cpp
    template <typename AtomType, typename DstTensor, typename SrcTensor>
    __aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src);
    ```

- 构造默认搬运原子对象。

    ```cpp
    template <typename CopyOperationType>
    __aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation);
    ```

- 构造指定Trait的搬运原子对象。

    ```cpp
    template <typename CopyOperationType, typename CopyTraitType>
    __aicore__ inline constexpr auto MakeCopy(
        const CopyOperationType& copyOperation, const CopyTraitType& copyTrait);
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomCopy` | 输入 | 搬运原子对象。L1 Buffer-> Bias Table Buffer通路可通过`MakeCopy(CopyL12BT{})`或`MakeCopy(CopyL12BT{}, CopyL12BTTraitDefault{})`构造。 |
| `copyOperation` | 输入 | 搬运操作对象。L1 Buffer-> Bias Table Buffer通路使用`CopyL12BT{}`。 |
| `copyTrait` | 输入 | 搬运Trait对象，用于指定搬运特性。L1 Buffer-> Bias Table Buffer默认Trait使用`CopyL12BTTraitDefault{}`。 |
| `dst` | 输出 | 目的张量，存储位置必须为`Location::BT`。目的地址需要满足Bias Table Buffer的分形和对齐约束。 |
| `src` | 输入 | 源张量，存储位置必须为`Location::L1`。源地址需要满足L1 Buffer的分形和对齐约束。 |

**表2**`CopyL12BTTrait`和`CopyL12BTTraitDefault`说明

```cpp
struct CopyL12BTTrait {};

constexpr CopyL12BTTrait DEFAULT_COPY_L1_BT_TRAIT;

struct CopyL12BTTraitDefault {
    using TraitType = CopyL12BTTrait;
    static constexpr const TraitType value = DEFAULT_COPY_L1_BT_TRAIT;
};
```

`CopyL12BTTrait`是实际使用的Trait类型，用于描述搬运特性；`CopyL12BTTraitDefault`是默认Trait包装类型，不新增字段，只通过`TraitType`指定实际Trait类型为`CopyL12BTTrait`，并通过`value`提供默认配置`DEFAULT_COPY_L1_TO_BT_TRAIT`。

| 字段名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `transposed` | `bool` | `false` | 表示底层搬运指令是否按转置方式执行。`false`表示非转置搬运，`true`表示转置搬运。接口会根据源张量和目的张量的Layout选择非转置或转置搬运，通常直接使用`CopyL12BTTraitDefault{}`。 |

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 数据类型

Ascend 950PR/Ascend 950DT支持的数据类型组合如下：

| 目的张量数据类型 | 源张量数据类型 |
| :--- | :--- |
| `float` | `bfloat16_t` |
| `float` | `half` |
| `float` | `float` |
| `int32_t` | `int32_t` |

## 约束说明

- 支持`NDLayoutPtn`或`NDExtLayoutPtn`，源张量和目的张量均需使用受支持的布局模式。
- 作为`Mmad`bias输入时，bias张量维度需要与矩阵计算的N方向匹配。

## 调用示例

连续搬运示例：

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1ToBTExample()
{
    using namespace AscendC::Te;

    using SrcType = half;
    using DstType = float;
    constexpr uint32_t n = 16;

    __cbuf__ SrcType l1Buf[n];
    __biasbuf__ DstType btBuf[n];

    auto l1Tensor = MakeTensor(
        MakeMemPtr(l1Buf),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<SrcType>>(1, n));

    auto btTensor = MakeTensor(
        MakeMemPtr(btBuf),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<DstType>>(1, n));

    auto copyAtom = MakeCopy(CopyL12BT{}, CopyL12BTTraitDefault{});
    Copy(copyAtom, btTensor, l1Tensor);
}
```

复制子张量示例：

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1SubTensorToBTExample()
{
    using namespace AscendC::Te;

    using SrcType = half;
    using DstType = float;
    constexpr uint32_t m = 4;
    constexpr uint32_t n = 32;
    constexpr uint32_t tileM = 4;
    constexpr uint32_t tileN = 16;

    __cbuf__ SrcType l1Buf[m * n];
    __biasbuf__ DstType btBuf[m * n];

    auto l1Tensor = MakeTensor(
        MakeMemPtr(l1Buf),
        MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<SrcType>>(m, n));

    auto btTensor = MakeTensor(
        MakeMemPtr(btBuf),
        MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<DstType>>(m, n));

    auto l1SubTensor = l1Tensor.Slice(MakeCoord(0, 16), MakeShape(tileM, tileN));
    auto btSubTensor = btTensor.Slice(MakeCoord(0, 16), MakeShape(tileM, tileN));

    auto copyAtom = MakeCopy(CopyL12BT{}, CopyL12BTTraitDefault{});
    Copy(copyAtom, btSubTensor, l1SubTensor);
}
```
