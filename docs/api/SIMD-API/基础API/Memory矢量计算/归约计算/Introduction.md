# 概述

# 归约计算

**表 1** 接口分类与主要功能

| 接口类别 | 主要功能 | 输出长度 |
| --- | --- | --- |
| [ReduceDataBlock](./ReduceDataBlock.md) | 对每个DataBlock内的数据求和/求最大值/求最小值。 | 1 element / DataBlock |
| [ReduceRepeat](./ReduceRepeat.md) | 对每个repeat内的数据求和/求最大值/求最小值。<br>其中求最大值或最小值时可选择输出对应的索引。 | 求和：1 element / repeat；<br>求最大值/最小值：1 element (1 index) / repeat。 |
| [ReducePairElem](./ReducePairElem.md) | 对相邻两个元素进行求和。 | 1 output element/ 2 input element |
| [ReduceSum](./ReduceSum.md) | 对所有输入数据求和。 | 1 element |
| [ReduceMax](./ReduceMax.md)/[ReduceMin](./ReduceMin.md) | 对所有输入数据求最大值/最小值。<br>可选择输出对应的索引。 | 1 element (1 index) |
| [GetReduceRepeatSumSpr(ISASI)](./寄存器辅助接口/GetReduceRepeatSumSpr(ISASI).md) | 获取`ReduceSum`接口的计算结果。 | 1 element |
| [GetReduceRepeatMaxMinSpr(ISASI)](./寄存器辅助接口/GetReduceRepeatMaxMinSpr(ISASI).md) | 获取`ReduceMax`/`ReduceMin`接口的结果（最值和索引）。 | 1 element (1 index) |

<cann-filter npu-type = "A3,910b">

<cann-filter npu-type = "A3">针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、</cann-filter><cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品</cann-filter>，在使用归约计算接口时可以参考[表 2](#tab2)选择合适的接口。

<a id="tab2"></a>

**表 2** <cann-filter npu-type = "A3">针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、</cann-filter><cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品</cann-filter>归约计算接口推荐使用场景

| 接口类别 | 推荐使用场景 | 支持的数据类型 |
| --- | --- | --- |
| `ReduceDataBlock` | 在数据类型为`float`时推荐使用。在`float`数据类型时，相比`ReduceRepeat`，`ReduceDataBlock`有更高的理论并行度；与`ReducePairElem`拥有相同的理论并行度，但是可以归约更多数据。在数据类型为`half`、数据量小于等于一个DataBlock时推荐使用。在`half`数据类型时`ReduceDataBlock`和`ReduceRepeat`具有相同的理论并行度。但`ReduceDataBlock`支持只对一个DataBlock进行归约，而`ReduceRepeat`需要对八个DataBlock（一个repeat）进行归约。在数据量小于等于一个DataBlock（32B）时`ReduceDataBlock`效率更高。如：当数据长度为2K时，可以先进行一次`ReduceRepeat`归约得到长度为16（32B），再进行一次`ReduceDataBlock`即可。此时一次`ReduceRepeat`+一次`ReduceDataBlock`耗时比两次`ReduceRepeat`短。 | `half`、`float` |
| `ReduceRepeat` | `ReduceRepeat`能够得到索引，在需要得到索引且对性能敏感的场景下推荐使用。在数据类型为`half`、数据量大于一个DataBlock时推荐使用。在`half`数据类型时`ReduceDataBlock`和`ReduceRepeat`具有相同的理论并行度。但`ReduceRepeat`能一次调用对八个DataBlock（一个repeat）进行归约。在数据量大于一个DataBlock（32B）时，使用`ReduceDataBlock`至少需要调用两次，此时一次`ReduceRepeat`耗时比两次`ReduceDataBlock`短。 | `half`、`float` |
| `ReducePairElem` | 适合在需要对相邻两个元素进行求和操作时使用。在数据类型为`half`，`ReducePairElem`的理论并行度是归约指令中最高的，因此在数据量较大需要多次调用`ReduceDataBlock`及`ReduceRepeat`时，该接口适合用于对数据进行初步归约。 | `half`、`float` |
| `ReduceSum` | 这类接口内部通过软件仿真实现，性能可能不及直接使用硬件指令实现的`ReduceDataBlock`和`ReduceRepeat`接口，不推荐使用。<br>但该类接口能通过一次调用得到归约结果，且支持更长的输入数据，适合用于对性能不敏感，需要快速验证实现的场景。 | `half`、`float` |
| `ReduceMax`/`ReduceMin` | 这类接口内部通过软件仿真实现，性能可能不及直接使用硬件指令实现的`ReduceDataBlock`和`ReduceRepeat`接口，不推荐使用。<br>但该类接口能通过一次调用得到归约结果，且支持更长的输入数据，适合用于对性能不敏感，需要快速验证实现的场景。 | `half`、`float` |
| `GetReduceRepeatSumSpr` | 获取tensor前n个数据计算场景的`ReduceSum`结果。 | `half`、`float` |
| `GetReduceRepeatMaxMinSpr` | 获取连续计算场景的`ReduceMax`/`ReduceMin`结果。 | `half`、`float` |

归约接口的选择可以参考如下[决策树](#fig1)：

<a id="fig1"></a>

**图 1**  归约接口选择决策树
![归约接口选择决策树](../../../../figures/reduce_decision_tree.png "归约接口选择决策树")

注：在数据类型为`half`时，`ReduceDataBlock`及`ReduceRepeat`接口并行度不如`ReducePairElem`及基础算术接口[Add](../基础算术/Add.md)，[Max](../基础算术/Max.md)，[Min](../基础算术/Min.md)。在数据量较大，需要多次调用`ReduceDataBlock`及`ReduceRepeat`时，可以考虑使用`ReducePairElem`或基础算术接口对数据进行初步归约后再调用归约计算接口。

</cann-filter>
