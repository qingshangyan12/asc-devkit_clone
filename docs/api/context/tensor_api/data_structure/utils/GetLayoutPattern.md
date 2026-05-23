# GetLayoutPattern

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

`GetLayoutPattern`用于从Layout类型中提取其绑定的LayoutPattern类型信息。

当Layout的第三个模板参数为`Std::tuple<LayoutPattern, TraitType>`时，`GetLayoutPattern<T>`返回对应的`LayoutPattern`类型；否则返回`Std::ignore_t`。

## 原型定义

```cpp
template <typename T>
using GetLayoutPattern = typename GetPattern<Std::remove_cvref_t<T>>::type;
```

## 参数说明

**表 1** 模板参数说明

| 参数名 | 类型 | 描述 |
|--------|------|------|
| T | 输入 | 待提取LayoutPattern的类型，通常为Layout类型或其引用/const形式。 |

## 返回值说明

`GetLayoutPattern<T>`是一个类型别名：

- 若`T`包含布局模式信息，则返回对应的`LayoutPattern`类型；
- 否则返回`Std::ignore_t`。

## 约束说明

- `T`通常应为Layout类型或可去除引用、cv限定后的Layout类型。

## 调用示例

```cpp
using namespace AscendC::Te;

using LayoutType = decltype(MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float>>(16, 16));
using Pattern = GetLayoutPattern<LayoutType>;
// Pattern = NZLayoutPtn

using PlainLayout = decltype(MakeLayout(MakeShape(16, 16), MakeStride(16, 1)));
using PlainPattern = GetLayoutPattern<PlainLayout>;
// PlainPattern = Std::ignore_t
```
