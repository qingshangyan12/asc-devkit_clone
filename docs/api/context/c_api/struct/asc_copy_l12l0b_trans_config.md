# asc_copy_l12l0b_trans_config

asc_copy_l12l0b_trans_config为数据搬运同时进行转置操作的控制参数。

## 结构体具体定义

```cpp
    union asc_copy_l12l0b_trans_config {
        uint64_t config = CAPI_DEFAULT_COPY_TRANS_CONFIG_VALUE;
        struct {
            uint64_t index_id : 16;
            uint64_t repeat : 8;
            uint64_t src_stride : 16;
            uint64_t dst_stride : 16;
            uint64_t addrmode : 1;
        };
    };
```

## 字段详解

|字段名|字段含义|
|----------|----------|
| index_id | 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0 为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535] ，默认值为0 |
| repeat | 迭代重复次数。repeat∈[0, 255]。默认为0。<br> 对于uint8_t/int8_t数据类型，每次迭代处理32 $ \times $ 32 $ \times $ 1B数据；<br> 对于half/bfloat16_t数据类型，每次迭代处理16 $ \times $ 16 $ \times $ 2B数据；<br> 对于float/int32_t/uint32_t数据类型，每次迭代处理16 $ \times $ 16 $ \times $ 4B数据。|
| src_stride | 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。默认值为0，取值范围：src_stride∈[0, 65535]。默认为0。<br> 对于uint8_t/int8_t数据类型，单位是32 $ \times $ 32 $ \times $ 1B； <br> 对于half/bfloat16_t数据类型，单位是16 $ \times $ 16 $ \times $ 2B；<br> 对于float/int32_t/uint32_t数据类型，单位是16 $ \times $ 16 $ \times $ 4B。 <br> 对于int4b_t数据类型，每次迭代处理16 $ \times $ 64 $ \times $ 0.5B数据。|
| dst_stride | 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_stride∈[0, 65535]。默认为0。 |
| addrmode |保留，默认值为0 |