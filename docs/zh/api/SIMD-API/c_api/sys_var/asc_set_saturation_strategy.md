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

设置饱和控制策略，对应CTRL[60]：

- `asc_override_strategy::USE_API`：表示按单指令/API路径配置饱和。
- `asc_override_strategy::USE_GLOBAL`：表示使用全局CTRL饱和配置。

该策略尤其影响CAST类饱和：只有在`USE_GLOBAL`时，通过[asc_set_saturation_flag](asc_set_saturation_flag.md)配置的`asc_saturation_mode::CAST`才会生效。枚举含义详见[asc_override_strategy](../enum/asc_override_strategy.md)。

## 函数原型

```cpp
__aicore__ inline void asc_set_saturation_strategy(asc_override_strategy strategy)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| strategy | 输入 | 饱和控制策略，取值见[asc_override_strategy](../enum/asc_override_strategy.md)。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 当策略为`USE_API`时，全局CAST饱和位不生效；若需要CAST全局饱和，必须先或同时将策略设置为`USE_GLOBAL`。
- 切换策略不会自动修改FLOAT/FLOAT8/INT/CAST各自的饱和开关位。

## 调用示例

场景：算子中将浮点激活值量化为int32时，源值可能超出int32表示范围。先将饱和策略切到`USE_GLOBAL`，再开启CAST饱和，然后调用[asc_float2int32](../scalar_compute/asc_float2int32.md)做转换。

```cpp
float src = 3.0e9f;  // 可能超出int32正数范围的浮点值
int32_t dst;

// 切到全局饱和策略
asc_set_saturation_strategy(asc_override_strategy::USE_GLOBAL);
// 开启CAST饱和
asc_set_saturation_flag(asc_saturation_mode::CAST, true);

// 浮点转整数：超出范围时按饱和模式处理
dst = asc_float2int32_rn(src);
```
