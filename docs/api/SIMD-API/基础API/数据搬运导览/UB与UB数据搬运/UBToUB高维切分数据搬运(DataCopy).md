# UB -> UB高维切分数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002382908021"></a>

支持Unified Buffer与Unified Buffer之间的高维切分数据搬运，数据在传输过程中保持原始格式和内容不变。

高维切分数据搬运可通过配置数据块个数、单个数据块长度、地址偏移等搬运参数，同时支持非连续和连续的数据搬运。

具体支持的数据通路为（以[逻辑位置TPosition](../../../../其他数据类型/TPosition.md)表示）：

- Unified Buffer -> Unified Buffer
    - VECIN -> VECCALC
    - VECCALC -> VECOUT

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[UB -> UB高维切分数据搬运(DataCopy)](../../Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB高维切分数据搬运(DataCopy).md)。

src和dst分别为源操作数和目的操作数；repeatParams为DataCopyParams类型的搬运参数，通过该参数可配置搬运的数据块大小、个数、间隔等信息，同时支持非连续和连续搬运。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```
