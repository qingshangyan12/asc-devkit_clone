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

用于设置接口[asc_copy_l12l0a](../cube_data_move/asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md)、[asc_copy_l12l0b](../cube_data_move/asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md)的2D格式搬运的repeat参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| config | 输入 | 用于设置接口asc_copy_l12l0a、asc_copy_l12l0b的2D格式搬运repeat参数。比特位说明参考表2。 |

**表2** 常用重复控制寄存器比特位说明

|L3D_RPT_B比特位    |功能|
| :-------     | :---- |
| L3D_RPT_B[15:0]    | 表示重复步长    |
| L3D_RPT_B[23:16]   | 表示在M或K方向的重复次数，默认值为1。      |
| L3D_RPT_B[24]      | 表示重复模式 <br> - 1'b0: 在M方向重复。 <br> - 1'b1: 在K方向重复。     |
| L3D_RPT_B[47:32]   | 表示输出矩阵在K方向上的步长，以分形为单位。     |
| L3D_RPT_B[63:48]   | 表示输出矩阵在M方向上的起始位置，以分形为单位。      |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需要配合接口[asc_copy_l12l0a](asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.md)、[asc_copy_l12l0b](asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.md)使用。

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅设置repeat控制参数，实际搬运由后续调用的2D格式搬运接口执行，须先调用本接口写入配置，再调用对应搬运接口。
- 配置写入后对后续2D格式搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同repeat参数，应在调用搬运接口前重新设置。
- `config`参数类型为`uint64_t`，与配置寄存器位宽一致（64bit），整个64bit立即数原样写入寄存器，无字段截断风险；调用方须自行保证各字段值落在对应位宽范围内。
- 配置寄存器为只写类型，本接口仅写不读，调用方无法通过本接口确认写入是否生效。

### repeat参数约束

- `rpt_time`为0时，后续2D格式搬运接口不执行搬运，被视为NOP（空操作）。
- 调用2D格式搬运接口时，必须配置输出矩阵在K方向上的步长和M方向上的起始位置。
- 未使能repeat模式时，repeat相关字段使用默认值。
- `rpt_mode`仅取0或1，其他值因1bit字段会被截断为低1位（如2→0、3→1），实际repeat方向可能与预期不符。

## 调用示例

```cpp
uint64_t config = 0;
asc_set_l3d_rpt_b(config);
```
