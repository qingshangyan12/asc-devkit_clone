# GM2UB随路转换-DN2NZ搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002544407954"></a>

## 产品支持情况<a name="zh-cn_topic_0000002565888823_section796754519912"></a>

<a name="zh-cn_topic_0000002565888823_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565888823_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002565888823_p1883113061818"><a name="zh-cn_topic_0000002565888823_p1883113061818"></a><a name="zh-cn_topic_0000002565888823_p1883113061818"></a><span id="zh-cn_topic_0000002565888823_ph20833205312295"><a name="zh-cn_topic_0000002565888823_ph20833205312295"></a><a name="zh-cn_topic_0000002565888823_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002565888823_p783113012187"><a name="zh-cn_topic_0000002565888823_p783113012187"></a><a name="zh-cn_topic_0000002565888823_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565888823_row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002565888823_p48327011813"><a name="zh-cn_topic_0000002565888823_p48327011813"></a><a name="zh-cn_topic_0000002565888823_p48327011813"></a><span id="zh-cn_topic_0000002565888823_ph583230201815"><a name="zh-cn_topic_0000002565888823_ph583230201815"></a><a name="zh-cn_topic_0000002565888823_ph583230201815"></a><term id="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002565888823_p7948163910184"><a name="zh-cn_topic_0000002565888823_p7948163910184"></a><a name="zh-cn_topic_0000002565888823_p7948163910184"></a>√</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002565888823_p14832120181815"><a name="zh-cn_topic_0000002565888823_p14832120181815"></a><a name="zh-cn_topic_0000002565888823_p14832120181815"></a><span id="zh-cn_topic_0000002565888823_ph1483216010188"><a name="zh-cn_topic_0000002565888823_ph1483216010188"></a><a name="zh-cn_topic_0000002565888823_ph1483216010188"></a><term id="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000002565888823_zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002565888823_p19948143911820"><a name="zh-cn_topic_0000002565888823_p19948143911820"></a><a name="zh-cn_topic_0000002565888823_p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002565888823_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

该接口提供将数据从Global Memory非对齐搬运至Unified Buffer的功能，可以根据开发者的需要自行填充数据。

填充的数据有两种配置方式：

-   配置结构体[DataCopyPadExtParams](#zh-cn_topic_0000002565888823_table2036831862917)的参数paddingValue，此时DataCopyPadExtParams的参数isPad需要设置为true。
-   使用寄存器配置接口[SetPadValue](寄存器配置说明.md#ZH-CN_TOPIC_0000002544248308)在接口外部设置需要填充的数据，此时DataCopyPadExtParams的参数isPad需要设置为false。

## 函数原型<a name="zh-cn_topic_0000002565888823_section82039854412"></a>

```
template <typename T> 
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams) 
```

## 参数说明<a name="zh-cn_topic_0000002565888823_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002565888823_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565888823_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002565888823_p10961458104110"><a name="zh-cn_topic_0000002565888823_p10961458104110"></a><a name="zh-cn_topic_0000002565888823_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002565888823_p6961155817415"><a name="zh-cn_topic_0000002565888823_p6961155817415"></a><a name="zh-cn_topic_0000002565888823_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565888823_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p29619586417"><a name="zh-cn_topic_0000002565888823_p29619586417"></a><a name="zh-cn_topic_0000002565888823_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p996110583417"><a name="zh-cn_topic_0000002565888823_p996110583417"></a><a name="zh-cn_topic_0000002565888823_p996110583417"></a>操作数以及paddingValue（待填充数据值）的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002565888823_section4219135304818">数据类型</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002565888823_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565888823_row15956194105014"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002565888823_p7956144195014"><a name="zh-cn_topic_0000002565888823_p7956144195014"></a><a name="zh-cn_topic_0000002565888823_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002565888823_p1295624145013"><a name="zh-cn_topic_0000002565888823_p1295624145013"></a><a name="zh-cn_topic_0000002565888823_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002565888823_p16956144145011"><a name="zh-cn_topic_0000002565888823_p16956144145011"></a><a name="zh-cn_topic_0000002565888823_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565888823_row5956546509"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565888823_p19287714181617"><a name="zh-cn_topic_0000002565888823_p19287714181617"></a><a name="zh-cn_topic_0000002565888823_p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565888823_p192871614151615"><a name="zh-cn_topic_0000002565888823_p192871614151615"></a><a name="zh-cn_topic_0000002565888823_p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565888823_p16287121461618"><a name="zh-cn_topic_0000002565888823_p16287121461618"></a><a name="zh-cn_topic_0000002565888823_p16287121461618"></a>目的操作数，类型为LocalTensor，存储位置为Unified Buffer，目的地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row4956154125018"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565888823_p142871414131614"><a name="zh-cn_topic_0000002565888823_p142871414131614"></a><a name="zh-cn_topic_0000002565888823_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565888823_p628711148165"><a name="zh-cn_topic_0000002565888823_p628711148165"></a><a name="zh-cn_topic_0000002565888823_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565888823_p0287191420164"><a name="zh-cn_topic_0000002565888823_p0287191420164"></a><a name="zh-cn_topic_0000002565888823_p0287191420164"></a>源操作数，类型为GlobalTensor，存储位置为Global Memory，源地址需要1B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row137841716478"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565888823_p17785197124710"><a name="zh-cn_topic_0000002565888823_p17785197124710"></a><a name="zh-cn_topic_0000002565888823_p17785197124710"></a>dataCopyParams</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565888823_p8785974471"><a name="zh-cn_topic_0000002565888823_p8785974471"></a><a name="zh-cn_topic_0000002565888823_p8785974471"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565888823_p273618166281"><a name="zh-cn_topic_0000002565888823_p273618166281"></a><a name="zh-cn_topic_0000002565888823_p273618166281"></a>搬运参数。DataCopyExtParams类型，具体参数说明请参考<a href="#zh-cn_topic_0000002565888823_table108821810182917">表3</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row1492820278281"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565888823_p173174119287"><a name="zh-cn_topic_0000002565888823_p173174119287"></a><a name="zh-cn_topic_0000002565888823_p173174119287"></a>padParams</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565888823_p1731841192818"><a name="zh-cn_topic_0000002565888823_p1731841192818"></a><a name="zh-cn_topic_0000002565888823_p1731841192818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565888823_p1173154118284"><a name="zh-cn_topic_0000002565888823_p1173154118284"></a><a name="zh-cn_topic_0000002565888823_p1173154118284"></a>从Global Memory搬运数据至Local Memory时，可以根据开发者需要，在搬运数据左边或右边填充数据。padParams是用于控制数据填充过程的参数。DataCopyPadExtParams类型，具体参数请参考<a href="#zh-cn_topic_0000002565888823_table2036831862917">表4</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 3**  DataCopyExtParams参数说明

<a name="zh-cn_topic_0000002565888823_table108821810182917"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565888823_row188832107291"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002565888823_p9883201014291"><a name="zh-cn_topic_0000002565888823_p9883201014291"></a><a name="zh-cn_topic_0000002565888823_p9883201014291"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002565888823_p28838102297"><a name="zh-cn_topic_0000002565888823_p28838102297"></a><a name="zh-cn_topic_0000002565888823_p28838102297"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565888823_row38836107290"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p175512298314"><a name="zh-cn_topic_0000002565888823_p175512298314"></a><a name="zh-cn_topic_0000002565888823_p175512298314"></a>blockCount</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p1855152943114"><a name="zh-cn_topic_0000002565888823_p1855152943114"></a><a name="zh-cn_topic_0000002565888823_p1855152943114"></a>搬运的数据块个数，uint16_t类型，取值范围：blockCount∈[0, 4095]，默认值为1。</p>
<p id="zh-cn_topic_0000002565888823_p140782213434"><a name="zh-cn_topic_0000002565888823_p140782213434"></a><a name="zh-cn_topic_0000002565888823_p140782213434"></a><strong id="zh-cn_topic_0000002565888823_b1899463611015"><a name="zh-cn_topic_0000002565888823_b1899463611015"></a><a name="zh-cn_topic_0000002565888823_b1899463611015"></a>注：blockCount = 0表示不执行搬运，该接口将被视为 NOP（空操作）</strong>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row06649993114"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p12551172933113"><a name="zh-cn_topic_0000002565888823_p12551172933113"></a><a name="zh-cn_topic_0000002565888823_p12551172933113"></a>blockLen</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p1551122912313"><a name="zh-cn_topic_0000002565888823_p1551122912313"></a><a name="zh-cn_topic_0000002565888823_p1551122912313"></a>搬运的每个数据块长度，数据类型为uint32_t，取值范围：blockLen∈[0, 2097151]，单位：1B。</p>
<p id="zh-cn_topic_0000002565888823_p138981724154320"><a name="zh-cn_topic_0000002565888823_p138981724154320"></a><a name="zh-cn_topic_0000002565888823_p138981724154320"></a><strong id="zh-cn_topic_0000002565888823_b9104112554315"><a name="zh-cn_topic_0000002565888823_b9104112554315"></a><a name="zh-cn_topic_0000002565888823_b9104112554315"></a>注：blockLen= 0表示不执行搬运，该接口将被视为 NOP（空操作）</strong>。<strong id="zh-cn_topic_0000002565888823_b13906164114712"><a name="zh-cn_topic_0000002565888823_b13906164114712"></a><a name="zh-cn_topic_0000002565888823_b13906164114712"></a>blockLen必须是 sizeof(T) 的整数倍，需要注意UB不要超限。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row754911153112"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p1755162913111"><a name="zh-cn_topic_0000002565888823_p1755162913111"></a><a name="zh-cn_topic_0000002565888823_p1755162913111"></a>srcStride</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p12551329183114"><a name="zh-cn_topic_0000002565888823_p12551329183114"></a><a name="zh-cn_topic_0000002565888823_p12551329183114"></a>源操作数相邻数据块之间的间隔（即前一个数据块<strong id="zh-cn_topic_0000002565888823_b139293112232"><a name="zh-cn_topic_0000002565888823_b139293112232"></a><a name="zh-cn_topic_0000002565888823_b139293112232"></a>结束地址</strong>与后一个数据块<strong id="zh-cn_topic_0000002565888823_b865853942514"><a name="zh-cn_topic_0000002565888823_b865853942514"></a><a name="zh-cn_topic_0000002565888823_b865853942514"></a>起始地址</strong>的差值），uint32_t类型，取值范围：srcStride∈[0, 2^32 - 1]，单位：1B。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row1788951317314"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p18551162912313"><a name="zh-cn_topic_0000002565888823_p18551162912313"></a><a name="zh-cn_topic_0000002565888823_p18551162912313"></a>dstStride</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p185511629123113"><a name="zh-cn_topic_0000002565888823_p185511629123113"></a><a name="zh-cn_topic_0000002565888823_p185511629123113"></a>目的操作数相邻数据块之间的间隔（即前一个数据块<strong id="zh-cn_topic_0000002565888823_b119775918341"><a name="zh-cn_topic_0000002565888823_b119775918341"></a><a name="zh-cn_topic_0000002565888823_b119775918341"></a>结束地址</strong>与后一个数据块<strong id="zh-cn_topic_0000002565888823_b1378813816359"><a name="zh-cn_topic_0000002565888823_b1378813816359"></a><a name="zh-cn_topic_0000002565888823_b1378813816359"></a>起始地址</strong>的差值），uint32_t类型，取值范围：dstStride∈[0, 2^32 - 1]，单位：dataBlock（32B）。</p>
<p id="zh-cn_topic_0000002565888823_p1158114162411"><a name="zh-cn_topic_0000002565888823_p1158114162411"></a><a name="zh-cn_topic_0000002565888823_p1158114162411"></a><strong id="zh-cn_topic_0000002565888823_b29731957152415"><a name="zh-cn_topic_0000002565888823_b29731957152415"></a><a name="zh-cn_topic_0000002565888823_b29731957152415"></a>注：需要注意UB不要超限</strong>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row354752223116"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p1655102943112"><a name="zh-cn_topic_0000002565888823_p1655102943112"></a><a name="zh-cn_topic_0000002565888823_p1655102943112"></a>rsv</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p175513298319"><a name="zh-cn_topic_0000002565888823_p175513298319"></a><a name="zh-cn_topic_0000002565888823_p175513298319"></a>保留字段。uint32_t类型，默认值为0。</p>
</td>
</tr>
</tbody>
</table>

**表 4**  DataCopyPadExtParams参数说明

<a name="zh-cn_topic_0000002565888823_table2036831862917"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565888823_row3368191852910"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002565888823_p16368018192910"><a name="zh-cn_topic_0000002565888823_p16368018192910"></a><a name="zh-cn_topic_0000002565888823_p16368018192910"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002565888823_p12368618172914"><a name="zh-cn_topic_0000002565888823_p12368618172914"></a><a name="zh-cn_topic_0000002565888823_p12368618172914"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565888823_row33691818142910"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p5768815642"><a name="zh-cn_topic_0000002565888823_p5768815642"></a><a name="zh-cn_topic_0000002565888823_p5768815642"></a>isPad</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p582714277511"><a name="zh-cn_topic_0000002565888823_p582714277511"></a><a name="zh-cn_topic_0000002565888823_p582714277511"></a>是否需要填充用户自定义的数据。</p>
<a name="zh-cn_topic_0000002565888823_ul1642772213615"></a><a name="zh-cn_topic_0000002565888823_ul1642772213615"></a><ul id="zh-cn_topic_0000002565888823_ul1642772213615"><li><span>true：</span><span>填充padding value</span>。</li></ul>
<a name="zh-cn_topic_0000002565888823_ul17193291765"></a><a name="zh-cn_topic_0000002565888823_ul17193291765"></a><ul id="zh-cn_topic_0000002565888823_ul17193291765"><li><span>false：</span>表示<span>用户不需要指定填充值，会默认填充</span>随机值。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row68961890418"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p19768415242"><a name="zh-cn_topic_0000002565888823_p19768415242"></a><a name="zh-cn_topic_0000002565888823_p19768415242"></a>leftPadding</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p776801518414"><a name="zh-cn_topic_0000002565888823_p776801518414"></a><a name="zh-cn_topic_0000002565888823_p776801518414"></a>连续搬运数据块左侧需要补充的数据范围，单位为元素个数。</p>
<p id="zh-cn_topic_0000002565888823_p476816151547"><a name="zh-cn_topic_0000002565888823_p476816151547"></a><a name="zh-cn_topic_0000002565888823_p476816151547"></a><strong id="zh-cn_topic_0000002565888823_b572142019453"><a name="zh-cn_topic_0000002565888823_b572142019453"></a><a name="zh-cn_topic_0000002565888823_b572142019453"></a>注：leftPadding所占的字节数均不能超过32字节。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row19818132419"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p176831514414"><a name="zh-cn_topic_0000002565888823_p176831514414"></a><a name="zh-cn_topic_0000002565888823_p176831514414"></a>rightPadding</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p7768201519413"><a name="zh-cn_topic_0000002565888823_p7768201519413"></a><a name="zh-cn_topic_0000002565888823_p7768201519413"></a>连续搬运数据块右侧需要补充的数据范围，单位为元素个数。</p>
<p id="zh-cn_topic_0000002565888823_p976819151445"><a name="zh-cn_topic_0000002565888823_p976819151445"></a><a name="zh-cn_topic_0000002565888823_p976819151445"></a><strong id="zh-cn_topic_0000002565888823_b12217191854515"><a name="zh-cn_topic_0000002565888823_b12217191854515"></a><a name="zh-cn_topic_0000002565888823_b12217191854515"></a>注：rightPadding所占的字节数均不能超过32字节。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565888823_row751018114414"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565888823_p7769315645"><a name="zh-cn_topic_0000002565888823_p7769315645"></a><a name="zh-cn_topic_0000002565888823_p7769315645"></a>paddingValue</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565888823_p37691815542"><a name="zh-cn_topic_0000002565888823_p37691815542"></a><a name="zh-cn_topic_0000002565888823_p37691815542"></a>左右两侧需要填充的数据值。</p>
<p id="zh-cn_topic_0000002565888823_p976917151146"><a name="zh-cn_topic_0000002565888823_p976917151146"></a><a name="zh-cn_topic_0000002565888823_p976917151146"></a>数据类型和源操作数保持一致。</p>
<p id="zh-cn_topic_0000002565888823_p57696159419"><a name="zh-cn_topic_0000002565888823_p57696159419"></a><a name="zh-cn_topic_0000002565888823_p57696159419"></a>当数据类型长度为64位时，paddingValue只能设置为0。</p>
</td>
</tr>
</tbody>
</table>

## blockLen+leftPadding+rightPadding满足32B对齐<a name="zh-cn_topic_0000002565888823_section66281619152612"></a>

**图 1**  blockLen+leftPadding+rightPadding满足32B对齐<a name="zh-cn_topic_0000002565888823_fig209471712171814"></a>  
![](figures/blockLen+leftPadding+rightPadding满足32B对齐.png "blockLen+leftPadding+rightPadding满足32B对齐")

如[图1](#zh-cn_topic_0000002565888823_fig209471712171814)，将两个数据块从GM搬运至UB，每个blockLen为54B，源操作数相邻数据块之间的间隔为1B，目的操作数相邻数据块之间的间隔为32B。

在blockLen左侧和右侧分别填充2个和3个half类型元素，此时blockLen + leftPadding + rightPadding = 54B + 2 \* 2B + 3 \* 2B = 64B，满足32B对齐。

DataCopyExtParams参数配置如下：

-   blockCount = 2，搬运2个数据块。
-   blockLen =54，每个连续传输数据块包含54B。
-   srcStride = 1，源操作数相邻数据块之间的间隔为1B。
-   dstStride = 1，目的操作数相邻数据块之间的间隔为1个DataBlock（32B）。

DataCopyPadExtParams参数配置如下：

-   isPad = true，使用DataCopyPadExtParams内参数paddingValue的值作为填充值；或者设置isPad = false，使用寄存器配置接口[SetPadValue](寄存器配置说明.md#ZH-CN_TOPIC_0000002544248308)在接口外部设置填充值。
-   leftPadding = 2，搬运结果左侧填充2个half数据。
-   rightPadding = 3，搬运结果右侧填充3个half数据。
-   paddingValue = 1，blockLen的左右两侧填充值为1。

## blockLen+leftPadding+rightPadding不满足32B对齐<a name="zh-cn_topic_0000002565888823_section73461020593"></a>

**图 2**  blockLen+leftPadding+rightPadding不满足32B对齐<a name="zh-cn_topic_0000002565888823_fig1443611515363"></a>  
![](figures/blockLen+leftPadding+rightPadding不满足32B对齐.png "blockLen+leftPadding+rightPadding不满足32B对齐")

如[图2](#zh-cn_topic_0000002565888823_fig1443611515363)，将两个数据块从GM搬运至UB，每个blockLen为47B，源操作数相邻数据块之间的间隔为1B，目的操作数相邻数据块之间的间隔为32B。

在blockLen左侧和右侧分别填充2个和3个half类型元素，此时blockLen + leftPadding + rightPadding = 48B + 2 \* 2B + 3 \* 2B = 58B，不满足32B对齐，该指令会在rightPadding后补dummy假数据，使得blockLen + leftPadding + rightPadding + dummy满足32B对齐。

DataCopyExtParams参数配置如下：

-   blockCount = 2，搬运2个数据块。
-   blockLen =48，每个连续传输数据块包含48B。
-   srcStride = 1，源操作数相邻数据块之间的间隔为1B。
-   dstStride = 1，目的操作数相邻数据块之间的间隔为1个DataBlock（32B）。

DataCopyPadExtParams参数配置如下：

-   isPad = true，使用DataCopyPadExtParams内参数paddingValue的值作为填充值；或者设置isPad = false，使用寄存器配置接口[SetPadValue](寄存器配置说明.md#ZH-CN_TOPIC_0000002544248308)在接口外部设置填充值。
-   leftPadding = 2，搬运结果左侧填充2个half数据。
-   rightPadding = 3，搬运结果右侧填充3个half数据。
-   paddingValue = 1，blockLen的左右两侧填充值为1。

## 数据类型<a name="zh-cn_topic_0000002565888823_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持数据类型为：int8\_t、uint8\_t、int16\_t、uint16\_t、half、bfloat16\_t、int32\_t、uint32\_t、 float、int64\_t、 uint64\_t、double

Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持数据类型为：int8\_t、uint8\_t、int16\_t、uint16\_t、half、bfloat16\_t、int32\_t、uint32\_t、 float、int64\_t、 uint64\_t、double

## API映射关系<a name="zh-cn_topic_0000002565888823_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（set\_mov\_pad\_val、copy\_gm\_to\_ubuf\_align）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
void set_mov_pad_val(uint64_t config);

void copy_gm_to_ubuf_align_b8(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_gm_to_ubuf_align_b16(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_gm_to_ubuf_align_b32(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);
```

## 约束说明<a name="zh-cn_topic_0000002565888823_section2045914466492"></a>

-   位于Global Memory的源地址必须1B对齐，位于Unified Buffer的目的地址必须32B对齐。
-   leftPadding、rightPadding所占字节数均不能超过32B。
-   blockLen必须是 sizeof\(T\) 的整数倍。
-   结构体DataCopyPadExtParams的参数paddingValue数据类型和源操作数保持一致。当数据类型为b64时，paddingValue只能设置为0。
-   结构体DataCopyPadExtParams的参数取值范围需要考虑UB超限问题。
