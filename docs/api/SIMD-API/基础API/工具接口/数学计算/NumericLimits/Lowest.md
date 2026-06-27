# Lowest

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | x </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品AI Core | x </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | x </cann-filter>|

## 功能说明

头文件路径为：`"basic_api/kernel_operator_limits_intf.h"`。

返回指定数据类型的最低有限值，即满足无其他有限值y符合y < x的有限值x。

## 函数原型

- 标量接口，返回值为标量：

    ```cpp
    constexpr __aicore__ static inline T Lowest()
    ```

- 矢量接口，为dstLocal前count个元素赋最低有限值：

    ```cpp
    __aicore__ static inline void Lowest(const LocalTensor<T>& dstLocal, uint32_t count)
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstLocal | 输出 | 目的操作数。<br><br>类型为[LocalTensor](../../../数据结构/LocalTensor和GlobalTensor定义/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。 |
| count | 输入 | 输入数据元素个数。 |

## 数据类型

<cann-filter npu-type="950">针对Ascend 950PR/Ascend 950DT，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。</cann-filter>

## 返回值说明

标量接口返回值为对应数据类型的最低有限值。

矢量接口无返回值。

**表 2**  Lowest各数据类型输出

<a name="table1710811591013"></a>
<table><thead align="left"><tr id="row910912590010"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p810910595014"><a name="p810910595014"></a><a name="p810910595014"></a>数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p710965913015"><a name="p710965913015"></a><a name="p710965913015"></a>十进制数</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p111109595016"><a name="p111109595016"></a><a name="p111109595016"></a>二进制各bit位</p>
</th>
</tr>
</thead>
<tbody><tr id="row121107591302"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1711015594016"><a name="p1711015594016"></a><a name="p1711015594016"></a>uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p211095913014"><a name="p211095913014"></a><a name="p211095913014"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p12661195619138"><a name="p12661195619138"></a><a name="p12661195619138"></a>0000 0000</p>
</td>
</tr>
<tr id="row121101591101"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p211016593012"><a name="p211016593012"></a><a name="p211016593012"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p5110165911010"><a name="p5110165911010"></a><a name="p5110165911010"></a>-128</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p15661456141311"><a name="p15661456141311"></a><a name="p15661456141311"></a>1000 0000</p>
</td>
</tr>
<tr id="row16110185912018"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p31101759903"><a name="p31101759903"></a><a name="p31101759903"></a>uint16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p711035918019"><a name="p711035918019"></a><a name="p711035918019"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p126611356181313"><a name="p126611356181313"></a><a name="p126611356181313"></a>0000 0000 0000 0000</p>
</td>
</tr>
<tr id="row811016591603"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1011065912020"><a name="p1011065912020"></a><a name="p1011065912020"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p811025914015"><a name="p811025914015"></a><a name="p811025914015"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p366015569135"><a name="p366015569135"></a><a name="p366015569135"></a>1000 0000 0000 0000</p>
</td>
</tr>
<tr id="row3110115912014"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1911045914017"><a name="p1911045914017"></a><a name="p1911045914017"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p3110105918015"><a name="p3110105918015"></a><a name="p3110105918015"></a>-65504</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p36601856131316"><a name="p36601856131316"></a><a name="p36601856131316"></a>1 11110 1111111111</p>
</td>
</tr>
<tr id="row1110185915016"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p61101459701"><a name="p61101459701"></a><a name="p61101459701"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p8228182253211"><a name="p8228182253211"></a><a name="p8228182253211"></a>-3.4e+38</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p11660145614139"><a name="p11660145614139"></a><a name="p11660145614139"></a>1 11111110 1111111</p>
</td>
</tr>
<tr id="row91101159708"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p81101159400"><a name="p81101159400"></a><a name="p81101159400"></a>uint32_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p157517544139"><a name="p157517544139"></a><a name="p157517544139"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p14660155612138"><a name="p14660155612138"></a><a name="p14660155612138"></a>0000 0000 0000 0000 0000 0000 0000 0000</p>
</td>
</tr>
<tr id="row17999201020216"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p159993101329"><a name="p159993101329"></a><a name="p159993101329"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p95751354181311"><a name="p95751354181311"></a><a name="p95751354181311"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p186591956181313"><a name="p186591956181313"></a><a name="p186591956181313"></a>1000 0000 0000 0000 0000 0000 0000 0000</p>
</td>
</tr>
<tr id="row74289115212"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p642812112216"><a name="p642812112216"></a><a name="p642812112216"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p557510543130"><a name="p557510543130"></a><a name="p557510543130"></a>-3.4e+38</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p18659165614139"><a name="p18659165614139"></a><a name="p18659165614139"></a>1 11111110 11111111111111111111111</p>
</td>
</tr>
</tbody>
</table>

## 约束说明

无。

## 调用示例

- 标量接口：

    ```cpp
    int32_t value = AscendC::NumericLimits<int32_t>::Lowest();
    ```

- 矢量接口：

    ```cpp
    AscendC::NumericLimits<int32_t>::Lowest(dstLocal, 256);// 返回256个int32_t类型的最低有限值
    ```
