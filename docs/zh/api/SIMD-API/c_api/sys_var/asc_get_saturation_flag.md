# asc_get_saturation_flag

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

按饱和模式类型读取当前是否开启饱和，并返回布尔类型的结果。

## 函数原型

```cpp
__aicore__ inline bool asc_get_saturation_flag(asc_saturation_mode saturation_mode)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| saturation_mode | 输入 | 饱和模式类型，取值见[asc_saturation_mode](../enum/asc_saturation_mode.md)。 |

## 返回值说明

`true`表示当前该模式下饱和已开启；`false`表示未开启（对INT为截断；对FLOAT/FLOAT8/CAST为非饱和或截断）。

## 流水类型

PIPE_S

## 约束说明

- 返回值是语义层结果，不能直接当作CTRL原始比特值使用；若需要原始寄存器值，请使用[asc_get_ctrl](asc_get_ctrl.md)。
- 对`CAST`模式，即使本接口返回`true`，也需同时满足饱和策略为`USE_GLOBAL`，CAST饱和配置才会在精度转换中真正生效。详见[asc_set_saturation_strategy](asc_set_saturation_strategy.md)。
- 各模式对应的CTRL比特位与极性说明见[asc_set_saturation_flag](asc_set_saturation_flag.md)。

## 调用示例

场景：进入float转int32量化逻辑前，先查询当前CAST饱和是否已开启。若未开启则临时打开并完成转换，转换结束后恢复原状态，避免影响后续其他计算路径。

```cpp
float src = 3.0e9f;  // 可能超出int32正数范围的浮点值
int32_t dst;

// 查询并保存当前CAST饱和状态
bool old_sat = asc_get_saturation_flag(asc_saturation_mode::CAST);

// 保证CAST全局策略生效；若尚未开启饱和则打开
asc_set_saturation_strategy(asc_override_strategy::USE_GLOBAL);
if (!old_sat) {
    asc_set_saturation_flag(asc_saturation_mode::CAST, true);
}

// 浮点转整数：超出范围时按饱和模式处理
dst = asc_float2int32_rn(src);

// 恢复进入本段逻辑前的CAST饱和状态
asc_set_saturation_flag(asc_saturation_mode::CAST, old_sat);
```
