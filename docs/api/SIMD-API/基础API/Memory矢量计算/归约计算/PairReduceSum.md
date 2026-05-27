# PairReduceSum

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

## 功能说明

PairReduceSum：相邻两个（奇偶）元素求和，例如（a1, a2, a3, a4, a5, a6...），相邻两个数据求和为（a1+a2,  a3+a4,  a5+a6, ......）。归约指令的总体介绍请参考[如何使用归约计算API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/类库API/基础API/常用操作速查指导/如何使用归约计算API.md)。

## 函数原型

- mask逐bit模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void PairReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const uint64_t mask[], const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

- mask连续模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void PairReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const int32_t mask, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。<br><br>Ascend 950PR/Ascend 950DT，支持的数据类型为：half/float<br><br>Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：half/float<br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：half/float<br><br>Atlas 200I/500 A2 推理产品，支持的数据类型为：half/float<br><br>Atlas 推理系列产品AI Core，支持的数据类型为：half/float<br><br>Atlas 训练系列产品，支持的数据类型为：half |
| isSetMask | 是否在接口内部设置mask。<br>true，表示在接口内部设置mask。false，表示在接口外部设置mask，开发者需要使用[SetVectorMask](../掩码操作/SetVectorMask.md)接口设置mask值。这种模式下，接口入参中的mask值设置为占位符MASK_PLACEHOLDER，用于占位，无实际含义。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。 |
| src | 输入 | 源操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。 |
| repeatTime | 输入 | 迭代次数。取值范围为[0, 255]。<br><br>关于该参数的具体描述请参考[高维切分API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| mask/mask[] | 输入 | [mask](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)用于控制每次迭代内参与计算的元素。<br>逐bit模式：可以按位控制哪些元素参与计算，bit位的值为1表示参与计算，0表示不参与。<br>mask为数组形式，数组长度和数组元素的取值范围和操作数的数据类型有关。当操作数为16位时，数组长度为2，mask[0]、mask[1]∈[0, 264-1]并且不同时为0；当操作数为32位时，数组长度为1，mask[0]∈(0, 264-1]；当操作数为64位时，数组长度为1，mask[0]∈(0, 232-1]。<br><br>例如，mask=[8, 0]，8=0b1000，表示仅第4个元素参与计算。<br><br>连续模式：表示前面连续的多少个元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，mask∈[1, 128]；当操作数为32位时，mask∈[1, 64]；当操作数为64位时，mask∈[1, 32]。 |
| dstRepStride | 输入 | 目的操作数相邻迭代间的地址步长。以一个repeat归约后的长度为单位。PairReduce完成后，一个repeat的长度减半。即单位为128Byte。<br><br>注意，此参数值Atlas 训练系列产品不支持配置0。 |
| srcBlkStride | 输入 | 单次迭代内datablock的地址步长。详细说明请参考[dataBlockStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的datablock数目。详细说明请参考[repeatStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |

## 返回值说明

无

## 约束说明

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。
- 如果两两相加的两个元素mask位未配置（即当前两个元素不参与运算），对于Atlas 200I/500 A2 推理产品，对应的目的操作数中的值会置为0，对于其他产品型号，对应的目的操作数中的值不会变化。比如float场景下对64个数使用当前指令，mask配置为62，表示最后两个元素不参与运算，对于Atlas 200I/500 A2 推理产品，目的操作数中最后一个值会返回0；对于其他产品型号，目的操作数中最后一个值不会变化。

## 调用示例

本样例中只展示Compute流程中的部分代码。完整样例可参考[PairReduceSum样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/pair_reduce_sum)。

- PairReduceSum-tensor高维切分计算样例-mask连续模式

    ```cpp
    // 设定mask为最多的128个全部元素参与计算
    int32_t mask = 256/sizeof(half);
    // 每个repeat128个元素，一共128个元素。
    int repeat = 1;
    // dstLocal: 目的操作数tensor
    // srcLocal: 源操作数tensor
    // srcBlkStride = 1, 在一个repeat中，block间没有空隙。
    // dstRepStride = 1, srcRepStride = 8, repeat间没有空隙。
    AscendC::PairReduceSum<half>(dstLocal, srcLocal, repeat, mask, 1, 1, 8);
    ```

- PairReduceSum-tensor高维切分计算样例-mask逐bit模式

    ```cpp
    // 设定mask为最多的128个全部元素参与计算
    uint64_t mask[2] = { UINT64_MAX, UINT64_MAX };
    // 每个repeat128个元素，一共128个元素。
    int repeat = 1;
    // dstLocal: 目的操作数tensor
    // srcLocal: 源操作数tensor
    // srcBlkStride = 1, 在一个repeat中，block间没有空隙。
    // dstRepStride = 1, srcRepStride = 8, repeat间没有空隙。
    AscendC::PairReduceSum<half>(dstLocal, srcLocal, repeat, mask, 1, 1, 8);
    ```

- 示例结果

    ```cpp
    输入数据src_gm：
    [1, 1, 1, -1, 2, 2, -1, 2,
     3, 3, 3, -1, 4, 4, -2, 4,
    ....
    ]
    输出数据dst_gm：
    [2, 0, 4, 1, 6, 2, 8, 2,
    ....
    ]
    ```
