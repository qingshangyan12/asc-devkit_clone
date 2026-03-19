# MakeZnLayout

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

构造Zn格式的Layout对象。Zn格式是NZ格式的转置版本，外层矩阵按列存储，内层矩阵按行存储。

## 函数原型

```cpp
template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeZnLayout(size_t row, size_t column)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| row | 输入 | 矩阵的行数 |
| column | 输入 | 矩阵的列数 |

## 返回值说明

返回Zn格式的Layout对象。

## 约束说明

- T必须是有效的数据类型，如half、float、int32_t等
- row和column必须为正整数
- 内层矩阵的大小固定为(32 / sizeof(T)) * 16