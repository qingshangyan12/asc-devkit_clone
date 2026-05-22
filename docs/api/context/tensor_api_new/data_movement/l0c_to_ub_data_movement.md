# L0C到UB数据搬运

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

该接口用于将 L0C Buffer 中的矩阵计算结果搬运到 UB。L0C 中的数据通常为 `Mmad` 的输出，数据布局为 NZ。搬运到 UB 时，接口会根据目的张量布局自动选择 NZ 到 ND、NZ 到 DN 或 NZ 到 NZ 的随路格式转换。

L0C 到 UB 搬运支持不量化输出、float 到 half 或 bfloat16_t 的直接转换输出，以及配合标量或张量量化参数的随路量化输出。随路 Relu、双目的模式和舍入方式通过 `CopyL0C2UBTrait` 配置；Mmad 与 Fixpipe 细粒度并行相关的 `unitFlag` 通过 `FixpipeParams` 配置。

随路量化、随路 Relu、随路格式转换、随路通道拆分以及随路通道合并的有效组合、中间数据类型和数据路径如下图所示。图中的 F32 到 F16、F32 到 BF16 为非量化模式，仅进行 cast；其余路径为随路 scalar 或 tensor 量化模式。针对 Ascend 950PR/Ascend 950DT，还支持 NZ2DN 随路格式转换。。

**图 1** L0C2UB流程图

![](figures/L0C2UB流程图.png "L0C2UB流程图")

## 函数原型

```cpp
template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src)

template <typename AtomType, typename DstTensor, typename SrcTensor, typename QuantParam,
    Std::enable_if_t<IsCopyQuantParamV<QuantParam>, int> Enable>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src,
    const QuantParam& quant)

template <typename CopyOperationType>
__aicore__ inline constexpr auto MakeCopy()

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto MakeCopy()
```

常用调用形式如下：

```cpp
auto atom = MakeCopy<CopyL0C2UB>().with(fixpipeParams);
Copy(atom, dst, src);
Copy(atom, dst, src, quant);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `dst` | 输出 | 目的张量，存储位置为 `Location::UB`。支持 `NDExtLayoutPtn`、`DNExtLayoutPtn` 和 `NZLayoutPtn`。目的布局为 ND 时使能 NZ2ND，目的布局为 DN 时使能 NZ2DN，目的布局为 NZ 时执行 NZ2NZ。 |
| `src` | 输入 | 源张量，存储位置为 `Location::L0C`，布局为 `NZLayoutPtn`，通常为 `Mmad` 的计算结果。 |
| `quant` | 输入 | 可选量化参数。传入 `uint64_t` 时表示 scalar 量化参数；传入 Tensor API 张量时表示 tensor 量化参数，张量通常位于 L1，元素类型为 `uint64_t`。 |
| `fixpipeParams` | 输入 | 可选搬运参数，类型为 `FixpipeParams`，通过 `MakeCopy<CopyL0C2UB>().with(fixpipeParams)` 绑定到 atom。未绑定时使用默认值。 |
| `atomCopy` | 输入 | 搬运原子对象，可由 `MakeCopy<CopyL0C2UB>()` 或 `MakeCopy<CopyL0C2UB, CopyL0C2UBTraitDefault>()` 构造。 |
| `traits` | 输入 | `MakeCopy` 的模板参数，用于指定 `CopyL0C2UB` 和对应 trait。 |

## CopyL0C2UBTrait说明

`CopyL0C2UBTrait` 用于配置 L0C 到 UB 搬运的编译期行为。

```cpp
struct CopyL0C2UBTrait {
    RoundMode roundMode = RoundMode::DEFAULT;
    bool enableRelu = false;
    bool enableChannelSplit = false;
    DualDstMode dualDstCtl = DUAL_DST_DISABLE;
};
```

| 成员 | 默认值 | 描述 |
| :--- | :--- | :--- |
| `roundMode` | `RoundMode::DEFAULT` | 舍入模式。`RoundMode::HYBRID` 仅在源类型为 `float`、目的类型为 `hifloat8_t` 的量化输出场景支持。 |
| `enableRelu` | `false` | 是否使能随路 Relu。 |
| `enableChannelSplit` | `false` | 是否使能 F32 Channel Split。 |
| `dualDstCtl` | `DUAL_DST_DISABLE` | 双目标模式控制，取值包括 `DUAL_DST_DISABLE`、`DUAL_DST_SPLIT_M`、`DUAL_DST_SPLIT_N`。详细说明参见 [L0C到UB双目标模式](l0c_to_ub_dual_dst.md)。 |

使用自定义 trait 的示例：

```cpp
constexpr CopyL0C2UBTrait l0c2ubTrait = {
    RoundMode::DEFAULT,
    false,
    false,
    DUAL_DST_DISABLE
};

struct CopyL0C2UBTraitCustom {
    using TraitType = CopyL0C2UBTrait;
    static constexpr const TraitType value = l0c2ubTrait;
};
```

## FixpipeParams说明

`FixpipeParams` 用于配置运行时搬运参数。

```cpp
struct FixpipeParams {
    uint8_t unitFlag = 0;
};
```

| 成员 | 默认值 | 描述 |
| :--- | :--- | :--- |
| `unitFlag` | `0` | 控制 Mmad 指令和 Fixpipe 指令的细粒度并行。`0` 表示不使能；`2` 表示使能且执行后不复位单元标记位；`3` 表示使能且执行后复位单元标记位。使能时，`MmadParams` 和 `FixpipeParams` 中的 `unitFlag` 需要配套设置为 `2` 或 `3`。 |

## 数据类型

L0C 到 UB 搬运根据是否传入量化参数自动选择量化模式。

| 源张量类型（L0C） | 目的张量类型（UB） | 调用形式 | 说明 |
| :--- | :--- | :--- | :--- |
| `float` | `float` | `Copy(atom, dst, src)` | 不量化输出。 |
| `int32_t` | `int32_t` | `Copy(atom, dst, src)` | 不量化输出。 |
| `float` | `half` | `Copy(atom, dst, src)` | 直接转换输出，对应 F32 到 F16。 |
| `float` | `bfloat16_t` | `Copy(atom, dst, src)` | 直接转换输出，对应 F32 到 BF16。 |
| `float` | `int8_t`、`uint8_t` | `Copy(atom, dst, src, quant)` | scalar 或 tensor 量化输出。 |
| `float` | `fp8_e4m3fn_t`、`hifloat8_t` | `Copy(atom, dst, src, quant)` | scalar 或 tensor 量化输出。`hifloat8_t` 支持 `RoundMode::DEFAULT` 和 `RoundMode::HYBRID`。 |
| `float` | `half`、`bfloat16_t`、`float` | `Copy(atom, dst, src, quant)` | scalar 或 tensor 量化输出。 |
| `int32_t` | `half`、`bfloat16_t`、`int8_t`、`uint8_t` | `Copy(atom, dst, src, quant)` | scalar 或 tensor 量化输出。 |

不传入 `quant` 时，接口选择不量化或直接转换模式；传入 `uint64_t` 时选择 scalar 量化模式；传入 Tensor API 张量时选择 tensor 量化模式。

## 布局和格式转换

| 源布局 | 目的布局 | 随路转换 | 说明 |
| :--- | :--- | :--- | :--- |
| `NZLayoutPtn` | `NDExtLayoutPtn` | NZ2ND | 将 L0C NZ 结果输出为 UB 上的 ND 排布。 |
| `NZLayoutPtn` | `DNExtLayoutPtn` | NZ2DN | 将 L0C NZ 结果输出为 UB 上的 DN 排布。 |
| `NZLayoutPtn` | `NZLayoutPtn` | NZ2NZ | 将 L0C NZ 结果输出为 UB 上的 NZ 排布。 |

底层搬运接口的 `nSize`、`mSize`、`srcStride`、`dstStride` 等参数由源张量和目的张量的布局描述推导得到。调用者需要保证布局中的 shape、stride 与实际内存空间一致。

## 关键特性

L0C 到 UB 搬运涉及 [随路量化](quant_pre.md)、[随路Relu](relu_pre.md)、[Int8/Int4 Channel Merge](int8_int4_channel_merge.md)、[L0C到UB双目标模式](l0c_to_ub_dual_dst.md) 等关键特性。

## 返回值说明

`Copy` 无返回值。`MakeCopy` 返回 `CopyAtom` 对象。

## 约束说明

- 目的张量必须位于 UB，源张量必须位于 L0C Buffer。
- 源张量布局必须为 `NZLayoutPtn`。目的张量支持 `NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`。
- 源 L0C NZ 地址要求 64 字节对齐。目的 UB 地址要求 32 字节对齐。
- 当搬运的 M 方向大小、N 方向大小为 0 时，底层搬运指令不会执行。
- N 方向大小取值范围为 `[0, 4095]`。目的矩阵为 NZ 输出时，通常要求 N 方向大小为 16 的倍数。
- tensor 量化参数张量应位于 L1，元素类型为 `uint64_t`，地址要求 32 字节对齐。详细约束参见 [随路量化](quant_pre.md)。
- 量化和 Relu 参数不能为 INF、NaN 或非规格化数。随路 Relu 详细约束参见 [随路Relu](relu_pre.md)。
- 当前不支持随路 pRelu 操作，参见 [随路Relu](relu_pre.md)。
- 目的地址写入区域不能重叠。若目的地址存在重叠写入，硬件不保证写入顺序。
- 使能 `unitFlag` 时，需要配合 `Mmad` 同时使能。
- 使用 `RoundMode::HYBRID` 时，源类型必须为 `float`，目的类型必须为 `hifloat8_t`。
- 当前搬出到 UB 不支持 F32 Channel Split，`enableChannelSplit` 应保持默认值 `false`。详细约束参见 [F32 Channel Split](f32_channel_split.md)。
- 目的布局为 `NZLayoutPtn` 时，不支持 `dualDstCtl` 设置为 `DUAL_DST_SPLIT_N`。双目标模式详细约束参见 [L0C到UB双目标模式](l0c_to_ub_dual_dst.md)。
- Channel Merge 为硬件自动使能，不能通过参数配置。详细约束参见 [Int8/Int4 Channel Merge](int8_int4_channel_merge.md)。

## 调用示例

### 不量化输出到ND

```cpp
using namespace AscendC::Te;

auto l0c = MakeTensor(MakeMemPtr<Location::L0C, float>(l0cAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));
auto ub = MakeTensor(MakeMemPtr<Location::UB, float>(ubAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(m, n));

FixpipeParams params;
auto atom = MakeCopy<CopyL0C2UB>().with(params);
Copy(atom, ub, l0c);
```

### 显式指定搬运原子对象

```cpp
using namespace AscendC::Te;

auto atom = MakeCopy<CopyL0C2UB, CopyL0C2UBTraitDefault>().with(FixpipeParams{});
Copy(atom, ub, l0c);
```

### scalar量化输出

```cpp
using namespace AscendC::Te;

auto l0c = MakeTensor(MakeMemPtr<Location::L0C, int32_t>(l0cAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<int32_t, 16>>(m, n));
auto ub = MakeTensor(MakeMemPtr<Location::UB, int8_t>(ubAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<int8_t>>(m, n));

uint64_t quant = 1;
auto atom = MakeCopy<CopyL0C2UB>().with(FixpipeParams{});
Copy(atom, ub, l0c, quant);
```

### tensor量化输出

```cpp
using namespace AscendC::Te;

auto l0c = MakeTensor(MakeMemPtr<Location::L0C, float>(l0cAddr),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, 16>>(m, n));
auto ub = MakeTensor(MakeMemPtr<Location::UB, half>(ubAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<half>>(m, n));
auto quant = MakeTensor(MakeMemPtr<Location::L1, uint64_t>(quantAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<uint64_t>>(1, n));

auto atom = MakeCopy<CopyL0C2UB>().with(FixpipeParams{});
Copy(atom, ub, l0c, quant);
```
