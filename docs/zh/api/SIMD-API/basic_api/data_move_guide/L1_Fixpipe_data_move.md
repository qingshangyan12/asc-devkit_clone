# L1与Fixpipe Bufferdata_move

## DataCopy（L1ToFixpipe Buffer连续data_move）<a name="ZH-CN_TOPIC_0000002543262934"></a>

DataCopy能够实现L1 Buffer至Fixpipe Buffer的连续data_move，data_move时格式和内容保持不变。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- L1 Buffer -> Fixpipe Buffer
    - C1 -> C2PIPE2GM

搬运的数据为[accompanying_quantization](../cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md)和[随路relu](../cube_compute_ISASI/cube_store_key_features/accompanying_relu.md)参数，接口具体介绍请参考：[DataCopy（L1ToFixpipe-Bufferdata_move）](../cube_compute_ISASI/cube_compute_load/DataCopy_L1ToFixpipe.md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count);
```

## DataCopy（L1ToFixpipe Bufferhigh_dim_splitdata_move）<a name="ZH-CN_TOPIC_0000002573902863"></a>

DataCopy能够实现L1 Buffer至Fixpipe Buffer的连续data_move和非连续data_move，data_move时格式和内容保持不变。

high_dim_split是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- L1 Buffer -> Fixpipe Buffer
    - C1 -> C2PIPE2GM

搬运的数据为[accompanying_quantization](../cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md)和[随路relu](../cube_compute_ISASI/cube_store_key_features/accompanying_relu.md)参数，接口具体介绍请参考：[DataCopy（L1ToFixpipe-Bufferdata_move）](../cube_compute_ISASI/cube_compute_load/DataCopy_L1ToFixpipe.md)。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```
