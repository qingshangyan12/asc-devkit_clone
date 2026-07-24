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

头文件路径：`"c_api/sys_var/sys_var.h"`。

用于设置指定计算类型的饱和标志。该接口通过设置CTRL寄存器的对应比特位来控制不同计算类型的饱和/截断模式。

不同饱和模式对应的CTRL寄存器比特位如下：

|SaturationMode|CTRL比特位|说明|
|:------|:------|:------|
|FLOAT|CTRL[48]|控制half浮点数计算饱和模式。0表示饱和模式（INF输出饱和为±MAX，NAN输出饱和为0），1表示非饱和模式（INF/NAN保持原输出）。|
|FLOAT8|CTRL[50]|控制fp8浮点数精度转换饱和模式。0表示饱和模式，1表示非饱和模式。|
|INT|CTRL[53]|控制整数计算饱和模式。0表示截断模式（溢出值按目标位数截断），1表示饱和模式（溢出值饱和到±MAX）。|
|CAST|CTRL[59]|控制浮点转整数或整数转整数时的精度转换饱和模式。0表示饱和模式，1表示截断模式。|

## 函数原型

```cpp
__aicore__ inline void asc_set_saturation_flag(AscendC::SaturationMode saturation_mode, bool enable_sat)
```

## 参数说明

|参数名|输入/输出|描述|
| :------ | :---  | :------------ |
|saturation_mode   |输入   |饱和模式类型，取值为`AscendC::SaturationMode::FLOAT`、`AscendC::SaturationMode::FLOAT8`、`AscendC::SaturationMode::INT`、`AscendC::SaturationMode::CAST`。|
|enable_sat   |输入   |是否使能饱和模式。`true`表示使能饱和，`false`表示关闭饱和。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 对于FLOAT、FLOAT8、CAST模式，`enable_sat`为`true`时将对应比特位置0（饱和），为`false`时置1（非饱和/截断）。
- 对于INT模式，`enable_sat`为`true`时将对应比特位置1（饱和），为`false`时置0（截断）。

## 调用示例

```cpp
// 使能half浮点数计算的饱和模式
asc_set_saturation_flag(AscendC::SaturationMode::FLOAT, true);
```
