# UB -> UB连续数据搬运(Copy)<a name="ZH-CN_TOPIC_0000002575088175"></a>

支持Unified Buffer和Unified Buffer之间的连续数据搬运，数据搬运时格式和内容保持不变。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Unified Buffer -> Unified Buffer
    - VECIN -> VECCALC
    - VECIN -> VECOUT
    - VECCALC -> VECIN
    - VECCALC -> VECOUT
    - VECOUT -> VECIN
    - VECOUT -> VECCALC

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[UB -> UB连续数据搬运(Copy)](../../Memory矢量计算/数据搬运/UB与UB数据搬运/UBToUB连续数据搬运(Copy).md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T, bool isSetMask = true>
__aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```
