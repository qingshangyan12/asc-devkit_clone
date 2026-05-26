# GM-\>L1高维切分数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002538231128"></a>

## 产品支持情况<a name="zh-cn_topic_0000002566658905_section796754519912"></a>

<a name="zh-cn_topic_0000002566658905_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566658905_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002566658905_p1883113061818"><a name="zh-cn_topic_0000002566658905_p1883113061818"></a><a name="zh-cn_topic_0000002566658905_p1883113061818"></a><span id="zh-cn_topic_0000002566658905_ph20833205312295"><a name="zh-cn_topic_0000002566658905_ph20833205312295"></a><a name="zh-cn_topic_0000002566658905_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002566658905_p783113012187"><a name="zh-cn_topic_0000002566658905_p783113012187"></a><a name="zh-cn_topic_0000002566658905_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566658905_row4926156113913"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002566658905_p17301775812"><a name="zh-cn_topic_0000002566658905_p17301775812"></a><a name="zh-cn_topic_0000002566658905_p17301775812"></a><span id="zh-cn_topic_0000002566658905_ph2272194216543"><a name="zh-cn_topic_0000002566658905_ph2272194216543"></a><a name="zh-cn_topic_0000002566658905_ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002566658905_p37256491200"><a name="zh-cn_topic_0000002566658905_p37256491200"></a><a name="zh-cn_topic_0000002566658905_p37256491200"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002566658905_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

该接口主要实现将矩阵从Global Memory搬运至L1 Buffer，数据搬运时格式和内容保持不变。

高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

## 函数原型<a name="zh-cn_topic_0000002566658905_section82039854412"></a>

接口同时支持非连续搬运和连续搬运：

-   对于连续搬运场景，推荐使用[GM-\>L1连续数据搬运\(DataCopy\)](GM--L1连续数据搬运(DataCopy).md#ZH-CN_TOPIC_0000002569070903)。

```
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyParams& repeatParams)
```

## 参数说明<a name="zh-cn_topic_0000002566658905_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002566658905_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566658905_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002566658905_p10961458104110"><a name="zh-cn_topic_0000002566658905_p10961458104110"></a><a name="zh-cn_topic_0000002566658905_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002566658905_p6961155817415"><a name="zh-cn_topic_0000002566658905_p6961155817415"></a><a name="zh-cn_topic_0000002566658905_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566658905_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p29619586417"><a name="zh-cn_topic_0000002566658905_p29619586417"></a><a name="zh-cn_topic_0000002566658905_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p996110583417"><a name="zh-cn_topic_0000002566658905_p996110583417"></a><a name="zh-cn_topic_0000002566658905_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002566658905_section4219135304818">数据类型</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002566658905_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566658905_row15956194105014"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002566658905_p7956144195014"><a name="zh-cn_topic_0000002566658905_p7956144195014"></a><a name="zh-cn_topic_0000002566658905_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002566658905_p1295624145013"><a name="zh-cn_topic_0000002566658905_p1295624145013"></a><a name="zh-cn_topic_0000002566658905_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002566658905_p16956144145011"><a name="zh-cn_topic_0000002566658905_p16956144145011"></a><a name="zh-cn_topic_0000002566658905_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566658905_row5956546509"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002566658905_p19287714181617"><a name="zh-cn_topic_0000002566658905_p19287714181617"></a><a name="zh-cn_topic_0000002566658905_p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002566658905_p192871614151615"><a name="zh-cn_topic_0000002566658905_p192871614151615"></a><a name="zh-cn_topic_0000002566658905_p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002566658905_p16287121461618"><a name="zh-cn_topic_0000002566658905_p16287121461618"></a><a name="zh-cn_topic_0000002566658905_p16287121461618"></a>目的操作数，类型为LocalTensor，存储位置为L1 Buffer，目的地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566658905_row4956154125018"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002566658905_p142871414131614"><a name="zh-cn_topic_0000002566658905_p142871414131614"></a><a name="zh-cn_topic_0000002566658905_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002566658905_p628711148165"><a name="zh-cn_topic_0000002566658905_p628711148165"></a><a name="zh-cn_topic_0000002566658905_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002566658905_p0287191420164"><a name="zh-cn_topic_0000002566658905_p0287191420164"></a><a name="zh-cn_topic_0000002566658905_p0287191420164"></a>源操作数，类型为GlobalTensor，存储位置为Global Memory，源地址需要1B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566658905_row1075785651510"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002566658905_p1728791441620"><a name="zh-cn_topic_0000002566658905_p1728791441620"></a><a name="zh-cn_topic_0000002566658905_p1728791441620"></a>repeatParams</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002566658905_p11287151451610"><a name="zh-cn_topic_0000002566658905_p11287151451610"></a><a name="zh-cn_topic_0000002566658905_p11287151451610"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002566658905_p10391459201718"><a name="zh-cn_topic_0000002566658905_p10391459201718"></a><a name="zh-cn_topic_0000002566658905_p10391459201718"></a>搬运参数，类型为DataCopyParams。通过该参数可以配置搬运的数据块个数、长度、地址间隔等信息，同时支持非连续和连续搬运。</p>
<p id="zh-cn_topic_0000002566658905_p125412312537"><a name="zh-cn_topic_0000002566658905_p125412312537"></a><a name="zh-cn_topic_0000002566658905_p125412312537"></a>DataCopyParams参数说明请参考<a href="#zh-cn_topic_0000002566658905_table144203616291">表3</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 3**  DataCopyParams结构体参数定义

<a name="zh-cn_topic_0000002566658905_table144203616291"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566658905_row742014692919"><th class="cellrowborder" valign="top" width="12.959999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002566658905_p1242012617291"><a name="zh-cn_topic_0000002566658905_p1242012617291"></a><a name="zh-cn_topic_0000002566658905_p1242012617291"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="87.03999999999999%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002566658905_p1342018622916"><a name="zh-cn_topic_0000002566658905_p1342018622916"></a><a name="zh-cn_topic_0000002566658905_p1342018622916"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566658905_row1642018613291"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p2420106192913"><a name="zh-cn_topic_0000002566658905_p2420106192913"></a><a name="zh-cn_topic_0000002566658905_p2420106192913"></a>blockCount</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p1297153919251"><a name="zh-cn_topic_0000002566658905_p1297153919251"></a><a name="zh-cn_topic_0000002566658905_p1297153919251"></a>搬运的数据块个数。uint16_t类型，取值范围：blockCount∈[0, 4095]，默认值为1。</p>
<p id="zh-cn_topic_0000002566658905_p393615391617"><a name="zh-cn_topic_0000002566658905_p393615391617"></a><a name="zh-cn_topic_0000002566658905_p393615391617"></a><strong id="zh-cn_topic_0000002566658905_b1899463611015"><a name="zh-cn_topic_0000002566658905_b1899463611015"></a><a name="zh-cn_topic_0000002566658905_b1899463611015"></a>注：blockCount = 0表示不执行搬运，该接口将被视为 NOP（空操作）</strong>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566658905_row174201161298"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p1742010618293"><a name="zh-cn_topic_0000002566658905_p1742010618293"></a><a name="zh-cn_topic_0000002566658905_p1742010618293"></a>blockLen</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p11780174752617"><a name="zh-cn_topic_0000002566658905_p11780174752617"></a><a name="zh-cn_topic_0000002566658905_p11780174752617"></a>搬运的每个数据块长度。uint16_t类型，取值范围：blockLen∈[0, 65535]，单位：DataBlock（32B）。</p>
<p id="zh-cn_topic_0000002566658905_p19949155131618"><a name="zh-cn_topic_0000002566658905_p19949155131618"></a><a name="zh-cn_topic_0000002566658905_p19949155131618"></a><strong id="zh-cn_topic_0000002566658905_b918318618166"><a name="zh-cn_topic_0000002566658905_b918318618166"></a><a name="zh-cn_topic_0000002566658905_b918318618166"></a>注：blockLen = 0表示不执行搬运，该接口将被视为 NOP（空操作）</strong>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566658905_row1542086182913"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p104209612298"><a name="zh-cn_topic_0000002566658905_p104209612298"></a><a name="zh-cn_topic_0000002566658905_p104209612298"></a>srcGap</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p762563317266"><a name="zh-cn_topic_0000002566658905_p762563317266"></a><a name="zh-cn_topic_0000002566658905_p762563317266"></a>源操作数相邻数据块之间的间隔（即前一个数据块<strong id="zh-cn_topic_0000002566658905_b139293112232"><a name="zh-cn_topic_0000002566658905_b139293112232"></a><a name="zh-cn_topic_0000002566658905_b139293112232"></a>结束地址</strong>与后一个数据块<strong id="zh-cn_topic_0000002566658905_b865853942514"><a name="zh-cn_topic_0000002566658905_b865853942514"></a><a name="zh-cn_topic_0000002566658905_b865853942514"></a>起始地址</strong>的差值），uint16_t类型，取值范围：srcGap∈[0, 65535]，单位：DataBlock（32B）。</p>
<a name="zh-cn_topic_0000002566658905_ul277581716716"></a><a name="zh-cn_topic_0000002566658905_ul277581716716"></a><ul id="zh-cn_topic_0000002566658905_ul277581716716"><li>blockCount = 1时，srcGap无意义，设置为0即可。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002566658905_row13420361297"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p642016112920"><a name="zh-cn_topic_0000002566658905_p642016112920"></a><a name="zh-cn_topic_0000002566658905_p642016112920"></a>dstGap</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p5420568299"><a name="zh-cn_topic_0000002566658905_p5420568299"></a><a name="zh-cn_topic_0000002566658905_p5420568299"></a>目的操作数相邻数据块之间的间隔（即前一个数据块<strong id="zh-cn_topic_0000002566658905_b119775918341"><a name="zh-cn_topic_0000002566658905_b119775918341"></a><a name="zh-cn_topic_0000002566658905_b119775918341"></a>结束地址</strong>与后一个数据块<strong id="zh-cn_topic_0000002566658905_b1378813816359"><a name="zh-cn_topic_0000002566658905_b1378813816359"></a><a name="zh-cn_topic_0000002566658905_b1378813816359"></a>起始地址</strong>的差值），uint16_t类型，取值范围：dstGap∈[0, 65535]，单位：DataBlock（32B）。</p>
<a name="zh-cn_topic_0000002566658905_ul42062191079"></a><a name="zh-cn_topic_0000002566658905_ul42062191079"></a><ul id="zh-cn_topic_0000002566658905_ul42062191079"><li>blockCount = 1时，dstGap无意义，设置为0即可。</li></ul>
</td>
</tr>
</tbody>
</table>

## 连续搬运<a name="zh-cn_topic_0000002566658905_section3150944151518"></a>

针对连续搬运场景，推荐使用[GM-\>L1连续数据搬运\(DataCopy\)](GM--L1连续数据搬运(DataCopy).md#ZH-CN_TOPIC_0000002569070903)，以下示例仅展示高维切分数据搬运接口具有连续搬运能力。

以half数据类型为例，源操作数的shape为1 \* 128。

[图1](#zh-cn_topic_0000002566658905_fig769164701717)将源操作数中128个元素连续搬运至目的操作数，DataCopyParams结构体参数配置如下：

-   blockCount = 1，搬运1个数据块，表示连续搬运。
-   blockLen = 8，一个数据块长度为8个DataBlock。
-   srcGap = 0，源操作数相邻数据块之间的间隔为0。
-   dstGap = 0，目的操作数相邻数据块之间的间隔为0。

**图 1**  连续搬运示意图<a name="zh-cn_topic_0000002566658905_fig769164701717"></a>  
![](figures/连续搬运示意图-51.png "连续搬运示意图-51")

## 非连续搬运<a name="zh-cn_topic_0000002566658905_section1084342081618"></a>

以half数据类型为例，源操作数的shape为1 \* 112。

[图2](#zh-cn_topic_0000002566658905_fig12371910189)将源操作数中非连续数据搬运至目的操作数，DataCopyParams结构体参数配置如下：

-   blockCount= 2，搬运2个数据块。
-   blockLen= 3，一个数据块的长度为3个DataBlock。
-   srcGap= 1，源操作数相邻数据块之间的间隔为1。
-   dstGap= 2，目的操作数相邻数据块之间的间隔为2。

**图 2**  非连续搬运示意图<a name="zh-cn_topic_0000002566658905_fig12371910189"></a>  
![](figures/非连续搬运示意图-52.png "非连续搬运示意图-52")

## 数据类型<a name="zh-cn_topic_0000002566658905_section4219135304818"></a>

源矩阵和目的矩阵支持的数据类型保持一致。

Ascend 950PR/Ascend 950DT，支持数据类型为：b4（fp4x2\_e2m1\_t、fp4x2\_e1m2\_t）、b8、b16、b32、b64

## API映射关系<a name="zh-cn_topic_0000002566658905_section5469132317498"></a>

与built-in接口映射关系：

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品，

接口是在built-in接口（copy\_gm\_to\_cbuf）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
void copy_gm_to_cbuf(__cbuf__ void *dst, __gm__ void *src, uint8_t sid, uint16_t nBurst, uint16_t lenBurst, uint16_t srcStride, uint16_t dstStride, pad_t padMode);
```

针对Ascend 950PR/Ascend 950DT，

接口是在built-in接口（copy\_gm\_to\_cbuf\_align\_v2）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
// 相同接口的不同原型区别在于源地址和目的地址的数据类型不同，支持的数据类型为b8、b16、b32，其中b4和b64是通过借助 uint8_t和uint32_t类型指令实现
void copy_gm_to_cbuf_align_v2(__cbuf__ uint8_t *dst_addr, __gm__ uint8_t *src_addr, uint8_t sid, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride);
copy_gm_to_cbuf_align_v2(__cbuf__ uint16_t *dst_addr, __gm__ uint16_t *src_addr, uint8_t sid, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride);
void copy_gm_to_cbuf_align_v2(__cbuf__ uint32_t *dst_addr, __gm__ uint32_t *src_addr, uint8_t sid, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride);
```

## 约束说明<a name="zh-cn_topic_0000002566658905_section2045914466492"></a>

-   位于Global Memory的源地址必须1B对齐，位于L1 Buffer的目的地址必须32B对齐。
-   当DataCopyParams结构体参数blockCount、blockLen任意一个值为0时，表示指令不会执行，该接口将被视为 NOP（空操作）。
-   DataCopyParams结构体参数的值需在取值范围内：

    **表 4**  DataCopyParams结构体参数取值范围

    <a name="zh-cn_topic_0000002566658905_table843765314306"></a>
    <table><thead align="left"><tr id="zh-cn_topic_0000002566658905_row2043745317306"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002566658905_p1743745312309"><a name="zh-cn_topic_0000002566658905_p1743745312309"></a><a name="zh-cn_topic_0000002566658905_p1743745312309"></a>参数名称</p>
    </th>
    <th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002566658905_p124371553173016"><a name="zh-cn_topic_0000002566658905_p124371553173016"></a><a name="zh-cn_topic_0000002566658905_p124371553173016"></a>取值范围</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="zh-cn_topic_0000002566658905_row18437453183011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p17437105318308"><a name="zh-cn_topic_0000002566658905_p17437105318308"></a><a name="zh-cn_topic_0000002566658905_p17437105318308"></a>blockCount</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p1383153213319"><a name="zh-cn_topic_0000002566658905_p1383153213319"></a><a name="zh-cn_topic_0000002566658905_p1383153213319"></a>[0, 4095]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566658905_row743716536309"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p8437753103017"><a name="zh-cn_topic_0000002566658905_p8437753103017"></a><a name="zh-cn_topic_0000002566658905_p8437753103017"></a>blockLen</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p1726871205412"><a name="zh-cn_topic_0000002566658905_p1726871205412"></a><a name="zh-cn_topic_0000002566658905_p1726871205412"></a>[0, 65535]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566658905_row19437155343014"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p14437253203018"><a name="zh-cn_topic_0000002566658905_p14437253203018"></a><a name="zh-cn_topic_0000002566658905_p14437253203018"></a>srcGap</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p2437153173017"><a name="zh-cn_topic_0000002566658905_p2437153173017"></a><a name="zh-cn_topic_0000002566658905_p2437153173017"></a>[0, 65535]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566658905_row194371253133011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566658905_p843715312301"><a name="zh-cn_topic_0000002566658905_p843715312301"></a><a name="zh-cn_topic_0000002566658905_p843715312301"></a>dstGap</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566658905_p1268112545"><a name="zh-cn_topic_0000002566658905_p1268112545"></a><a name="zh-cn_topic_0000002566658905_p1268112545"></a>[0, 65535]</p>
    </td>
    </tr>
    </tbody>
    </table>

## 理论性能说明<a name="zh-cn_topic_0000002566658905_section11310948115013"></a>

## 调用示例<a name="zh-cn_topic_0000002566658905_section088124295117"></a>

-   **[产品支持情况](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538071204.md)**  

-   **[功能说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002569070909.md)**  

-   **[函数原型](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002568950905.md)**  

-   **[参数说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538231130.md)**  

-   **[数据类型](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538071206.md)**  

-   **[API映射关系](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002569070911.md)**  

-   **[约束说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002568950907.md)**  

-   **[理论性能说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538231132.md)**  

-   **[调用示例](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538071208.md)**  

