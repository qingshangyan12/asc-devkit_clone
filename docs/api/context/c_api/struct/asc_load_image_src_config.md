# asc_load_image_src_config

asc_load_image_src_config为[asc_load_image_to_cbuf](../cube_dmamove/asc_load_image_to_cbuf.md)接口需要配置的源操作数相关的参数结构体。

## 结构体具体定义

```cpp
union asc_load_image_src_config {
    uint64_t config = CAPI_DEFAULT_LOAD_IMAGE_SRC_CONFIG_VALUE;
    struct {
        uint64_t hor_size : 13;
        uint64_t ver_size : 13;
        uint64_t hor_start_pos : 13;
        uint64_t ver_start_pos : 13;
        uint64_t reserved1 : 12;
    };
};
```

## 字段详解

|字段名|字段含义|
|----------|----------|
| hor_size | 从源图中加载图片的水平宽度，单位为像素。取值范围：[1, 4094]。 |
| ver_size | 从源图中加载图片的垂直高度，单位为像素。取值范围：[1, 4094]。 |
| hor_start_pos | 加载图片在源图片上的水平起始地址，单位为像素，默认值：0。取值范围：[0, 4095]。 |
| ver_start_pos | 加载图片在源图片上的垂直起始地址，单位为像素，默认值：0。取值范围：[0, 4095]。 |