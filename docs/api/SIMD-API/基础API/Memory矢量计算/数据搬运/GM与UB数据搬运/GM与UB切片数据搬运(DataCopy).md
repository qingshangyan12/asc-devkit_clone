# GM与UB切片数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002568770166"></a>

## 产品支持情况<a name="zh-cn_topic_0000002565968945_section796754519912"></a>

<a name="zh-cn_topic_0000002565968945_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565968945_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002565968945_p1883113061818"><a name="zh-cn_topic_0000002565968945_p1883113061818"></a><a name="zh-cn_topic_0000002565968945_p1883113061818"></a><span id="zh-cn_topic_0000002565968945_ph20833205312295"><a name="zh-cn_topic_0000002565968945_ph20833205312295"></a><a name="zh-cn_topic_0000002565968945_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002565968945_p783113012187"><a name="zh-cn_topic_0000002565968945_p783113012187"></a><a name="zh-cn_topic_0000002565968945_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody></tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002565968945_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

支持数据的切片搬运，提取多维Tensor数据的子集进行搬运。

## 函数原型<a name="zh-cn_topic_0000002565968945_section82039854412"></a>

-   GM-\>UB

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[], const uint32_t dimValue = 1)
    ```

-   UB-\>GM

    ```
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T> &dst, const LocalTensor<T> &src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[], const uint32_t dimValue = 1)
    ```

## 参数说明<a name="zh-cn_topic_0000002565968945_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002565968945_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565968945_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002565968945_p10961458104110"><a name="zh-cn_topic_0000002565968945_p10961458104110"></a><a name="zh-cn_topic_0000002565968945_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002565968945_p6961155817415"><a name="zh-cn_topic_0000002565968945_p6961155817415"></a><a name="zh-cn_topic_0000002565968945_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565968945_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968945_p29619586417"><a name="zh-cn_topic_0000002565968945_p29619586417"></a><a name="zh-cn_topic_0000002565968945_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968945_p996110583417"><a name="zh-cn_topic_0000002565968945_p996110583417"></a><a name="zh-cn_topic_0000002565968945_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002565968945_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565968945_row15956194105014"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002565968945_p7956144195014"><a name="zh-cn_topic_0000002565968945_p7956144195014"></a><a name="zh-cn_topic_0000002565968945_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002565968945_p1295624145013"><a name="zh-cn_topic_0000002565968945_p1295624145013"></a><a name="zh-cn_topic_0000002565968945_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002565968945_p16956144145011"><a name="zh-cn_topic_0000002565968945_p16956144145011"></a><a name="zh-cn_topic_0000002565968945_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565968945_row5956546509"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565968945_p19287714181617"><a name="zh-cn_topic_0000002565968945_p19287714181617"></a><a name="zh-cn_topic_0000002565968945_p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565968945_p192871614151615"><a name="zh-cn_topic_0000002565968945_p192871614151615"></a><a name="zh-cn_topic_0000002565968945_p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565968945_p16287121461618"><a name="zh-cn_topic_0000002565968945_p16287121461618"></a><a name="zh-cn_topic_0000002565968945_p16287121461618"></a>目的操作数。</p>
<a name="zh-cn_topic_0000002565968945_ul17617194613257"></a><a name="zh-cn_topic_0000002565968945_ul17617194613257"></a><ul id="zh-cn_topic_0000002565968945_ul17617194613257"><li>类型为LocalTensor时，存储位置为Unified Buffer，目的地址需要32B对齐。</li><li>类型为GlobalTensor时，存储位置为Global Memory，目的地址需要1B对齐。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968945_row4956154125018"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565968945_p142871414131614"><a name="zh-cn_topic_0000002565968945_p142871414131614"></a><a name="zh-cn_topic_0000002565968945_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565968945_p628711148165"><a name="zh-cn_topic_0000002565968945_p628711148165"></a><a name="zh-cn_topic_0000002565968945_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565968945_p0287191420164"><a name="zh-cn_topic_0000002565968945_p0287191420164"></a><a name="zh-cn_topic_0000002565968945_p0287191420164"></a>源操作数。</p>
<a name="zh-cn_topic_0000002565968945_ul174993262613"></a><a name="zh-cn_topic_0000002565968945_ul174993262613"></a><ul id="zh-cn_topic_0000002565968945_ul174993262613"><li>类型为GlobalTensor时，存储位置为Global Memory，源地址需要1B对齐。</li><li>类型为LocalTensor时，存储位置为Unified Buffer，源地址需要32B对齐。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968945_row187261835102011"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565968945_p1372633532019"><a name="zh-cn_topic_0000002565968945_p1372633532019"></a><a name="zh-cn_topic_0000002565968945_p1372633532019"></a>dstSliceInfo</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565968945_p97261435142012"><a name="zh-cn_topic_0000002565968945_p97261435142012"></a><a name="zh-cn_topic_0000002565968945_p97261435142012"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565968945_p072623511206"><a name="zh-cn_topic_0000002565968945_p072623511206"></a><a name="zh-cn_topic_0000002565968945_p072623511206"></a>源操作数切片信息，类型为SliceInfo。通过改参数可以配置切片的起始和终止元素个数、间隔、长度等信息。</p>
<p id="zh-cn_topic_0000002565968945_p18226206122318"><a name="zh-cn_topic_0000002565968945_p18226206122318"></a><a name="zh-cn_topic_0000002565968945_p18226206122318"></a>SliceInfo参数说明请参考。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968945_row25611637122020"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565968945_p5561537192013"><a name="zh-cn_topic_0000002565968945_p5561537192013"></a><a name="zh-cn_topic_0000002565968945_p5561537192013"></a>srcSliceInfo</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565968945_p1656133782010"><a name="zh-cn_topic_0000002565968945_p1656133782010"></a><a name="zh-cn_topic_0000002565968945_p1656133782010"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565968945_p195611437192019"><a name="zh-cn_topic_0000002565968945_p195611437192019"></a><a name="zh-cn_topic_0000002565968945_p195611437192019"></a>目的操作数切片信息，类型为SliceInfo。通过改参数可以配置切片的起始和终止元素个数、间隔、长度等信息。SliceInfo参数说明请参考。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968945_row1075785651510"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002565968945_p1728791441620"><a name="zh-cn_topic_0000002565968945_p1728791441620"></a><a name="zh-cn_topic_0000002565968945_p1728791441620"></a>dimValue</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002565968945_p11287151451610"><a name="zh-cn_topic_0000002565968945_p11287151451610"></a><a name="zh-cn_topic_0000002565968945_p11287151451610"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002565968945_p25431398275"><a name="zh-cn_topic_0000002565968945_p25431398275"></a><a name="zh-cn_topic_0000002565968945_p25431398275"></a>操作数维度信息，默认值为1。</p>
</td>
</tr>
</tbody>
</table>

**表 3**  SliceInfo结构体参数定义

<a name="zh-cn_topic_0000002565968945_table144203616291"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002565968945_row742014692919"><th class="cellrowborder" valign="top" width="12.959999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002565968945_p1242012617291"><a name="zh-cn_topic_0000002565968945_p1242012617291"></a><a name="zh-cn_topic_0000002565968945_p1242012617291"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="87.03999999999999%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002565968945_p1342018622916"><a name="zh-cn_topic_0000002565968945_p1342018622916"></a><a name="zh-cn_topic_0000002565968945_p1342018622916"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002565968945_row1642018613291"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968945_p42771239121712"><a name="zh-cn_topic_0000002565968945_p42771239121712"></a><a name="zh-cn_topic_0000002565968945_p42771239121712"></a>startIndex</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968945_p113258476173"><a name="zh-cn_topic_0000002565968945_p113258476173"></a><a name="zh-cn_topic_0000002565968945_p113258476173"></a>切片的起始元素位置，uint32_t类型，单位是元素个数。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968945_row174201161298"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968945_p12778396171"><a name="zh-cn_topic_0000002565968945_p12778396171"></a><a name="zh-cn_topic_0000002565968945_p12778396171"></a>endIndex</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968945_p1632634715178"><a name="zh-cn_topic_0000002565968945_p1632634715178"></a><a name="zh-cn_topic_0000002565968945_p1632634715178"></a>切片的终止元素位置，uint32_t类型，单位是元素个数。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968945_row1542086182913"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968945_p6277143991716"><a name="zh-cn_topic_0000002565968945_p6277143991716"></a><a name="zh-cn_topic_0000002565968945_p6277143991716"></a>stride</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968945_p432674771713"><a name="zh-cn_topic_0000002565968945_p432674771713"></a><a name="zh-cn_topic_0000002565968945_p432674771713"></a>切片的间隔元素个数，uint32_t类型。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968945_row13420361297"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968945_p17277123915179"><a name="zh-cn_topic_0000002565968945_p17277123915179"></a><a name="zh-cn_topic_0000002565968945_p17277123915179"></a>burstLen</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968945_p16442099222"><a name="zh-cn_topic_0000002565968945_p16442099222"></a><a name="zh-cn_topic_0000002565968945_p16442099222"></a>横向切片，每一片数据的长度，仅在dimValue = 1时生效，超出1维的情况下，必须配置为1，不支持配置成其他值。uint32_t类型，单位：datablock（32B）。</p>
<p id="zh-cn_topic_0000002565968945_p1490419231327"><a name="zh-cn_topic_0000002565968945_p1490419231327"></a><a name="zh-cn_topic_0000002565968945_p1490419231327"></a>比如，srcSliceInfo的List为 {{16, 70, 7, 3, 87},  {0, 2, 1, 1, 3}}，{16, 70, 7, 3, 87}表示第一维的切片信息，burstLen设置为3，表示一个切片数据段大小为3个datablock； {0, 2, 1, 1, 3}为第二维的切片信息，burstLen仅能设置为1。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002565968945_row72945110259"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002565968945_p72601617193111"><a name="zh-cn_topic_0000002565968945_p72601617193111"></a><a name="zh-cn_topic_0000002565968945_p72601617193111"></a>shapeValue</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002565968945_p426051714315"><a name="zh-cn_topic_0000002565968945_p426051714315"></a><a name="zh-cn_topic_0000002565968945_p426051714315"></a>当前维度的原始长度。uint32_t类型，单位为元素个数。</p>
</td>
</tr>
</tbody>
</table>

## 二维切片搬运<a name="zh-cn_topic_0000002565968945_section13815175812385"></a>

如下图所示，以float数据类型为例，切片搬运接口能从shape为3\*87的源操作中，选取四个不连续的横向切片搬运到目的操作数中，下面结合图示对SliceInfo结构体参数进行解析：

**图 1**  二维切片搬运示意图<a name="zh-cn_topic_0000002565968945_fig422819202304"></a>  

![](figures/切片数据搬运示意图.png)

-   dimValue为2，表示操作数有2维。
-   srcSliceInfo为 \{\{16, 70, 7, 3, 87\},  \{0, 2, 1, 1, 3\}\}
    -   \{16, 70, 7, 3, 87\}是针对单独一行， 即从一维的角度来配置，每个元素代表一个数：

        **startIndex**  = 16，表示有效数据段从第16个数开始；

        **endIndex**  = 70，表示有效数据段到第70个数结束；

        **stride**  = 7，单位为元素个数，表示相邻的2个切片数据段间隔的元素个数，为7个0的间距；

        **burstLen**  = 3，单位为32B，表示在这一个有效数据段中，一个切片数据段大小为3个datablock；

        **shapeValue**  = 87，表示单独一行的长度，单位为元素个数，即 8 \* 10 + 7 = 87个元素。

    -   \{0, 2, 1, 1, 3\}是针对多行，即从二维的角度来配置，每个元素代表一行：

        **startIndex**  = 0，表示有效数据段从第0行开始；

        **endIndex**  = 2，表示有效数据段到第2行结束；

        **stride**  = 1，表示相邻的2个切片数据段中间隔元素为1行；

        **burstLen**  = 1，在dimValue \> 1时必须填为1；

        **shapeValue**  = 3，表明一共有3行。

-   dstSliceInfo为\{\{0, 47, 0, 3, 48\}, \{0, 1, 0, 1, 2\}\}
    -   \{0, 47, 0, 3, 48\}是针对单独一行， 即从一维的角度来配置，每个元素代表一个数：

        **startIndex**  = 0，表示有效数据段从第0个数开始；

        **endIndex**  = 47，表示有效数据段到第47个数结束；

        **stride**  = 0，单位为元素个数，表示相邻的2个切片数据段间隔的元素个数，为0表示两个切片数据段没有间距；

        **burstLen**  = 3，单位为32B，表示在这一个有效数据段中，一个切片数据段大小为3个datablock；

        **shapeValue**  = 48，表示单独一行的长度，单位为元素个数，即8 \* 6 = 48个元素。

    -   \{0, 1, 0, 1, 2\} 是针对多行，即从二维的角度来配置，每个元素代表1行：

        **startIndex **= 0，表示有效数据段从第0行开始；

        **endIndex **= 1，表示有效数据段到第1行结束；

        **stride **= 0，表示相邻的2个切片数据段没有间隔；

        **burstLen**  = 1，在dimValue \> 1时必须填为1；

        **shapeValue **= 2，表示一共有2行。

## 数据类型<a name="zh-cn_topic_0000002565968945_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持数据类型为：int8\_t、uint8\_t、int16\_t、uint16\_t、int32\_t、uint32\_t、half、bfloat16\_t、float

Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持数据类型为：int8\_t、uint8\_t、int16\_t、uint16\_t、int32\_t、uint32\_t、half、bfloat16\_t、float

## API映射关系<a name="zh-cn_topic_0000002565968945_section5469132317498"></a>

该接口属于软仿实现，无直接映射关系。涉及以下built-in指令的使用：

```
// GM -> UB
void copy_gm_to_ubuf(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride);

void copy_gm_to_ubuf_align_b8(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_gm_to_ubuf_align_b16(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_gm_to_ubuf_align_b32(__ubuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

// UB -> GM
void copy_ubuf_to_gm(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride);

void copy_ubuf_to_gm_align_b8(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

void copy_ubuf_to_gm_align_b16(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_tsrcGap, uint32_t dstGap);

void copy_ubuf_to_gm_align_b32(__gm__ void *dst, __ubuf__ void *src, uint8_t sid, uint16_t nBurst, uint32_t lenBurst, uint8_t leftPaddingNum, uint8_t rightPaddingNum, uint32_t srcGap, uint32_t dstGap);

```

## 约束说明<a name="zh-cn_topic_0000002565968945_section2045914466492"></a>

-   位于Global Memory的地址必须1B对齐，位于Unified Buffer的地址必须32B对齐。
-   burstLen，仅在dimValue = 1时生效，超出1维的情况下，必须配置为1，不支持配置成其他值。

-   切片数据搬运中的横向burstLen大小设置，需要用户自己通过计算：横向切片元素个数\* sizeof\(T\)/32byte。横向切片元素个数\* sizeof\(T\)的大小必须是32byte的倍数。
-   SliceInfo 结构体的成员变量 startIndex、endIndex、stride、burstLen的取值须满足：

    ![](figures/zh-cn_formulaimage_0000002567536318.png)

-   SliceInfo 结构体的成员变量 startIndex、endIndex、shapeValue的取值须满足：![](figures/zh-cn_formulaimage_0000002567581556.png)
-   切片数据搬运中的SliceInfo结构体数组大小和dimValue需要保持一致，并且不超过8。
-   切片数据搬运中的srcSliceInfo结构体数组大小的和dstSliceInfo的结构体数组大小需要保持一致，两者的结构体中的burstLen需要相等（srcSliceInfo\[i\].burstLen = dstSliceInfo\[i\].burstLen）。
-   切片数据搬运对参数有一定要求，建议使用者参考调用示例，并在CPU上仿真结果无误后，再到NPU侧执行。

## 理论性能说明<a name="zh-cn_topic_0000002565968945_section11310948115013"></a>

软仿接口不涉及理论性能。

## 调用示例<a name="zh-cn_topic_0000002565968945_section088124295117"></a>

待补充
