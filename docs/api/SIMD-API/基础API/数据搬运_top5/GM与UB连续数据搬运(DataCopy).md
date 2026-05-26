# GM与UB连续数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002574022807"></a>

DataCopy能够实现Global Memory和Unified Buffer之间的连续数据搬运，数据搬运时格式和内容保持不变。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

搬运的数据用于[矢量计算](Memory矢量计算.md#ZH-CN_TOPIC_0000002525764374)，具体的接口请参考：[GM与UB连续数据搬运\(DataCopy\)](GM与UB连续数据搬运(DataCopy)-7.md#ZH-CN_TOPIC_0000002544407902)

-   GM-\>UB

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const uint32_t count)
    ```

-   UB-\>GM

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
    ```

