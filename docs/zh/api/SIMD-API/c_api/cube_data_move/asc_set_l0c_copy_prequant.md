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

数据搬运过程中进行随路量化时，通过调用该接口设置量化流程中的标量量化参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config)
```

## 参数说明

**表1** 参数说明

|参数名|输入/输出|描述|
|------------|------------|-----------|
| config     | 输入     | 量化过程中使用到的标量量化参数。|

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
