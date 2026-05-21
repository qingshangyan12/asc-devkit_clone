# GetStride

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Ascend 950PR/Ascend 950DT | √ |

## 头文件/库文件

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

Tensor API 为头文件模板接口，不需要链接额外库文件。

## 功能说明

GetStride用于从Layout中提取 Stride 部分：

- **不指定模板参数 `Is...`**：返回完整的 Stride 元组。
- **指定模板参数 `Is...`**：返回 Stride 中对应索引的子结构。单个索引返回对应步长值或子 tuple；多个索引返回由它们组成的新 tuple。

## 函数原型

```cpp
template <size_t... Is, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto GetStride(const Layout<ShapeType, StrideType>& layout)

template <size_t... Is, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto GetStride(Layout<ShapeType, StrideType>& layout)
```

## 参数说明

**表 1** 模板参数说明

| 参数名 | 输入/输出 | 描述 |
| -------- | ----------- | ------ |
| Is... | 输入 | 可选。多级索引递归选取。 |
| ShapeType | 输入 | Layout的shape类型。 |
| StrideType | 输入 | Layout的stride类型。 |

**表 2** 参数说明

| 参数名 | 输入/输出 | 描述 |
| -------- | ----------- | ------ |
| layout | 输入 | Layout对象。 |

## 返回值说明

返回描述内存访问步长的 Stride 对象（或其子结构）。类型与模板参数 `Is...` 的指定方式相关。

## 约束说明

- Is...必须在 Shape 底层 tuple 的有效范围内。

## 调用示例

```cpp
using namespace AscendC::Te;

auto layout = MakeLayout(MakeShape(10, 20, 30), MakeStride(1, 10, 200));

// 不指定 Is... → 返回完整 stride 元组
auto fullStride = GetStride(layout);         // (1, 10, 200)

// 指定单个索引 → 返回对应元素
auto s0 = GetStride<0>(layout);             // 1

// 指定多个索引 → 组成新 tuple
auto s01 = GetStride<0, 1>(layout);         // (1, 10)
```
