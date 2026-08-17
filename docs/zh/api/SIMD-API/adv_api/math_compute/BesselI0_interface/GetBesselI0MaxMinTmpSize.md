# GetBesselI0MaxMinTmpSize

## 功能说明

kernel侧BesselI0接口的计算不需要任何临时空间，本接口用于在host侧获取需要预留/申请的最大和最小临时空间大小。本实现恒返回 maxValue = minValue = 0，即"0 表示计算不需要临时空间"，与高阶API家族的既定语义一致。

## 函数原型

```cpp
void GetBesselI0MaxMinTmpSize(const AscendC::TensorShape& srcShape, const uint32_t typeSize, const bool isReuseSource, uint32_t& maxValue, uint32_t& minValue)
```

## 参数说明

**表1**  参数列表

| 参数名 | 输入/输出 | 功能 |
| --- | --- | --- |
| srcShape | 输入 | 输入的shape信息，参数类型为[AscendC::TensorShape](../../data_structures/TensorShape.md)。本实现未使用该参数（结果与 shape 无关），保留以对齐高阶API家族签名。 |
| typeSize | 输入 | 输入的数据类型大小，单位为字节。比如输入的数据类型为float，此处应传入4。本实现未使用该参数。 |
| isReuseSource | 输入 | 是否允许修改源操作数。本实现未使用该参数。 |
| maxValue | 输出 | BesselI0接口能完成计算所需的最大临时空间大小，恒为 0。最大空间大小为0表示计算不需要临时空间。 |
| minValue | 输出 | BesselI0接口能完成计算所需最小临时空间大小，恒为 0。最小空间大小为0表示计算不需要临时空间。 |

## 返回值说明

无

## 约束说明

无

## 调用示例

完整的调用样例请参考[更多样例](../more_examples.md)。

```cpp
// 输入shape信息为1024;算子输入的数据类型为float;不允许修改源操作数
std::vector<int64_t> shape_vec = {1024};
AscendC::TensorShape shape(shape_vec);
uint32_t minValue;
uint32_t maxValue;
AscendC::GetBesselI0MaxMinTmpSize(shape, 4, false, maxValue, minValue);
// maxValue = 0, minValue = 0，无需预留临时空间
```
