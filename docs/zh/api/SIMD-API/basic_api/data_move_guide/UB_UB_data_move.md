# UB_UB_data_move<a name="ZH-CN_TOPIC_0000001499154365"></a>

## overall_description<a name="ZH-CN_TOPIC_0000002574022829"></a>

Unified Buffer（UB）之间的data_move提供了灵活的接口支持，共有两组接口：DataCopy提供了基础的data_move能力，支持连续和非连续data_move；Copy除了支持连续和非连续data_move，还提供了mask操作和DataBlock间隔操作的能力，支持在接口内部和接口外部设置mask。具体接口功能如下表所示。

**表 1**  UB_UB_data_move接口overview

<a name="zh-cn_topic_0000002568294885_table12546123753110"></a>

| data_path | 功能 | 描述 |
| :---: | :--- | :--- |
| UB->UB | 连续data_move(DataCopy) | 提供基础的data_move能力，数据在传输过程中保持原始格式和内容不变，支持连续data_move。 |
| UB->UB | high_dim_splitdata_move(DataCopy) | 提供基础的data_move能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的data_move。 |
| UB->UB | 连续data_move(Copy) | 支持连续data_move，data_move时格式和内容保持不变。 |
| UB->UB | mask式高维data_move(Copy) | 支持mask操作和DataBlock间隔操作，支持在接口内部和接口外部设置mask。 |

## DataCopy（UB -> UB连续data_move）<a name="ZH-CN_TOPIC_0000002382907025"></a>

支持Unified Buffer与Unified Buffer之间的连续data_move，数据在传输过程中保持原始格式和内容不变。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- Unified Buffer -> Unified Buffer
    - VECIN -> VECCALC
    - VECCALC -> VECOUT

搬运的数据用于[矢量计算](../memory_vector_compute/memory_vector_compute.md)，具体的接口请参考：[DataCopy（UB -> UB连续data_move）](../memory_vector_compute/data_move/DataCopy_UBToUB_continuous.md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```

## DataCopy（UB -> UBhigh_dim_splitdata_move）<a name="ZH-CN_TOPIC_0000002382908321"></a>

支持Unified Buffer与Unified Buffer之间的high_dim_splitdata_move，数据在传输过程中保持原始格式和内容不变。

high_dim_splitdata_move可通过配置数据块个数、单个数据块长度、地址偏移等搬运参数，同时支持非连续和连续的data_move。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- Unified Buffer -> Unified Buffer
    - VECIN -> VECCALC
    - VECCALC -> VECOUT

搬运的数据用于[矢量计算](../memory_vector_compute/memory_vector_compute.md)，具体的接口请参考：[DataCopy（UB -> UBhigh_dim_splitdata_move）](../memory_vector_compute/data_move/DataCopy_UBToUB_highdim_split.md)。

src和dst分别为源操作数和目的操作数；repeatParams为DataCopyParams类型的搬运参数，通过该参数可配置搬运的数据块大小、个数、间隔等信息，同时支持非连续和连续搬运。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```

## Copy（UB -> UB连续data_move）<a name="ZH-CN_TOPIC_0000002575088175"></a>

支持Unified Buffer和Unified Buffer之间的连续data_move，data_move时格式和内容保持不变。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- Unified Buffer -> Unified Buffer
    - VECIN -> VECCALC
    - VECIN -> VECOUT
    - VECCALC -> VECIN
    - VECCALC -> VECOUT
    - VECOUT -> VECIN
    - VECOUT -> VECCALC

搬运的数据用于[矢量计算](../memory_vector_compute/memory_vector_compute.md)，具体的接口请参考：[Copy（UB -> UB连续data_move）](../memory_vector_compute/data_move/Copy_UBToUB_continuous.md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T, bool isSetMask = true>
__aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```

## Copy（UB -> UBmask式高维data_move）<a name="ZH-CN_TOPIC_0000002575088676"></a>

支持Unified Buffer和Unified Buffer之间的data_move，data_move时格式和内容保持不变，支持mask操作和DataBlock间隔操作。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- Unified Buffer -> Unified Buffer
    - VECIN -> VECCALC
    - VECIN -> VECOUT
    - VECCALC -> VECIN
    - VECCALC -> VECOUT
    - VECOUT -> VECIN
    - VECOUT -> VECCALC

搬运的数据用于[矢量计算](../memory_vector_compute/memory_vector_compute.md)，具体的接口请参考：[Copy（UB -> UBmask式高维data_move）](../memory_vector_compute/data_move/Copy_UBToUB_mask_highdim_split.md)。

src和dst分别为源操作数和目的操作数；mask用于控制每次迭代内参与计算的元素；repeatTime为重复迭代次数；repeatParams为CopyRepeatParams类型的搬运参数。

- mask逐bit模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask[], const uint8_t repeatTime, const CopyRepeatParams& repeatParams)
    ```

- mask连续模式

    ```cpp
    template <typename T, bool isSetMask = true>
    __aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask, const uint8_t repeatTime, const CopyRepeatParams& repeatParams)
    ```
