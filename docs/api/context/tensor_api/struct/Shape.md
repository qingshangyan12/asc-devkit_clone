# Shape

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

Shape用于定义张量的形状，描述张量在各维度上的大小。

## 类型定义

Shape 是一个模板别名，用于表示张量的形状：

```cpp
template <typename... Shapes>
using Shape = Std::tuple<Shapes...>;
```

其中：
- `Shapes...` 是可变参数模板，表示各维度的大小
- 实际类型为 `Std::tuple<Shapes...>`

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|----------|------|
| Shapes... | 输入 | 各维度的大小，类型为size_t等整数类型或者Std::Int类型。 |

## API 映射关系

Shape 通常通过 MakeShape 函数创建：

```cpp
// MakeShape 函数原型
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t)
```
其中，参数t为各维度的大小，类型为size_t等整数类型或者Std::Int类型。

## 约束说明

- Shape的维度数量不能超过硬件支持的最大维度数。
- 各维度的值必须为正整数。
- 支持的数据类型包括：size_t、int等整数类型或者Std::Int类型。

## 调用示例

```cpp
// 使用整数类型创建一个3维张量的Shape
auto shape = AscendC::Te::MakeShape(10, 20, 30);

// 获取各维度的大小
auto dim0 = AscendC::Std::get<0>(shape); // dim0 = 10
auto dim1 = AscendC::Std::get<1>(shape); // dim1 = 20
auto dim2 = AscendC::Std::get<2>(shape); // dim2 = 30

// 使用Std::Int创建一个3维张量的Shape
auto shape = AscendC::Te::MakeShape(AscendC::Std::Int<10>{}, AscendC::Std::Int<20>{}, AscendC::Std::Int<30>{});

// 获取各维度的大小
auto dim0 = AscendC::Std::get<0>(shape); // dim0 = 10
auto dim1 = AscendC::Std::get<1>(shape); // dim1 = 20
auto dim2 = AscendC::Std::get<2>(shape); // dim2 = 30
```