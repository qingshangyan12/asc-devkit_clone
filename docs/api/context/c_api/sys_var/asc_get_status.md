# asc_get_status

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|<cann-filter npu_type = "950"> Ascend 950PR/Ascend 950DT | √</cann-filter> |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

获取状态信息。

## 函数原型

```cpp
__aicore__ inline int64_t asc_get_status()
```

## 参数说明

无

## 返回值说明

状态信息。各bit含义如下：
| bit范围    | 含义 |
| ----------- |:----|
| 0 | 指令缓存奇偶校验错误，取指单元应重新取当前缓存行。 |
| 1 | 指令缓存多命中错误，取指单元应重新取当前缓存行。 |
| 2 | UB、L1、L0A、L0B、L0C、重排序缓冲区、tlu缓冲区、标量缓冲区、数据缓冲、向量发射队列ECC单比特错误。 |
| 3 | NEG指令输入为最小负数。 |
| 4 | 整数ADD、MUL、SUB和MADD运算溢出。 |
| 5 | 浮点运算溢出。SIMD指令s16和s32算术运算溢出也会上报到该位。 |
| 6 | 浮点运算下溢（结果浮点数小于非规格化数能表示的最小值，则结果为0）。 |
| 7 | 将任意浮点数转换为无符号整数时，输入为负数。 |
| 8 | 从L0C到UB的数据搬运过程中发生溢出（F32->F16、s32->F16）。 |
| 9 | 从L0C到UB的数据搬运过程中发生下溢（F32->F16）。 |
| 10 | CUBE累加运算溢出（可能是F32、F16、s32）。 |
| 11 | CUBE累加运算下溢（可能是F32、F16）。 |
| 12 | AIPP FP16计算溢出。 |
| 13 | 标量指令输入为NaN/INF。 |
| 14 | 向量指令输入为NaN/INF。 |
| 15 | CUBE指令输入为NaN/INF。 |
| 23:16 | 标量指令的STATUS[3]、STATUS[4]、STATUS[5]、STATUS[6]、STATUS[13]或AIPP/MTE指令的STATUS[12]、STATUS[61]最后发出告警时的PC[3:2]地址。 |
| 31:24 | 向量指令的STATUS[5]、STATUS[6]、STATUS[7]、STATUS[8]、STATUS[9]、STATUS[14]最后发出告警时的PC[5:2]地址。 |
| 39:32 | CUBE指令的STATUS[10]、STATUS[11]、STATUS[15]最后发出告警时的PC[10:2]地址。 |
| 47:40 | 向量运算中最后发生告警的重复次数 |
| 51:48 | 奇偶校验错误次数，当错误发生超过15次时，该计数器会饱和。 |
| 55:52 | 多命中错误次数，当错误发生超过15次时，该计数器会饱和。 |
| 59:56 | ECC单比特错误次数，当错误发生超过15次时，该计数器会饱和。 |
| 60 | INSERT指令的N+K大于63 |
| 61 | MTE指令输入为NaN/INF |
| 63:62 | 保留位。 |

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
int64_t smmu_tag_version = asc_get_status();
printf("smmu tag version is %x", smmu_tag_version);// 需用%x将其打印成十六进制的数
```