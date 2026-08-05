# SetLoadDataPaddingValue<a name="ZH-CN_TOPIC_0000001834740625"></a>

## 产品支持情况<a name="section1550532418810"></a>

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
- Atlas 200I/500 A2 推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_mm_intf.h"`。

用于调用[LoadData（卷积数据搬运）](../矩阵计算的搬入/LoadData_3D.md)接口时设置Pad填充的数值。LoadData（卷积数据搬运）的模板参数isSetPadding设置为false时，用户需要通过本接口设置Pad填充的数值，设置为true时，本接口设置的填充值不生效。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <typename T>
__aicore__ inline void SetLoadDataPaddingValue(const T padValue)
```

## 参数说明<a name="section622mcpsimp"></a>

**表1** 参数说明

| 参数名称 | 输入/输出 | 含义 |
| ------ | ------ | ------ |
| padValue | 输入 | Pad填充值的数值。 |

## 数据类型

<!-- npu="950" id10 -->Ascend 950PR/Ascend 950DT，支持数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。<!-- end id10 --><br>
<!-- npu="310p" id11 -->Atlas 推理系列产品AI Core，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half。<!-- end id11 --><br>
<!-- npu="910b" id12 -->Atlas A2训练系列产品/Atlas A2推理系列产品，支持数据类型：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。<!-- end id12 --><br>
<!-- npu="A3" id13 -->Atlas A3训练系列产品/Atlas A3推理系列产品，支持数据类型：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。<!-- end id13 --><br>
<!-- npu="310b" id14 -->Atlas 200I/500 A2 推理产品，支持数据类型：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。<!-- end id14 --><br>