# Shape

`Shape`用于定义张量的逻辑形状，用于描述各维度的大小。

## 需要包含的头文件

```cpp
#include "tensor_api/tensor.h"
```

## 原型定义

```cpp
template <typename... Shapes>
using Shape = Std::tuple<Shapes...>;
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|----------|------|
| `Shapes...` | 输入 | 可变参数模板，表示各维度的形状值，也可表示嵌套的`Shape`子结构。 |

## API映射关系

`Shape`通常通过[MakeShape](layout_structure_api/MakeShape.md)函数创建。
