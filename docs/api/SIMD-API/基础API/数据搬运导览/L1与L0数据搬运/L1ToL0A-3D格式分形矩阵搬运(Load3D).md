# L1ToL0A 3D格式分形矩阵搬运(Load3D)<a name="ZH-CN_TOPIC_0000002574022819"></a>

Load3D本质上是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L1 Buffer -> L0A Buffer
    - A1 -> A2

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：[Load3D](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/Load3D.md)

src和dst分别为源操作数和目的操作数；loadDataParams为搬运参数。

- Load3Dv1接口

    ```cpp
    template <typename T, const IsResetLoad3dConfig &defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = PrimT<T>, typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
    __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV1<U>& loadDataParams)
    ```

- Load3Dv2接口

    ```cpp
    template <typename T, const IsResetLoad3dConfig& defaultConfig = IS_RESER_LOAD3D_DEFAULT_CONFIG, typename U = PrimT<T>, typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
    __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2<U>& loadDataParams)
    ```

<cann-filter npu-type="950">

- Load3Dv2Pro接口（仅Ascend 950PR/Ascend 950DT支持）

    ```cpp
    template <typename T>
    __aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData3DParamsV2Pro& loadDataParams)
    ```

</cann-filter>
