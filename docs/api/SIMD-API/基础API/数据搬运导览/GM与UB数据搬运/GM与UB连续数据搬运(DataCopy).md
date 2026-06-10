# GM与UB连续数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002382908021"></a>

支持Global Memory与Unified Buffer之间的连续数据搬运，数据在传输过程中保持原始格式和内容不变。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Global Memory -> Unified Buffer
    - GM -> VECIN
- Unified Buffer -> Global Memory
    - VECOUT -> GM
    - <cann-filter npu-type="310p">CO2 -> GM（仅Atlas 推理系列产品AI Core支持）</cann-filter>

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[GM与UB连续数据搬运(DataCopy)](../../Memory矢量计算/数据搬运/GM与UB数据搬运/GM与UB连续数据搬运(DataCopy).md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

- Global Memory -> Unified Buffer

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const uint32_t count)
    ```

- Unified Buffer -> Global Memory

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
    ```
