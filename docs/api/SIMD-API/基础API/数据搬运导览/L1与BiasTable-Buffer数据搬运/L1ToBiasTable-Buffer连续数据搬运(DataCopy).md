# L1ToBiasTable Buffer连续数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002574022837"></a>

DataCopy能够实现L1 Buffer至BiasTable Buffer的连续数据搬运，数据搬运时格式和内容保持不变。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L1 Buffer -> BiasTable Buffer
    - C1 -> C2

搬运的数据用于矩阵计算，接口具体介绍请参考：[L1ToBiasTable-Buffer数据搬运（DataCopy）](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/L1ToBiasTable-Buffer数据搬运（DataCopy）.md)

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```
