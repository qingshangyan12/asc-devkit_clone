# asc_deq_scale_config

asc_deq_scale_config是设置量化操作的数据结构。

## 结构体具体定义

```cpp
union asc_deq_scale_config {
    uint64_t val;
    struct {
        uint64_t reserved1 : 5;
        uint64_t offset : 9;
        uint64_t sign_mode : 1;
        uint64_t reserved2 : 17;
        uint64_t scale;
    };
};
```

## 字段详解

|字段名|字段含义|
|----------|----------|
| offset | 量化参数，用于非向量模式下设置DEQSCALE寄存器的值。默认为0。 |
| sign_mode | 表示量化结果是否带符号，用于非向量模式场景。 |
| scale | 量化参数。 |