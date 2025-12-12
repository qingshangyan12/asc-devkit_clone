# asc_gather_config

asc_gather_config为[asc_gather](../vector_compute/asc_gather.md)接口用于控制操作数地址步长的数据结构。具体使用方法可参考[如何使用高维切分计算API](../general_instruction.md#如何使用高维切分计算api)。

## 结构体具体定义

```cpp
union asc_gather_config {
    uint64_t config = CAPI_DEFAULT_GATHER_CONFIG_VALUE;
    struct {
        uint64_t offset_addr : 32;
        uint64_t dst_repeat_stride : 12;
        uint64_t reserved1 : 12;
        uint64_t repeat : 8;
    };
};
```

## 字段详解

|字段名|字段含义|
|----------|----------|
| offset_addr | 源操作数的每个元素对应的地址偏移。 |
| dst_repeat_stride | 目的操作数相邻迭代间相同DataBlock的地址步长。 |
| repeat | 迭代次数。 |