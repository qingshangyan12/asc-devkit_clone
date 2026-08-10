# asc_set_ndim_pad_value

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

本接口用于设置[asc_ndim_copy_gm2ub](./asc_ndim_copy_gm2ub.md)在常数填充模式下使用的填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。

填充值的数据类型应为本接口支持的数据类型，并与`asc_ndim_copy_gm2ub`搬运的数据类型一致。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 模板原型（占位符形式）

```c
__aicore__ inline void asc_set_ndim_pad_value(<dtype> pad_value)
```

### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：int32_t类型填充值设置。
__aicore__ inline void asc_set_ndim_pad_value(int32_t pad_value)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `pad_value` | 输入 | 常数填充值。当`padding_mode`设置为`true`时，后续调用`asc_ndim_copy_gm2ub`将使用该值填充搬运数据的边界区域。`pad_value`的数据类型应为本接口支持的数据类型，并与搬运数据类型一致。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口仅设置填充值，实际填充由后续调用的多维数据搬运接口在搬运过程中取用该填充值完成，须先调用本接口写入配置，再调用对应搬运接口。
- 常数填充模式下，还需配置各维左右两侧的填充元素数。`loop0`的左右填充元素数通过`asc_ndim_copy_gm2ub`的`loop0_lp_count`和`loop0_rp_count`参数配置，`loop1`至`loop4`的左右填充元素数通过[asc_set_ndim_pad_count](./asc_set_ndim_pad_count.md)配置。
- 配置写入后对后续所有`asc_ndim_copy_gm2ub`接口调用生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同填充值，应在调用搬运接口前重新调用本接口设置。
- 填充值仅在搬运接口选择常数填充模式时被取用；若选为最近值填充模式，本接口设置不生效。

## 调用示例

```cpp
half pad_value = 2.0;
asc_set_ndim_pad_value(pad_value);
```
