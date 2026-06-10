# UB -> GM非对齐数据搬运(DataCopyPad)<a name="ZH-CN_TOPIC_0000001894460401"></a>

该接口提供将数据从Unified Buffer非对齐搬运至Global Memory的功能。

对于非32字节对齐的场景，在读取Unified Buffer数据时会填入dummy假数据对齐到32B，搬入Global Memory时会将dummy空数据丢弃，从而实现Unified Buffer到Global Memory的非对齐搬运。

具体支持的数据通路为（以[逻辑位置TPosition](../../../../其他数据类型/TPosition.md)表示）：

- Unified Buffer -> Global Memory
    - VECIN -> GM
    - VECOUT -> GM

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[UB -> GM非对齐数据搬运(DataCopyPad)](../../Memory矢量计算/数据搬运/GM与UB数据搬运/UBToGM非对齐数据搬运(DataCopyPad).md)。

src和dst分别为源操作数和目的操作数；dataCopyParams为DataCopyExtParams类型的搬运参数。

- 不支持配置设置数据搬运模式mode

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams)
    ```

<cann-filter npu-type="950">

- 支持配置设置数据搬运模式mode（仅Ascend 950PR/Ascend 950DT支持）

    ```cpp
    template <typename T, PaddingMode mode = PaddingMode::Normal>
    __aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams)
    ```

</cann-filter>
