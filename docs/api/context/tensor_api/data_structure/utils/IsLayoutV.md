# IsLayoutV

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

`IsLayoutV`用于判断一个类型是否为Layout类型。

当前实现中，满足以下形式的类型会返回`true`：

```cpp
Layout<T, U, S>
```

其中`T`通常为Shape类型，`U`通常为Stride类型，`S`为附加信息类型。

## 原型定义

```cpp
template <typename T>
constexpr bool IsLayoutV = IsLayout<Std::remove_cvref_t<T>>::value;
```

## 参数说明

**表 1** 模板参数说明

| 参数名 | 类型 | 描述 |
|--------|------|------|
| T | 输入 | 待判断的类型。 |

## 返回值说明

- 若`T`是Layout类型，则返回`true`；
- 否则返回`false`。

## 约束说明

- 判断时会自动去除`T`的引用和cv限定。

## 调用示例

```cpp
using namespace AscendC::Te;

auto layout = MakeLayout(MakeShape(16, 16), MakeStride(16, 1));

constexpr bool isLayout = IsLayoutV<decltype(layout)>;
// isLayout = true

constexpr bool isLayoutRef = IsLayoutV<const decltype(layout)&>;
// isLayoutRef = true

constexpr bool isTuple = IsLayoutV<Std::tuple<int, int>>;
// isTuple = false
```
