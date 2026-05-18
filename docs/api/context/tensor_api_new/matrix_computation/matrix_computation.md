# 矩阵计算

## 产品支持情况

| 产品 | 不带bias | 带bias |
| :--- | :------: | :-----: |
| Ascend 950PR/Ascend 950DT | √ | √ |

## 功能说明

头文件为：`#include "tensor_api/tensor.h"`

矩阵计算接口用于完成L0A Buffer左矩阵和L0B Buffer右矩阵的矩阵乘加，结果写入L0C Buffer。默认模式为普通矩阵计算。数学表达式为：

```text
C = A * B
```

带bias输入时，bias作为C矩阵初始值参与计算，等价于：

```text
C = Bias + A * B
```

Mmad的矩阵乘加关系可参考下图：

![Mmad矩阵乘加数学表达式](../../figures/mmad_formula.png)

Tensor API的`Mmad`使用显式传入的`MmadAtom`配置矩阵计算功能。A、B、C的逻辑含义如下：

矩阵A、B、C的逻辑关系和物理存储位置如下图所示：

![Mmad矩阵计算示意图](../../figures/mmad_matrix_layout.png)

## 函数原型

- 执行不带bias的矩阵计算。

    ```cpp
    template <typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor>
    __aicore__ inline void Mmad(
        const MmadAtom<AtomType>& atomMmad, const DstTensor& dst, const FmTensor& fm, const FilterTensor& filter);
    ```

- 执行带bias的矩阵计算。

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
    __aicore__ inline constexpr auto MakeMmad(const MmadOperationType& mmadOperation);
    ```

- 构造指定Trait的矩阵计算原子对象。

    ```cpp
    template <typename MmadOperationType, typename MmadTraitType>
    __aicore__ inline constexpr auto MakeMmad(
        const MmadOperationType& mmadOperation, const MmadTraitType& mmadTrait);
    ```

## 参数说明

**表 1** 通用参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomMmad` | 输入 | 矩阵计算原子对象。默认模式可通过`MakeMmad(MmadOperation{})`或`MakeMmad(MmadOperation{}, MmadTraitDefault{})`构造；需要配置`MmadParams`时，通过`.with(params)`生成携带参数的原子对象。 |
| `mmadOperation` | 输入 | 矩阵计算操作对象，使用`MmadOperation{}`。 |
| `mmadTrait` | 输入 | 矩阵计算Trait对象，用于指定矩阵计算特性。默认模式使用`MmadTraitDefault{}`；MX Mmad模式需要使用`mmadType`为`MmadType::MX`的Trait类型对象。 |
| `dst` | 输出 | 结果矩阵C，存储位置必须为`Location::L0C`。 |
| `fm` | 输入 | 左矩阵A，存储位置必须为`Location::L0A`。 |
| `filter` | 输入 | 右矩阵B，存储位置必须为`Location::L0B`。 |
| `bias` | 输入 | 可选bias张量，存储位置支持`Location::BIAS`或`Location::L0C`。 |

**表 2** `MmadParams`参数说明

| 参数名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `m` | `uint16_t` | `0` | 左矩阵A的高度，结果矩阵C的高度。 |
| `n` | `uint16_t` | `0` | 右矩阵B的宽度，结果矩阵C的宽度。 |
| `k` | `uint16_t` | `0` | 左矩阵A的宽度，右矩阵B的高度。 |
| `unitFlag` | `uint8_t` | `0` | 控制`Mmad`和后续`Fixpipe`细粒度并行。`0`表示不使能；`2`表示使能且执行后不复位单元标记位；`3`表示使能且执行后复位单元标记位。 |
| `cmatrixInitVal` | `bool` | `false` | 不带bias调用时，控制是否初始化C矩阵。`true`表示初始化后计算，`false`表示基于C矩阵原值累加。 |

**表 3** `MmadTrait`参数说明

| 参数名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `fmOffset` | `int32_t` | `0` | 左矩阵offset，当前Tensor API实现中作为兼容参数保留。 |
| `kDirectionAlign` | `bool` | `false` | K方向对齐控制，当前Tensor API实现中作为兼容参数保留。 |
| `cmatrixSource` | `bool` | `false` | 不带bias调用时，配合`cmatrixInitVal`指示C矩阵初始值来源。带bias调用时该配置无效，由bias张量位置决定。 |
| `disableGemv` | `bool` | `true` | M=1 场景下是否关闭GEMV模式。`false`表示开启GEMV，`true`表示关闭GEMV。 |
| `mmadType` | `MmadType` | `MmadType::NORMAL` | 矩阵计算类型。默认模式使用`MmadType::NORMAL`，MX Mmad模式使用`MmadType::MX`。 |

## 返回值说明

`Mmad`无返回值。`MakeMmad`返回`MmadAtom`对象。

## 数据类型

Ascend 950PR/Ascend 950DT默认模式支持如下A、B、C数据类型组合：

| A数据类型 | B数据类型 | C数据类型 |
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

带bias场景中：

- 当bias位于`Location::BIAS`时，`int8_t * int8_t -> int32_t`组合使用`int32_t` bias，其余输出为`float`的组合使用`float` bias。
- 当bias位于`Location::L0C`时，bias数据类型需要与C数据类型一致。
- `hifloat8_t * hifloat8_t -> float`场景由Tensor API在调用`Mmad`时设置对应控制位，并按硬件要求执行计算。

## 约束说明

- 调用`Mmad`时需要显式传入`MmadAtom`对象。
- `dst`必须位于L0C Buffer，`fm`必须位于L0A Buffer，`filter`必须位于L0B Buffer。
- `dst`、`fm`、`filter`的Layout需要满足Tensor API对`NZLayoutPtn`、`ZNLayoutPtn`等分形布局的编译期检查。
- L0C起始地址需要满足矩阵计算硬件对齐要求；L0A、L0B起始地址需要满足对应输入分形对齐要求。
- `m`、`n`、`k`需要与参与计算的矩阵尺寸一致，并满足矩阵计算约束。
- 使用bias输入时，bias张量必须位于`Location::BIAS`或`Location::L0C`。
- 使用`unitFlag`时，通常需要让后续L0C搬出接口的`FixpipeParams::unitFlag`与`MmadParams::unitFlag`配合设置。
- 当M、K、N不是 16 的倍数时，硬件仍以 16 * 16 分形块组织数据，尾块中的无效数据会占用分形块空间但不参与有效计算。有效数据与无效数据排布方式如下图所示：

![非16倍数场景中有效数据与无效数据示意图](../../figures/mmad_tail_valid_data.png)

## 关键特性说明

### C矩阵初始化

不带bias调用时，`cmatrixInitVal`控制是否初始化C矩阵。需要计算新结果时，通常第一次K方向累加设置为`true`；后续K分块累加设置为`false`。

带bias调用时，Tensor API根据bias张量位置确定C矩阵初始值来源：bias位于`Location::BIAS`时从BiasTable Buffer初始化，bias位于`Location::L0C`时从L0C初始化。

连续两次`Mmad`沿K方向累加时，通常需要关注中间结果的写读依赖。同步优化的阈值关系可参考下图：

![同步优化阈值公式](../../figures/mmad_pipebarrier_threshold.png)

### UnitFlag

`unitFlag`用于控制`Mmad`与后续`Fixpipe`的细粒度并行。启用时，`MmadParams::unitFlag`和结果搬出的`FixpipeParams::unitFlag`需要配合设置，常见取值为`2`和`3`。

### GEMV模式

当M=1 时，可通过`MmadTrait::disableGemv`控制是否开启GEMV模式。默认值为`true`，表示关闭GEMV。

### MX Mmad模式

MX Mmad模式不是单独的函数入口，而是通过`MmadTrait::mmadType`选择。使用MX Mmad时，需要将`mmadType`设置为`MmadType::MX`，并通过`MakeMmad(MmadOperation{}, MmadTraitMX{})`构造矩阵计算原子对象。

```cpp
struct MmadTraitMX {
    using TraitType = MmadTrait;
    static constexpr const TraitType value = MmadTrait(0, false, false, true, MmadType::MX);
};
```

MX Mmad支持如下A、B、C数据类型组合：

| A数据类型 | B数据类型 | C数据类型 |
| :--- | :--- | :--- |
| `fp4x2_e2m1_t` | `fp4x2_e2m1_t` | `float` |
| `fp4x2_e2m1_t` | `fp4x2_e1m2_t` | `float` |
| `fp4x2_e1m2_t` | `fp4x2_e2m1_t` | `float` |
| `fp4x2_e1m2_t` | `fp4x2_e1m2_t` | `float` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` | `float` |
| `fp8_e4m3fn_t` | `fp8_e5m2_t` | `float` |
| `fp8_e5m2_t` | `fp8_e4m3fn_t` | `float` |
| `fp8_e5m2_t` | `fp8_e5m2_t` | `float` |

MX Mmad通常需要在调用`Mmad`前完成ScaleA和ScaleB数据搬运。ScaleA搬运可参考[L1到L0ScaleA数据搬运](../data_movement/l1_to_l0scalea_data_movement.md)，ScaleB搬运可参考[L1到L0ScaleB数据搬运](../data_movement/l1_to_l0scaleb_data_movement.md)。Scale数据不作为`Mmad`函数参数传入，而是由对应数据搬运接口提前准备到硬件要求的位置。

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
    auto atom = MakeMmad(MmadOperation{}, MmadTraitDefault{}).with(params);
    Mmad(atom, l0C, l0A, l0B);
}
```

- MX Mmad调用示例

```cpp
#include "tensor_api/tensor.h"

struct MmadTraitMX {
    using TraitType = AscendC::Te::MmadTrait;
    static constexpr const TraitType value = AscendC::Te::MmadTrait(
        0, false, false, true, AscendC::Te::MmadType::MX);
};

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
    auto atom = MakeMmad(MmadOperation{}, MmadTraitMX{}).with(params);
    Mmad(atom, l0C, l0A, l0B);
}
```
