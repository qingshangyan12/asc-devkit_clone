# L1ToL0B 稠密权重矩阵搬运(LoadDataWithSparse)<a name="ZH-CN_TOPIC_0000002574022821"></a>

用于从L1中搬运以512Byte为单位存放的稠密权重矩阵到L0B里，同时搬运以128Byte为单位的索引矩阵到内置的专用buffer空间（用于后续MmadWithSparse接口进行读取）。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L1 Buffer -> L0B Buffer
    - B1 -> B2

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：[LoadDataWithSparse](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/LoadDataWithSparse.md)

<cann-filter npu-type="A3,910b">

- 仅如下产品型号支持：

    <cann-filter npu-type="A3">

    Atlas A3 训练系列产品/Atlas A3 推理系列产品；

    </cann-filter>

    <cann-filter npu-type="910b">

    Atlas A2 训练系列产品/Atlas A2 推理系列产品；

    </cann-filter>

    ```cpp
    template <typename T = int8_t, typename U = uint8_t, typename Std::enable_if<Std::is_same<PrimT<T>, int8_t>::value, bool>::type = true, typename Std::enable_if<Std::is_same<PrimT<U>, uint8_t>::value, bool>::type = true>
    __aicore__ inline void LoadDataWithSparse(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<U>& idx, const LoadData2dParams& loadDataParam)
    ```

</cann-filter>
