# IsIntegralConstantV

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

`IsIntegralConstantV`用于判断一个类型是否为`Std::Int<Value>`形式的整型编译期常量类型。

## 原型定义

```cpp
template <typename T>
constexpr bool IsIntegralConstantV = IsIntegralConstant<T>::value;
```

## 参数说明

**表 1** 模板参数说明

| 参数名 | 类型 | 描述 |
|--------|------|------|
| T | 输入 | 待判断的类型。 |

## 返回值说明

- 若`T`是`Std::Int<Value>`类型，则返回`true`；
- 否则返回`false`。

## 约束说明

- 当前仅对`Std::Int<Value>`特化为`true`，不自动接受其他形式的`integral_constant`。

## 调用示例

```cpp
using namespace AscendC::Te;

constexpr bool isInt16 = IsIntegralConstantV<Std::Int<16>>;
// isInt16 = true

constexpr bool isAlias16 = IsIntegralConstantV<_16>;
// isAlias16 = true

constexpr bool isSizeT = IsIntegralConstantV<size_t>;
// isSizeT = false
```
