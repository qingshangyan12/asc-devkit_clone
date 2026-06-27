# NegativeInfinity

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

返回指定数据类型的负无穷大值。

## 函数原型

- 标量接口，返回值为标量：

    ```cpp
    constexpr __aicore__ static inline T NegativeInfinity()
    ```

- 矢量接口，为dstLocal前count个元素赋负无穷大值：

    ```cpp
    __aicore__ static inline void NegativeInfinity(const LocalTensor<T>& dstLocal, uint32_t count)
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstLocal | 输出 | 目的操作数。<br><br>类型为[LocalTensor](../../../数据结构/LocalTensor和GlobalTensor定义/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。 |
| count | 输入 | 输入数据元素个数。 |

## 数据类型

<cann-filter npu-type="950">针对Ascend 950PR/Ascend 950DT，支持的数据类型为：half、bfloat16_t、float。</cann-filter>

## 返回值说明

标量接口返回值为对应数据类型的[负无穷大值](../../../数据结构/内置数据类型.md)。

矢量接口无返回值。

**表 2**  NegativeInfinity各数据类型输出

<a name="table1710811591013"></a>
<table><thead align="left"><tr id="row910912590010"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p810910595014"><a name="p810910595014"></a><a name="p810910595014"></a>数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p710965913015"><a name="p710965913015"></a><a name="p710965913015"></a>十进制数</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p111109595016"><a name="p111109595016"></a><a name="p111109595016"></a>二进制各bit位</p>
</th>
</tr>
</thead>
<tbody><tr id="row3110115912014"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1911045914017"><a name="p1911045914017"></a><a name="p1911045914017"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p3110105918015"><a name="p3110105918015"></a><a name="p3110105918015"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p36601856131316"><a name="p36601856131316"></a><a name="p36601856131316"></a>1 11111 0000000000</p>
</td>
</tr>
<tr id="row1110185915016"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p61101459701"><a name="p61101459701"></a><a name="p61101459701"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1711055920018"><a name="p1711055920018"></a><a name="p1711055920018"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p11660145614139"><a name="p11660145614139"></a><a name="p11660145614139"></a>1 11111111 0000000</p>
</td>
</tr>
<tr id="row74289115212"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p642812112216"><a name="p642812112216"></a><a name="p642812112216"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p557510543130"><a name="p557510543130"></a><a name="p557510543130"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p18659165614139"><a name="p18659165614139"></a><a name="p18659165614139"></a>1 11111111 00000000000000000000000</p>
</td>
</tr>
</tbody>
</table>

## 约束说明

无。

## 调用示例

- 标量接口：

    ```cpp
    float value = AscendC::NumericLimits<float>::NegativeInfinity();
    ```

- 矢量接口：

    ```cpp
    AscendC::NumericLimits<float>::NegativeInfinity(dstLocal, 256);// 返回256个float类型的负无穷大值
    ```
