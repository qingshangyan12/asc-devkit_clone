# L1 Buffer到Fixpipe Buffer数据搬运

## 产品支持情况

|产品|是否支持|
|--------|--------|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

头文件为：#include "tensor_api/tensor.h"

该接口用于将L1 Buffer中的数据搬运到Fixpipe Buffer。Fixpipe Buffer可用于L0C Buffer输出搬运中的量化参数或中间参数准备。

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

|参数名|输入/输出|描述|
|--------|--------|--------|
|`dst`|输出|目的张量，存储位置为`Location::FIXBUF`。|
|`src`|输入|源张量，存储位置为`Location::L1`。|
|`atomCopy`|输入|搬运原子对象，可由`MakeCopy(CopyL12FB{})`或`MakeCopy(CopyL12FB{}, CopyL12FBTraitDefault{})`构造。|

## 返回值说明

`Copy`无返回值。

## 约束说明

- 目的张量必须位于Fixpipe Buffer，源张量必须位于L1 Buffer。
- 支持的数据类型组合为`uint64_t`目的和`uint64_t`源。
- 常用布局为`NDExtLayoutPtn`到`NDExtLayoutPtn`。
- 该接口通常与L0C Buffer到Global Memory或L0C Buffer到Unified Buffer的量化输出流程配合使用。

## 调用示例

```cpp
using namespace AscendC::Te;

auto l1Param = MakeTensor(MakeMemPtr<Location::L1, uint64_t>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<uint64_t>>(1, n));
auto fixbuf = MakeTensor(MakeMemPtr<Location::FIXBUF, uint64_t>(fixbufAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<uint64_t>>(1, n));

auto atom = MakeCopy(CopyL12FB{});
Copy(atom, fixbuf, l1Param);
```
