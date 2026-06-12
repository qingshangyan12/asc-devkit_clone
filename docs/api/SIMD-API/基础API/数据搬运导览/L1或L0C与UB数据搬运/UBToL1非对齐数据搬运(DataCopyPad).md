# UBToL1非对齐数据搬运(DataCopyPad)

该接口主要实现将数据从Unified Buffer搬运至L1 Buffer，并支持在数据搬运时提供非对齐数据搬运功能，可以根据开发者的需要自行填充数据。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Unified Buffer -> L1 Buffer
    - UB -> C1

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：[UBToL1非对齐数据搬运（DataCopyPad）](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1非对齐数据搬运（DataCopyPad）.md)

src和dst分别为源操作数和目的操作数；dataCopyParams和nd2nzParams为搬运参数。

```cpp
template <typename T>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
```
