# L1 Buffer到Fixpipe Buffer数据搬运

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：`#include "tensor_api/tensor.h"`

该接口用于将L1 Buffer中的数据搬运到Fixpipe Buffer，数据搬运过程中格式和内容保持不变。Fixpipe Buffer可用于L0C输出搬运中的随路量化、随路ReLU等参数准备。

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
| `atomCopy` | 输入 | 搬运原子对象。L1 Buffer-> Fixpipe Buffer通路可通过`MakeCopy(CopyL12FB{})`或`MakeCopy(CopyL12FB{}, CopyL12FBTraitDefault{})`构造。 |
| `copyOperation` | 输入 | 搬运操作对象。L1 Buffer-> Fixpipe Buffer通路使用`CopyL12FB{}`。 |
| `copyTrait` | 输入 | 搬运Trait对象，用于指定搬运特性。L1 Buffer-> Fixpipe Buffer默认Trait使用`CopyL12FBTraitDefault{}`。 |
| `dst` | 输出 | 目的张量，存储位置必须为`Location::FIXBUF`。目的地址需要满足Fixpipe Buffer的对齐约束。 |
| `src` | 输入 | 源张量，存储位置必须为`Location::L1`。源地址需要满足L1 Buffer的对齐约束。 |

**表2**`CopyL12FBTrait`和`CopyL12FBTraitDefault`说明

```cpp
struct CopyL12FBTrait {};

constexpr CopyL12FBTrait DEFAULT_COPY_L1_FB_TRAIT;

struct CopyL12FBTraitDefault {
    using TraitType = CopyL12FBTrait;
    static constexpr const TraitType value = DEFAULT_COPY_L1_FB_TRAIT;
};
```

`CopyL12FBTrait`是实际使用的Trait类型，用于描述搬运特性；`CopyL12FBTraitDefault`是默认Trait包装类型，不新增字段，只通过`TraitType`指定实际Trait类型为`CopyL12FBTrait`，并通过`value`提供默认配置`DEFAULT_COPY_L1_FB_TRAIT`。

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 数据类型

Ascend 950PR/Ascend 950DT支持的数据类型组合如下：`uint64_t`
源张量和目的张量的基础数据类型需要保持一致。

## API映射关系

与built-in接口映射关系：

`CopyL12FB`接口对应的底层built-in接口为：

```cpp
__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint16_t burst_num,
    uint16_t burst_len, uint16_t src_gap_size, uint16_t dst_gap_size);
```

## 约束说明

- 支持`NDLayoutPtn`或`NDExtLayoutPtn`，源张量和目的张量均需使用受支持的布局模式。
- 该接口通常与L0C到GM或L0C到UB的量化输出流程配合使用。

## 调用示例

连续搬运示例：

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1ToFixpipeExample()
{
    using namespace AscendC::Te;

    using DataType = uint64_t;
    constexpr uint32_t n = 16;

    __cbuf__ DataType l1Buf[n];
    __fbuf__ DataType fixbuf[n];

    auto l1Tensor = MakeTensor(
        MakeMemPtr(l1Buf),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<DataType>>(1, n));

    auto fixbufTensor = MakeTensor(
        MakeMemPtr(fixbuf),
        MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<DataType>>(1, n));

    auto copyAtom = MakeCopy(CopyL12FB{}, CopyL12FBTraitDefault{});
    Copy(copyAtom, fixbufTensor, l1Tensor);
}
```

复制子张量示例：

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1SubTensorToFixpipeExample()
{
    using namespace AscendC::Te;

    using DataType = uint64_t;
    constexpr uint32_t m = 4;
    constexpr uint32_t n = 32;
    constexpr uint32_t tileM = 4;
    constexpr uint32_t tileN = 16;

    __cbuf__ DataType l1Buf[m * n];
    __fbuf__ DataType fixbuf[m * n];

    auto l1Tensor = MakeTensor(
        MakeMemPtr(l1Buf),
        MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<DataType>>(m, n));

    auto fixbufTensor = MakeTensor(
        MakeMemPtr(fixbuf),
        MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<DataType>>(m, n));

    auto l1SubTensor = l1Tensor.Slice(MakeCoord(0, 16), MakeShape(tileM, tileN));
    auto fixbufSubTensor = fixbufTensor.Slice(MakeCoord(0, 16), MakeShape(tileM, tileN));

    auto copyAtom = MakeCopy(CopyL12FB{}, CopyL12FBTraitDefault{});
    Copy(copyAtom, fixbufSubTensor, l1SubTensor);
}
```
