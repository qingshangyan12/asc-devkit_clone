# GMToL1随路转换-DN2NZ搬运(DataCopy)

该接口主要实现将数据从Global Memory搬运至L1 Buffer，并支持在数据搬运时进行[DN](../../矩阵计算（ISASI）/矩阵计算分形介绍/关键分形格式详解.md)到[NZ](../../矩阵计算（ISASI）/矩阵计算分形介绍/关键分形格式详解.md)格式的转换。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Global Memory -> L1 Buffer
    - GM -> A1/B1

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：[GMToL1随路转换-DN2NZ搬运（DataCopy）](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1随路转换-DN2NZ搬运（DataCopy）.md)

<cann-filter npu-type="950">

src和dst分别为源操作数和目的操作数；intriParams为DN2NZ搬运配置参数。

- 支持enableSmallC0模式（仅Ascend 950PR/Ascend 950DT支持）

```cpp
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const Dn2NzParams& intriParams);
```

</cann-filter>
