# MakeNnLayout

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 函数原型

```cpp
template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeNnLayout(U row, S column)
```

## 功能描述

创建 NN 格式的 Layout 对象。NN 格式是 NPU 中用于 fp8_e8m0_t 数据类型的特殊分块存储格式。

## 参数说明

| 参数名 | 类型 | 描述 |
|--------|------|------|
| T | 模板参数 | 数据类型，通常为 fp8_e8m0_t |
| U | 模板参数 | 行参数类型，可以是编译时常量或运行时变量 |
| S | 模板参数 | 列参数类型，可以是编译时常量或运行时变量 |
| row | U | 矩阵的行数 |
| column | S | 矩阵的列数 |

## 返回值

- 输入为编译时常量时，返回Nn格式的Layout类型。
- 输入为整型变量时，返回Nn格式的Layout对象。
- 返回对齐后的Layout，对齐方式及对应位置的参数大小说明详见[Layout和层次化表述法](../../../Layout和层次化表述法.md)。

## 使用示例

```cpp
// 使用编译时常量
using Layout1 = decltype(MakeNnLayout<fp8_e8m0_t, Std::Int<32>, Std::Int<64>>(Std::Int<32>{}, Std::Int<64>{}));

// 使用运行时变量
size_t row = 32;
size_t col = 64;
auto layout2 = MakeNnLayout<fp8_e8m0_t>(row, col);
```
