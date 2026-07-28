# Copy（Unified Buffer到Unified Buffer数据搬运）

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

Tensor API通过`Copy`接口统一执行不同通路数据搬运。该接口用于在Unified Buffer内部搬运数据。`Copy`接口根据源张量和目的张量的存储位置、数据类型和Layout选择具体搬运实现。搬运块数和搬运长度由Tensor Layout推导，用户不需要在`Copy`调用中额外传入搬运参数。

该通路支持以下同格式分形搬运：

- `ND2ND`：源、目的张量均使用`NDExtLayoutPtn`；内层分形固定为`1 * 1`，外层按行优先方式排布。
- `DN2DN`：源、目的张量均使用`DNExtLayoutPtn`；内层分形固定为`1 * 1`，外层按列优先方式排布。
- `NZ2NZ`：源、目的张量均使用`NZLayoutPtn`；内层分形按Z字形排布，外层分形按N字形排布。
- `ZN2ZN`：源、目的张量均使用`ZNLayoutPtn`；内层分形按N字形排布，外层分形按Z字形排布。

受当前实现限制，由Layout推导的搬运块数必须为1，具体原因如下。

> **注意：**
>
> 当前实现将`StrideRow1`或`StrideColumn1`表示的相邻块首地址间隔直接换算为DataBlock数，并传给底层指令的`src_gap`和`dst_gap`字段；但这两个字段表示前一块尾地址到下一块首地址之间的间隙。因此，当搬运块数大于1时，底层指令的实际块首地址递增量为“单块长度 + Layout块首地址间隔”，比Layout描述的递增量多一个单块长度。当前版本仅可在搬运块数为1时使用；不得用于多块搬运。

## 函数原型

- 执行Unified Buffer到Unified Buffer搬运。

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

Unified Buffer到Unified Buffer通路使用`CopyUB2UB{}`作为`copyOperation`，使用`CopyUB2UBTraitDefault{}`作为默认trait对象。

## 参数说明

**模板参数说明**

| 参数名 | 描述 |
| :--- | :--- |
| AtomType | `CopyAtom`的模板参数，由`MakeCopy(CopyUB2UB{})`或`MakeCopy(CopyUB2UB{}, CopyUB2UBTraitDefault{})`推导得到。 |
| DstTensor | 目的张量类型。该通路需为Tensor API Tensor类型，存储位置为`Location::UB`。 |
| SrcTensor | 源张量类型。该通路需为Tensor API Tensor类型，存储位置为`Location::UB`。 |
| CopyOperationType | 搬运操作对象类型。该通路取`CopyUB2UB`。 |
| CopyTraitType | 搬运trait对象类型。该通路默认取`CopyUB2UBTraitDefault`。当前默认trait不包含用户可配置字段。 |

**函数参数说明**

**表1**  `Copy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| atomCopy | 输入 | 搬运原子对象，可通过`MakeCopy(CopyUB2UB{})`或`MakeCopy(CopyUB2UB{}, CopyUB2UBTraitDefault{})`构造。 |
| dst | 输出 | 目的张量，存储位置为`Location::UB`。 |
| src | 输入 | 源张量，存储位置为`Location::UB`。 |

**表2**  `MakeCopy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| copyOperation | 输入 | 搬运操作对象，取`CopyUB2UB{}`。 |
| copyTrait | 输入 | 搬运trait对象，默认取`CopyUB2UBTraitDefault{}`。 |

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

- 该通路在AIV的`PIPE_V`流水上执行。

- 该通路仅在AIV侧执行；在AIC侧调用不会发射搬运指令。
- 源地址和目的地址均必须32B对齐。
- 源张量和目的张量必须使用相同的数据类型、相同的Shape（包括内层和外层各维取值）以及相同的`LayoutPattern`。源、目的Stride可以不同。
- ND、DN格式仅支持内层分形的Shape为`1 * 1`的`NDExtLayoutPtn`、`DNExtLayoutPtn`，不支持只有外层矩阵的`NDLayoutPtn`、`DNLayoutPtn`。
- 使用`MakeFrameLayout`构造NZ或ZN Layout时，需要将张量元素类型作为第二个模板参数，例如`MakeFrameLayout<NZLayoutPtn, half>(M, N)`，以便按实际数据类型推导`C0_ELEMENT`。
- `MakeFrameLayout<LayoutPattern>(M, N)`构造的是由内层矩阵和外层矩阵组成的两层Layout。当前实现只按该两层结构提取Shape和Stride，不支持`MakeFrameLayout<LayoutPattern>(B, M, N)`在矩阵维度前增加的Batch维度。
- `ShapeRow0`、`ShapeRow1`、`ShapeColumn0`和`ShapeColumn1`必须大于0。
- 受当前多块步长实现限制，由Layout推导的搬运块数必须为1。各格式的精确条件为：ND格式`ShapeRow1 = M = 1`；DN格式`ShapeColumn1 = N = 1`；NZ格式`ShapeColumn1 = 1`，即`1 <= N <= C0_ELEMENT`；ZN格式`ShapeRow1 = 1`，即`1 <= M <= C0_ELEMENT`。
- 由Layout推导的单块数据在b4类型按两个逻辑元素占1B打包后，其字节长度必须为32B的整数倍。实现使用右移5位将字节数转换为DataBlock数，因此不足32B的尾部字节不会被搬运。
- 单块搬运长度换算为DataBlock后的取值范围为[1, 65535]。源、目的张量的实际分配空间必须覆盖全部搬运数据。
- 对于`fp4x2_e1m2_t`和`fp4x2_e2m1_t`，两个逻辑元素占1B，一个DataBlock包含64个b4逻辑元素。
- `Copy`只发射异步流水指令，不插入同步。若本次搬运的源区域由前序指令写入，或目的区域会被后续指令读取或改写，调用方必须在对应的数据依赖点插入同步。

## 关键特性说明

### 同格式数据搬运

同格式数据搬运表示在Unified Buffer内部搬运数据，搬运前后数据格式保持不变。接口根据源Layout和目的Layout自动推导搬运参数，支持`ND2ND`、`DN2DN`、`NZ2NZ`和`ZN2ZN`。

受当前实现限制，该通路仅支持单块搬运，不支持由Layout推导出的多块搬运。单块数据的字节长度必须是32B的整数倍，不足32B的尾部数据不会被搬运。

#### ND2ND搬运

ND2ND搬运表示源数据和目的数据均按ND格式排布。`Copy`将一个ND数据块从Unified Buffer中的源区域搬运到目的区域，搬运前后数据格式不变。当前仅支持单行ND数据搬运。

#### DN2DN搬运

DN2DN搬运表示源数据和目的数据均按DN格式排布。`Copy`将一个DN数据块从Unified Buffer中的源区域搬运到目的区域，搬运前后数据格式不变。当前仅支持单列DN数据搬运。

#### NZ2NZ搬运

NZ2NZ搬运表示源数据和目的数据均按NZ格式排布。`Copy`将一个NZ数据块从Unified Buffer中的源区域搬运到目的区域，搬运前后数据格式不变。当前仅支持单块NZ数据搬运。

#### ZN2ZN搬运

ZN2ZN搬运表示源数据和目的数据均按ZN格式排布。`Copy`将一个ZN数据块从Unified Buffer中的源区域搬运到目的区域，搬运前后数据格式不变。当前仅支持单块ZN数据搬运。

## 调用示例

以下示例在Unified Buffer内搬运一个ND行向量。`MakeFrameLayout<NDExtLayoutPtn>(1, 64)`生成的层次化Layout为`Shape = ((1, 1), (1, 64))`、`Stride = ((0, 64), (0, 1))`：内层分形的Shape为`1 * 1`，外层矩阵包含1行、64列。实现根据`ShapeRow1 = 1`推导出一个搬运块；64个`half`元素占128B，满足DataBlock对齐要求。

```cpp
#include "tensor_api/tensor.h"

using namespace AscendC::Te;

constexpr uint32_t M = 1;
constexpr uint32_t N = 64;

__aicore__ inline void CopyUbToUb()
{
    __ubuf__ half srcBuf[M * N];
    __ubuf__ half dstBuf[M * N];

    auto srcTensor = MakeTensor(
        MakeMemPtr(srcBuf), MakeFrameLayout<NDExtLayoutPtn>(M, N));
    auto dstTensor = MakeTensor(
        MakeMemPtr(dstBuf), MakeFrameLayout<NDExtLayoutPtn>(M, N));

    auto copyUb2Ub = MakeCopy(CopyUB2UB{}, CopyUB2UBTraitDefault{});
    Copy(copyUb2Ub, dstTensor, srcTensor);
}
```
