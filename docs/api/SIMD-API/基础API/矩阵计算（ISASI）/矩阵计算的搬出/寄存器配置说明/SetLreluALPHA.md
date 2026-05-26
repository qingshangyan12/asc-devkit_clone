# SetLreluALPHA<a name="ZH-CN_TOPIC_0000002568950987"></a>

## 产品支持情况<a name="zh-cn_topic_0000002547689123_section18204144912492"></a>

|产品|是否支持|
|----------|:----------:|
|Ascend 950PR/Ascend 950DT|x|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|√|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|√|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Kirin X90|x|
|Kirin  9030|x|

## 功能说明<a name="zh-cn_topic_0000002547689123_section618mcpsimp"></a>

DataCopy数据搬运（[L0C-\>GM](../L0C到GM数据搬运.md)、[L0C-\>L1](../L0C到L1数据搬运.md)）过程中设置随路Leaky ReLU alpha值。

## 函数原型<a name="zh-cn_topic_0000002547689123_section620mcpsimp"></a>

```cpp
template<template T>
__aicore__ inline void SetLreluALPHA(T config)
```

## 参数说明<a name="zh-cn_topic_0000002547689123_section622mcpsimp"></a>

**表 1**  参数说明

|参数名称|输入/输出|含义|
|----------|----------|----------|
|T|输入|随路Leaky ReLU alpha值的数据类型，只支持half、float|
|config|输入|随路Leaky ReLU alpha值|

## 数据类型

支持的数据类型为：half、float。

## 返回值说明

无

## 约束说明<a name="zh-cn_topic_0000002547689123_section633mcpsimp"></a>

- 随路Leaky ReLU alpha值的数据类型，只支持half、float。

> [!NOTE]说明
> 该接口非正式对外接口，正式接口待补充

## 调用示例

无
