# L0CToUB随路量化激活搬运(DataCopy)

该接口主要实现将数据从L0C Buffer搬运至Unified Buffer，并支持多种随路能力的组合，接口内包含了设置寄存器与数据搬运能力。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L0C Buffer -> Unified Buffer
    - CO1 -> UB

搬运的数据为[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)的结果，接口具体介绍请参考：[L0C到UB数据搬运（DataCopy）](../../矩阵计算（ISASI）/矩阵计算的搬出/L0C到UB数据搬运（DataCopy）.md)

<cann-filter npu-type="310p">

该接口仅Atlas 推理系列产品AI Core支持。

src和dst分别为源操作数和目的操作数；intriParams和enhancedParams为搬运参数。

- 源操作数和目的操作数类型一致

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
    ```

- 源操作数和目的操作数类型不一致

    ```cpp
    template <typename T, typename U>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<U>& src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
    ```

</cann-filter>
