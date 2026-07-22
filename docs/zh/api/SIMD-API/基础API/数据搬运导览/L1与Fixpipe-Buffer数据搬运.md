# L1与Fixpipe Buffer数据搬运

## DataCopy（L1ToFixpipe Buffer连续数据搬运）<a name="ZH-CN_TOPIC_0000002543262934"></a>

DataCopy能够实现L1 Buffer至Fixpipe Buffer的连续数据搬运，数据搬运时格式和内容保持不变。

具体支持的数据通路为（以[逻辑位置TPosition](../../基础API/数据结构/辅助数据结构/TPosition.md)表示）：

- L1 Buffer -> Fixpipe Buffer
    - C1 -> C2PIPE2GM

搬运的数据为[随路量化](../%E7%9F%A9%E9%98%B5%E8%AE%A1%E7%AE%97%EF%BC%88ISASI%EF%BC%89/%E7%9F%A9%E9%98%B5%E6%90%AC%E5%87%BA%E5%85%B3%E9%94%AE%E7%89%B9%E6%80%A7%E8%AF%B4%E6%98%8E/%E9%9A%8F%E8%B7%AF%E9%87%8F%E5%8C%96.md)和[随路relu](../%E7%9F%A9%E9%98%B5%E8%AE%A1%E7%AE%97%EF%BC%88ISASI%EF%BC%89/%E7%9F%A9%E9%98%B5%E6%90%AC%E5%87%BA%E5%85%B3%E9%94%AE%E7%89%B9%E6%80%A7%E8%AF%B4%E6%98%8E/%E9%9A%8F%E8%B7%AFReLU.md)参数，接口具体介绍请参考：[DataCopy（L1ToFixpipe-Buffer数据搬运）](../矩阵计算（ISASI）/矩阵计算的搬入/DataCopy（L1ToFixpipe-Buffer数据搬运）.md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count);
```

## DataCopy（L1ToFixpipe Buffer高维切分数据搬运）<a name="ZH-CN_TOPIC_0000002573902863"></a>

DataCopy能够实现L1 Buffer至Fixpipe Buffer的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的数据通路为（以[逻辑位置TPosition](../../基础API/数据结构/辅助数据结构/TPosition.md)表示）：

- L1 Buffer -> Fixpipe Buffer
    - C1 -> C2PIPE2GM

搬运的数据为[随路量化](../%E7%9F%A9%E9%98%B5%E8%AE%A1%E7%AE%97%EF%BC%88ISASI%EF%BC%89/%E7%9F%A9%E9%98%B5%E6%90%AC%E5%87%BA%E5%85%B3%E9%94%AE%E7%89%B9%E6%80%A7%E8%AF%B4%E6%98%8E/%E9%9A%8F%E8%B7%AF%E9%87%8F%E5%8C%96.md)和[随路relu](../%E7%9F%A9%E9%98%B5%E8%AE%A1%E7%AE%97%EF%BC%88ISASI%EF%BC%89/%E7%9F%A9%E9%98%B5%E6%90%AC%E5%87%BA%E5%85%B3%E9%94%AE%E7%89%B9%E6%80%A7%E8%AF%B4%E6%98%8E/%E9%9A%8F%E8%B7%AFReLU.md)参数，接口具体介绍请参考：[DataCopy（L1ToFixpipe-Buffer数据搬运）](../矩阵计算（ISASI）/矩阵计算的搬入/DataCopy（L1ToFixpipe-Buffer数据搬运）.md)。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```
