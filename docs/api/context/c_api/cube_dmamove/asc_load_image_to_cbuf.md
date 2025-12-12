
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
    __aicore__ inline void asc_load_image_to_cbuf(__cbuf__ half* dst, const asc_load_image_src_config& src_config, const asc_load_image_dst_config& dst_config)
    __aicore__ inline void asc_load_image_to_cbuf(__cbuf__ int8_t* dst, const asc_load_image_src_config& src_config, const asc_load_image_dst_config& dst_config)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ half* dst, const asc_load_image_src_config& src_config, const asc_load_image_dst_config& dst_config)
    __aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ int8_t* dst, const asc_load_image_src_config& src_config, const asc_load_image_dst_config& dst_config)
    ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（向量）的起始地址。 |
| src_config | 输入 | 源操作数相关的计算配置参数，详细说明请参考[asc_load_image_src_config](../struct/asc_load_image_src_config.md)。 |
| dst_config | 输入 | 目的操作数相关的计算配置参数，详细说明请参考[asc_load_image_dst_config](../struct/asc_load_image_dst_config.md)。 |

### 返回值说明

无

### 流水类型

PIPE_TYPE_MTE2

### 约束说明

- dst的起始地址需要32字节对齐。
- 需配合asc_set_aipp_functions接口使用，该接口用于配置图像预处理的相关参数。
- 当通过asc_set_aipp_functions接口配置padding模式为块填充模式或者镜像块填充模式时，左右padding的长度必须小于或等于[src_config](../struct/asc_load_image_src_config.md).hor_size，上下padding的长度必须小于或等于[src_config](../struct/asc_load_image_src_config.md).ver_size。
- 加载到dst的图片大小加上asc_set_aipp_functions接口配置的padding大小必须小于等于所在存储空间的大小。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

### 调用示例

```cpp
uint64_t offset = 0;
__cbuf__ half* dst = dst[offset];
asc_load_image_to_cbuf(dst, params);
```