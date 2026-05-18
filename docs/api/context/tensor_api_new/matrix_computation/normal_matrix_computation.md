# 普通矩阵计算

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

普通矩阵计算接口用于完成 L0A Buffer 左矩阵和 L0B Buffer 右矩阵的矩阵乘加，结果写入 L0C Buffer。数学表达式为：

```text
C = A * B
```

带 bias 输入时，bias 作为 C 矩阵初始值参与计算，等价于：

```text
C = Bias + A * B
```

Mmad 的矩阵乘加关系可参考下图：

![Mmad矩阵乘加数学表达式](../../figures/mmad_formula.png)

Tensor API 的 `Mmad` 使用显式传入的 `MmadAtom` 配置矩阵计算功能。A、B、C 的逻辑含义如下：

| 矩阵 | 物理位置 | 逻辑维度 | 推荐布局 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| A | `Location::L0A` | M * K | `NZLayoutPtn` | 左矩阵输入。 |
| B | `Location::L0B` | K * N | `ZNLayoutPtn` | 右矩阵输入。 |
| C | `Location::L0C` | M * N | `NZLayoutPtn` | 结果矩阵输出，也可作为累加初始值来源。 |
| Bias | `Location::BIAS` 或 `Location::L0C` | 1 * N 或 M * N | `NDLayoutPtn` / `NZLayoutPtn` | 可选输入，用于初始化 C 矩阵。 |

矩阵 A、B、C 的逻辑关系和物理存储位置如下图所示：

![Mmad矩阵计算示意图](../../figures/mmad_matrix_layout.png)

## 函数原型

- 执行不带 bias 的矩阵计算。

    ```cpp
    template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor>
    __aicore__ inline void Mmad(
        const MmadAtom<AtomType>& atomMmad, const DstTensor& dst, const FmTensor& fm, const FilterTensor& filter);
    ```

- 执行带 bias 的矩阵计算。

    ```cpp
    template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor, typename BiasTensor,
        Std::enable_if_t<IsAttrTensorV<BiasTensor>, int> Enable>
    __aicore__ inline void Mmad(
        const MmadAtom<AtomType>& atomMmad, const DstTensor& dst, const FmTensor& fm, const FilterTensor& filter,
        const BiasTensor& bias);
    ```

- 构造默认矩阵计算原子对象。

    ```cpp
    template <typename MmadOperationType>
    __aicore__ inline constexpr auto MakeMmad();
    ```

- 构造指定 Trait 的矩阵计算原子对象。

    ```cpp
    template <typename MmadOperationType, typename MmadTraitType>
    __aicore__ inline constexpr auto MakeMmad();
    ```

常用调用形式如下：

```cpp
MmadParams params(m, n, k, unitFlag, cmatrixInitVal);
auto atom = MakeMmad<MmadOperation, MmadTraitDefault>().with(params);
Mmad(atom, dst, fm, filter);
Mmad(atom, dst, fm, filter, bias);
```

## 参数说明

**表 1** 通用参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomMmad` | 输入 | 矩阵计算原子对象。普通矩阵计算可通过 `MakeMmad<MmadOperation>()` 或 `MakeMmad<MmadOperation, MmadTraitDefault>()` 构造；需要配置 `MmadParams` 时，通过 `.with(params)` 生成携带参数的原子对象。 |
| `dst` | 输出 | 结果矩阵 C，存储位置必须为 `Location::L0C`。 |
| `fm` | 输入 | 左矩阵 A，存储位置必须为 `Location::L0A`。 |
| `filter` | 输入 | 右矩阵 B，存储位置必须为 `Location::L0B`。 |
| `bias` | 输入 | 可选 bias 张量，存储位置支持 `Location::BIAS` 或 `Location::L0C`。 |

**表 2** `MmadParams` 参数说明

| 参数名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `m` | `uint16_t` | `0` | 左矩阵 A 的高度，也是结果矩阵 C 的高度。 |
| `n` | `uint16_t` | `0` | 右矩阵 B 的宽度，也是结果矩阵 C 的宽度。 |
| `k` | `uint16_t` | `0` | 左矩阵 A 的宽度，也是右矩阵 B 的高度。 |
| `unitFlag` | `uint8_t` | `0` | 控制 `Mmad` 和后续 `Fixpipe` 细粒度并行。`0` 表示不使能；`2` 表示使能且执行后不复位单元标记位；`3` 表示使能且执行后复位单元标记位。 |
| `cmatrixInitVal` | `bool` | `false` | 不带 bias 调用时，控制是否初始化 C 矩阵。`true` 表示初始化后计算，`false` 表示基于 C 矩阵原值累加。 |

**表 3** `MmadTrait` 参数说明

| 参数名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `fmOffset` | `int32_t` | `0` | 左矩阵 offset，当前 Tensor API 实现中作为兼容参数保留。 |
| `kDirectionAlign` | `bool` | `false` | K 方向对齐控制，当前 Tensor API 实现中作为兼容参数保留。 |
| `cmatrixSource` | `bool` | `false` | 不带 bias 调用时，配合 `cmatrixInitVal` 指示 C 矩阵初始值来源。带 bias 调用时该配置无效，由 bias 张量位置决定。 |
| `disableGemv` | `bool` | `true` | M=1 场景下是否关闭 GEMV 模式。`false` 表示开启 GEMV，`true` 表示关闭 GEMV。 |
| `mmadType` | `MmadType` | `MmadType::NORMAL` | 矩阵计算类型。普通矩阵计算使用 `MmadType::NORMAL`。 |

## 返回值说明

`Mmad` 无返回值。`MakeMmad` 返回 `MmadAtom` 对象。

## 数据类型

Ascend 950PR/Ascend 950DT 普通矩阵计算支持如下 A、B、C 数据类型组合：

| A 数据类型 | B 数据类型 | C 数据类型 |
| :--- | :--- | :--- |
| `int8_t` | `int8_t` | `int32_t` |
| `half` | `half` | `float` |
| `float` | `float` | `float` |
| `bfloat16_t` | `bfloat16_t` | `float` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` | `float` |
| `fp8_e4m3fn_t` | `fp8_e5m2_t` | `float` |
| `fp8_e5m2_t` | `fp8_e4m3fn_t` | `float` |
| `fp8_e5m2_t` | `fp8_e5m2_t` | `float` |
| `hifloat8_t` | `hifloat8_t` | `float` |

带 bias 场景中：

- 当 bias 位于 `Location::BIAS` 时，`int8_t * int8_t -> int32_t` 组合使用 `int32_t` bias，其余输出为 `float` 的组合使用 `float` bias。
- 当 bias 位于 `Location::L0C` 时，bias 数据类型需要与 C 数据类型一致。
- `hifloat8_t * hifloat8_t -> float` 场景由 Tensor API 在调用 `Mmad` 时设置对应控制位，并按硬件要求执行计算。

## 约束说明

- 该接口仅在 AI Core 侧执行。
- 调用 `Mmad` 时需要显式传入 `MmadAtom` 对象。
- `dst` 必须位于 L0C Buffer，`fm` 必须位于 L0A Buffer，`filter` 必须位于 L0B Buffer。
- `dst`、`fm`、`filter` 的 Layout 需要满足 Tensor API 对 `NZLayoutPtn`、`ZNLayoutPtn` 等分形布局的编译期检查。
- L0C 起始地址需要满足矩阵计算硬件对齐要求；L0A、L0B 起始地址需要满足对应输入分形对齐要求。
- `m`、`n`、`k` 需要与参与计算的矩阵尺寸一致，并满足矩阵计算约束。
- 使用 bias 输入时，bias 张量必须位于 `Location::BIAS` 或 `Location::L0C`。
- 使用 `unitFlag` 时，通常需要让后续 L0C 搬出接口的 `FixpipeParams::unitFlag` 与 `MmadParams::unitFlag` 配合设置。

当 M、K、N 不是 16 的倍数时，硬件仍以 16 * 16 分形块组织数据，尾块中的无效数据会占用分形块空间但不参与有效计算。有效数据与无效数据排布方式如下图所示：

![非16倍数场景中有效数据与无效数据示意图](../../figures/mmad_tail_valid_data.png)

## 流水类型

```cpp
PIPE_M
```

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void NormalMmadExample()
{
    using namespace AscendC::Te;

    constexpr uint16_t m = 16;
    constexpr uint16_t n = 16;
    constexpr uint16_t k = 16;

    __ca__ half l0aBuf[m * k];
    __cb__ half l0bBuf[k * n];
    __cc__ float l0cBuf[m * n];

    auto l0A = MakeTensor(
        MakeMemPtr(l0aBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));
    auto l0B = MakeTensor(
        MakeMemPtr(l0bBuf),
        MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(k, n));
    auto l0C = MakeTensor(
        MakeMemPtr(l0cBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));

    MmadParams params(m, n, k, 0, true);
    auto atom = MakeMmad<MmadOperation, MmadTraitDefault>().with(params);
    Mmad(atom, l0C, l0A, l0B);
}
```

## 关键特性说明

### C矩阵初始化

不带 bias 调用时，`cmatrixInitVal` 控制是否初始化 C 矩阵。需要计算新结果时，通常第一次 K 方向累加设置为 `true`；后续 K 分块累加设置为 `false`。

带 bias 调用时，Tensor API 根据 bias 张量位置确定 C 矩阵初始值来源：bias 位于 `Location::BIAS` 时从 BiasTable Buffer 初始化，bias 位于 `Location::L0C` 时从 L0C 初始化。

连续两次 `Mmad` 沿 K 方向累加时，通常需要关注中间结果的写读依赖。同步优化的阈值关系可参考下图：

![同步优化阈值公式](../../figures/mmad_pipebarrier_threshold.png)

### UnitFlag

`unitFlag` 用于控制 `Mmad` 与后续 `Fixpipe` 的细粒度并行。启用时，`MmadParams::unitFlag` 和结果搬出的 `FixpipeParams::unitFlag` 需要配合设置，常见取值为 `2` 和 `3`。

### GEMV模式

当 M=1 时，可通过 `MmadTrait::disableGemv` 控制是否开启 GEMV 模式。默认值为 `true`，表示关闭 GEMV。

### Trait配置

如需覆盖 `MmadTrait`，可定义包含 `TraitType` 和 `value` 的 Trait 类型，并通过 `MakeMmad<MmadOperation, CustomMmadTrait>()` 构造对应的 `MmadAtom`。
