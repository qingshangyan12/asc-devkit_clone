# ReduceSum

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

对所有的输入数据求和。归约指令的总体介绍请参考[如何使用归约计算API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/类库API/基础API/常用操作速查指导/如何使用归约计算API.md)。

ReduceSum的相加方式分为两种：

- 方式一：同一repeat内先按照二叉树累加、不同repeat的结果也按照二叉树累加。

    假设源操作数为128个half类型的数据\[data0,data1,data2...data127\]，一个repeat可以计算完，计算过程如下。

    1.  data0和data1相加得到data00，data2和data3相加得到data01，...，data124和data125相加得到data62，data126和data127相加得到data63；
    2.  data00和data01相加得到data000，data02和data03相加得到data001，...，data62和data63相加得到data031；
    3.  以此类推，得到目的操作数为1个half类型的数据\[data\]。

    需要注意的是两两相加的计算过程中，计算结果大于65504时结果保存为65504。例如源操作数为\[60000,60000,-30000,100\]，首先60000+60000溢出，结果为65504，第二步计算-30000+100=-29900，第四步计算65504-29900=35604。

- 方式二：同一repeat内采用二叉树累加，不同repeat的结果按顺序累加。

不同硬件形态对应的ReduceSum相加方式如下：

Ascend 950PR/Ascend 950DT  采用方式一

Atlas A3 训练系列产品/Atlas A3 推理系列产品tensor前n个数据计算接口采用方式二，tensor高维切分计算接口采用方式一

Atlas A2 训练系列产品/Atlas A2 推理系列产品tensor前n个数据计算接口采用方式二，tensor高维切分计算接口采用方式一

Atlas 200I/500 A2 推理产品采用方式一

Atlas 推理系列产品AI Core采用方式一

Atlas 训练系列产品采用方式一

Kirin X90采用方式一

Kirin 9030采用方式一

## 函数原型

- tensor前n个数据计算

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void ReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<T>& sharedTmpBuffer, const int32_t count)
    ```

- tensor高维切分计算
    - mask逐bit模式

        ```cpp
        template <typename T>
        __aicore__ inline void ReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<T>& sharedTmpBuffer, const uint64_t mask[], const int32_t repeatTime, const int32_t srcRepStride)
        ```

    - mask连续模式

        ```cpp
        template <typename T>
        __aicore__ inline void ReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<T>& sharedTmpBuffer, const int32_t mask, const int32_t repeatTime, const int32_t srcRepStride)
        ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。<br><br>Ascend 950PR/Ascend 950DT，支持的数据类型为：half/float/uint64_t/int64_t<br><br>Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：half/float<br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：half/float<br><br>Atlas 200I/500 A2 推理产品，支持的数据类型为：half/float<br><br>Atlas 推理系列产品AI Core，支持的数据类型为：half/float<br><br>Atlas 训练系列产品，支持的数据类型为：half<br><br>Kirin X90，支持的数据类型为：half/float<br><br>Kirin 9030，支持的数据类型为：half/float |
| isSetMask | 是否在接口内部设置mask，默认值为true。<br>true，表示在接口内部设置mask。false，表示在接口外部设置mask，开发者需要使用[SetVectorMask](../掩码操作/SetVectorMask.md)接口设置mask值。这种模式下，接口入参中的mask值设置为占位符MASK_PLACEHOLDER，用于占位，无实际含义。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要保证2字节对齐（针对half数据类型），4字节对齐（针对float数据类型）。 |
| src | 输入 | 源操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。<br><br>源操作数的数据类型需要与目的操作数保持一致。 |
| sharedTmpBuffer | 输入 | 指令执行期间用于存储中间结果，用于内部计算所需操作空间，具体计算方法可参考下文中[sharedTmpBuffer大小计算的介绍](#li8735184264517)。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。<br><br>数据类型需要与目的操作数保持一致。 |
| count | 输入 | 参与计算的元素个数。<br><br>参数取值范围和操作数的数据类型有关，数据类型不同，能够处理的元素个数最大值不同，最大处理的数据量不能超过UB大小限制。 |
| mask/mask[] | 输入 | [mask](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)用于控制每次迭代内参与计算的元素。<br>逐bit模式：可以按位控制哪些元素参与计算，bit位的值为1表示参与计算，0表示不参与。<br>mask为数组形式，数组长度和数组元素的取值范围和操作数的数据类型有关。当操作数为16位时，数组长度为2，mask[0]、mask[1]∈[0, 264-1]并且不同时为0；当操作数为32位时，数组长度为1，mask[0]∈(0, 264-1]；当操作数为64位时，数组长度为1，mask[0]∈(0, 232-1]。<br><br>例如，mask=[8, 0]，8=0b1000，表示仅第4个元素参与计算。<br><br>连续模式：表示前面连续的多少个元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，mask∈[1, 128]；当操作数为32位时，mask∈[1, 64]；当操作数为64位时，mask∈[1, 32]。 |
| repeatTime | 输入 | 迭代次数。保证不超过int32_t最大值的范围即可。 |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的datablock数目。详细说明请参考[repeatStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |

- 对于功能说明中的两种相加方式，sharedTmpBuffer的计算大小如下所示：
    - 方式一：按照如下计算公式计算最小所需空间：

        ```cpp
        // 先定义一个向上取整函数
        int RoundUp(int a, int b)
        {
            return (a + b - 1) / b;
        }

        // 然后定义参与计算的数据类型
        int typeSize = 2;                           // half类型为2Bytes，float类型为4Bytes，按需填入

        // 再根据数据类型定义两个单位
        int elementsPerBlock = 32 / typeSize;       // 1个datablock存放的元素个数
        int elementsPerRepeat = 256 / typeSize;     // 1次repeat可以处理的元素个数

        // 最后确定首次最大repeat值
        int firstMaxRepeat = repeatTime;           // 此处需要注意：对于tensor高维切分计算接口，firstMaxRepeat就是repeatTime；对于tensor前n个数据计算接口，firstMaxRepeat为count/elementsPerRepeat，比如在half类型下firstMaxRepeat就是count/128，在float类型下为count/64，按需填入，对于count<elementsPerRepeat的场景，firstMaxRepeat就是1

        int iter1OutputCount = firstMaxRepeat;                                              // 第一轮操作产生的元素个数
        int iter1AlignEnd = RoundUp(iter1OutputCount, elementsPerBlock) * elementsPerBlock; // 第一轮产生的元素个数做向上取整
        int finalWorkLocalNeedSize = iter1AlignEnd;                                         // 最终sharedTmpBuffer所需的elements空间大小就是第一轮操作产生元素做向上取整后的结果

        ```

    - 方式二：传入任意大小的sharedTmpBuffer，sharedTmpBuffer的值不会被改变。

## 返回值说明

无

## 约束说明

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../通用说明和约束.md#section668772811100)。需要使用sharedTmpBuffer的情况下，支持dst与sharedTmpBuffer地址重叠（通常情况下dst比sharedTmpBuffer所需的空间要小），此时sharedTmpBuffer必须满足最小所需空间要求，否则不支持地址重叠。

- 针对Ascend 950PR/Ascend 950DT，uint64\_t/int64\_t数据类型仅支持tensor前n个数据计算接口。
- 该接口内部通过软件仿真来实现ReduceSum功能，某些场景下，性能可能不及直接使用硬件指令实现的[BlockReduceSum](BlockReduceSum.md)和[WholeReduceSum](WholeReduceSum.md)接口。针对不同场景合理使用归约指令可以带来性能提升，相关介绍请参考[选择低延迟指令，优化归约操作性能](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/算子实践参考/SIMD算子性能优化/矢量计算/选择低延迟指令-优化归约操作性能.md)，具体样例请参考[ReduceCustom](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/03_libraries/04_reduce/reduce)。

## 调用示例

详细示例请参考[ReduceSum样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation)。

- tensor高维切分计算样例-mask连续模式

    ```cpp
    // dstLocal,srcLocal和sharedTmpBuffer均为half类型,srcLocal的计算数据量为8320,并且连续排布，使用tensor高维切分计算接口，设定repeatTime为65，mask为全部元素参与计算
    int32_t mask = 128;
    AscendC::ReduceSum<half>(dstLocal, srcLocal, sharedTmpBuffer, mask, 65, 8);
    ```

- tensor高维切分计算样例-mask逐bit模式

    ```cpp
    // dstLocal,srcLocal和sharedTmpBuffer均为half类型,srcLocal的计算数据量为8320,并且连续排布，使用tensor高维切分计算接口，设定repeatTime为65，mask为全部元素参与计算
    uint64_t mask[2] = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF };
    AscendC::ReduceSum<half>(dstLocal, srcLocal, sharedTmpBuffer, mask, 65, 8);
    ```

- tensor前n个数据计算样例

    ```cpp
    // dstLocal,srcLocal和sharedTmpBuffer均为half类型,srcLocal的计算数据量为8320,并且连续排布，使用tensor前n个数据计算接口
    AscendC::ReduceSum<half>(dstLocal, srcLocal, sharedTmpBuffer, 8320);
    ```

- tensor高维切分计算接口完整示例:

    ```cpp
    #include "kernel_operator.h"
    int srcDataSize = 8320;
    int dstDataSize = 16;
    int mask = 128;
    int repStride = 8;
    int repeat = srcDataSize / mask; // 这里是65

    // 初始化srcLocal 、dstLocal 、sharedTmpBuffer
    AscendC::LocalTensor<half> srcLocal = inQueueSrc.DeQue<half>();
    AscendC::LocalTensor<half> dstLocal = outQueueDst.AllocTensor<half>();
    AscendC::LocalTensor<half> sharedTmpBuffer = workQueue.AllocTensor<half>();
    // mask为128 一次计算128个元素,65次repeat计算完8320个数
    AscendC::ReduceSum<half>(dstLocal, srcLocal, sharedTmpBuffer, mask, repeat, repStride);
    // 释放Tensor
    outQueueDst.EnQue<half>(dstLocal);
    inQueueSrc.FreeTensor(srcLocal);
    workQueue.FreeTensor(sharedTmpBuffer);
    ```

    示例结果如下：

    ```cpp
    输入数据(src_gm):
    [1. 1. 1. ... 1. 1. 1.]
    输出数据(dst_gm):
    [8320.    0.    0.    0.    0.    0.    0.    0.    0.    0.    0.    0.
        0.    0.    0.    0.]
    ```

- tensor前n个数据计算接口完整示例:

    ```cpp
    #include "kernel_operator.h"

    int srcDataSize = 288;
    // 初始化srcLocal 、dstLocal 、sharedTmpBuffer
    AscendC::LocalTensor<half> srcLocal = inQueueSrc.DeQue<half>();
    AscendC::LocalTensor<half> dstLocal = outQueueDst.AllocTensor<half>();
    AscendC::LocalTensor<half> sharedTmpBuffer = workQueue.AllocTensor<half>();

    // level2接口计算前288个数，计算前288个数的和
    AscendC::ReduceSum<half>(dstLocal, srcLocal, sharedTmpBuffer, srcDataSize);
    // 释放Tensor
    outQueueDst.EnQue<half>(dstLocal);
    inQueueSrc.FreeTensor(srcLocal);
    workQueue.FreeTensor(sharedTmpBuffer);
    ```

    示例结果如下：

    ```cpp
    输入数据(src_gm):
    [1. 1. 1. ... 1. 1. 1.]
    输出数据(dst_gm):
    [288.   0.   0.   0.   0.   0.   0.   0.   0.   0.   0.   0.   0.   0.   0.   0.]
    ```
