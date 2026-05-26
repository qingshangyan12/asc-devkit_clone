# UB2UB高维切分数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002544248314"></a>

## 产品支持情况<a name="zh-cn_topic_0000002536779698_section796754519912"></a>

<a name="zh-cn_topic_0000002536779698_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002536779698_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002536779698_p1883113061818"><a name="zh-cn_topic_0000002536779698_p1883113061818"></a><a name="zh-cn_topic_0000002536779698_p1883113061818"></a><span id="zh-cn_topic_0000002536779698_ph20833205312295"><a name="zh-cn_topic_0000002536779698_ph20833205312295"></a><a name="zh-cn_topic_0000002536779698_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002536779698_p783113012187"><a name="zh-cn_topic_0000002536779698_p783113012187"></a><a name="zh-cn_topic_0000002536779698_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody></tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002536779698_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

DataCopy能够实现Global Memory和Unified Buffer之间的连续数据搬运和非连续数据搬运，数据搬运时格式和内容保持不变。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

## 函数原型<a name="zh-cn_topic_0000002536779698_section82039854412"></a>

接口同时支持非连续搬运和连续搬运，对于连续搬运场景，推荐使用[UB-\>UB连续数据搬运\(DataCopy\)](UB--UB连续数据搬运(DataCopy)-13.md#ZH-CN_TOPIC_0000002574928197)。

```
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```

## 参数说明<a name="zh-cn_topic_0000002536779698_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002536779698_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002536779698_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002536779698_p10961458104110"><a name="zh-cn_topic_0000002536779698_p10961458104110"></a><a name="zh-cn_topic_0000002536779698_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002536779698_p6961155817415"><a name="zh-cn_topic_0000002536779698_p6961155817415"></a><a name="zh-cn_topic_0000002536779698_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002536779698_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p29619586417"><a name="zh-cn_topic_0000002536779698_p29619586417"></a><a name="zh-cn_topic_0000002536779698_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p996110583417"><a name="zh-cn_topic_0000002536779698_p996110583417"></a><a name="zh-cn_topic_0000002536779698_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002536779698_section4219135304818">数据类型</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002536779698_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002536779698_row15956194105014"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002536779698_p7956144195014"><a name="zh-cn_topic_0000002536779698_p7956144195014"></a><a name="zh-cn_topic_0000002536779698_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002536779698_p1295624145013"><a name="zh-cn_topic_0000002536779698_p1295624145013"></a><a name="zh-cn_topic_0000002536779698_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002536779698_p16956144145011"><a name="zh-cn_topic_0000002536779698_p16956144145011"></a><a name="zh-cn_topic_0000002536779698_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002536779698_row5956546509"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002536779698_p19287714181617"><a name="zh-cn_topic_0000002536779698_p19287714181617"></a><a name="zh-cn_topic_0000002536779698_p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002536779698_p192871614151615"><a name="zh-cn_topic_0000002536779698_p192871614151615"></a><a name="zh-cn_topic_0000002536779698_p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002536779698_p16287121461618"><a name="zh-cn_topic_0000002536779698_p16287121461618"></a><a name="zh-cn_topic_0000002536779698_p16287121461618"></a>目的操作数，类型为LocalTensor，存储位置为Unified Buffer，目的地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002536779698_row4956154125018"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002536779698_p142871414131614"><a name="zh-cn_topic_0000002536779698_p142871414131614"></a><a name="zh-cn_topic_0000002536779698_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002536779698_p628711148165"><a name="zh-cn_topic_0000002536779698_p628711148165"></a><a name="zh-cn_topic_0000002536779698_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002536779698_p0287191420164"><a name="zh-cn_topic_0000002536779698_p0287191420164"></a><a name="zh-cn_topic_0000002536779698_p0287191420164"></a>源操作数，类型为LocalTensor，存储位置为Unified Buffer，源地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002536779698_row1075785651510"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002536779698_p1728791441620"><a name="zh-cn_topic_0000002536779698_p1728791441620"></a><a name="zh-cn_topic_0000002536779698_p1728791441620"></a>repeatParams</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002536779698_p11287151451610"><a name="zh-cn_topic_0000002536779698_p11287151451610"></a><a name="zh-cn_topic_0000002536779698_p11287151451610"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002536779698_p10391459201718"><a name="zh-cn_topic_0000002536779698_p10391459201718"></a><a name="zh-cn_topic_0000002536779698_p10391459201718"></a>搬运参数，类型为DataCopyParams。通过该参数可以配置搬运的数据块个数、长度、地址间隔等信息，同时支持非连续和连续搬运。</p>
<p id="zh-cn_topic_0000002536779698_p125412312537"><a name="zh-cn_topic_0000002536779698_p125412312537"></a><a name="zh-cn_topic_0000002536779698_p125412312537"></a>DataCopyParams参数说明请参考<a href="#zh-cn_topic_0000002536779698_table144203616291">表3</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 3**  DataCopyParams结构体参数定义

<a name="zh-cn_topic_0000002536779698_table144203616291"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002536779698_row742014692919"><th class="cellrowborder" valign="top" width="12.959999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002536779698_p1242012617291"><a name="zh-cn_topic_0000002536779698_p1242012617291"></a><a name="zh-cn_topic_0000002536779698_p1242012617291"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="87.03999999999999%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002536779698_p1342018622916"><a name="zh-cn_topic_0000002536779698_p1342018622916"></a><a name="zh-cn_topic_0000002536779698_p1342018622916"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002536779698_row1642018613291"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p2420106192913"><a name="zh-cn_topic_0000002536779698_p2420106192913"></a><a name="zh-cn_topic_0000002536779698_p2420106192913"></a>blockCount</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p1297153919251"><a name="zh-cn_topic_0000002536779698_p1297153919251"></a><a name="zh-cn_topic_0000002536779698_p1297153919251"></a>搬运的数据块个数。uint16_t类型，取值范围：blockCount∈[0, 4095]，默认值为1。</p>
<p id="zh-cn_topic_0000002536779698_p393615391617"><a name="zh-cn_topic_0000002536779698_p393615391617"></a><a name="zh-cn_topic_0000002536779698_p393615391617"></a><strong id="zh-cn_topic_0000002536779698_b1899463611015"><a name="zh-cn_topic_0000002536779698_b1899463611015"></a><a name="zh-cn_topic_0000002536779698_b1899463611015"></a>注：blockCount = 0表示不执行搬运，该接口将被视为 NOP（空操作）</strong>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002536779698_row174201161298"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p1742010618293"><a name="zh-cn_topic_0000002536779698_p1742010618293"></a><a name="zh-cn_topic_0000002536779698_p1742010618293"></a>blockLen</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p11780174752617"><a name="zh-cn_topic_0000002536779698_p11780174752617"></a><a name="zh-cn_topic_0000002536779698_p11780174752617"></a>搬运的每个数据块长度，单位为DataBlock（32字节）。uint16_t类型，取值范围：blockLen∈[0, 65535]。</p>
<p id="zh-cn_topic_0000002536779698_p19949155131618"><a name="zh-cn_topic_0000002536779698_p19949155131618"></a><a name="zh-cn_topic_0000002536779698_p19949155131618"></a><strong id="zh-cn_topic_0000002536779698_b918318618166"><a name="zh-cn_topic_0000002536779698_b918318618166"></a><a name="zh-cn_topic_0000002536779698_b918318618166"></a>注：blockLen = 0表示不执行搬运，该接口将被视为 NOP（空操作）</strong>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002536779698_row1542086182913"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p104209612298"><a name="zh-cn_topic_0000002536779698_p104209612298"></a><a name="zh-cn_topic_0000002536779698_p104209612298"></a>srcGap</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p10146125312186"><a name="zh-cn_topic_0000002536779698_p10146125312186"></a><a name="zh-cn_topic_0000002536779698_p10146125312186"></a>源操作数相邻数据块之间的间隔（即前一个数据块<strong id="zh-cn_topic_0000002536779698_b139293112232"><a name="zh-cn_topic_0000002536779698_b139293112232"></a><a name="zh-cn_topic_0000002536779698_b139293112232"></a>结束地址</strong>与后一个数据块<strong id="zh-cn_topic_0000002536779698_b865853942514"><a name="zh-cn_topic_0000002536779698_b865853942514"></a><a name="zh-cn_topic_0000002536779698_b865853942514"></a>起始地址</strong>的差值），uint16_t类型，取值范围：srcGap∈[0, 65535]，单位：DataBlock（32B）。</p>
<a name="zh-cn_topic_0000002536779698_ul277581716716"></a><a name="zh-cn_topic_0000002536779698_ul277581716716"></a><ul id="zh-cn_topic_0000002536779698_ul277581716716"><li>blockCount = 1时，srcGap无意义，设置为0即可。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002536779698_row13420361297"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p642016112920"><a name="zh-cn_topic_0000002536779698_p642016112920"></a><a name="zh-cn_topic_0000002536779698_p642016112920"></a>dstGap</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p1398722102012"><a name="zh-cn_topic_0000002536779698_p1398722102012"></a><a name="zh-cn_topic_0000002536779698_p1398722102012"></a>目的操作数相邻数据块之间的间隔（即前一个数据块<strong id="zh-cn_topic_0000002536779698_b119775918341"><a name="zh-cn_topic_0000002536779698_b119775918341"></a><a name="zh-cn_topic_0000002536779698_b119775918341"></a>结束地址</strong>与后一个数据块<strong id="zh-cn_topic_0000002536779698_b1378813816359"><a name="zh-cn_topic_0000002536779698_b1378813816359"></a><a name="zh-cn_topic_0000002536779698_b1378813816359"></a>起始地址</strong>的差值），uint16_t类型，取值范围：dstGap∈[0, 65535]，单位：DataBlock（32B）。</p>
<a name="zh-cn_topic_0000002536779698_ul3374123152014"></a><a name="zh-cn_topic_0000002536779698_ul3374123152014"></a><ul id="zh-cn_topic_0000002536779698_ul3374123152014"><li>blockCount = 1时，dstGap无意义，设置为0即可。</li></ul>
</td>
</tr>
</tbody>
</table>

## 连续搬运<a name="zh-cn_topic_0000002536779698_section017053241315"></a>

针对连续搬运场景，推荐使用[UB-\>UB连续数据搬运\(DataCopy\)](UB--UB连续数据搬运(DataCopy)-13.md#ZH-CN_TOPIC_0000002574928197)，以下示例仅展示高维切分数据搬运接口具有连续搬运能力。

以half数据类型为例，源操作数的shape为1 \* 128。

[图1](#zh-cn_topic_0000002536779698_fig1379313811381)将源操作数中128个元素连续搬运至目的操作数，DataCopyParams结构体参数配置如下：

-   blockCount = 1，搬运1个数据块，表示连续搬运。
-   blockLen = 8，一个数据块长度为8个DataBlock。
-   srcGap = 0，源操作数相邻数据块之间的间隔为0。
-   dstGap = 0，目的操作数相邻数据块之间的间隔为0。

**图 1**  连续搬运示意图<a name="zh-cn_topic_0000002536779698_fig1379313811381"></a>  
![](figures/连续搬运示意图-5.png "连续搬运示意图-5")

## 非连续搬运<a name="zh-cn_topic_0000002536779698_section13815175812385"></a>

以half数据类型为例，源操作数的shape为1 \* 112。

[图2](#zh-cn_topic_0000002536779698_fig1648934717412)将源操作数中非连续数据搬运至目的操作数，DataCopyParams结构体参数配置如下：

-   blockCount= 2，搬运2个数据块。
-   blockLen= 3，一个数据块的长度为3个DataBlock。
-   srcGap= 1，源操作数相邻数据块之间的间隔为1。
-   dstGap= 2，目的操作数相邻数据块之间的间隔为2。

**图 2**  非连续搬运示意图<a name="zh-cn_topic_0000002536779698_fig1648934717412"></a>  
![](figures/非连续搬运示意图-6.png "非连续搬运示意图-6")

## 数据类型<a name="zh-cn_topic_0000002536779698_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

## API映射关系<a name="zh-cn_topic_0000002536779698_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（copy\_ubuf\_to\_ubuf）的基础上进行抽象封装实现的，其对应的底层built-in接口为：

```
void copy_ubuf_to_ubuf(__ubuf__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride);
```

## 约束说明<a name="zh-cn_topic_0000002536779698_section2045914466492"></a>

-   位于Unified Buffer的目的地址必须32B对齐。
-   当DataCopyParams结构体参数blockCount、blockLen任意一个值为0时，表示指令不会执行，该接口将被视为 NOP（空操作）。
-   DataCopyParams结构体参数的值需在取值范围内：

    **表 4**  DataCopyParams结构体参数取值范围

    <a name="zh-cn_topic_0000002536779698_table843765314306"></a>
    <table><thead align="left"><tr id="zh-cn_topic_0000002536779698_row2043745317306"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002536779698_p1743745312309"><a name="zh-cn_topic_0000002536779698_p1743745312309"></a><a name="zh-cn_topic_0000002536779698_p1743745312309"></a>参数名称</p>
    </th>
    <th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002536779698_p124371553173016"><a name="zh-cn_topic_0000002536779698_p124371553173016"></a><a name="zh-cn_topic_0000002536779698_p124371553173016"></a>取值范围</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="zh-cn_topic_0000002536779698_row18437453183011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p17437105318308"><a name="zh-cn_topic_0000002536779698_p17437105318308"></a><a name="zh-cn_topic_0000002536779698_p17437105318308"></a>blockCount</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p1383153213319"><a name="zh-cn_topic_0000002536779698_p1383153213319"></a><a name="zh-cn_topic_0000002536779698_p1383153213319"></a>[0, 4095]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002536779698_row743716536309"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p8437753103017"><a name="zh-cn_topic_0000002536779698_p8437753103017"></a><a name="zh-cn_topic_0000002536779698_p8437753103017"></a>blockLen</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p11457174216599"><a name="zh-cn_topic_0000002536779698_p11457174216599"></a><a name="zh-cn_topic_0000002536779698_p11457174216599"></a>[0, 65535]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002536779698_row19437155343014"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p14437253203018"><a name="zh-cn_topic_0000002536779698_p14437253203018"></a><a name="zh-cn_topic_0000002536779698_p14437253203018"></a>srcGap</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p2437153173017"><a name="zh-cn_topic_0000002536779698_p2437153173017"></a><a name="zh-cn_topic_0000002536779698_p2437153173017"></a>[0, 65535]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002536779698_row194371253133011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002536779698_p843715312301"><a name="zh-cn_topic_0000002536779698_p843715312301"></a><a name="zh-cn_topic_0000002536779698_p843715312301"></a>dstGap</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002536779698_p5420568299"><a name="zh-cn_topic_0000002536779698_p5420568299"></a><a name="zh-cn_topic_0000002536779698_p5420568299"></a>[0, 65535]</p>
    </td>
    </tr>
    </tbody>
    </table>

