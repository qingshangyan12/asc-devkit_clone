# UB-\>GM非对齐数据搬运\(DataCopyPad\)<a name="ZH-CN_TOPIC_0000002574928191"></a>

## 产品支持情况<a name="zh-cn_topic_0000002565968807_section796754519912"></a>

<a name="zh-cn_topic_0000002565968807_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565968807_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002565968807_p1883113061818"><a name="zh-cn_topic_0000002565968807_p1883113061818"></a><a name="zh-cn_topic_0000002565968807_p1883113061818"></a><span id="zh-cn_topic_0000002565968807_ph20833205312295"><a name="zh-cn_topic_0000002565968807_ph20833205312295"></a><a name="zh-cn_topic_0000002565968807_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002565968807_p783113012187"><a name="zh-cn_topic_0000002565968807_p783113012187"></a><a name="zh-cn_topic_0000002565968807_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565968807_row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002565968807_p48327011813"><a name="zh-cn_topic_0000002565968807_p48327011813"></a><a name="zh-cn_topic_0000002565968807_p48327011813"></a><span id="zh-cn_topic_0000002565968807_ph583230201815"><a name="zh-cn_topic_0000002565968807_ph583230201815"></a><a name="zh-cn_topic_0000002565968807_ph583230201815"></a><term id="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002565968807_p7948163910184"><a name="zh-cn_topic_0000002565968807_p7948163910184"></a><a name="zh-cn_topic_0000002565968807_p7948163910184"></a>√</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968807_row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002565968807_p14832120181815"><a name="zh-cn_topic_0000002565968807_p14832120181815"></a><a name="zh-cn_topic_0000002565968807_p14832120181815"></a><span id="zh-cn_topic_0000002565968807_ph1483216010188"><a name="zh-cn_topic_0000002565968807_ph1483216010188"></a><a name="zh-cn_topic_0000002565968807_ph1483216010188"></a><term id="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000002565968807_zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002565968807_p19948143911820"><a name="zh-cn_topic_0000002565968807_p19948143911820"></a><a name="zh-cn_topic_0000002565968807_p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002565968807_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

该接口提供将数据从Unified Buffer非对齐搬运至Global Memory的功能。

-   对于32B对齐搬运搬运场景，从UB读取的所有数据都会搬运至GM。
-   对于非32B对齐的场景，在读取UB数据时会填入dummy假数据，对齐到32B，搬入GM时会将dummy空数据丢弃，从而实现UB到GM的非对齐搬运，如[图1](#zh-cn_topic_0000002565968807_fig15924125113617)。

**图 1**  数据搬运过程<a name="zh-cn_topic_0000002565968807_fig15924125113617"></a>  
![](figures/数据搬运过程.png "数据搬运过程")

## 函数原型<a name="zh-cn_topic_0000002565968807_section82039854412"></a>

```
template <typename T>
__aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams)
```

## 参数说明<a name="zh-cn_topic_0000002565968807_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002565968807_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565968807_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002565968807_p10961458104110"><a name="zh-cn_topic_0000002565968807_p10961458104110"></a><a name="zh-cn_topic_0000002565968807_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002565968807_p6961155817415"><a name="zh-cn_topic_0000002565968807_p6961155817415"></a><a name="zh-cn_topic_0000002565968807_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565968807_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p29619586417"><a name="zh-cn_topic_0000002565968807_p29619586417"></a><a name="zh-cn_topic_0000002565968807_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p996110583417"><a name="zh-cn_topic_0000002565968807_p996110583417"></a><a name="zh-cn_topic_0000002565968807_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002565968807_section4219135304818">数据类型</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002565968807_table66911721148"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565968807_row176917201412"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002565968807_p5691122121416"><a name="zh-cn_topic_0000002565968807_p5691122121416"></a><a name="zh-cn_topic_0000002565968807_p5691122121416"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002565968807_p1869110201419"><a name="zh-cn_topic_0000002565968807_p1869110201419"></a><a name="zh-cn_topic_0000002565968807_p1869110201419"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002565968807_p1691929142"><a name="zh-cn_topic_0000002565968807_p1691929142"></a><a name="zh-cn_topic_0000002565968807_p1691929142"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565968807_row7691142121413"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565968807_p1169115271411"><a name="zh-cn_topic_0000002565968807_p1169115271411"></a><a name="zh-cn_topic_0000002565968807_p1169115271411"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565968807_p369172101416"><a name="zh-cn_topic_0000002565968807_p369172101416"></a><a name="zh-cn_topic_0000002565968807_p369172101416"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565968807_p1175268171410"><a name="zh-cn_topic_0000002565968807_p1175268171410"></a><a name="zh-cn_topic_0000002565968807_p1175268171410"></a>目的操作数，类型为为GlobalTensor，存储位置为Global Memory，目的地址需要1B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968807_row66914217145"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565968807_p12691192131416"><a name="zh-cn_topic_0000002565968807_p12691192131416"></a><a name="zh-cn_topic_0000002565968807_p12691192131416"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565968807_p569118220140"><a name="zh-cn_topic_0000002565968807_p569118220140"></a><a name="zh-cn_topic_0000002565968807_p569118220140"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565968807_p26914219141"><a name="zh-cn_topic_0000002565968807_p26914219141"></a><a name="zh-cn_topic_0000002565968807_p26914219141"></a>源操作数，类型LocalTensor，存储位置为Unified Buffer，源地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968807_row1075785651510"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565968807_p1728791441620"><a name="zh-cn_topic_0000002565968807_p1728791441620"></a><a name="zh-cn_topic_0000002565968807_p1728791441620"></a>dataCopyParams</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565968807_p11287151451610"><a name="zh-cn_topic_0000002565968807_p11287151451610"></a><a name="zh-cn_topic_0000002565968807_p11287151451610"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565968807_p199341953115415"><a name="zh-cn_topic_0000002565968807_p199341953115415"></a><a name="zh-cn_topic_0000002565968807_p199341953115415"></a>搬运参数类型，通过该参数可以配置搬运的数据块个数、长度、地址间隔等信息，同时支持非连续和连续搬运。DataCopyExtParams类型，具体参数说明请参考<a href="#zh-cn_topic_0000002565968807_table144203616291">表3</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 3**  DataCopyExtParams结构体参数定义

<a name="zh-cn_topic_0000002565968807_table144203616291"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565968807_row742014692919"><th class="cellrowborder" valign="top" width="12.959999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002565968807_p1573101033918"><a name="zh-cn_topic_0000002565968807_p1573101033918"></a><a name="zh-cn_topic_0000002565968807_p1573101033918"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="87.03999999999999%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002565968807_p7573191019395"><a name="zh-cn_topic_0000002565968807_p7573191019395"></a><a name="zh-cn_topic_0000002565968807_p7573191019395"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565968807_row1642018613291"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p1457312101394"><a name="zh-cn_topic_0000002565968807_p1457312101394"></a><a name="zh-cn_topic_0000002565968807_p1457312101394"></a>blockCount</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p18573121053915"><a name="zh-cn_topic_0000002565968807_p18573121053915"></a><a name="zh-cn_topic_0000002565968807_p18573121053915"></a>搬运的数据块个数，数据类型为uint16_t，取值范围：blockCount∈[0, 4095]。</p>
<p id="zh-cn_topic_0000002565968807_p821315919437"><a name="zh-cn_topic_0000002565968807_p821315919437"></a><a name="zh-cn_topic_0000002565968807_p821315919437"></a><strong id="zh-cn_topic_0000002565968807_b1899463611015"><a name="zh-cn_topic_0000002565968807_b1899463611015"></a><a name="zh-cn_topic_0000002565968807_b1899463611015"></a>注：blockCount = 0表示不执行搬运，该接口将被视为 NOP（空操作）</strong>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968807_row174201161298"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p19573171073914"><a name="zh-cn_topic_0000002565968807_p19573171073914"></a><a name="zh-cn_topic_0000002565968807_p19573171073914"></a>blockLen</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p15731210203917"><a name="zh-cn_topic_0000002565968807_p15731210203917"></a><a name="zh-cn_topic_0000002565968807_p15731210203917"></a>搬运的每个数据块长度，数据类型为uint32_t，取值范围：blockLen∈[0, 2097151]，单位：1B。</p>
<p id="zh-cn_topic_0000002565968807_p1396141417441"><a name="zh-cn_topic_0000002565968807_p1396141417441"></a><a name="zh-cn_topic_0000002565968807_p1396141417441"></a><strong id="zh-cn_topic_0000002565968807_b263917145441"><a name="zh-cn_topic_0000002565968807_b263917145441"></a><a name="zh-cn_topic_0000002565968807_b263917145441"></a>注：blockLen= 0表示不执行搬运，该接口将被视为 NOP（空操作）</strong>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968807_row1542086182913"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p175731610183919"><a name="zh-cn_topic_0000002565968807_p175731610183919"></a><a name="zh-cn_topic_0000002565968807_p175731610183919"></a>srcStride</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p12551329183114"><a name="zh-cn_topic_0000002565968807_p12551329183114"></a><a name="zh-cn_topic_0000002565968807_p12551329183114"></a>源操作数相邻数据块之间的间隔（即前一个数据块<strong id="zh-cn_topic_0000002565968807_b139293112232"><a name="zh-cn_topic_0000002565968807_b139293112232"></a><a name="zh-cn_topic_0000002565968807_b139293112232"></a>结束地址</strong>与后一个数据块<strong id="zh-cn_topic_0000002565968807_b865853942514"><a name="zh-cn_topic_0000002565968807_b865853942514"></a><a name="zh-cn_topic_0000002565968807_b865853942514"></a>起始地址</strong>的差值），uint32_t类型，取值范围：srcStride∈[0, 2^32 - 1]，单位：dataBlock（32B）。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968807_row13420361297"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p057351013919"><a name="zh-cn_topic_0000002565968807_p057351013919"></a><a name="zh-cn_topic_0000002565968807_p057351013919"></a>dstStride</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p9422151918588"><a name="zh-cn_topic_0000002565968807_p9422151918588"></a><a name="zh-cn_topic_0000002565968807_p9422151918588"></a>目的操作数相邻数据块之间的间隔（即前一个数据块<strong id="zh-cn_topic_0000002565968807_b119775918341"><a name="zh-cn_topic_0000002565968807_b119775918341"></a><a name="zh-cn_topic_0000002565968807_b119775918341"></a>结束地址</strong>与后一个数据块<strong id="zh-cn_topic_0000002565968807_b1378813816359"><a name="zh-cn_topic_0000002565968807_b1378813816359"></a><a name="zh-cn_topic_0000002565968807_b1378813816359"></a>起始地址</strong>的差值），uint32_t类型，取值范围：dstStride∈[0, 2^32 - 1]，单位：1B。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968807_row2075810219179"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p17428152705514"><a name="zh-cn_topic_0000002565968807_p17428152705514"></a><a name="zh-cn_topic_0000002565968807_p17428152705514"></a>rsv</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p842842719551"><a name="zh-cn_topic_0000002565968807_p842842719551"></a><a name="zh-cn_topic_0000002565968807_p842842719551"></a>保留字段，默认值为0。</p>
</td>
</tr>
</tbody>
</table>

**图 2**  blockLen不满足32字节对齐<a name="zh-cn_topic_0000002565968807_fig179380535217"></a>  
![](figures/blockLen不满足32字节对齐.png "blockLen不满足32字节对齐")

如[图2](#zh-cn_topic_0000002565968807_fig179380535217)，将两个数据块搬运从UB搬运至GM，每个数据块的blockLen为47B，源操作数相邻数据块之间的间隔为32B，目的操作数相邻数据块之间的间隔为1B。

由于数据块的blockLen为 47B不满足32B对齐，在读取UB数据时会填入dummy空数据，对齐到32B，搬入GM时会将dummy空数据丢弃，从而实现UB到GM的非对齐搬运。

为了实现上述搬运，DataCopyExtParams结构体参数配置如下：

-   blockCount = 2，搬运2个数据块。
-   blockLen = 47，每个连续传输数据块包含47B。
-   srcStride = 1，源操作数相邻数据块之间的间隔为1，单位：32B。
-   dstStride = 1，目的操作数相邻数据块之间的间隔为1，单位：1B。

## 数据类型<a name="zh-cn_topic_0000002565968807_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持数据类型为：int8\_t、uint8\_t、int16\_t、uint16\_t、int32\_t、uint32\_t、int64\_t、 uint64\_t、 half、bfloat16\_t、float、double

Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持数据类型为：int8\_t、uint8\_t、int16\_t、uint16\_t、int32\_t、uint32\_t、int64\_t、 uint64\_t、 half、bfloat16\_t、float、double

## API映射关系<a name="zh-cn_topic_0000002565968807_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（copy\_ubuf\_to\_gm\_align\_xx）的基础上进行抽象封装实现的，其对应的底层built-in接口为：

```
void copy_ubuf_to_gm_align_b8(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_ubuf_to_gm_align_b16(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_tsrcGap, uint32_t dstGap);

void copy_ubuf_to_gm_align_b32(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);
```

## 约束说明<a name="zh-cn_topic_0000002565968807_section2045914466492"></a>

-   位于Unified Buffer的源地址必须32B对齐，位于Global Memory的目的地址必须1B对齐。
-   DataCopyExtParams结构体参数的值需在取值范围内：

    **表 4**  DataCopyExtParams结构体参数取值范围

    <a name="zh-cn_topic_0000002565968807_table843765314306"></a>
    <table><thead align="left"><tr id="zh-cn_topic_0000002565968807_row2043745317306"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002565968807_p1743745312309"><a name="zh-cn_topic_0000002565968807_p1743745312309"></a><a name="zh-cn_topic_0000002565968807_p1743745312309"></a>参数名称</p>
    </th>
    <th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002565968807_p124371553173016"><a name="zh-cn_topic_0000002565968807_p124371553173016"></a><a name="zh-cn_topic_0000002565968807_p124371553173016"></a>取值范围</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="zh-cn_topic_0000002565968807_row18437453183011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p1264817377468"><a name="zh-cn_topic_0000002565968807_p1264817377468"></a><a name="zh-cn_topic_0000002565968807_p1264817377468"></a>blockCount</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p1297153919251"><a name="zh-cn_topic_0000002565968807_p1297153919251"></a><a name="zh-cn_topic_0000002565968807_p1297153919251"></a>[0, 4095]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002565968807_row743716536309"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p116487377463"><a name="zh-cn_topic_0000002565968807_p116487377463"></a><a name="zh-cn_topic_0000002565968807_p116487377463"></a>blockLen</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p258445192811"><a name="zh-cn_topic_0000002565968807_p258445192811"></a><a name="zh-cn_topic_0000002565968807_p258445192811"></a>[0, 2097151]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002565968807_row19437155343014"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p964893714463"><a name="zh-cn_topic_0000002565968807_p964893714463"></a><a name="zh-cn_topic_0000002565968807_p964893714463"></a>srcStride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p165841451162817"><a name="zh-cn_topic_0000002565968807_p165841451162817"></a><a name="zh-cn_topic_0000002565968807_p165841451162817"></a>[0, 2^32 - 1]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002565968807_row194371253133011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968807_p56494378469"><a name="zh-cn_topic_0000002565968807_p56494378469"></a><a name="zh-cn_topic_0000002565968807_p56494378469"></a>dstStride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968807_p106496376462"><a name="zh-cn_topic_0000002565968807_p106496376462"></a><a name="zh-cn_topic_0000002565968807_p106496376462"></a>[0, 2^32 - 1]</p>
    </td>
    </tr>
    </tbody>
    </table>

