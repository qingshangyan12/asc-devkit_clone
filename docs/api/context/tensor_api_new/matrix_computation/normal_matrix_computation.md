# 普通矩阵计算

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

普通矩阵计算接口用于完成 L0A 左矩阵和 L0B 右矩阵的矩阵乘加，结果写入 L0C。数学表达式为 `C = A * B`。带 bias 输入时，bias 可作为 C 矩阵初始值参与计算。

当输入矩阵位于 Global Memory 时，通常先使用 GM 到 L1 的 `Copy` 将 A/B 矩阵搬入 L1 Buffer，再使用 L1 到 L0A/L0B 的 `Copy` 将数据搬入矩阵计算输入位置。GM 到 L1 阶段可根据 Layout 自动完成 ND 到 NZ、ND 到 ZN 等随路格式转换。详细说明可参考 [GM到L1数据搬运](../data_movement/gm_to_l1_data_movement.md)。

## 函数原型

- 自动使用默认矩阵计算原子对象。

    ```cpp
    template <typename T, typename U, typename S,
        Std::enable_if_t<IsAttrTensorV<T> && IsAttrTensorV<U> && IsAttrTensorV<S>, int>, typename... Params>
    __aicore__ inline void
    Mmad(const T& dst, const U& fm, const S& filter, const Params& ...params)
    ```

- 使用 `MmadAtom` 指定矩阵计算操作。

    ```cpp
    template <typename T, typename... Params>
    __aicore__ inline void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params)
    ```

- 构造 `MmadAtom`。

    ```cpp
    template <typename... Args>
    __aicore__ inline constexpr auto MakeMmad(const Args& ...traits)
    ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 结果矩阵张量，存储位置为 `Location::L0C`。 |
| `fm` | 输入 | 左矩阵张量，存储位置为 `Location::L0A`。 |
| `filter` | 输入 | 右矩阵张量，存储位置为 `Location::L0B`。 |
| `params` | 输入 | 可变参数。普通场景传入 `MmadParams`，带 bias 场景传入 bias 张量和 `MmadParams`。 |
| `atomMmad` | 输入 | 矩阵计算原子对象，可由 `MakeMmad(MmadOperation{}, MmadTraitDefault{})` 构造。 |
| `traits` | 输入 | `MakeMmad` 的构造参数。普通矩阵计算使用 `MmadOperation` 和 `MmadTraitDefault`，也可通过 `.with(MmadTrait{})` 覆盖 trait。 |

**MmadParams 参数说明**

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `m` | 输入 | 左矩阵高度，也是结果矩阵高度。类型为 `uint16_t`。 |
| `n` | 输入 | 右矩阵宽度，也是结果矩阵宽度。类型为 `uint16_t`。 |
| `k` | 输入 | 左矩阵宽度和右矩阵高度。类型为 `uint16_t`。 |
| `unitFlag` | 输入 | 控制 Mmad 与后续 Fixpipe 细粒度并行的标志。类型为 `uint8_t`。默认值为 `0`。 |
| `cmatrixInitVal` | 输入 | 是否初始化 C 矩阵。类型为 `bool`。默认值为 `false`。 |

**MmadTrait 参数说明**

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `fmOffset` | 输入 | 左矩阵 offset。当前实现保留该参数。类型为 `int32_t`，默认值为 `0`。 |
| `kDirectionAlign` | 输入 | K 方向对齐控制。当前实现保留该参数。类型为 `bool`，默认值为 `false`。 |
| `cmatrixSource` | 输入 | C 矩阵初始值是否来源于 bias 或 L0C。类型为 `bool`，默认值为 `false`。 |
| `disableGemv` | 输入 | 是否关闭 GEMV 模式。类型为 `bool`，默认值为 `true`。 |
| `mmadType` | 输入 | 矩阵计算类型。普通矩阵计算取 `MmadType::NORMAL`。 |

## 返回值说明

`Mmad` 无返回值。`MakeMmad` 返回 `MmadAtom` 对象。

## 约束说明

- 目的张量必须位于 L0C，左矩阵必须位于 L0A，右矩阵必须位于 L0B。
- 如果 A/B 矩阵源数据位于 Global Memory，需要先通过 GM 到 L1 的 Copy 原子对象搬入 L1 Buffer，再通过 L1 到 L0A/L0B 的 Copy 原子对象搬入 L0A 或 L0B。
- 支持的数据类型组合包括 `int32_t = int8_t * int8_t`、`float = half * half`、`float = float * float`、`float = bfloat16_t * bfloat16_t`、`float = fp8_e4m3fn_t * fp8_e4m3fn_t`、`float = fp8_e4m3fn_t * fp8_e5m2_t`、`float = fp8_e5m2_t * fp8_e4m3fn_t`、`float = fp8_e5m2_t * fp8_e5m2_t`、`float = hifloat8_t * hifloat8_t`。
- 带 bias 场景中，bias 位于 BiasTable Buffer 时支持 `int32_t` bias 配合 `int32_t = int8_t * int8_t`，以及 `float` bias 配合 float 输出的浮点计算组合。bias 位于 L0C 时，bias 数据类型需要与目的张量一致。
- `m`、`n`、`k` 的取值需要满足底层矩阵计算指令约束，通常为分形大小的整数倍或由布局保证有效分形覆盖。
- L0A、L0B、L0C 地址需要满足矩阵计算硬件对齐要求。

## 调用示例

```cpp
using namespace AscendC::Te;

auto gmA = MakeTensor(MakeMemPtr<Location::GM>(gmAAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(m, k));
auto gmB = MakeTensor(MakeMemPtr<Location::GM>(gmBAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(k, n));

auto l1A = MakeTensor(MakeMemPtr<Location::L1>(l1AAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));
auto l1B = MakeTensor(MakeMemPtr<Location::L1>(l1BAddr),
    MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(k, n));

auto l0A = MakeTensor(MakeMemPtr<Location::L0A>(l0aAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));
auto l0B = MakeTensor(MakeMemPtr<Location::L0B>(l0bAddr),
    MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(k, n));
auto l0C = MakeTensor(MakeMemPtr<Location::L0C>(l0cAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));

auto copyGm2L1 = MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>();
auto copyL12L0A = MakeCopy<CopyL12L0A, CopyL12L0ATraitDefault>();
auto copyL12L0B = MakeCopy<CopyL12L0B, CopyL12L0BTraitDefault>();

Copy(copyGm2L1, l1A, gmA);
Copy(copyGm2L1, l1B, gmB);
Copy(copyL12L0A, l0A, l1A);
Copy(copyL12L0B, l0B, l1B);

MmadParams params(m, n, k, 0, true);
auto atom = MakeMmad<MmadOperation, MmadTraitDefault>().with(params);
Mmad(atom, l0C, l0A, l0B);
```
