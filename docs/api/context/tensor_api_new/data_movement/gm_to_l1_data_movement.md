# GM到L1数据搬运

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |


## 功能说明

头文件为: #include "tensor_api/tensor.h"

该接口用于将Global Memory中的数据搬运到L1 Buffer。Tensor API通过`Copy`统一执行搬运，Global Memory到L1 Buffer通路需要使用`CopyGM2L1`构造搬运原子对象，再将该对象传入`Copy`。接口根据源张量和目的张量的存储位置、数据类型和Layout选择具体搬运实现。搬运块数、搬运长度、源/目的侧步长以及NZ转换相关信息由Tensor Layout推导，用户不需要在`Copy`调用中额外传入搬运参数。

该通路用于矩阵计算输入搬入，支持以下场景：
- 连续数据搬入：源数据从Global Memory搬入L1 Buffer后数据格式不变。
- 高维切分数据搬入：源/目的侧存在步长时，根据Layout推导多段搬运。
- 随路格式转换搬入：从Global Memory搬入L1 Buffer的同时完成ND/DN到NZ/ZN的矩阵排布转换。
- Scale数据搬入：用于MX矩阵计算中ScaleA/ScaleB数据从Global Memory搬入L1 Buffer。
特性细节可参考关键特性说明。

## 函数原型

- 执行Global Memory到L1 Buffer搬运。

    ```cpp
    template <typename AtomType, typename DstTensor, typename SrcTensor>
    __aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy,
        const DstTensor& dst, const SrcTensor& src)
    ```

- 使用默认trait构造搬运原子对象。

    ```cpp
    template <typename CopyOperationType>
    __aicore__ inline constexpr auto MakeCopy()
    ```

- 使用指定trait构造搬运原子对象。

    ```cpp
    template <typename CopyOperationType, typename CopyTraitType>
    __aicore__ inline constexpr auto MakeCopy()
    ```

Global Memory到L1 Buffer通路使用`CopyGM2L1`作为`CopyOperationType`，使用`CopyGM2L1TraitDefault`作为默认trait类型。

## 参数说明

**模板参数说明**

| 参数名 | 描述 |
| :--- | :--- |
| `AtomType` | `CopyAtom`的模板参数，由`MakeCopy<CopyGM2L1>()`或`MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>()`推导得到。 |
| `DstTensor` | 目的张量类型。Global Memory到L1 Buffer搬运时需为Tensor API Tensor类型，存储位置为`Location::L1`。 |
| `SrcTensor` | 源张量类型。Global Memory到L1 Buffer搬运时需为Tensor API Tensor类型，存储位置为`Location::GM`。 |
| `CopyOperationType` | 搬运操作类型。Global Memory到L1 Buffer搬运取`CopyGM2L1`。 |
| `CopyTraitType` | 搬运trait类型。Global Memory到L1 Buffer默认取`CopyGM2L1TraitDefault`。 |

**函数参数说明**

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomCopy` | 输入 | 搬运原子对象。Global Memory到L1 Buffer搬运可通过`MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>()`构造。 |
| `dst` | 输出 | 目的张量，存储位置为`Location::L1`。 |
| `src` | 输入 | 源张量，存储位置为`Location::GM`。 |

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 数据类型

源张量和目的张量的数据类型需要保持一致。

- 连续数据搬入和高维切分数据搬入支持`half`、`bfloat16_t`、`float`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`int64_t`、`uint64_t`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`。
- ND2NZ/DN2NZ随路格式转换路径支持`half`、`bfloat16_t`、`float`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`。
- Scale数据搬入支持`fp8_e8m0_t`。

## 约束说明

**Tensor与Layout约束**

- `dst`和`src`必须为Tensor API Tensor类型。
- 目的张量必须位于L1 Buffer，源张量必须位于Global Memory。
- 支持的Layout组合需要满足Layout支持情况中的范围，不支持的组合会在编译期进入不支持路径。
- 源张量和目的张量的数据类型需要保持一致。
- Global Memory到L1 Buffer `Copy`不直接暴露搬运参数，相关参数由`src/dst` Tensor的Layout自动推导。Layout推导出的参数仍需满足搬运指令约束。

**地址与对齐约束**

- 位于Global Memory的源地址必须1B对齐，位于L1 Buffer的目的地址必须32B对齐。
- 格式保持的连续搬运场景中，有效搬运字节数需要32B对齐。若未对齐，搬运量会按硬件语义向下取整到32B对齐。
- 格式保持的高维切分搬运场景中，搬运长度、源侧步长和目的侧步长均按32B DataBlock推导。使用时应保证Layout描述的行长和步长能推导出满足搬运约束的参数。
- ND2NZ/DN2NZ随路转换场景中，目的矩阵尾部按转换搬运语义补0，对齐约束由Layout自动推导。
- b4/fp4数据随路转换时按b8搬运粒度处理，Layout推导参数需要满足对应粒度约束；DN2NZ类转换中N方向有效元素数需要满足偶数约束。

**连续与非连续搬运约束**

连续与非连续搬运搬入映射到`asc_copy_gm2l1_align`。Tensor API不暴露以下参数，但Layout推导出的内部参数需满足对应范围。

| 内部参数 | 含义 | 取值范围 |
| :--- | :--- | :--- |
| `blockCount` | 搬运数据块个数 | `[0, 4095]` |
| `blockLen` | 单个数据块长度，单位为32B DataBlock | `[0, 65535]` |
| `srcGap`/`srcStride` | 源侧相邻数据块间隔，单位为32B DataBlock | `[0, 65535]` |
| `dstGap`/`dstStride` | 目的侧相邻数据块间隔，单位为32B DataBlock | `[0, 65535]` |

当`blockCount`或`blockLen`任一值为0时，该次搬运视为空操作。

**ND2NZ随路转换参数约束**

ND2NZ类随路转换映射到`asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_nd2nz`。Tensor API不暴露以下参数，但Layout推导出的内部参数需满足对应范围。

| 内部参数 | 含义 | 取值范围 |
| :--- | :--- | :--- |
| `ndNum` | 搬运的ND矩阵个数 | `[0, 4095]` |
| `nValue` | 单个ND矩阵的行数 | `[0, 16384]` |
| `dValue` | 单个ND矩阵的列数 | `[0, 65535]` |
| `srcNdMatrixStride` | 相邻ND矩阵起始地址偏移 | `[0, 65535]` |
| `srcDValue` | 源矩阵一行包含的元素个数 | `[1, 65535]` |
| `dstNzC0Stride` | NZ矩阵中相邻Z矩阵起始地址偏移 | `[1, 16384]` |
| `dstNzNStride` | NZ矩阵中相邻行起始地址偏移 | `[1, 16384]` |
| `dstNzMatrixStride` | 相邻NZ矩阵起始地址偏移 | `[0, 65535]` |

当`ndNum`、`nValue`或`dValue`任一值为0时，该次搬运视为空操作。

**DN2NZ随路转换参数约束**

DN2NZ类随路转换映射到`asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_dn2nz`。Tensor API不暴露以下参数，但Layout推导出的内部参数需满足对应范围。

| 内部参数 | 含义 | 取值范围 |
| :--- | :--- | :--- |
| `dnNum` | 搬运的DN矩阵个数 | `[0, 4095]` |
| `nValue` | 单个DN矩阵的N方向元素个数 | `[0, 16384]` |
| `dValue` | 单个DN矩阵的D方向元素个数 | `[0, 2^21 - 1]` |
| `srcDnMatrixStride` | 相邻DN矩阵起始地址偏移 | `[0, 2^40 - 1]` |
| `srcDValue` | 源矩阵一行包含的元素个数 | `[1, 2^40 - 1]` |
| `dstNzC0Stride` | NZ矩阵中相邻Z矩阵起始地址偏移 | `[1, 16384]` |
| `dstNzNStride` | NZ矩阵中相邻行起始地址偏移 | `[1, 16384]` |
| `dstNzMatrixStride` | 相邻NZ矩阵起始地址偏移 | `[0, 65535]` |

当`dnNum`、`nValue`或`dValue`任一值为0时，该次搬运视为空操作。

**目的地址重叠约束**

- ND2NZ/DN2NZ随路转换搬入到L1 Buffer的目的数据区域不能重叠。如果存在重叠写入，不保证重叠数据的写入顺序。
- 连续数据搬入未额外声明支持目的地址重叠，使用时应避免由Layout推导出的目的数据区域发生非预期重叠。

**Cache控制约束**

- 源Global Memory张量可以通过`SetL2CacheHint`设置L2 Cache控制模式，Global Memory到L1 Buffer路径会读取该模式作为搬运的cache控制参数。
- L2 Cache控制模式为`CacheMode::CACHE_MODE_DISABLE`和`CacheMode::CACHE_MODE_NORMAL`。

## 关键特性说明

### 连续数据搬入

连续数据搬入表示源张量和目的张量的数据格式保持一致，仅从Global Memory搬入L1 Buffer。支持`NDExtLayoutPtn/NDLayoutPtn`到`NDExtLayoutPtn/NDLayoutPtn`、`NZLayoutPtn`到`NZLayoutPtn`、`ZNLayoutPtn`到`ZNLayoutPtn`。

当Layout描述的源矩阵和目的矩阵均为连续空间时，`Copy`会推导为单段连续搬运。

**图1** 连续数据搬入示意图

![](../../figures/连续搬运示意图-53.png "连续数据搬入示意图")

### 高维切分数据搬入

高维切分数据搬入表示源/目的侧Layout中存在步长或切分维度，`Copy`会根据Layout推导为多段搬运。该场景关注的是高维结构被切分后的连续片段搬运，而不是单段连续搬入。

当Layout描述的数据块之间存在步长时，`Copy`会根据Layout推导为多段搬运。

**图2** 高维切分数据搬入示意图

![](../../figures/非连续搬运示意图-55.png "高维切分数据搬入示意图")

### 随路格式转换搬入

随路格式转换搬入表示从Global Memory搬入L1 Buffer时同步完成矩阵排布转换。接口根据源Layout和目的Layout自动选择转换方向，支持以下组合：

| 源Layout | 目的Layout | 说明 |
| :--- | :--- | :--- |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `NZLayoutPtn` | ND格式搬入并转换为NZ格式。 |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `ZNLayoutPtn` | ND格式搬入并转换为ZN格式。 |
| `DNExtLayoutPtn`、`DNLayoutPtn` | `NZLayoutPtn` | DN格式搬入并转换为NZ格式。 |
| `DNExtLayoutPtn`、`DNLayoutPtn` | `ZNLayoutPtn` | DN格式搬入并转换为ZN格式。 |

ND到NZ的格式转换等价于DN到ZN的格式转换，DN到NZ的格式转换等价于ND到ZN的格式转换。

**图3** ND2NZ与DN2ZN示意图

![](../../figures/ND2NZ与DN2ZN示意图.png "ND2NZ与DN2ZN示意图")

#### ND2NZ连续搬运

以half数据类型为例，图4为ND2NZ连续搬运示意图。该场景由Layout自动推导单段搬运参数，源矩阵和目的矩阵保持连续排布，目的端按32B对齐语义补0。

**图4** ND2NZ转换示意图（连续搬运）

![](../../figures/ND2NZ转换示意图（连续搬运）.png "ND2NZ转换示意图（连续搬运）")

#### ND2NZ非连续搬运

以half数据类型为例，图5为ND2NZ非连续搬运示意图。该场景由Layout自动推导多段搬运参数，适用于源侧或目的侧存在步长的高维切分场景，目的端按转换语义补0。

**图5** ND2NZ转换示意图（非连续搬运）

![](../../figures/ND2NZ转换示意图（非连续搬运）.png "ND2NZ转换示意图（非连续搬运）")

#### DN2NZ连续搬运

以half数据类型为例，图6为DN2NZ连续搬运示意图。该场景由Layout自动推导单段搬运参数，源矩阵和目的矩阵保持连续排布，目的端按32B对齐语义补0。

**图6** DN2NZ转换示意图（连续搬运）

![](../../figures/DN2NZ转换示意图（连续搬运）.png "DN2NZ转换示意图（连续搬运）")

#### DN2NZ非连续搬运

以half数据类型为例，图7为DN2NZ非连续搬运示意图。该场景由Layout自动推导多段搬运参数，适用于源侧或目的侧存在步长的高维切分场景，目的端按转换语义补0。

**图7** DN2NZ转换示意图（非连续搬运）

![](../../figures/DN2NZ转换示意图（非连续搬运）.png "DN2NZ转换示意图（非连续搬运）")

### Scale数据搬入

Scale数据搬入用于矩阵计算中scale相关数据从Global Memory搬入L1 Buffer。A矩阵相关scale支持`ScaleANDLayoutPtn`到`ZZLayoutPtn`、`ScaleADNLayoutPtn`到`ZZLayoutPtn`、`ZZLayoutPtn`到`ZZLayoutPtn`；B矩阵相关scale支持`ScaleBNDLayoutPtn`到`NNLayoutPtn`、`ScaleBDNLayoutPtn`到`NNLayoutPtn`、`NNLayoutPtn`到`NNLayoutPtn`。

## Layout支持情况

| 源Layout | 目的Layout | 搬运场景 |
| :--- | :--- | :--- |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `NDExtLayoutPtn`、`NDLayoutPtn` | 连续数据搬入。 |
| `NZLayoutPtn` | `NZLayoutPtn` | NZ格式保持搬入。 |
| `ZNLayoutPtn` | `ZNLayoutPtn` | ZN格式保持搬入。 |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `NZLayoutPtn` | ND到NZ随路格式转换。 |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `ZNLayoutPtn` | ND到ZN随路格式转换。 |
| `DNExtLayoutPtn`、`DNLayoutPtn` | `NZLayoutPtn` | DN到NZ随路格式转换。 |
| `DNExtLayoutPtn`、`DNLayoutPtn` | `ZNLayoutPtn` | DN到ZN随路格式转换。 |
| `ScaleANDLayoutPtn` | `ZZLayoutPtn` | ScaleA数据随路转换搬入。 |
| `ScaleADNLayoutPtn` | `ZZLayoutPtn` | ScaleA数据随路转换搬入。 |
| `ZZLayoutPtn` | `ZZLayoutPtn` | ScaleA数据格式保持搬入。 |
| `ScaleBNDLayoutPtn` | `NNLayoutPtn` | ScaleB数据随路转换搬入。 |
| `ScaleBDNLayoutPtn` | `NNLayoutPtn` | ScaleB数据随路转换搬入。 |
| `NNLayoutPtn` | `NNLayoutPtn` | ScaleB数据格式保持搬入。 |

## 调用示例

以下示例将Global Memory中的ND矩阵搬入L1 Buffer，并转换为NZ布局。

```cpp
#include "tensor_api/tensor.h"

using namespace AscendC::Te;

constexpr uint32_t M = 32;
constexpr uint32_t K = 64;

__aicore__ inline void CopyGmToL1(__gm__ half* gmAddr)
{
    __cbuf__ half l1Buf[M * K];

    auto gmA = MakeTensor(MakeMemPtr(gmAddr),
        MakeFrameLayout<NDExtLayoutPtn>(M, K));
    auto l1A = MakeTensor(MakeMemPtr(l1Buf),
        MakeFrameLayout<NZLayoutPtn>(M, K));

    gmA.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);

    auto copyGm2L1 = MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>();
    Copy(copyGm2L1, l1A, gmA);
}
```
