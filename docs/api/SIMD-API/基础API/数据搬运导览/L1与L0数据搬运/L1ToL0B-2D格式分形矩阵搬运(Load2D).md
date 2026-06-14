# L1ToL0B 2D格式分形矩阵搬运(Load2D)<a name="ZH-CN_TOPIC_0000002543262922"></a>

Load2D能够实现L1 Buffer到L0B Buffer之间的数据搬运。负责完成普通矩阵计算所需的2D格式数据的搬运，以大小为512Byte的数据分形为单位进行搬运。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L1 Buffer -> L0B Buffer
    - B1 -> B2

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：接口具体介绍请参考：[Load2D](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L0-Buffer/Load2D.md)

src和dst分别为源操作数和目的操作数；loadDataParams为搬运参数。

```cpp
template <typename T>
__aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LoadData2DParams& loadDataParams)
```
