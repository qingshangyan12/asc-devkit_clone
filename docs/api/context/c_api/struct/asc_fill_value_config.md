# asc_fill_value_config

asc_fill_value_config为fill_value的初始化参数结构体，包含[asc_fill_l0a_value](../cube_dmamove/asc_fill_l0a_value.md)/[asc_fill_l0b_value](../cube_dmamove/asc_fill_l0b_value.md)/[asc_fill_l1_value](../cube_dmamove/asc_fill_l1_value.md)接口需要配置的各种初始化参数。

## 结构体具体定义

```cpp
union asc_fill_value_config {
    uint64_t config = CAPI_DEFAULT_FILL_VALUE_CONFIG_VALUE;
    struct {
        uint64_t repeat : 15;
        uint64_t reserved1 : 1;
        uint64_t block_num : 15;
        uint64_t reserved2 : 1;
        uint64_t dst_gap : 15;
        uint64_t reserved3 : 17;
    };
};
```

## 字段详解

|字段名|字段含义|
|----------|----------|
| repeat | 迭代次数，默认值：0。取值范围：[0, 32767]。 |
| block_num | 每次迭代初始化的数据块个数，默认值：0。取值范围：[0, 32767]。<br>目的操作数位于L1 Buffer时，数据块大小是32B；目的操作数位于L0A Buffer/L0B Buffer时，数据块大小是512B。 |
| dst_gap | 目的操作数前一个迭代结束地址到后一个迭代起始地址之间的距离，默认值：0。取值范围：[0, 32767]。<br>目的操作数位于L1 Buffer时，单位是32B；目的操作数位于L0A Buffer/L0B Buffer时，单位是512B。 |