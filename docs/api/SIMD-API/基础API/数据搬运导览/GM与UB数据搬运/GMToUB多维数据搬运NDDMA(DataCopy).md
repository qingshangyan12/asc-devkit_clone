# GM -> UB多维数据搬运NDDMA(DataCopy)<a name="ZH-CN_TOPIC_0000002544407954"></a>

多维数据搬运接口，相比于基础数据搬运接口，可更加自由配置搬入的维度信息以及对应的Stride。

具体支持的数据通路为（以[逻辑位置TPosition](../../../../其他数据类型/TPosition.md)表示）：

- Global Memory -> Unified Buffer
    - GM -> VECIN

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[GM -> UB多维数据搬运NDDMA(DataCopy)](../../Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB多维数据搬运NDDMA(DataCopy).md)。

src和dst分别为源操作数和目的操作数；params为NdDmaParams类型的搬运参数，配置多维度搬运信息。

- Global Memory -> Unified Buffer，支持多维度搬运

    ```cpp
    template <typename T, uint8_t dim, const NdDmaConfig& config = kDefaultNdDmaConfig>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const NdDmaParams<T, dim>& params)
    ```

- NDDMA Cache刷新

    ```cpp
    __aicore__ inline void NdDmaDci()
    ```
