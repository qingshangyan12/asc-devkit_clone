# ReduceDataBlock<a name="ZH-CN_TOPIC_0000002572865484"></a>

## 产品支持情况<a name="zh-cn_topic_0000002599206475_section796754519912"></a>

| 产品 | 是否支持 |
| ---- | -------- |
| <cann-filter npu-type = "950">Ascend 950PR/Ascend 950DT | √</cann-filter> |
| <cann-filter npu-type = "A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √</cann-filter> |
| <cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √</cann-filter> |
| <cann-filter npu-type = "310b">Atlas 200I/500 A2 推理产品 | √</cann-filter> |
| <cann-filter npu-type = "310p">Atlas 推理系列产品AI Core | √</cann-filter> |
| <cann-filter npu-type = "310p">Atlas 推理系列产品Vector Core | x</cann-filter> |
| <cann-filter npu-type = "910">Atlas 训练系列产品 | √</cann-filter> |
| <cann-filter npu-type = "x90">Kirin X90 | √</cann-filter> |
| <cann-filter npu-type = "9030">Kirin 9030 | √</cann-filter> |

## 功能说明<a name="zh-cn_topic_0000002599206475_section106841136114319"></a>

头文件路径为：`"basic_api/kernel_operator_vec_reduce_intf.h"`。

如图1所示，ReduceDataBlock接口对输入数据以DataBlock为单位进行归约操作，根据模板参数reduceType，对每个DataBlock内的数据求和/求最大值/求最小值。

ReduceDataBlock对输入数据进行多次迭代计算，每个迭代会取出8个DataBlock（每个DataBlock数据块内部地址连续，大小为32字节）进行计算，输出结果会连续写入目的地址。

**图 1**  ReduceDataBlock归约示意图<a name="zh-cn_topic_0000002599206475_fig1326013126114"></a>  
![](figures/ReduceDataBlock归约示意图.png "ReduceDataBlock归约示意图")

## 函数原型<a name="zh-cn_topic_0000002599206475_section82039854412"></a>

- mask逐bit模式

    ```cpp
    template <ReduceType reduceType, typename T, typename U, bool isSetMask = true>
    __aicore__ inline void ReduceDataBlock(const LocalTensor<T>& dst, const LocalTensor<U>& src, const uint64_t mask[], const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

- mask连续模式

    ```cpp
    template <ReduceType reduceType, typename T, typename U, bool isSetMask = true>
    __aicore__ inline void ReduceDataBlock(const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t mask, const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

## 参数说明<a name="zh-cn_topic_0000002599206475_section16128134420472"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| ------ | ---- |
| reduceType | 归约操作类型。<br>类型为ReduceType枚举类型，支持：<br>&bull; ReduceType::SUM：求和<br>&bull; ReduceType::MAX：求最大值<br>&bull; ReduceType::MIN：求最小值 |
| T | 目的操作数数据类型。 |
| U | 源操作数数据类型。 |
| isSetMask | 是否在接口内部设置mask。<br>&bull; true，表示在接口内部设置mask。<br>&bull; false，表示在接口外部设置mask，开发者需要使用[SetVectorMask](SetVectorMask.md#ZH-CN_TOPIC_0000002574024783)接口设置mask值。这种模式下，本接口入参中的mask值必须设置为占位符MASK_PLACEHOLDER。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| -------- | -------- | ---- |
| dst | 输出 | 目的操作数。<br>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT（存储位置为Unified Buffer）。<br>LocalTensor的起始地址在half数据类型时需要保证16字节对齐，在float数据类型时需要保证32字节对齐。 |
| src | 输入 | 源操作数。<br>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT（存储位置为Unified Buffer）。<br>LocalTensor的起始地址需要32字节对齐。 |
| mask[]/mask | 输入 | mask用于控制每次迭代内参与计算的源操作数。设置详见[API掩码操作涉及参数](通用说明.md#zh-cn_topic_0000002516385690_table208110588155)。<br>**注：当一个DataBlock内的所有元素对应的mask都为0时，不会有值写入对应目的操作数，保留目的操作数原有数据；当mask = 0时，不会执行归约操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。** |
| repeatTime | 输入 | 迭代次数。取值范围为[0, 255]。<br>**注：repeatTime = 0表示不会执行归约操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。** |
| dstRepStride | 输入 | 目的操作数相邻迭代间的地址步长。以一个repeatTime归约后的长度为单位。取值范围为[0, 2<sup>16</sup>-1]。<br>**注：每个repeatTime归约8个DataBlock，得到8个元素，在half数据类型时，dstRepStride单位为16字节；在float数据类型时，dstRepStride单位为32字节。** |
| srcBlkStride | 输入 | 单次迭代内DataBlock的地址步长，单位为32字节。取值范围为[0, 2<sup>16</sup>-1]。 |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的DataBlock数目。取值范围为[0, 2<sup>16</sup>-1]。 |

**注：以上高维切分相关参数mask，repeatTime，dstRepStride，srcBlkStride，srcRepStride请参考[高维切分](高维切分.md#ZH-CN_TOPIC_0000002556644319)中的介绍。**

## 数据类型<a name="zh-cn_topic_0000002599206475_section4219135304818"></a>

支持的数据类型如下：

- <cann-filter npu-type = "950">Ascend 950PR/Ascend 950DT，支持half、float。</cann-filter>
- <cann-filter npu-type = "A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持half、float。</cann-filter>
- <cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持half、float。</cann-filter>
- <cann-filter npu-type = "310b">Atlas 200I/500 A2 推理产品，支持half、float。</cann-filter>
- <cann-filter npu-type = "310p">Atlas 推理系列产品 AI Core，支持half、float。</cann-filter>
- <cann-filter npu-type = "910">Atlas 训练系列产品，支持half。</cann-filter>
- <cann-filter npu-type = "x90">Kirin X90，支持half、float。</cann-filter>
- <cann-filter npu-type = "9030">Kirin 9030，支持half、float。</cann-filter>

目的操作数与源操作数的数据类型需要保持一致。

## 返回值说明

无

## 关键特性说明<a name="zh-cn_topic_0000002599206475_section1124342410319"></a>

接口以二叉树的形式计算每个DataBlock内的结果。

以half类型的数据求和为例，在每个DataBlock内有16个数，通过二叉树的方式，两两相加，计算过程如下图所示：

1. data1和data2相加得到data01，data3和data4相加得到data02，...，data13和data14相加得到data07，data15和data16相加得到data08；
2. data01和data02相加得到data001，data03和data04相加得到data002，...，data07和data08相加得到data004；
3. 以此类推，得到目的操作数为1个half类型的数据sum。

**图 2**  ReduceDataBlock\<ReduceType::SUM\>求和示意图<a name="zh-cn_topic_0000002599206475_fig1835162020519"></a>  
![](figures/ReduceDataBlock-ReduceType-SUM-求和示意图.png "ReduceDataBlock-ReduceType-SUM-求和示意图")

## 约束说明<a name="zh-cn_topic_0000002599206475_section2045914466492"></a>
<!-- TODO 需要确定哪些约束是公共的，哪些是特有的 -->
- 目的操作数与源操作数的数据类型需要保持一致。
- 操作数地址重叠约束请参考[地址重叠约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section668772811100)。
- dstRepStride的单位取决于操作数数据类型，在half数据类型时，单位为16字节（即半个DataBlock）；在float数据类型时，单位为32字节（即一个完整的DataBlock）。具体使用方法可参考[ReduceDataBlock示例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum)。
- src起始地址的对齐约束请参见[Unified Buffer地址对齐约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section796754519912)；dst起始地址对齐约束与操作数数据类型有关，在half数据类型时需要保证16字节对齐，在float数据类型时需要保证32字节对齐。
- repeatTime = 0表示不会执行归约操作，不会对目的操作数进行写入，该接口将被视为NOP（空操作）。
- 当一个DataBlock内的所有元素对应的mask都为0时，不会有值写入对应目的操作数，保留目的操作数原有数据；当mask = 0时，不会执行归约操作，不会对目的操作数进行写入，该接口将被视为NOP（空操作）。
- dstRepStride，srcBlkStride，srcRepStride取值范围为$[0, 2^{16}-1]$，需要结合UB的实际大小避免出现越界。
- float数据类型只支持寄存器非饱和模式，half数据类型默认是寄存器非饱和模式。寄存器的非饱和/饱和模式具体配置方式**待补充**。
    - 下图说明reduceType取ReduceType::SUM时，在饱和模式下half数据类型的计算过程，源操作数为$[60000,60000,-30000,100]$，首先$60000+60000$溢出，结果为$65504$，然后计算$-30000+100=-29900$，最后计算$65504-29900=35604$。

**图 3**  ReduceDataBlock\<ReduceType::SUM\>饱和模式数值溢出情况处理说明<a name="zh-cn_topic_0000002599206475_fig1310042173"></a>  
![](figures/ReduceDataBlock-ReduceType-SUM-饱和模式数值溢出情况处理说明.png "ReduceDataBlock-ReduceType-SUM-饱和模式数值溢出情况处理说明")

- 对于Atlas 200I/500 A2 推理产品，若配置mask[]/mask参数后，存在某个datablock里的任何一个元素都不参与计算，则该datablock内所有元素的最大值会填充为-inf返回。比如float场景下，当mask配置为32，即只计算前4个datablock，则后四个datablock内的最大值会返回-inf。half场景下会返回-65504。

## 调用示例<a name="zh-cn_topic_0000002599206475_section088124295117"></a>

完整示例可参考[ReduceDataBlock示例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum)。
