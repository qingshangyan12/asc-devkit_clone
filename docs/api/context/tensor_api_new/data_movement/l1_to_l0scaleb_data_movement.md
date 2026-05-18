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

该接口用于将 L1 Buffer 中的 MX ScaleB 数据搬运到 L0ScaleB Buffer，通常作为 MX 矩阵计算中 ScaleB 的加载路径。Tensor API 通过 `dst` 和 `src` 的位置标签自动分发到 L1 -> L0ScaleB 通路，也可以通过 `CopyAtom<CopyTraits<CopyL12L0ScaleB, ...>>` 显式指定该通路。

该通路的数据类型固定为 `fp8_e8m0_t`。ScaleA、ScaleB 的分形排布和缩放计算关系可参考下图，其中 ScaleB 对应小 N 大 N 的 MX scale 排布：

![ScaleA和ScaleB缩放示意图](../../figures/zh-cn_image_0000002549011155.png)

接口支持直接从源张量起始位置搬运，也支持传入 `coord` 从源张量指定逻辑坐标处开始搬运；未传入 `coord` 时，从源张量 `(0, 0)` 位置开始搬运。

## 函数原型

- 自动根据张量位置分发。

    ```cpp
    template <typename T, typename U, Std::enable_if_t<IsAttrTensorV<T> && IsAttrTensorV<U>, int>,
        typename... Params>
    __aicore__ inline void
    Copy(const T& dst, const U& src, const Params& ...params)
    ```

- 显式指定 `CopyL12L0ScaleB`。

    ```cpp
    template <typename T, typename... Params>
    __aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params)
    ```

- 构造 `CopyL12L0ScaleB` 原子对象。

    ```cpp
    template <typename... Args>
    __aicore__ inline constexpr auto MakeCopy(const Args& ...traits)
    ```

## 参数说明

**表 1** 通用参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 目的张量，存储位置必须为 `Location::L0ScaleB`，布局必须为 `NNLayoutPtn`。 |
| `src` | 输入 | 源张量，存储位置必须为 `Location::L1`，布局必须为 `NNLayoutPtn`。 |
| `params` | 输入 | 可变参数。当前常用形式为可选的 `coord`，由 `MakeCoord` 构造，用于指定源张量搬运起始逻辑坐标。 |
| `atomCopy` | 输入 | 搬运原子对象。显式调用时使用 `CopyAtom<CopyTraits<CopyL12L0ScaleB, CopyL12L0ScaleBTraitDefault>>`，或通过 `MakeCopy(CopyL12L0ScaleB{}, CopyL12L0ScaleBTraitDefault{})` 构造。 |
| `traits` | 输入 | `MakeCopy` 的构造参数。L1 -> L0ScaleB 默认使用 `CopyL12L0ScaleB` 和 `CopyL12L0ScaleBTraitDefault`。 |

**表 2** Tensor API到底层MX Load2D参数的映射说明

Tensor API 不直接暴露 `asc_copy_l12l0b_mx` 的运行时参数，而是根据 `dst`、`src` 的 Layout、Shape、Stride 以及可选 `coord` 推导底层搬运参数。

| 底层参数 | Tensor API来源 | 含义 |
| :--- | :--- | :--- |
| `x_start_pos` | 未传 `coord` 时为 `0`；传入 `coord` 时由 `coord[1]` 按 N 方向分形粒度换算得到 | 源 ScaleB 的 N 方向起始位置。 |
| `y_start_pos` | 未传 `coord` 时为 `0`；传入 `coord` 时由 `coord[0]` 按 MX scale 的 K 方向粒度换算得到 | 源 ScaleB 的 K 方向起始位置。 |
| `x_step` | 由目的张量 `NNLayoutPtn` 的 Shape 推导 | N 方向搬运长度。 |
| `y_step` | 由目的张量 `NNLayoutPtn` 的 Shape 推导 | K 方向搬运长度。 |
| `src_stride` | 由源张量 `NNLayoutPtn` 的 Stride 推导 | 源 ScaleB 相邻分形起始地址间隔，单位为 32 Byte。 |
| `dst_stride` | 由目的张量 `NNLayoutPtn` 的 Shape 推导 | 目的 ScaleB 相邻分形起始地址间隔，单位为 32 Byte。 |

## 返回值说明

`Copy` 无返回值。`MakeCopy` 返回 `CopyAtom` 对象。

## 数据类型

Ascend 950PR/Ascend 950DT 上，L1 -> L0ScaleB 通路仅支持 `fp8_e8m0_t`。

源张量和目的张量的基础数据类型需要保持一致。

## API映射关系

该 Tensor API 通路是在 L1 -> L0ScaleB MX scale Load2D 能力上进行抽象封装实现的。

| Tensor API场景 | 底层搬运能力 |
| :--- | :--- |
| `NNLayoutPtn -> NNLayoutPtn` | `asc_copy_l12l0b_mx`，用于将 ScaleB 从 L1 Buffer 搬运到 L0ScaleB Buffer。 |

## 约束说明

- 该接口仅在 AI Core 侧执行。
- 目的张量必须位于 L0ScaleB Buffer，源张量必须位于 L1 Buffer。
- 源张量和目的张量的数据类型均必须为 `fp8_e8m0_t`。
- 源张量和目的张量布局均必须为 `NNLayoutPtn`，通常使用 `MakeFrameLayout<NNLayoutPtn, _2>(...)` 构造。
- 源地址和目的地址需要满足 32 Byte 对齐要求。
- 构造 `Location::L0ScaleB` 目的指针时，需要按底层 MX scale 指令要求使用 L0ScaleB 地址粒度；裸地址场景可参考调用示例中的 `/ 16` 写法。
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

    Copy(l0ScaleBTensor, l1Tensor);

    auto coord = MakeCoord(_0{}, _0{});
    Copy(l0ScaleBTensor, l1Tensor, coord);

    auto atom = MakeCopy(CopyL12L0ScaleB{}, CopyL12L0ScaleBTraitDefault{});
    Copy(atom, l0ScaleBTensor, l1Tensor, coord);
}
```

## 关键特性说明

### ScaleB专用位置

`Location::L0ScaleB` 是 MX 矩阵计算中 ScaleB 的专用目的位置，不等同于普通 B 矩阵使用的 `Location::L0B`。调用 `Mmad` 前，需要分别完成 B 矩阵到 L0B、ScaleB 到 L0ScaleB 的数据准备。

### 坐标搬运

传入 `coord` 时，`coord[0]` 用于描述 ScaleB 的 K 方向起始逻辑坐标，`coord[1]` 用于描述 ScaleB 的 N 方向起始逻辑坐标。Tensor API 会根据 `NNLayoutPtn` 和 MX scale 的分形粒度换算到底层 `x_start_pos` 和 `y_start_pos`。
