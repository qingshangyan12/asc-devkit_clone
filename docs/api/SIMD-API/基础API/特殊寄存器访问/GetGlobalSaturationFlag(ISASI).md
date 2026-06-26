# GetGlobalSaturationFlag(ISASI)

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

头文件路径为：`"basic_api/kernel_operator_common_intf.h"`。

读取饱和模式的全局生效标志位CTRL[60]。

## 函数原型

```cpp
__aicore__ inline bool GetGlobalSaturationFlag()
```

## 参数说明

无

## 返回值说明

返回true表示CTRL[60]为1，开启全局设置饱和；返回false表示CTRL[60]为0，使用单指令设置饱和。

## 约束说明

仅支持Ascend 950PR/Ascend 950DT。

## 调用示例

如下示例中查询是否开启饱和模式全局生效。

```cpp
bool res = AscendC::GetGlobalSaturationFlag();
```
