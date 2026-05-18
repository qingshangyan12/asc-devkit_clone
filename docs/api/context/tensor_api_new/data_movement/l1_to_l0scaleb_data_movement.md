# L1到L0ScaleB数据搬运

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：`#include "tensor_api/tensor.h"`

该接口用于将L1 Buffer中的MX ScaleB数据搬运到L0ScaleB Buffer。当前接口通过`CopyAtom`配置搬运功能，调用`Copy`时需要显式传入由`MakeCopy`构造的原子对象。

该通路的数据类型固定为`fp8_e8m0_t`。ScaleA、ScaleB的分形排布和缩放计算关系可参考下图，其中ScaleB对应小N大N的MX scale排布：

![ScaleA和ScaleB缩放示意图](../../figures/zh-cn_image_0000002549011155.png)

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
| `atomCopy` | 输入 | 搬运原子对象。L1 -> L0ScaleB通路可通过`MakeCopy(CopyL12L0ScaleB{})`或`MakeCopy(CopyL12L0ScaleB{}, CopyL12L0ScaleBTraitDefault{})`构造。 |
| `copyOperation` | 输入 | 搬运操作对象。L1 -> L0ScaleB通路使用`CopyL12L0ScaleB{}`。 |
| `copyTrait` | 输入 | 搬运Trait对象，用于指定搬运特性。L1 -> L0ScaleB默认Trait使用`CopyL12L0ScaleBTraitDefault{}`。 |
| `dst` | 输出 | 目的张量，存储位置必须为`Location::L0ScaleB`，布局必须为`NNLayoutPtn`。 |
| `src` | 输入 | 源张量，存储位置必须为`Location::L1`，布局必须为`NNLayoutPtn`。 |

**表2**`CopyL12L0ScaleBTrait`说明

```cpp
struct LoadDataTrait {
    bool transposed = false;
};

using CopyL12L0ScaleBTrait = LoadDataTrait;
```

| 字段名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `transposed` | `bool` | `false` | `LoadDataTrait`中的通用字段。通常直接使用`CopyL12L0ScaleBTraitDefault{}`。 |

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 数据类型

Ascend 950PR/Ascend 950DT上，L1 -> L0ScaleB通路仅支持`fp8_e8m0_t`。

源张量和目的张量的基础数据类型需要保持一致。

## 约束说明

- 该接口仅在AI Core侧执行。
- 目的张量必须位于L0ScaleB Buffer，源张量必须位于L1 Buffer。
- 源张量和目的张量的数据类型均必须为`fp8_e8m0_t`。
- 源张量和目的张量布局均必须为`NNLayoutPtn`，通常使用`MakeFrameLayout<NNLayoutPtn, _2>(...)`构造。
- 源地址和目的地址需要满足32 Byte对齐要求。
- 构造`Location::L0ScaleB`目的指针时，通常基于L0B Buffer基地址按接口要求转换地址值。
- Tensor Layout采用层次化表述法，Shape、Stride等数值数据支持运行时整数和`Te::_N`等静态整数。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1ToL0ScaleBExample()
{
    using namespace AscendC::Te;

    constexpr uint32_t k = 32;
    constexpr uint32_t n = 16;

    __cbuf__ fp8_e8m0_t l1Buf[k * n] = {0};
    __cb__ fp8_e8m0_t l0BBuf[k * n] = {0};

    auto l1Tensor = MakeTensor(
        MakeMemPtr<Location::L1>(l1Buf),
        MakeFrameLayout<NNLayoutPtn, _2>(k, n));

    // ScaleB address is derived from the L0B base pointer.
    auto l0ScaleBPtr = MakeMemPtr<Location::L0ScaleB, fp8_e8m0_t>(
        reinterpret_cast<uint64_t>(l0BBuf) / 16);
    auto l0ScaleBTensor = MakeTensor(
        l0ScaleBPtr,
        MakeFrameLayout<NNLayoutPtn, _2>(k, n));

    auto copyAtom = MakeCopy(CopyL12L0ScaleB{}, CopyL12L0ScaleBTraitDefault{});
    Copy(copyAtom, l0ScaleBTensor, l1Tensor);
}
```

- 复制子张量

  当只需要从L1 Tensor中搬运一个ScaleB子区域到L0ScaleB时，可以先通过`tensor.Slice()`构造源子张量，再将该子张量作为`Copy`的源张量传入。

  `tensor.Slice(startCoord, sliceInfo)`中，`startCoord`表示子张量在原Tensor中的起始逻辑坐标，`sliceInfo`可使用`MakeShape`描述子张量的逻辑尺寸，也可传入Layout描述子张量布局。子张量的Layout、数据类型和地址对齐要求仍需满足L1 -> L0ScaleB通路约束。

  ```cpp
  #include "tensor_api/tensor.h"

  __aicore__ inline void CopyL1SubTensorToL0ScaleBExample()
  {
      using namespace AscendC::Te;

      constexpr uint32_t k = 32;
      constexpr uint32_t n = 32;
      constexpr uint32_t tileK = 16;
      constexpr uint32_t tileN = 16;

      __cbuf__ fp8_e8m0_t l1Buf[k * n] = {0};
      __cb__ fp8_e8m0_t l0BBuf[tileK * tileN] = {0};

      auto l1Tensor = MakeTensor(
          MakeMemPtr<Location::L1>(l1Buf),
          MakeFrameLayout<NNLayoutPtn, _2>(k, n));

      // ScaleB address is derived from the L0B base pointer.
      auto l0ScaleBPtr = MakeMemPtr<Location::L0ScaleB, fp8_e8m0_t>(
          reinterpret_cast<uint64_t>(l0BBuf) / 16);
      auto l0ScaleBTensor = MakeTensor(
          l0ScaleBPtr,
          MakeFrameLayout<NNLayoutPtn, _2>(tileK, tileN));

      auto l1SubTensor = l1Tensor.Slice(MakeCoord(16, 0), MakeShape(tileK, tileN));
      auto copyAtom = MakeCopy(CopyL12L0ScaleB{}, CopyL12L0ScaleBTraitDefault{});
      Copy(copyAtom, l0ScaleBTensor, l1SubTensor);
  }
  ```
