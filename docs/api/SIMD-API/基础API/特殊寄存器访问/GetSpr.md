# GetSpr

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | x </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品AI Core | x </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | x </cann-filter>|

## 功能说明

获取指定特殊寄存器的值，当前仅为预留接口。

## 函数原型

```cpp
template <SpecialPurposeReg spr>
__aicore__ inline int64_t GetSpr()
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| spr | 特殊寄存器，类型为SpecialPurposeReg枚举类，具体的取值请参考[表2](#table37531617424)，当前暂不支持任何SpecialPurposeReg类型的取值。 |

**表 2**  SpecialPurposeReg模板参数说明<a id='table37531617424'></a>

| 取值 | 含义 |
| --- | --- |
| AR | 一个特殊的地址寄存器，通常配合[Squeeze](../Reg矢量计算/比较与选择/Squeeze.md)Reg矢量计算API一起使用，[Squeeze](../Reg矢量计算/比较与选择/Squeeze.md)Reg矢量计算API会存储有效元素的总字节数到AR寄存器。 |

## 返回值说明

返回int64\_t类型的特殊寄存器中的数值。

## 约束说明

无

## 调用示例

预留接口，无调用示例。
