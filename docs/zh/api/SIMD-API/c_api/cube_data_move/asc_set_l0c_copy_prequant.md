# asc_set_l0c_copy_prequant

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

设置Fixpipe前级量化（pre-stage quantization）参数，将64 bit立即数`config`原样写入前级量化配置寄存器。该配置寄存器由后续调用的L0C Buffer数据搬运接口（Fixpipe通路，将L0C Buffer中的矩阵累加结果搬运到OUT/L1 Buffer/UB）共享读取，用于在搬运路径上执行反量化、量化前处理、F16乘法、位掩码等定点转换。`config`的字段分布随L0C Buffer数据搬运接口所选的前级量化模式（`quant_pre`模式）变化，调用方须按所选模式的字段定义拼装64 bit`config`，字段定义见参数说明章节。

随路量化模式下，前级量化参数需通过`asc_set_l0c_copy_prequant`预先配置，配置完成后调用L0C Buffer数据搬运接口执行反量化、量化前处理、F16乘法、位掩码等定点转换。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | Fixpipe前级量化参数配置立即数，类型为`uint64_t`，与前级量化配置寄存器位宽一致（64 bit），无截断风险。调用方需按所选`quant_pre`模式对应的字段定义拼装64 bit`config`，字段分布见下表。配置写入后对后续所有L0C Buffer数据搬运接口生效，直至再次调用本接口覆盖。 |

**表2** `config`字段拼装说明

`config`的字段分布随L0C Buffer数据搬运接口的`quant_pre`模式切换，同一bit区域在不同模式下含义不同。下表按字段列出所有可能的字段定义，调用方按所选`quant_pre`模式对应字段拼装。

| 字段名 | 含义 | 位宽 | 取值范围 |
|---|---|---|---|
| M1 scale | 前级量化M1乘子，硬件按 (1, 8, 10) 浮点格式解读（1符号位 + 8指数位 + 10尾数位），不可为INF/NAN或非规格化数，否则量化结果错误。占用`config`的第13~31位。 | 19 bit | [0, 2^19−1] |
| F16乘法因子 | float->half类前级量化模式下作为标准`half`数据乘子附加到L0C Buffer`half`数据上。占用`config`的低16位（第0~15位）。 | 16 bit | [0, 65535] |
| s17 offset高8bit | 对应前级量化模式下s17偏移量的高8位，与s17偏移量低9位（s9 offset字段）组合为完整s17偏移量。占用`config`的第0~7位。 | 8 bit | [0, 255] |
| s9 offset | 对应前级量化模式下的s9偏移量，或DEQ16模式下s17偏移量的低9位。占用`config`的第37~45位。 | 9 bit | [0, 511] |
| s5 offset | 对应前级量化模式下的s5偏移量。占用`config`的第37~41位。 | 5 bit | [0, 31] |
| deq符号位 | 对应前级量化模式下指示反量化结果是否带符号。<br>&nbsp;&nbsp;&bull; `0`：无符号；<br>&nbsp;&nbsp;&bull; `1`：有符号。占用`config`的第46位。 | 1 bit | [0, 1] |
| `BitMask` | 位掩码操作，仅b8和s16输出数据类型有效，其他输出dtype下该字段被忽略。<br>&nbsp;&nbsp;&bull; `0`：禁用位掩码；<br>&nbsp;&nbsp;&bull; `1`：掩码1个LSB至0；<br>&nbsp;&nbsp;&bull; `2`：掩码2个LSB至0；<br>&nbsp;&nbsp;&bull; `3`：掩码3个LSB至0；<br>&nbsp;&nbsp;&bull; `4`：掩码4个LSB至0；5~7为非法值，日志会记录告警，实际掩码行为未定义。占用`config`的第47~49位。 | 3 bit | [0, 7] |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

<!-- npu="950" id8 -->
### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅设置前级量化参数，实际定点转换由后续调用的L0C Buffer数据搬运接口在搬运路径上执行，须先调用本接口写入配置，再调用对应L0C Buffer数据搬运接口。
- 配置写入后对后续所有L0C Buffer数据搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同前级量化参数，应在调用L0C Buffer数据搬运接口前重新调用本接口设置。
- `config`字段分布随L0C Buffer数据搬运接口所选的`quant_pre`模式切换，调用方须按所选模式对应的字段定义拼装`config`，未使用的bit区域填0。
- M1 scale字段（`config`第13~31位）按`(1,8,10)`浮点格式解读，不可为INF/NAN或非规格化数，否则前级量化结果错误。
- BitMask字段（`config`第47~49位）仅b8和s16输出数据类型有效，其他输出dtype下该字段被硬件忽略；BitMask有效取值为0~4，5~7为非法值，会打印日志告警，实际掩码行为未定义。
- `config`参数类型为`uint64_t`，与前级量化配置寄存器位宽一致（64bit），无字段截断风险，整个64bit立即数原样写入配置寄存器。

<!-- end id8 -->

## 调用示例

```cpp
uint64_t config = 2;
asc_set_l0c_copy_prequant(config);
```
