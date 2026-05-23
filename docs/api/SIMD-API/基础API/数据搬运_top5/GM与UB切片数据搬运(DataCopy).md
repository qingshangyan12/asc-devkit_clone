# GM与UB切片数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002599489363"></a>

DataCopy能够实现Global Memory和Unified Buffer之间的支持数据的切片搬运，提取多维Tensor数据的子集进行搬运。

src和dst分别为源操作数和目的操作数；srcSliceInfo/dstSliceInfo是目的操作数/源操作数切片信息配置参数；dimValue是操作数维度信息，默认值为1。

搬运的数据用于[矢量计算](Memory矢量计算.md#ZH-CN_TOPIC_0000002525764374)，具体的接口请参考：[GM与UB切片数据搬运\(DataCopy\)](GM与UB切片数据搬运(DataCopy)-9.md#ZH-CN_TOPIC_0000002568770166)

-   GM-\>UB

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[], const uint32_t dimValue = 1)
    ```

-   UB-\>GM

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T> &dst, const LocalTensor<T> &src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[], const uint32_t dimValue = 1)
    ```

