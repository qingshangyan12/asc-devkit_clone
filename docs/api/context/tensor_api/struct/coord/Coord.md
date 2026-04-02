# Coord

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

Coord用于定义张量的坐标，用于访问张量中特定位置的元素。

## 类型定义

```cpp
// Coord定义为存储各维度坐标的元组
template <typename... Coords>
using Coord = Std::tuple<Coords...>;
```

## 约束说明

- Coord的维度数量必须与对应的Shape维度数量一致。
- 各维度的坐标值必须在对应Shape维度的有效范围内。
- 支持的数据类型包括：size_t、int等整数类型或者Std::Int类型。

## 调用示例

```cpp
// 使用整数类型创建一个3维张量的坐标
auto coord = AscendC::Te::MakeCoord(5, 10, 15);

// 获取各维度的坐标
auto coord0 = AscendC::Std::get<0>(coord); // coord0 = 5
auto coord1 = AscendC::Std::get<1>(coord); // coord1 = 10
auto coord2 = AscendC::Std::get<2>(coord); // coord2 = 15

// 使用Std::Int类型创建一个3维张量的坐标
auto coord = AscendC::Te::MakeCoord(AscendC::Std::Int<5>{}, AscendC::Std::Int<10>{}, AscendC::Std::Int<15>{});

// 获取各维度的坐标
auto coord0 = AscendC::Std::get<0>(coord); // coord0 = 5
auto coord1 = AscendC::Std::get<1>(coord); // coord1 = 10
auto coord2 = AscendC::Std::get<2>(coord); // coord2 = 15
```