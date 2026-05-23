# GM-\>L1随路转换-ND2NZ搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002573902841"></a>

该接口主要实现将数据从Global Memory搬运至L1 Buffer，并支持在数据搬运时进行[ND](关键分形格式详解.md#zh-cn_topic_0000002545089965_section958745018719)到[NZ](关键分形格式详解.md#zh-cn_topic_0000002545089965_section363412741113)格式的转换。

src和dst分别为源操作数和目的操作数；intriParams为ND2NZ搬运配置参数。

搬运的数据用于[矩阵计算](矩阵计算.md#ZH-CN_TOPIC_0000002568950889)，接口具体介绍请参考：[GM-\>L1随路转换-ND2NZ搬运\(DataCopy\)](GM--L1随路转换-ND2NZ搬运(DataCopy)-57.md#ZH-CN_TOPIC_0000002569070913)

针对Ascend 950PR/Ascend 950DT：

```
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const Nd2NzParams& intriParams)
```

