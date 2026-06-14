# GMToL1 NZ数据搬运(Load2D)<a name="ZH-CN_TOPIC_0000002543422574"></a>

该接口主要实现将数据从Global Memory搬运至L1 Buffer，负责完成普通矩阵计算所需的2D格式数据的搬运，以大小为512Byte的数据分形为单位进行搬运。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Global Memory -> L1 Buffer
    - GM -> A1/B1

搬运的数据用于[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)，接口具体介绍请参考：[GMToL1-Load2D指令搬运](../../矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/GMToL1-Load2D指令搬运.md)

src和dst分别为源操作数和目的操作数；loadDataParams为搬运参数。

```cpp
template <typename T>
__aicore__ inline void LoadData(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParams& loadDataParams)
```
