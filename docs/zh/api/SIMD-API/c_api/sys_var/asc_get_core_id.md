# asc_get_core_id

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

获取当前核的核ID（COREID），用于在系统调试或链路追踪场景下区分同一进程内不同的核。COREID是一个16 bit只读特殊寄存器，由系统控制在kernel启动前配置，kernel运行期间不期望被修改。本接口无参数，固定读取当前核上下文中的COREID寄存器值并按`int64_t`返回。

## 函数原型

```c
__aicore__ inline int64_t asc_get_core_id()
```

## 参数说明

无

## 返回值说明

返回`int64_t`类型的当前核的核ID。取值范围[0, 2^16−1]（COREID寄存器宽度为16 bit，硬件保证返回值落在该范围内）。

## 流水类型

PIPE_S

## 约束说明

- 本接口为只读查询接口，不修改任何寄存器或存储状态，可在AIC与AIV上下文中调用。
- 返回值由系统控制在kernel启动前配置完成，kernel运行期间不应依赖返回值发生变化。
- 返回值落在16 bit范围内，按`int64_t`返回时高48位为0，调用方无需额外掩码处理。
- 本接口读取的是COREID寄存器（核级ID），与子块ID、Block ID等其他层级标识不同，不可混用。

## 调用示例

无