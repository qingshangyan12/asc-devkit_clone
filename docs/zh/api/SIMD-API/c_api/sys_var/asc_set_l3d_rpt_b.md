# asc_set_l3d_rpt_b

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

本接口用于配置右矩阵（B组）Feature Map的repeat参数。配置写入后，由[asc_copy_l12l0a](../cube_data_move/asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md)或[asc_copy_l12l0b](../cube_data_move/asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md)的3D格式搬运接口在执行Image to Column展开时使用。

仅当3D格式搬运接口的`f_matrix_ctrl`设置为true、选择右矩阵的Feature Map属性时，本接口的配置生效。`f_matrix_ctrl`设置为false时，需使用[asc_set_l13d_rpt](../cube_data_move/asc_set_l13d_rpt.md)配置左矩阵的repeat参数。

## 函数原型

```c
__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| config | 输入 | 右矩阵Feature Map的64 bit repeat配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。 |

**表2** config字段说明 <a id="table2"></a>

| bit范围 | 对应Basic API参数 | 描述 |
|---|---|---|
| 15:0 | repeatStride | 相邻两次迭代起始地址的偏移，取值范围为[0, 65535]。`repeatMode`为0时，单位为16个元素；`repeatMode`为1时，单位为`32 / sizeof(T)`个元素，T为源操作数的数据类型。 |
| 23:16 | repeatTime | 迭代次数，取值范围为[0, 255]，默认值为1。取值为0时，后续3D格式搬运接口被视为NOP（空操作）。 |
| 24 | repeatMode | 迭代方向。取值为0时，沿M（height）方向迭代；取值为1时，沿K（width）方向迭代。 |
| 31:25 | - | 保留位，需设置为0。 |
| 47:32 | dstStride | 输出矩阵K轴方向的偏移，取值范围为[0, 65535]，单位为512字节分形。Ascend 950PR/Ascend 950DT上必须根据目的矩阵排布配置本字段。 |
| 63:48 | dstMposition | 输出矩阵M轴方向的起始位置，取值范围为[0, 65535]，单位为分形。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- 本接口需在对应的3D格式搬运接口执行前调用，并将搬运接口的`f_matrix_ctrl`设置为true。配置会持续生效，直至再次调用本接口覆盖。
- 使用右矩阵Feature Map时，还需通过[asc_set_l13d_fmatrix_b](../cube_data_move/asc_set_l13d_fmatrix_b.md)配置Feature Map属性；需要padding时，通过[asc_set_l12l0_padding_val](./asc_set_l12l0_padding_val.md)配置填充值。
- `repeatTime`为0时不执行搬运。`repeatMode`只能为0或1，并按照对应的单位配置`repeatStride`。
- `dstStride`需与目的矩阵的K轴排布匹配，避免不同迭代写入的分形发生非预期重叠。
- `config[31:25]`为保留位，需设置为0。接口会将`config`整体写入，不会检查各字段的取值范围。

## 调用示例

```cpp
constexpr uint16_t repeat_stride = 0;
constexpr uint8_t repeat_time = 1;
constexpr uint8_t repeat_mode = 0;     // 沿M（height）方向迭代
constexpr uint16_t dst_stride = 1;     // 输出矩阵K轴方向偏移，单位为512字节分形
constexpr uint16_t dst_m_position = 0; // 输出矩阵M轴方向起始位置

uint64_t config = (static_cast<uint64_t>(dst_m_position) << 48) |
                  (static_cast<uint64_t>(dst_stride) << 32) |
                  (static_cast<uint64_t>(repeat_mode) << 24) |
                  (static_cast<uint64_t>(repeat_time) << 16) |
                  static_cast<uint64_t>(repeat_stride);
asc_set_l3d_rpt_b(config);
```
