# asc_set_l0c2gm_lrelu_alpha

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

头文件路径：`"c_api/cube_datamove/cube_datamove.h"`。

设置L0C Buffer搬出场景下随路Leaky ReLU激活函数的斜率系数alpha。配置写入后作用于后续调用的L0C Buffer搬出接口的随路激活阶段，作为Leaky ReLU计算的alpha系数：当输入小于0时，输出按`alpha × 输入`计算；当输入大于等于0时，输出等于输入本身。配置值通过`half`或`float`引用传入，按重载指定的精度写入对应寄存器。

本接口写入的配置与[asc_set_l0c2gm_relu_alpha](asc_set_l0c2gm_relu_alpha.md)指向同一物理配置项，两者为同一配置的不同命名入口，调用任一接口设置后另一接口读取到的也是同一值，无需重复设置。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

本接口的配置需与配套setter接口按功能分组配合使用：
- 随路激活模式下，本接口设置的Leaky ReLU alpha需与[asc_set_l0c2gm_relu_alpha](asc_set_l0c2gm_relu_alpha.md)预先配置的ReLU激活参数搭配生效；
- 随路量化模式下，需通过[asc_set_l0c_copy_prequant](asc_set_l0c_copy_prequant.md)、[asc_set_l0c2gm_quant_post](asc_set_l0c2gm_quant_post.md)预先配置前/后级量化参数；
- NZ2ND/NZ2DN格式转换场景下，需通过[asc_set_l0c2gm_nz2nd](../cube_compute/asc_set_l0c2gm_nz2nd.md)、[asc_set_l0c2gm_channel_para](asc_set_l0c2gm_channel_para.md)预先配置通道与格式转换参数；
- 基础搬运配置需通过[asc_set_l0c2gm_config](../cube_compute/asc_set_l0c2gm_config.md)预先配置。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(<dtype>& config)
```

### dtype支持的数据类型

dtype支持的数据类型为`half`、`float`。

### 典型示例

```cpp
// 示例：配置float类型的Leaky ReLU系数。
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(float& config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `config` | 输入 | Leaky ReLU激活函数的斜率系数alpha。当随路激活计算输入小于0时，输出按`alpha × 输入`；输入大于等于0时输出等于输入。按重载dtype决定写入精度：`half`重载按`half`精度写入，`float`重载按`float`精度写入。取值范围为对应dtype可表示的全部有限值，无整数取值范围限制。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

<!-- npu="950" id8 -->
### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅写入Leaky ReLU alpha配置，实际随路激活计算由后续调用的L0C Buffer搬出接口执行，须先调用本接口写入配置，再调用对应搬出接口。
- 配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口或同名配置入口覆盖配置；不同搬出任务间若需不同alpha值，应在调用搬出接口前重新调用本接口设置。
- 本接口与配套setter接口协同配置L0C Buffer搬出接口的完整随路能力参数集，调用搬出接口前须完成全部相关配套setter的配置（配套setter名见功能说明章节）。
- 本接口与[asc_set_l0c2gm_relu_alpha](asc_set_l0c2gm_relu_alpha.md)指向同一物理配置项，二者为同一配置的不同命名入口，调用任一接口设置即可，无需重复设置；若两次调用设置不同值，后写入的值覆盖先写入的值。

### config精度与dtype一致性约束

- `config`按`half`重载写入时按`half`精度生效，按`float`重载写入时按`float`精度生效。若实际随路激活计算对alpha精度有要求，应按下游搬出接口计算精度选择对应重载，避免`half`重载写入后参与`float`精度计算时产生精度损失。

<!-- end id8 -->

## 调用示例

```cpp
half config = 0.5;
asc_set_l0c2gm_lrelu_alpha(config);
```
