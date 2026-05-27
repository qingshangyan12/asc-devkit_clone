# ReduceRepeat

## 产品支持情况

| 产品 | 是否支持 |
| ---- | -------- |
| <cann-filter npu-type = "950">Ascend 950PR/Ascend 950DT | √</cann-filter> |
| <cann-filter npu-type = "A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √</cann-filter> |
| <cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √</cann-filter> |
| <cann-filter npu-type = "310b">Atlas 200I/500 A2 推理产品 | √</cann-filter> |
| <cann-filter npu-type = "310p">Atlas 推理系列产品 AI Core | √</cann-filter> |
| <cann-filter npu-type = "310p">Atlas 推理系列产品 Vector Core | x</cann-filter> |
| <cann-filter npu-type = "910">Atlas 训练系列产品 | √</cann-filter> |
| <cann-filter npu-type = "x90">Kirin X90 | √</cann-filter> |
| <cann-filter npu-type = "9030">Kirin 9030 | √</cann-filter> |

## 功能说明

头文件路径为：`"basic_api/kernel_operator_vec_reduce_intf.h"`。

ReduceRepeat接口用于对每个repeat内所有数据进行归约操作，根据模板参数reduceType进行求和/求最大值/求最小值操作，结果按顺序写入目标地址。

在求最大值或最小值时可以返回对应的索引，返回的索引值为每个repeat内部索引。支持高维切分计算。

**图 1**  ReduceRepeat计算示意图
![](../../../../figures/ReduceRepeat.png "ReduceRepeat计算示意图")

完整示例请参考：[ReduceRepeat样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。

## 函数原型

- mask逐bit模式

    ```cpp
    template <ReduceType reduceType, typename T, typename U, bool isSetMask = true>
    __aicore__ inline void ReduceRepeat(const LocalTensor<T>& dst, const LocalTensor<U>& src, const uint64_t mask[], const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride, ReduceOrder order = ReduceOrder::ORDER_VALUE_INDEX)
    ```

- mask连续模式

    ```cpp
    template <ReduceType reduceType, typename T, typename U, bool isSetMask = true>
    __aicore__ inline void ReduceRepeat(const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t mask, const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride, ReduceOrder order = ReduceOrder::ORDER_VALUE_INDEX)
    ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| reduceType | 归约操作类型，ReduceType类型，支持SUM、MAX、MIN。<br>enum class ReduceType {<br>    NONE = -1, // 无效类型，当前API不支持该类型<br>    SUM = 0,   // 求和<br>    MAX,       // 求最大值<br>    MIN,       // 求最小值<br>}; |
| T | 目的操作数数据类型。 |
| U | 源操作数数据类型。 |
| isSetMask | 是否在接口内部设置mask。<br>true，表示在接口内部设置mask。false，表示在接口外部设置mask，开发者需要使用[SetVectorMask](SetVectorMask.md#ZH-CN_TOPIC_0000002574024783)接口设置mask值。这种模式下，本接口入参中的mask值必须设置为占位符MASK_PLACEHOLDER。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。<br><br>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>注：LocalTensor的起始地址对齐要求参考[表3](#zh-cn_topic_0000002599326429_table16987516135013)。 |
| src | 输入 | 源操作数。<br><br>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。 |
| mask[]/mask | 输入 | mask用于控制每次迭代内参与计算的元素。详细设置参考[掩码](掩码.md#ZH-CN_TOPIC_0000002573904669)。 |
| repeatTime | 输入 | 重复迭代次数。<br><br>矢量计算单元，每次读取连续的256Bytes数据进行计算，为完成对输入数据的处理，必须通过多次迭代（repeat）才能完成所有数据的读取与计算。repeatTime表示迭代的次数。<br><br>关于该参数的具体描述请参考[高维切分](高维切分.md#ZH-CN_TOPIC_0000002556644319)。 |
| dstRepStride | 输入 | 目的操作数相邻迭代间的地址步长，以一个repeatTime归约后的长度为单位。取值范围为[0, 216-1]。<br><br>注：dstRepStride的单位受order参数的影响，参考[表3](#zh-cn_topic_0000002599326429_table16987516135013)。 |
| srcBlkStride | 输入 | 单次迭代内DataBlock的地址步长，单位为32字节。取值范围为[0, 216-1]。 |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的DataBlock数目。取值范围为[0, 216-1]。 |
| order | 输入 | 在归约操作类型为MAX或MIN时，指定dst中最大值value和索引值index的相对位置以及返回结果行为，参数类型为ReduceOrder枚举类型，默认值为ORDER_VALUE_INDEX。<br><br>ReduceOrder取值如下：<br>ORDER_VALUE_INDEX：表示value位于低半部，返回结果存储顺序为[value, index]。ORDER_INDEX_VALUE：表示index位于低半部，返回结果存储顺序为[index, value]。ORDER_ONLY_VALUE：表示只返回最值，返回结果存储顺序为[value]。ORDER_ONLY_INDEX：表示只返回最值索引，返回结果存储顺序为[index]。<br><br>注：归约操作类型为SUM时，该参数不生效。 |

**注：以上高维切分相关参数mask，repeatTime，dstRepStride，srcBlkStride，srcRepStride请参考[高维切分](高维切分.md#ZH-CN_TOPIC_0000002556644319)中的介绍。**

**表 3**  地址对齐和步长单位

<a name="zh-cn_topic_0000002599326429_table16987516135013"></a>
| 归约操作类型 | 数据类型 | dst地址对齐要求 | dstRepStride单位 |
| --- | --- | --- | --- |
| SUM | half | 2B | 2B |
| SUM | float | 4B | 4B |

| 归约操作类型 | 数据类型 | order | dst地址对齐要求 | dstRepStride单位 |
| --- | --- | --- | --- | --- |
| MAX/MIN | half | ORDER_VALUE_INDEX | 4B | 4B |
| MAX/MIN | half | ORDER_INDEX_VALUE | 4B | 4B |
| MAX/MIN | half | ORDER_ONLY_VALUE | 2B | 2B |
| MAX/MIN | half | ORDER_ONLY_INDEX | 4B | 4B |
| MAX/MIN | float | ORDER_VALUE_INDEX | 8B | 8B |
| MAX/MIN | float | ORDER_INDEX_VALUE | 8B | 8B |
| MAX/MIN | float | ORDER_ONLY_VALUE | 4B | 4B |
| MAX/MIN | float | ORDER_ONLY_INDEX | 4B | 4B |

## 数据类型

操作数支持的数据类型为：half/float。

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品  ，目的操作数与源操作数的数据类型需要保持一致。

## 约束说明

**归约操作类型取SUM/MAX/MIN时的约束**：

- 操作数地址重叠约束请参考[地址重叠约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section668772811100)。

- 源操作数地址对齐约束请参考[地址对齐约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section796754519912)，dst的起始地址对齐要求请参考[表3](#zh-cn_topic_0000002599326429_table16987516135013)。
- repeatTime = 0表示不会执行归约操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。
- dstRepStride, srcBlkStride, srcRepStride取值范围为\[0, 2<sup>16</sup>-1\]，需要结合UB的实际大小避免出现越界。
- dstRepStride的单位受order参数的影响，请参考[表3](#zh-cn_topic_0000002599326429_table16987516135013)。
- 针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品  ，目的操作数与源操作数的数据类型需要保持一致。

**归约操作类型取MAX/MIN时的约束：**

- 如果存在多个最值，该指令会将最小索引写入目的操作数。
- 索引按操作数的数据类型存储，读取索引需要将类型转换到整型。

## 关键特性说明

- **索引值需要强制类型转换**：

    dst的最值索引按照最值的数据类型存储，比如dst为half类型时，索引按照half类型存储，因此读取索引需要使用reinterpret\_cast方法转换到整数类型。若最值数据类型是half，需要使用reinterpret\_cast<uint16\_t\*\>；若最值数据类型是float，需要使用reinterpret\_cast<uint32\_t\*\>。特别地，ORDER\_ONLY\_INDEX（仅返回最值索引）情况下，读取索引值index时都需要使用reinterpret\_cast<uint32\_t\*\>。

    例如，输入数据是half类型，计算结果为\[0.9985,  6.8e-06\]，6.8e-06需要使用reinterpret\_cast<uint16\_t\*\>方法转换得到索引值114。转换示例如下：

    ```cpp
    half maxIndex = dst.GetValue(1);
    uint16_t realIndex = *reinterpret_cast<uint16_t*>(&maxIndex);
    ```

- 归约操作类型取SUM时接口以二叉树累加的方式完成每个repeat内的求和，详情请参考[ReduceDataBlock关键特性说明](ReduceDataBlock.md#zh-cn_topic_0000002599206475_section1124342410319)。

## 调用示例

完整示例可参考[ReduceRepeat样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。
