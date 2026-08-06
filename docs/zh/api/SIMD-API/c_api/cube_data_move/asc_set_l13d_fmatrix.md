# asc_set_l13d_fmatrix

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

头文件路径：`"c_api/cube_datamove/cube_datamove.h"`。

设置3D格式分形矩阵搬运（im2col展开）的Feature Map属性描述，包括W维（宽度方向）与H维（高度方向）的尺寸，以及左、右、上、下四个方向的padding大小。本接口通过结构体`asc_l13d_fmatrix_config`传入配置，结构体字段已为C类型，用户直接对字段赋值即可，无需位域拼装。配置写入后作用于后续从左矩阵获取Feature Map属性的3D格式分形矩阵搬运接口。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

3D格式分形矩阵搬运（im2col展开）场景下，本接口配置左矩阵的Feature Map属性；双源模式需再通过`asc_set_l13d_fmatrix_b`配置右矩阵的第二个Feature Map属性，填充值需通过`asc_set_l13d_padding`配置，repeat控制参数需通过`asc_set_l13d_rpt`配置，L1 Buffer边界值需通过`asc_set_l13d_size`配置，各配套setter需配合使用后再调用3D格式分形矩阵搬运接口执行搬运。

## 函数原型

```cpp
__aicore__ inline void asc_set_l13d_fmatrix(asc_l13d_fmatrix_config& config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | Feature Map属性配置结构体，类型为`asc_l13d_fmatrix_config`。用户通过结构体字段`l1_height`、`l1_width`、`padding_left_size`、`padding_right_size`、`padding_top_size`、`padding_bottom_size`赋值，结构体内部以`uint64_t config`整体提交。结构体默认值为`ASC_DEFAULT_L13D_FMATRIX_CONFIG`（对应所有字段为0）。各字段含义与取值范围见下表。 |

**表2** `asc_l13d_fmatrix_config`结构体字段说明

| 字段名 | 数据类型 | 含义 | 取值范围 |
|---|---|---|---|
| `l1_width` | `uint16_t` | Feature Map在W维（宽度方向）的尺寸。 | [0, 2^16−1] |
| `l1_height` | `uint16_t` | Feature Map在H维（高度方向）的尺寸。 | [0, 2^16−1] |
| `padding_left_size` | `uint8_t` | Feature Map左侧padding大小。 | [0, 2^8−1] |
| `padding_right_size` | `uint8_t` | Feature Map右侧padding大小。 | [0, 2^8−1] |
| `padding_top_size` | `uint8_t` | Feature Map顶部padding大小。 | [0, 2^8−1] |
| `padding_bottom_size` | `uint8_t` | Feature Map底部padding大小。 | [0, 2^8−1] |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

<!-- npu="950" id8 -->
### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口需要配合asc_copy_l12l0a/asc_copy_l12l0b的3D格式搬运接口使用，并在搬运接口之前调用。
- 仅当3D格式搬运接口的f_matrix_ctrl参数指示从左矩阵获取Feature Map属性时使用本接口；否则使用asc_set_l13d_fmatrix_b。
- 配置写入后对后续所有从左矩阵获取Feature Map属性的3D格式分形矩阵搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同Feature Map属性，应在调用搬运接口前重新调用本接口设置。
- 结构体字段位宽与C类型完全匹配（`uint16_t`字段16bit、`uint8_t`字段8bit），用户字段值在C类型范围内即可被硬件完整接收，无截断风险。

<!-- end id8 -->

## 调用示例

```cpp
asc_l13d_fmatrix_config config;
config.l1_height = 1;
config.l1_width = 2;
config.padding_left_size = 3;
config.padding_right_size = 4;
config.padding_top_size = 5;
config.padding_bottom_size = 6;
asc_set_l13d_fmatrix(config);
```
