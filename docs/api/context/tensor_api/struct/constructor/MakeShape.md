# MakeShape

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

构造Shape对象，用于定义张量的形状。

## 函数原型

```cpp
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| t | 输入 | 各维度的大小，可变参数 |

## 返回值说明

返回Shape<Ts...>对象。

## 约束说明

- 参数数量不能超过硬件支持的最大维度数
- 各参数必须为正整数
- 支持的数据类型包括：size_t、int等整数类型

## 调用示例

```cpp
// 创建一个3维张量的Shape
auto shape = AscendC::MakeShape(10, 20, 30);

// 获取各维度的大小
auto dim0 = AscendC::Std::get<0>(shape.value); // dim0 = 10
auto dim1 = AscendC::Std::get<1>(shape.value); // dim1 = 20
auto dim2 = AscendC::Std::get<2>(shape.value); // dim2 = 30
```