# Matmul特性介绍<a name="ZH-CN_TOPIC_0000002500468238"></a>

除了前述[基础知识](../基础知识.md)和[算子实现](../算子实现.md)中介绍的基本计算能力外，Matmul矩阵编程还提供了适用于不同场景的处理能力及多种功能，具体场景和功能列于下表中，详细内容请见后续章节。

**表 1**  Matmul功能特性表

<a name="zh-cn_topic_0000002298078557_table1220612410208"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002298078557_row19206141132015"><th class="cellrowborder" valign="top" width="23.64%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002298078557_p52061441122017"><a name="zh-cn_topic_0000002298078557_p52061441122017"></a><a name="zh-cn_topic_0000002298078557_p52061441122017"></a>特性描述</p>
</th>
<th class="cellrowborder" valign="top" width="76.36%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002298078557_p1206124122015"><a name="zh-cn_topic_0000002298078557_p1206124122015"></a><a name="zh-cn_topic_0000002298078557_p1206124122015"></a>功能简介</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002298078557_row8206941192015"><td class="cellrowborder" valign="top" width="23.64%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002298078557_p108541234142315"><a name="zh-cn_topic_0000002298078557_p108541234142315"></a><a name="zh-cn_topic_0000002298078557_p108541234142315"></a><a href="多核对齐切分.md">多核对齐切分</a></p>
</td>
<td class="cellrowborder" valign="top" width="76.36%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002298078557_p8702111310525"><a name="zh-cn_topic_0000002298078557_p8702111310525"></a><a name="zh-cn_topic_0000002298078557_p8702111310525"></a>在多核场景中，支持将矩阵数据沿M、N、K轴切分，满足M能被singleCoreM整除、N能被singleCoreN整除、K能被singleCoreK整除的对齐场景时的处理方式，从而实现多核并行计算矩阵乘。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002298078557_row12206184142016"><td class="cellrowborder" valign="top" width="23.64%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002298078557_p954132121113"><a name="zh-cn_topic_0000002298078557_p954132121113"></a><a name="zh-cn_topic_0000002298078557_p954132121113"></a><a href="多核非对齐切分.md">多核非对齐切分</a></p>
</td>
<td class="cellrowborder" valign="top" width="76.36%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002298078557_p17648104611560"><a name="zh-cn_topic_0000002298078557_p17648104611560"></a><a name="zh-cn_topic_0000002298078557_p17648104611560"></a>在多核场景中，支持将矩阵数据沿M、N、K轴切分。当出现M不能被singleCoreM整除、或N不能被singleCoreN整除、或K不能被singleCoreK整除的非对齐场景（即尾块场景）时的处理方式。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002298078557_row12207144114201"><td class="cellrowborder" valign="top" width="23.64%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002298078557_p5853634122310"><a name="zh-cn_topic_0000002298078557_p5853634122310"></a><a name="zh-cn_topic_0000002298078557_p5853634122310"></a><a href="异步场景处理.md">异步场景处理</a></p>
</td>
<td class="cellrowborder" valign="top" width="76.36%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002298078557_p182077411209"><a name="zh-cn_topic_0000002298078557_p182077411209"></a><a name="zh-cn_topic_0000002298078557_p182077411209"></a>MIX场景（包含矩阵计算和矢量计算）下不需要等待矩阵乘计算完成，先执行其它计算。</p>
</td>
</tr>
<tr id="row5682043577"><td class="cellrowborder" valign="top" width="23.64%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002298078557_p2779724105810"><a name="zh-cn_topic_0000002298078557_p2779724105810"></a><a name="zh-cn_topic_0000002298078557_p2779724105810"></a><a href="矩阵乘输出的量化-反量化.md">矩阵乘输出的量化/反量化</a></p>
</td>
<td class="cellrowborder" valign="top" width="76.36%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002298078557_p8779024175813"><a name="zh-cn_topic_0000002298078557_p8779024175813"></a><a name="zh-cn_topic_0000002298078557_p8779024175813"></a>将矩阵乘的计算结果从CO1搬出到Global Memory时，对矩阵元素执行数据量化或反量化操作。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002298078557_row1320716413201"><td class="cellrowborder" valign="top" width="23.64%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002298078557_p1854719415227"><a name="zh-cn_topic_0000002298078557_p1854719415227"></a><a name="zh-cn_topic_0000002298078557_p1854719415227"></a><a href=https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta3/API/ascendcopapi/atlasascendc_api_07_0618.html">矩阵向量乘</a></p>
</td>
<td class="cellrowborder" valign="top" width="76.36%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002298078557_p120764172019"><a name="zh-cn_topic_0000002298078557_p120764172019"></a><a name="zh-cn_topic_0000002298078557_p120764172019"></a>矩阵向量乘即GEMV，指矩阵乘计算中M=1，K&gt;1的场景，即对形状为(1, K)的左矩阵A实现矩阵乘计算。</p>
</td>
</tr>
<tr id="row1892083116812"><td class="cellrowborder" valign="top" width="23.64%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002298078557_p87791524145813"><a name="zh-cn_topic_0000002298078557_p87791524145813"></a><a name="zh-cn_topic_0000002298078557_p87791524145813"></a><a href="4-2稀疏矩阵乘.md">4:2稀疏矩阵乘</a></p>
</td>
<td class="cellrowborder" valign="top" width="76.36%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002298078557_p97799246588"><a name="zh-cn_topic_0000002298078557_p97799246588"></a><a name="zh-cn_topic_0000002298078557_p97799246588"></a>4:2稀疏矩阵乘，又称Sparse Matmul。指对稀疏左矩阵A和4:2稠密化的右矩阵B实现矩阵乘计算。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002298078557_row16577182820235"><td class="cellrowborder" valign="top" width="23.64%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002298078557_p13858194593417"><a name="zh-cn_topic_0000002298078557_p13858194593417"></a><a name="zh-cn_topic_0000002298078557_p13858194593417"></a><a href=https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta3/API/ascendcopapi/atlasascendc_api_07_0619.html">Batch Matmul复用Bias矩阵</a></p>
</td>
<td class="cellrowborder" valign="top" width="75.96000000000001%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002298078557_p16559162418579"><a name="zh-cn_topic_0000002298078557_p16559162418579"></a><a name="zh-cn_topic_0000002298078557_p16559162418579"></a>每个Batch的Matmul计算复用同一个不带Batch轴的Bias矩阵。</p>
</td>
</tr>
</tbody>
</table>
