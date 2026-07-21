# SK\_BIND<a name="ZH-CN_TOPIC_0000002565043461"></a>

## 产品支持情况<a name="section1550532418810"></a>

<!-- npu="950" id3 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id3 -->
<!-- npu="A3" id4 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id4 -->
<!-- npu="910b" id5 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id5 -->
<!-- npu="310b" id6 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id7 -->
<!-- npu="310p" id8 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id8 -->
<!-- npu="910" id9 -->
- Atlas 训练系列产品：不支持
<!-- end id9 -->
<!-- npu="x90" id1 -->
- Kirin X90：不支持
<!-- end id1 -->
<!-- npu="9030" id2 -->
- Kirin 9030：不支持
<!-- end id2 -->

## 功能说明<a name="section259105813316"></a>

本接口为算子SuperKernel场景提供绑定原核函数和SK子函数的能力。核函数直调算子的完整适配方法请参见[核函数直调算子额外适配说明](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/guide/编程指南/高级编程/SuperKernel/核函数直调算子额外适配说明.md)。

## 函数原型<a name="section2067518173415"></a>

```cpp
// GF, cap, SK0, ...
#define SK_BIND(...)
```

## 参数说明<a name="section158061867342"></a>

<a name="zh-cn_topic_0235751031_table33761356"></a>
<table><thead align="left"><tr id="zh-cn_topic_0235751031_row27598891"><th class="cellrowborder" valign="top" width="16.49%" id="mcps1.1.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="11.93%" id="mcps1.1.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="71.58%" id="mcps1.1.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row42461942101815"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.1.4.1.1 "><p id="p45208478318"><a name="p45208478318"></a><a name="p45208478318"></a>GF</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.1.4.1.2 "><p id="p135196472314"><a name="p135196472314"></a><a name="p135196472314"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.1.4.1.3 "><p id="p8686154014598"><a name="p8686154014598"></a><a name="p8686154014598"></a>核函数函数签名。</p>
</td>
</tr>
<tr id="row163919564263"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.1.4.1.1 "><p id="p1563916565265"><a name="p1563916565265"></a><a name="p1563916565265"></a>cap</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.1.4.1.2 "><p id="p59396564285"><a name="p59396564285"></a><a name="p59396564285"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.1.4.1.3 "><p id="p183314251610"><a name="p183314251610"></a><a name="p183314251610"></a>uint64_t类型，用于标记当前算子的SuperKernel特性。SuperKernel框架根据这些信息调整相应的同步和融合策略。各bit位含义如下，可按位或组合：</p>
<a name="ul138368184718"></a><a name="ul138368184718"></a><ul id="ul138368184718">
<li>1：early start wait flag，表示算子内使用了<a href="https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/api/SIMD-API/基础API/同步控制/任务间同步/WaitPreTaskEnd.md">WaitPreTaskEnd</a>接口。</li>
<li>2：early start set flag，表示算子内使用了<a href="https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/api/SIMD-API/基础API/同步控制/任务间同步/SetNextTaskStart.md">SetNextTaskStart</a>接口。</li>
<li>4：disable_dcci，表示当前算子需要SuperKernel框架关闭调用前后的DCCI ALL指令。详细说明请参见<a href="https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/guide/编程指南/高级编程/SuperKernel/算子适配说明.md">算子适配说明</a>。</li>
<li>8：disable_batchmode_check，表示跳过当前算子的<code>__schedmode__</code>检查，使该算子可继续参与SuperKernel融合。</li>
</ul>
<p id="p15604318228"><a name="p15604318228"></a><a name="p15604318228"></a>例如，同时配置early start wait flag和early start set flag时，cap取值为3。</p>
</td>
</tr>
<tr id="row91818456218"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.1.4.1.1 "><p id="p3182455216"><a name="p3182455216"></a><a name="p3182455216"></a>SK0</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.1.4.1.2 "><p id="p121884517219"><a name="p121884517219"></a><a name="p121884517219"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.1.4.1.3 "><p id="p218144514213"><a name="p218144514213"></a><a name="p218144514213"></a>SK子函数签名。</p>
</td>
</tr>
<tr id="row1781010838"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.1.4.1.1 "><p id="p2815101932"><a name="p2815101932"></a><a name="p2815101932"></a>...</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.1.4.1.2 "><p id="p11812101933"><a name="p11812101933"></a><a name="p11812101933"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.1.4.1.3 "><p id="p8184724649"><a name="p8184724649"></a><a name="p8184724649"></a>SK1~SK3</p>
<p id="p481171018314"><a name="p481171018314"></a><a name="p481171018314"></a>可提供多个SK子函数签名，包含SK0最多四个函数签名。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section43265506459"></a>

-   一个核函数最多绑定四个SK子函数。
-   核函数直调算子目前仅支持在npugraph_ex后端进入SuperKernel，不支持GE图模式。

## 需要包含的头文件<a name="section10354115115916"></a>

使用该接口需要包含"kernel\_operator.h"头文件。

```cpp
#include "kernel_operator.h"
```

## 调用示例<a name="section990974612242"></a>

以下示例展示SK_BIND接口的基本用法，SK子函数签名、参数结构体定义等完整规则请参见[核函数直调算子额外适配说明](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/guide/编程指南/高级编程/SuperKernel/核函数直调算子额外适配说明.md)。

```cpp
#include "kernel_operator.h"

// 原普通kernel保留（用于非SuperKernel场景）
__global__ __vector__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z, uint32_t totalLength) 
{ 
    KernelAdd op; 
    op.Init(x, y, z, totalLength); 
    op.Process(); 
} 

// 规则3：定义参数结构体（根据原global函数的实际参数定义）
struct AddCustomArgs {
   GM_ADDR x;                      // 对应add_custom的第一个参数 
   GM_ADDR y;                      // 对应add_custom的第二个参数 
   GM_ADDR z;                      // 对应add_custom的第三个参数 
   uint32_t totalLength;           // 对应add_custom的第四个参数
};  
// 定义一个带模板参数的SK子函数
// 模板参数仅用于实例化出不同的符号，不影响函数逻辑
template<uint32_t splitNum>
__sk__ __vector__ void add_custom_sk(const AddCustomArgs *args, sk::SkSystemArgs *sysArgs/* 可选添加sysArgs参数 */)
{
    // 从结构体中获取参数
    GM_ADDR x = args->x;
    GM_ADDR y = args->y; 
    GM_ADDR z = args->z; 
    uint32_t totalLength = args->totalLength; 
    // 规则5：逻辑与global函数一致
    KernelAdd op;
    op.Init(x, y, z, totalLength);
    op.Process();
}
// 规则6：使用SK_BIND绑定
// 通过指定模板参数实例化出4个不同的符号
// cap取值4表示配置disable_dcci
SK_BIND(add_custom, 4, add_custom_sk<0>, add_custom_sk<1>, add_custom_sk<2>, add_custom_sk<3>);
```
