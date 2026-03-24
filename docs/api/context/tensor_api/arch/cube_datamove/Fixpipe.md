# Fixpipe

## 产品支持情况<a name="section73648168211"></a>

| 产品                                        | 是否支持 |
| ------------------------------------------- | :------: |
| Ascend 950PR/Ascend 950DT                   |    √     |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 |    x     |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 |    x     |
| Atlas 200I/500 A2 推理产品                  |    x     |
| Atlas 推理系列产品AI Core                   |    x     |
| Atlas 推理系列产品Vector Core               |    x     |
| Atlas 训练系列产品                          |    x     |
| Atlas 200/300/500 推理产品                  |    x     |


## 功能说明<a name="section967653716714"></a>

#include "tensor_api.h"

Fixpipe能够在矩阵计算完成后对计算结果进行处理，并通过PIPE_FIX流水完成L0C到GM/UB的数据搬运，支持以下功能：
1. 数据搬运：将矩阵计算结果从CO1搬运到Global Memory/UB
2. 量化操作：支持float/int32_t到int8_t/uint8_t/hifloat8_t/fp8_e4m3fn_t等类型的量化
3. ReLU激活：可在搬运过程中执行ReLU操作
4. 通道拆分：支持float到float的通道拆分
5. 双目标模式：支持启用双目标模式控制

## 函数原型<a name="section1790918208125"></a>

```cpp
// 非量化接口
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeTemplate<T, U>, void>::type Fixpipe(const T& dst, const U& src, const FixpipeParams& params = FixpipeParams{})

// 量化接口
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename S>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeQuantTemplate<T, U, S>, void>::type 
Fixpipe(const T& dst, const U& src, const S& quant, const FixpipeParams& params = FixpipeParams{});

// 带坐标偏移的非量化接口
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename Coord>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeTemplateWithCoord<T, U, Coord>, void>::type
Fixpipe(const T& dst, const U& src, const Coord& coord, const FixpipeParams& params = FixpipeParams{});

// 带坐标偏移的量化接口
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename S, typename Coord>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeQuantTemplateWithCoord<T, U, S, Coord>, void>::type
Fixpipe(const T& dst, const U& src, const S& quant, const Coord& coord, const FixpipeParams& params = FixpipeParams{});
```

## 参数说明<a name="section966471201217"></a>

**表 1**  模板参数说明

| 参数名            | 描述                                                         |
| ----------------- | ------------------------------------------------------------ |
| T                 | 目的操作数的数据类型，通过TensorTrait构造的GlobalTensor类型/LocalTensor类型。 |
| U                 | 源操作数的数据类型，通过TensorTrait构造的LocalTensor类型。  |
| S                 | 量化参数的数据类型，可以是Scalar或者是通过TensorTrait构造的LocalTensor类型。支持的数据类型为uint64_t。 |
| Coord             | 坐标偏移的数据类型，通过MakeCoord构造的Std::tuple类型。 |
| FixpipeTrait | Fixpipe相关配置参数，类型为FixpipeTrait：<br/>struct FixpipeTrait {<br/>    RoundMode roundMode = RoundMode::DEFAULT;<br/>    bool enableRelu = false;<br/>    bool enableChannelSplit = false;<br/>    DualDstMode dualDstCtl = DualDstMode::DUAL_DST_DISABLE;<br/>};<br/>constexpr FixpipeTrait DEFAULT_FIXPIPE_TRAIT; |


**表 2**  FixpipeTrait参数说明

| 参数名   | 描述                                                         |
| -------- | :----------------------------------------------------------- |
| roundMode | RoundMode是一个枚举类型，用于控制量化场景下的舍入模式，仅针对float量化为hifloat8_t场景生效，默认值为RoundMode::DEFAULT。若需要设置为混合舍入模式（对应的量化模式为QuantMode_t::VQF322HIF8_PRE_HYBRID/QuantMode_t::QF322HIF8_PRE_HYBRID），设置为RoundMode::HYBRID。|
| enableRelu | 是否使能relu的开关， false:不使能relu功能，true:使能relu功能。|
| enableChannelSplit | 是否使能通道拆分的功能。仅在dst和src都为float时才能使能通道拆分，且不能同时使能ChannelSplit和NZ2ND功能。 |
| dualDstCtl | DualDstMode是一个枚举类型，用于双目标模式控制，默认值为DualDstMode::DUAL_DST_DISABLE（即不启用双目标模式控制）。当启用双目标模式控制时，L0C中的M×N矩阵将被分成两半，并同时写入两个子块（SUB BLOCK）的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。支持设置DualDstMode::DUAL_DST_SPLIT_M从M维度分成两半，设置DualDstMode::DUAL_DST_SPLIT_N从N维度分成两半。仅支持在普通搬运模式或者NZ2ND搬运场景下使用，不支持随路功能场景。 |


**表 3**  参数说明

| 参数名 | 输入/输出 | 含义         |
| ------ | --------- | ------------ |
| dst    | 输出      | 目的操作数。 |
| src    | 输入      | 源操作数。   |
| quant  | 输入      | 量化参数。     |
| coord  | 输入      | 目的操作数上的偏移坐标。 |
| params | 输入      | 运行时参数结构体，类型为FixpipeParams:<br/>struct FixpipeParams {<br/> uint8_t unitFlag = 0;<br/>} |


**表 4**  FixpipeParams参数说明

| 参数名 | 描述         |
| ------ |------------ |
| unitFlag  | unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，使能该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适合用于在L0C Buffer累加的场景。默认值为0，使能该功能时，unitFlag设置为3即可。 |


## 数据类型支持

**表 5**  L0C->GM数据类型支持

| 源操作数类型 | 目的操作数类型 | 支持情况 |
|------------|--------------|---------|
| float | int8_t | √ |
| float | uint8_t | √ |
| float | hifloat8_t | √ |
| float | fp8_e4m3fn_t | √ |
| float | half | √ |
| float | bfloat16_t | √ |
| float | float | √ |
| int32_t | int8_t | √ |
| int32_t | uint8_t | √ |
| int32_t | half | √ |
| int32_t | bfloat16_t | √ |
| int32_t | int32_t | √ |

**表 6**  L0C->UB数据类型支持

| 源操作数类型 | 目的操作数类型 | 支持情况 |
|------------|--------------|---------|
| float | int8_t | √ |
| float | uint8_t | √ |
| float | hifloat8_t | x |
| float | fp8_e4m3fn_t | x |
| float | half | √ |
| float | bfloat16_t | √ |
| float | float | √ |
| int32_t | int8_t | √ |
| int32_t | uint8_t | √ |
| int32_t | half | √ |
| int32_t | bfloat16_t | √ |
| int32_t | int32_t | √ |

## 返回值说明<a name="section44801012174220"></a>

VerifyingFixpipeTemplate/VerifyingFixpipeQuantTemplateWithCoord表达式为模板参数校验表达式，执行效果如下：

-   校验表达式成立，表明输入数据类型符合使用约束，Fixpipe函数表达式返回值为void。
-   校验表达式不成立，表明输入数据类型不符合使用约束时，Fixpipe函数声明失效，继而会导致编译错误。

## 理论性能

当前版本暂不支持理论性能计算。

## 约束说明<a name="section1333717617510"></a>

-   地址重叠约束：源操作数与目的操作数不允许地址重叠。
-   地址对齐约束：源操作数起始地址和数据长度要求64字节对齐。
-   参数组合约束：dst和src都为float时才能使能通道拆分，且不能同时使能ChannelSplit和NZ2ND功能。
-   环境影响约束：接口调用不影响全局状态，无需复原。
-   特殊数据类型约束：使用float->hifloat8_t特殊类型时，需注意FixpipeTrait量化舍入参数配置。
-   性能约束：quant参数的Layout中每一行的长度必须能够被主块（512）和尾块（n % 512）整除。
-   异常和边界值处理：输入为inf或nan时，输出量化结果为inf或nan；输入为0时，输出为0；输入超过MAX/MIN范围时，输出为MAX/MIN。
-   Tensor Layout相关约束：
    -   TensorTrait中的Layout表述，统一采用[层次化表述法](../struct/defenition/Layout和层次化表述法.md)。
    -   Shape、Stride只支持四个维度的数据配置，针对不同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[数据通路说明]。
    -   Shape、Stride等数值数据，仅支持size_t类型和Std::Int类型。
    -   Fixpipe量化接口中，quant参数的Layout中每一行的长度必须能够被主块（512）和尾块（n % 512）整除。

## 流水类型

PIPE_FIX

## 数据通路说明<a name="section840010692511"></a>

**表 7**  L0C Buffer -> GM通路使用说明

| 项目                                         | 内容                                                         |
| -------------------------------------------- | ------------------------------------------------------------ |
| 模板参数T                                    | 数据类型为GlobalTensor<TensorTrait>/LocalTensor<TensorTrait>。                        |
| 模板参数U                                    | 数据类型为LocalTensor<TensorTrait>。                         |
| 目的操作数TPosition取值范围                  | TPosition::GM / TPosition::UB                                                 |
| 源操作数TPosition取值范围                    | TPosition::CO1                                               |
| 源操作数数据对齐要求                         | 起始地址和数据长度要求64字节对齐。                           |
| 目的操作数数据对齐要求                       | 无对齐要求。                                                 |
| 数据排布要求                                 | NZ Layout -> NZ Layout : 源操作数和目的操作数仅支持连续排布和非连续排布。<br/>NZ Layout -> ND Layout ：源操作数和目的操作数仅支持连续排布和非连续排布。<br/>NZ Layout -> DN Layout：源操作数和目的操作数仅支持连续排布和非连续排布。<br/>NZ Layout内层矩阵的Shape大小为16 * 16。 |

## 特性说明

### 量化特性
Fixpipe支持对矩阵计算结果进行量化操作，可将float或int32_t类型量化为int8_t、uint8_t、hifloat8_t、fp8_e4m3fn_t等低精度类型。

### ReLU特性
通过FixpipeTrait中的enableRelu参数，可以在数据搬运的同时执行ReLU操作。

### 通道拆分特性
当dst和src都为float时，可以使能通道拆分功能，提高数据搬运效率。

## 调用示例<a name="section163951002119"></a>

```cpp
...
// CO1 -> GM

size_t mLength = 128;
size_t kLength = 64;

// 其中DstT为输出的数据类型，支持int8_t、uint8_t、hifloat8_t、fp8_e4m3fn_t、half、bfloat16_t、int32_t、float
auto dstLayout = AscendC::MakeNDLayout<DstT>(mLength, kLength);
auto dstPtr = MakeGMmemPtr(dst);  // dst为给定的gm地址
auto globalC = MakeTensor(dstPtr, dstLayout);

// 其中SrcT为L0C上的数据类型，支持int32_t/float
auto l0CLayout = AscendC::MakeL0CLayout<SrcT>(mLength, kLength);
constexpr int32_t  L0C_MAX = 128*128;
__cc__ float l0CAddr[L0C_MAX];
auto l0CPtr = MakeL0CmemPtr(l0CAddr);
auto l0CBuff = l0CPtr + 0;
auto l0CTensor = MakeTensor(l0CBuff, l0CLayout);
// 下面接口为3种调用方式的示例
// 调用方式1
Copy(CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}, globalC, l0CTensor);
// 调用方式2
CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}.Call(globalC, l0CTensor);
// 调用方式3
auto atomCopyL0C2GM = MakeCopy(CopyL0C2GM{}, FixpipeTraitDefault{});
atomCopyL0C2GM.Call(globalC, l0CTensor);

// 带坐标场景传参调用示例, 其中row和col为期望搬运到目标输出GM上的偏移坐标
Copy(CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}, globalC, l0CTensor, MakeCoord(row, col));

// 运行时参数调用示例，其中FixpipeParams中传参3为unitFlag值
Copy(CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}, globalC, l0CTensor, MakeCoord(row, col), AscendC::Te::FixpipeParams{3});

// Scalar量化场景调用示例
uint64_t quantScalar = 3;
Copy(CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}, globalC, l0CTensor, quantScalar, MakeCoord(row, col), AscendC::Te::FixpipeParams{3});

// Vector量化场景调用示例
// 构造L1上量化Tensor对象
__cbuf__ uint64_t l1QuantAddr[128];
auto l1QuantBuff = l1QuantPtr + 0;
auto l1QuantLayout = MakeNDLayout<uint64_t>(AscendC::Std::Int<1>{}, n);
auto quantTensor = MakeTensor(l1QuantBuff, l1QuantLayout);
// 使用CopyGM2L1方法将GM上的量化Tensor搬运到L1上，其中globalQuant为GM上的量化Tensor
Copy(CopyAtom<CopyTraits<CopyGM2L1, DataCopyTraitDefault>>{}, quantTensor, globalQuant);
// 调用CopyL0C2GM时将L1上的量化Tensor作为参数传递
Copy(CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}, globalC, l0CTensor, quantTensor, MakeCoord(row, col), AscendC::Te::FixpipeParams{3});
...
```
完整样例请参考[TensorAPI样例代码](https://gitcode.com/cann/asc-devkit/tree/tensor_api_experimental/examples/02_features/05_tensor_api)
