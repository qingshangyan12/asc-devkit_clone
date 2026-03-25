# DataCopyGM2L1

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

DataCopy提供数据搬运功能，该接口支持Global Memory到L1 Buffer之间的数据搬运，同时支持随路数据格式转换。

## 函数原型

- 不带Coord的接口

    ```cpp
    template <const DataCopyTrait& trait = DEFAULT_DATA_COPY_TRAIT, typename T, typename U>
    __aicore__ inline typename Std::enable_if<VerifyingDataCopyTemplate<T, U>, void>::type
    DataCopy(const T& dst, const U& src)
    ```

- 带Coord的接口

    ```cpp
    template <const DataCopyTrait& trait = DEFAULT_DATA_COPY_TRAIT, typename T, typename U, typename Coord>
    __aicore__ inline typename Std::enable_if<VerifyingDataCopyTemplateWithCoord<T, U, Coord>, void>::type
    DataCopy(const T& dst, const U& src, const Coord& coord)
    ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| -- | -- |
| T | 目的操作数的数据类型，通过MakeTensor构造的[LocalTensor类型](../../tensor/Tensor.md)，存储位置支持L1，数据格式支持ND、NZ、ZN、ZZ和NN。 |
| U | 源操作数的数据类型，通过MakeTensor构造的[LocalTensor类型](../../tensor/Tensor.md)，存储位置支持GM，数据格式仅支持ND、DN、NZ、ZN、ZZ、NN、ScaleAND、ScaleADN、ScaleBND和ScaleBDN。 |
| Coord | 坐标偏移的数据类型，通过MakeCoord构造的[Coord类型](../../tensor/Coord.md)。 |
| DataCopyTrait | 预留参数，保持默认值即可。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 含义 |
| -- | -- | -- |
| dst | 输出 | 目的操作数。 |
| src | 输入 | 源操作数。 |
| coord | 输入 | 源操作数上的偏移坐标。 |

## 数据类型和数据格式支持

**表 3**  GM->L1数据类型和数据格式支持

| 源操作数数据格式 | 目的操作数数据格式 | 源操作数/目的操作数数据类型 |
| -- | -- | -- |
| ND | ND | int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float/int64_t/uint64_t |
| ND | NZ | fp4x2_e2m1_t/fp4x2_e1m2_t/int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float |
| ND | ZN | int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float |
| DN | NZ | int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float |
| DN | ZN | fp4x2_e2m1_t/fp4x2_e1m2_t/int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float |
| NZ | NZ | int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float/int64_t/uint64_t |
| ScaleAND | ZZ | fp8_e8m0_t |
| ScaleADN | ZZ | fp8_e8m0_t |
| ZZ | ZZ | fp8_e8m0_t |
| ScaleBND | NN | fp8_e8m0_t |
| ScaleBDN | NN | fp8_e8m0_t |
| NN | NN | fp8_e8m0_t |

## 返回值说明

VerifyingDataCopyTemplate表达式为模板参数T和U的校验表达式，VerifyingDataCopyTemplateWithCoord表达式为模板参数T、U和Coord的校验表达式，执行效果如下：

- 校验表达式成立，表明输入参数符合DataCopy接口入参数据类型约束，DataCopy函数表达式返回值为void。
- 校验表达式不成立，表明输入参数不符合DataCopy接口入参数据类型约束时，DataCopy函数声明失效，继而会导致编译错误。

## 约束说明

- 地址重叠约束：无约束。
- 地址对齐约束：源操作数起始地址要求按元素位宽对齐。目的操作数起始地址要求满足32字节对齐。
- 参数组合约束：无约束。
- 环境影响约束：无约束。
- 特殊数据类型约束：无约束。
- 性能约束：无约束。
- 异常和边界值处理：无约束。
- Tensor Layout相关约束：
  - Shape、Stride只支持四维，针对不同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[层次化表达法](../../tensor/Layout和层次化表述法.md)。
  - Shape、Stride具体维度的数据，仅支持基础整数类型和Std::Int类型。
  - ND2ND场景只支持一维数据格式，Layout的Shape中的ShapeRow1或ShapeColumn1需要设置为Std::Int<1>{}。

## 调用示例

```cpp
...
// 以GM->L1，half类型为例
using namespace AscendC::Te;
using SrcT = half;
using DstT = half;
size_t m = 128;
size_t k = 64;

auto srcLayout = MakeNDLayout<SrcT>(m, k);
auto dstLayout = MakeNZLayout<DstT>(m, k);

__gm__ half gmAAddr[256*256*2];
__cbuf__ half l1Addr[256*256*2];
auto gmAPtr = MakeGMmemPtr(gmAAddr);
auto l1Ptr = MakeL1memPtr(l1Addr);
auto gmABuff = gmAPtr + 0;
auto l1Buff = l1Ptr + 0;

auto srcTensor = MakeTensor(gmABuff, srcLayout);
auto dstTensor = MakeTensor(l1Buff, dstLayout);

// 调用方式1
Copy(CopyAtom<CopyTraits<CopyGM2L1, DataCopyTraitDefault>>{}, dstTensor, srcTensor);

// 调用方式2
CopyAtom<CopyTraits<CopyGM2L1, DataCopyTraitDefault>>{}.Call(dstTensor, srcTensor);

// 调用方式3
auto atomCopyGM2L1 = MakeCopy(CopyGM2L1{}, DataCopyTraitDefault{});
atomCopyGM2L1.Call(dstTensor, srcTensor);
...
```

完整样例请参考[TensorAPI样例代码](../../../../../../examples/02_features/05_tensor_api)
