# GM -> UB随路转换ND2NZ搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002349187356"></a>

该接口为软仿接口，从易用性角度出发进行设计，支持在从Global Memory到Unified Buffer的数据搬运过程中进行ND到NZ格式的转换。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Global Memory -> Unified Buffer
    - GM -> VECIN

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[GM -> UB随路转换ND2NZ搬运(DataCopy)](../../Memory矢量计算/数据搬运/GM与UB数据搬运/GMToUB随路转换ND2NZ搬运(DataCopy).md)。

src和dst分别为源操作数和目的操作数；intriParams为Nd2NzParams类型的搬运参数，配置ND到NZ格式转换信息。

- 不支持enableSmallC0模式

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const Nd2NzParams& intriParams)
    ```

<cann-filter npu-type="950">

- 支持enableSmallC0模式（仅Ascend 950PR/Ascend 950DT支持）

    ```cpp
    template <typename T, bool enableSmallC0 = false>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const Nd2NzParams& intriParams)
    ```

</cann-filter>
