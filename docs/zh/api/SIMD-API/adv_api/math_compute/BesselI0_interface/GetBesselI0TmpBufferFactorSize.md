# GetBesselI0TmpBufferFactorSize

## 功能说明

该接口用于获取maxLiveNodeCount和extraBuf，在固定空间大小的情况下，通过maxLiveNodeCount和extraBuf可以推算算子单次最大计算元素数量。maxLiveNodeCount表示临时空间是单次计算数据量所占空间的多少倍；extraBuf表示使用的额外临时空间大小。

BesselI0（RegBase 实现）不需要任何临时空间，本接口恒返回 maxLiveNodeCount = 0、extraBuffer = 0，调用方无需为其预留临时空间。

## 函数原型

```cpp
void GetBesselI0TmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuffer)
```

## 参数说明

**表1**  参数列表

| 参数名 | 输入/输出 | 功能 |
| --- | --- | --- |
| typeSize | 输入 | 输入的数据类型大小，单位为字节。比如输入的数据类型为float，此处应传入4。本实现未使用该参数。 |
| maxLiveNodeCount | 输出 | 最大存活节点数，表示临时空间是单次计算数据量所占空间的多少倍。本实现恒为 0。 |
| extraBuffer | 输出 | 使用的额外临时空间大小，单位为字节。本实现恒为 0。 |

## 返回值说明

无

## 约束说明

本接口恒返回 maxLiveNodeCount = 0，调用方使用其反推单次最大计算元素数量时需判断该值非 0，避免除零错误；对 BesselI0 而言该推算无意义（无需临时空间）。

## 调用示例

完整的调用样例请参考[更多样例](../more_examples.md)。

```cpp
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuf = 0;
AscendC::GetBesselI0TmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuf);
// maxLiveNodeCount = 0, extraBuf = 0，无需预留临时空间
```
