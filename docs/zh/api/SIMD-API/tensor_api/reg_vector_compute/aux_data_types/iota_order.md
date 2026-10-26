# iota_order

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/reg_index_compute.h"`。

`iota_order`用于指定索引生成接口产生递增序列或递减序列。

## 类型定义

```cpp
enum class iota_order : uint8_t {
    increase,
    decrease,
};
```

| 参数名 | 描述 |
| :-- | :-- |
| increase | 生成递增索引序列。 |
| decrease | 生成递减索引序列。 |
