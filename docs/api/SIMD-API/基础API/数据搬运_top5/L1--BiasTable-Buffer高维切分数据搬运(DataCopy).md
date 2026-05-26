# L1-\>BiasTable Buffer高维切分数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002543262936"></a>

DataCopy能够实现L1 Buffer至BiasTable Buffer的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

搬运的数据用于矩阵计算，接口具体介绍请参考：[L1到Bias Table Buffer数据搬运](L1到Bias-Table-Buffer数据搬运.md#ZH-CN_TOPIC_0000002538071240)

-   连续或非连续搬运场景（高维切分）

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
    ```

-   连续或非连续搬运场景，支持源操作数和目的操作数数据类型不一致

    ```
    template <typename T, typename U>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<U>& src, const DataCopyParams& repeatParams)
    ```

