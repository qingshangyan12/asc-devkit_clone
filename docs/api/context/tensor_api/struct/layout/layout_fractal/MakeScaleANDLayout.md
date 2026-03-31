# MakeScaleBDNLayout

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 函数原型

```cpp
template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleANDLayout(U row, S column)
```

## 功能描述

创建 ScaleAND 格式的 Layout 对象。ScaleBDN 格式用于矩阵 B 的转置布局（scaleK, n），常用于矩阵乘法中 B 矩阵转置后的存储。

## 参数说明

| 参数名 | 类型 | 描述 |
|--------|------|------|
| T | 模板参数 | 数据类型，支持数据类型为fp8_e8m0_t。 |
| U | 模板参数 | 行参数类型，可以是编译时常量或运行时变量 |
| S | 模板参数 | 列参数类型，可以是编译时常量或运行时变量 |
| row | U | 矩阵的行数 |
| column | S | 矩阵的列数 |

## 返回值

- 输入为编译时常量时，返回ScaleANDLayout格式的Layout类型。
- 输入为整型变量时，返回ScaleANDLayout格式的Layout对象。
- ScaleANDLayout分形数值及对齐说明可参见[Layout和层次化表述法](../../../Layout和层次化表述法.md)。

## 使用示例

```cpp
// 使用编译时常量
using Layout1 = decltype(MakeScaleANDLayout<fp8_e8m0_t, Std::Int<64>, Std::Int<32>>(Std::Int<64>{}, Std::Int<32>{}));

// 使用运行时变量
size_t scaleK = 64;
size_t n = 32;
auto layout2 = MakeScaleANDLayout<fp8_e8m0_t>(scaleK, n);
```


