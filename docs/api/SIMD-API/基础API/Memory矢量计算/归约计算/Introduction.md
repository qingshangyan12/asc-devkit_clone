# 简介

# 归约计算

**表 1** **接口分类与使用场景**

| 接口类别 | 主要功能 | 输出长度 | 推荐使用场景 | 支持的数据类型 |
| --- | --- | --- | --- | --- |
| ReduceDataBlock | 对每个DataBlock内的数据求和/求最大值/求最小值 | 1 element / DataBlock | 在数据类型为float时推荐使用。在float数据类型时，相比ReduceRepeat，ReduceDataBlock有更高的理论并行度；与ReducePairElem拥有相同的理论并行度，但是可以归约更多数据。在数据类型为half、数据量小于等于一个DataBlock时推荐使用。在half数据类型时ReduceDataBlock和ReduceRepeat具有相同的理论并行度。但ReduceDataBlock支持只对一个DataBlock进行归约，而ReduceRepeat需要对八个DataBlock（一个repeat）进行归约。在数据量小于等于一个DataBlock（32B）时ReduceDataBlock效率更高。如：当数据长度为2K时，可以先进行一次ReduceRepeat归约得到长度为16（32B），再进行一次ReduceDataBlock即可。此时一次ReduceRepeat+一次ReduceDataBlock耗时比两次ReduceRepeat短。 | half、float |
| ReduceRepeat | 对每个repeat内的数据求和/求最大值/求最小值<br><br>其中求最大值或最小值时可选择输出对应的索引 | 求和：1 element / repeat<br><br>求最大值/最小值：1 element (1 index) / repeat | ReduceRepeat能够得到索引，在需要得到索引且对性能敏感的场景下推荐使用。在数据类型为half、数据量大于一个DataBlock时推荐使用。在half数据类型时ReduceDataBlock和ReduceRepeat具有相同的理论并行度。但ReduceRepeat能一次调用对八个DataBlock（一个repeat）进行归约。在数据量大于一个DataBlock（32B）时，使用ReduceDataBlock至少需要调用两次，此时一次ReduceRepeat耗时比两次BlockReduce短。 | half、float |
| ReducePairElem | 对相邻两个元素进行求和 | 1 output element/ 2 input element | 适合在需要对相邻两个元素进行求和操作时使用。在数据类型为half，ReducePairElem的理论并行度是归约指令中最高的，因此在数据量较大需要多次调用ReduceDataBlock及ReduceRepeat时，该接口适合用于对数据进行初步归约。 | half、float |
| ReduceSum | 对所有输入数据求和 | 1 element | 这类接口内部通过软件仿真实现，性能可能不及直接使用硬件指令实现的ReduceDataBlock和ReduceRepeat接口，不推荐使用。<br><br>但该类接口能通过一次调用得到归约结果，且支持更长的输入数据，适合用于对性能不敏感，需要快速验证实现的场景。 | half、float |
| ReduceMax/Min | 对所有输入数据求最大值/最小值<br><br>可选择输出对应的索引 | 1 element (1 index) | 这类接口内部通过软件仿真实现，性能可能不及直接使用硬件指令实现的ReduceDataBlock和ReduceRepeat接口，不推荐使用。<br><br>但该类接口能通过一次调用得到归约结果，且支持更长的输入数据，适合用于对性能不敏感，需要快速验证实现的场景。 | half、float |
| GetReduceRepeatSumSpr | 获取ReduceSum接口的计算结果 | 1 element | 获取tensor前n个数据计算场景的ReduceSum结果 | half、float |
| GetReduceRepeatMaxMinSpr | 获取ReduceMax/ReduceMin接口的结果（最值和索引） | 1 element (1 index) | 获取连续计算场景的ReduceMax/ReduceMin结果 | half、float |

归约接口的选择可以参考如下决策树：

**图 1**  归约接口选择决策树
![](figures/归约接口选择决策树.png "归约接口选择决策树")

注：在数据类型为half时，ReduceDataBlock及ReduceRepeat接口并行度不如ReducePairElem及基础算术接口Add，Max，Min。在数据量较大，需要多次调用ReduceDataBlock及ReduceRepeat时，可以考虑使用ReducePairElem或基础算术接口对数据进行初步归约后再调用归约计算接口。
