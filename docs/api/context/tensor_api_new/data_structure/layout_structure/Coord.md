# Coord

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

头文件为：#include "tensor_api/tensor.h"

Coord用于定义张量的坐标，用于访问张量中特定位置的元素。

## 类型定义

Coord是一个模板别名，用于表示张量的坐标：

```cpp
template <typename... Coords>
using Coord = Std::tuple<Coords...>;
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|----------|------|
| Coords... | 输入 | 可变参数模板，表示各维度的坐标。|

## API映射关系

Coord通常通过[MakeCoord](layout_structure_api/MakeCoord.md)函数创建。