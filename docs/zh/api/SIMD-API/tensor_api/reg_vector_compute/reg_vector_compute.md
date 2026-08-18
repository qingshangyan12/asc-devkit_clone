# Reg 矢量计算

Tensor API Reg 矢量计算接口以 `asc::te::reg_tensor<T>` 为输入和返回值，mask 随寄存器对象传递，支持返回值风格和 C++ 运算符表达式。

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 接口分类

| 分类 | 说明 |
| --- | --- |
| [基础算术](basic_arithmetic/basic_arithmetic.md) | 基础算术、数学函数、激活函数、进位和宽乘。 |
| [逻辑与位运算](logical_compute/logical_compute.md) | 逻辑非、按位运算和移位。 |
| [复合计算](composite_compute/composite_compute.md) | 乘加、差值、指数和类型转换等融合计算。 |
| [直方图计算](histogram_compute/histogram_compute.md) | 频率直方图和累计直方图。 |

## 编程模型

```cpp
auto mask = asc::te::all_mask<float>();
auto src0 = input0.load(make_coord(0, 0)).with_mask(mask);
auto src1 = input1.load(make_coord(0, 0)).with_mask(mask);
auto result = asc::te::abs(src0) + src1;
output.store(make_coord(0, 0), result);
```

接口只能在支持 `__simd_callee__` 调用的设备侧函数中使用。当前实现面向 `__NPU_ARCH__ == 3510`。
