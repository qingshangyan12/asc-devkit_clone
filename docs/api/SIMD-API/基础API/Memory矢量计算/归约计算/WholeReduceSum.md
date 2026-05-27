# WholeReduceSum

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

每个迭代内所有数据求和。归约指令的总体介绍请参考[如何使用归约计算API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/类库API/基础API/常用操作速查指导/如何使用归约计算API.md)。

## 函数原型

- mask逐bit模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void WholeReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask[], const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

- mask连续模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void WholeReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t mask, const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

- mask逐bit模式

    ```cpp
    template <typename T, bool isSetMask = true, typename U = T>
    __aicore__ inline void WholeReduceSum(const LocalTensor<U>& dst, const LocalTensor<T>& src, const uint64_t mask[], const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

- mask连续模式

    ```cpp
    template <typename T, bool isSetMask = true, typename U = T>
    __aicore__ inline void WholeReduceSum(const LocalTensor<U>& dst, const LocalTensor<T>& src, const int32_t mask, const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 源操作数数据类型。 |
| U | 目的操作数数据类型。 |
| isSetMask | 是否在接口内部设置mask。<br>true，表示在接口内部设置mask。false，表示在接口外部设置mask，开发者需要使用[SetVectorMask](../掩码操作/SetVectorMask.md)接口设置mask值。这种模式下，接口入参中的mask值设置为占位符MASK_PLACEHOLDER，用于占位，无实际含义。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要保证2字节对齐（针对half数据类型），4字节对齐（针对float数据类型）。<br><br>Ascend 950PR/Ascend 950DT，支持的数据类型为：uint32_t/int32_t/half/float<br><br>Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：half/float<br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：half/float<br><br>Atlas 200I/500 A2 推理产品，支持的数据类型为：half/float<br><br>Atlas 推理系列产品AI Core，支持的数据类型为：half/float<br><br>Atlas 训练系列产品，支持的数据类型为：half/float<br><br>Kirin X90，支持的数据类型为：half/float<br><br>Kirin 9030，支持的数据类型为：half/float |
| src | 输入 | 源操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。<br><br>源操作数的数据类型需要与目的操作数保持一致。<br><br>针对Ascend 950PR/Ascend 950DT：src数据类型uint16_t和int16_t时，dst数据类型分别为uint32_t和int32_t，其他情况下，dst数据类型均与src数据类型相同。<br><br>Ascend 950PR/Ascend 950DT，支持的数据类型为：uint16_t/int16_t/uint32_t/int32_t/half/float<br><br>Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：half/float<br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：half/float<br><br>Atlas 200I/500 A2 推理产品，支持的数据类型为：half/float<br><br>Atlas 推理系列产品AI Core，支持的数据类型为：half/float<br><br>Atlas 训练系列产品，支持的数据类型为：支持的数据类型为half/float<br><br>Kirin X90，支持的数据类型为：half/float<br><br>Kirin 9030，支持的数据类型为：half/float |
| mask/mask[] | 输入 | [mask](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)用于控制每次迭代内参与计算的元素。<br>逐bit模式：可以按位控制哪些元素参与计算，bit位的值为1表示参与计算，0表示不参与。<br>mask为数组形式，数组长度和数组元素的取值范围和操作数的数据类型有关。当操作数为16位时，数组长度为2，mask[0]、mask[1]∈[0, 264-1]并且不同时为0；当操作数为32位时，数组长度为1，mask[0]∈(0, 264-1]；当操作数为64位时，数组长度为1，mask[0]∈(0, 232-1]。<br><br>例如，mask=[8, 0]，8=0b1000，表示仅第4个元素参与计算。<br><br>连续模式：表示前面连续的多少个元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，mask∈[1, 128]；当操作数为32位时，mask∈[1, 64]；当操作数为64位时，mask∈[1, 32]。 |
| repeatTime | 输入 | 迭代次数。取值范围为[0, 255]。<br><br>关于该参数的具体描述请参考[高维切分API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| dstRepStride | 输入 | 目的操作数相邻迭代间的地址步长。以一个repeat归约后的长度为单位。<br><br>单位为dst数据类型所占字节长度。比如当dst为half时，单位为2Bytes。<br><br>注意，此参数值Atlas 训练系列产品不支持配置0。 |
| srcBlkStride | 输入 | 单次迭代内datablock的地址步长。详细说明请参考[dataBlockStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的DataBlock数目。详细说明请参考[repeatStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |

## 返回值说明

无

## 约束说明

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../通用说明和约束.md#section668772811100)。

- 对于WholeReduceSum，其内部的相加方式采用二叉树方式，两两相加：

    假设源操作数为128个half类型的数据\[data0,data1,data2...data127\]，一个repeat可以计算完，计算过程如下。

    1.  data0和data1相加得到data00，data2和data3相加得到data01...data124和data125相加得到data62，data126和data127相加得到data63；
    2.  data00和data01相加得到data000，data02和data03相加得到data001...data62和data63相加得到data031；
    3.  以此类推，得到目的操作数为1个half类型的数据\[data\]。

    需要注意的是两两相加的计算过程中，计算结果大于65504时结果保存为65504。例如源操作数为\[60000,60000,-30000,100\]，首先60000+60000溢出，结果为65504，第二步计算-30000+100=-29900，第四步计算65504-29900=35604。

## 调用示例

完整样例可参考[WholeReduce系列归约指令样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。

- tensor高维切分计算样例-mask连续模式

    ```cpp
    // dstLocal,srcLocal均为half类型,srcLocal的计算数据量为512，连续排布，计算结果也需要连续排布，使用tensor高维切分计算接口，设定mask为最多的128个全部元素参与计算
    // 根据以上信息，推断出repeatTime为4，dstRepStride为1，srcBlkStride为1，srcRepStride为8
    AscendC::WholeReduceSum<half>(dstLocal, srcLocal, 128, 4, 1, 1, 8);
    ```

- tensor高维切分计算样例-mask逐bit模式

    ```cpp
    // dstLocal,srcLocal均为half类型，srcLocal的计算数据量为512，连续排布，计算结果也需要连续排布，使用tensor高维切分计算接口，设定mask为最多的128个全部元素参与计算
    uint64_t mask[2] = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF };

    // 根据以上信息，推断出repeatTime为4，dstRepStride为1，srcBlkStride为1，srcRepStride为8
    AscendC::WholeReduceSum<half>(dstLocal, srcLocal, mask, 4, 1, 1, 8);
    ```

- 针对不同场景合理使用归约指令可以带来性能提升，相关介绍请参考[选择低延迟指令，优化归约操作性能](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/算子实践参考/SIMD算子性能优化/矢量计算/选择低延迟指令-优化归约操作性能.md)，具体样例请参考[ReduceCustom](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/03_libraries/04_reduce/reduce)。
