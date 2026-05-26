# L1-\>Fixpipe Buffer连续数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002543262934"></a>

DataCopy能够实现L1 Buffer至Fixpipe Buffer的连续数据搬运，数据搬运时格式和内容保持不变。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

搬运的数据为[随路量化](随路量化.md#ZH-CN_TOPIC_0000002568950983)和[随路relu](随路relu.md#ZH-CN_TOPIC_0000002538231206)参数，接口具体介绍请参考：[L1到Fixpipe Buffer数据搬运](L1到Fixpipe-Buffer数据搬运.md#ZH-CN_TOPIC_0000002569070951)

```
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count);
```

