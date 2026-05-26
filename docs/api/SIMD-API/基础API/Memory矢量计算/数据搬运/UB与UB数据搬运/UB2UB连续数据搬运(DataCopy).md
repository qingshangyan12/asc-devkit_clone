# UB2UB连续数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002574928197"></a>

## 产品支持情况<a name="zh-cn_topic_0000002567699425_section796754519912"></a>

<a name="zh-cn_topic_0000002567699425_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002567699425_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002567699425_p1883113061818"><a name="zh-cn_topic_0000002567699425_p1883113061818"></a><a name="zh-cn_topic_0000002567699425_p1883113061818"></a><span id="zh-cn_topic_0000002567699425_ph20833205312295"><a name="zh-cn_topic_0000002567699425_ph20833205312295"></a><a name="zh-cn_topic_0000002567699425_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002567699425_p783113012187"><a name="zh-cn_topic_0000002567699425_p783113012187"></a><a name="zh-cn_topic_0000002567699425_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody></tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002567699425_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

DataCopy能够实现Unified Buffer和Unified Buffer之间的连续数据搬运，数据搬运时格式和内容保持不变。

## 函数原型<a name="zh-cn_topic_0000002567699425_section82039854412"></a>

```
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```

## 参数说明<a name="zh-cn_topic_0000002567699425_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002567699425_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002567699425_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002567699425_p10961458104110"><a name="zh-cn_topic_0000002567699425_p10961458104110"></a><a name="zh-cn_topic_0000002567699425_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002567699425_p6961155817415"><a name="zh-cn_topic_0000002567699425_p6961155817415"></a><a name="zh-cn_topic_0000002567699425_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002567699425_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002567699425_p29619586417"><a name="zh-cn_topic_0000002567699425_p29619586417"></a><a name="zh-cn_topic_0000002567699425_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002567699425_p996110583417"><a name="zh-cn_topic_0000002567699425_p996110583417"></a><a name="zh-cn_topic_0000002567699425_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002567699425_section4219135304818">数据类型</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002567699425_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002567699425_row15956194105014"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002567699425_p7956144195014"><a name="zh-cn_topic_0000002567699425_p7956144195014"></a><a name="zh-cn_topic_0000002567699425_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002567699425_p1295624145013"><a name="zh-cn_topic_0000002567699425_p1295624145013"></a><a name="zh-cn_topic_0000002567699425_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002567699425_p16956144145011"><a name="zh-cn_topic_0000002567699425_p16956144145011"></a><a name="zh-cn_topic_0000002567699425_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002567699425_row5956546509"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002567699425_p19287714181617"><a name="zh-cn_topic_0000002567699425_p19287714181617"></a><a name="zh-cn_topic_0000002567699425_p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002567699425_p192871614151615"><a name="zh-cn_topic_0000002567699425_p192871614151615"></a><a name="zh-cn_topic_0000002567699425_p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002567699425_p25471539132516"><a name="zh-cn_topic_0000002567699425_p25471539132516"></a><a name="zh-cn_topic_0000002567699425_p25471539132516"></a>目的操作数。类型为LocalTensor，存储位置为Unified Buffer，目的地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699425_row4956154125018"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002567699425_p142871414131614"><a name="zh-cn_topic_0000002567699425_p142871414131614"></a><a name="zh-cn_topic_0000002567699425_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002567699425_p628711148165"><a name="zh-cn_topic_0000002567699425_p628711148165"></a><a name="zh-cn_topic_0000002567699425_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002567699425_p461180132619"><a name="zh-cn_topic_0000002567699425_p461180132619"></a><a name="zh-cn_topic_0000002567699425_p461180132619"></a>源操作数。类型为LocalTensor，存储位置为Unified Buffer，源地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699425_row137841716478"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002567699425_p17785197124710"><a name="zh-cn_topic_0000002567699425_p17785197124710"></a><a name="zh-cn_topic_0000002567699425_p17785197124710"></a>count</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002567699425_p8785974471"><a name="zh-cn_topic_0000002567699425_p8785974471"></a><a name="zh-cn_topic_0000002567699425_p8785974471"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002567699425_p9785117184720"><a name="zh-cn_topic_0000002567699425_p9785117184720"></a><a name="zh-cn_topic_0000002567699425_p9785117184720"></a>参与搬运的元素个数。count * sizeof(T)需要32B对齐， 若未对齐，则搬运量会向下取整到32B对齐。</p>
</td>
</tr>
</tbody>
</table>

以half数据类型为例，源操作数的shape为1 \* 128。当count = 128时，[图1](#zh-cn_topic_0000002567699425_fig13481537531)将源操作数中128个元素连续搬运至目的操作数。

**图 1**  连续搬运示意图<a name="zh-cn_topic_0000002567699425_fig13481537531"></a>  
![](figures/连续搬运示意图-4.png "连续搬运示意图-4")

## 数据类型<a name="zh-cn_topic_0000002567699425_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

## API映射关系<a name="zh-cn_topic_0000002567699425_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（copy\_ubuf\_to\_ubuf）的基础上进行抽象封装实现的，其对应的底层built-in接口为：

```
void copy_ubuf_to_ubuf(__ubuf__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride);
```

## 约束说明<a name="zh-cn_topic_0000002567699425_section2045914466492"></a>

-   位于Unified Buffer的地址必须32B对齐。
-   count \* sizeof\(T\)需要32B对齐， 若未对齐，则搬运量会向下取整到32B对齐。

