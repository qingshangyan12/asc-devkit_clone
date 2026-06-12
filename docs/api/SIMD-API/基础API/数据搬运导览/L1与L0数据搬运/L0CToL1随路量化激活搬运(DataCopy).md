# L0CToL1随路量化激活搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002543262924"></a>

该接口主要实现将数据从L0C Buffer搬运至L1 Buffer，并支持多种随路能力的组合，需要设置不同的寄存器。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L0C Buffer -> L1 Buffer
    - CO1 -> C1

搬运的数据为[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)的结果，接口具体介绍请参考：[L0C到L1数据搬运（DataCopy）](../../矩阵计算（ISASI）/矩阵计算的搬出/L0C到L1数据搬运（DataCopy）.md)

src和dst分别为源操作数和目的操作数；intriParams为搬运参数。

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<U>& src, const DataCopyCO12DstParams& intriParams)
```
