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

对RELU_ALPHA寄存器中的值进行设置。这是一个64bit的寄存器，存储在fixpipe或cube指令中进行Scalar ReLU时使用的alpha值。

其中各bit含义如下：

**表1** RELU_ALPHA寄存器比特位说明

| bit范围 |                                           含义                                           |
|:------|:--------------------------------------------------------------------------------------:|
| 31:13 | 表示ReLU_PRE中Scalar ReLU的M2值，只有Scalar ReLU时生效。硬件将以1位符号位，8位指数位和10位尾数位的格式用于计算，不能是INF/NAN。  |
| 63:45 | 表示ReLU_POST中Scalar ReLU的M2值，只有Scalar ReLU时生效。硬件将以1位符号位，8位指数位和10位尾数位的格式用于计算，不能是INF/NAN。 |

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config)
```

## 参数说明

**表2** 参数说明

|参数名|输入/输出| 描述        |
| :------ | :---  |:----------|
|config   |输入   | 待设置的寄存器值。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅写入随路Scalar ReLU的alpha乘子配置，实际随路激活计算由后续调用的L0C Buffer搬出接口在搬运路径上执行，须先调用本接口写入配置，再调用对应L0C Buffer搬出接口。
- 配置写入后对后续所有L0C Buffer搬出接口生效，直至再次调用本接口覆盖配置；不同搬出任务间若需不同alpha乘子，应在调用L0C Buffer搬出接口前重新调用本接口设置。
- 本接口与配套setter接口协同配置L0C Buffer搬出接口的完整随路能力参数集，调用L0C Buffer搬出接口前须完成全部相关配套setter的配置（配套setter名见功能说明章节）。
- M2 alpha乘子字段（`config`第13~31位）与M4 alpha乘子字段（`config`第45~63位）按`(1,8,10)`浮点格式解读，不可为INF/NAN或非规格化数，否则随路激活结果错误。
- `config`参数类型为`uint64_t`，与配置寄存器位宽一致（64bit），整个64bit立即数原样写入寄存器，无字段截断风险。
- 本接口写入的配置寄存器与`asc_set_l0c2gm_lrelu_alpha`指向同一物理配置项，二者为同一配置的不同命名入口，调用任一接口设置即可，无需重复设置；若两次调用设置不同值，后写入的值覆盖先写入的值。
- 配置寄存器为只写类型，本接口仅写不读，调用方无法通过本接口确认写入是否生效。

## 调用示例

```cpp
uint32_t pre_m2 = (0<<18)|(127<<10)|0;
uint32_t post_m2 = (0<<18)|(124<<10)|0;
uint64_t config = ((uint64_t)pre_m2<<13)|((uint64_t)post_m2<<45);
asc_set_l0c2gm_relu_alpha(config);
```
