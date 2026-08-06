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

设置第二组3D格式分形矩阵搬运（im2col展开）的repeat控制参数，包含相邻repeat之间的步长、repeat次数与repeat方向，以及输出矩阵在K维的步长与M维起始位置。本接口接收`uint64_t`立即数整体写入配置寄存器，调用方须按`config`字段拼装说明自行拼装64 bit `config`后传入。配置写入后作用于后续调用的3D格式分形矩阵搬运接口在第二组Feature Map描述（双源拼接场景下由搬运指令的Feature Map选择位选用）下的repeat行为。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

双源拼接场景下，需通过[asc_set_l13d_fmatrix_b](asc_set_l13d_fmatrix_b.md)预先配置第二组Feature Map属性、通过[asc_set_l12l0_padding_val](asc_set_l12l0_padding_val.md)预先配置第二组填充值、通过[asc_set_l13d_size](asc_set_l13d_size.md)预先配置L1 Buffer边界值，再调用本接口配置第二组repeat参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config)
```

## 参数说明

**表1**参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | 第二组3D格式分形搬运repeat控制参数的64 bit立即数，类型为`uint64_t`，与配置寄存器位宽一致（64 bit），整个64 bit立即数原样写入寄存器。调用方须按`config`字段拼装说明（表2）的位域定义自行拼装后整体传入。取值范围 [0, 2^64−1]。 |

**表2**`config`字段拼装说明

| 字段名 | 含义 | 位宽 | 取值范围 |
|---|---|---|---|
| `rpt_stride` | 相邻repeat之间的步长。<br>&nbsp;&nbsp;&bull; `rpt_mode=0`（M方向repeat）：单位为16个元素，步长作用于M方向相邻repeat之间的地址偏移；<br>&nbsp;&nbsp;&bull; `rpt_mode=1`（K方向repeat）：单位为分形列大小（随搬运数据类型变化，`int4b_t`为64元素，`b8`为32元素，`b16`为16元素，`b32`为8元素），步长作用于K方向相邻repeat之间的地址偏移。占用`config`的低16位（第0~15位）。 | 16 bit | [0, 2^16−1] |
| rpt_time | repeat次数，即3D格式分形搬运接口对同一批Feature Map重复搬运的迭代次数。不可为0，传入0时硬件不执行搬运（指令置为空操作）。占用`config`的第16~23位。 | 8 bit | [0, 2^8−1] |
| `rpt_mode` | repeat方向选择。<br>&nbsp;&nbsp;&bull; `0`：M方向repeat，沿M维（行方向）重复搬运；<br>&nbsp;&nbsp;&bull; `1`：K方向repeat，沿K维（通道方向）重复搬运。占用`config`的第24位。 | 1 bit | [0, 1] |
| m_dst_stride | 输出矩阵在K维的步长，单位为分形，用于K方向repeat时控制相邻repeat输出矩阵在K维的地址偏移。占用`config`的第32~47位。 | 16 bit | [0, 2^16−1] |
| m_dst_start | 输出矩阵在M维的起始位置，单位为分形，用于控制首个repeat输出矩阵在M维的写入起点。占用`config`的高16位（第48~63位）。 | 16 bit | [0, 2^16−1] |

未列出的位（第25~31位）为保留位，调用方拼装`config`时应填0，避免改变默认行为。

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
