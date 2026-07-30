# asc_set_saturation_strategy

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
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

头文件路径：`"c_api/sys_var/sys_var.h"`。

设置饱和模式的覆盖策略。通过设置CTRL寄存器的CTRL[60]比特位来控制饱和模式的使用来源。

## 函数原型

```cpp
__aicore__ inline void asc_set_saturation_strategy(asc_override_strategy strategy)
```

## 参数说明

表1 参数说明

|参数名|输入/输出|描述|
| :------ | :---  | :------------ |
|strategy   |输入   |覆盖策略，取值为`asc_override_strategy::USE_API`或`asc_override_strategy::USE_GLOBAL`。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 设置为`USE_API`时，CTRL[60]置0，使用API级饱和配置生效。
- 设置为`USE_GLOBAL`时，CTRL[60]置1，使用全局饱和配置生效。

## 调用示例

```cpp
asc_set_saturation_strategy(asc_override_strategy::USE_API);
```
