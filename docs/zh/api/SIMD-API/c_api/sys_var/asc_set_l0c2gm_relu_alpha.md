# asc_set_l0c2gm_relu_alpha

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

本接口用于配置L0C Buffer搬出过程中，预处理阶段Scalar ReLU使用的alpha系数。配置写入后，由后续的[asc_copy_l0c2gm](../cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)等L0C Buffer搬出接口使用。

当搬出接口的`relu_pre_mode`设置为2（Scalar ReLU）时，本接口配置的alpha系数生效：输入大于等于0时，输出等于输入；输入小于0时，输出等于`alpha * 输入`。

## 函数原型

```c
__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| config | 输入 | Scalar ReLU的64 bit alpha系数配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。 |

**表2** config字段说明 <a id="table2"></a>

| bit范围 | 字段 | 描述 |
|---|---|---|
| 12:0 | - | 保留位，需设置为0。 |
| 31:13 | M2 | 预处理阶段Scalar ReLU的alpha系数。该字段为19 bit的(1, 8, 10)浮点格式，即1 bit符号位、8 bit指数位和10 bit尾数位。 |
| 44:32 | - | 保留位，需设置为0。 |
| 63:45 | M4 | 后处理阶段Scalar ReLU的alpha系数，格式与M2相同。当前L0C Buffer搬出C API的`relu_post`为无效参数，本字段需设置为0。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- 本接口需在对应的L0C Buffer搬出接口执行前调用，并将搬出接口的`relu_pre_mode`设置为2。其他ReLU模式不使用本接口配置的M2字段。
- M2系数不能为INF、NAN或非规格化数。系数为0时，指数位和尾数位均需为0。
- 由标准IEEE 754 FP32数值生成M2字段时，保留符号位、8 bit指数位和高10 bit尾数位，舍弃低13 bit尾数位。
- `config[12:0]`、`config[44:32]`以及当前不支持的M4字段需设置为0。接口会将`config`整体写入，不会检查字段值。
- 配置会持续生效，直至再次调用本接口覆盖。不同搬出任务使用的alpha系数不同时，需在对应搬出接口执行前重新配置。

## 调用示例

```cpp
// alpha = 0.5：(1, 8, 10)格式的符号位为0、指数为126、尾数为0
constexpr uint32_t relu_pre_m2 = (0U << 18) | (126U << 10);
uint64_t config = static_cast<uint64_t>(relu_pre_m2) << 13;
asc_set_l0c2gm_relu_alpha(config);
```
