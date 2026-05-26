# GM与L1高维切分数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002543262916"></a>

DataCopy能够实现Global Memory和L1 Buffer之间的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

搬运的数据用于[矩阵计算](矩阵计算.md#ZH-CN_TOPIC_0000002568950889)，接口具体介绍请参考：[GM-\>L1高维切分数据搬运\(DataCopy\)](GM--L1高维切分数据搬运(DataCopy).md#ZH-CN_TOPIC_0000002538231128)

-   GM-\>L1

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyParams& repeatParams)
    ```

-   L1-\>GM

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const  LocalTensor<T>& src, const DataCopyParams& repeatParams)
    ```

