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

头文件路径：`"c_api/sys_var/sys_var.h"`。

获取指定计算类型的饱和标志。通过读取CTRL寄存器的对应比特位来查询当前饱和/截断模式的状态。不同饱和模式对应的CTRL比特位参考[asc_set_saturation_flag](asc_set_saturation_flag.md)。

## 函数原型

```cpp
__aicore__ inline bool asc_get_saturation_flag(asc_saturation_mode saturation_mode)
```

## 参数说明

|参数名|输入/输出|描述|
| :------ | :---  | :------------ |
|saturation_mode   |输入   |饱和模式类型，取值为`asc_saturation_mode::FLOAT`、`asc_saturation_mode::FLOAT8`、`asc_saturation_mode::INT`、`asc_saturation_mode::CAST`。|

## 返回值说明

返回`bool`类型。`true`表示当前已使能饱和模式，`false`表示未使能。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
bool isSat = asc_get_saturation_flag(asc_saturation_mode::FLOAT);
```
