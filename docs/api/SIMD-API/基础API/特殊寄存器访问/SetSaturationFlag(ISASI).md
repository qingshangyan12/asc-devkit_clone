# SetSaturationFlag(ISASI)

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
|<cann-filter npu-type="950"> Ascend 950PR/Ascend 950DT | √ </cann-filter>|
|<cann-filter npu-type="A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="310b"> Atlas 200I/500 A2 推理产品 | x </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品AI Core | x </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品Vector Core | x </cann-filter>|
|<cann-filter npu-type="910"> Atlas 训练系列产品 | x </cann-filter>|
|<cann-filter npu-type="x90"> Kirin X90 | x </cann-filter>|
|<cann-filter npu-type="9030"> Kirin 9030 | x </cann-filter>|

## 功能说明

头文件路径为：`"basic_api/kernel_operator_common_intf.h"`。

设置饱和开关的标志位，以控制计算/精度转换时饱和模式的开启与关闭。

## 函数原型

```cpp
template <SaturationMode mode>
__aicore__ inline void SetSaturationFlag(bool enableSat)
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
| -------- | -------- |
| mode | 配置的对应饱和模式。<br>当mode配置为FLOAT时，控制浮点数计算和浮点数精度转换时的饱和模式，开启饱和模式时inf输出会被饱和为±MAX，NaN输出会被饱和为0；关闭饱和模式时inf/NaN保持原输出。<br><cann-filter npu-type="950">当mode配置为FLOAT8时，控制浮点数精度转换时的NaN饱和模式，开启饱和模式时NaN输出会被转换为0.0；关闭饱和模式时NaN输出保持NaN。</cann-filter><br>当mode配置为INT时，控制整数计算指令的饱和模式，开启饱和模式时溢出值饱和到±MAX；关闭饱和模式时溢出值按目标数据类型位数截断，保留低位，舍弃高位。<br>当mode配置为CAST时，控制浮点数转整数或整数转整数时的精度转换饱和模式，开启饱和模式时溢出值会被饱和为±MAX；关闭饱和模式时溢出值会按目标数据类型位数截断，保留低位，舍弃高位。<br><pre>enum class SaturationMode : uint8_t {<br>   FLOAT,      // CTRL[48]，控制浮点数计算和浮点数精度转换时的饱和模式;<br>   FLOAT8,     // CTRL[50]，控制浮点数精度转换时的NaN饱和模式，仅支持Ascend 950PR/Ascend 950DT;<br>   INT,        // CTRL[53]，控制整数计算指令的饱和模式;<br>   CAST,       // CTRL[59]，控制浮点数转整数或整数转整数时的精度转换饱和模式;<br>   SAT_GLOBAL  // CTRL[60]，不支持通过本接口配置，请使用SetGlobalSaturationFlag/GetGlobalSaturationFlag。<br>};</pre>  |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| enableSat | 输入 | 是否开启对应饱和模式。true表示开启饱和模式，false表示关闭饱和模式。 |

## 返回值说明

- <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品支持SaturationMode::FLOAT、SaturationMode::INT、SaturationMode::CAST。</cann-filter>
- <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品支持SaturationMode::FLOAT、SaturationMode::INT、SaturationMode::CAST。</cann-filter>
- <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT支持SaturationMode::FLOAT、SaturationMode::FLOAT8、SaturationMode::INT、SaturationMode::CAST。</cann-filter>
- SaturationMode::SAT_GLOBAL不支持通过本接口配置，Ascend 950PR/Ascend 950DT请使用[SetGlobalSaturationFlag](./SetGlobalSaturationFlag(ISASI).md)和[GetGlobalSaturationFlag](./GetGlobalSaturationFlag(ISASI).md)配置和读取CTRL[60]。

## 约束说明

无

## 调用示例

如下示例中开启整数转整数时精度转换的饱和模式。

```cpp
AscendC::SetSaturationFlag<AscendC::SaturationMode::CAST>(true);
```
