
# asc_load_image_to_cbuf

## AI处理器支持情况

| AI处理器类型         | 是否支持 |
| :-----------------------| :-----:|
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

### 功能说明

将图像数据从Global Memory搬运到L1 Buffer。

### 函数原型

- 常规计算

    ```cpp
    __aicore__ inline void asc_load_image_to_cbuf(__cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid)
    __aicore__ inline void asc_load_image_to_cbuf(__cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid)
    __aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid)
    ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| hor_size | 输入 | 从源图中加载图片的水平宽度，单位为像素。取值范围：[1, 4094]。 |
| ver_size | 输入 | 从源图中加载图片的垂直高度，单位为像素。取值范围：[1, 4094]。 |
| hor_start_pos | 输入 | 加载图片在源图片上的水平起始地址，单位为像素。取值范围：[0, 4095]。 |
| ver_start_pos | 输入 | 加载图片在源图片上的垂直起始地址，单位为像素。取值范围：[0, 4095]。 |
| src_hor_res | 输入 | 源图片水平宽度，单位为像素。取值范围：[1, 4094]。 |
| top_pad_size | 输入 | 目的图片顶部填充的像素数。取值范围：[0, 32]。 |
| bot_pad_size | 输入 | 目的图片底部填充的像素数。取值范围：[0, 32]。 |
| left_pad_size | 输入 | 目的图片左边填充的像素数。取值范围：[0, 32]。 |
| right_pad_size | 输入 | 目的图片右边填充的像素数。取值范围：[0, 32]。 |
| sid | 输入 | 保留，未使用。填0即可。 |

### 返回值说明

无

### 流水类型

PIPE_MTE2

### 约束说明

- dst的起始地址需要32字节对齐。
- 需配合asc_set_aipp_functions接口使用，该接口用于配置图像预处理的相关参数。
- 当通过asc_set_aipp_functions接口配置padding模式为块填充模式或者镜像块填充模式时，左右padding的长度必须小于或等于hor_size，上下padding的长度必须小于或等于ver_size。
- 加载到dst的图片大小加上asc_set_aipp_functions接口配置的padding大小必须小于等于所在存储空间的大小。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

### 调用示例

```cpp
constexpr uint64_t total_length = 128;    // total_length指参与搬运的数据总长度
__ca__ half dst[total_length * sizeof(half)];
asc_load_image_to_cbuf(dst, params);
```