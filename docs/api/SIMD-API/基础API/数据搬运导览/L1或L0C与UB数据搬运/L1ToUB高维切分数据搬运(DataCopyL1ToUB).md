# L1ToUB高维切分数据搬运(DataCopyL1ToUB)

DataCopyL1ToUB能够实现L1 Buffer到Unified Buffer的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L1 Buffer -> Unified Buffer
    - C1 -> UB

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：[L1到UB数据搬运（DataCopyL1ToUB）](../../矩阵计算（ISASI）/矩阵计算的搬出/L1到UB数据搬运（DataCopyL1ToUB）.md)

<cann-filter npu-type="950">

该接口仅Ascend 950PR/Ascend 950DT支持。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

```cpp
template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```

</cann-filter>
