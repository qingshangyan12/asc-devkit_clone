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
- 本接口仅设置第二组repeat控制参数，实际搬运由后续调用的3D格式分形矩阵搬运接口在选用第二组Feature Map描述时执行，须先调用本接口写入配置，再调用对应搬运接口。
- 配置写入后对后续所有选用第二组Feature Map描述的3D格式分形搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同repeat参数，应在调用搬运接口前重新调用本接口设置。
- 本接口须与其他配套setter接口配合使用：先调用配置第二组Feature Map属性、第二组填充值、L1 Buffer边界值的配套setter接口，再调用本接口配置第二组repeat参数，最后调用3D格式分形搬运接口（配套setter名见功能说明章节）执行搬运。
- `config`参数类型为`uint64_t`，与配置寄存器位宽一致（64bit），整个64bit立即数原样写入寄存器，无字段截断风险；调用方须自行保证各字段值落在对应位宽范围内。
- 配置寄存器为只写类型，本接口仅写不读，调用方无法通过本接口确认写入是否生效。

### repeat参数约束

- `rpt_time`不可为0，否则对应3D格式分形搬运指令被置为空操作，不执行搬运。
- `rpt_stride`单位随`rpt_mode`切换：`rpt_mode=0`时单位为16个元素，`rpt_mode=1`时单位为分形列大小（随搬运数据类型变化，int4b_t为64元素，int8_t/uint8_t为32元素，half/int16_t/bfloat16_t为16元素，int32_t/float为8元素）；用户须按所选repeat方向设置对应单位的步长值。
- `rpt_mode`仅取0或1，其他值因1bit字段会被截断为低1位（如2→0、3→1），硬件不报错但实际repeat方向与预期不符，可能沿错误维度重复搬运。

## 调用示例

```cpp
uint64_t config = 0;
asc_set_l3d_rpt_b(config);
```
