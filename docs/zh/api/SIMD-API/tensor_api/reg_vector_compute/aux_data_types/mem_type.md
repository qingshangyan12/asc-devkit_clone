# mem_type

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/reg_sync_control.h"`。

`mem_type`用于指定[mem_bar](../reg_sync_control/mem_bar.md)同步的源流水线和目的流水线。

## 类型定义

```cpp
enum class mem_type {
    vec_load,
    vec_store,
    scalar_load,
    scalar_store,
    vec_all,
    scalar_all,
};
```

| 参数名 | 描述 |
| :-- | :-- |
| vec_load | 矢量读UB流水线，对应UB到寄存器的数据搬入接口，例如`load`。 |
| vec_store | 矢量写UB流水线，对应寄存器到UB的数据搬出接口，例如`store`。 |
| scalar_load | 标量读UB流水线。 |
| scalar_store | 标量写UB流水线。 |
| vec_all | 所有矢量读写UB流水线。 |
| scalar_all | 所有标量读写UB流水线。 |

支持的源、目的流水线组合请参考[mem_bar](../reg_sync_control/mem_bar.md)。
