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

对通道步长参数的专用寄存器的比特位进行设置。需要配合接口[asc_copy_l0c2l1](../cube_data_move/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md)或[asc_copy_l0c2gm](../cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)使用。
仅当调用接口时开启NZ2DN模式时，此功能才有效。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| config | 输入 | 设置的寄存器值。常用通道步长寄存器比特位说明参考表2。 |

**表2** 常用通道步长寄存器比特位说明

|CHANNEL_PARA比特位    |功能|
| :-------     | :---- |
| CHANNEL_PARA[48-0]    |  保留位，设置无效  |
| CHANNEL_PARA[63-48]    |  表示loop0源步长，单位为C0_SIZE。当开启NZ2DN模式时有效，且不能为0。  |

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
