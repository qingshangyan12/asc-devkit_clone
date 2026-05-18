# MX矩阵计算

## 产品支持情况

| 产品 | 不带 bias | 带 bias |
| :--- | :------: | :-----: |
| Ascend 950PR/Ascend 950DT | √ | √ |

## 头文件/库文件

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

## 功能说明

MX 矩阵计算是 Tensor API 中通过 `MmadTrait::mmadType` 选择的矩阵乘加模式，用于低精度 MX 数据类型的矩阵计算。该模式仍以 L0A 为左矩阵、L0B 为右矩阵、L0C 为结果矩阵，调用入口与普通 `Mmad` 一致。

数学表达式为：

```text
C = A * B
```

带 bias 输入时，bias 作为 C 矩阵初始值参与计算：

```text
C = Bias + A * B
```

Mmad 的矩阵乘加关系可参考下图：

![Mmad矩阵乘加数学表达式](../../figures/mmad_formula.png)

MX 计算通常需要先通过数据搬运流程准备 A、B 以及对应的 ScaleA/ScaleB 数据，再调用 `Mmad` 完成矩阵计算。

矩阵 A、B、C 的逻辑关系和物理存储位置如下图所示：

![Mmad矩阵计算示意图](../../figures/mmad_matrix_layout.png)

## 函数原型

- 自动使用默认矩阵计算原子对象。该重载默认使用 `MmadType::NORMAL`，MX 模式需使用下方自定义 trait 形式。

    ```cpp
    template <typename T, typename U, typename S,
        Std::enable_if_t<IsAttrTensorV<T> && IsAttrTensorV<U> && IsAttrTensorV<S>, int>,
        typename... Params>
    __aicore__ inline void Mmad(const T& dst, const U& fm, const S& filter, const Params& ...params)
    ```

- 使用 `MmadAtom` 调用矩阵计算操作。

    ```cpp
    template <typename T, typename... Params>
    __aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params)
    ```

- 通过模板实参指定自定义 trait。

    ```cpp
    template <typename Tp, const Tp& traits, typename T, typename... Params>
    __aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params)
    ```

- 构造 `MmadAtom`。

    ```cpp
    template <typename... Args>
    __aicore__ inline constexpr auto MakeMmad(const Args& ...traits)
    ```

常用调用形式如下：

```cpp
constexpr MmadTrait MX_MMAD_TRAIT(0, false, false, true, MmadType::MX);

auto atom = MakeMmad(MmadOperation{}, MmadTraitDefault{});
Mmad<MmadTrait, MX_MMAD_TRAIT>(atom, dst, fm, filter, params);
Mmad<MmadTrait, MX_MMAD_TRAIT>(atom, dst, fm, filter, bias, params);
```

也可以将 `MmadParams` 固化到 atom 中：

```cpp
auto atomWithParams = MakeMmad(MmadOperation{}, MmadTraitDefault{}).with(params);
Mmad<MmadTrait, MX_MMAD_TRAIT>(atomWithParams, dst, fm, filter);
```

## 参数说明

**表 1** 通用参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 结果矩阵 C，存储位置必须为 `Location::L0C`，数据类型通常为 `float`。 |
| `fm` | 输入 | 左矩阵 A，存储位置必须为 `Location::L0A`，数据类型为 MX 支持的低精度类型。 |
| `filter` | 输入 | 右矩阵 B，存储位置必须为 `Location::L0B`，数据类型为 MX 支持的低精度类型。 |
| `bias` | 输入 | 可选 bias 张量，存储位置支持 `Location::BIAS` 或 `Location::L0C`，数据类型为 `float`。 |
| `params` | 输入 | `MmadParams`，描述 M/N/K 尺寸、unitFlag 和 C 矩阵初始化方式。 |
| `atomMmad` | 输入 | 矩阵计算原子对象，可由 `MakeMmad(MmadOperation{}, MmadTraitDefault{})` 构造。MX 模式通过 `Mmad<TraitType, trait>` 的模板实参指定。 |
| `traits` | 输入 | `MakeMmad` 的构造参数。`.with(params)` 可将 `MmadParams` 固化到 atom 中。 |

**表 2** `MmadParams` 参数说明

| 参数名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `m` | `uint16_t` | `0` | 左矩阵 A 的高度，也是结果矩阵 C 的高度。 |
| `n` | `uint16_t` | `0` | 右矩阵 B 的宽度，也是结果矩阵 C 的宽度。 |
| `k` | `uint16_t` | `0` | 左矩阵 A 的宽度，也是右矩阵 B 的高度。 |
| `unitFlag` | `uint8_t` | `0` | 控制 `Mmad` 和后续 `Fixpipe` 细粒度并行。 |
| `cmatrixInitVal` | `bool` | `false` | 不带 bias 调用时，控制是否初始化 C 矩阵。带 bias 调用时该配置无效。 |

**表 3** `MmadTrait` 参数说明

| 参数名 | 类型 | MX模式推荐值 | 描述 |
| :--- | :--- | :--- | :--- |
| `fmOffset` | `int32_t` | `0` | 左矩阵 offset，当前 Tensor API 实现中作为兼容参数保留。 |
| `kDirectionAlign` | `bool` | `false` | K 方向对齐控制，当前 Tensor API 实现中作为兼容参数保留。 |
| `cmatrixSource` | `bool` | `false` | 不带 bias 调用时，配合 `cmatrixInitVal` 指示 C 矩阵初始值来源。 |
| `disableGemv` | `bool` | `true` | M=1 场景下是否关闭 GEMV 模式。 |
| `mmadType` | `MmadType` | `MmadType::MX` | 矩阵计算类型。MX 矩阵计算必须设置为 `MmadType::MX`。 |

## 返回值说明

`Mmad` 无返回值。`MakeMmad` 返回 `MmadAtom` 对象。

## 数据类型

Ascend 950PR/Ascend 950DT MX 矩阵计算支持如下 A、B、C 数据类型组合：

| A 数据类型 | B 数据类型 | C 数据类型 |
| :--- | :--- | :--- |
| `fp4x2_e2m1_t` | `fp4x2_e2m1_t` | `float` |
| `fp4x2_e2m1_t` | `fp4x2_e1m2_t` | `float` |
| `fp4x2_e1m2_t` | `fp4x2_e2m1_t` | `float` |
| `fp4x2_e1m2_t` | `fp4x2_e1m2_t` | `float` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` | `float` |
| `fp8_e4m3fn_t` | `fp8_e5m2_t` | `float` |
| `fp8_e5m2_t` | `fp8_e4m3fn_t` | `float` |
| `fp8_e5m2_t` | `fp8_e5m2_t` | `float` |

带 bias 场景中，bias 位于 `Location::BIAS` 或 `Location::L0C` 时，bias 数据类型均需要为 `float`。

## API映射关系

Tensor API 的 MX 矩阵计算在底层映射到 AscendC MX 矩阵乘加能力。

| Tensor API场景 | 底层能力 |
| :--- | :--- |
| 不带 bias，`MmadType::MX` | `asc_mmad_mx(dst, fm, filter, m, k, n, unitFlag, disableGemv, cmatrixSource, cmatrixInitVal)` |
| 带 bias，`MmadType::MX` | `asc_mmad_mx` 的 bias 形式，Tensor API 将 C 和 bias 地址组合后传入底层指令。 |

## 约束说明

- 该接口仅在 AI Core 侧执行。
- `dst` 必须位于 L0C Buffer，`fm` 必须位于 L0A Buffer，`filter` 必须位于 L0B Buffer。
- 需要通过 `Mmad<TraitType, trait>` 指定 `MmadTrait::mmadType = MmadType::MX` 的 constexpr trait。
- `dst`、`fm`、`filter` 的 Layout 需要满足 Tensor API 对分形布局的编译期检查。
- MX 计算通常需要在调用 `Mmad` 前完成 ScaleA/ScaleB 数据搬运。ScaleA 搬运可参考 [L1到L0ScaleA数据搬运](../data_movement/l1_to_l0scalea_data_movement.md)，ScaleB 搬运可参考 [L1到L0ScaleB数据搬运](../data_movement/l1_to_l0scaleb_data_movement.md)。
- `m`、`n`、`k` 需要与参与计算的矩阵尺寸一致，并满足底层 MX 矩阵计算指令约束。
- 使用 bias 输入时，bias 张量必须位于 `Location::BIAS` 或 `Location::L0C`。

## 流水类型

```cpp
PIPE_M
```

## 调用示例

```cpp
#include "tensor_api/tensor.h"

constexpr AscendC::Te::MmadTrait MX_MMAD_TRAIT(
    0, false, false, true, AscendC::Te::MmadType::MX);

__aicore__ inline void MxMmadExample()
{
    using namespace AscendC::Te;

    constexpr uint16_t m = 16;
    constexpr uint16_t n = 16;
    constexpr uint16_t k = 16;

    __ca__ fp8_e4m3fn_t l0aBuf[m * k];
    __cb__ fp8_e5m2_t l0bBuf[k * n];
    __cc__ float l0cBuf[m * n];

    auto l0A = MakeTensor(
        MakeMemPtr(l0aBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<fp8_e4m3fn_t>>(m, k));
    auto l0B = MakeTensor(
        MakeMemPtr(l0bBuf),
        MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<fp8_e5m2_t>>(k, n));
    auto l0C = MakeTensor(
        MakeMemPtr(l0cBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));

    MmadParams params(m, n, k, 0, true);
    auto atom = MakeMmad(MmadOperation{}, MmadTraitDefault{});

    Mmad<MmadTrait, MX_MMAD_TRAIT>(atom, l0C, l0A, l0B, params);
}
```

## 关键特性说明

### MX模式选择

MX 模式不是单独的函数入口，而是通过 `MmadTrait::mmadType` 选择。未指定自定义 trait 时默认进入普通矩阵计算通路。自定义 trait 需要以 constexpr 对象形式传给 `Mmad<TraitType, trait>`，通常定义在命名空间作用域。

### Scale数据准备

MX 数据类型计算通常需要配合 scale 数据。Scale 数据不作为 `Mmad` 函数参数传入，而是在调用 `Mmad` 前通过 [L1到L0ScaleA数据搬运](../data_movement/l1_to_l0scalea_data_movement.md) 和 [L1到L0ScaleB数据搬运](../data_movement/l1_to_l0scaleb_data_movement.md) 准备到硬件要求的位置。

### Bias输入

带 bias 调用时，Tensor API 根据 bias 张量位置决定 C 矩阵初始值来源。bias 位于 `Location::BIAS` 时从 BiasTable Buffer 初始化，bias 位于 `Location::L0C` 时从 L0C 初始化。
