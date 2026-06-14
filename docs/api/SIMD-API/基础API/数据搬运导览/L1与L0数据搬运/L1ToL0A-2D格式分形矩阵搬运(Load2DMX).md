# L1ToL0A 2D格式分形矩阵搬运(Load2DMX)

Load2DMX负责完成矩阵计算所需的左右矩阵数据和对应的左右量化系数矩阵数据的搬运，其中左右矩阵数据以大小为512Byte的数据分形为单位进行搬运，左右量化系数矩阵以32Byte的数据分形为单位进行搬运。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L1 Buffer -> L0A Buffer
    - A1 -> A2

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：接口具体介绍请参考：[Load2DMX](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/Load2DMX.md)

<cann-filter npu-type="950">

src和srcMx为源操作数，dst为目的操作数；loadDataParams和loadMxDataParams为搬运参数。

- Load2DMX接口（仅Ascend 950PR/Ascend 950DT支持）

    ```cpp
    template <typename T, typename U = T>
    __aicore__ inline void LoadData(const LocalTensor<U>& dst, const LocalTensor<T>& src, const LocalTensor<fp8_e8m0_t>& srcMx, const LoadData2DParamsV2& loadDataParams, const LoadData2DMxParams& loadMxDataParams)
    ```

- Load2Dv2MX接口，支持源操作数和目的操作数数据类型不一致（仅Ascend 950PR/Ascend 950DT支持）

    ```cpp
    template <typename T, typename U>
    __aicore__ inline void LoadData(const LocalTensor<U>& dst, const LocalTensor<T>& src0, const LocalTensor<fp8_e8m0_t>& srcMx, const LoadData2DParamsV2& loadDataParams, const LoadData2DMxParams& loadMxDataParams)
    ```

</cann-filter>
