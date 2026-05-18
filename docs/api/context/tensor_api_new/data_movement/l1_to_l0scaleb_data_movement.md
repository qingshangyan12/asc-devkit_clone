# L1到L0ScaleB数据搬运

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

该接口用于将 L1 Buffer 中的 MX ScaleB 数据搬运到 L0ScaleB Buffer。当前接口通过 `CopyAtom` 配置搬运功能，调用 `Copy` 时需要显式传入由 `MakeCopy` 构造的原子对象。

该通路的数据类型固定为 `fp8_e8m0_t`。ScaleA、ScaleB 的分形排布和缩放计算关系可参考下图，其中 ScaleB 对应小 N 大 N 的 MX scale 排布：

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
    __aicore__ inline constexpr auto MakeCopy();
    ```

- 构造指定 Trait 的搬运原子对象。

    ```cpp
    template <typename CopyOperationType, typename CopyTraitType>
    __aicore__ inline constexpr auto MakeCopy();
    ```

## 参数说明

**表 1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomCopy` | 输入 | 搬运原子对象。L1 -> L0ScaleB 通路可通过 `MakeCopy<CopyL12L0ScaleB>()` 或 `MakeCopy<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>()` 构造。 |
| `dst` | 输出 | 目的张量，存储位置必须为 `Location::L0ScaleB`，布局必须为 `NNLayoutPtn`。 |
| `src` | 输入 | 源张量，存储位置必须为 `Location::L1`，布局必须为 `NNLayoutPtn`。 |

## 返回值说明

`Copy` 无返回值。`MakeCopy` 返回 `CopyAtom` 对象。

## 数据类型

Ascend 950PR/Ascend 950DT 上，L1 -> L0ScaleB 通路仅支持 `fp8_e8m0_t`。

源张量和目的张量的基础数据类型需要保持一致。

## 支持的布局组合

| 目的张量 Layout | 源张量 Layout | 搬运方式 |
| :--- | :--- | :--- |
| `NNLayoutPtn` | `NNLayoutPtn` | ScaleB 搬运 |

## 约束说明

- 该接口仅在 AI Core 侧执行。
- 目的张量必须位于 L0ScaleB Buffer，源张量必须位于 L1 Buffer。
- 源张量和目的张量的数据类型均必须为 `fp8_e8m0_t`。
- 源张量和目的张量布局均必须为 `NNLayoutPtn`，通常使用 `MakeFrameLayout<NNLayoutPtn, _2>(...)` 构造。
- 源地址和目的地址需要满足 32 Byte 对齐要求。
- 构造 `Location::L0ScaleB` 目的指针时，通常基于 L0B Buffer 基地址按接口要求转换地址值。
- Tensor Layout 采用层次化表述法，Shape、Stride 等数值数据支持运行时整数和 `Te::_N` 等静态整数。

## 流水类型

```cpp
PIPE_MTE1
```

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

    auto copyAtom = MakeCopy<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>();
    Copy(copyAtom, l0ScaleBTensor, l1Tensor);
}
```

## 复制子张量

当只需要从 L1 Tensor 中搬运一个 ScaleB 子区域到 L0ScaleB 时，可以先通过 `tensor.Slice()` 构造源子张量，再将该子张量作为 `Copy` 的源张量传入。`Slice` 只生成张量视图，不立即搬运数据；真正的数据搬运仍由 `Copy(copyAtom, dst, srcSubTensor)` 完成。

`tensor.Slice(startCoord, sliceInfo)` 中，`startCoord` 表示子张量在原 Tensor 中的起始逻辑坐标，`sliceInfo` 可使用 `MakeShape` 描述子张量的逻辑尺寸，也可传入 Layout 描述子张量布局。子张量的 Layout、数据类型和地址对齐要求仍需满足 L1 -> L0ScaleB 通路约束。

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
    auto copyAtom = MakeCopy<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>();
    Copy(copyAtom, l0ScaleBTensor, l1SubTensor);
}
```

## 关键特性说明

### ScaleB专用位置

`Location::L0ScaleB` 是 MX 矩阵计算中 ScaleB 的专用目的位置，不等同于普通右矩阵使用的 `Location::L0B`。调用 `Mmad` 前，需要分别完成右矩阵到 L0B、ScaleB 到 L0ScaleB 的数据准备。

### 地址来源

ScaleB 的目的地址通常通过 L0B Buffer 基地址转换得到。构造 Tensor 时，目的指针需要使用 `Location::L0ScaleB` 标识，使后续搬运原子对象进入 ScaleB 通路。
