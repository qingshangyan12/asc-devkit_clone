# asc_get_status

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

读取当前核上下文中的64bit状态寄存器并按`int64_t`返回。该寄存器是64bit状态寄存器，记录核运行过程中各类告警事件的发生情况，各状态位一旦因告警行为置位即保持，直到复位或被读取清除。

## 函数原型

```c
__aicore__ inline int64_t asc_get_status()
```

## 参数说明

无

## 返回值说明

状态信息。各bit含义如下：
**表1** 状态寄存器比特位说明

| bit范围    | 含义 |
| ----------- |:----|
| 5 | 浮点运算溢出。SIMD指令int16_t和int32_t算术运算溢出也会上报到该位。 |
| 6 | 浮点运算下溢（结果浮点数小于非规格化数能表示的最小值，则结果为0）。 |
| 7 | 将任意浮点数转换为无符号整数时，输入为负数。 |
| 8 | 从L0C到UB的数据搬运过程中发生溢出（float->half、int32_t->half）。 |
| 9 | 从L0C到UB的数据搬运过程中发生下溢（float->half）。 |
| 10 | CUBE累加运算溢出（可能是float、half、int32_t）。 |
| 11 | CUBE累加运算下溢（可能是float、half）。 |
| 13 | 标量指令输入为NaN/INF。 |
| 14 | 向量指令输入为NaN/INF。 |
| 15 | CUBE指令输入为NaN/INF。 |
| 61 | 数据搬运类指令输入为NaN/INF |
| 其它bit位 | 保留位。 |

## 流水类型

PIPE_S

## 约束说明

- 本接口为只读查询接口，仅读取状态寄存器当前值，可在AIC与AIV上下文中调用。
- 状态寄存器位域承载多类告警语义，调用方应按状态寄存器位域定义逐bit解析返回值，不可将整体值作为单一数值使用。
- 本接口不支持纯SIMT编译模式，在该模式下包含`sys_var.h`头文件会触发编译错误。

## 调用示例

```cpp
int64_t status = asc_get_status();
printf("status is %x", status);// 需用%x将其打印成十六进制的数
```
