# asc_set_saturation_flag

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

按饱和模式类型设置CTRL寄存器上对应的饱和控制位。`enable_sat`表示API语义上的“是否开启饱和”，接口内部会按模式写入正确的比特极性，开发者无需手动区分不同模式的0/1含义。

各模式与CTRL比特位及`enable_sat=true`时的写入值如下：

**表1** 饱和模式与CTRL比特位映射

| saturation_mode | CTRL比特位 | enable_sat=true时写入 | enable_sat=false时写入 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| FLOAT | CTRL[48] | 0（饱和） | 1（非饱和） | 浮点计算/部分浮点精度转换场景的饱和控制。 |
| FLOAT8 | CTRL[50] | 0（饱和） | 1（非饱和） | 浮点8相关场景的饱和控制。 |
| INT | CTRL[53] | 1（饱和） | 0（截断） | 整数计算溢出时的饱和/截断控制。 |
| CAST | CTRL[59] | 0（饱和） | 1（截断） | 浮点转整数或整数转整数时的精度转换饱和控制。仅在饱和策略为`USE_GLOBAL`时生效。 |

CTRL寄存器更完整的比特位说明还可参考[asc_set_ctrl](asc_set_ctrl.md)。推荐优先使用本接口，而不是直接对CTRL做位运算。

## 函数原型

```cpp
__aicore__ inline void asc_set_saturation_flag(asc_saturation_mode saturation_mode, bool enable_sat)
```

## 参数说明

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| saturation_mode | 输入 | 饱和模式类型，取值见[asc_saturation_mode](../enum/asc_saturation_mode.md)。 |
| enable_sat | 输入 | 是否开启饱和。`true`表示开启饱和，`false`表示关闭饱和（对INT模式为截断；对FLOAT/FLOAT8/CAST为非饱和或截断，见上表）。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- `enable_sat`是API语义参数，不是CTRL原始比特值。FLOAT/FLOAT8/CAST开启饱和时写入0，INT开启饱和时写入1，二者极性相反，请勿按同一bit语义理解。
- 当`saturation_mode`为`CAST`时，还需将饱和策略设置为`asc_override_strategy::USE_GLOBAL`（CTRL[60]=1），CAST对应的CTRL[59]才会生效。可通过[asc_set_saturation_strategy](asc_set_saturation_strategy.md)配置。
- 本接口仅修改对应饱和控制位，不会破坏CTRL上其他无关比特位。

## 调用示例

场景：算子中将浮点激活值量化为int32时，源值可能超出int32表示范围。先切到全局CAST策略并开启CAST饱和，再调用[asc_float2int32](../scalar_compute/asc_float2int32.md)做转换，使溢出结果饱和到±MAX，而不是按位截断。

```cpp
float src = 3.0e9f;  // 可能超出int32正数范围的浮点值
int32_t dst;

// 使用全局CAST饱和策略，使CTRL[59]生效
asc_set_saturation_strategy(asc_override_strategy::USE_GLOBAL);
// 开启CAST饱和
asc_set_saturation_flag(asc_saturation_mode::CAST, true);

// 浮点转整数：超出范围时按饱和模式处理
dst = asc_float2int32_rn(src);
```
