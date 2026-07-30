# asc_get_saturation_strategy

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

获取饱和模式的覆盖策略。通过读取CTRL寄存器的CTRL[60]比特位来查询当前饱和模式的使用来源。

## 函数原型

```cpp
__aicore__ inline asc_override_strategy asc_get_saturation_strategy()
```

## 参数说明

无

## 返回值说明

返回`asc_override_strategy`类型。`USE_API`表示使用API级饱和配置，`USE_GLOBAL`表示使用全局饱和配置。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
asc_override_strategy strategy = asc_get_saturation_strategy();
```
