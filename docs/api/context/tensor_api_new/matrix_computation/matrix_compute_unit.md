# 矩阵计算单元

针对Ascend 950PR/Ascend 950DT产品，矩阵计算单元架构图如下：

**图 1**  矩阵计算单元架构图<a name="zh-cn_topic_0000002513373312_fig1082103513519"></a>  
![](figures/矩阵计算单元架构图-26.png "矩阵计算单元架构图-26")

矩阵计算关联的存储单元为：

<a name="zh-cn_topic_0000002513373312_table187608336169"></a>
<table><tbody><tr id="zh-cn_topic_0000002513373312_row1677933317161"><td class="cellrowborder" valign="top" width="16.09%"><p id="zh-cn_topic_0000002513373312_p577919334164"><a name="zh-cn_topic_0000002513373312_p577919334164"></a><a name="zh-cn_topic_0000002513373312_p577919334164"></a>L0A Buffer</p>
</td>
<td class="cellrowborder" valign="top" width="83.91%"><p id="zh-cn_topic_0000002513373312_p16779123391617"><a name="zh-cn_topic_0000002513373312_p16779123391617"></a><a name="zh-cn_topic_0000002513373312_p16779123391617"></a>AI Core内部物理存储单元，通常用于存储矩阵计算的左矩阵。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002513373312_row15779113301616"><td class="cellrowborder" valign="top" width="16.09%"><p id="zh-cn_topic_0000002513373312_p37791633191610"><a name="zh-cn_topic_0000002513373312_p37791633191610"></a><a name="zh-cn_topic_0000002513373312_p37791633191610"></a>L0B Buffer</p>
</td>
<td class="cellrowborder" valign="top" width="83.91%"><p id="zh-cn_topic_0000002513373312_p4779133321614"><a name="zh-cn_topic_0000002513373312_p4779133321614"></a><a name="zh-cn_topic_0000002513373312_p4779133321614"></a>AI Core内部物理存储单元，通常用于存储矩阵计算的右矩阵。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002513373312_row5779133141613"><td class="cellrowborder" valign="top" width="16.09%"><p id="zh-cn_topic_0000002513373312_p277973361611"><a name="zh-cn_topic_0000002513373312_p277973361611"></a><a name="zh-cn_topic_0000002513373312_p277973361611"></a>L0C Buffer</p>
</td>
<td class="cellrowborder" valign="top" width="83.91%"><p id="zh-cn_topic_0000002513373312_p1477993311165"><a name="zh-cn_topic_0000002513373312_p1477993311165"></a><a name="zh-cn_topic_0000002513373312_p1477993311165"></a>AI Core内部物理存储单元，通常用于存储矩阵计算的结果，以及矩阵累加初始化值。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002513373312_row67791332165"><td class="cellrowborder" valign="top" width="16.09%"><p id="zh-cn_topic_0000002513373312_p6779203310169"><a name="zh-cn_topic_0000002513373312_p6779203310169"></a><a name="zh-cn_topic_0000002513373312_p6779203310169"></a>L1 Buffer</p>
</td>
<td class="cellrowborder" valign="top" width="83.91%"><p id="zh-cn_topic_0000002513373312_p677913381619"><a name="zh-cn_topic_0000002513373312_p677913381619"></a><a name="zh-cn_topic_0000002513373312_p677913381619"></a>AI Core内部物理存储单元，空间相对较大，通常用于缓存矩阵计算的输入数据。 矩阵计算的输入一般需要从GM搬运到L1 Buffer，然后分别搬运到L0A Buffer和L0B Buffer。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002513373312_row49201557161613"><td class="cellrowborder" valign="top" width="16.09%"><p id="zh-cn_topic_0000002513373312_p1645352113174"><a name="zh-cn_topic_0000002513373312_p1645352113174"></a><a name="zh-cn_topic_0000002513373312_p1645352113174"></a>Fixpipe Buffer</p>
</td>
<td class="cellrowborder" valign="top" width="83.91%"><p id="zh-cn_topic_0000002513373312_p2045372191713"><a name="zh-cn_topic_0000002513373312_p2045372191713"></a><a name="zh-cn_topic_0000002513373312_p2045372191713"></a>AI Core内部物理存储单元，通常用于存储Fixpipe搬运过程中所需的量化参数等数据。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002513373312_row14841436171717"><td class="cellrowborder" valign="top" width="16.09%"><p id="zh-cn_topic_0000002513373312_p89761336101716"><a name="zh-cn_topic_0000002513373312_p89761336101716"></a><a name="zh-cn_topic_0000002513373312_p89761336101716"></a>BiasTable Buffer</p>
</td>
<td class="cellrowborder" valign="top" width="83.91%"><p id="zh-cn_topic_0000002513373312_p897613362174"><a name="zh-cn_topic_0000002513373312_p897613362174"></a><a name="zh-cn_topic_0000002513373312_p897613362174"></a>偏置存储，AI Core内部物理存储单元，通常用于存储矩阵计算所需的Bias（偏置）数据。</p>
</td>
</tr>
</tbody>
</table>
