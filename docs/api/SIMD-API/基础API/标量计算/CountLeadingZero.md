# CountLeadingZero<a name="ZH-CN_TOPIC_0000001834229569"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
| ---- | -------- |
| <cann-filter npu_type = "950"><term>Ascend 950PR/Ascend 950DT<term>                  | √</cann-filter> |
| <cann-filter npu_type = "A3"><term>Atlas A3 训练系列产品/Atlas A3 推理系列产品<term>   | √</cann-filter> |
| <cann-filter npu_type = "910b"><term>Atlas A2 训练系列产品/Atlas A2 推理系列产品<term> | √</cann-filter> |
| <cann-filter npu_type = "310b"><term>Atlas 200I/500 A2 推理产品<term> | x</cann-filter> |
| <cann-filter npu_type = "310p"><term>Atlas 推理系列产品AI Core<term> | √</cann-filter> |
| <cann-filter npu_type = "310p"><term>Atlas 推理系列产品Vector Core<term> | x</cann-filter> |
| <cann-filter npu_type = "910"><term>Atlas 训练系列产品<term> | x</cann-filter> |
| <cann-filter npu_type = "x90"><term>Kirin X90<term> | √</cann-filter> |
| <cann-filter npu_type = "9030"><term>Kirin 9030<term> | √</cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件为： \#include "basic\_api/kernel\_operator\_scalar\_intf.h"

统计uint64\_t类型数值的二进制表示中前导0的数量（二进制表示中最高有效位到第一个'1'的'0'的数量，输入为0时返回64）。

以下CountLeadingZero的调用结果如下图所示：

```cpp
int64_t leadingZeros = AscendC::CountLeadingZero(0x0100000000000020ULL); // 返回7
```

**图 1**  CountLeadingZero调用示意图<a name="fig1"></a>  
![CountLeadingZero调用示意图](../../../figures/CountLeadingZero调用示意图.png "CountLeadingZero调用示意图")

## 函数原型<a name="section620mcpsimp"></a>

```cpp
__aicore__ inline int64_t CountLeadingZero(uint64_t valueIn)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  参数说明

| 参数说明 | 输入/输出 | 描述 |
| ------ | ------ | ------ |
| valueIn | 输入 | 被统计的二进制数字。 |

## 数据类型

支持uint64\_t。

## 返回值说明<a name="section640mcpsimp"></a>

int64\_t类型的数，valueIn的前导0的个数。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section837496171220"></a>

```cpp
int64_t leadingZeros;
uint64_t valueIn = 0x000000000000FFFFULL;          // 二进制表示有48个前导0
leadingZeros = AscendC::CountLeadingZero(valueIn); // 返回48

valueIn = 0;                                       // 二进制表示全为0
leadingZeros = AscendC::CountLeadingZero(valueIn); // 返回64

valueIn = 0xFFFFFFFFFFFFFFFFULL;                   // 二进制表示全为1
leadingZeros = AscendC::CountLeadingZero(valueIn); // 返回0
```
