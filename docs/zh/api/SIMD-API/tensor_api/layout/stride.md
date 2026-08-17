# stride

> [!NOTE]说明
>本接口为试验接口，在后续版本中可能会调整或改进，不保证后续兼容性。请开发者在使用过程中关注后续版本更新。

stride用于定义张量各维度在内存中的步长，用于描述同一维度相邻元素在内存中的间隔。

## 需要包含的头文件

```cpp
#include "tensor_api/tensor.h"
```

## 原型定义

```cpp
template <typename... Strides>
using stride = Std::tuple<Strides...>;
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|----------|------|
| Strides... | 输入 | 可变参数模板，表示各维度的步长值，也可表示嵌套的stride子结构。 |

## API映射关系

stride通常通过[make_stride](make_stride.md)函数创建，通过[get_stride](get_stride.md)函数和layout的[stride](layout.md)成员函数获取。
