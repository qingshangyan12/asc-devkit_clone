# asc_set_l0c2gm_quant_post

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

设置Fixpipe后级量化（post-stage quantization）参数，将64 bit立即数`config`原样写入后级量化配置寄存器（REQSCALE）。该配置寄存器由后续调用的L0C Buffer搬出接口（将L0C Buffer中的矩阵累加结果搬运到OUT/L1 Buffer/UB的Fixpipe通路）共享读取，用于在搬运路径上执行反量化、量化后处理等定点转换。`config`的字段分布承载后级量化乘子M3/M4与各输出数据类型对应的偏移量（offset），具体字段定义见参数说明章节。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

随路量化模式下，本接口用于配置L0C Buffer搬出接口的后级量化参数，需通过[asc_set_l0c2gm_config](../cube_compute/asc_set_l0c2gm_config.md)、[asc_set_l0c_copy_prequant](asc_set_l0c_copy_prequant.md)预先配置前置量化与量化门控使能；随路激活模式下，需通过[asc_set_l0c2gm_relu_alpha](asc_set_l0c2gm_relu_alpha.md)、[asc_set_l0c2gm_lrelu_alpha](asc_set_l0c2gm_lrelu_alpha.md)预先配置激活参数；通道与NZ2DN场景下，需通过[asc_set_l0c2gm_channel_para](asc_set_l0c2gm_channel_para.md)、[asc_set_l0c2gm_nz2nd](../cube_compute/asc_set_l0c2gm_nz2nd.md)预先配置通道参数与NZ2DN转换参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c2gm_quant_post(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | Fixpipe后级量化参数配置立即数，类型为`uint64_t`，与后级量化配置寄存器位宽一致（64 bit），无截断风险，整个64 bit立即数原样写入寄存器。调用方需按所选后级量化模式对应的字段定义拼装64 bit `config`，字段分布见下表。配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口覆盖。取值范围 [0, 2^64−1]。 |

**表2** `config`字段拼装说明

`config`的字段分布承载后级量化乘子M3/M4与偏移量（offset），同一bit区域在不同后级量化模式下含义不同。M3/M4乘子按 (1, 8, 10) 浮点格式解读（1符号位 + 8指数位 + 10尾数位），不可为INF/NAN或非规格化数，否则后级量化结果错误。下表按字段列出所有可能的字段定义，调用方按所选后级量化模式对应字段拼装。

| 字段名 | 含义 | 位宽 | 取值范围 |
|---|---|---|---|
| M3乘子 | 后级量化M3乘子，硬件按 (1, 8, 10) 浮点格式解读（1符号位 + 8指数位 + 10尾数位），不可为INF/NAN或非规格化数，否则后级量化结果错误。占用`config`的第13~31位。 | 19 bit | [0, 2^19−1] |
| M4乘子 | 后级量化M4乘子，当随路激活模式将后级ReLU选为非normal ReLU模式时与M3协同生效，硬件按 (1, 8, 10) 浮点格式解读（1符号位 + 8指数位 + 10尾数位），不可为INF/NAN或非规格化数，否则后级量化结果错误。占用`config`的第13~31位（与M3共享同一位域，按后级ReLU模式选择生效）。 | 19 bit | [0, 2^19−1] |
| s4偏移量 | 后级量化输出为s4数据类型时的偏移量，5 bit有符号整数。占用`config`的低5位（第0~4位）。 | 5 bit | [0, 31] |
| b8偏移量 | 后级量化输出为b8数据类型时的偏移量，9 bit有符号整数，配合符号位（`config`第9位）共同指示结果符号。占用`config`的第0~8位。 | 9 bit | [0, 511] |
| b8偏移量符号位 | 后级量化输出为b8数据类型时偏移量的符号位。• 0：无符号；• 1：有符号。占用`config`的第9位。 | 1 bit | [0, 1] |
| s16/s32偏移量 | 后级量化输出为s16或s32数据类型时的偏移量，17 bit有符号整数，由高8位（`config`第32~40位）与低9位（`config`第0~8位）拼接组成。占用`config`的第0~8位与第32~40位。 | 17 bit | [0, 2^17−1] |

未列出的位为保留位，调用方拼装`config`时应填0，避免改变默认行为。

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅写入后级量化参数配置寄存器，实际定点转换由后续调用的L0C Buffer搬出接口在搬运路径上执行，须先调用本接口写入配置，再调用对应L0C Buffer搬出接口。
- 配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口覆盖配置；不同搬出任务间若需不同后级量化参数，应在调用L0C Buffer搬出接口前重新调用本接口设置。
- 本接口与配套setter接口协同配置L0C Buffer搬出接口的完整随路能力参数集，调用L0C Buffer搬出接口前须完成全部相关配套setter的配置（配套setter名见功能说明章节）。
- M3乘子字段（`config`第13~31位）与M4乘子字段（与M3共享同一位域）按`(1,8,10)`浮点格式解读，不可为INF/NAN或非规格化数，否则后级量化结果错误。
- `config`参数类型为`uint64_t`，与后级量化配置寄存器位宽一致（64bit），整个64bit立即数原样写入寄存器，无字段截断风险。
- 后级量化配置寄存器为只写类型，本接口仅写不读，调用方无法通过本接口确认写入是否生效。
- 后级量化功能（Quant POST）在当前芯片上为保留功能，调用本接口写入的配置不会在L0C Buffer搬出接口的随路后级量化阶段生效；调用方在当前芯片下不应依赖本接口产生实际后级量化效果，`config`字段拼装仅供接口契约完整性保留。

## 调用示例
```cpp
const uint64_t MASK_8_0 = 0x1FF;
const uint64_t MASK_BIT9 = 0x200;
const uint64_t MASK_31_13 = 0xFFFFE000;

uint64_t field_8_0 = 0x1AB;
uint64_t bit_9 = 1;
uint64_t field_31_13 = 0x000EF;

uint64_t config = 0;

config = (config & ~MASK_8_0) | (field_8_0 & MASK_8_0);
config = (config & ~MASK_BIT9) | ((bit_9<<9) & MASK_BIT9);
config = (config & ~MASK_31_13) | (field_31_13 & MASK_31_13);
asc_set_l0c2gm_quant_post(config);
```
