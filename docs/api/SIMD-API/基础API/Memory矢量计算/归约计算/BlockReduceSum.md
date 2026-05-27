# BlockReduceSum

## 产品支持情况

| 产品 | 是否支持 |
|------|:------:|
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品 AI Core | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品 Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | √ </cann-filter>|
| <cann-filter npu-type="x90">Kirin X90 | √ </cann-filter>|
| <cann-filter npu-type="9030">Kirin 9030 | √ </cann-filter>|

## 功能说明

对每个datablock内所有元素求和。源操作数相加采用二叉树方式，两两相加。归约指令的总体介绍请参考[如何使用归约计算API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/类库API/基础API/常用操作速查指导/如何使用归约计算API.md)。

以128个half类型的数据求和为例，每个datablock可以计算16个half类型数据，分成8个datablock进行计算；每个datablock内，通过二叉树的方式，两两相加，BlockReduceSum求和示意图如下。

**图 1**  BlockReduceSum求和示意图
![](../../../../figures/BlockReduceSum求和示意图.png "BlockReduceSum求和示意图")

需要注意的是两两相加的计算过程中，计算结果大于65504时结果保存为65504。例如，源操作数为\[60000,60000,-30000,100\]，首先60000+60000溢出，结果为65504，然后计算-30000+100=-29900，最后计算65504-29900=35604，计算示意图如下图所示。

**图 2**  存在溢出场景时的计算示意图
![](../../../../figures/存在溢出场景时的计算示意图.png "存在溢出场景时的计算示意图")

## 函数原型

- mask逐比特模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void BlockReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src,const int32_t repeatTime, const uint64_t mask[], const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

- mask连续模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void BlockReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src,const int32_t repeatTime, const int32_t mask, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
|--------|------|
| T | 操作数数据类型。Ascend 950PR/Ascend 950DT 支持 half/float；Atlas A3 训练系列产品/Atlas A3 推理系列产品 支持 half/float；Atlas A2 训练系列产品/Atlas A2 推理系列产品 支持 half/float；Atlas 200I/500 A2 推理产品 支持 half/float；Atlas 推理系列产品 AI Core 支持 half/float；Atlas 训练系列产品 支持 half；Kirin X90 支持 half/float；Kirin 9030 支持 half/float |
| isSetMask | 是否在接口内部设置mask。true：在接口内部设置mask。false：在接口外部设置mask，开发者需要使用 [SetVectorMask](../掩码操作/SetVectorMask.md) 接口设置mask值，此时接口入参中的mask设置为占位符 MASK_PLACEHOLDER，无实际含义。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
|----------|-----------|------|
| dst | 输出 | 目的操作数。类型为 [LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为 VECIN/VECCALC/VECOUT。LocalTensor起始地址需保证：half类型 16字节对齐，float类型 32字节对齐。 |
| src | 输入 | 源操作数。类型为 [LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为 VECIN/VECCALC/VECOUT。LocalTensor起始地址需32字节对齐。 |
| repeatTime | 输入 | 迭代次数，取值范围 [0, 255]。具体描述请参考 [高维切分API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| mask/mask[] | 输入 | mask 用于控制每次迭代内参与计算的元素。逐bit模式：可按位控制元素参与计算，bit位值为1表示参与。mask为数组形式，数组长度与数据类型有关：16位时数组长度为2，mask[0], mask[1]∈[0, 2^64-1]且不同时为0；32位时数组长度为1，mask[0]∈(0, 2^64-1]；64位时数组长度为1，mask[0]∈(0, 2^32-1]。例如 mask=[8,0]，8=0b1000，表示仅第4个元素参与计算。连续模式：表示前面连续多少个元素参与计算，16位时 mask∈[1,128]；32位时 mask∈[1,64]；64位时 mask∈[1,32]。 |
| dstRepStride | 输入 | 目的操作数相邻迭代间的地址步长，以一个repeatTime归约后的长度为单位。每个repeatTime(8个datablock)归约后得8个元素，half类型时 RepStride单位为16Byte，float类型时为32Byte。注意：Atlas 训练系列产品不支持配置0。 |
| srcBlkStride | 输入 | 单次迭代内datablock的地址步长。详细说明请参考 [dataBlockStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即每次迭代跳过的datablock数目。详细说明请参考 [repeatStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |

## 返回值说明

无

## 约束说明

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。

- 为了节省地址空间，您可以定义一个Tensor，供源操作数与目的操作数同时使用（即地址重叠），需要注意计算后的目的操作数数据不能覆盖未参与计算的源操作数，需要谨慎使用。
- 对于Atlas 200I/500 A2 推理产品，若配置的mask/mask\[\]参数后，存在某个datablock里的任何一个元素都不参与计算，则该datablock内所有元素的和会填充为0返回。比如float场景下，当mask配置为32，即只计算前4个datablock，则后四个datablock内的和会返回0。

## 调用示例

本样例中只展示Compute流程中的部分代码。完整样例可参考[BlockReduce系列归约指令样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum)。

- BlockReduceSum-tensor高维切分计算样例-mask连续模式

    ```cpp
    // 设定mask为最多的128个全部元素参与计算
    int32_t mask = 256/sizeof(half);
    // 每个repeat128个元素，一共128个元素。
    int repeat = 1;
    // dstLocal: 目的操作数tensor
    // srcLocal: 源操作数tensor
    // srcBlkStride = 1, 在一个repeat中，block间没有空隙。
    // dstRepStride = 1, srcRepStride = 8, repeat间没有空隙。
    AscendC::BlockReduceSum<half>(dstLocal, srcLocal, repeat, mask, 1, 1, 8);
    ```

- BlockReduceSum-tensor高维切分计算样例-mask逐bit模式

    ```cpp
    // 设定mask为最多的128个全部元素参与计算
    uint64_t mask[2] = { UINT64_MAX, UINT64_MAX };
    // 每个repeat128个元素，一共128个元素。
    int repeat = 1;
    // dstLocal: 目的操作数tensor
    // srcLocal: 源操作数tensor
    // srcBlkStride = 1, 在一个repeat中，block间没有空隙。
    // dstRepStride = 1, srcRepStride = 8, repeat间没有空隙。
    AscendC::BlockReduceSum<half>(dstLocal, srcLocal, repeat, mask, 1, 1, 8);
    ```

结果示例如下：

```cpp
输入数据src_gm:
[1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2,
 ...
 3, 3, 3, 3, 3, 3, 3, 3,
 3, 3, 3, 3, 3, 3, 3, 3]

输出数据dst_gm:
[16, 32, ..., 48]
```

- 针对不同场景合理使用归约指令可以带来性能提升，相关介绍请参考[选择低延迟指令，优化归约操作性能](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/算子实践参考/SIMD算子性能优化/矢量计算/选择低延迟指令-优化归约操作性能.md)，具体样例请参考[ReduceCustom](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/03_libraries/04_reduce/reduce)。
