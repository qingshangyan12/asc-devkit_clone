# L1到L0A数据搬运

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

该接口用于将 L1 Buffer 中的矩阵数据搬运到 L0A Buffer，通常作为 Cube 矩阵计算中 A 矩阵从 A1 到 A2 的加载路径。Tensor API 通过 `dst` 和 `src` 的位置标签自动分发到 L1 -> L0A 通路，也可以通过 `CopyAtom<CopyTraits<CopyL12L0A, ...>>` 显式指定该通路。

该通路以 512 Byte 数据分形为基本搬运单位。不同数据类型在 L0A 中对应的单分形矩阵形态如下：

| 数据位宽 | L0A单分形矩阵形态 |
| :--- | :--- |
| b4 | 16 * 64 |
| b8 | 16 * 32 |
| b16 | 16 * 16 |
| b32 | 16 * 8 |

接口支持非转置搬运和转置搬运。传入 `coord` 时，可以从源张量指定逻辑坐标处开始搬运；未传入 `coord` 时，从源张量 `(0, 0)` 位置开始搬运。

## 函数原型

- 自动根据张量位置分发。

    ```cpp
    template <typename T, typename U, Std::enable_if_t<IsAttrTensorV<T> && IsAttrTensorV<U>, int>,
        typename... Params>
    __aicore__ inline void Copy(const T& dst, const U& src, const Params& ...params)
    ```

- 显式指定 `CopyL12L0A`。

    ```cpp
    template <typename T, typename... Params>
    __aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params)
    ```

- 构造 `CopyL12L0A` 原子对象。

    ```cpp
    template <typename... Args>
    __aicore__ inline constexpr auto MakeCopy(const Args& ...traits)
    ```

## 参数说明

**表 1** 通用参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 目的张量，存储位置必须为 `Location::L0A`。目的地址需要满足 L0A Buffer 的分形和对齐约束。 |
| `src` | 输入 | 源张量，存储位置必须为 `Location::L1`。源地址需要满足 L1 Buffer 的分形和对齐约束。 |
| `params` | 输入 | 可变参数。当前常用形式为可选的 `coord`，由 `MakeCoord` 构造，用于指定源张量搬运起始逻辑坐标。 |
| `atomCopy` | 输入 | 搬运原子对象。显式调用时使用 `CopyAtom<CopyTraits<CopyL12L0A, CopyL12L0ATraitDefault>>`，或通过 `MakeCopy(CopyL12L0A{}, CopyL12L0ATraitDefault{})` 构造。 |
| `traits` | 输入 | `MakeCopy` 的构造参数。L1 -> L0A 默认使用 `CopyL12L0A` 和 `CopyL12L0ATraitDefault`。 |

**表 2** Tensor API到底层Load2D参数的映射说明

Tensor API 不直接暴露 `LoadData2DParamsV2`，而是根据 `dst`、`src` 的 Layout、Shape、Stride 以及可选 `coord` 推导底层搬运参数。

| 底层参数 | Tensor API来源 | 含义 |
| :--- | :--- | :--- |
| `mStartPosition` | 未传 `coord` 时为 `0`；传入 `coord` 时由 `coord` 的 M 方向逻辑坐标换算得到 | 源矩阵 M 方向起始位置，单位为 16 个元素。 |
| `kStartPosition` | 未传 `coord` 时为 `0`；传入 `coord` 时由 `coord` 的 K 方向逻辑坐标换算得到 | 源矩阵 K 方向起始位置，单位为 32 Byte。 |
| `mStep` | 由目的张量或源张量的 Shape 推导 | M 方向搬运长度，单位为 16 个元素。 |
| `kStep` | 由目的张量或源张量的 Shape 推导 | K 方向搬运长度，单位为 32 Byte。 |
| `srcStride` | 由源张量的 Stride 推导 | 源矩阵相邻分形起始地址间隔，单位为 512 Byte。 |
| `dstStride` | 由目的张量的 Stride 推导 | 目的矩阵相邻分形起始地址间隔，单位为 512 Byte。 |
| `transpose` | 由源、目的 Layout 组合自动选择 | 是否对每个分形矩阵执行转置搬运。 |

对于 L1 -> L0A 通路，`coord` 的换算规则如下：

| Layout组合 | `mStartPosition`来源 | `kStartPosition`来源 |
| :--- | :--- | :--- |
| `NZLayoutPtn -> NZLayoutPtn` | `coord[0] / 16` | `coord[1] / C0_ELEMENT` |
| `ZNLayoutPtn -> NZLayoutPtn` | `coord[1] / 16` | `coord[0] / C0_ELEMENT` |

其中 `C0_ELEMENT` 与数据类型相关，对应 32 Byte 数据宽度。

## 返回值说明

`Copy` 无返回值。`MakeCopy` 返回 `CopyAtom` 对象。

## 数据类型

Ascend 950PR/Ascend 950DT 支持的数据类型包括：

`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`float`。

源张量和目的张量的基础数据类型需要保持一致。

## API映射关系

该 Tensor API 通路是在 L1 -> L0A Load2D 能力上进行抽象封装实现的。Tensor API 根据 Tensor 的位置、Layout 和数据类型选择底层实现。

| Tensor API场景 | 底层搬运能力 |
| :--- | :--- |
| `NZLayoutPtn -> NZLayoutPtn` | `asc_copy_l12l0a`，概念上对应非转置 L1 -> L0A Load2D 搬运。 |
| `ZNLayoutPtn -> NZLayoutPtn` | `asc_copy_l12l0a_transpose`，概念上对应转置 L1 -> L0A Load2D 搬运。 |
| b4 数据类型 | 使用 b4 专用搬运实现，概念上对应 `load_cbuf_to_ca_s4` 能力。 |
| b8/b16/b32 数据类型 | 使用对应位宽的 L1 -> L0A Load2D 搬运能力。 |

## 约束说明

- 该接口仅在 AI Core 侧执行。
- 目的张量必须位于 L0A Buffer，源张量必须位于 L1 Buffer。
- 目的地址位于 L0A Buffer 时，起始地址需要满足 512 Byte 对齐要求；源地址位于 L1 Buffer 时，起始地址需要满足 32 Byte 对齐要求。
- 普通非转置搬运支持 `NZLayoutPtn -> NZLayoutPtn`。
- 普通转置搬运支持 `ZNLayoutPtn -> NZLayoutPtn`。
- `mStep` 或 `kStep` 推导结果为 0 时，底层搬运视为空操作。
- 启用转置搬运时，不同数据类型需要满足不同约束：b4 数据类型要求 `mStep` 为 4 的倍数；b8 数据类型要求 `mStep` 为 2 的倍数；b16 数据类型无额外倍数要求；b32 数据类型要求 `kStep` 为 2 的倍数。
- Tensor Layout 采用层次化表述法，Shape、Stride 等数值数据支持运行时整数和 `Te::_N` 等静态整数。

## 流水类型

```cpp
PIPE_MTE1
```

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1ToL0AExample()
{
    using namespace AscendC::Te;

    using AType = half;
    constexpr uint32_t m = 16;
    constexpr uint32_t k = 16;

    __cbuf__ AType l1Buf[m * k];
    __ca__ AType l0aBuf[m * k];

    auto l1Tensor = MakeTensor(
        MakeMemPtr(l1Buf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<AType>>(m, k));

    auto l0aTensor = MakeTensor(
        MakeMemPtr(l0aBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<AType>>(m, k));

    Copy(l0aTensor, l1Tensor);

    auto coord = MakeCoord(_0{}, _0{});
    Copy(l0aTensor, l1Tensor, coord);

    auto atom = MakeCopy(CopyL12L0A{}, CopyL12L0ATraitDefault{});
    Copy(atom, l0aTensor, l1Tensor, coord);
}
```

## 关键特性说明

### 非转置搬运

当源张量和目的张量均为 `NZLayoutPtn` 时，接口按非转置方式从 L1 Buffer 搬运到 L0A Buffer。此时 `coord[0]` 表示源矩阵 M 方向逻辑坐标，`coord[1]` 表示源矩阵 K 方向逻辑坐标。

### 转置搬运

当源张量为 `ZNLayoutPtn`、目的张量为 `NZLayoutPtn` 时，接口按转置方式搬运。转置搬运会对每个分形矩阵执行转置，适用于矩阵计算中需要按 L0A 分形形态组织 A 矩阵的场景。

### 起始位置和步长

Tensor API 的 `coord` 使用逻辑元素坐标描述起始位置，底层 Load2D 的 `mStartPosition` 和 `kStartPosition` 由该逻辑坐标换算得到。`mStep`、`kStep`、`srcStride`、`dstStride` 由张量 Layout 中的 Shape 和 Stride 推导，用户无需手动构造 `LoadData2DParamsV2`。
