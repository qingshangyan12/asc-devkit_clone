# asc_set_l0c2gm_config

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

本接口用于设置L0C Buffer搬出场景下随路tensor量化、随路tensor激活、UnitFlag功能的相关配置，具体功能说明如下：

- 功能一：设置随路tensor量化参数的起始地址索引。当L0C Buffer搬出接口启用tensor量化模式时，硬件将从Fixpipe Buffer中按本接口设置的起始地址索引读取N维量化参数，N代表搬出矩阵的列数。
- 功能二：设置随路tensor激活参数的起始地址索引。当L0C Buffer搬出接口启用tensor激活模式时，硬件将从Fixpipe Buffer中按本接口设置的起始地址索引读取N维激活参数，N代表搬出矩阵的列数。
- 功能三：UnitFlag全局状态初始化：当需要关闭L0C Buffer搬出接口的UnitFlag功能，或矩阵乘加计算与搬出的数据量不一致时，本接口可以将UnitFlag功能的全局状态设置为关闭。

本接口设置的配置写入后对后续[asc_copy_l0c2gm](../cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)等所有L0C Buffer搬出接口生效，直至再次调用本接口修改配置。

## 函数原型

```c
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre,
                                             uint64_t quant_pre,
                                             bool enable_unit_flag)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| relu_pre | 输入 | tensor激活参数在Fixpipe Buffer中的起始地址索引，单位为64字节，取值范围为[0, 255]。该参数仅低8 bit有效。tensor激活参数的地址需64字节对齐，地址索引的计算方式为：Fixpipe Buffer物理地址/64。仅当后续L0C Buffer搬出接口选择tensor激活模式时，本参数生效。 |
| quant_pre | 输入 | tensor量化参数在Fixpipe Buffer中的起始地址索引，单位为128字节，取值范围为[0, 255]。该参数仅低8 bit有效。tensor量化参数的地址需128字节对齐，地址索引的计算方式为：Fixpipe Buffer物理地址/128。仅当后续L0C Buffer搬出接口选择tensor量化模式时，本参数生效。 |
| enable_unit_flag | 输入 | 是否清零L0C Buffer所有存储块的unitFlag。取值为true时清零，取值为false时保持当前状态。本参数是一次性的清零控制，不用于开启后续搬出接口的unitFlag功能。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- tensor激活参数需先搬运到Fixpipe Buffer。调用本接口后，需在后续搬出接口读取激活参数前完成PIPE_FIX流水同步。`relu_pre`超出取值范围时仅低8 bit生效，将导致硬件从错误的地址读取量化参数。
- tensor量化参数需先搬运到Fixpipe Buffer。调用本接口后，需在后续搬出接口读取量化参数前完成PIPE_FIX流水同步。`quant_pre`超出取值范围时仅低8 bit生效，将导致硬件从错误的地址读取量化参数。
- 本接口的配置会持续生效，直至再次调用本接口覆盖。不同搬出任务使用的配置不同时，需在对应搬出接口执行前重新配置；每次调用均会同时覆盖三个参数。
- `enable_unit_flag`用于消除L0C Buffer中残留unitFlag对后续任务的影响。是否使用unitFlag搬出数据，由后续搬出接口的unitFlag相关参数控制。

## 调用示例

```cpp
// quant_fb为已写入tensor量化参数且按128字节对齐的Fixpipe Buffer地址
uint64_t quant_pre = reinterpret_cast<uintptr_t>(quant_fb) / 128;
asc_set_l0c2gm_config(0, quant_pre, false);
asc_sync_pipe(PIPE_FIX);
```
