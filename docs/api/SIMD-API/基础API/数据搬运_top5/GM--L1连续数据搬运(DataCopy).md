# GM-\>L1连续数据搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002569070903"></a>

## 产品支持情况<a name="zh-cn_topic_0000002535739034_section796754519912"></a>

<a name="zh-cn_topic_0000002535739034_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002535739034_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002535739034_p1883113061818"><a name="zh-cn_topic_0000002535739034_p1883113061818"></a><a name="zh-cn_topic_0000002535739034_p1883113061818"></a><span id="zh-cn_topic_0000002535739034_ph20833205312295"><a name="zh-cn_topic_0000002535739034_ph20833205312295"></a><a name="zh-cn_topic_0000002535739034_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002535739034_p783113012187"><a name="zh-cn_topic_0000002535739034_p783113012187"></a><a name="zh-cn_topic_0000002535739034_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002535739034_row75812513379"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002535739034_p17301775812"><a name="zh-cn_topic_0000002535739034_p17301775812"></a><a name="zh-cn_topic_0000002535739034_p17301775812"></a><span id="zh-cn_topic_0000002535739034_ph2272194216543"><a name="zh-cn_topic_0000002535739034_ph2272194216543"></a><a name="zh-cn_topic_0000002535739034_ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002535739034_p37256491200"><a name="zh-cn_topic_0000002535739034_p37256491200"></a><a name="zh-cn_topic_0000002535739034_p37256491200"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002535739034_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

该接口能够将矩阵从Global Memory连续搬运至L1 Buffer，数据搬运时格式和内容保持不变。

## 函数原型<a name="zh-cn_topic_0000002535739034_section82039854412"></a>

```
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const uint32_t count)
```

## 参数说明<a name="zh-cn_topic_0000002535739034_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002535739034_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002535739034_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002535739034_p10961458104110"><a name="zh-cn_topic_0000002535739034_p10961458104110"></a><a name="zh-cn_topic_0000002535739034_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002535739034_p6961155817415"><a name="zh-cn_topic_0000002535739034_p6961155817415"></a><a name="zh-cn_topic_0000002535739034_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002535739034_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002535739034_p29619586417"><a name="zh-cn_topic_0000002535739034_p29619586417"></a><a name="zh-cn_topic_0000002535739034_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002535739034_p996110583417"><a name="zh-cn_topic_0000002535739034_p996110583417"></a><a name="zh-cn_topic_0000002535739034_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002535739034_section4219135304818">数据类型</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002535739034_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002535739034_row15956194105014"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002535739034_p7956144195014"><a name="zh-cn_topic_0000002535739034_p7956144195014"></a><a name="zh-cn_topic_0000002535739034_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002535739034_p1295624145013"><a name="zh-cn_topic_0000002535739034_p1295624145013"></a><a name="zh-cn_topic_0000002535739034_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002535739034_p16956144145011"><a name="zh-cn_topic_0000002535739034_p16956144145011"></a><a name="zh-cn_topic_0000002535739034_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002535739034_row5956546509"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002535739034_p19287714181617"><a name="zh-cn_topic_0000002535739034_p19287714181617"></a><a name="zh-cn_topic_0000002535739034_p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002535739034_p192871614151615"><a name="zh-cn_topic_0000002535739034_p192871614151615"></a><a name="zh-cn_topic_0000002535739034_p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002535739034_p16287121461618"><a name="zh-cn_topic_0000002535739034_p16287121461618"></a><a name="zh-cn_topic_0000002535739034_p16287121461618"></a>目的操作数，类型为LocalTensor，存储位置为L1 Buffer，目的地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002535739034_row4956154125018"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002535739034_p142871414131614"><a name="zh-cn_topic_0000002535739034_p142871414131614"></a><a name="zh-cn_topic_0000002535739034_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002535739034_p628711148165"><a name="zh-cn_topic_0000002535739034_p628711148165"></a><a name="zh-cn_topic_0000002535739034_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002535739034_p0287191420164"><a name="zh-cn_topic_0000002535739034_p0287191420164"></a><a name="zh-cn_topic_0000002535739034_p0287191420164"></a>源操作数，类型为GlobalTensor，存储位置为Global Memory，源地址需要1B对齐。</p>
<p id="zh-cn_topic_0000002535739034_p085119513615"><a name="zh-cn_topic_0000002535739034_p085119513615"></a><a name="zh-cn_topic_0000002535739034_p085119513615"></a><a href="SetL2CacheHint.md#ZH-CN_TOPIC_0000002563688488">SetL2CacheHint</a>支持的L2 Cache控制模式如下：CacheMode::CACHE_MODE_DISABLE、CacheMode::CACHE_MODE_NORMAL</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002535739034_row137841716478"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002535739034_p17785197124710"><a name="zh-cn_topic_0000002535739034_p17785197124710"></a><a name="zh-cn_topic_0000002535739034_p17785197124710"></a>count</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002535739034_p8785974471"><a name="zh-cn_topic_0000002535739034_p8785974471"></a><a name="zh-cn_topic_0000002535739034_p8785974471"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002535739034_p9785117184720"><a name="zh-cn_topic_0000002535739034_p9785117184720"></a><a name="zh-cn_topic_0000002535739034_p9785117184720"></a>参与搬运的元素个数。count * sizeof(T)需要32B对齐， 若未对齐，则搬运量会向下取整到32B对齐。</p>
</td>
</tr>
</tbody>
</table>

以half数据类型为例，源操作数的shape为1 \* 128。当count = 128时，[图1](#zh-cn_topic_0000002535739034_fig79455329161)将源操作数中128个元素连续搬运至目的操作数。

**图 1**  连续搬运示意图<a name="zh-cn_topic_0000002535739034_fig79455329161"></a>  
![](figures/连续搬运示意图-50.png "连续搬运示意图-50")

## 数据类型<a name="zh-cn_topic_0000002535739034_section4219135304818"></a>

源矩阵和目的矩阵支持的数据类型保持一致。

Ascend 950PR/Ascend 950DT，支持数据类型为：b4（fp4x2\_e2m1\_t、fp4x2\_e1m2\_t）、b8、b16、b32、b64

## API映射关系<a name="zh-cn_topic_0000002535739034_section5469132317498"></a>

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

## 约束说明<a name="zh-cn_topic_0000002535739034_section2045914466492"></a>

-   位于Global Memory的源地址必须1B对齐，位于L1 Buffer的目的地址必须32B对齐。
-   调用连续搬运接口时，count \* sizeof\(T\)需要32B对齐， 若未对齐，则搬运量会向下取整到32B对齐。

## 理论性能说明<a name="zh-cn_topic_0000002535739034_section11310948115013"></a>

## 调用示例<a name="zh-cn_topic_0000002535739034_section088124295117"></a>

-   **[产品支持情况](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002568950899.md)**  

-   **[功能说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538231124.md)**  

-   **[函数原型](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538071200.md)**  

-   **[参数说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002569070905.md)**  

-   **[数据类型](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002568950901.md)**  

-   **[API映射关系](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538231126.md)**  

-   **[约束说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538071202.md)**  

-   **[理论性能说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002569070907.md)**  

-   **[调用示例](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002568950903.md)**  

