# asc_set_ndim_pad_count

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/vector_datamove/vector_datamove.h"`。

设置NDDMA高维切分搬运模式下loop1~loop4各维左右两侧的padding元素个数。配置写入后供后续调用的[asc_ndim_copy_gm2ub](./asc_ndim_copy_gm2ub.md)接口使用。loop0维度的左右padding元素个数由`asc_ndim_copy_gm2ub`的`loop0_lp_count`和`loop0_rp_count`参数直接设置，不通过本接口配置。

本接口仅在AIV上执行有效，在AIC上调用为空操作。使用高维切分搬运的常数填充模式时，还需通过[asc_set_ndim_pad_value](./asc_set_ndim_pad_value.md)配置填充值；各维源端和目的端步长需通过[asc_set_ndim_loop_stride](./asc_set_ndim_loop_stride.md)配置。

## 函数原型

```c
__aicore__ inline void asc_set_ndim_pad_count(asc_ndim_pad_count_config& config)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | NDDMA高维切分搬运的padding元素个数配置，类型为`asc_ndim_pad_count_config&`。该配置包含loop1~loop4各维的左侧和右侧padding元素个数，共8个`uint8_t`字段；每个字段的单位为元素个数，取值范围为[0，255]，默认值为0。详细说明请参见[asc_ndim_pad_count_config](../struct/asc_ndim_pad_count_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- 本接口仅写入loop1~loop4各维的padding元素个数，实际填充和搬运由后续调用的[asc_ndim_copy_gm2ub](./asc_ndim_copy_gm2ub.md)执行。须先调用本接口写入配置，再调用搬运接口。
- loop0维度的左右padding元素个数不在`asc_ndim_pad_count_config`中配置，需通过`asc_ndim_copy_gm2ub`的`loop0_lp_count`和`loop0_rp_count`参数设置。
- `config`中的8个padding计数字段均为`uint8_t`，单位为元素个数，取值范围为[0，255]。配置对象默认初始化为0，表示loop1~loop4各维左右两侧均不填充。
- 配置写入后对后续高维切分搬运接口生效，直至再次调用本接口覆盖配置。不同搬运任务需要不同padding元素个数时，应在调用搬运接口前重新配置。
- 常数填充模式下，除本接口外，还需通过[asc_set_ndim_pad_value](./asc_set_ndim_pad_value.md)设置与搬运数据类型一致的填充值。

## 调用示例

```cpp
asc_ndim_pad_count_config config; // 所有字段默认值为0。
config.loop1_lp_count = 1;
config.loop1_rp_count = 2;
asc_set_ndim_pad_count(config);
```
