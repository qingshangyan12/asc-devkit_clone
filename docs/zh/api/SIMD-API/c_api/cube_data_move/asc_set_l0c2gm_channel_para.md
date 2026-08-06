# asc_set_l0c2gm_channel_para

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

设置L0C Buffer搬出场景下Fixpipe通道参数配置寄存器，将64 bit立即数`config`原样写入配置寄存器。该配置寄存器由后续调用的L0C Buffer搬出接口（将L0C Buffer中的矩阵累加结果经Fixpipe通路搬运到OUT/L1 Buffer/UB）共享读取，用于在搬运路径上配置通道相关步长与通道指示信息。`config`的字段分布按L0C Buffer搬出接口所选的使能模式（循环增强模式、循环增强合并模式、NZ2DN转换模式、Winograd后处理模式等）切换，调用方须按所选模式的字段定义拼装64 bit`config`，字段定义见参数说明章节。

配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口覆盖配置。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

随路量化模式下需通过`asc_set_l0c2gm_quant_post`预先配置量化参数；随路激活（ReLU / LReLU）模式下需通过`asc_set_l0c2gm_relu_alpha`、`asc_set_l0c2gm_lrelu_alpha`预先配置激活系数；NZ2DN格式转换模式下需通过`asc_set_l0c2gm_nz2nd`预先配置转换参数；基础搬出配置需通过`asc_set_l0c2gm_config`预先配置。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config)
```

## 参数说明

**表1**参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | Fixpipe通道参数配置立即数，类型为`uint64_t`，与通道参数配置寄存器位宽一致（64 bit），无截断风险。调用方需按L0C Buffer搬出接口所选使能模式对应的字段定义拼装64 bit`config`，字段分布见下表。配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口覆盖。 |

**表2**`config`字段拼装说明

`config`的字段分布随L0C Buffer搬出接口的使能模式切换，同一bit区域在不同模式下含义不同。下表按字段列出所有可能的字段定义，调用方按所选使能模式对应字段拼装，未使用bit区域填0。

| 字段名 | 含义 | 位宽 | 取值范围 |
|---|---|---|---|
| C0通道步长 | 循环增强模式下相邻C0通道在源端（L0C Buffer）的步长，单位C0_size（C0_size = 16 × sizeof(src dtype)，src dtype为`float`/`int32_t`时为64字节，src dtype为`half`/`bfloat16_t`时为32字节）。占用`config`的低16位（第0~15位）。仅当L0C Buffer搬出接口开启循环增强模式时本字段生效；本字段解码后不可为0，否则触发异常。 | 16 bit | [0, 2^16−1] |
| 通道合并步长 | 循环增强合并模式下相邻通道合并块的步长，单位C0_size（同上公式）。占用`config`的第16~20位。仅当L0C Buffer搬出接口同时开启循环增强模式与通道合并使能时本字段生效。 | 5 bit | [0, 31] |
| C0通道指示位 | Winograd后处理场景下，L1 Buffer中b8源数据的C0通道位置指示。当L0C Buffer搬出接口开启S8/U8反量化且开启Winograd后处理时本字段生效。• 0：当前16通道b8源数据位于每个32字节单元的前16字节；• 1：位于每个32字节单元的后16字节。占用`config`的第32位。 | 1 bit | [0, 1] |
| loop0源端步长 | NZ2DN转换模式下loop0在源端（L0C Buffer）的步长，单位C0_size（同上公式）。占用`config`的第48~53位。仅当L0C Buffer搬出接口开启NZ2DN转换时本字段生效；本字段解码后不可为0，否则触发异常。 | 6 bit | [0, 63] |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅写入Fixpipe通道参数配置寄存器，实际通道步长/通道合并/通道指示/loop0源端步长计算由后续调用的L0C Buffer搬出接口在搬运路径上执行，须先调用本接口写入配置，再调用对应L0C Buffer搬出接口。
- 配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口覆盖配置；不同搬出任务间若需不同通道参数，应在调用L0C Buffer搬出接口前重新调用本接口设置。
- 本接口与配套setter接口协同配置L0C Buffer搬出接口的完整随路能力参数集，调用L0C Buffer搬出接口前须完成全部相关配套setter的配置（配套setter名见功能说明章节）。
- `config`参数类型为`uint64_t`，与通道参数配置寄存器位宽一致（64bit），无字段截断风险，整个64bit立即数原样写入配置寄存器。
- `config`字段分布随L0C Buffer搬出接口所选使能模式切换，调用方须按所选模式对应的字段定义拼装`config`，未使用bit区域填0。

### 字段生效与非法配置约束

- C0通道步长字段（`config`第0~15位）仅当L0C Buffer搬出接口开启循环增强模式时生效；本字段解码后不可为0，否则触发异常。
- 通道合并步长字段（`config`第16~20位）仅当L0C Buffer搬出接口同时开启循环增强模式与通道合并使能时生效，其余模式下该bit区域被忽略。
- C0通道指示位字段（`config`第32位）仅当L0C Buffer搬出接口同时开启S8/U8反量化与Winograd后处理时生效，其余模式下该bit区域被忽略。
- loop0源端步长字段（`config`第48~53位）仅当L0C Buffer搬出接口开启NZ2DN转换时生效；本字段解码后不可为0，否则触发异常。

## 调用示例

```cpp
uint64_t config = 0;
asc_set_l0c2gm_channel_para(config);
```
