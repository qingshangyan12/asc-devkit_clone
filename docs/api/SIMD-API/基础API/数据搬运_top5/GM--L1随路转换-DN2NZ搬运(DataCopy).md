# GM-\>L1随路转换-DN2NZ搬运\(DataCopy\)<a name="ZH-CN_TOPIC_0000002563847574"></a>

针对Ascend 950PR/Ascend 950DT：

## 产品支持情况<a name="zh-cn_topic_0000002560454030_section796754519912"></a>

<a name="zh-cn_topic_0000002560454030_table1756814513352"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002560454030_row656911553511"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002560454030_p1883113061818"><a name="zh-cn_topic_0000002560454030_p1883113061818"></a><a name="zh-cn_topic_0000002560454030_p1883113061818"></a><span id="zh-cn_topic_0000002560454030_ph20833205312295"><a name="zh-cn_topic_0000002560454030_ph20833205312295"></a><a name="zh-cn_topic_0000002560454030_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002560454030_p783113012187"><a name="zh-cn_topic_0000002560454030_p783113012187"></a><a name="zh-cn_topic_0000002560454030_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002560454030_row75812513379"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002560454030_p17301775812"><a name="zh-cn_topic_0000002560454030_p17301775812"></a><a name="zh-cn_topic_0000002560454030_p17301775812"></a><span id="zh-cn_topic_0000002560454030_ph2272194216543"><a name="zh-cn_topic_0000002560454030_ph2272194216543"></a><a name="zh-cn_topic_0000002560454030_ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002560454030_p37256491200"><a name="zh-cn_topic_0000002560454030_p37256491200"></a><a name="zh-cn_topic_0000002560454030_p37256491200"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002560454030_section106841136114319"></a>

头文件为: \#include "basic\_api/kernel\_operator\_data\_copy\_intf.h"

该接口主要实现将矩阵从Global Memory搬运至L1 Buffer，并支持在数据搬运时进行DN到NZ格式的转换。

DN到NZ的格式转换等价于ND到ZN的格式转换，如下图所示：

![](figures/nd2nz.png)

## 函数原型<a name="zh-cn_topic_0000002560454030_section82039854412"></a>

```
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const Dn2NzParams& intriParams);
```

## 参数说明<a name="zh-cn_topic_0000002560454030_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002560454030_table49614585413"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002560454030_row996115820412"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002560454030_p10961458104110"><a name="zh-cn_topic_0000002560454030_p10961458104110"></a><a name="zh-cn_topic_0000002560454030_p10961458104110"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002560454030_p6961155817415"><a name="zh-cn_topic_0000002560454030_p6961155817415"></a><a name="zh-cn_topic_0000002560454030_p6961155817415"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002560454030_row4961205811416"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p29619586417"><a name="zh-cn_topic_0000002560454030_p29619586417"></a><a name="zh-cn_topic_0000002560454030_p29619586417"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p996110583417"><a name="zh-cn_topic_0000002560454030_p996110583417"></a><a name="zh-cn_topic_0000002560454030_p996110583417"></a>源操作数或者目的操作数的数据类型。支持的数据类型请参考<a href="GM--L1随路转换-ND2NZ搬运(DataCopy)-57.md#zh-cn_topic_0000002566538879_section4219135304818">数据类型</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row19471252112319"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p425395515263"><a name="zh-cn_topic_0000002560454030_p425395515263"></a><a name="zh-cn_topic_0000002560454030_p425395515263"></a>enableSmallC0</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p17254115514267"><a name="zh-cn_topic_0000002560454030_p17254115514267"></a><a name="zh-cn_topic_0000002560454030_p17254115514267"></a>SmallC0模式开关：当dValue小于等于4的时候，C0_SIZE会补齐到4 * sizeof(T) Bytes，默认不开启。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002560454030_table917mcpsimp"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002560454030_row923mcpsimp"><th class="cellrowborder" valign="top" width="15.02%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002560454030_p925mcpsimp"><a name="zh-cn_topic_0000002560454030_p925mcpsimp"></a><a name="zh-cn_topic_0000002560454030_p925mcpsimp"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="9.86%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002560454030_p927mcpsimp"><a name="zh-cn_topic_0000002560454030_p927mcpsimp"></a><a name="zh-cn_topic_0000002560454030_p927mcpsimp"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="75.12%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002560454030_p929mcpsimp"><a name="zh-cn_topic_0000002560454030_p929mcpsimp"></a><a name="zh-cn_topic_0000002560454030_p929mcpsimp"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002560454030_row930mcpsimp"><td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002560454030_p2925016172518"><a name="zh-cn_topic_0000002560454030_p2925016172518"></a><a name="zh-cn_topic_0000002560454030_p2925016172518"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="9.86%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002560454030_p199251416112517"><a name="zh-cn_topic_0000002560454030_p199251416112517"></a><a name="zh-cn_topic_0000002560454030_p199251416112517"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="75.12%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002560454030_p292591672516"><a name="zh-cn_topic_0000002560454030_p292591672516"></a><a name="zh-cn_topic_0000002560454030_p292591672516"></a>目的操作数，类型为LocalTensor。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row937mcpsimp"><td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002560454030_p3926171610253"><a name="zh-cn_topic_0000002560454030_p3926171610253"></a><a name="zh-cn_topic_0000002560454030_p3926171610253"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="9.86%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002560454030_p4926121682518"><a name="zh-cn_topic_0000002560454030_p4926121682518"></a><a name="zh-cn_topic_0000002560454030_p4926121682518"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="75.12%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002560454030_p195101478511"><a name="zh-cn_topic_0000002560454030_p195101478511"></a><a name="zh-cn_topic_0000002560454030_p195101478511"></a>源操作数，类型为GlobalTensor。</p>
<p id="zh-cn_topic_0000002560454030_p49261616142516"><a name="zh-cn_topic_0000002560454030_p49261616142516"></a><a name="zh-cn_topic_0000002560454030_p49261616142516"></a><a href="SetL2CacheHint.md#ZH-CN_TOPIC_0000002563688488">SetL2CacheHint</a>支持的L2 Cache控制模式如下：CacheMode::CACHE_MODE_DISABLE、CacheMode::CACHE_MODE_NORMAL。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row4726155915388"><td class="cellrowborder" valign="top" width="15.02%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002560454030_p21111014132612"><a name="zh-cn_topic_0000002560454030_p21111014132612"></a><a name="zh-cn_topic_0000002560454030_p21111014132612"></a>intriParams</p>
</td>
<td class="cellrowborder" valign="top" width="9.86%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002560454030_p71115140262"><a name="zh-cn_topic_0000002560454030_p71115140262"></a><a name="zh-cn_topic_0000002560454030_p71115140262"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="75.12%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002560454030_p10415122261"><a name="zh-cn_topic_0000002560454030_p10415122261"></a><a name="zh-cn_topic_0000002560454030_p10415122261"></a>搬运参数，Dn2NzParams类型，具体参数说明请参考表<a href="#zh-cn_topic_0000002560454030_table9182515919">Dn2NzParams结构体参数定义</a>；</p>
</td>
</tr>
</tbody>
</table>

**表 3**  Dn2NzParams结构体参数定义

<a name="zh-cn_topic_0000002560454030_table9182515919"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002560454030_row151816516917"><th class="cellrowborder" valign="top" width="14.719999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002560454030_p18182513916"><a name="zh-cn_topic_0000002560454030_p18182513916"></a><a name="zh-cn_topic_0000002560454030_p18182513916"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="85.28%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002560454030_p11449719134719"><a name="zh-cn_topic_0000002560454030_p11449719134719"></a><a name="zh-cn_topic_0000002560454030_p11449719134719"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002560454030_row1818105113916"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p8928559104716"><a name="zh-cn_topic_0000002560454030_p8928559104716"></a><a name="zh-cn_topic_0000002560454030_p8928559104716"></a>dnNum</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p844917194478"><a name="zh-cn_topic_0000002560454030_p844917194478"></a><a name="zh-cn_topic_0000002560454030_p844917194478"></a>传输DN矩阵的数目，取值范围：ndNum∈[0, 4095]。</p>
<p id="zh-cn_topic_0000002560454030_p240811478199"><a name="zh-cn_topic_0000002560454030_p240811478199"></a><a name="zh-cn_topic_0000002560454030_p240811478199"></a><strong id="zh-cn_topic_0000002560454030_b1899463611015"><a name="zh-cn_topic_0000002560454030_b1899463611015"></a><a name="zh-cn_topic_0000002560454030_b1899463611015"></a>注：dnNum = 0表示不执行搬运，该接口将被视为 NOP（空操作）。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row2968131992515"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p102019313485"><a name="zh-cn_topic_0000002560454030_p102019313485"></a><a name="zh-cn_topic_0000002560454030_p102019313485"></a>nValue</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p11299126114913"><a name="zh-cn_topic_0000002560454030_p11299126114913"></a><a name="zh-cn_topic_0000002560454030_p11299126114913"></a>DN矩阵的行数，取值范围：nValue∈[0, 16384]。</p>
<p id="zh-cn_topic_0000002560454030_p3901221943"><a name="zh-cn_topic_0000002560454030_p3901221943"></a><a name="zh-cn_topic_0000002560454030_p3901221943"></a><strong id="zh-cn_topic_0000002560454030_b990182742"><a name="zh-cn_topic_0000002560454030_b990182742"></a><a name="zh-cn_topic_0000002560454030_b990182742"></a>注：nValue = 0表示不执行搬运，该接口将被视为 NOP（空操作）。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row1589112062510"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p1397111514816"><a name="zh-cn_topic_0000002560454030_p1397111514816"></a><a name="zh-cn_topic_0000002560454030_p1397111514816"></a>dValue</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p144493191478"><a name="zh-cn_topic_0000002560454030_p144493191478"></a><a name="zh-cn_topic_0000002560454030_p144493191478"></a>DN矩阵的列数，取值范围：dValue∈[0,  2^21-1]。</p>
<p id="zh-cn_topic_0000002560454030_p15671106413"><a name="zh-cn_topic_0000002560454030_p15671106413"></a><a name="zh-cn_topic_0000002560454030_p15671106413"></a><strong id="zh-cn_topic_0000002560454030_b86711601249"><a name="zh-cn_topic_0000002560454030_b86711601249"></a><a name="zh-cn_topic_0000002560454030_b86711601249"></a>注：dValue = 0表示不执行搬运，该接口将被视为 NOP（空操作）。</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row3593192082512"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p1217581074813"><a name="zh-cn_topic_0000002560454030_p1217581074813"></a><a name="zh-cn_topic_0000002560454030_p1217581074813"></a>srcDnMatrixStride</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p358913371945"><a name="zh-cn_topic_0000002560454030_p358913371945"></a><a name="zh-cn_topic_0000002560454030_p358913371945"></a>源操作数相邻DN矩阵起始地址间的偏移，取值范围：srcDNMatrixStride∈[0,  2^40-1]，单位为元素。</p>
<a name="zh-cn_topic_0000002560454030_ul141355479108"></a><a name="zh-cn_topic_0000002560454030_ul141355479108"></a><ul id="zh-cn_topic_0000002560454030_ul141355479108"><li>dnNum = 1时，dstNzMatrixStride无意义，设置为0即可。</li></ul>
<a name="zh-cn_topic_0000002560454030_ul158431058161016"></a><a name="zh-cn_topic_0000002560454030_ul158431058161016"></a><ul id="zh-cn_topic_0000002560454030_ul158431058161016"><li>dnNum ≠ 1时，当srcDnMatrixStride = 0时，表示源矩阵中搬入的每个DN矩阵都从第一个DN矩阵读取。</li></ul>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row1185111015588"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p180672644819"><a name="zh-cn_topic_0000002560454030_p180672644819"></a><a name="zh-cn_topic_0000002560454030_p180672644819"></a>srcDValue</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p128531313134818"><a name="zh-cn_topic_0000002560454030_p128531313134818"></a><a name="zh-cn_topic_0000002560454030_p128531313134818"></a>源操作数同一DN矩阵的相邻行起始地址间的偏移，取值范围：srcDValue∈[1,  2^40-1]，单位为元素。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row1585714414415"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p101497513422"><a name="zh-cn_topic_0000002560454030_p101497513422"></a><a name="zh-cn_topic_0000002560454030_p101497513422"></a>dstNzC0Stride</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p11492512428"><a name="zh-cn_topic_0000002560454030_p11492512428"></a><a name="zh-cn_topic_0000002560454030_p11492512428"></a>DN转换到NZ格式后，源操作数中的一列会转换为目的操作数的多行。dstNzC0Stride表示，目的NZ矩阵中，来自源操作数同一列的多行数据相邻行起始地址间的偏移，取值范围：dstNzC0Stride∈[1,  65535]，单位：C0_SIZE（32B）。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row1881103664120"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p014955124220"><a name="zh-cn_topic_0000002560454030_p014955124220"></a><a name="zh-cn_topic_0000002560454030_p014955124220"></a>dstNzNStride</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p4149145174213"><a name="zh-cn_topic_0000002560454030_p4149145174213"></a><a name="zh-cn_topic_0000002560454030_p4149145174213"></a>目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dstNzNStride∈[1, 65535]，单位：C0_SIZE（32B）。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002560454030_row57391832114112"><td class="cellrowborder" valign="top" width="14.719999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p3149175204211"><a name="zh-cn_topic_0000002560454030_p3149175204211"></a><a name="zh-cn_topic_0000002560454030_p3149175204211"></a>dstNzMatrixStride</p>
</td>
<td class="cellrowborder" valign="top" width="85.28%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p01491753428"><a name="zh-cn_topic_0000002560454030_p01491753428"></a><a name="zh-cn_topic_0000002560454030_p01491753428"></a>目的NZ矩阵中，相邻NZ矩阵起始地址的偏移，取值范围：dstNzMatrixStride∈[0,  65535]，单位为元素个数。</p>
<a name="zh-cn_topic_0000002560454030_ul25267272811"></a><a name="zh-cn_topic_0000002560454030_ul25267272811"></a><ul id="zh-cn_topic_0000002560454030_ul25267272811"><li>dnNum = 1时，dstNzMatrixStride无意义，设置为0即可。</li><li>dnNum ≠ 1时，当dstNzMatrixStride = 0时，表示目的矩阵中搬入的每个NZ矩阵都会覆盖第一个NZ矩阵。</li></ul>
</td>
</tr>
</tbody>
</table>

## 连续搬运<a name="zh-cn_topic_0000002560454030_section7495192151713"></a>

以half数据类型为例，[图1](#zh-cn_topic_0000002560454030_fig7420114233419)为DN2NZ连续搬运示意图。Dn2NzParams结构体参数配置说明如下：

-   dnNum = 1，表示搬运的ND矩阵的数量为1。
-   nValue = 16，一个DN矩阵的行数。
-   dValue = 23，一个ND矩阵的列数。当dValue \* sizeof\(T\)不满足32B对齐时，在目的矩阵中会补0对齐到32B。
-   srcDnMatrixStride  = 0，只有一个DN矩阵，该值为0。
-   srcDValue = 16，表示源矩阵一行的所含元素个数，即32个half数据类型的元素。
-   dstNzC0Stride = 16，NZ矩阵中相邻Z矩阵起始地址的偏移，偏移为16个C0\_SIZE，也就是16\*32B。
-   dstNzNStride = 1，表示NZ矩阵中相邻行起始地址的偏移。
-   dstNzMatrixStride = 0，输入只有一个DN矩阵，该值为0。

**图 1**  DN2NZ转换示意图（连续搬运）<a name="zh-cn_topic_0000002560454030_fig7420114233419"></a>  
![](figures/DN2NZ转换示意图（连续搬运）.png "DN2NZ转换示意图（连续搬运）")

## 非连续搬运<a name="zh-cn_topic_0000002560454030_section1721231581717"></a>

以half数据类型为例，[图2](#zh-cn_topic_0000002560454030_fig19978184883316)为DN2NZ非连续搬运示意图。Dn2NzParams结构体参数配置说明如下：

-   dnNum = 2，表示搬运的ND矩阵的数量为2。
-   nValue = 8，一个DN矩阵的行数。
-   dValue = 23，一个DN矩阵的列数。当dValue \* sizeof\(T\)不满足32B对齐时，在目的矩阵中会补0对齐到32B。
-   srcDnMatrixStride  = 500，表示相邻ND矩阵起始地址的偏移，每行16个元素，共25行，偏移为16 \* 25= 400个元素。
-   srcDValue = 16，表示源矩阵一行的所含元素个数，即16个half数据类型的元素。
-   dstNzC0Stride = 37，NZ矩阵中相邻Z矩阵起始地址的偏移，偏移为37个C0\_SIZE。
-   dstNzNStride = 2，表示NZ矩阵中相邻行起始地址的偏移。
-   dstNzMatrixStride = 320，相邻NZ矩阵起始地址的偏移，每行16个元素，共20行，偏移为16 \* 20 = 320个元素。

**图 2**  DN2NZ转换示意图（非连续搬运）<a name="zh-cn_topic_0000002560454030_fig19978184883316"></a>  

![](figures/dn2nz.png)

## SmallC0模式<a name="zh-cn_topic_0000002560454030_section543423634518"></a>

当dValue小于等于4的时候，可以使能SmallC0模式，C0\_SIZE会对齐到4 \* sizeof\(T\) Bytes，当dValue \* sizeof\(T\)不满足4 \* sizeof\(T\) 对齐时，在目的矩阵中会补0对齐到4 \* sizeof\(T\) 。

若整个目的矩阵没有C0\_SIZE（32B）对齐，则目的矩阵中尾部补0直到32B对齐。

在该模式下，dstNzNstride参数和dstNzC0Stride参数，无需配置。

**图 3**  DN2NZ转换示意图（Small C0模式）<a name="zh-cn_topic_0000002560454030_fig2528756123117"></a>  

![](figures/dn2nz-smallc0---副本.png)

## 数据类型<a name="zh-cn_topic_0000002560454030_section4219135304818"></a>

源矩阵和目的矩阵支持的数据类型保持一致。

Ascend 950PR/Ascend 950DT，支持数据类型为：b4（fp4x2\_e2m1\_t、fp4x2\_e1m2\_t）、b8、b16、b32

## API映射关系<a name="zh-cn_topic_0000002560454030_section5469132317498"></a>

与built-in接口映射关系：

接口是在built-in接口（copy\_gm\_to\_cbuf\_multi\_dn2nz）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
// 相同接口的不同原型区别在于源地址和目的地址的数据类型不同，支持的数据类型为b8、b16、b32，由于没有对应b4类型的指令，因此在搬运b4数据类型的时候是借助uint8_t类型的built-in接口实现。
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ bfloat16_t *dst_addr, __gm__ bfloat16_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ float8_e4m3_t *dst_addr, __gm__ float8_e4m3_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ float8_e5m2_t *dst_addr, __gm__ float8_e5m2_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ half *dst_addr, __gm__ half *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ float *dst_addr, __gm__ float *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ hifloat8_t *dst_addr, __gm__ hifloat8_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ int16_t *dst_addr, __gm__ int16_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ int32_t *dst_addr, __gm__ int32_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ int8_t *dst_addr, __gm__ int8_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ uint16_t *dst_addr, __gm__ uint16_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ uint32_t *dst_addr, __gm__ uint32_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
void copy_gm_to_cbuf_multi_dn2nz(__cbuf__ uint8_t *dst_addr, __gm__ uint8_t *src_addr, uint8_t sid, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);
```

## 约束说明<a name="zh-cn_topic_0000002560454030_section2045914466492"></a>

-   当输入数据类型是b4时，DN2NZ过程中数据是基于 b8 类型进行处理，因此参数需要根据 b8 类型进行设置，nValue必须是2的倍数。
-   位于Global Memory的源地址必须1B对齐，位于L1 Buffer的目的地址必须32B对齐。
-   当Dn2NzParams结构体参数dnNum、nValue、dValue任意一个值为0时，该指令不会执行，该接口将被视为 NOP（空操作）。
-   搬运至L1 Buffer的数据不能重叠，如果存在重叠写入，硬件不会产生任何警告或错误，并且不保证重叠数据的写入顺序。
-   Dn2NzParams结构体参数的值需在取值范围内：

    **表 4**  Nd2NzParams结构体参数取值范围

    <a name="zh-cn_topic_0000002560454030_table843765314306"></a>
    <table><thead align="left"><tr id="zh-cn_topic_0000002560454030_row2043745317306"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002560454030_p1743745312309"><a name="zh-cn_topic_0000002560454030_p1743745312309"></a><a name="zh-cn_topic_0000002560454030_p1743745312309"></a>参数名称</p>
    </th>
    <th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002560454030_p124371553173016"><a name="zh-cn_topic_0000002560454030_p124371553173016"></a><a name="zh-cn_topic_0000002560454030_p124371553173016"></a>取值范围</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="zh-cn_topic_0000002560454030_row18437453183011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p17437105318308"><a name="zh-cn_topic_0000002560454030_p17437105318308"></a><a name="zh-cn_topic_0000002560454030_p17437105318308"></a>dnNum</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p1383153213319"><a name="zh-cn_topic_0000002560454030_p1383153213319"></a><a name="zh-cn_topic_0000002560454030_p1383153213319"></a>[0, 4095]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002560454030_row743716536309"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p8437753103017"><a name="zh-cn_topic_0000002560454030_p8437753103017"></a><a name="zh-cn_topic_0000002560454030_p8437753103017"></a>nValue</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p543755323018"><a name="zh-cn_topic_0000002560454030_p543755323018"></a><a name="zh-cn_topic_0000002560454030_p543755323018"></a>[0, 16384]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002560454030_row19437155343014"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p14437253203018"><a name="zh-cn_topic_0000002560454030_p14437253203018"></a><a name="zh-cn_topic_0000002560454030_p14437253203018"></a>dValue</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p2437153173017"><a name="zh-cn_topic_0000002560454030_p2437153173017"></a><a name="zh-cn_topic_0000002560454030_p2437153173017"></a>[0, 2^21-1]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002560454030_row194371253133011"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p843715312301"><a name="zh-cn_topic_0000002560454030_p843715312301"></a><a name="zh-cn_topic_0000002560454030_p843715312301"></a>srcDnMatrixStride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p7437453193010"><a name="zh-cn_topic_0000002560454030_p7437453193010"></a><a name="zh-cn_topic_0000002560454030_p7437453193010"></a>[0, 2^40-1]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002560454030_row64375539304"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p12437853143013"><a name="zh-cn_topic_0000002560454030_p12437853143013"></a><a name="zh-cn_topic_0000002560454030_p12437853143013"></a>srcDValue</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p15437195311304"><a name="zh-cn_topic_0000002560454030_p15437195311304"></a><a name="zh-cn_topic_0000002560454030_p15437195311304"></a>[1, 2^40-1]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002560454030_row9437653103010"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p1643755393013"><a name="zh-cn_topic_0000002560454030_p1643755393013"></a><a name="zh-cn_topic_0000002560454030_p1643755393013"></a>dstNzC0Stride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p1843710531304"><a name="zh-cn_topic_0000002560454030_p1843710531304"></a><a name="zh-cn_topic_0000002560454030_p1843710531304"></a>[1, 16384]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002560454030_row11437165393014"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p1143713535302"><a name="zh-cn_topic_0000002560454030_p1143713535302"></a><a name="zh-cn_topic_0000002560454030_p1143713535302"></a>dstNzNStride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p619994719312"><a name="zh-cn_topic_0000002560454030_p619994719312"></a><a name="zh-cn_topic_0000002560454030_p619994719312"></a>[1, 16384]</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002560454030_row4976112483114"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002560454030_p69768247312"><a name="zh-cn_topic_0000002560454030_p69768247312"></a><a name="zh-cn_topic_0000002560454030_p69768247312"></a>dstNzMatrixStride</p>
    </td>
    <td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002560454030_p129761124183110"><a name="zh-cn_topic_0000002560454030_p129761124183110"></a><a name="zh-cn_topic_0000002560454030_p129761124183110"></a>[0, 65535]</p>
    </td>
    </tr>
    </tbody>
    </table>

## 调用示例<a name="zh-cn_topic_0000002560454030_section088124295117"></a>

