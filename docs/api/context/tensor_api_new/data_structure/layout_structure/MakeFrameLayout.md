# MakeFrameLayout

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

MakeFrameLayout 用于根据标准分形排布模式构造带类型信息的Layout对象。该接口接受矩阵的行数、列数，结合指定的 LayoutPattern（排布模式）和 LayoutTrait（布局特征参数），在编译期计算 shape 和 stride。

`LayoutPattern` 决定数据在内存中的分形排列方式，是 Tensor API 实现高效数据搬运和矩阵计算的核心前提。不同存储位置间的搬运和计算操作要求源/目的张量满足特定的 `LayoutPattern` 组合。

### LayoutTrait 的指定方式

`LayoutTrait` 用于指定数据类型和 C0 维度基数。`MakeFrameLayout` 支持以下三种传递方式：

1. **`LayoutTraitDefault<T>`**：指定数据类型 `T`，C0自动计算。
   ```cpp
   MakeFrameLayout<NzLayoutPtn, LayoutTraitDefault<float>>(m, n)
   ```

2. **`_C0`**：直接指定 C0 维度的元素数，数据类型使用默认值,_C0是Std::Int<C0>的简写形式。
   ```cpp
   MakeFrameLayout<NzLayoutPtn, _16>(m, n)
   ```

3. **不传入 Trait**：在分形构造和Trait无关，或Trait值固定的场景下，不传入 Trait，内部根据 `LayoutPattern` 查找对应的默认 Trait。这样的分形有NDLayoutPtn, DNLayoutPtn, DNExtLayoutPtn, NDExtLayoutPtn, ScaleANDLayoutPtn, ScaleADNLayoutPtn, ScaleBNDLayoutPtn, ScaleBDNLayoutPtn。
   ```cpp
   MakeFrameLayout<NDLayoutPtn>(m, n)
   ```

### 支持的 LayoutPattern

| LayoutPattern | 全称 | 说明 | 典型用途 |
| :--- | :--- | :--- | :--- |
| `NDLayoutPtn` | N-Dimension | 连续排列，无分形变换 | L1/UB 通用连续布局 |
| `NDExtLayoutPtn` | N-Dimension Extended | 带外维度的连续排列 | GM 端连续数据描述 |
| `NZLayoutPtn` | N-Zigzag | 矩阵分形排列，行方向按 FRACTAL_FIXED 分块 | L1 → L0A/L0B 搬运的标准输入格式 |
| `ZNLayoutPtn` | Zigzag-N | NZ 的转置形式 | L1 → L0B/L0A 搬运的转置输入格式 |
| `DNLayoutPtn` | Deinterleave-N | 列主序排列 | 反交织数据场景 |
| `DNExtLayoutPtn` | Deinterleave-N Extended | 带外维度的列主序排列 | GM 端反交织数据描述 |
| `NNLayoutPtn` | N-N | Scale 数据的 N-N 分形排列 | MX ScaleB 矩阵计算 |
| `ZZLayoutPtn` | Zigzag-Zigzag | 行列双向分形排列 | MX Scale 搬运 |
| `ScaleANDLayoutPtn` | ScaleA N-Dimension | ScaleA 连续排列 | MX ScaleA 输入 |
| `ScaleADNLayoutPtn` | ScaleA Deinterleave-N | ScaleA 反交织排列 | MX ScaleA 输入 |
| `ScaleBNDLayoutPtn` | ScaleB N-Dimension | ScaleB 连续排列 | MX ScaleB 输入 |
| `ScaleBDNLayoutPtn` | ScaleB Deinterleave-N | ScaleB 反交织排列 | MX ScaleB 输入 |

## 函数原型

- 指定 `LayoutPattern` 和 `LayoutTrait`。

    ```cpp
    template <typename LayoutPattern, typename TraitType, typename... Args>
    __aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args)
    ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `LayoutPattern` | 编译期 | 布局模式模板参数，可选值见上表。 |
| `TraitType` | 编译期 | 布局特征类型，可为 `LayoutTraitDefault<T>`、`LayoutTraitScale`、`LayoutTraitFP4`、`Std::ignore_t` 或自定义整型常量 `Std::Int<C0>`。默认内部根据 `LayoutPattern` 自动推导。 |
| `C0Element` | 编译期 | C0 维度的元素数。配合默认 Trait 类型使用，不指定时根据 `LayoutPattern` 自动确定。 |
| `Args` | 输入 | 构造参数。常见为矩阵的行数 `m` 和列数 `n`。各 `LayoutPattern` 的构造参数数量可能不同。 |

## 返回值说明

返回 `Layout<ShapeType, StrideType, Std::tuple<LayoutPattern, TraitType>>` 类型的对象，其中 `ShapeType` 和 `StrideType` 由 `LayoutPattern` 和构造参数在编译期推导。

## 约束说明

- `LayoutPattern` 必须是 12 种已支持的排布模式之一，不支持的模式会触发编译期 `static_assert` 报错。
- 构造参数的个数和含义由各 `LayoutPattern` 决定。常见二维场景传入 `(m, n)` 即可。
- `TraitType` 必须为整型常量、TraitType 类型或具体数据类型。
- MX 相关 `LayoutPattern`（NN / ZZ / ScaleA / ScaleB 系列）要求 C0 元素数为 2，仅支持 `fp8_e8m0_t` 类型。

## 调用示例

```cpp
using namespace AscendC::Te;

// 构造 GM 端 ND 布局张量
auto layoutGM = MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(m, n);
auto gmTensor = MakeTensor(MakeMemPtr<Location::GM>(gmAddr), layoutGM);

// 构造 L1 端 NZ 布局张量（L0A/L0B 搬运的标准格式）
auto layoutL1 = MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float>>(m, n);
auto l1Tensor = MakeTensor(MakeMemPtr<Location::L1>(l1Addr), layoutL1);

// 使用默认 Trait 构造 NZ 布局（自动推导为 LayoutTraitDefault<uint16_t>）
auto layoutNZ = MakeFrameLayout<NZLayoutPtn>(m, n);
auto l1TensorNZ = MakeTensor(MakeMemPtr<Location::L1>(l1Addr), layoutNZ);

// 指定 C0 元素数构造
auto layoutC0 = MakeFrameLayout<NZLayoutPtn, 16>(m, n);
auto l1TensorC0 = MakeTensor(MakeMemPtr<Location::L1>(l1Addr), layoutC0);

// MX Scale 场景
auto layoutScaleA = MakeFrameLayout<ScaleANDLayoutPtn, LayoutTraitScale>(m, n);
auto scaleATensor = MakeTensor(MakeMemPtr<Location::L1>(scaleAddr), layoutScaleA);
```
