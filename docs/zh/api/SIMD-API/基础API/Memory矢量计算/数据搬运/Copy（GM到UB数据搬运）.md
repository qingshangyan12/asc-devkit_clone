# Copy（Global Memory到Unified Buffer数据搬运）

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`tensor_api/tensor.h`。

Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于将Global Memory中的数据搬运到Unified Buffer。`Copy`接口根据源张量和目的张量的存储位置、数据类型和Layout选择具体搬运实现。搬运块数、搬运长度以及源/目的侧步长由Tensor Layout推导，用户不需要在`Copy`调用中额外传入搬运参数。

该通路支持以下场景：

- 连续数据搬运：源数据从Global Memory搬入Unified Buffer后数据格式不变。
- 高维切分数据搬运：源侧或目的侧存在外层Stride时，根据Layout推导多段搬运参数。

源、目的Layout必须使用相同的`LayoutPattern`。支持以下同格式分形搬运：

- `ND2ND`：源、目的张量均使用`NDExtLayoutPtn`；内层分形固定为`1 * 1`，外层按行优先方式排布。
- `DN2DN`：源、目的张量均使用`DNExtLayoutPtn`；内层分形固定为`1 * 1`，外层按列优先方式排布。
- `NZ2NZ`：源、目的张量均使用`NZLayoutPtn`；内层分形按Z字形排布，外层分形按N字形排布。
- `ZN2ZN`：源、目的张量均使用`ZNLayoutPtn`；内层分形按N字形排布，外层分形按Z字形排布。

## 函数原型

- 执行Global Memory到Unified Buffer搬运。

    ```cpp
    template <typename AtomType, typename DstTensor, typename SrcTensor>
    __aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy,
        const DstTensor& dst, const SrcTensor& src)
    ```

- 使用默认trait构造搬运原子对象。

    ```cpp
    template <typename CopyOperationType>
    __aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation)
    ```

- 使用指定trait构造搬运原子对象。

    ```cpp
    template <typename CopyOperationType, typename CopyTraitType>
    __aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation,
        const CopyTraitType& copyTrait)
    ```

Global Memory到Unified Buffer通路使用`CopyGM2UB{}`作为`copyOperation`，使用`CopyGM2UBTraitDefault{}`作为默认trait对象。

## 参数说明

**模板参数说明**

| 参数名 | 描述 |
| :--- | :--- |
| AtomType | `CopyAtom`的模板参数，由`MakeCopy(CopyGM2UB{})`或`MakeCopy(CopyGM2UB{}, CopyGM2UBTraitDefault{})`推导得到。 |
| DstTensor | 目的张量类型。该通路需为Tensor API Tensor类型，存储位置为`Location::UB`。 |
| SrcTensor | 源张量类型。该通路需为Tensor API Tensor类型，存储位置为`Location::GM`。 |
| CopyOperationType | 搬运操作对象类型。该通路取`CopyGM2UB`。 |
| CopyTraitType | 搬运trait对象类型。该通路默认取`CopyGM2UBTraitDefault`。当前默认trait不包含用户可配置字段。 |

**函数参数说明**

**表1**  `Copy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| atomCopy | 输入 | 搬运原子对象，可通过`MakeCopy(CopyGM2UB{})`或`MakeCopy(CopyGM2UB{}, CopyGM2UBTraitDefault{})`构造。 |
| dst | 输出 | 目的张量，存储位置为`Location::UB`。 |
| src | 输入 | 源张量，存储位置为`Location::GM`。Global Memory侧的Tensor Engine Cache属性会用于本次搬运。 |

**表2**  `MakeCopy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| copyOperation | 输入 | 搬运操作对象，取`CopyGM2UB{}`。 |
| copyTrait | 输入 | 搬运trait对象，默认取`CopyGM2UBTraitDefault{}`。 |

## 数据类型

源张量和目的张量的数据类型需要保持一致。

| 源操作数数据格式 | 目的操作数数据格式 | 源操作数/目的操作数数据类型 |
| :--- | :--- | :--- |
| ND（`NDExtLayoutPtn`） | ND（`NDExtLayoutPtn`） | fp4x2_e2m1_t/fp4x2_e1m2_t/int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float/int64_t/uint64_t |
| DN（`DNExtLayoutPtn`） | DN（`DNExtLayoutPtn`） | fp4x2_e2m1_t/fp4x2_e1m2_t/int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float/int64_t/uint64_t |
| NZ（`NZLayoutPtn`） | NZ（`NZLayoutPtn`） | fp4x2_e2m1_t/fp4x2_e1m2_t/int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float/int64_t/uint64_t |
| ZN（`ZNLayoutPtn`） | ZN（`ZNLayoutPtn`） | fp4x2_e2m1_t/fp4x2_e1m2_t/int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float/int64_t/uint64_t |

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 约束说明

- 该通路在AIV的`PIPE_MTE2`流水上执行。

- 该通路仅在AIV侧执行；在AIC侧调用不会发射搬运指令。
- 位于Global Memory的源地址必须1B对齐，位于Unified Buffer的目的地址必须32B对齐。
- 源张量和目的张量必须使用相同的数据类型、相同的Shape（包括内层和外层各维取值）以及相同的`LayoutPattern`。源、目的Stride可以不同。
- ND、DN格式仅支持内层分形的Shape为`1 * 1`的`NDExtLayoutPtn`、`DNExtLayoutPtn`，不支持只有外层矩阵的`NDLayoutPtn`、`DNLayoutPtn`。
- 使用`MakeFrameLayout`构造NZ或ZN Layout时，需要将张量元素类型作为第二个模板参数，例如`MakeFrameLayout<NZLayoutPtn, half>(M, N)`，以便按实际数据类型推导`C0_ELEMENT`。
- `MakeFrameLayout<LayoutPattern>(M, N)`构造的是由内层矩阵和外层矩阵组成的两层Layout。当前实现只按该两层结构提取Shape和Stride，不支持`MakeFrameLayout<LayoutPattern>(B, M, N)`在矩阵维度前增加的Batch维度；搬运多个矩阵时，需要对每个矩阵分别调用`Copy`。
- `ShapeRow0`、`ShapeRow1`、`ShapeColumn0`和`ShapeColumn1`必须大于0。由Layout推导的搬运块数取值范围为[1, 4095]，单块搬运长度取值范围为[1, 2097151]，单位为字节。
- 源、目的侧Stride均表示相邻搬运块首地址之间的间隔，单位为字节。Unified Buffer侧的块首地址间隔不等于单块搬运长度时，该间隔必须是32B的整数倍。
- 设搬运块数为`blockCount`、单块搬运长度为`blockLen`字节、某一侧相邻块首地址间隔为`blockStride`字节，则该侧的分配空间至少需要覆盖`(blockCount - 1) * blockStride + blockLen`字节。
- 对于`fp4x2_e1m2_t`和`fp4x2_e2m1_t`，两个逻辑元素占1B。由Layout推导的单块逻辑元素数和块首地址间隔对应的逻辑元素数必须为偶数；Unified Buffer侧的32B对齐等价于64个b4逻辑元素对齐。
- `Copy`只发射异步流水指令，不插入同步。若本次搬运的源区域由前序指令写入，或目的区域会被后续指令读取或改写，调用方必须在对应的数据依赖点插入同步。

## 关键特性说明

### 同格式数据搬入

同格式数据搬入表示从Global Memory搬入Unified Buffer后数据格式保持不变。接口根据源Layout和目的Layout自动推导搬运参数，支持`ND2ND`、`DN2DN`、`NZ2NZ`和`ZN2ZN`。

#### ND2ND连续搬运

ND2ND连续搬运表示源矩阵和目的矩阵均按ND格式连续排布。该场景由Layout自动推导搬运参数，将Global Memory中的ND数据连续搬入Unified Buffer，搬运前后数据格式不变。

#### ND2ND非连续搬运

ND2ND非连续搬运表示源侧或目的侧存在外层Stride。该场景归属于高维切分数据搬入，`Copy`会根据Layout自动推导多段搬运参数，将各数据块搬入Unified Buffer中的对应位置。

#### DN2DN连续搬运

DN2DN连续搬运表示源矩阵和目的矩阵均按DN格式连续排布。该场景由Layout自动推导搬运参数，将Global Memory中的DN数据连续搬入Unified Buffer，搬运前后数据格式不变。

#### DN2DN非连续搬运

DN2DN非连续搬运表示源侧或目的侧存在外层Stride。该场景归属于高维切分数据搬入，`Copy`会根据Layout自动推导多段搬运参数，将各数据块搬入Unified Buffer中的对应位置。

#### NZ2NZ连续搬运

NZ2NZ连续搬运表示源矩阵和目的矩阵均按NZ格式连续排布。该场景由Layout自动推导搬运参数，将Global Memory中的NZ数据连续搬入Unified Buffer，搬运前后数据格式不变。

#### NZ2NZ非连续搬运

NZ2NZ非连续搬运表示源侧或目的侧存在外层Stride。该场景归属于高维切分数据搬入，`Copy`会根据Layout自动推导多段搬运参数，将各数据块搬入Unified Buffer中的对应位置，目的端间隔区域不会被写入。

#### ZN2ZN连续搬运

ZN2ZN连续搬运表示源矩阵和目的矩阵均按ZN格式连续排布。该场景由Layout自动推导搬运参数，将Global Memory中的ZN数据连续搬入Unified Buffer，搬运前后数据格式不变。

#### ZN2ZN非连续搬运

ZN2ZN非连续搬运表示源侧或目的侧存在外层Stride。该场景归属于高维切分数据搬入，`Copy`会根据Layout自动推导多段搬运参数，将各数据块搬入Unified Buffer中的对应位置，目的端间隔区域不会被写入。

## 调用示例

以下示例将Global Memory中的ND矩阵连续搬入Unified Buffer。`MakeFrameLayout<NDExtLayoutPtn>(M, N)`生成的层次化Layout为`Shape = ((1, M), (1, N))`、`Stride = ((0, N), (0, 1))`：内层分形的Shape为`1 * 1`，外层矩阵包含`M`行、`N`列。

```cpp
#include "tensor_api/tensor.h"

using namespace AscendC::Te;

constexpr uint32_t M = 32;
constexpr uint32_t N = 64;

__aicore__ inline void CopyGmToUb(__gm__ half* gmAddr)
{
    __ubuf__ half ubBuf[M * N];

    auto gmTensor = MakeTensor(
        MakeMemPtr(gmAddr), MakeFrameLayout<NDExtLayoutPtn>(M, N));
    auto ubTensor = MakeTensor(
        MakeMemPtr(ubBuf), MakeFrameLayout<NDExtLayoutPtn>(M, N));

    auto copyGm2Ub = MakeCopy(CopyGM2UB{}, CopyGM2UBTraitDefault{});
    Copy(copyGm2Ub, ubTensor, gmTensor);
}
```
