# LoadData

## 产品支持情况

|产品|是否支持|
|--|:-:|
|Ascend 950PR/Ascend 950DT|√|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|x|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|x|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Atlas 200/300/500 推理产品|x|


## 功能说明

LoadData接口支持Local Memory内部的数据搬运，能够完成A1到A2或B1到B2的整块矩阵数据搬运。
该接口通过A1/B1与A2/B2数据排布格式，自动完成是否转置的推导。

## 函数原型

```cpp
// 不带坐标的整块搬运
template<const LoadDataTrait& trait = DEFAULT_LOAD_DATA_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingLoadDataTemplate<T, U>, void>::type 
LoadData(const T& dst, const U& src)

// 带坐标偏移的搬运
template<const LoadDataTrait& trait = DEFAULT_LOAD_DATA_TRAIT, typename T, typename U, class Coord>
__aicore__ inline typename Std::enable_if<VerifyingLoadDataTemplateWithCoord<T, U, Coord>, void>::type 
LoadData(const T& dst, const U& src, const Coord& coord)
```

## 参数说明

**表 1**  模板参数说明

|参数名|描述|
|--|--|
|T|目的操作数的数据类型，通过MakeTensor构造的LocalTensor类型。|
|U|源操作数的数据类型，通过MakeTensor构造的LocalTensor类型。|
|Coord|坐标偏移的数据类型，通过MakeCoord构造的Std::tuple类型。|
|LoadDataTrait|LoadData相关配置参数结构体，类型为LoadDataTrait，具体参考[表3](#table3)。DEFAULT_LOAD_DATA_TRAIT为默认取值。|

**表 2**  参数说明

|参数名|输入/输出|含义|
|--|--|--|
|dst|输出|目的操作数。|
|src|输入|源操作数。|
|coord|输入|源操作数上的偏移坐标。|

**表 3<a id="table3"></a>**  LoadDataTrait参数说明

|参数名|含义|
|--|--|
|transposed|是否启用转置功能，对每个分形矩阵进行转置，默认为false，该参数无需设置，接口内部自动推导，具体参考[数据排布支持](#format)。|

## 数据类型支持

**表 4** A1/B1->A2/B2数据类型支持

|源操作数类型|目的操作数类型|是否支持|
|--|--|--|
|half|half|√|
|bfloat16_t|bfloat16_t|√|
|uint32_t|uint32_t|√|
|int32_t|int32_t|√|
|float|float|√|
|uint8_t|uint8_t|√|
|int8_t|int8_t|√|
|fp8_e4m3fn_t|fp8_e4m3fn_t|√|
|fp8_e5m2_t|fp8_e5m2_t|√|
|hifloat8_t|hifloat8_t|√|
|fp4x2_e2m1_t|fp4x2_e2m1_t|√|
|fp4x2_e1m2_t|fp4x2_e1m2_t|√|
|uint16_t|uint16_t|√|
|int16_t|int16_t|√|

## 数据排布支持<a id="format"></a>

**表 5** A1->A2数据排布支持

|源操作数排布|目的操作数排布|是否转置|
|--|--|--|
|Nz|Nz|x|
|Zn|Nz|√|

**表 6** B1->B2数据排布支持

|源操作数排布|目的操作数排布|是否转置|
|--|--|--|
|Nz|Zn|√|
|Zn|Zn|x|

## 约束说明

- Tensor Layout相关约束：
  - Shape、Stride仅支持四维，针对不同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[层次化表述法](../../tensor/Layout和层次化表述法.md)。
  - Shape、Stride具体维度的数据，仅支持基础整数类型和Std::Int类型。
  - LoadData要求A1、B1上Layout排布格式为Nz或Zn，A2上Layout排布为Nz，B2上Layout排布为Zn。
- A1->A2和B1->B2通路使能转置时，即Zn2Nz或Nz2Zn，支持的数据类型约束如下：
  - b32数据类型要求源矩阵Shape在K轴方向16对齐。
  - b8数据类型要求源矩阵Shape在M轴方向32对齐。
  - b4数据类型要求源矩阵Shape在M轴方向64对齐。
 
## 返回值说明

VerifyingLoadDataTemplate/VerifyingLoadDataTemplateWithCoord表达式为模板参数校验表达式，执行效果如下：

-   校验表达式成立，表明输入数据类型符合使用约束，LoadData函数表达式返回值为void。
-   校验表达式不成立，表明输入数据类型不符合使用约束时，LoadData函数声明失效，继而会导致编译错误。

## 调用示例

```cpp
// 以L1->L0A，half类型为例
using namespace AscendC::Te;
using SrcT = half;
using DstT = half;
size_t m = 128;
size_t k = 64;

auto dstLayout = MakeNZLayout<DstT>(m, k);
auto srcLayout = MakeNZLayout<SrcT>(m, k);

__cbuf__ half l1Addr[256*256*2];
__ca__ half l0AAddr[128*128*2];
auto l1Ptr = MakeL1memPtr(l1Addr);
auto l0APtr = MakeL0AmemPtr(l0AAddr);
auto l1Buff = l1Ptr + 0;
auto l0ABuff = l0APtr + 0;

auto srcTensor = MakeTensor(l1Buff, srcLayout);
auto dstTensor = MakeTensor(l0ABuff, dstLayout);

// 调用方式1
Copy(CopyAtom<CopyTraits<CopyL12L0, LoadDataTraitDefault>>{}, dstTensor, srcTensor);

// 调用方式2
CopyAtom<CopyTraits<CopyL12L0, LoadDataTraitDefault>>{}.Call(dstTensor, srcTensor);

// 调用方式3
auto atmoCopyL1A2L0A = MakeCopy(CopyL12L0P{}, LoadDataTraitDefault{});
atmoCopyL1A2L0A.Call(dstTensor, srcTensor);
```

