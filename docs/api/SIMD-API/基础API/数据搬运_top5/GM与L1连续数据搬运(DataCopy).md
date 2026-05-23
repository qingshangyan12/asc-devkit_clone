# GM与L1连续数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002574022813"></a>

DataCopy能够实现Global Memory和L1 Buffer之间的连续数据搬运，数据搬运时格式和内容保持不变。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

搬运的数据用于[矩阵计算](矩阵计算.md#ZH-CN_TOPIC_0000002568950889)，接口具体介绍请参考：[GM-\>L1连续数据搬运\(DataCopy\)](GM--L1连续数据搬运(DataCopy).md#ZH-CN_TOPIC_0000002569070903)

-   GM-\>L1

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const uint32_t count)
    ```

-   L1-\>GM

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
    ```

