# GM与L1连续数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002574022813"></a>

DataCopy能够实现Global Memory和L1 Buffer之间的连续数据搬运，数据搬运时格式和内容保持不变。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Global Memory -> L1 Buffer
    - GM -> A1/B1
- L1 Buffer -> Global Memory
    - A1/B1 -> GM

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：[GMToL1连续数据搬运（DataCopy）](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1连续数据搬运（DataCopy）.md)

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

- Global Memory -> L1 Buffer

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const uint32_t count)
    ```

- L1 Buffer -> Global Memory

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
    ```
