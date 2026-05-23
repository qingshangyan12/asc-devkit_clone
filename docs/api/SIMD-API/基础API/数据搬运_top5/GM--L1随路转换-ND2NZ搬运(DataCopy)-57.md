# GM-\>L1随路转换-ND2NZ搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002569070913"></a>

## 产品支持情况<a name="zh-cn_topic_0000002566538879_section796754519912"></a>

<a name="zh-cn_topic_0000002566538879_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566538879_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002566538879_p1883113061818"><a name="zh-cn_topic_0000002566538879_p1883113061818"></a><a name="zh-cn_topic_0000002566538879_p1883113061818"></a><span id="zh-cn_topic_0000002566538879_ph20833205312295"><a name="zh-cn_topic_0000002566538879_ph20833205312295"></a><a name="zh-cn_topic_0000002566538879_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002566538879_p783113012187"><a name="zh-cn_topic_0000002566538879_p783113012187"></a><a name="zh-cn_topic_0000002566538879_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566538879_row715815131398"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002566538879_p17301775812"><a name="zh-cn_topic_0000002566538879_p17301775812"></a><a name="zh-cn_topic_0000002566538879_p17301775812"></a><span id="zh-cn_topic_0000002566538879_ph2272194216543"><a name="zh-cn_topic_0000002566538879_ph2272194216543"></a><a name="zh-cn_topic_0000002566538879_ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002566538879_p37256491200"><a name="zh-cn_topic_0000002566538879_p37256491200"></a><a name="zh-cn_topic_0000002566538879_p37256491200"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002566538879_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

该接口主要实现将矩阵从Global Memory搬运至L1 Buffer，并支持在数据搬运时进行ND到NZ格式的转换。

ND到NZ的格式转换等价于DN到ZN的格式转换，如[图1](#zh-cn_topic_0000002566538879_fig444462417355)。

**图 1**  ND2NZ与DN2ZN示意图<a name="zh-cn_topic_0000002566538879_fig444462417355"></a>  
![](figures/ND2NZ与DN2ZN示意图.png "ND2NZ与DN2ZN示意图")

## 函数原型<a name="zh-cn_topic_0000002566538879_section82039854412"></a>

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const Nd2NzParams& intriParams)
```

针对Ascend 950PR/Ascend 950DT：

```
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const Nd2NzParams& intriParams)
```

## 参数说明<a name="zh-cn_topic_0000002566538879_section16128134420472"></a>

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品：

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002566538879_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566538879_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002566538879_p10961458104110"><a name="zh-cn_topic_0000002566538879_p10961458104110"></a><a name="zh-cn_topic_0000002566538879_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002566538879_p6961155817415"><a name="zh-cn_topic_0000002566538879_p6961155817415"></a><a name="zh-cn_topic_0000002566538879_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566538879_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p29619586417"><a name="zh-cn_topic_0000002566538879_p29619586417"></a><a name="zh-cn_topic_0000002566538879_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p996110583417"><a name="zh-cn_topic_0000002566538879_p996110583417"></a><a name="zh-cn_topic_0000002566538879_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002566538879_section4219135304818">数据类型</a>。</p>
</td>
</tr>
</tbody>
</table>

针对Ascend 950PR/Ascend 950DT：

**表 2**  模板参数说明

<a name="zh-cn_topic_0000002566538879_table15621101814710"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566538879_row136221218154716"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002566538879_p166227184478"><a name="zh-cn_topic_0000002566538879_p166227184478"></a><a name="zh-cn_topic_0000002566538879_p166227184478"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002566538879_p156220181476"><a name="zh-cn_topic_0000002566538879_p156220181476"></a><a name="zh-cn_topic_0000002566538879_p156220181476"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566538879_row162261864716"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p20622318154713"><a name="zh-cn_topic_0000002566538879_p20622318154713"></a><a name="zh-cn_topic_0000002566538879_p20622318154713"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p56221018114712"><a name="zh-cn_topic_0000002566538879_p56221018114712"></a><a name="zh-cn_topic_0000002566538879_p56221018114712"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="#zh-cn_topic_0000002566538879_section4219135304818">数据类型</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row19622118154710"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p36225187477"><a name="zh-cn_topic_0000002566538879_p36225187477"></a><a name="zh-cn_topic_0000002566538879_p36225187477"></a>enableSmallC0</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p15622111864714"><a name="zh-cn_topic_0000002566538879_p15622111864714"></a><a name="zh-cn_topic_0000002566538879_p15622111864714"></a>SmallC0模式开关：当dValue小于等于4的时候，C0_SIZE会补齐到4 * sizeof(T) Bytes。</p>
<p id="zh-cn_topic_0000002566538879_p7622718114716"><a name="zh-cn_topic_0000002566538879_p7622718114716"></a><a name="zh-cn_topic_0000002566538879_p7622718114716"></a>默认不开启。</p>
<p id="zh-cn_topic_0000002566538879_p562241811472"><a name="zh-cn_topic_0000002566538879_p562241811472"></a><a name="zh-cn_topic_0000002566538879_p562241811472"></a>特性细节可参考：<a href="#zh-cn_topic_0000002566538879_fig2528756123117">图4</a></p>
</td>
</tr>
</tbody>
</table>

**表 3**  参数说明

<a name="zh-cn_topic_0000002566538879_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566538879_row15956194105014"><th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002566538879_p7956144195014"><a name="zh-cn_topic_0000002566538879_p7956144195014"></a><a name="zh-cn_topic_0000002566538879_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.42084208420842%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002566538879_p1295624145013"><a name="zh-cn_topic_0000002566538879_p1295624145013"></a><a name="zh-cn_topic_0000002566538879_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.92779277927792%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002566538879_p16956144145011"><a name="zh-cn_topic_0000002566538879_p16956144145011"></a><a name="zh-cn_topic_0000002566538879_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566538879_row5956546509"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002566538879_p19287714181617"><a name="zh-cn_topic_0000002566538879_p19287714181617"></a><a name="zh-cn_topic_0000002566538879_p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002566538879_p192871614151615"><a name="zh-cn_topic_0000002566538879_p192871614151615"></a><a name="zh-cn_topic_0000002566538879_p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002566538879_p16287121461618"><a name="zh-cn_topic_0000002566538879_p16287121461618"></a><a name="zh-cn_topic_0000002566538879_p16287121461618"></a>目的操作数，目的矩阵dst，类型为LocalTensor，存储位置为L1 Buffer，目的地址需要32B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row4956154125018"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002566538879_p142871414131614"><a name="zh-cn_topic_0000002566538879_p142871414131614"></a><a name="zh-cn_topic_0000002566538879_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002566538879_p628711148165"><a name="zh-cn_topic_0000002566538879_p628711148165"></a><a name="zh-cn_topic_0000002566538879_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002566538879_p0287191420164"><a name="zh-cn_topic_0000002566538879_p0287191420164"></a><a name="zh-cn_topic_0000002566538879_p0287191420164"></a>源操作数，源矩阵src，类型为GlobalTensor，存储位置为Global Memory，目的地址需要1B对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row1075785651510"><td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002566538879_p1728791441620"><a name="zh-cn_topic_0000002566538879_p1728791441620"></a><a name="zh-cn_topic_0000002566538879_p1728791441620"></a>intriParams</p>
</td>
<td class="cellrowborder" valign="top" width="8.42084208420842%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002566538879_p11287151451610"><a name="zh-cn_topic_0000002566538879_p11287151451610"></a><a name="zh-cn_topic_0000002566538879_p11287151451610"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.92779277927792%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002566538879_p10391459201718"><a name="zh-cn_topic_0000002566538879_p10391459201718"></a><a name="zh-cn_topic_0000002566538879_p10391459201718"></a>ND2NZ搬运参数，类型为Nd2NzParams。</p>
<p id="zh-cn_topic_0000002566538879_p125412312537"><a name="zh-cn_topic_0000002566538879_p125412312537"></a><a name="zh-cn_topic_0000002566538879_p125412312537"></a>Nd2NzParams参数说明请参考<a href="#zh-cn_topic_0000002566538879_table144203616291">表4</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 4**  Nd2NzParams结构体参数定义

<a name="zh-cn_topic_0000002566538879_table144203616291"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002566538879_row742014692919"><th class="cellrowborder" valign="top" width="12.959999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002566538879_p1242012617291"><a name="zh-cn_topic_0000002566538879_p1242012617291"></a><a name="zh-cn_topic_0000002566538879_p1242012617291"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="87.03999999999999%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002566538879_p1342018622916"><a name="zh-cn_topic_0000002566538879_p1342018622916"></a><a name="zh-cn_topic_0000002566538879_p1342018622916"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002566538879_row1642018613291"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p2420106192913"><a name="zh-cn_topic_0000002566538879_p2420106192913"></a><a name="zh-cn_topic_0000002566538879_p2420106192913"></a>ndNum</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p1297153919251"><a name="zh-cn_topic_0000002566538879_p1297153919251"></a><a name="zh-cn_topic_0000002566538879_p1297153919251"></a>源矩阵中ND矩阵的数量，取值范围：ndNum∈[0, 4095]。</p>
<p id="zh-cn_topic_0000002566538879_p124209612298"><a name="zh-cn_topic_0000002566538879_p124209612298"></a><a name="zh-cn_topic_0000002566538879_p124209612298"></a><strong id="zh-cn_topic_0000002566538879_b1899463611015"><a name="zh-cn_topic_0000002566538879_b1899463611015"></a><a name="zh-cn_topic_0000002566538879_b1899463611015"></a>注：ndNum = 0表示不执行搬运，该接口将被视为 NOP（空操作）。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row174201161298"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p1742010618293"><a name="zh-cn_topic_0000002566538879_p1742010618293"></a><a name="zh-cn_topic_0000002566538879_p1742010618293"></a>nValue</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p20420116202912"><a name="zh-cn_topic_0000002566538879_p20420116202912"></a><a name="zh-cn_topic_0000002566538879_p20420116202912"></a>源矩阵中ND矩阵的行数，取值范围：nValue∈[0, 16384]。</p>
<p id="zh-cn_topic_0000002566538879_p1194618526253"><a name="zh-cn_topic_0000002566538879_p1194618526253"></a><a name="zh-cn_topic_0000002566538879_p1194618526253"></a><strong id="zh-cn_topic_0000002566538879_b18208185320255"><a name="zh-cn_topic_0000002566538879_b18208185320255"></a><a name="zh-cn_topic_0000002566538879_b18208185320255"></a>注：nValue = 0表示不执行搬运，该接口将被视为 NOP（空操作）。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row1542086182913"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p104209612298"><a name="zh-cn_topic_0000002566538879_p104209612298"></a><a name="zh-cn_topic_0000002566538879_p104209612298"></a>dValue</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p1122512982714"><a name="zh-cn_topic_0000002566538879_p1122512982714"></a><a name="zh-cn_topic_0000002566538879_p1122512982714"></a>源矩阵中ND矩阵的列数，取值范围：dValue∈[0, 65535]。当dValue * sizeof(T)不满足32B对齐时，在目的矩阵中会补0对齐到32B。</p>
<p id="zh-cn_topic_0000002566538879_p1182105922510"><a name="zh-cn_topic_0000002566538879_p1182105922510"></a><a name="zh-cn_topic_0000002566538879_p1182105922510"></a><strong id="zh-cn_topic_0000002566538879_b133079452613"><a name="zh-cn_topic_0000002566538879_b133079452613"></a><a name="zh-cn_topic_0000002566538879_b133079452613"></a>注：dValue = 0表示不执行搬运，该接口将被视为 NOP（空操作）。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row13420361297"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p642016112920"><a name="zh-cn_topic_0000002566538879_p642016112920"></a><a name="zh-cn_topic_0000002566538879_p642016112920"></a>srcNdMatrixStride</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p5420568299"><a name="zh-cn_topic_0000002566538879_p5420568299"></a><a name="zh-cn_topic_0000002566538879_p5420568299"></a>源矩阵相邻ND矩阵起始地址间的偏移，取值范围：srcNdMatrixStride∈[0, 65535]，单位：元素个数。</p>
<a name="zh-cn_topic_0000002566538879_ul244214293817"></a><a name="zh-cn_topic_0000002566538879_ul244214293817"></a><ul id="zh-cn_topic_0000002566538879_ul244214293817"><li>ndNum = 1时，srcNdMatrixStride无意义，设置为0即可。</li><li>ndNum ≠ 1时，当srcNdMatrixStride = 0时，表示重复搬出源矩阵的第一个ND矩阵。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row1054971515311"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p55491015203116"><a name="zh-cn_topic_0000002566538879_p55491015203116"></a><a name="zh-cn_topic_0000002566538879_p55491015203116"></a>srcDValue</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p25501015123117"><a name="zh-cn_topic_0000002566538879_p25501015123117"></a><a name="zh-cn_topic_0000002566538879_p25501015123117"></a>源矩阵一行中包含的元素个数，取值范围：srcDValue∈[1, 65535]，单位：元素个数。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row54901219193118"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p14906197317"><a name="zh-cn_topic_0000002566538879_p14906197317"></a><a name="zh-cn_topic_0000002566538879_p14906197317"></a>dstNzC0Stride</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p8490161953111"><a name="zh-cn_topic_0000002566538879_p8490161953111"></a><a name="zh-cn_topic_0000002566538879_p8490161953111"></a>ND转换为NZ格式后，目的NZ矩阵中相邻Z矩阵起始地址的偏移，取值范围：dstNzC0Stride∈[1, 16384]，单位：C0_SIZE（32B）。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row17568152473115"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p4568122473118"><a name="zh-cn_topic_0000002566538879_p4568122473118"></a><a name="zh-cn_topic_0000002566538879_p4568122473118"></a>dstNzNStride</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p1956892403115"><a name="zh-cn_topic_0000002566538879_p1956892403115"></a><a name="zh-cn_topic_0000002566538879_p1956892403115"></a>目的矩阵中，NZ矩阵中相邻行起始地址的偏移，取值范围：dstNzNStride∈[1, 16384]，单位：C0_SIZE（32B）。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002566538879_row3227142753113"><td class="cellrowborder" valign="top" width="12.959999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p92273271314"><a name="zh-cn_topic_0000002566538879_p92273271314"></a><a name="zh-cn_topic_0000002566538879_p92273271314"></a>dstNzMatrixStride</p>
</td>
<td class="cellrowborder" valign="top" width="87.03999999999999%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p6227122793111"><a name="zh-cn_topic_0000002566538879_p6227122793111"></a><a name="zh-cn_topic_0000002566538879_p6227122793111"></a>目的矩阵中，相邻NZ矩阵起始地址的偏移，取值范围：dstNzMatrixStride∈[0, 65535]，单位：元素个数。</p>
<a name="zh-cn_topic_0000002566538879_ul25267272811"></a><a name="zh-cn_topic_0000002566538879_ul25267272811"></a><ul id="zh-cn_topic_0000002566538879_ul25267272811"><li>ndNum = 1时，dstNzMatrixStride无意义，设置为0即可。</li><li>ndNum ≠ 1时，当dstNzMatrixStride = 0时，表示目的矩阵中搬入的每个NZ矩阵都会覆盖第一个NZ矩阵。</li></ul>
</td>
</tr>
</tbody>
</table>

## 连续搬运<a name="zh-cn_topic_0000002566538879_section7495192151713"></a>

以half数据类型为例，[图2](#zh-cn_topic_0000002566538879_fig7420114233419)为ND2NZ连续搬运示意图。Nd2NzParams结构体参数配置说明如下：

-   ndNum = 1，表示搬运的ND矩阵的数量为1。
-   nValue = 16，一个ND矩阵的行数。
-   dValue = 23，一个ND矩阵的列数。当dValue \* sizeof\(T\)不满足32B对齐时，在目的矩阵中会补0对齐到32B。
-   srcNdMatrixStride  = 0，只有一个ND矩阵，该值为0。
-   srcDValue = 32，表示源矩阵一行的所含元素个数，即32个half数据类型的元素。
-   dstNzC0Stride = 16，NZ矩阵中相邻Z矩阵起始地址的偏移，偏移为16个C0\_SIZE。
-   dstNzNStride = 1，表示NZ矩阵中相邻行起始地址的偏移。
-   dstNzMatrixStride = 0，只有一个NZ矩阵，该值为0。

**图 2**  ND2NZ转换示意图（连续搬运）<a name="zh-cn_topic_0000002566538879_fig7420114233419"></a>  
![](figures/ND2NZ转换示意图（连续搬运）.png "ND2NZ转换示意图（连续搬运）")

## 非连续搬运<a name="zh-cn_topic_0000002566538879_section1721231581717"></a>

以half数据类型为例，[图3](#zh-cn_topic_0000002566538879_fig19978184883316)为ND2NZ非连续搬运示意图。Nd2NzParams结构体参数配置说明如下：

-   ndNum = 2，表示搬运的ND矩阵的数量为2。
-   nValue = 8，一个ND矩阵的行数。
-   dValue = 23，一个ND矩阵的列数。当dValue \* sizeof\(T\)不满足32B对齐时，在目的矩阵中会补0对齐到32B。
-   srcNdMatrixStride  = 384，表示相邻ND矩阵起始地址的偏移，每行32个元素，共12行，偏移为32 \* 12= 384个元素。
-   srcDValue = 32，表示源矩阵一行的所含元素个数，即32个half数据类型的元素。
-   dstNzC0Stride = 37，NZ矩阵中相邻Z矩阵起始地址的偏移，偏移为37个C0\_SIZE。
-   dstNzNStride = 2，表示NZ矩阵中相邻行起始地址的偏移。
-   dstNzMatrixStride = 320，相邻NZ矩阵起始地址的偏移，每行16个元素，共20行，偏移为16 \* 20 = 320个元素。

**图 3**  ND2NZ转换示意图（非连续搬运）<a name="zh-cn_topic_0000002566538879_fig19978184883316"></a>  
![](figures/ND2NZ转换示意图（非连续搬运）.png "ND2NZ转换示意图（非连续搬运）")

## SmallC0模式<a name="zh-cn_topic_0000002566538879_section543423634518"></a>

当dValue小于等于4的时候，可以使能SmallC0模式，C0\_SIZE会补齐到4 \* sizeof\(T\) Bytes。在该模式下，dstNzNstride参数和dstNzC0Stride参数无效，无需配置。

**图 4**  ND2NZ转换示意图（Small C0模式）<a name="zh-cn_topic_0000002566538879_fig2528756123117"></a>  
![](figures/ND2NZ转换示意图（Small-C0模式）.png "ND2NZ转换示意图（Small-C0模式）")

## 数据类型<a name="zh-cn_topic_0000002566538879_section4219135304818"></a>

源矩阵和目的矩阵支持的数据类型保持一致。

Ascend 950PR/Ascend 950DT，支持数据类型为：b4（fp4x2\_e2m1\_t、fp4x2\_e1m2\_t）、b8、b16、b32

## API映射关系<a name="zh-cn_topic_0000002566538879_section5469132317498"></a>

与built-in接口映射关系：

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品，

接口是在built-in接口（copy\_gm\_to\_cbuf\_multi\_nd2nz\_xx）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
// 相同接口的不同原型区别在于源地址和目的地址的数据类型不同，支持的数据类型为b8、b16、b32
void copy_gm_to_cbuf_multi_nd2nz_b8(__cbuf__ int8_t *dst, __gm__ int8_t *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
void copy_gm_to_cbuf_multi_nd2nz_b8(__cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
void copy_gm_to_cbuf_multi_nd2nz_b16(__cbuf__ half *dst, __gm__ half *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
void copy_gm_to_cbuf_multi_nd2nz_b16(__cbuf__ int16_t *dst, __gm__ int16_t *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
void copy_gm_to_cbuf_multi_nd2nz_b16(__cbuf__ uint16_t *dst, __gm__ uint16_t *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
void copy_gm_to_cbuf_multi_nd2nz_b16(__cbuf__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
void copy_gm_to_cbuf_multi_nd2nz_b32s(__cbuf__ float *dst, __gm__ float *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
void copy_gm_to_cbuf_multi_nd2nz_b32s(__cbuf__ int32_t *dst, __gm__ int32_t *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
void copy_gm_to_cbuf_multi_nd2nz_b32s(__cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint8_t sid, uint16_t ndNum, uint16_t nValue, uint16_t dValue, uint16_t srcNdMatrixStride, uint16_t srcDValue, uint16_t dstNzC0Stride, uint16_t dstNzNStride, uint16_t dstNzMatrixStride);
```

针对Ascend 950PR/Ascend 950DT：

接口是在built-in接口（copy\_gm\_to\_cbuf\_multi\_nd2nz）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
// 相同接口的不同原型区别在于源地址和目的地址的数据类型不同，支持的数据类型为b8、b16、b32，由于搬运指令只和数据类型位宽有关，因此所有b8数据类型统一使用int8_t类型接口，所有b16数据类型统一使用half类型接口，所有b32数据类型统一使用float类型接口
void copy_gm_to_cbuf_multi_nd2nz(__cbuf__ half *dst_addr, __gm__ half *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl_mode, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_nd2nz(__cbuf__ float *dst_addr, __gm__ float *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl_mode, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_nd2nz(__cbuf__ int8_t *dst_addr, __gm__ int8_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl_mode, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
```

## 约束说明<a name="zh-cn_topic_0000002566538879_section2045914466492"></a>

-   当输入数据类型是b4时，ND2NZ过程中数据是基于 b8 类型进行处理，因此参数需要根据 b8 类型进行设置。
-   位于Global Memory的源地址必须1B对齐，位于L1 Buffer的目的地址必须32B对齐。
-   当Nd2NzParams结构体参数ndNum、nValue、dValue任意一个值为0时，该指令不会执行，该接口将被视为 NOP（空操作）。
-   搬运至L1 Buffer的数据不能重叠，如果存在重叠写入，硬件不会产生任何警告或错误，并且不保证重叠数据的写入顺序。
-   Nd2NzParams结构体参数的值需在取值范围内：

    **表 5**  Nd2NzParams结构体参数取值范围

    <a name="zh-cn_topic_0000002566538879_table843765314306"></a>
    <table><thead align="left"><tr id="zh-cn_topic_0000002566538879_row2043745317306"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002566538879_p1743745312309"><a name="zh-cn_topic_0000002566538879_p1743745312309"></a><a name="zh-cn_topic_0000002566538879_p1743745312309"></a>参数名称</p>
    </th>
    <th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002566538879_p124371553173016"><a name="zh-cn_topic_0000002566538879_p124371553173016"></a><a name="zh-cn_topic_0000002566538879_p124371553173016"></a>取值范围</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="zh-cn_topic_0000002566538879_row18437453183011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p17437105318308"><a name="zh-cn_topic_0000002566538879_p17437105318308"></a><a name="zh-cn_topic_0000002566538879_p17437105318308"></a>ndNum</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p1383153213319"><a name="zh-cn_topic_0000002566538879_p1383153213319"></a><a name="zh-cn_topic_0000002566538879_p1383153213319"></a>[0, 4095]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566538879_row743716536309"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p8437753103017"><a name="zh-cn_topic_0000002566538879_p8437753103017"></a><a name="zh-cn_topic_0000002566538879_p8437753103017"></a>nValue</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p543755323018"><a name="zh-cn_topic_0000002566538879_p543755323018"></a><a name="zh-cn_topic_0000002566538879_p543755323018"></a>[0, 16384]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566538879_row19437155343014"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p14437253203018"><a name="zh-cn_topic_0000002566538879_p14437253203018"></a><a name="zh-cn_topic_0000002566538879_p14437253203018"></a>dValue</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p2437153173017"><a name="zh-cn_topic_0000002566538879_p2437153173017"></a><a name="zh-cn_topic_0000002566538879_p2437153173017"></a>[0, 65535]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566538879_row194371253133011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p843715312301"><a name="zh-cn_topic_0000002566538879_p843715312301"></a><a name="zh-cn_topic_0000002566538879_p843715312301"></a>srcNdMatrixStride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p7437453193010"><a name="zh-cn_topic_0000002566538879_p7437453193010"></a><a name="zh-cn_topic_0000002566538879_p7437453193010"></a>[0, 65535]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566538879_row64375539304"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p12437853143013"><a name="zh-cn_topic_0000002566538879_p12437853143013"></a><a name="zh-cn_topic_0000002566538879_p12437853143013"></a>srcDValue</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p15437195311304"><a name="zh-cn_topic_0000002566538879_p15437195311304"></a><a name="zh-cn_topic_0000002566538879_p15437195311304"></a>[1, 65535]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566538879_row9437653103010"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p1643755393013"><a name="zh-cn_topic_0000002566538879_p1643755393013"></a><a name="zh-cn_topic_0000002566538879_p1643755393013"></a>dstNzC0Stride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p1843710531304"><a name="zh-cn_topic_0000002566538879_p1843710531304"></a><a name="zh-cn_topic_0000002566538879_p1843710531304"></a>[1, 16384]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566538879_row11437165393014"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p1143713535302"><a name="zh-cn_topic_0000002566538879_p1143713535302"></a><a name="zh-cn_topic_0000002566538879_p1143713535302"></a>dstNzNStride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p619994719312"><a name="zh-cn_topic_0000002566538879_p619994719312"></a><a name="zh-cn_topic_0000002566538879_p619994719312"></a>[1, 16384]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002566538879_row4976112483114"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002566538879_p69768247312"><a name="zh-cn_topic_0000002566538879_p69768247312"></a><a name="zh-cn_topic_0000002566538879_p69768247312"></a>dstNzMatrixStride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002566538879_p129761124183110"><a name="zh-cn_topic_0000002566538879_p129761124183110"></a><a name="zh-cn_topic_0000002566538879_p129761124183110"></a>[0, 65535]</p>
    </td>
    </tr>
    </tbody>
    </table>

## 理论性能说明<a name="zh-cn_topic_0000002566538879_section11310948115013"></a>

待补充

## 调用示例<a name="zh-cn_topic_0000002566538879_section088124295117"></a>

一、A矩阵不转置，half数据类型下，GM-\>L1 搬运过程中进行随路ND到NZ的数据排布转换示意图

![](figures/exam.png)

**使用示例片段:**

GM--\>L1，搬运A矩阵

```
// m=40,k=56,fractalShape[0] = 16,fractalShape[1] = 16,fractalSize = 16 * fractalShape[1]
AscendC::Nd2NzParams nd2nzA1Params;
// 传输ND矩阵的数目
nd2nzA1Params.ndNum = 1;
// ND矩阵的行数
nd2nzA1Params.nValue = m;
// ND矩阵的列数
nd2nzA1Params.dValue = k;
// 只传输了1个ND矩阵，该参数无效
nd2nzA1Params.srcNdMatrixStride = 0;
// 源操作数同一ND矩阵的相邻行起始地址间的偏移
nd2nzA1Params.srcDValue = k;
// 以下这个参数取A矩阵在L1上，高度方向的对齐后的长度
nd2nzA1Params.dstNzC0Stride = CeilAlign(m, fractalShape[0]);
nd2nzA1Params.dstNzNStride = 1;
nd2nzA1Params.dstNzMatrixStride = 0;
AscendC::DataCopy(a1Local, aGM, nd2nzA1Params);
```

完整示例请参考：[DataCopy\_GM2L1样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0)。

-   **[产品支持情况](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002568950909.md)**  

-   **[功能说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538231134.md)**  

-   **[函数原型](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538071210.md)**  

-   **[参数说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002569070915.md)**  

-   **[数据类型](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002568950911.md)**  

-   **[API映射关系](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538231136.md)**  

-   **[约束说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002538071212.md)**  

-   **[理论性能说明](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002569070917.md)**  

-   **[调用示例](data01-PA_TEMP-8-zh-cn_bookmap_0000002544732345260515130655359-temp-zh-cn_topic_0000002568950913.md)**  

