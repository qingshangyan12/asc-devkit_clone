# GetBitCount<a name="ZH-CN_TOPIC_0000001787630010"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
| ---- | -------- |
| <cann-filter npu-type = "950">Ascend 950PR/Ascend 950DT | √</cann-filter> |
| <cann-filter npu-type = "A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √</cann-filter> |
| <cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √</cann-filter> |
| <cann-filter npu-type = "310b">Atlas 200I/500 A2 推理产品 | x</cann-filter> |
| <cann-filter npu-type = "310p">Atlas 推理系列产品AI Core | √</cann-filter> |
| <cann-filter npu-type = "310p">Atlas 推理系列产品Vector Core | x</cann-filter> |
| <cann-filter npu-type = "910">Atlas 训练系列产品 | x</cann-filter> |
| <cann-filter npu-type = "x90">Kirin X90 | √</cann-filter> |
| <cann-filter npu-type = "9030">Kirin 9030 | √</cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为："basic\_api/kernel\_operator\_scalar\_intf.h"

统计uint64\_t类型数值的二进制表示中指定比特值（0或1）的出现次数。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <int countValue> 
__aicore__ inline int64_t GetBitCount(uint64_t valueIn)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| ------ | ---- |
| countValue | 指定需统计的比特值。取值为0或1。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | ----- | ----- |
| valueIn | 输入 | 被统计的二进制数字。 |

## 数据类型

支持的数据类型如下：uint64\_t。

## 返回值说明

int64\_t类型的数，valueIn中0或者1的个数。

## 约束说明<a name="section633mcpsimp"></a>

countValue取值为0或1。

## 调用示例<a name="section837496171220"></a>

```cpp
uint64_t valueIn = 0xffff;    // 二进制表示中有16个1，48个0

int64_t oneCount = AscendC::GetBitCount<1>(0xFFFF); // 统计valueIn二进制格式中1的个数，二进制格式中有16个1

int64_t zeroCount = AscendC::GetBitCount<0>(0xFFFF); // 统计valueIn二进制格式中0的个数，二进制格式中有48个0
```
