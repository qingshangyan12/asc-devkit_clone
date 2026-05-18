# L1到L0ScaleA数据搬运

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

该接口用于将 L1 Buffer 中的 MX ScaleA 数据搬运到 L0ScaleA Buffer。当前接口通过 `CopyAtom` 配置搬运功能，调用 `Copy` 时需要显式传入由 `MakeCopy` 构造的原子对象。

该通路的数据类型固定为 `fp8_e8m0_t`。ScaleA、ScaleB 的分形排布和缩放计算关系可参考下图，其中 ScaleA 对应小 Z 大 Z 的 MX scale 排布：

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

- 构造指定 Trait 的搬运原子对象。

    ```cpp
    template <typename CopyOperationType, typename CopyTraitType>
    __aicore__ inline constexpr auto MakeCopy(
        const CopyOperationType& copyOperation, const CopyTraitType& copyTrait);
    ```

## 参数说明

**表 1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomCopy` | 输入 | 搬运原子对象。L1 -> L0ScaleA 通路可通过 `MakeCopy(CopyL12L0ScaleA{})` 或 `MakeCopy(CopyL12L0ScaleA{}, CopyL12L0ScaleATraitDefault{})` 构造。 |
| `copyOperation` | 输入 | 搬运操作对象。L1 -> L0ScaleA 通路使用 `CopyL12L0ScaleA{}`。 |
| `copyTrait` | 输入 | 搬运 Trait 对象，用于指定搬运特性。L1 -> L0ScaleA 默认 Trait 使用 `CopyL12L0ScaleATraitDefault{}`。 |
| `dst` | 输出 | 目的张量，存储位置必须为 `Location::L0ScaleA`，布局必须为 `ZZLayoutPtn`。 |
| `src` | 输入 | 源张量，存储位置必须为 `Location::L1`，布局必须为 `ZZLayoutPtn`。 |

## 返回值说明

`Copy` 无返回值。`MakeCopy` 返回 `CopyAtom` 对象。

## 数据类型

Ascend 950PR/Ascend 950DT 上，L1 -> L0ScaleA 通路仅支持 `fp8_e8m0_t`。

源张量和目的张量的基础数据类型需要保持一致。

## 支持的布局组合

| 目的张量 Layout | 源张量 Layout | 搬运方式 |
| :--- | :--- | :--- |
| `ZZLayoutPtn` | `ZZLayoutPtn` | ScaleA 搬运 |

## 约束说明

- 该接口仅在 AI Core 侧执行。
- 目的张量必须位于 L0ScaleA Buffer，源张量必须位于 L1 Buffer。
- 源张量和目的张量的数据类型均必须为 `fp8_e8m0_t`。
- 源张量和目的张量布局均必须为 `ZZLayoutPtn`，通常使用 `MakeFrameLayout<ZZLayoutPtn, _2>(...)` 构造。
- 源地址和目的地址需要满足 32 Byte 对齐要求。
- 构造 `Location::L0ScaleA` 目的指针时，通常基于 L0A Buffer 基地址按接口要求转换地址值。
- Tensor Layout 采用层次化表述法，Shape、Stride 等数值数据支持运行时整数和 `Te::_N` 等静态整数。

## 流水类型

```cpp
PIPE_MTE1
```

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1ToL0ScaleAExample()
{
    using namespace AscendC::Te;

    constexpr uint32_t m = 16;
    constexpr uint32_t k = 32;

    __cbuf__ fp8_e8m0_t l1Buf[m * k] = {0};
    __ca__ fp8_e8m0_t l0ABuf[m * k] = {0};

    auto l1Tensor = MakeTensor(
        MakeMemPtr<Location::L1>(l1Buf),
        MakeFrameLayout<ZZLayoutPtn, _2>(m, k));

    // ScaleA address is derived from the L0A base pointer.
    auto l0ScaleAPtr = MakeMemPtr<Location::L0ScaleA, fp8_e8m0_t>(
        reinterpret_cast<uint64_t>(l0ABuf) / 16);
    auto l0ScaleATensor = MakeTensor(
        l0ScaleAPtr,
        MakeFrameLayout<ZZLayoutPtn, _2>(m, k));

    auto copyAtom = MakeCopy(CopyL12L0ScaleA{}, CopyL12L0ScaleATraitDefault{});
    Copy(copyAtom, l0ScaleATensor, l1Tensor);
}
```

## 复制子张量

当只需要从 L1 Tensor 中搬运一个 ScaleA 子区域到 L0ScaleA 时，可以先通过 `tensor.Slice()` 构造源子张量，再将该子张量作为 `Copy` 的源张量传入。

`tensor.Slice(startCoord, sliceInfo)` 中，`startCoord` 表示子张量在原 Tensor 中的起始逻辑坐标，`sliceInfo` 可使用 `MakeShape` 描述子张量的逻辑尺寸，也可传入 Layout 描述子张量布局。子张量的 Layout、数据类型和地址对齐要求仍需满足 L1 -> L0ScaleA 通路约束。

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1SubTensorToL0ScaleAExample()
{
    using namespace AscendC::Te;

    constexpr uint32_t m = 32;
    constexpr uint32_t k = 32;
    constexpr uint32_t tileM = 16;
    constexpr uint32_t tileK = 16;

    __cbuf__ fp8_e8m0_t l1Buf[m * k] = {0};
    __ca__ fp8_e8m0_t l0ABuf[tileM * tileK] = {0};

    auto l1Tensor = MakeTensor(
        MakeMemPtr<Location::L1>(l1Buf),
        MakeFrameLayout<ZZLayoutPtn, _2>(m, k));

    // ScaleA address is derived from the L0A base pointer.
    auto l0ScaleAPtr = MakeMemPtr<Location::L0ScaleA, fp8_e8m0_t>(
        reinterpret_cast<uint64_t>(l0ABuf) / 16);
    auto l0ScaleATensor = MakeTensor(
        l0ScaleAPtr,
        MakeFrameLayout<ZZLayoutPtn, _2>(tileM, tileK));

    auto l1SubTensor = l1Tensor.Slice(MakeCoord(16, 0), MakeShape(tileM, tileK));
    auto copyAtom = MakeCopy(CopyL12L0ScaleA{}, CopyL12L0ScaleATraitDefault{});
    Copy(copyAtom, l0ScaleATensor, l1SubTensor);
}
```

## 关键特性说明

### ScaleA专用位置

`Location::L0ScaleA` 是 MX 矩阵计算中 ScaleA 的专用目的位置，不等同于普通左矩阵使用的 `Location::L0A`。调用 `Mmad` 前，需要分别完成左矩阵到 L0A、ScaleA 到 L0ScaleA 的数据准备。

### 地址来源

ScaleA 的目的地址通常通过 L0A Buffer 基地址 `/16` 转换得到。构造 Tensor 时，目的指针需要使用 `Location::L0ScaleA` 标识，使后续搬运原子对象进入 ScaleA 通路。
