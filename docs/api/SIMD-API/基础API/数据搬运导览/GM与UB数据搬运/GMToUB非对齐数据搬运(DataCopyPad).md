# GM -> UB非对齐数据搬运(DataCopyPad)<a name="ZH-CN_TOPIC_0000001894460401"></a>

该接口提供将数据从Global Memory非对齐搬运至Unified Buffer的功能，可以根据开发者的需要自行填充数据。当每个搬运的数据块长度（blockLen）非32字节对齐时，每一个数据块都需要填充数据至32字节对齐。

<cann-filter npu-type="950">特别地，针对Ascend 950PR/Ascend 950DT，支持Compact模式，该模式支持单个数据块非32字节对齐，将所有数据块合并成一个连续数据块，在该数据块右侧填充数据至32字节对齐。</cann-filter>

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Global Memory -> Unified Buffer
    - GM -> VECIN
    - GM -> VECOUT
    - <cann-filter npu-type="950">GM -> VECCALC（仅Ascend 950PR/Ascend 950DT支持）</cann-filter>

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[GM -> UB非对齐数据搬运(DataCopyPad)](../../Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB非对齐数据搬运(DataCopyPad).md)。

src和dst分别为源操作数和目的操作数；dataCopyParams为DataCopyExtParams类型的搬运参数；padParams为DataCopyPadExtParams类型的填充控制参数。

- 不支持配置设置数据搬运模式mode

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
    ```

<cann-filter npu-type="950">

- 支持配置设置数据搬运模式mode（仅Ascend 950PR/Ascend 950DT支持）

    ```cpp
    template <typename T, PaddingMode mode = PaddingMode::Normal>
    __aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
    ```

</cann-filter>
