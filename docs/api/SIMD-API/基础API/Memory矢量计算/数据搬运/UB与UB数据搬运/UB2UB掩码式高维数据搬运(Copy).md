# UB2UB掩码式高维数据搬运(Copy)<a name="ZH-CN_TOPIC_0000002575088175"></a>

## 产品支持情况<a name="zh-cn_topic_0000002567699435_section796754519912"></a>

<a name="zh-cn_topic_0000002567699435_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002567699435_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002567699435_p1883113061818"><a name="zh-cn_topic_0000002567699435_p1883113061818"></a><a name="zh-cn_topic_0000002567699435_p1883113061818"></a><span id="zh-cn_topic_0000002567699435_ph20833205312295"><a name="zh-cn_topic_0000002567699435_ph20833205312295"></a><a name="zh-cn_topic_0000002567699435_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002567699435_p783113012187"><a name="zh-cn_topic_0000002567699435_p783113012187"></a><a name="zh-cn_topic_0000002567699435_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody></tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002567699435_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

DataCopy能够实现Unified Buffer和Unified Buffer之间的数据搬运，数据搬运时格式和内容保持不变，支持mask操作和DataBlock间隔操作。

## 函数原型<a name="zh-cn_topic_0000002567699435_section82039854412"></a>

-   mask逐bit模式

    ```
    template <typename T, bool isSetMask = true>
    __aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask[], const uint8_t repeatTime, const CopyRepeatParams& repeatParams)
    ```

-   mask连续模式

    ```
    template <typename T, bool isSetMask = true>
    __aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask, const uint8_t repeatTime, const CopyRepeatParams& repeatParams)
    ```

## 参数说明<a name="zh-cn_topic_0000002567699435_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002567699435_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002567699435_row996115820412"><th class="cellrowborder" valign="top" width="16.470000000000002%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002567699435_p10961458104110"><a name="zh-cn_topic_0000002567699435_p10961458104110"></a><a name="zh-cn_topic_0000002567699435_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="83.53%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002567699435_p6961155817415"><a name="zh-cn_topic_0000002567699435_p6961155817415"></a><a name="zh-cn_topic_0000002567699435_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002567699435_row4961205811416"><td class="cellrowborder" valign="top" width="16.470000000000002%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002567699435_p29619586417"><a name="zh-cn_topic_0000002567699435_p29619586417"></a><a name="zh-cn_topic_0000002567699435_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="83.53%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002567699435_p996110583417"><a name="zh-cn_topic_0000002567699435_p996110583417"></a><a name="zh-cn_topic_0000002567699435_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002567699435_section4219135304818">数据类型</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699435_row170312439218"><td class="cellrowborder" valign="top" width="16.470000000000002%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002567699435_p1523033018174"><a name="zh-cn_topic_0000002567699435_p1523033018174"></a><a name="zh-cn_topic_0000002567699435_p1523033018174"></a>isSetMask</p>
</td>
<td class="cellrowborder" valign="top" width="83.53%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002567699435_p4230030161711"><a name="zh-cn_topic_0000002567699435_p4230030161711"></a><a name="zh-cn_topic_0000002567699435_p4230030161711"></a>是否在接口内部设置mask。</p>
<a name="zh-cn_topic_0000002567699435_ul5426332122212"></a><a name="zh-cn_topic_0000002567699435_ul5426332122212"></a><ul id="zh-cn_topic_0000002567699435_ul5426332122212"><li>true，表示在接口内部设置mask。</li><li>false，表示在接口外部设置mask，开发者需要使用<a href="SetVectorMask.md#ZH-CN_TOPIC_0000002574024783">SetVectorMask</a>接口设置mask值。这种模式下，本接口入参中的mask值必须设置为占位符MASK_PLACEHOLDER。</li></ul>
<p id="zh-cn_topic_0000002567699435_p1477124811291"><a name="zh-cn_topic_0000002567699435_p1477124811291"></a><a name="zh-cn_topic_0000002567699435_p1477124811291"></a>具体使用方式可参考<a href="掩码.md#ZH-CN_TOPIC_0000002573904669">掩码</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002567699435_table1055216132132"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002567699435_row105531513121315"><th class="cellrowborder" valign="top" width="16.49%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002567699435_p5553171319138"><a name="zh-cn_topic_0000002567699435_p5553171319138"></a><a name="zh-cn_topic_0000002567699435_p5553171319138"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="11.93%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002567699435_p5553151313131"><a name="zh-cn_topic_0000002567699435_p5553151313131"></a><a name="zh-cn_topic_0000002567699435_p5553151313131"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="71.58%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002567699435_p655316136139"><a name="zh-cn_topic_0000002567699435_p655316136139"></a><a name="zh-cn_topic_0000002567699435_p655316136139"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002567699435_row5553201314135"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002567699435_p8553813111314"><a name="zh-cn_topic_0000002567699435_p8553813111314"></a><a name="zh-cn_topic_0000002567699435_p8553813111314"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002567699435_p755318134134"><a name="zh-cn_topic_0000002567699435_p755318134134"></a><a name="zh-cn_topic_0000002567699435_p755318134134"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002567699435_p16287121461618"><a name="zh-cn_topic_0000002567699435_p16287121461618"></a><a name="zh-cn_topic_0000002567699435_p16287121461618"></a>目的操作数，类型为LocalTensor，存储位置为Unified Buffer，目的地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699435_row6553613191315"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002567699435_p195531113161311"><a name="zh-cn_topic_0000002567699435_p195531113161311"></a><a name="zh-cn_topic_0000002567699435_p195531113161311"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002567699435_p155310135134"><a name="zh-cn_topic_0000002567699435_p155310135134"></a><a name="zh-cn_topic_0000002567699435_p155310135134"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002567699435_p0287191420164"><a name="zh-cn_topic_0000002567699435_p0287191420164"></a><a name="zh-cn_topic_0000002567699435_p0287191420164"></a>源操作数，类型为LocalTensor，存储位置为Unified Buffer，源地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699435_row645416263812"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002567699435_p3354163214812"><a name="zh-cn_topic_0000002567699435_p3354163214812"></a><a name="zh-cn_topic_0000002567699435_p3354163214812"></a>mask[]/mask</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002567699435_p43545322818"><a name="zh-cn_topic_0000002567699435_p43545322818"></a><a name="zh-cn_topic_0000002567699435_p43545322818"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002567699435_p94543261085"><a name="zh-cn_topic_0000002567699435_p94543261085"></a><a name="zh-cn_topic_0000002567699435_p94543261085"></a>mask用于控制每次迭代内参与计算的元素。详细设置参考<a href="掩码.md#ZH-CN_TOPIC_0000002573904669">掩码</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699435_row185542138131"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002567699435_p755471321311"><a name="zh-cn_topic_0000002567699435_p755471321311"></a><a name="zh-cn_topic_0000002567699435_p755471321311"></a>repeatTime</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002567699435_p135541313101314"><a name="zh-cn_topic_0000002567699435_p135541313101314"></a><a name="zh-cn_topic_0000002567699435_p135541313101314"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002567699435_p459215246418"><a name="zh-cn_topic_0000002567699435_p459215246418"></a><a name="zh-cn_topic_0000002567699435_p459215246418"></a>重复迭代次数。矢量计算单元，每次读取连续的256Bytes数据进行计算，为完成对输入数据的处理，必须通过多次迭代（repeat）才能完成所有数据的读取与计算。repeatTime表示迭代的次数。</p>
<p id="zh-cn_topic_0000002567699435_p203197325715"><a name="zh-cn_topic_0000002567699435_p203197325715"></a><a name="zh-cn_topic_0000002567699435_p203197325715"></a>关于该参数的具体描述请参考<a href="高维切分.md#ZH-CN_TOPIC_0000002556644319">高维切分</a>。</p>
<p id="zh-cn_topic_0000002567699435_p552793416534"><a name="zh-cn_topic_0000002567699435_p552793416534"></a><a name="zh-cn_topic_0000002567699435_p552793416534"></a><strong id="zh-cn_topic_0000002567699435_b874163919531"><a name="zh-cn_topic_0000002567699435_b874163919531"></a><a name="zh-cn_topic_0000002567699435_b874163919531"></a>注：repeatTime = 0表示不会执行计算操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699435_row76214291884"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002567699435_p1137017353818"><a name="zh-cn_topic_0000002567699435_p1137017353818"></a><a name="zh-cn_topic_0000002567699435_p1137017353818"></a>repeatParams</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002567699435_p73709352081"><a name="zh-cn_topic_0000002567699435_p73709352081"></a><a name="zh-cn_topic_0000002567699435_p73709352081"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002567699435_p136219291384"><a name="zh-cn_topic_0000002567699435_p136219291384"></a><a name="zh-cn_topic_0000002567699435_p136219291384"></a>控制操作数地址步长的参数。CopyRepeatParams类型，包含操作数相邻迭代间相同DataBlock的地址步长，操作数同一迭代内不同DataBlock的地址步长等参数。CopyRepeatParams参数说明请参考<a href="#zh-cn_topic_0000002567699435_table992513120142">表3</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 3**  CopyRepeatParams结构体参数定义

<a name="zh-cn_topic_0000002567699435_table992513120142"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002567699435_row1192673141420"><th class="cellrowborder" valign="top" width="16.05%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002567699435_p1242012617291"><a name="zh-cn_topic_0000002567699435_p1242012617291"></a><a name="zh-cn_topic_0000002567699435_p1242012617291"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="83.95%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002567699435_p1342018622916"><a name="zh-cn_topic_0000002567699435_p1342018622916"></a><a name="zh-cn_topic_0000002567699435_p1342018622916"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002567699435_row129269314142"><td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002567699435_p2420106192913"><a name="zh-cn_topic_0000002567699435_p2420106192913"></a><a name="zh-cn_topic_0000002567699435_p2420106192913"></a>dstStride</p>
</td>
<td class="cellrowborder" valign="top" width="83.95%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002567699435_p1297153919251"><a name="zh-cn_topic_0000002567699435_p1297153919251"></a><a name="zh-cn_topic_0000002567699435_p1297153919251"></a>目的操作数同一迭代内相邻DataBlock的地址步长，uint16_t类型，取值范围：dstStride∈[0, 65535]，单位：DataBlock（32B）。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699435_row16926331191414"><td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002567699435_p1742010618293"><a name="zh-cn_topic_0000002567699435_p1742010618293"></a><a name="zh-cn_topic_0000002567699435_p1742010618293"></a>srcStride</p>
</td>
<td class="cellrowborder" valign="top" width="83.95%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002567699435_p11780174752617"><a name="zh-cn_topic_0000002567699435_p11780174752617"></a><a name="zh-cn_topic_0000002567699435_p11780174752617"></a>源操作数同一迭代内相邻DataBlock的地址步长，uint16_t类型，取值范围：srcStride∈[0, 65535]，单位：DataBlock（32B）</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699435_row39268314144"><td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002567699435_p104209612298"><a name="zh-cn_topic_0000002567699435_p104209612298"></a><a name="zh-cn_topic_0000002567699435_p104209612298"></a>dstRepeatSize</p>
</td>
<td class="cellrowborder" valign="top" width="83.95%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002567699435_p10146125312186"><a name="zh-cn_topic_0000002567699435_p10146125312186"></a><a name="zh-cn_topic_0000002567699435_p10146125312186"></a>目的操作数相邻迭代间相同DataBlock的地址步长，uint16_t类型，取值范围：dstRepeatSize∈[0, 4095]，单位：DataBlock（32B）。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002567699435_row4926203115149"><td class="cellrowborder" valign="top" width="16.05%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002567699435_p642016112920"><a name="zh-cn_topic_0000002567699435_p642016112920"></a><a name="zh-cn_topic_0000002567699435_p642016112920"></a>srcRepeatSize</p>
</td>
<td class="cellrowborder" valign="top" width="83.95%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002567699435_zh-cn_topic_0000002267504656_p104891468211"><a name="zh-cn_topic_0000002567699435_zh-cn_topic_0000002267504656_p104891468211"></a><a name="zh-cn_topic_0000002567699435_zh-cn_topic_0000002267504656_p104891468211"></a>源操作数相邻迭代间相同DataBlock的地址步长，uint16_t类型，取值范围：srcRepeatSize∈[0, 4095]，单位：DataBlock（32B）。</p>
</td>
</tr>
</tbody>
</table>

## 数据类型<a name="zh-cn_topic_0000002567699435_section4219135304818"></a>

源操作数和目的操作数支持的数据类型保持一致。

## API映射关系<a name="zh-cn_topic_0000002567699435_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（vcopy）的基础上进行抽象封装实现的，其对应的底层built-in接口为：

```
void vcopy(__ubuf__ int16_t *dst, __ubuf__ int16_t *src, uint8_t repeat, uint16_t dstStride, uint16_t srcStride, uint16_t dstRepeatSize, uint16_t srcRepeatSize);

void vcopy(__ubuf__ int32_t *dst, __ubuf__ int32_t *src, uint8_t repeat, uint16_t dstStride, uint16_t srcStride, uint16_t dstRepeatSize, uint16_t srcRepeatSize);

void vcopy(__ubuf__ uint16_t *dst, __ubuf__ uint16_t *src, uint8_t repeat, uint16_t dstStride, uint16_t srcStride, uint16_t dstRepeatSize, uint16_t srcRepeatSize);

void vcopy(__ubuf__ uint32_t *dst, __ubuf__ uint32_t *src, uint8_t repeat, uint16_t dstStride, uint16_t srcStride, uint16_t dstRepeatSize, uint16_t srcRepeatSize);
```

## 约束说明<a name="zh-cn_topic_0000002567699435_section2045914466492"></a>

-   地址对齐约束参考[地址对齐约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section796754519912)。
-   操作数地址重叠约束请参考[地址重叠约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section668772811100)。
-   repeatTime = 0表示不会执行计算操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。
-   Copy和矢量计算API一样，支持和掩码操作API配合使用。但counter模式配合高维切分计算API时，和[通用的counter模式](接口内设置mask.md#ZH-CN_TOPIC_0000002543264752)有一定差异。具体差异如下：
    -   通用的counter模式：Mask代表**整个矢量计算参与计算的元素个数，迭代次数不生效**。
    -   Copy高维切分计算API的counter模式：Mask代表**每次Repeat中处理的元素个数，迭代次数生效。**

## 调用示例<a name="zh-cn_topic_0000002567699435_section088124295117"></a>

完整示例可参考[Copy样例](https://gitcode.com/cann/asc-devkit/tree/016b968f0528e78580ba95d34cf28faad80fcc96/examples/02_features/07_data_movement/copy)。

