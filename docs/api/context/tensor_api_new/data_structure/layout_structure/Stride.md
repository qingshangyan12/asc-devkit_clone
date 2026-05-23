# Stride

`Stride`用于定义张量各维度在内存中的步长，用于描述同一维度相邻元素在内存中的间隔。

## 需要包含的头文件

```cpp
#include "tensor_api/tensor.h"
```

## 原型定义

```cpp
template <typename... Strides>
using Stride = Std::tuple<Strides...>;
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|----------|------|
| `Strides...` | 输入 | 可变参数模板，表示各维度的步长值，也可表示嵌套的`Stride`子结构。 |

## API映射关系

`Stride`通常通过[MakeStride](layout_structure_api/MakeStride.md)函数创建。
