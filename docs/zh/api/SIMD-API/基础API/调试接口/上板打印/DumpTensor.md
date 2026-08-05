# DumpTensor<a name="ZH-CN_TOPIC_0000001584608650"></a>

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明<a name="section259105813316"></a>

头文件路径为：`"basic_api/kernel_operator_dump_tensor_intf.h"`。

该接口可以打印Tensor的内容，同时支持打印自定义的标签（仅支持uint32_t数据类型的信息），比如打印当前行号等。
在算子kernel侧实现代码中需要打印Tensor数据的地方，调用DumpTensor接口打印相关内容。样例如下：

```cpp
AscendC::DumpTensor(srcLocal, 5, dataLen);
```
> [!CAUTION]注意
> 该接口主要用于调试分析，开启后会对算子性能产生一定影响，通常在调试阶段使用，生产环境建议关闭。<br>
> 默认情况下，调用该接口就会打印相关内容，开发者可以参考[关闭ASCENDC_DUMP说明](../关闭ASCENDC_DUMP说明.md)，按需关闭该接口功能。

打印示例如下：  
```plain
DumpTensor: desc=5, addr=0, data_type=float16, position=UB, dump_size=32
[19.000000, 4.000000, 38.000000, 50.000000, 39.000000, 67.000000, 84.000000, 98.000000, 21.000000, 36.000000, 18.000000, 46.000000, 10.000000, 92.000000, 26.000000, 38.000000, 39.000000, 9.000000, 82.000000, 37.000000, 35.000000, 65.000000, 97.000000, 59.000000, 89.000000, 63.000000, 70.000000, 57.000000, 35.000000, 3.000000, 16.000000,
42.000000]
DumpTensor: desc=5, addr=100, data_type=float16, position=UB, dump_size=32
[6.000000, 34.000000, 52.000000, 38.000000, 73.000000, 38.000000, 35.000000, 14.000000, 67.000000, 62.000000, 30.000000, 49.000000, 86.000000, 37.000000, 84.000000, 18.000000, 38.000000, 18.000000, 44.000000, 21.000000, 86.000000, 99.000000, 13.000000, 79.000000, 84.000000, 9.000000, 48.000000, 74.000000, 52.000000, 99.000000, 80.000000,
53.000000]
...
DumpTensor: desc=5, addr=0, data_type=float16, position=UB, dump_size=32
[35.000000, 41.000000, 41.000000, 22.000000, 84.000000, 49.000000, 60.000000, 0.000000, 90.000000, 14.000000, 67.000000, 80.000000, 16.000000, 46.000000, 16.000000, 83.000000, 6.000000, 70.000000, 97.000000, 28.000000, 97.000000, 62.000000, 80.000000, 22.000000, 53.000000, 37.000000, 23.000000, 58.000000, 65.000000, 28.000000, 4.000000,
29.000000]
```

## 函数原型<a name="section2067518173415"></a>

- 无Tensor shape的打印

    ```cpp
    template <typename T>
    __aicore__ inline void DumpTensor(const LocalTensor<T> &tensor, uint32_t desc, uint32_t dumpSize)
    template <typename T>
    __aicore__ inline void DumpTensor(const GlobalTensor<T>& tensor, uint32_t desc, uint32_t dumpSize)
    ```

- 带Tensor shape的打印

    ```cpp
    template <typename T>
    __aicore__ inline void DumpTensor(const LocalTensor<T>& tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
    template <typename T>
    __aicore__ inline void DumpTensor(const GlobalTensor<T>& tensor, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
    ```

## 参数说明<a name="section158061867342"></a>

**表1**  模板参数说明

| 参数名称 | 描述 |
| ------ | ------ |
| T | 需要dump的Tensor的数据类型。 |

**表2**  参数说明

| 参数名称 | 输入/输出 | 描述 |
| ------ | ------ | ------ |
| tensor | 输入 | 需要dump的Tensor。<br>•待dump的tensor位于Unified Buffer/L1 Buffer/L0 Buffer时使用LocalTensor类型的tensor参数输入。<br>•待dump的tensor位于Global Memory时使用GlobalTensor类型的tensor参数输入。|
| desc | 输入 | 用户自定义附加信息（行号或其他自定义数字）。<br>在使用DumpTensor功能时，用户可通过desc参数附加自定义信息，以便在不同场景下区分Dump内容的来源。此功能有助于精准定位具体DumpTensor的输出，提升调试与分析效率。|
| dumpSize | 输入 | 需要dump的元素个数。|
| shapeInfo | 输入 | 传入Tensor的shape信息，可按照shape信息进行打印。<br>•当Shape尺寸大于dumpSize元素个数时，按照ShapeInfo打印元素，不足的Dump数据用"-"展示。<br>•当Shape尺寸小于等于dumpSize元素个数时，按照ShapeInfo打印元素，多出的Dump数据不展示。|

## 数据类型

<!-- npu="950" id101 -->
- Ascend 950PR/Ascend 950DT，T支持的数据类型为：bool、int8_t、uint8_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t。<!-- end id101 -->
<!-- npu="A3" id102 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品，T支持的数据类型为：bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t。<!-- end id102 -->
<!-- npu="910b" id103 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品，T支持的数据类型为：bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t。<!-- end id103 -->
<!-- npu="310b" id104 -->
- Atlas 200I/500 A2 推理产品，T支持的数据类型为：bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t。<!-- end id104 -->
<!-- npu="310p" id105 -->
- Atlas 推理系列产品AI Core，T支持的数据类型为：bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t。<!-- end id105 -->