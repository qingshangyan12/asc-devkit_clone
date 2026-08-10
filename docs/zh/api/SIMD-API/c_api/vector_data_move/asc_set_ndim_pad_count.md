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

本接口用于配置NDDMA高维切分搬运中`loop1`至`loop4`各维左右两侧的填充元素数。配置结果供后续调用[asc_ndim_copy_gm2ub](./asc_ndim_copy_gm2ub.md)时使用。

`loop0`的左右填充元素数由`asc_ndim_copy_gm2ub`的`loop0_lp_count`和`loop0_rp_count`参数配置，不通过本接口配置。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

```c
__aicore__ inline void asc_set_ndim_pad_count(asc_ndim_pad_count_config& config)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | NDDMA高维切分搬运的填充元素数配置，类型为`asc_ndim_pad_count_config&`。详细字段说明请参见[asc_ndim_pad_count_config](../struct/asc_ndim_pad_count_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 调用`asc_ndim_copy_gm2ub`前，必须先调用本接口完成`loop1`至`loop4`各维的填充元素数配置。
- 配置结果对后续的`asc_ndim_copy_gm2ub`调用生效，直至再次调用本接口覆盖配置。不同搬运任务需要不同填充元素数时，应在调用`asc_ndim_copy_gm2ub`前重新配置。
- 使用常数填充模式时，还需通过[asc_set_ndim_pad_value](./asc_set_ndim_pad_value.md)设置与搬运数据类型一致的填充值。

## 调用示例

```cpp
asc_ndim_pad_count_config config; // 所有字段默认值为0。
config.loop1_lp_count = 1;
config.loop1_rp_count = 2;
asc_set_ndim_pad_count(config);
```
