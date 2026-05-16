# GetSFFValue<a name="ZH-CN_TOPIC_0000001787630014"></a>

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

查找uint64\_t类型数值的二进制表示中从最低有效位开始的首个指定比特值（0或1）的位置，如果没找到则返回-1。

以下GetSFFValue的调用结果如下图所示：

```cpp
int64_t sffValue = AscendC::GetSFFValue<1>(0x1000000000000010ULL); // 返回4
```

**图 1**  使用GetSFFValue查找从最低有效位开始首个1的位置<a name="fig1"></a>
![使用GetSFFValue查找从最低有效位开始首个1的位置](../../../figures/使用GetSFFValue查找从最低有效位开始首个1的位置.png "使用GetSFFValue查找从最低有效位开始首个1的位置")

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <int countValue>
__aicore__ inline int64_t GetSFFValue(uint64_t valueIn)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数说明 | 描述 |
| ------ | ------ |
| countValue| 指定要查找的比特值，取值为0或1。 |

**表 2**  参数说明

| 参数说明 | 输入/输出 | 描述 |
| ------ | ------ | ------ |
| valueIn | 输入 | 被查找的二进制数字。 |

## 数据类型

支持uint64\_t。

## 返回值说明<a name="section640mcpsimp"></a>

int64\_t类型的数，valueIn中第一个0或1出现的位置。

## 约束说明<a name="section633mcpsimp"></a>

countValue取值为0或1。

## 调用示例<a name="section837496171220"></a>

```cpp
int64_t sffValue;
uint64_t valueIn = 0xFFFFFFFFFFFFFFFFULL;    // 二进制表示全为1
sffValue = AscendC::GetSFFValue<0>(valueIn); // 返回-1

valueIn = 0;                                 // 二进制表示全为0
sffValue = AscendC::GetSFFValue<1>(valueIn); // 返回-1

valueIn = 0xFF00ULL;
sffValue = AscendC::GetSFFValue<0>(valueIn); // 返回0
sffValue = AscendC::GetSFFValue<1>(valueIn); // 返回8

valueIn = 0x00FFULL;
sffValue = AscendC::GetSFFValue<0>(valueIn); // 返回8
sffValue = AscendC::GetSFFValue<1>(valueIn); // 返回0
```
