# gather_mode

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/reg_data_load.h"`。

`gather_mode`用于指定[load_gather](../reg_data_load/load_gather.md)从UB收集数据的粒度。

## 类型定义

```cpp
enum class gather_mode {
    element = 0,
    data_block,
};
```

| 枚举值 | 描述 |
| --- | --- |
| `element` | 根据索引逐元素收集，索引单位为元素。 |
| `data_block` | 根据索引按DataBlock（32B）收集，索引单位为字节。 |

