# MX矩阵计算

## 产品支持情况

| 产品 | 不带bias | 带bias |
| :--- | :------: | :-----: |
| Ascend 950PR/Ascend 950DT | √ | √ |

## 功能说明

头文件为：`#include "tensor_api/tensor.h"`

MX矩阵计算是Tensor API中通过`MmadTrait::mmadType`选择的矩阵乘加模式，用于低精度MX数据类型的矩阵计算。该模式仍以L0A为左矩阵、L0B为右矩阵、L0C为结果矩阵。

数学表达式为：

```text
C = A * B
```

带bias输入时，bias作为C矩阵初始值参与计算：

```text
C = Bias + A * B
```

Mmad的矩阵乘加关系可参考下图：

![Mmad矩阵乘加数学表达式](../../figures/mmad_formula.png)

MX计算通常需要先通过数据搬运流程准备A、B以及对应的ScaleA/ScaleB数据，再调用`Mmad`完成矩阵计算。

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
| `atomMmad` | 输入 | 矩阵计算原子对象。MX模式需要通过`MakeMmad(MmadOperation{}, MmadTraitMX{})`构造，其中`MmadTraitMX::value.mmadType`为`MmadType::MX`。需要配置`MmadParams`时，通过`.with(params)`生成携带参数的原子对象。 |
| `mmadOperation` | 输入 | 矩阵计算操作对象。MX矩阵计算使用`MmadOperation{}`。 |
| `mmadTrait` | 输入 | 矩阵计算Trait对象，用于指定矩阵计算特性。MX模式需要使用`mmadType`为`MmadType::MX`的Trait类型对象，例如`MmadTraitMX{}`。 |
| `dst` | 输出 | 结果矩阵C，存储位置必须为`Location::L0C`，数据类型通常为`float`。 |
| `fm` | 输入 | 左矩阵A，存储位置必须为`Location::L0A`，数据类型为MX支持的低精度类型。 |
| `filter` | 输入 | 右矩阵B，存储位置必须为`Location::L0B`，数据类型为MX支持的低精度类型。 |
| `bias` | 输入 | 可选bias张量，存储位置支持`Location::BIAS`或`Location::L0C`，数据类型为`float`。 |

**表 2** `MmadParams`参数说明

| 参数名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `m` | `uint16_t` | `0` | 左矩阵A的高度，也是结果矩阵C的高度。 |
| `n` | `uint16_t` | `0` | 右矩阵B的宽度，也是结果矩阵C的宽度。 |
| `k` | `uint16_t` | `0` | 左矩阵A的宽度，也是右矩阵B的高度。 |
| `unitFlag` | `uint8_t` | `0` | 控制`Mmad`和后续`Fixpipe`细粒度并行。 |
| `cmatrixInitVal` | `bool` | `false` | 不带bias调用时，控制是否初始化C矩阵。带bias调用时该配置无效。 |

**表 3** `MmadTrait`参数说明

| 参数名 | 类型 | MX模式推荐值 | 描述 |
| :--- | :--- | :--- | :--- |
| `fmOffset` | `int32_t` | `0` | 左矩阵offset，当前Tensor API实现中作为兼容参数保留。 |
| `kDirectionAlign` | `bool` | `false` | K方向对齐控制，当前Tensor API实现中作为兼容参数保留。 |
| `cmatrixSource` | `bool` | `false` | 不带bias调用时，配合`cmatrixInitVal`指示C矩阵初始值来源。 |
| `disableGemv` | `bool` | `true` | M=1 场景下是否关闭GEMV模式。 |
| `mmadType` | `MmadType` | `MmadType::MX` | 矩阵计算类型。MX矩阵计算必须设置为`MmadType::MX`。 |

MX模式需要通过自定义Trait类型设置`MmadType::MX`。自定义Trait构建形式如下：

```cpp
struct MmadTraitMX {
    using TraitType = MmadTrait;
    static constexpr const TraitType value = MmadTrait(0, false, false, true, MmadType::MX);
};
```

## 返回值说明

`Mmad`无返回值。`MakeMmad`返回`MmadAtom`对象。

## 数据类型

Ascend 950PR/Ascend 950DT MX矩阵计算支持如下A、B、C数据类型组合：

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

带bias场景中，bias位于`Location::BIAS`或`Location::L0C`时，bias数据类型均需要为`float`。

## 约束说明

- 该接口仅在AI Core侧执行。
- 调用`Mmad`时需要显式传入`MmadAtom`对象。
- `dst`必须位于L0C Buffer，`fm`必须位于L0A Buffer，`filter`必须位于L0B Buffer。
- MX模式需要通过`MakeMmad(MmadOperation{}, MmadTraitMX{})`构造`MmadAtom`，其中`MmadTraitMX::value.mmadType`为`MmadType::MX`。
- `dst`、`fm`、`filter`的Layout需要满足Tensor API对分形布局的编译期检查。
- MX计算通常需要在调用`Mmad`前完成ScaleA/ScaleB数据搬运。ScaleA搬运可参考[L1到L0ScaleA数据搬运](../data_movement/l1_to_l0scalea_data_movement.md)，ScaleB搬运可参考[L1到L0ScaleB数据搬运](../data_movement/l1_to_l0scaleb_data_movement.md)。
- `m`、`n`、`k`需要与参与计算的矩阵尺寸一致，并满足MX矩阵计算约束。
- 使用bias输入时，bias张量必须位于`Location::BIAS`或`Location::L0C`。

## 流水类型

```cpp
PIPE_M
```

## 调用示例

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

## 关键特性说明

### MX模式选择

MX模式不是单独的函数入口，而是通过`MmadTrait::mmadType`选择。自定义Trait类型需要提供`TraitType`和`value`，并通过`MakeMmad(MmadOperation{}, MmadTraitMX{})`构造用于MX模式的矩阵计算原子对象。

### Scale数据准备

MX数据类型计算通常需要配合scale数据。Scale数据不作为`Mmad`函数参数传入，而是在调用`Mmad`前通过[L1到L0ScaleA数据搬运](../data_movement/l1_to_l0scalea_data_movement.md)和[L1到L0ScaleB数据搬运](../data_movement/l1_to_l0scaleb_data_movement.md)准备到硬件要求的位置。

### Bias输入

带bias调用时，Tensor API根据bias张量位置决定C矩阵初始值来源。bias位于`Location::BIAS`时从BiasTable Buffer初始化，bias位于`Location::L0C`时从L0C初始化。
