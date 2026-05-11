# L1到Fixpipe Buffer数据搬运

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

该接口用于将L1 Buffer中的数据搬运到Fixpipe Buffer，数据搬运过程中格式和内容保持不变。Fixpipe Buffer可用于L0C输出搬运中的随路量化、随路ReLU等参数准备。

TensorAPI中该搬运通过源Tensor和目的Tensor的`Shape`、`Stride`推导底层搬运参数：

- 连续搬运：使用连续布局描述源Tensor和目的Tensor，调用`Copy(dst, src)`搬运完整Tensor。
- 高维切分搬运：通过Tensor.Slice的`Coord`和`Shape`描述非连续数据块，调用`Copy(dst, src)`完成多块搬运。

## 函数原型

- 显式指定`CopyL12FB`。

    ```cpp
    template <typename T, typename... Params>
    __aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params)
    ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 目的张量，存储位置为 `Location::FIXBUF`。 |
| `src` | 输入 | 源张量，存储位置为 `Location::L1`。 |
| `params` | 输入 | 可变参数。当前 L1 到 Fixpipe Buffer 搬运不需要额外运行时参数，连续搬运和高维切分搬运均由 Tensor 布局描述。 |
| `atomCopy` | 输入 | 搬运原子对象，显式调用时使用 `CopyAtom<CopyTraits<CopyL12FB, CopyL12FBTraitDefault>>` 或 `MakeCopy(CopyL12FB{}, CopyL12FBTraitDefault{})`。 |

## 约束说明

- 目的张量必须位于Fixpipe Buffer，源张量必须位于L1 Buffer。
- 支持的数据类型组合为`uint64_t`目的和`uint64_t`源。
- 支持`NDLayoutPtn`或`NDExtLayoutPtn`，源张量和目的张量均需使用受支持的布局模式。
- 该接口通常与L0C到GM或L0C到UB的量化输出流程配合使用。
- 该接口仅在AIC侧执行，AIV侧调用不会执行底层搬运指令。

## 调用示例

连续搬运示例：

```cpp
using namespace AscendC::Te;

auto l1Param = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<uint64_t>>(1, n));
auto fixbuf = MakeTensor(MakeMemPtr<Location::FIXBUF>(fixbufAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<uint64_t>>(1, n));

auto copyL12FB = MakeCopy<CopyL12FB, CopyL12FBTraitDefault>();
Copy(copyL12FB, fixbuf, l1Param);
```

高维切分搬运示例：

```cpp
using namespace AscendC::Te;

auto l1Full = MakeTensor(MakeMemPtr<Location::L1>(l1Addr),
    MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<uint64_t>>(rowCount, srcRowStride));
auto fixbufFull = MakeTensor(MakeMemPtr<Location::FIXBUF>(fixbufAddr),
    MakeFrameLayout<NDLayoutPtn, LayoutTraitDefault<uint64_t>>(rowCount, dstRowStride));

auto l1Param = l1Full.Slice(MakeCoord(0, srcColOffset), MakeShape(rowCount, colCount));
auto fixbuf = fixbufFull.Slice( MakeCoord(0, dstColOffset), MakeShape(rowCount, colCount));

auto copyL12FB = MakeCopy<CopyL12FB, CopyL12FBTraitDefault>();
Copy(copyL12FB, fixbuf, l1Param);
```
