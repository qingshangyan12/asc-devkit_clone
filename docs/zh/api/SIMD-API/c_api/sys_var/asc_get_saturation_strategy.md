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

读取当前饱和控制策略，对应CTRL[60]：

- CTRL[60]=0时，返回`asc_override_strategy::USE_API`。
- CTRL[60]=1时，返回`asc_override_strategy::USE_GLOBAL`。

常用于临时切换策略前保存原配置，或与[asc_set_saturation_strategy](asc_set_saturation_strategy.md)配合做回读校验。

## 函数原型

```cpp
__aicore__ inline asc_override_strategy asc_get_saturation_strategy()
```

## 参数说明

无

## 返回值说明

当前饱和控制策略，类型为[asc_override_strategy](../enum/asc_override_strategy.md)。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

场景：进入float转int32量化前，先查询当前饱和策略。若不是全局策略，则临时切到`USE_GLOBAL`并开启CAST饱和，转换结束后恢复原策略。

```cpp
float src = 3.0e9f;  // 可能超出int32正数范围的浮点值
int32_t dst;

// 查询并保存当前饱和策略
asc_override_strategy old_strategy = asc_get_saturation_strategy();

// CAST饱和仅在USE_GLOBAL时生效，必要时切换策略并打开开关
if (old_strategy != asc_override_strategy::USE_GLOBAL) {
    asc_set_saturation_strategy(asc_override_strategy::USE_GLOBAL);
}
asc_set_saturation_flag(asc_saturation_mode::CAST, true);

// 浮点转整数：超出范围时按饱和模式处理
dst = asc_float2int32_rn(src);

// 恢复进入本段逻辑前的饱和策略
asc_set_saturation_strategy(old_strategy);
```
