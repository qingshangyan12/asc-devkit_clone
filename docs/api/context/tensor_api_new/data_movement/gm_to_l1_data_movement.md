# Global Memory到L1 Buffer数据搬运

## 产品支持情况

|产品|是否支持|
|--------|--------|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

头文件为：#include "tensor_api/tensor.h"

该接口用于将Global Memory中的数据搬运到L1 Buffer。接口通过张量的存储位置和布局模式选择连续搬运、ND到NZ转换、DN到NZ转换、NZ到NZ搬运、ZN到ZN搬运以及MX Scale布局搬运等实现。

## 函数原型

```cpp
template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src)
```

```cpp
template <typename CopyOperationType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation)

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation, const CopyTraitType& copyTrait)
```

## 参数说明

|参数名|输入/输出|描述|
|--------|--------|--------|
|`dst`|输出|目的张量，存储位置为`Location::L1`。|
|`src`|输入|源张量，存储位置为`Location::GM`。|
|`atomCopy`|输入|搬运原子对象，可由`MakeCopy(CopyGM2L1{})`或`MakeCopy(CopyGM2L1{}, CopyGM2L1TraitDefault{})`构造。|
|`traits`|输入|`MakeCopy`的构造参数，Global Memory到L1 Buffer搬运使用`CopyGM2L1`和`CopyGM2L1TraitDefault`。|

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 约束说明

- 目的张量必须位于L1 Buffer，源张量必须位于Global Memory。
- 支持的布局组合包括`NDExtLayoutPtn`到`NDExtLayoutPtn`、`NDExtLayoutPtn`到`NZLayoutPtn`、`NDExtLayoutPtn`到`ZNLayoutPtn`、`DNExtLayoutPtn`到`NZLayoutPtn`、`DNExtLayoutPtn`到`ZNLayoutPtn`、`NZLayoutPtn`到`NZLayoutPtn`、`ZNLayoutPtn`到`ZNLayoutPtn`、`ScaleANDLayoutPtn`到`ZZLayoutPtn`、`ScaleADNLayoutPtn`到`ZZLayoutPtn`、`ZZLayoutPtn`到`ZZLayoutPtn`、`ScaleBNDLayoutPtn`到`NNLayoutPtn`、`ScaleBDNLayoutPtn`到`NNLayoutPtn`、`NNLayoutPtn`到`NNLayoutPtn`。
- 普通数据类型支持`int8_t`、`uint8_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`hifloat8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。部分ND对齐搬运还支持`fp4x2_e1m2_t`、`fp4x2_e2m1_t`、`int64_t`、`uint64_t`。
- 源张量和目的张量的数据类型需要保持一致。
- 对齐要求由所选布局和底层搬运指令决定，使用时应保证Global Memory地址和L1 Buffer地址满足硬件搬运对齐要求。

## 调用示例

```cpp
using namespace AscendC::Te;

auto gmA = MakeTensor(MakeMemPtr<Location::GM, float>(gmAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(m, n));
auto l1A = MakeTensor(MakeMemPtr<Location::L1, float>(l1Addr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(m, n));

auto atom = MakeCopy(CopyGM2L1{});
Copy(atom, l1A, gmA);
```
