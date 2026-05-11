# Add自定义算子开发<a name="ZH-CN_TOPIC_0000002500781060"></a>

本入门教程，将会引导你完成以下任务，体验Ascend C  SIMD算子开发基本流程。

1.  算子分析，明确数学表达式和计算逻辑等内容；
2.  Add算子核函数开发；
3.  算子核函数运行验证。

在正式的开发之前，还需要先完成环境准备工作，开发Ascend C算子的基本流程如下图所示：

**图 1**  开发Ascend C算子的基本流程<a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_fig72899531739"></a>  
![](../../../figures/开发Ascend-C算子的基本流程.png "开发Ascend-C算子的基本流程")

>[!NOTE]说明
>-  请点击[LINK](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/00_introduction/01_vector/basic_api_memory_allocator_add)获取样例代码。
>-  使用本教程只需要您具有一定的C/C++基础，在此基础上，如果您已经对Ascend C编程模型有一定的了解，您可以在实际操作的过程中加深对理论的理解；如果您还没有开始了解Ascend C编程模型，也无需担心，您可以先尝试跑通教程中的样例，参考教程最后的[指引](#zh-cn_topic_0000001565030288_section128349412384)进行进一步的学习。

## 环境准备<a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_section412999115218"></a>

-   CANN软件安装

    开发算子前，需要先准备好开发环境和运行环境，开发环境和运行环境的介绍和具体的安装步骤可参见《CANN 软件安装指南》。

-   环境变量配置

    安装CANN软件后，使用CANN运行用户进行编译、运行时，需要以CANN运行用户登录环境，执行source $_\{INSTALL\_DIR\}_/set\_env.sh命令设置环境变量。$\{INSTALL\_DIR\}请替换为CANN软件安装后文件存储路径。以root用户安装为例，安装后文件默认存储路径为：/usr/local/Ascend/cann。

## 算子分析<a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_section1242991675619"></a>

主要分析算子的数学表达式、输入输出的数量、Shape范围以及计算逻辑的实现，明确需要调用的Ascend C接口。下文以Add算子为例，介绍具体的分析过程。

1.  明确算子的数学表达式及计算逻辑。

    Add算子的数学表达式为：

    ![](../../../figures/zh-cn_formulaimage_0000002501241896.png)

    计算逻辑是：从外部存储Global Memory搬运数据至内部存储Local Memory，然后使用Ascend C计算接口完成两个输入参数相加，得到最终结果，再搬运到Global Memory上。

2.  明确输入和输出。
    -   Add算子有两个输入：x与y，输出为z。
    -   本样例中算子输入支持的数据类型为float，算子输出的数据类型与输入数据类型相同。
    -   算子输入支持的shape为（1，4096），输出shape与输入shape相同。
    -   算子输入支持的[format](../../../编程指南/概念原理和术语/神经网络和算子/数据排布格式.md)为：ND。

3.  确定核函数名称和参数。
    -   本样例中核函数命名为add\_custom。
    -   根据对算子输入输出的分析，确定核函数有3个参数x，y，z；x，y为输入参数，z为输出参数。

4.  确定算子实现所需接口。
    -   实现涉及外部存储和内部存储间的数据搬运，查看Ascend C  API参考中的数据搬运接口，需要使用[DataCopy](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/DataCopy.md)来实现数据搬移。
    -   本样例只涉及矢量计算的加法操作，查看Ascend C  API参考中的矢量计算接口[Memory矢量计算](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/Memory矢量计算.md)，初步分析可使用Add接口[Add](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/Add.md)实现x+y。
    -   计算中使用到的Tensor数据结构，通过静态Tensor编程方式创建LocalTensor。本样例使用[LocalMemAllocator](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/LocalMemAllocator.md)的[Alloc](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/Alloc.md)接口为LocalTensor分配Unified Buffer内存。
    -   静态Tensor编程方式需要开发者手动管理流水间同步，会使用到[SetFlag/WaitFlag](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/SetFlag-WaitFlag(ISASI).md)等同步接口；Kernel入口处还需要调用[InitSocState](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/InitSocState.md)初始化全局状态寄存器。

通过以上分析，得到Ascend C  Add算子的设计规格如下：

**表 1** Ascend C  Add算子设计规格

<a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_table164881913121819"></a>
<table><tbody><tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_row1848911314188"><th class="firstcol" valign="top" id="mcps1.2.6.1.1"><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p54891613141818"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p54891613141818"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p54891613141818"></a>算子类型（OpType）</p>
</th>
<td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p1748971311817"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p1748971311817"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p1748971311817"></a>AddCustom</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_row108131259145713"><th class="firstcol" rowspan="3" valign="top" width="19.71%" id="mcps1.2.6.2.1"><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p84891313191819"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p84891313191819"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p84891313191819"></a>算子输入</p>
</th>
<td class="cellrowborder" valign="top" width="19.250000000000004%" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1081305915577"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1081305915577"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1081305915577"></a><strong id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b1720613329588"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b1720613329588"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b1720613329588"></a>name</strong></p>
</td>
<td class="cellrowborder" valign="top" width="21.970000000000002%" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p481313597575"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p481313597575"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p481313597575"></a><strong id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b621853235814"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b621853235814"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b621853235814"></a>shape</strong></p>
</td>
<td class="cellrowborder" valign="top" width="21.610000000000003%" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1781315915718"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1781315915718"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1781315915718"></a><strong id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b4228123295815"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b4228123295815"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b4228123295815"></a>data type</strong></p>
</td>
<td class="cellrowborder" valign="top" width="17.460000000000004%" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p12813559125720"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p12813559125720"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p12813559125720"></a><strong id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b622933295812"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b622933295812"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_b622933295812"></a>format</strong></p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_row19489161331815"><td class="cellrowborder" valign="top" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p134891113131814"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p134891113131814"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p134891113131814"></a>x</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p1283525118245"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p1283525118245"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p1283525118245"></a>(1, 4096)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p12624946102416"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p12624946102416"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p12624946102416"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p173195184235"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p173195184235"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p173195184235"></a>ND</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_row1348951391811"><td class="cellrowborder" valign="top" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p13489191311184"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p13489191311184"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p13489191311184"></a>y</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p18710686591"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p18710686591"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p18710686591"></a>(1, 4096)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p76241246152416"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p76241246152416"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p76241246152416"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.2.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p0319161811238"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p0319161811238"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p0319161811238"></a>ND</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_row2489171310187"><th class="firstcol" valign="top" width="19.71%" id="mcps1.2.6.5.1"><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p4489913171810"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p4489913171810"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p4489913171810"></a>算子输出</p>
</th>
<td class="cellrowborder" valign="top" width="19.250000000000004%" headers="mcps1.2.6.5.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p34899136188"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p34899136188"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p34899136188"></a>z</p>
</td>
<td class="cellrowborder" valign="top" width="21.970000000000002%" headers="mcps1.2.6.5.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p13645129155915"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p13645129155915"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p13645129155915"></a>(1, 4096)</p>
</td>
<td class="cellrowborder" valign="top" width="21.610000000000003%" headers="mcps1.2.6.5.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p4624546192418"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p4624546192418"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p4624546192418"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="17.460000000000004%" headers="mcps1.2.6.5.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1396317231699"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1396317231699"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1396317231699"></a>ND</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_row628260192410"><th class="firstcol" valign="top" id="mcps1.2.6.6.1"><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p22821601246"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p22821601246"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p22821601246"></a>核函数名称</p>
</th>
<td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.6.6.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1359311404260"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1359311404260"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1359311404260"></a>add_custom</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_row24890138181"><th class="firstcol" rowspan="4" valign="top" id="mcps1.2.6.7.1"><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p64894131189"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p64894131189"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p64894131189"></a>使用的主要接口</p>
</th>
<td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.6.7.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p193715912115"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p193715912115"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p193715912115"></a>DataCopy：数据搬运接口</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_row9293141202412"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.6.7.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p2293241142412"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p2293241142412"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p2293241142412"></a>Add：矢量基础算术接口</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_row2463132420116"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.6.7.1 "><p id="zh-cn_topic_0000001565030288_p10878133151120"><a name="zh-cn_topic_0000001565030288_p10878133151120"></a><a name="zh-cn_topic_0000001565030288_p10878133151120"></a>LocalMemAllocator、Alloc：静态Tensor内存管理接口</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_row1036344318916"><td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.6.7.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p4174848399"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p4174848399"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p4174848399"></a>SetFlag、WaitFlag：同步控制接口</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_row54819813236"><th class="firstcol" valign="top" id="mcps1.2.6.11.1"><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p74958142316"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p74958142316"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_zh-cn_topic_0229825559_zh-cn_topic_0229823837_zh-cn_topic_0211294710_p74958142316"></a>算子实现文件名称</p>
</th>
<td class="cellrowborder" colspan="4" valign="top" headers="mcps1.2.6.11.1 "><p id="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1180818388211"><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1180818388211"></a><a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_p1180818388211"></a>add.asc</p>
</td>
</tr>
</tbody>
</table>

## 核函数开发<a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_section822273613219"></a>

完成环境准备和初步的算子分析后，即可开始Ascend C核函数的开发。开发之前请先从[LINK](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/00_introduction/01_vector/basic_api_memory_allocator_add)获取样例代码，以下样例代码在add.asc中实现。

本样例使用1个Vector核处理4096个float元素，数据规模可以一次放入Unified Buffer，因此不做核间切分，也不需要额外的tiling参数。

1.  根据样例规格定义常量。

    ```
    #include <cstdint>
    #include <cstddef>
    #include "kernel_operator.h"

    namespace {
    constexpr uint32_t BLOCK_NUM = 1;
    constexpr uint32_t DATA_LENGTH = 4096;
    constexpr size_t DATA_BYTE_SIZE = DATA_LENGTH * sizeof(float);
    constexpr int32_t DEVICE_ID = 0;
    }
    ```

2.  根据[核函数定义和调用](../../../编程指南/编程模型/AI-Core-SIMD编程/核函数.md#zh-cn_topic_0000001447989210_section1915102519220)中介绍的规则进行**核函数的定义**，并在核函数中调用算子类的Init和Process函数，算子类实现在后续步骤中介绍。

    ```
    __global__ __vector__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z)
    {
        AscendC::InitSocState();
        KernelAdd op;
        op.Init(x, y, z);
        op.Process();
    }
    ```

    -   使用\_\_global\_\_函数类型限定符来标识它是一个核函数，可以被<<<\>\>\>调用；使用\_\_vector\_\_函数类型限定符来标识该核函数在Vector核上执行。指针入参变量需要增加变量类型限定符\_\_gm\_\_，表明该指针变量指向Global Memory上某处内存地址。为了统一表达，使用GM\_ADDR宏来修饰入参，GM\_ADDR宏定义如下：

        ```
        #define GM_ADDR __gm__ uint8_t*
        ```

    -   静态Tensor编程方式需要在Kernel入口处调用InitSocState接口初始化全局状态寄存器；算子类的Init函数完成GlobalTensor初始化，Process函数完成算子实现的核心逻辑。

3.  **根据静态Tensor编程方式实现算子类**，本样例中定义KernelAdd算子类，其具体实现如下：

    ```
    class KernelAdd {
    public:
        __aicore__ inline KernelAdd() = default;

        __aicore__ inline void Init(GM_ADDR x, GM_ADDR y, GM_ADDR z)
        {
            xGm.SetGlobalBuffer((__gm__ float *)x, DATA_LENGTH);
            yGm.SetGlobalBuffer((__gm__ float *)y, DATA_LENGTH);
            zGm.SetGlobalBuffer((__gm__ float *)z, DATA_LENGTH);
        }

        __aicore__ inline void Process()
        {
            AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
            AscendC::LocalTensor<float> xLocal = ubAllocator.Alloc<float, DATA_LENGTH>();
            AscendC::LocalTensor<float> yLocal = ubAllocator.Alloc<float, DATA_LENGTH>();
            AscendC::LocalTensor<float> zLocal = ubAllocator.Alloc<float, DATA_LENGTH>();

            AscendC::DataCopy(xLocal, xGm, DATA_LENGTH);
            AscendC::DataCopy(yLocal, yGm, DATA_LENGTH);
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

            AscendC::Add(zLocal, xLocal, yLocal, DATA_LENGTH);
            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);

            AscendC::DataCopy(zGm, zLocal, DATA_LENGTH);
            AscendC::SetFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE3_MTE2>(EVENT_ID0);
        }

    private:
        AscendC::GlobalTensor<float> xGm;
        AscendC::GlobalTensor<float> yGm;
        AscendC::GlobalTensor<float> zGm;
    };
    ```

    Init函数通过GlobalTensor类的[SetGlobalBuffer](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/GlobalTensor.md)接口设置x、y、z在Global Memory上的起始地址以及长度。Process函数先通过LocalMemAllocator为x、y、z分别分配LocalTensor，再完成“搬入、计算、搬出”三个阶段。

    -   搬入阶段：使用[DataCopy](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/DataCopy.md)将x、y从Global Memory搬运到Local Memory，然后通过MTE2\_V事件保证搬入完成后再进入Vector计算。
    -   计算阶段：使用[Add](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/Add.md)完成矢量加法，然后通过V\_MTE3事件保证计算完成后再搬出。
    -   搬出阶段：使用DataCopy将z从Local Memory搬运回Global Memory，并通过MTE3\_MTE2事件等待搬出完成。

## 核函数运行验证<a name="zh-cn_topic_0000001565030288_zh-cn_topic_0000001552186366_section136721210114419"></a>

完成Kernel侧核函数开发后，即可编写Host侧的核函数调用程序。实现从Host侧的APP程序调用算子，执行计算过程。

1.  Host侧应用程序框架的编写。

    ```
    // Host侧应用程序需要包含的头文件
    #include "acl/acl.h"
    // Kernel侧需要包含的头文件
    #include "kernel_operator.h"
    // 文件读写工具头文件
    #include "data_utils.h"
    // 核函数开发部分
    ...
    
    __global__ __vector__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z)
    {
        AscendC::InitSocState();
        KernelAdd op;
        op.Init(x, y, z);
        op.Process();
    }
    
    // 通过<<<...>>>内核调用符调用算子
    void KernelCall(aclrtStream stream)
    {
    ...
    }
    
    // 算子验证主程序
    int32_t main()
    {
        aclInit(nullptr);
        aclrtSetDevice(DEVICE_ID);
        aclrtStream stream = nullptr;
        aclrtCreateStream(&stream);
    
        KernelCall(stream);
    
        aclrtDestroyStream(stream);
        aclrtResetDevice(DEVICE_ID);
        aclFinalize();
        return 0;
    }
    ```

2.  编写**通过<<<...\>\>\>内核调用符调用**算子的代码。

    **图 5**  调用步骤<a name="zh-cn_topic_0000001565030288_fig1034911325594"></a>  
    ![](../../../figures/调用步骤.png "调用步骤")

    如下示例中的acl API使用方法请参考[《Runtime运行时API》](https://hiascend.com/document/redirect/CannCommunityRuntimeApi)。

    ```
    void KernelCall(aclrtStream stream)
    {
        uint8_t *inputXHost = nullptr;
        uint8_t *inputYHost = nullptr;
        uint8_t *outputHost = nullptr;
        uint8_t *inputXDevice = nullptr;
        uint8_t *inputYDevice = nullptr;
        uint8_t *outputDevice = nullptr;
        size_t fileSize = DATA_BYTE_SIZE;

        // 将Host上的输入数据拷贝到Device侧
        aclrtMallocHost((void **)(&inputXHost), DATA_BYTE_SIZE);
        aclrtMallocHost((void **)(&inputYHost), DATA_BYTE_SIZE);
        aclrtMallocHost((void **)(&outputHost), DATA_BYTE_SIZE);
        aclrtMalloc((void **)(&inputXDevice), DATA_BYTE_SIZE, ACL_MEM_MALLOC_HUGE_FIRST);
        aclrtMalloc((void **)(&inputYDevice), DATA_BYTE_SIZE, ACL_MEM_MALLOC_HUGE_FIRST);
        aclrtMalloc((void **)(&outputDevice), DATA_BYTE_SIZE, ACL_MEM_MALLOC_HUGE_FIRST);

        if (!ReadFile("./input/input_x.bin", fileSize, inputXHost, DATA_BYTE_SIZE) || fileSize != DATA_BYTE_SIZE) {
            ERROR_LOG("failed to read input x");
            return;
        }
        fileSize = DATA_BYTE_SIZE;
        if (!ReadFile("./input/input_y.bin", fileSize, inputYHost, DATA_BYTE_SIZE) || fileSize != DATA_BYTE_SIZE) {
            ERROR_LOG("failed to read input y");
            return;
        }
        aclrtMemcpy(inputXDevice, DATA_BYTE_SIZE, inputXHost, DATA_BYTE_SIZE, ACL_MEMCPY_HOST_TO_DEVICE);
        aclrtMemcpy(inputYDevice, DATA_BYTE_SIZE, inputYHost, DATA_BYTE_SIZE, ACL_MEMCPY_HOST_TO_DEVICE);

        // 用内核调用符<<<...>>>调用核函数完成指定的运算
        add_custom<<<BLOCK_NUM, nullptr, stream>>>(inputXDevice, inputYDevice, outputDevice);
        aclrtSynchronizeStream(stream);

        // 将Device上的运算结果拷贝回Host
        aclrtMemcpy(outputHost, DATA_BYTE_SIZE, outputDevice, DATA_BYTE_SIZE, ACL_MEMCPY_DEVICE_TO_HOST);
        WriteFile("./output/output.bin", outputHost, DATA_BYTE_SIZE);

        // 释放申请的资源
        aclrtFree(outputDevice);
        aclrtFree(inputYDevice);
        aclrtFree(inputXDevice);
        aclrtFreeHost(outputHost);
        aclrtFreeHost(inputYHost);
        aclrtFreeHost(inputXHost);
    }
    ```

3.  CMake编译配置如下：

    ```
    cmake_minimum_required(VERSION 3.16)
    set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")
    set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")
    # find_package(ASC)是CMake中用于查找和配置Ascend C编译工具链的命令
    find_package(ASC REQUIRED)
    # 指定项目支持的语言包括ASC和CXX，ASC表示支持使用毕昇编译器对Ascend C编程语言进行编译
    project(kernel_samples LANGUAGES ASC CXX)
    
    add_executable(add
        add.asc
    )
    
    # 通过编译选项设置NPU架构
    target_compile_options(add PRIVATE
       $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    )
    ```

4.  编译和运行步骤如下

    ```
    mkdir -p build && cd build; 
    cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;
    python3 ../scripts/gen_data.py
    ./add
    python3 ../scripts/verify_result.py output/output.bin output/golden.bin
    ```

    执行结果如下，说明精度比对成功。

    ```
    test pass!
    ```

5.  NPU上板打印

    这一步不是运行样例的必需步骤，主要用于排查算子精度问题。如果运行`verify_result.py`后发现比对不通过，需要进一步确认NPU侧数据是否符合预期，可以在`Process()`的数据搬入、计算、搬出等关键位置临时插入[DumpTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/DumpTensor.md)打印。

    ```cpp
    constexpr uint32_t DUMP_SIZE = 16;

    // DataCopy搬入前：查看GlobalTensor上的输入。
    AscendC::DumpTensor(xGm, 1, DUMP_SIZE);
    AscendC::DumpTensor(yGm, 2, DUMP_SIZE);

    // WaitFlag<AscendC::HardEvent::MTE2_V>之后：查看搬入后的LocalTensor。
    AscendC::DumpTensor(xLocal, 3, DUMP_SIZE);
    AscendC::DumpTensor(yLocal, 4, DUMP_SIZE);

    // WaitFlag<AscendC::HardEvent::V_MTE3>之后：查看计算得到的LocalTensor。
    AscendC::DumpTensor(zLocal, 5, DUMP_SIZE);

    // WaitFlag<AscendC::HardEvent::MTE3_MTE2>之后：查看GlobalTensor上的输出。
    AscendC::DumpTensor(zGm, 6, DUMP_SIZE);
    ```

    DumpTensor接口的第2个参数为自定义附加信息，可按调试习惯设置为行号或阶段编号，用于区分不同位置的打印结果。

6.  NPU上板性能调优

    样例精度比对通过后，如果需要进一步了解算子在NPU上的执行性能，可以使用`msprof op`采集上板性能数据。采集前请先去掉前一步临时插入的DumpTensor打印，或通过`ASCENDC_DUMP=0`关闭打印功能，避免打印开销影响性能数据。

    在`build`目录下执行如下命令，采集当前样例的性能数据。

    ```
    msprof op ./add
    ```

    采集完成后，可查看生成结果目录中的文件，初步判断算子瓶颈。

    ```
    ├── ArithmeticUtilization.csv  # cube/vector指令cycle占比
    ├── L2Cache.csv                # L2 Cache命中率
    ├── Memory.csv                 # UB、L1和主存储器读写带宽速率
    ├── MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
    ├── PipeUtilization.csv        # pipe类指令耗时和占比
    ├── ResourceConflictRatio.csv  # UB bank conflict等资源冲突占比
    └── visualize_data.bin         # MindStudio Insight呈现文件
    ```

    对于本样例，可以先关注`ArithmeticUtilization.csv`和`PipeUtilization.csv`，了解Vector计算与数据搬运的耗时占比；再结合`Memory.csv`、`MemoryUB.csv`、`L2Cache.csv`观察带宽和缓存命中情况。如果需要进一步做Roofline瓶颈分析或查看指令流水图，可参考[性能调优](../../../编程指南/调试调优/性能调优.md)。

    >[!NOTE]说明
    >-  该样例仅支持如下型号：
    >    - Ascend 950PR/Ascend 950DT
    >    - Atlas A3 训练系列产品/Atlas A3 推理系列产品
    >    - Atlas A2 训练系列产品/Atlas A2 推理系列产品
    >- _-_-npu-arch用于指定NPU的架构版本，dav-后为架构版本号，请替换为您实际使用的架构版本号。各AI处理器型号对应的架构版本号请通过[AI处理器型号和\_\_NPU\_ARCH\_\_的对应关系](../../../编程指南/语言扩展层/SIMD-BuiltIn关键字.md#table65291052154114)进行查询。
    >-   使用CPU调试或NPU仿真模式时，可在cmake命令中添加`-DCMAKE_ASC_RUN_MODE=cpu`或`-DCMAKE_ASC_RUN_MODE=sim`参数。

## 接下来的引导<a name="zh-cn_topic_0000001565030288_section128349412384"></a>

如果您对教程中的静态Tensor、同步事件等概念不了解，导致阅读过程有些吃力，可以参考[编程模型](../../../编程指南/编程模型/编程模型.md)学习基本概念，再来回顾本教程；如果您已经了解相关概念，并跑通了该样例，您可以参考[矢量编程](../../../算子实践参考/SIMD算子实现/矢量编程/矢量编程.md)了解Ascend C矢量编程中的更多细节。
