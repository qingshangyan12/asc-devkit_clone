# asc_set_l0c2gm_relu_alpha

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

设置L0C Buffer搬出场景下随路Scalar ReLU激活函数的alpha乘子。配置写入后作用于后续调用的L0C Buffer搬出接口（将L0C Buffer中的矩阵累加结果搬运到OUT/L1 Buffer/UB的Fixpipe通路）的随路激活阶段，作为Scalar ReLU计算的alpha乘子：当输入大于等于0时输出等于输入本身；当输入小于0时输出按`alpha × 输入`计算。本接口按`uint64_t`整体写入64 bit配置寄存器，调用方须按alpha乘子的字段定义自行拼装64 bit`config`后传入。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

随路激活模式下，本接口用于配置L0C Buffer搬出接口的Scalar ReLU激活alpha乘子，需通过[asc_set_l0c2gm_lrelu_alpha](asc_set_l0c2gm_lrelu_alpha.md)预先配置随路LReLU激活的alpha乘子；随路量化模式下，需通过[asc_set_l0c2gm_config](../cube_compute/asc_set_l0c2gm_config.md)、[asc_set_l0c2gm_quant_post](asc_set_l0c2gm_quant_post.md)、[asc_set_l0c_copy_prequant](asc_set_l0c_copy_prequant.md)预先配置量化使能与前后级量化参数；通道与NZ2DN场景下，需通过[asc_set_l0c2gm_channel_para](asc_set_l0c2gm_channel_para.md)、[asc_set_l0c2gm_nz2nd](../cube_compute/asc_set_l0c2gm_nz2nd.md)预先配置通道参数与NZ2DN转换参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | 待写入配置寄存器的64 bit立即数，类型为`uint64_t`，与配置寄存器位宽一致（64 bit），无字段截断风险，整个64 bit立即数原样写入寄存器。调用方须按`config`字段拼装说明（表2）的位域定义自行拼装后整体传入。取值范围[0, 2^64−1]。 |

**表2** `config`字段拼装说明

`config`的字段分布承载Scalar ReLU的前级alpha乘子M2与后级alpha乘子M4，硬件按(1, 8, 10)浮点格式解读（1符号位 + 8指数位 + 10尾数位），不可为INF/NAN。

| 字段名 | 含义 | 位宽 | 取值范围 |
|---|---|---|---|
| M2 alpha乘子 | Scalar ReLU前级（ReLU PRE）的alpha乘子M2，当L0C Buffer搬出接口的随路激活模式将前级ReLU选为Scalar ReLU模式时生效。硬件按(1, 8, 10)浮点格式解读（1符号位 + 8指数位 + 10尾数位），不可为INF/NAN，否则随路激活结果错误。占用`config`的第13~31位。 | 19 bit | [0, 2^19−1] |
| M4 alpha乘子 | Scalar ReLU后级（ReLU POST）的alpha乘子M4，当L0C Buffer搬出接口的随路激活模式将后级ReLU选为Scalar ReLU模式时生效。硬件按(1, 8, 10)浮点格式解读（1符号位 + 8指数位 + 10尾数位），不可为INF/NAN，否则随路激活结果错误。占用`config`的第45~63位。 | 19 bit | [0, 2^19−1] |

未列出的位为保留位，调用方拼装`config`时应填0，避免改变默认行为。

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅写入随路Scalar ReLU的alpha乘子配置，实际随路激活计算由后续调用的L0C Buffer搬出接口在搬运路径上执行，须先调用本接口写入配置，再调用对应L0C Buffer搬出接口。
- 配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口覆盖配置；不同搬出任务间若需不同alpha乘子，应在调用L0C Buffer搬出接口前重新调用本接口设置。
- 本接口与配套setter接口协同配置L0C Buffer搬出接口的完整随路能力参数集，调用L0C Buffer搬出接口前须完成全部相关配套setter的配置（配套setter名见功能说明章节）。
- M2 alpha乘子字段（`config`第13~31位）与M4 alpha乘子字段（`config`第45~63位）按`(1,8,10)`浮点格式解读，不可为INF/NAN或非规格化数，否则随路激活结果错误。
- `config`参数类型为`uint64_t`，与配置寄存器位宽一致（64bit），整个64bit立即数原样写入寄存器，无字段截断风险。
- 本接口写入的配置寄存器与[asc_set_l0c2gm_lrelu_alpha](asc_set_l0c2gm_lrelu_alpha.md)指向同一物理配置项，二者为同一配置的不同命名入口，调用任一接口设置即可，无需重复设置；若两次调用设置不同值，后写入的值覆盖先写入的值。
- 配置寄存器为只写类型，本接口仅写不读，调用方无法通过本接口确认写入是否生效。

## 调用示例

```cpp
uint32_t pre_m2 = (0<<18)|(127<<10)|0;
uint32_t post_m2 = (0<<18)|(124<<10)|0;
uint64_t config = ((uint64_t)pre_m2<<13)|((uint64_t)post_m2<<45);
asc_set_l0c2gm_relu_alpha(config);
```
