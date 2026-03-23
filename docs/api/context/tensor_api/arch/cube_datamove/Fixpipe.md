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

Fixpipe能够在矩阵计算完成后对计算结果进行处理，例如量化操作，并把数据从Local Memory搬运到Global Memory中。

## 函数原型<a name="section1790918208125"></a>

```cpp
// 无量化版本
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeTemplate<T, U>, void>::type 
Fixpipe(const T& dst, const U& src, const FixpipeParams& params = FixpipeParams{})

// 标量量化版本
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename S>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeQuantTemplate<T, U, S>, void>::type 
Fixpipe(const T& dst, const U& src, const S& quant, const FixpipeParams& params = FixpipeParams{})

// 带坐标切片的无量化版本
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename Coord>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeTemplateWithCoord<T, U, Coord>, void>::type 
Fixpipe(const T& dst, const U& src, const Coord& coord, const FixpipeParams& params = FixpipeParams{})

// 带坐标切片的标量量化版本
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename S, typename Coord>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeQuantTemplateWithCoord<T, U, S, Coord>, void>::type 
Fixpipe(const T& dst, const U& src, const S& quant, const Coord& coord, const FixpipeParams& params = FixpipeParams{})
```

## 参数说明<a name="section966471201217"></a>

**表 1**  模板参数说明

| 参数名            | 描述                                                         |
| ----------------- | ------------------------------------------------------------ |
| T                 | 目的操作数的数据类型，通过TensorTrait构造的LocalTensor或GlobalTensor类型。支持Hardware::GM或Hardware::UB。 |
| U                 | 源操作数的数据类型，通过TensorTrait构造的LocalTensor类型，必须是L0C（Hardware::L0C）存储位置。  |
| S                 | 量化参数的数据类型，可以是scalar（uint64_t）或者是通过TensorTrait构造的GlobalTensor类型（存储在L1）。 |
| Coord             | 坐标切片参数，使用std::tuple类型，用于对dst和src进行切片操作。 |
| FixpipeTrait       | Fixpipe相关配置参数，类型为FixpipeTrait，包含以下成员：<br/>- RoundMode roundMode: 舍入模式，默认为RoundMode::DEFAULT<br/>- bool enableRelu: 是否使能ReLU激活，默认为false<br/>- bool enableChannelSplit: 是否使能通道分割，默认为false<br/>- DualDstMode dualDstCtl: 双目标控制模式，默认为DUAL_DST_DISABLE<br/>constexpr FixpipeTrait DEFAULT_FIXPIPE_TRAIT; |
| FixpipeParams     | Fixpipe运行参数，类型为FixpipeParams，包含以下成员：<br/>- uint8_t unitFlag: 单元标志，默认为0<br/>constexpr FixpipeParams DEFAULT_FIXPIPE_PARAMS; |


**表 2**  量化参数说明

| 参数名   | 描述                                                         |
| -------- | :----------------------------------------------------------- |
| quantPre | QuantMode_t是一个枚举类型，用于控制量化模式，默认值为QuantMode_t::NoQuant，即不使能量化功能。QuantMode_t取值如下：<br>NoQuant，不使能量化功能F322F16，float量化成half，量化结果支持INF_NAN模式<br/>F322BF16，float量化成bfloat16_t，量化结果支持INF_NAN模式<br/>DEQF16，int32_t量化成half, scalar量化，量化结果不支持INF_NAN模式<br/>VDEQF16，int32_t量化成half，tensor量化，量化结果不支持INF_NAN模式QF322B8_PRE，float量化成uint8_t/int8_t，scalar量化VQ<br/>F322B8_PRE，float量化成uint8_t/int8_t，tensor量化REQ8，int32_t量化成uint8_t/int8_t，scalar量化<br/>VREQ8，int32_t量化成uint8_t/int8_t，tensor量化<br/>QF322FP8_PRE，float量化成fp8_e4m3fn_t，scalar量化<br/>VQF322FP8_PRE，float量化成fp8_e4m3fn_t，tensor量化<br/>QF322HIF8_PRE，float量化成hifloat8_t(Half to Away Round)，scalar量化<br/>VQF322HIF8_PRE，float量化成hifloat8_t(Half to Away Round)，tensor量化<br/>QF322HIF8_PRE_HYBRID，float量化成hifloat8_t(Hybrid Round)，scalar量化<br/>VQF322HIF8_PRE_HYBRID，float量化成hifloat8_t(Hybrid Round)，tensor量化<br/>QS322BF16_PRE，int32_t量化成bfloat16_t，scalar量化<br/>VQS322BF16_PRE，int32_t量化成bfloat16_t，tensor量化<br/>QF322F16_PRE，float量化成half，scalar量化<br/>VQF322F16_PRE，float量化成half，tensor量化<br/>QF322BF16_PRE，float量化成bfloat16_t，scalar量化<br/>VQF322BF16_PRE，float量化成bfloat16_t，tensor量化<br/>QF322F32_PRE，float量化成float，scalar量化，该量化模式精度无法达到双万分之一，可以达到双千分之一。如果有双万分之一的精度要求，建议使用AscendDeQuant高阶APIVQF322F32_PRE，float量化成float，tensor量化，该量化模式精度无法达到双万分之一，可以达到双千分之一。如果有双万分之一的精度要求，建议使用AscendDeQuant高阶API |


**表 3**  参数说明

| 参数名 | 输入/输出 | 含义         |
| ------ | --------- | ------------ |
| dst    | 输出      | 目的操作数，支持GM或UB存储位置。 |
| src    | 输入      | 源操作数，必须是L0C存储位置。   |
| quant  | 输入      | 量化参数，可以是scalar（uint64_t）或tensor（L1存储）。     |
| coord  | 输入      | 坐标切片参数，用于对dst和src进行切片操作。     |
| params | 输入      | Fixpipe运行参数，包含unitFlag等配置。     |

## API映射关系
与built-in接口映射关系：
XXX接口是在built-in接口(更新为编译器提供的接口)的基础上进行抽象封装实现的，其对应的底层built-in接口为：
```cpp
// 按照数据类型顺序列出所映射的接口
```

## 返回值说明<a name="section44801012174220"></a>

VerifyingFixpipeTemplate表达式为模板参数校验表达式，执行效果如下：

-   校验表达式成立，表明输入数据类型符合使用约束，Fixpipe函数表达式返回值为void。
-   校验表达式不成立，表明输入数据类型不符合使用约束时，Fixpipe函数声明失效，继而会导致编译错误。

## 约束说明<a name="section1333717617510"></a>

-   Tensor Layout相关约束：
    -   TensorTrait中的Layout表述，统一采用[层次化表述法](../struct/defenition/Layout和层次化表述法.md)。
    -   Shape、Stride只支持四个维度的数据配置，针对不同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[数据通路说明]。
    -   Shape、Stride等数值数据，仅支持size\_t类型和Std::Int类型。
    -   Fixpipe量化接口中，quant参数的Layout中每一行的长度必须能够被主块（512）和尾块（n % 512）整除

-   quant参数数据通路和dataCopy L1 -\> FB保持一致。

## 数据通路说明<a name="section840010692511"></a>

Fixpipe支持以下数据通路：

**表 4**  L0C Buffer -> GM通路使用说明

| 项目                                         | 内容                                                         |
| -------------------------------------------- | ------------------------------------------------------------ |
| 模板参数T                                    | 数据类型为GlobalTensor<TensorTrait>。                        |
| 模板参数U                                    | 数据类型为LocalTensor<TensorTrait>，必须是L0C存储位置。                         |
| 目的操作数TPosition取值范围                  | TPosition::GM                                                |
| 源操作数TPosition取值范围                    | TPosition::CO1                                               |
| 目的操作数的数据类型                         | float、int32_t、half、bfloat16_t、uint8_t、int8_t、fp8_e4m3fn_t、hifloat8_t等（根据量化模式确定）                                               |
| 源操作数的数据类型                           | float、int32_t                                               |
| 源操作数和目的操作数基础数据类型是否要求一致 | 否（支持量化转换）                                                           |
| 源操作数数据对齐要求                         | 起始地址和数据长度要求64字节对齐。                           |
| 目的操作数数据对齐要求                       | 无对齐要求。                                                 |
| 数据排布要求                                 | 支持多种格式转换：<br/>- NZ Layout -> NZ Layout：源操作数和目的操作数仅支持连续排布和非连续排布<br/>- NZ Layout -> ND Layout：源操作数和目的操作数仅支持连续排布和非连续排布<br/>- NZ Layout -> DN Layout：源操作数和目的操作数仅支持连续排布和非连续排布<br/>NZ Layout内层矩阵的Shape大小为16 * 16。 |

**表 5**  L0C Buffer -> UB通路使用说明（仅Ascend 950PR/Ascend 950DT支持）

| 项目                                         | 内容                                                         |
| -------------------------------------------- | ------------------------------------------------------------ |
| 模板参数T                                    | 数据类型为LocalTensor<TensorTrait>。                        |
| 模板参数U                                    | 数据类型为LocalTensor<TensorTrait>，必须是L0C存储位置。                         |
| 目的操作数TPosition取值范围                  | TPosition::UB                                                |
| 源操作数TPosition取值范围                    | TPosition::CO1                                               |
| 目的操作数的数据类型                         | float、int32_t、half、bfloat16_t、uint8_t、int8_t、fp8_e4m3fn_t、hifloat8_t等（根据量化模式确定）                                               |
| 源操作数的数据类型                           | float、int32_t                                               |
| 源操作数和目的操作数基础数据类型是否要求一致 | 否（支持量化转换）                                                           |
| 源操作数数据对齐要求                         | 起始地址和数据长度要求64字节对齐。                           |
| 目的操作数数据对齐要求                       | 无对齐要求。                                                 |
| 数据排布要求                                 | 支持多种格式转换：<br/>- NZ Layout -> NZ Layout：源操作数和目的操作数仅支持连续排布和非连续排布<br/>- NZ Layout -> ND Layout：源操作数和目的操作数仅支持连续排布和非连续排布<br/>- NZ Layout -> DN Layout：源操作数和目的操作数仅支持连续排布和非连续排布<br/>NZ Layout内层矩阵的Shape大小为16 * 16。 |

## 调用示例<a name="section163951002119"></a>

### 示与其他1：无量化，L0C到GM，NZ到NZ格式

```cpp
// L0C -> GM, NZ Layout -> NZ Layout
using SrcT = float;
using DstT = float;
size_t mLength = 128;
size_t kLength = 64;

auto dstLayout = AscendC::MakeNZLayout<DstT>(mLength, kLength);
auto srcLayout = AscendC::MakeNZLayout<SrcT>(mLength, kLength);

auto srcTrait = AscendC::MakeTensorTrait<SrcT, TPosition::CO1>(srcLayout);
auto dstTrait = AscendC::MakeTensorTrait<DstT, TPosition::GM>(dstLayout);

using srcTraitType = decltype(srcTrait);
using dstTraitType = decltype(dstTrait);

LocalTensor<srcTraitType> L0CTensor;
L0CTensor.SetTensorTrait(srcTrait);

GlobalTensor<dstTraitType> GlobalTensor;
GlobalTensor.SetTensorTrait(dstTrait);

// 不做量化
AscendC::Fixpipe(GlobalTensor, L0CTensor);
```

### 示与其他2：标量量化，L0C到GM，NZ到ND格式

```cpp
// L0C -> GM, NZ Layout -> ND Layout, float量化为half
using SrcT = float;
using DstT = half;
size_t mLength = 128;
size_t kLength = 64;

auto dstLayout = AscendC::MakeRowMajorLayout<DstT>(mLength, kLength);
auto srcLayout = AscendC::MakeNZLayout<SrcT>(mLength, kLength);

auto srcTrait = AscendC::MakeTensorTrait<SrcT, TPosition::CO1>(srcLayout);
auto dstTrait = AscendC::MakeTensorTrait<DstT, TPosition::GM>(dstLayout);

using srcTraitType = decltype(srcTrait);
using dstTraitType = decltype(dstTrait);

LocalTensor<srcTraitType> L0CTensor;
L0CTensor.SetTensorTrait(srcTrait);

GlobalTensor<dstTraitType> GlobalTensor;
GlobalTensor.SetTensorTrait(dstTrait);

// 从float量化为half（标量量化）
constexpr static AscendC::FixpipeTrait trait = {AscendC::QuantMode_t::F322F16};
uint64_t quant = 0;
AscendC::Fixpipe<trait>(GlobalTensor, L0CTensor, quant);
```

### 示与其他3：张量量化，L0C到GM，NZ到ND格式

```cpp
// L0C -> GM, NZ Layout -> ND Layout, int32_t量化为half（张量量化）
using SrcT = int32_t;
using DstT = half;
size_t mLength = 128;
size_t kLength = 64;

auto dstLayout = AscendC::MakeRowMajorLayout<DstT>(mLength, kLength);
auto srcLayout = AscendC::MakeNZLayout<SrcT>(mLength, kLength);

auto srcTrait = AscendC::MakeTensorTrait<SrcT, TPosition::CO1>(srcLayout);
auto dstTrait = AscendC::MakeTensorTrait<DstT, TPosition::GM>(dstLayout);

using srcTraitType = decltype(srcTrait);
using dstTraitType = decltype(dstTrait);

LocalTensor<srcTraitType> L0CTensor;
L0CTensor.SetTensorTrait(srcTrait);

GlobalTensor<dstTraitType> GlobalTensor;
GlobalTensor.SetTensorTrait(dstTrait);

// 创建量化参数张量（存储在L1）
auto quantLayout = AscendC::MakeRowMajorLayout<uint64_t>(mLength, kLength);
auto quantTrait = AscendC::MakeTensorTrait<uint64_t, TPosition::C1>(quantLayout);

GlobalTensor<decltype(quantTrait)> QuantTensor;
QuantTensor.SetTensorTrait(quantTrait);

// 从int32_t量化为half（张量量化）
constexpr static AscendC::FixpipeTrait trait = {AscendC::QuantumMode_t::VDEQF16};
AscendC::Fixpipe<trait>(GlobalTensor, L0CTensor, QuantTensor);
```

### 示与其他4：带坐标切片（仅Ascend 950PR/Ascend 950DT支持）

```cpp
// L0C -> GM, 使用坐标切片
using SrcT = float;
using DstT = float;
size_t mLength = 256;
size_t kLength = 128;

auto dstLayout = AscendC::MakeNZLayout<DstT>(mLength, kLength);
auto srcLayout = AscendC::MakeNZLayout<SrcT>(mLength, kLength);

auto srcTrait = AscendC::MakeTensorTrait<SrcT, TPosition::CO1>(srcLayout);
auto dstTrait = AscendC::MakeTensorTrait<DstT, TPosition::GM>(dstLayout);

using srcTraitType = decltype(srcTrait);
using dstTraitType = decltype(dstTrait);

LocalTensor<srcTraitType> L0CTensor;
L0CTensor.SetTensorTrait(srcTrait);

GlobalTensor<dstTraitType> GlobalTensor;
GlobalTensor.SetTensorTrait(dstTrait);

// 使用坐标切片，只搬运部分数据
auto coord = AscendC::MakeCoord(
    AscendC::MakeCoord(0, 0),  // dst起始坐标
    AscendC::MakeCoord(0, 0)   // src起始坐标
);

AscendC::Fixpipe(GlobalTensor, L0CTensor, coord);
```

## 实现原理<a name="section_implementation"></a>

Fixpipe接口基于编译器提供的built-in接口实现，内部通过模板特化和类型分发机制自动选择最优的数据搬运路径：

1. **架构适配**：通过CURRENT_ARCH_VERSION宏区分不同架构（如2201、3510），每个架构有独立的实现
2. **格式转换路由**：根据源和目的Tensor的Layout（NZ、ND、DN）自动选择对应的实现类
3. **量化模式分发**：根据量化参数类型（标量、张量、无量化）和量化模式自动选择量化实现
4. **坐标切片支持**：支持通过Coord参数对Tensor进行切片，实现部分数据搬运

内部实现类层次结构：
- FixpipeImpl：主入口，提供多个重载版本
- FixpipeRouting：路由分发，根据硬件位置和架构选择具体实现
- FixpipeL0C2GM2201/3510：架构特定实现，处理L0C到GM的搬运
- FixpipeQuantL0C2GM2201/3510：量化版本实现
- 各格式转换类：如FixpipeNz2Nz、FixpipeNz2Nd、FixpipeNz2Dn等

