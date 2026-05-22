# L1到Unified Buffer数据搬运

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：`#include "tensor_api/tensor.h"`

该接口用于将L1 Buffer中的数据搬运到Unified Buffer，数据搬运过程中格式和内容保持不变。支持ND、DN、NZ等布局的同布局搬运。

TensorAPI中该搬运通过源Tensor和目的Tensor的`Shape`、`Stride`推导底层搬运参数：

- 连续搬运：使用连续布局描述源Tensor和目的Tensor，调用`Copy(dst, src)`搬运完整Tensor。
- 复制子张量：通过`Tensor.Slice`的`Coord`和`Shape`描述非连续数据块，调用`Copy(dst, src)`完成多块搬运。

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
| `atomCopy` | 输入 | 搬运原子对象。L1 Buffer-> Unified Buffer通路可通过`MakeCopy(CopyL12UB{})`或`MakeCopy(CopyL12UB{}, CopyL12UBTraitDefault{})`构造。 |
| `copyOperation` | 输入 | 搬运操作对象。L1 Buffer-> Unified Buffer通路使用`CopyL12UB{}`。 |
| `copyTrait` | 输入 | 搬运Trait对象，用于指定搬运特性。L1 Buffer-> Unified Buffer默认Trait使用`CopyL12UBTraitDefault{}`。 |
| `dst` | 输出 | 目的张量，存储位置必须为`Location::UB`。目的地址需要满足Unified Buffer的对齐约束。 |
| `src` | 输入 | 源张量，存储位置必须为`Location::L1`。源地址需要满足L1 Buffer的对齐约束。 |

**表2**`CopyL12UBTrait`和`CopyL12UBTraitDefault`说明

```cpp
struct CopyL12UBTrait {};

constexpr CopyL12UBTrait DEFAULT_COPY_L1_TO_UB_TRAIT;

struct CopyL12UBTraitDefault {
    using TraitType = CopyL12UBTrait;
    static constexpr const TraitType value = DEFAULT_COPY_L1_TO_UB_TRAIT;
};
```

`CopyL12UBTrait`是实际使用的Trait类型，用于描述搬运特性；`CopyL12UBTraitDefault`是默认Trait包装类型，不新增字段，只通过`TraitType`指定实际Trait类型为`CopyL12UBTrait`，并通过`value`提供默认配置`DEFAULT_COPY_L1_TO_UB_TRAIT`。

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 数据类型

源张量和目的张量的数据类型需要保持一致。

## 约束说明

- 支持`NDLayoutPtn`、`NDExtLayoutPtn`、`DNLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`，源张量和目的张量的数据排布需要保持一致。。
- 搬运长度和步长由布局计算得到，调用者需要保证布局描述与实际内存空间匹配。
- ND和DN场景下，L1和Unified Buffer上申请的空间以及Tensor的步长需要满足32字节对齐要求。

## 调用示例

连续搬运示例：

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1ToUBExample()
{
    using namespace AscendC::Te;

    using DataType = int8_t;
    constexpr uint32_t m = 1;
    constexpr uint32_t n = 32;

    __cbuf__ DataType l1Buf[m * n];
    __ubuf__ DataType ubBuf[m * n];

    auto l1Tensor = MakeTensor(
        MakeMemPtr(l1Buf),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<DataType>>(m, n));

    auto ubTensor = MakeTensor(
        MakeMemPtr(ubBuf),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<DataType>>(m, n));

    auto copyAtom = MakeCopy(CopyL12UB{}, CopyL12UBTraitDefault{});
    Copy(copyAtom, ubTensor, l1Tensor);
}
```

复制子张量示例：

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1SubTensorToUBExample()
{
    using namespace AscendC::Te;

    using DataType = int8_t;
    constexpr uint32_t m = 4;
    constexpr uint32_t n = 32;
    constexpr uint32_t tileM = 4;
    constexpr uint32_t tileN = 16;

    __cbuf__ DataType l1Buf[m * n];
    __ubuf__ DataType ubBuf[m * n];

    auto l1Tensor = MakeTensor(
        MakeMemPtr(l1Buf),
        MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<DataType>>(m, n));

    auto ubTensor = MakeTensor(
        MakeMemPtr(ubBuf),
        MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<DataType>>(m, n));

    auto l1SubTensor = l1Tensor.Slice(MakeCoord(0, 16), MakeShape(tileM, tileN));
    auto ubSubTensor = ubTensor.Slice(MakeCoord(0, 16), MakeShape(tileM, tileN));

    auto copyAtom = MakeCopy(CopyL12UB{}, CopyL12UBTraitDefault{});
    Copy(copyAtom, ubSubTensor, l1SubTensor);
}
```
