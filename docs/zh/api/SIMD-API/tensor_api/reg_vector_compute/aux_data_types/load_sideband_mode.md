# load_sideband_mode

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/reg_data_load.h"`。

`load_sideband_mode`用于指定[load](../reg_data_load/load.md)从UB搬入寄存器时的数据排列方式。

## 类型定义

```cpp
enum class load_sideband_mode : uint8_t {
    direct,
    deintlv,
    downsample,
    unpack,
    unpack4,
    upsample,
};
```

| 枚举值 | 描述 |
| --- | --- |
| `direct` | 连续搬入一个矢量寄存器长度的数据。 |
| `deintlv` | 解交织搬入，将偶数和奇数索引元素分别写入两个结果寄存器。 |
| `downsample` | 2倍下采样，每隔一个元素保留一个元素。 |
| `unpack` | 2倍展开，在每个源元素后补一个0元素。 |
| `unpack4` | 4倍展开，在每个源元素后补三个0元素。 |
| `upsample` | 2倍上采样，将每个源元素重复两次。 |

