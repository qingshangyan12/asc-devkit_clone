# asc_set_l12l0_padding_val

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

头文件路径：`"c_api/sys_var/sys_var.h"`。

本接口用于配置右矩阵（B组）Feature Map的Pad填充值。配置写入后，由[asc_copy_l12l0a](../cube_data_move/asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md)或[asc_copy_l12l0b](../cube_data_move/asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md)的3D格式搬运接口在执行Image to Column展开时使用，对Feature Map边界外的数据进行定值填充。

仅当3D格式搬运接口的`f_matrix_ctrl`设置为true、选择右矩阵的Feature Map属性时，本接口的配置生效。`f_matrix_ctrl`设置为false时，本接口的配置不生效。

## 函数原型

```c
__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| config | 输入 | 右矩阵Feature Map的64 bit Pad填充值配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。 |

**表2** config参数说明 <a id="table2"></a>

| 搬运数据位宽 | `config[31:0]`的配置方法 |
| :--- | :--- |
| 8 bit | `config[7:0]`为Pad填充值的原始位模式，`config[15:8]`必须与`config[7:0]`相同，`config[31:16]`设置为0。 |
| 16 bit | `config[15:0]`为Pad填充值的原始位模式，`config[31:16]`设置为0。 |
| 32 bit | `config[31:0]`为Pad填充值的原始位模式。 |

`config[63:32]`为保留位，必须设置为0。

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- 本接口需在对应的3D格式搬运接口执行前调用，并将搬运接口的`f_matrix_ctrl`设置为true。配置会持续生效，直至再次调用本接口覆盖。
- 使用右矩阵Feature Map时，还需通过[asc_set_l13d_fmatrix_b](../cube_data_move/asc_set_l13d_fmatrix_b.md)配置Feature Map属性，并通过[asc_set_l3d_rpt_b](./asc_set_l3d_rpt_b.md)配置repeat参数。
- 配置8 bit Pad填充值时，必须将同一个8 bit原始位模式分别写入`config[7:0]`和`config[15:8]`，否则搬运结果未定义。本接口不会自动复制填充值。
- 本接口直接写入完整配置，不检查字段取值。除[表2](#table2)中使用的字段外，其余位必须设置为0。

## 调用示例

```cpp
// 将int8_t类型的Pad填充值配置为-1，其原始位模式为0xFF。
constexpr uint8_t paddingValue = 0xFF;
uint64_t config = static_cast<uint64_t>(paddingValue) |
                  (static_cast<uint64_t>(paddingValue) << 8);
asc_set_l12l0_padding_val(config);
```
