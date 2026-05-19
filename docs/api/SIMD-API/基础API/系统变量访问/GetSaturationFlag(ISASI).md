# GetSaturationFlag\(ISASI\)<a name="ZH-CN_TOPIC_0000002361149961"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持  |
| :----------------------- | :------: |
|<cann-filter npu-type="950"> Ascend 950PR/Ascend 950DT | x </cann-filter>|
|<cann-filter npu-type="A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="310b"> Atlas 200I/500 A2 推理产品 | x </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品AI Core | x </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品Vector Core | x </cann-filter>|
|<cann-filter npu-type="910"> Atlas 训练系列产品 | x </cann-filter>|
|<cann-filter npu-type="x90"> Kirin X90 | x </cann-filter>|
|<cann-filter npu-type="9030"> Kirin 9030 | x </cann-filter>|

## 功能说明<a name="section618mcpsimp"></a>

获取计算 / 精度转换时的饱和模式是否开启。返回true表示开启饱和模式，false表示未开启饱和模式。

## 函数原型<a name="section620mcpsimp"></a>

```
template <SaturationMode mode>
__aicore__ inline bool GetSaturationFlag();
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table4835205712588"></a>
<table><thead align="left"><tr id="row118356578583"><th class="cellrowborder" valign="top" width="18.27%" id="mcps1.2.3.1.1"><p id="p48354572582"><a name="p48354572582"></a><a name="p48354572582"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="81.73%" id="mcps1.2.3.1.2"><p id="p583535795817"><a name="p583535795817"></a><a name="p583535795817"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1835857145817"><td class="cellrowborder" valign="top" width="18.27%" headers="mcps1.2.3.1.1 "><p id="p5835457165816"><a name="p5835457165816"></a><a name="p5835457165816"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="81.73%" headers="mcps1.2.3.1.2 "><p id="p168351657155818"><a name="p168351657155818"></a><a name="p168351657155818"></a>配置的对应饱和模式。

当mode配置为FLOAT时，开启饱和模式，inf输出会被饱和为±MAX，nan输出会被饱和为0；关闭饱和模式，inf/nan保持原输出。

当mode配置为CAST时，开启饱和模式，溢出值会被饱和为±MAX；关闭饱和模式，溢出值会按目标数据类型位数截断，保留低位，舍弃高位。
</p>
<a name="screen09015195591"></a><a name="screen09015195591"></a><pre class="screen" codetype="Cpp" id="screen09015195591">
enum class SaturationMode : uint8_t {
    FLOAT,     // 控制浮点数计算和浮点数精度转换时的饱和模式
    CAST       // 控制浮点数转整数或整数转整数时的精度转换饱和模式
};
</pre>
</td>

</tbody>
</table>



## 返回值说明<a name="section640mcpsimp"></a>

返回true表示开启饱和模式，false表示未开启饱和模式。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section11279242185011"></a>

-   如下示例中查询是否使能整数转整数时精度转换的饱和模式。

    ```
    bool res = AscendC::GetSaturationFlag<AscendC::SaturationMode::CAST>();
    ```

