# GetReduceRepeatMaxMinSpr(ISASI)

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | x </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品 AI Core | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品 Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | x </cann-filter>|

## 功能说明

头文件为：\#include "basic\_api/kernel\_operator\_vec\_reduce\_intf.h"

获取[ReduceMax](../ReduceMax.md)、[ReduceMin](../ReduceMin.md)接口的结果（最值以及对应的索引值），仅支持连续计算场景（repeatStride=8，blockStride=1）。计算结果以全局变量形式存储，可以随时调用获取。

## 函数原型

<cann-filter npu-type="950,A3,910b">

以下函数原型仅支持:

<cann-filter npu-type="950">

Ascend 950PR/Ascend 950DT

</cann-filter>

<cann-filter npu-type="A3">

Atlas A3 训练系列产品/Atlas A3 推理系列产品

</cann-filter>

<cann-filter npu-type="910b">

Atlas A2 训练系列产品/Atlas A2 推理系列产品

</cann-filter>

- 获取[ReduceMax](../ReduceMax.md)、[ReduceMin](../ReduceMin.md)连续场景下的最大值与最小值，以及相应的索引值。

    ```cpp
    template <typename T>
    __aicore__ inline void GetReduceRepeatMaxMinSpr(T &maxMinValue, T &maxMinIndex)
    ```
    
</cann-filter>

<cann-filter npu-type="910">

以下函数原型仅支持

Atlas 训练系列产品

- 获取[ReduceMax](../ReduceMax.md)、[ReduceMin](../ReduceMin.md)连续场景下的最大值与最小值。

    ```cpp
    template <typename T>
    __aicore__ inline void GetReduceRepeatMaxMinSpr(T &maxMinValue)
    ```

</cann-filter>

## 参数说明

**表 1**  模板参数说明
<!--  -->
| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| maxMinValue | 输出 | ReduceMax/ReduceMin返回的最值。 |
| maxMinIndex | 输出 | ReduceMax/ReduceMin返回的最值索引。 |

## 数据类型

操作数支持的数据类型为：half/float。

## 返回值说明

无

## 约束说明

- 针对Ascend 950PR/Ascend 950DT，由于ReduceMax/ReduceMin的内部实现原因，直接调用GetReduceRepeatMaxMinSpr接口无法获取到准确的索引值，验证时需要使用[ReduceRepeat](../ReduceRepeat.md)接口来获取准确的索引值。同时，GetReduceRepeatMaxMinSpr必须紧跟着[ReduceRepeat](../ReduceRepeat.md)接口进行调用。
- 针对Atlas A2 训练系列产品/Atlas A2 推理系列产品，由于ReduceMax/ReduceMin的内部实现原因，直接调用GetReduceRepeatMaxMinSpr接口无法获取到准确的索引值，验证时需要使用[ReduceRepeat](../ReduceRepeat.md)接口来获取准确的索引值。
- 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品，由于ReduceMax/ReduceMin的内部实现原因，直接调用GetReduceRepeatMaxMinSpr接口无法获取到准确的索引值，验证时需要使用[ReduceRepeat](../ReduceRepeat.md)接口来获取准确的索引值。
- 索引maxMinIndex数据\`是按照ReduceMax/ReduceMin的数据类型进行存储的，比如ReduceMax/ReduceMin使用half类型时，maxMinIndex是按照half类型进行存储的，如果按照half格式进行读取，maxMinIndex的值是不对的，因此maxMinIndex的读取需要使用reinterpret\_cast方法转换到整数类型，若输入数据类型是half，需要使用reinterpret\_cast<uint16\_t\*\>，若输入是float，需要使用reinterpret\_cast<uint32\_t\*\>。
- 操作数数据类型T，需要与所使用的ReduceMax/ReduceMin指令的操作数数据类型一致，否则会导致精度错误。<!-- TODO 待测试 -->

## 调用示例

具体可参考[ReduceRepeat系列归约指令样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。

1. 以ReduceMax指令为例，首先执行ReduceMax指令。

    ```cpp
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<float> work;
    AscendC::LocalTensor<float> dst;
    int32_t mask = 64;
    AscendC::ReduceMax(dst, src, work, mask, 1, 8, true); // 连续场景，srcRepStride = 8，且calIndex = true
    ```

2. 获取上述ReduceMax指令的最值与索引值。

    针对Atlas A2 训练系列产品/Atlas A2 推理系列产品，需要使用ReduceRepeat指令获取准确的索引值，然后再调用GetReduceRepeatMaxMinSpr指令。

    ```cpp
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<float> dst;
    int32_t mask = 64;
    AscendC::ReduceRepeat<AscendC::ReduceType::MAX>(dst, src, mask, 1, 1, 1, 8);
    float val = 0;   // 最大值
    float idx = 0;   // 最大值的索引值，与ReduceMax的结果相同，保证和ReduceRepeat的调用次序，而且要配对调用
    AscendC::GetReduceRepeatMaxMinSpr<float>(val, idx);
    ```

    针对Atlas A3 训练系列产品/Atlas A3 推理系列产品，需要使用ReduceRepeat指令获取准确的索引值，然后再调用GetReduceRepeatMaxMinSpr指令。

    ```cpp
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<float> dst;
    int32_t mask = 64;
    AscendC::ReduceRepeat<AscendC::ReduceType::MAX>(dst, src, mask, 1, 1, 1, 8);
    float val = 0;   // 最大值
    float idx = 0;   // 最大值的索引值，与ReduceMax的结果相同，保证和ReduceRepeat的调动次序，而且要配对调用
    AscendC::GetReduceRepeatMaxMinSpr<float>(val, idx);
    ```

    针对Atlas推理系列产品AI Core版本，则可在调用ReduceMax后直接调用GetReduceRepeatMaxMinSpr指令获取其最大/最小值。

    ```cpp
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<float> work;
    AscendC::LocalTensor<float> dst;
    int32_t mask = 64;
    AscendC::ReduceMax(dst, src, work, mask, 1, 8, true);
    float val = 0;   // 最大值
    AscendC::GetReduceRepeatMaxMinSpr<float>(val); // 保证和ReduceMax的调动次序，而且要配对调用
    ```

    针对Ascend 950PR/Ascend 950DT，需要先执行ReduceRepeat指令，随后立即调用GetReduceRepeatMaxMinSpr指令。

    ```cpp
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<float> dst;
    int32_t mask = 64;
    float val = 0;   // 最大值
    float idx = 0;   // 最大值的索引值，与ReduceMax的结果相同
    AscendC::ReduceRepeat<AscendC::ReduceType::MAX>(dst, src, mask, 1, 1, 1, 8);
    AscendC::GetReduceRepeatMaxMinSpr<float>(val, idx); // 保证和ReduceRepeat的调动次序，而且要配对调用
    ```
