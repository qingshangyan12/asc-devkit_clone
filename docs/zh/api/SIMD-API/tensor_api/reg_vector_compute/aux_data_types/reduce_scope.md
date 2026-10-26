# reduce_scope

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/reduction_compute.h"`。

`reduce_scope`用于指定归约接口的计算范围。

## 类型定义

```cpp
enum class reduce_scope : uint8_t {
    element,
    datablock,
    pair,
};
```

| 枚举值 | 描述 |
| --- | --- |
| `element` | 对源寄存器中的所有有效元素归约。 |
| `datablock` | 分别对每个DataBlock（32B）内的有效元素归约。 |
| `pair` | 对相邻元素成对归约，仅支持明确声明该模式的接口。 |

不同归约接口支持的范围可能不同，具体请参考[归约计算](../reduction_compute/reduction_compute.md)下各接口说明。

