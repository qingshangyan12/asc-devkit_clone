# UB -> UB连续数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002382908021"></a>

支持Unified Buffer与Unified Buffer之间的连续数据搬运，数据在传输过程中保持原始格式和内容不变。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Unified Buffer -> Unified Buffer
    - VECIN -> VECCALC
    - VECCALC -> VECOUT

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[UB -> UB连续数据搬运(DataCopy)](../../Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB连续数据搬运(DataCopy).md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```
