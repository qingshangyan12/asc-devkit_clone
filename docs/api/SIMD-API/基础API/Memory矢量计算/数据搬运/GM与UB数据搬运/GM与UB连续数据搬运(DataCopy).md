# GM与UB连续数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002544407902"></a>

## 产品支持情况<a name="zh-cn_topic_0000002534928976_section796754519912"></a>

<a name="zh-cn_topic_0000002534928976_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002534928976_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002534928976_p1883113061818"><a name="zh-cn_topic_0000002534928976_p1883113061818"></a><a name="zh-cn_topic_0000002534928976_p1883113061818"></a><span id="zh-cn_topic_0000002534928976_ph20833205312295"><a name="zh-cn_topic_0000002534928976_ph20833205312295"></a><a name="zh-cn_topic_0000002534928976_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002534928976_p783113012187"><a name="zh-cn_topic_0000002534928976_p783113012187"></a><a name="zh-cn_topic_0000002534928976_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody></tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002534928976_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

DataCopy能够实现Global Memory和Unified Buffer之间的连续数据搬运，数据搬运时格式和内容保持不变。

## 函数原型<a name="zh-cn_topic_0000002534928976_section82039854412"></a>

-   GM-\>UB

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const uint32_t count)
    ```

-   UB-\>GM

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
    ```

## 参数说明<a name="zh-cn_topic_0000002534928976_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002534928976_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002534928976_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002534928976_p10961458104110"><a name="zh-cn_topic_0000002534928976_p10961458104110"></a><a name="zh-cn_topic_0000002534928976_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002534928976_p6961155817415"><a name="zh-cn_topic_0000002534928976_p6961155817415"></a><a name="zh-cn_topic_0000002534928976_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002534928976_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002534928976_p29619586417"><a name="zh-cn_topic_0000002534928976_p29619586417"></a><a name="zh-cn_topic_0000002534928976_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002534928976_p996110583417"><a name="zh-cn_topic_0000002534928976_p996110583417"></a><a name="zh-cn_topic_0000002534928976_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002534928976_section4219135304818">数据类型</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002534928976_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002534928976_row15956194105014"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002534928976_p7956144195014"><a name="zh-cn_topic_0000002534928976_p7956144195014"></a><a name="zh-cn_topic_0000002534928976_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002534928976_p1295624145013"><a name="zh-cn_topic_0000002534928976_p1295624145013"></a><a name="zh-cn_topic_0000002534928976_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002534928976_p16956144145011"><a name="zh-cn_topic_0000002534928976_p16956144145011"></a><a name="zh-cn_topic_0000002534928976_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002534928976_row5956546509"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002534928976_p19287714181617"><a name="zh-cn_topic_0000002534928976_p19287714181617"></a><a name="zh-cn_topic_0000002534928976_p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002534928976_p192871614151615"><a name="zh-cn_topic_0000002534928976_p192871614151615"></a><a name="zh-cn_topic_0000002534928976_p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002534928976_p25471539132516"><a name="zh-cn_topic_0000002534928976_p25471539132516"></a><a name="zh-cn_topic_0000002534928976_p25471539132516"></a>目的操作数。</p>
<a name="zh-cn_topic_0000002534928976_ul17617194613257"></a><a name="zh-cn_topic_0000002534928976_ul17617194613257"></a><ul id="zh-cn_topic_0000002534928976_ul17617194613257"><li>类型为LocalTensor时，存储位置为Unified Buffer，目的地址需要32B对齐。</li><li>类型为GlobalTensor时，存储位置为Global Memory，目的地址需要1B对齐。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002534928976_row4956154125018"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002534928976_p142871414131614"><a name="zh-cn_topic_0000002534928976_p142871414131614"></a><a name="zh-cn_topic_0000002534928976_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002534928976_p628711148165"><a name="zh-cn_topic_0000002534928976_p628711148165"></a><a name="zh-cn_topic_0000002534928976_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002534928976_p461180132619"><a name="zh-cn_topic_0000002534928976_p461180132619"></a><a name="zh-cn_topic_0000002534928976_p461180132619"></a>源操作数。</p>
<a name="zh-cn_topic_0000002534928976_ul174993262613"></a><a name="zh-cn_topic_0000002534928976_ul174993262613"></a><ul id="zh-cn_topic_0000002534928976_ul174993262613"><li>类型为GlobalTensor时，存储位置为Global Memory，源地址需要1B对齐。</li><li>类型为LocalTensor时，存储位置为Unified Buffer，源地址需要32B对齐。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002534928976_row137841716478"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002534928976_p17785197124710"><a name="zh-cn_topic_0000002534928976_p17785197124710"></a><a name="zh-cn_topic_0000002534928976_p17785197124710"></a>count</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002534928976_p8785974471"><a name="zh-cn_topic_0000002534928976_p8785974471"></a><a name="zh-cn_topic_0000002534928976_p8785974471"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002534928976_p9785117184720"><a name="zh-cn_topic_0000002534928976_p9785117184720"></a><a name="zh-cn_topic_0000002534928976_p9785117184720"></a>参与搬运的元素个数。count * sizeof(T)需要32B对齐， 若未对齐，则搬运量会向下取整到32B对齐。</p>
</td>
</tr>
</tbody>
</table>

以half数据类型为例，源操作数的shape为1 \* 128。当count = 128时，[图1](#zh-cn_topic_0000002534928976_fig54801848104717)将源操作数中128个元素连续搬运至目的操作数。

**图 1**  连续搬运示意图<a name="zh-cn_topic_0000002534928976_fig54801848104717"></a>  
![](figures/连续搬运示意图.png "连续搬运示意图")

## 数据类型<a name="zh-cn_topic_0000002534928976_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

## API映射关系<a name="zh-cn_topic_0000002534928976_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（copy\_gm\_to\_ubuf、copy\_ubuf\_to\_gm）的基础上进行抽象封装实现的，其对应的底层built-in接口为：

```
void copy_gm_to_ubuf(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride);

void copy_ubuf_to_gm(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride);
```

## 约束说明<a name="zh-cn_topic_0000002534928976_section2045914466492"></a>

-   位于Global Memory的地址必须1B对齐，位于Unified Buffer的地址必须32B对齐。
-   调用连续搬运接口时，count \* sizeof\(T\)需要32B对齐， 若未对齐，则搬运量会向下取整到32B对齐。

