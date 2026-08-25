# 逻辑与位运算

本节介绍基于`reg_tensor<T>`的取反、按位运算和移位接口。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [operator!](logical_not.md) | 逐元素取反。 |
| [operator&](bit_and.md) | 逐元素按位与。 |
| [operator\|](bit_or.md) | 逐元素按位或。 |
| [operator^](bit_xor.md) | 逐元素按位异或。 |
| [operator<<](shift_left.md) | 逐元素左移。 |
| [operator>>](shift_right.md) | 逐元素右移。 |

各接口的mask规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
