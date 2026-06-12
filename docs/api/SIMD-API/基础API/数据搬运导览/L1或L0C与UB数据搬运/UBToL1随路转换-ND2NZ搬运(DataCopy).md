# UBToL1随路转换-ND2NZ搬运(DataCopy)

该接口主要实现将数据从Unified Buffer搬运至L1 Buffer，并支持在数据搬运时进行[ND](../../矩阵计算（ISASI）/矩阵计算分形介绍/关键分形格式详解.md)到[NZ](../../矩阵计算（ISASI）/矩阵计算分形介绍/关键分形格式详解.md)格式的转换。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Unified Buffer -> L1 Buffer
    - UB -> C1

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：[UBToL1随路转换-ND2NZ搬运（DataCopy）](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1随路转换-ND2NZ搬运（DataCopy）.md)

src和dst分别为源操作数和目的操作数；intriParams为ND2NZ搬运配置参数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const Nd2NzParams& intriParams)
```
