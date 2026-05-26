# UB-\>UB连续数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002543262932"></a>

DataCopy能够实现Unified Buffer和Unified Buffer之间连续数据搬运，数据搬运时格式和内容保持不变。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

搬运的数据用于[矢量计算](Memory矢量计算.md#ZH-CN_TOPIC_0000002525764374)，具体的接口请参考：[UB-\>UB连续数据搬运\(DataCopy\)](UB--UB连续数据搬运(DataCopy)-13.md#ZH-CN_TOPIC_0000002574928197)

```
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```

