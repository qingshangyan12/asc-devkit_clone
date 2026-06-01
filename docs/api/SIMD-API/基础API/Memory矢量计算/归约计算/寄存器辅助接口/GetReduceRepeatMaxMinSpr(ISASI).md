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

头文件路径为：`"basic_api/kernel_operator_vec_reduce_intf.h"`。

// 获取reducerepeat 所有iteration上的最值及索引，对于reducemax和reducemin来说没法获取，
获取[RepeatReduce<MAX/MIN>](../ReduceRepeat.md)、[ReduceMax](../ReduceMax.md)、[ReduceMin](../ReduceMin.md)接口的结果（最值以及对应的索引值），仅支持连续计算场景（repeatStride=8，blockStride=1）。计算结果以全局变量形式存储，可以随时调用获取。

## 函数原型

<cann-filter npu-type="950,A3,910b">

以下函数原型仅支持：

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

<cann-filter npu-type="310p">

以下函数原型仅支持：

Atlas 推理系列产品 AI Core

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
| maxMinValue | 输出 | [ReduceMax](../ReduceMax.md)/[ReduceMin](../ReduceMin.md)返回的最值。 |
| maxMinIndex | 输出 | [ReduceMax](../ReduceMax.md)/[ReduceMin](../ReduceMin.md)返回的最值索引。 |

## 数据类型

操作数支持的数据类型为：`half`/`float`。

## 返回值说明

无

## 约束说明

- <cann-filter npu-type="950">针对Ascend 950PR/Ascend 950DT，由于`ReduceMax`/`ReduceMin`的内部实现原因，直接调用`GetReduceRepeatMaxMinSpr`接口无法获取到准确的索引值，验证时需要使用[ReduceRepeat](../ReduceRepeat.md)接口来获取准确的索引值。同时，`GetReduceRepeatMaxMinSpr`必须紧跟着[ReduceRepeat](../ReduceRepeat.md)接口进行调用。</cann-filter>
- <cann-filter npu-type = "A3,910b"><cann-filter npu-type = "A3">针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、</cann-filter><cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品</cann-filter>，由于`ReduceMax`/`ReduceMin`的内部实现原因，直接调用`GetReduceRepeatMaxMinSpr`接口无法获取到准确的索引值，验证时需要使用[ReduceRepeat](../ReduceRepeat.md)接口来获取准确的索引值。</cann-filter>
- 索引`maxMinIndex`数据是按照`ReduceMax`/`ReduceMin`的数据类型进行存储的，比如`ReduceMax`/`ReduceMin`使用`half`类型时，`maxMinIndex`是按照`half`类型进行存储的，如果按照`half`格式进行读取，`maxMinIndex`的值是不对的，因此`maxMinIndex`的读取需要使用`reinterpret_cast`方法转换到整数类型，若输入数据类型是`half`，需要使用`reinterpret_cast<uint16_t*>`，若输入是`float`，需要使用`reinterpret_cast<uint32_t*>`。
- 操作数数据类型T，需要与所使用的`ReduceMax`/`ReduceMin`指令的操作数数据类型一致，否则会导致精度错误。<!-- TODO 待测试 -->
- 可能超出数据范围，因为index用的32位

## 调用示例

具体可参考[ReduceRepeat系列归约指令样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。

1. 以`ReduceMax`指令为例，首先执行`ReduceMax`指令。

    ```cpp
    AscendC::LocalTensor<float> src;
    AscendC::LocalTensor<float> work;
    AscendC::LocalTensor<float> dst;
    int32_t mask = 64;
    AscendC::ReduceMax(dst, src, work, mask, 1, 8, true); // 连续场景，srcRepStride = 8，且calIndex = true
    ```

2. 获取上述`ReduceMax`指令的最值与索引值。

    <cann-filter npu-type="950">

    - 针对Ascend 950PR/Ascend 950DT，需要先执行`ReduceRepeat`指令，随后立即调用`GetReduceRepeatMaxMinSpr`指令。

        ```cpp
        AscendC::LocalTensor<float> src;
        AscendC::LocalTensor<float> dst;
        int32_t mask = 64;
        float val = 0;   // 最大值
        float idx = 0;   // 最大值的索引值，与ReduceMax的结果相同
        AscendC::ReduceRepeat<AscendC::ReduceType::MAX>(dst, src, mask, 1, 1, 1, 8);
        AscendC::GetReduceRepeatMaxMinSpr<float>(val, idx); // 保证和ReduceRepeat的调用次序，而且要配对调用
        ```

    </cann-filter>

    <cann-filter npu-type = "A3,910b">

    - <cann-filter npu-type = "A3">针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、</cann-filter><cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品</cann-filter>，需要使用`ReduceRepeat`指令获取准确的索引值，然后再调用`GetReduceRepeatMaxMinSpr`指令。

        ```cpp
        AscendC::LocalTensor<float> src;
        AscendC::LocalTensor<float> dst;
        int32_t mask = 64;
        AscendC::ReduceRepeat<AscendC::ReduceType::MAX>(dst, src, mask, 1, 1, 1, 8);
        float val = 0;   // 最大值
        float idx = 0;   // 最大值的索引值，与ReduceMax的结果相同，保证和ReduceRepeat的调用次序，而且要配对调用
        AscendC::GetReduceRepeatMaxMinSpr<float>(val, idx);
        ```

    </cann-filter>

    <cann-filter npu-type="310p">

    - 针对Atlas 推理系列产品 AI Core版本，则可在调用`ReduceMax`后直接调用`GetReduceRepeatMaxMinSpr`指令获取其最大/最小值。

        ```cpp
        AscendC::LocalTensor<float> src;
        AscendC::LocalTensor<float> work;
        AscendC::LocalTensor<float> dst;
        int32_t mask = 64;
        AscendC::ReduceMax(dst, src, work, mask, 1, 8, true);
        float val = 0;   // 最大值
        AscendC::GetReduceRepeatMaxMinSpr<float>(val); // 保证和ReduceMax的调用次序，而且要配对调用
        ```

    </cann-filter>
