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

LoadData接口支持Local Memory内部的数据搬运，能够完成L1 Buffer -\> L0A Buffer或L1 Buffer -\> L0B Buffer（A1到A2或B1到B2）的整块矩阵数据搬运。

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
|transposed|是否启用转置功能，对每个分形矩阵进行转置，默认为false：<br>&bull; true：启用<br>&bull; false：不启用<br>|

## 返回值说明>

VerifyingLoadDataTemplate/VerifyingLoadDataTemplateWithCoord表达式为模板参数校验表达式，执行效果如下：

-   校验表达式成立，表明输入数据类型符合使用约束，LoadData函数表达式返回值为void。
-   校验表达式不成立，表明输入数据类型不符合使用约束时，LoadData函数声明失效，继而会导致编译错误。

## 约束说明

- Tensor Layout相关约束：
  - TensorTrait中的Layout表述，统一采用[层次化表述法](../struct/defenition/Layout和层次化表述法.md)同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[数据通路说明]。
  - dst的Shape、Stride只支持两个维度的数据配置。
  - Shape、Stride等数值数据，仅支持size\_t类型和Std::[Int]类型。

## 数据通路说明

**表 4**  L1 Buffer -\> L0A Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor。|
|模板参数U|数据类型为LocalTensor。|
|目的操作数TPosition取值范围|TPosition::A2|
|源操作数TPosition取值范围|TPosition::A1|
|目的操作数的数据类型|half、bfloat16_t、uint32_t、int32_t、float、uint8_t、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t、fp4x2_e2m1_t、fp4x2_e1m2_t|
|源操作数的数据类型|half、bfloat16_t、uint32_t、int32_t、float、uint8_t、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t、fp4x2_e2m1_t、fp4x2_e1m2_t|
|源操作数和目的操作数基础数据类型是否要求一致|是|
|源操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|目的操作数数据对齐要求|起始地址和数据长度要求512字节对齐。|
|数据排布要求|NZ/ZN Layout -> NZ Layout : 源操作数和目的操作数均仅支持连续排布。<br>NZ Layout内层矩阵的Shape大小是16 * (32 / sizeof(T))。<br>ZN Layout内层矩阵的Shape大小是(32 / sizeof(T)) * 16。|


**表 5**  L1 Buffer -\> L0B Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor。|
|模板参数U|数据类型为LocalTensor。|
|目的操作数TPosition取值范围|TPosition::B2|
|源操作数TPosition取值范围|TPosition::B1|
|目的操作数的数据类型|half、bfloat16_t、uint32_t、int32_t、float、uint8_t、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t、fp4x2_e2m1_t、fp4x2_e1m2_t|
|源操作数的数据类型|half、bfloat16_t、uint32_t、int32_t、float、uint8_t、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t、fp4x2_e2m1_t、fp4x2_e1m2_t|
|源操作数和目的操作数基础数据类型是否要求一致|是|
|源操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|目的操作数数据对齐要求|起始地址和数据长度要求512字节对齐。|
|数据排布要求|NZ/ZN Layout -> ZN Layout : 源操作数和目的操作数均仅支持连续排布。<br>NZ Layout内层矩阵的Shape大小是16 * (32 / sizeof(T))。<br>ZN Layout内存矩阵的Shape大小是 (32 / sizeof(T)) * 16。|

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

