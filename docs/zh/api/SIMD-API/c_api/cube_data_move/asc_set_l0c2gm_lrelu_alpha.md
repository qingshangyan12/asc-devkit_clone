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

用于设置asc_copy_l0c2l1或asc_copy_l0c2gm接口计算过程中使用的Leaky ReLU alpha值。该值只支持half和float两种数据类型。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config)
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(float& config)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| config | 输入 | Leaky ReLU alpha值。 |

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
- 本接口与`asc_set_l0c2gm_relu_alpha`指向同一物理配置项，二者为同一配置的不同命名入口，调用任一接口设置即可，无需重复设置；若两次调用设置不同值，后写入的值覆盖先写入的值。

### config精度与dtype一致性约束

- `config`按`half`重载写入时按`half`精度生效，按`float`重载写入时按`float`精度生效。若实际随路激活计算对alpha精度有要求，应按下游搬出接口计算精度选择对应重载，避免`half`重载写入后参与`float`精度计算时产生精度损失。

<!-- end id8 -->

## 调用示例

```cpp
half config = 0.5;
asc_set_l0c2gm_lrelu_alpha(config);
```
