# asc_load_image_dst_config

asc_load_image_dst_config为[asc_load_image_to_cbuf](../cube_dmamove/asc_load_image_to_cbuf.md)接口需要配置的目的操作数相关的参数结构体。

## 结构体具体定义

```cpp
union asc_load_image_dst_config {
    uint64_t config = CAPI_DEFAULT_LOAD_IMAGE_DST_CONFIG_VALUE;
    struct {
        uint64_t src_hor_res : 16;
        uint64_t top_pad_size : 8;
        uint64_t bot_pad_size : 8;
        uint64_t left_pad_size : 13;
        uint64_t right_pad_size : 13;
        uint64_t sid : 4;
        uint64_t reserved2 : 2;
    };
};
```

## 字段详解

|字段名|字段含义|
|----------|----------|
| src_hor_res | 源图片水平宽度，单位为像素。取值范围：[1, 4094]。 |
| top_pad_size | 目的图片顶部填充的像素数，默认值：0。取值范围：[0, 32]。 |
| bot_pad_size | 目的图片底部填充的像素数，默认值：0。取值范围：[0, 32]。 |
| left_pad_size | 目的图片左边填充的像素数，默认值：0。取值范围：[0, 32]。 |
| right_pad_size | 目的图片右边填充的像素数，默认值：0。取值范围：[0, 32]。 |
| sid | 保留，未使用。填0即可。 |