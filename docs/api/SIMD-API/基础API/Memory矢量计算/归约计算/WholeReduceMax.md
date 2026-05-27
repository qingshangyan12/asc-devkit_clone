# WholeReduceMax

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品AI Core | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | √ </cann-filter>|
| <cann-filter npu-type="x90">Kirin X90 | √ </cann-filter>|
| <cann-filter npu-type="9030">Kirin 9030 | √ </cann-filter>|

## 功能说明

每个repeat内所有数据求最大值以及其索引index，返回的索引值为每个repeat内部索引。归约指令的总体介绍请参考[如何使用归约计算API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/类库API/基础API/常用操作速查指导/如何使用归约计算API.md)。

## 函数原型

- mask逐bit模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void WholeReduceMax(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask[], const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride, ReduceOrder order = ReduceOrder::ORDER_VALUE_INDEX)
    ```

- mask连续模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void WholeReduceMax(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t mask, const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride, ReduceOrder order = ReduceOrder::ORDER_VALUE_INDEX)
    ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。<br><br>Ascend 950PR/Ascend 950DT，支持的数据类型为：uint16_t/int16_t/uint32_t/int32_t/half/float<br><br>Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：half/float<br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：half/float<br><br>Atlas 200I/500 A2 推理产品，支持的数据类型为：half/float<br><br>Atlas 推理系列产品AI Core，支持的数据类型为：half/float<br><br>Atlas 训练系列产品，支持的数据类型为：half<br><br>Kirin X90，支持的数据类型为：half/float<br><br>Kirin 9030，支持的数据类型为：half/float |
| isSetMask | 是否在接口内部设置mask。<br>true，表示在接口内部设置mask。false，表示在接口外部设置mask，开发者需要使用[SetVectorMask](../掩码操作/SetVectorMask.md)接口设置mask值。这种模式下，接口入参中的mask值设置为占位符MASK_PLACEHOLDER，用于占位，无实际含义。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要保证4字节对齐（针对half数据类型），8字节对齐（针对float数据类型）。 |
| src | 输入 | 源操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。<br><br>源操作数的数据类型需要与目的操作数保持一致。 |
| mask/mask[] | 输入 | [mask](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)用于控制每次迭代内参与计算的元素。<br>逐bit模式：可以按位控制哪些元素参与计算，bit位的值为1表示参与计算，0表示不参与。<br>mask为数组形式，数组长度和数组元素的取值范围和操作数的数据类型有关。当操作数为16位时，数组长度为2，mask[0]、mask[1]∈[0, 264-1]并且不同时为0；当操作数为32位时，数组长度为1，mask[0]∈(0, 264-1]；当操作数为64位时，数组长度为1，mask[0]∈(0, 232-1]。<br><br>例如，mask=[8, 0]，8=0b1000，表示仅第4个元素参与计算。<br><br>连续模式：表示前面连续的多少个元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，mask∈[1, 128]；当操作数为32位时，mask∈[1, 64]；当操作数为64位时，mask∈[1, 32]。 |
| repeatTime | 输入 | 迭代次数。取值范围为[0, 255]。<br><br>关于该参数的具体描述请参考[高维切分API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| dstRepStride | 输入 | 目的操作数相邻迭代间的地址步长。以一个repeat归约后的长度为单位。<br><br>返回索引和最值时，单位为dst数据类型所占字节长度的两倍。比如当dst为half时，单位为4Bytes；<br><br>仅返回最值时，单位为dst数据类型所占字节长度；<br><br>仅返回索引时，单位为uint32_t类型所占字节长度。<br><br>注意，此参数值Atlas 训练系列产品不支持配置0。 |
| srcBlkStride | 输入 | 单次迭代内datablock的地址步长。详细说明请参考[dataBlockStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的datablock数目。详细说明请参考[repeatStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| order | 输入 | 使用order参数指定dst中index与value的相对位置以及返回结果行为，ReduceOrder类型，默认值为ORDER_VALUE_INDEX。取值范围如下：<br>ORDER_VALUE_INDEX：表示value位于低半部，返回结果存储顺序为[value, index]。ORDER_INDEX_VALUE：表示index位于低半部，返回结果存储顺序为[index, value]。ORDER_ONLY_VALUE：表示只返回最值，返回结果存储顺序为[value]。ORDER_ONLY_INDEX：表示只返回最值索引，返回结果存储顺序为[index]。<br><br>Ascend 950PR/Ascend 950DT，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE、ORDER_ONLY_VALUE、ORDER_ONLY_INDEX。<br><br>Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE、ORDER_ONLY_VALUE、ORDER_ONLY_INDEX。<br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE、ORDER_ONLY_VALUE、ORDER_ONLY_INDEX。<br><br>Atlas 200I/500 A2 推理产品，支持ORDER_VALUE_INDEX、ORDER_ONLY_VALUE。<br><br>Atlas 推理系列产品AI Core，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE。<br><br>Atlas 训练系列产品，支持ORDER_VALUE_INDEX。<br><br>Kirin X90，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE。<br><br>Kirin 9030，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE。 |

## 返回值说明

无

## 约束说明

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../通用说明和约束.md#section668772811100)。

- dst结果存储顺序由order决定，默认为最值、最值索引。返回结果中索引index数据按照dst的数据类型进行存储，比如dst使用half类型时，index按照half类型进行存储，读取时需要使用reinterpret\_cast方法转换到整数类型。若输入数据类型是half，需要使用reinterpret\_cast<uint16\_t\*\>，若输入是float，需要使用reinterpret\_cast<uint32\_t\*\>。特别地，针对Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品，ORDER\_ONLY\_INDEX（仅返回最值索引）情况下，读取index时都需要使用reinterpret\_cast<uint32\_t\*\>。针对Ascend 950PR/Ascend 950DT，ORDER\_ONLY\_INDEX（仅返回最值索引）情况下，当操作数数据类型为uint16\_t/int16\_t/half时，读取index都需要使用reinterpret\_cast<uint32\_t\*\>。
- 针对不同场景合理使用归约指令可以带来性能提升，相关介绍请参考[选择低延迟指令，优化归约操作性能](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/算子实践参考/SIMD算子性能优化/矢量计算/选择低延迟指令-优化归约操作性能.md)，具体样例请参考[ReduceCustom](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/03_libraries/04_reduce/reduce)。

## 调用示例

完整样例可参考[WholeReduce系列归约指令样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。

- tensor高维切分计算样例-mask连续模式

    ```cpp
    // dstLocal,srcLocal均为half类型，srcLocal的计算数据量为512，连续排布，计算结果也需要连续排布，使用tensor高维切分计算接口，设定mask为最多的128个全部元素参与计算
    // 根据以上信息，推断出repeatTime为4，dstRepStride为1，srcBlkStride为1，srcRepStride为8
    // 若求最大值及索引，并且需要存储顺序为[value, index]的结果，可以使用默认order，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, 128, 4, 1, 1, 8);

    // 若求最大值及索引，并且需要存储顺序为[index, value]的结果，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, 128, 4, 1, 1, 8, AscendC::ReduceOrder::ORDER_INDEX_VALUE);

    // 若只求最大值，并且需要存储[value]的结果，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, 128, 4, 1, 1, 8, AscendC::ReduceOrder::ORDER_ONLY_VALUE);

    // 若只求索引，并且需要存储[index]的结果，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, 128, 4, 1, 1, 8, AscendC::ReduceOrder::ORDER_ONLY_INDEX);
    ```

- tensor高维切分计算样例-mask逐bit模式

    ```cpp
    // dstLocal,srcLocal均为half类型，srcLocal的计算数据量为512，连续排布，计算结果也需要连续排布，使用tensor高维切分计算接口，设定mask为最多的128个全部元素参与计算
    uint64_t mask[2] = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF };

    // 根据以上信息，推断出repeatTime为4，dstRepStride为1，srcBlkStride为1，srcRepStride为8

    // 若求最大值及索引，并且需要存储顺序为[value, index]的结果，使用默认order，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, mask, 4, 1, 1, 8);

    // 若求最大值及索引，并且需要存储顺序为[index, value]的结果，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, mask, 4, 1, 1, 8, AscendC::ReduceOrder::ORDER_INDEX_VALUE);

    // 若只求最大值，并且需要存储[value]的结果，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, mask, 4, 1, 1, 8, AscendC::ReduceOrder::ORDER_ONLY_VALUE);

    // 若只求索引，并且需要存储[index]的结果，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, mask, 4, 1, 1, 8, AscendC::ReduceOrder::ORDER_ONLY_INDEX);
    ```

    示例结果如下：

    ```cpp
    输入数据src_gm：
    [1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
     1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
     1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
     1   1   1   1   11   1   1   1   1   1   1   1   1   1   1   1
     1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
     1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
     1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
     1   1   1   1   1   1   1   1   1   1   1   1   1   1   1   1
     2   2   2   2   2   2   2   2   2   2   2   2   2   2   2   2
     2   2   2   2   2   2   2   2   2   2   2   2   2   2   2   2
     2   2   2   2   2   2   2   2   2   2   2   2   2   2   2   2
     2   2   2   2   2   2   2   2   2   2   2   2   2   2   2   2
     2   2   2   2   2   2   2   2   2   2   2   2   2   2   2   2
     2   2   2   2   2   2   2   2   2   2   2   2   2   2   2   2
     2   2   2   2   12   2   2   2   2   2   2   2   2   2   2   2
     2   2   2   2   2   2   2   2   2   2   2   2   2   2   2   2
     ...
     3   3   3   3   3   3   3   3   3   3   3   3   3   3   3   3
     3   3   3   13   3   3   3   3   3   3   3   3   3   3   3   3
     3   3   3   3   3   3   3   3   3   3   3   3   3   3   3   3
     3   3   3   3   3   3   3   3   3   3   3   3   3   3   3   3
     3   3   3   3   3   3   3   3   3   3   3   3   3   3   3   3
     3   3   3   3   3   3   3   3   3   3   3   3   3   3   3   3
     3   3   3   3   3   3   3   3   3   3   3   3   3   3   3   3
     3   3   3   3   3   3   3   3   3   3   3   3   3   3   3   3]

    若ReduceOrder类型为ORDER_VALUE_INDEX或默认，则输出数据dst_gm：
    [11 3.09944e-06 12 5.96046e-06 ... 13 1.13249e-06]
    若ReduceOrder类型为ORDER_INDEX_VALUE，则输出数据dst_gm：
    [3.09944e-06 11 5.96046e-06 12 ... 1.13249e-06 13]
    若ReduceOrder类型为ORDER_ONLY_VALUE，则输出数据dst_gm：
    [11 12 ... 13 0 0 0 ...]
    若ReduceOrder类型为ORDER_ONLY_VALUE，则输出数据dst_gm：
    [3.09944e-06 0 5.96046e-06 0 ... 1.13249e-06 0]

    其中，index的值为int数值的二进制，在half中的表达，以上述结果为例：
    前128个数中，11的位置在对应的repeat中为52，十六进制为0x3400，对应half值为3.09944e-06。
    第二个128个数中，12的位置在对应的repeat中为100，十六进制为0x6400，对应half值为5.96046e-06。
    最后128个数中，13的位置在对应的repeat中为19，十六进制为0x1300，对应half值为1.13249e-06。
    ```
