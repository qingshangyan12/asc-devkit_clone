# UB-\>UB掩码式高维数据搬运\(Copy\)<a name="ZH-CN_TOPIC_0000002575088175"></a>

## 产品支持情况<a name="zh-cn_topic_0000002567699435_section796754519912"></a>

| 产品 | 是否支持 |
| :--- | :---: |
|<cann-filter npu-type = "950"> Ascend 950PR/Ascend 950DT | x </cann-filter>|
|<cann-filter npu-type = "A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type = "910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type = "310b"> Atlas 200I/500 A2 推理产品 | √ </cann-filter>|
|<cann-filter npu-type = "310p"> Atlas 推理系列产品AI Core | x </cann-filter>|
|<cann-filter npu-type = "310p"> Atlas 推理系列产品Vector Core | x </cann-filter>|
|<cann-filter npu-type = "910"> Atlas 训练系列产品 | x </cann-filter>|
|<cann-filter npu-type = "x90"> Kirin X90 | √ </cann-filter>|
|<cann-filter npu-type = "9030"> Kirin 9030 | √ </cann-filter>|

## 功能说明<a name="zh-cn_topic_0000002567699435_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

DataCopy能够实现Unified Buffer和Unified Buffer之间的数据搬运，数据搬运时格式和内容保持不变，支持mask操作和DataBlock间隔操作。

## 函数原型<a name="zh-cn_topic_0000002567699435_section82039854412"></a>

-   mask逐bit模式

    ```
    template <typename T, bool isSetMask = true>
    __aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask[], const uint8_t repeatTime, const CopyRepeatParams& repeatParams)
    ```

-   mask连续模式 

    ```
    template <typename T, bool isSetMask = true>
    __aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask, const uint8_t repeatTime, const CopyRepeatParams& repeatParams)
    ```

## 参数说明<a name="zh-cn_topic_0000002567699435_section16128134420472"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 源操作数或者目的操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002567699435_section4219135304818)。 |
| isSetMask | 是否在接口内部设置mask。<br>&bull; true，表示在接口内部设置mask。<br>&bull; false，表示在接口外部设置mask，开发者需要使用[SetVectorMask](SetVectorMask.md#ZH-CN_TOPIC_0000002574024783)接口设置mask值。这种模式下，本接口入参中的mask值必须设置为占位符MASK_PLACEHOLDER。<br>isSetMask的具体使用方式可参考[掩码](掩码.md#ZH-CN_TOPIC_0000002573904669)。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数，类型为LocalTensor，存储位置为Unified Buffer，目的地址需要32B对齐。 |
| src | 输入 | 源操作数，类型为LocalTensor，存储位置为Unified Buffer，源地址需要32B对齐。 |
| mask[]/mask | 输入 | mask用于控制每次迭代内参与计算的元素。详细设置参考[掩码](掩码.md#ZH-CN_TOPIC_0000002573904669)。 |
| repeatTime | 输入 | 重复迭代次数。矢量计算单元，每次读取连续的256Bytes数据进行计算，为完成对输入数据的处理，必须通过多次迭代（repeat）才能完成所有数据的读取与计算。repeatTime表示迭代的次数。<br>关于该参数的具体描述请参考[高维切分](高维切分.md#ZH-CN_TOPIC_0000002556644319)。<br>**注：repeatTime = 0表示不会执行计算操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。** |
| repeatParams | 输入 | 控制操作数地址步长的参数。CopyRepeatParams类型，包含操作数相邻迭代间相同DataBlock的地址步长，操作数同一迭代内不同DataBlock的地址步长等参数。CopyRepeatParams参数说明请参考[表3](#zh-cn_topic_0000002567699435_table992513120142)。 |

**表 3**  CopyRepeatParams结构体参数定义

| 参数名称 | 含义 |
| --- | --- |
| dstStride | 目的操作数同一迭代内相邻DataBlock的地址步长，uint16_t类型，取值范围：dstStride∈[0, 65535]，单位：DataBlock（32B）。 |
| srcStride | 源操作数同一迭代内相邻DataBlock的地址步长，uint16_t类型，取值范围：srcStride∈[0, 65535]，单位：DataBlock（32B） |
| dstRepeatSize | 目的操作数相邻迭代间相同DataBlock的地址步长，uint16_t类型，取值范围：dstRepeatSize∈[0, 4095]，单位：DataBlock（32B）。 |
| srcRepeatSize | 源操作数相邻迭代间相同DataBlock的地址步长，uint16_t类型，取值范围：srcRepeatSize∈[0, 4095]，单位：DataBlock（32B）。 |

## 数据类型<a name="zh-cn_topic_0000002567699435_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持数据类型为：uint16\_t/int16\_t/half/bfloat16\_t/uint32\_t/int32\_t/float

Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持数据类型为：uint16\_t/int16\_t/half/bfloat16\_t/uint32\_t/int32\_t/float

## API映射关系<a name="zh-cn_topic_0000002567699435_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（vcopy）的基础上进行抽象封装实现的，其对应的底层built-in接口为：

```
void vcopy(__ubuf__ int16_t *dst, __ubuf__ int16_t *src, uint8_t repeat, uint16_t dstStride, uint16_t srcStride, uint16_t dstRepeatSize, uint16_t srcRepeatSize);

void vcopy(__ubuf__ int32_t *dst, __ubuf__ int32_t *src, uint8_t repeat, uint16_t dstStride, uint16_t srcStride, uint16_t dstRepeatSize, uint16_t srcRepeatSize);

void vcopy(__ubuf__ uint16_t *dst, __ubuf__ uint16_t *src, uint8_t repeat, uint16_t dstStride, uint16_t srcStride, uint16_t dstRepeatSize, uint16_t srcRepeatSize);

void vcopy(__ubuf__ uint32_t *dst, __ubuf__ uint32_t *src, uint8_t repeat, uint16_t dstStride, uint16_t srcStride, uint16_t dstRepeatSize, uint16_t srcRepeatSize);
```

## 约束说明<a name="zh-cn_topic_0000002567699435_section2045914466492"></a>

-   地址对齐约束参考[地址对齐约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section796754519912)。
-   操作数地址重叠约束请参考[地址重叠约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section668772811100)。
-   repeatTime = 0表示不会执行计算操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。
-   Copy和矢量计算API一样，支持和掩码操作API配合使用。但counter模式配合高维切分计算API时，和[通用的counter模式](接口内设置mask.md#ZH-CN_TOPIC_0000002543264752)有一定差异。具体差异如下：
    -   通用的counter模式：Mask代表**整个矢量计算参与计算的元素个数，迭代次数不生效**。
    -   Copy高维切分计算API的counter模式：Mask代表**每次Repeat中处理的元素个数，迭代次数生效。**

## 调用示例<a name="zh-cn_topic_0000002567699435_section088124295117"></a>

完整示例可参考[Copy样例](https://gitcode.com/cann/asc-devkit/tree/016b968f0528e78580ba95d34cf28faad80fcc96/examples/02_features/07_data_movement/copy)。
