# 核函数与VF函数<a name="ZH-CN_TOPIC_0000002593909249"></a>

SIMD与SIMT混合编程涉及多种类型的函数，它们之间遵循严格的调用关系和层级约束。本章节将会介绍整体函数类型和调用关系，再详细展开每种函数的定义、调用语法及约束。

SIMD与SIMT混合编程中涉及的函数类型如下表所示：

<a name="zh-cn_topic_0000002571578013_table1818191292017"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002571578013_row41811712112018"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.1"><p id="zh-cn_topic_0000002571578013_p3399101952013"><a name="zh-cn_topic_0000002571578013_p3399101952013"></a><a name="zh-cn_topic_0000002571578013_p3399101952013"></a>修饰符</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.2"><p id="zh-cn_topic_0000002571578013_p1039981902017"><a name="zh-cn_topic_0000002571578013_p1039981902017"></a><a name="zh-cn_topic_0000002571578013_p1039981902017"></a>函数功能</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.3"><p id="zh-cn_topic_0000002571578013_p173991119122018"><a name="zh-cn_topic_0000002571578013_p173991119122018"></a><a name="zh-cn_topic_0000002571578013_p173991119122018"></a>调用方式</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002571578013_row5182121252014"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002571578013_p23991719182018"><a name="zh-cn_topic_0000002571578013_p23991719182018"></a><a name="zh-cn_topic_0000002571578013_p23991719182018"></a>__global__ __aicore__</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002571578013_p12399819112018"><a name="zh-cn_topic_0000002571578013_p12399819112018"></a><a name="zh-cn_topic_0000002571578013_p12399819112018"></a>算子入口，协调VF执行</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002571578013_p43991419142018"><a name="zh-cn_topic_0000002571578013_p43991419142018"></a><a name="zh-cn_topic_0000002571578013_p43991419142018"></a>Host侧通过&lt;&lt;&lt;...&gt;&gt;&gt;调用</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row18182111232018"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002571578013_p75827493259"><a name="zh-cn_topic_0000002571578013_p75827493259"></a><a name="zh-cn_topic_0000002571578013_p75827493259"></a>__aicore__</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002571578013_p1639910195208"><a name="zh-cn_topic_0000002571578013_p1639910195208"></a><a name="zh-cn_topic_0000002571578013_p1639910195208"></a>Device侧辅助函数</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002571578013_p1539981972014"><a name="zh-cn_topic_0000002571578013_p1539981972014"></a><a name="zh-cn_topic_0000002571578013_p1539981972014"></a>核函数或同级函数调用</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row1718217127202"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002571578013_p223160142615"><a name="zh-cn_topic_0000002571578013_p223160142615"></a><a name="zh-cn_topic_0000002571578013_p223160142615"></a>__simt_vf__</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002571578013_p2039916193206"><a name="zh-cn_topic_0000002571578013_p2039916193206"></a><a name="zh-cn_topic_0000002571578013_p2039916193206"></a>线程级并行计算任务</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002571578013_p1539981910204"><a name="zh-cn_topic_0000002571578013_p1539981910204"></a><a name="zh-cn_topic_0000002571578013_p1539981910204"></a>通过SIMT提供的asc_vf_call接口调用</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row191821612202011"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002571578013_p7641116154913"><a name="zh-cn_topic_0000002571578013_p7641116154913"></a><a name="zh-cn_topic_0000002571578013_p7641116154913"></a>__simd_vf__</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002571578013_p63991519202018"><a name="zh-cn_topic_0000002571578013_p63991519202018"></a><a name="zh-cn_topic_0000002571578013_p63991519202018"></a>向量级并行计算任务</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002571578013_p16399191916204"><a name="zh-cn_topic_0000002571578013_p16399191916204"></a><a name="zh-cn_topic_0000002571578013_p16399191916204"></a>通过SIMD提供的asc_vf_call接口调用</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row7855201552319"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002571578013_p1642191624914"><a name="zh-cn_topic_0000002571578013_p1642191624914"></a><a name="zh-cn_topic_0000002571578013_p1642191624914"></a>__simt_callee__</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002571578013_p2856171502319"><a name="zh-cn_topic_0000002571578013_p2856171502319"></a><a name="zh-cn_topic_0000002571578013_p2856171502319"></a>SIMT VF的子函数</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002571578013_p1685617158235"><a name="zh-cn_topic_0000002571578013_p1685617158235"></a><a name="zh-cn_topic_0000002571578013_p1685617158235"></a>SIMT VF内部调用</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row6856215162317"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002571578013_p20856191514239"><a name="zh-cn_topic_0000002571578013_p20856191514239"></a><a name="zh-cn_topic_0000002571578013_p20856191514239"></a>__simd_callee__</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002571578013_p12856161514238"><a name="zh-cn_topic_0000002571578013_p12856161514238"></a><a name="zh-cn_topic_0000002571578013_p12856161514238"></a>SIMD VF的辅助子函数</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002571578013_p1685631510231"><a name="zh-cn_topic_0000002571578013_p1685631510231"></a><a name="zh-cn_topic_0000002571578013_p1685631510231"></a>SIMD VF内部调用</p>
</td>
</tr>
</tbody>
</table>

函数调用关系如下图所示：

![](../../../../figures/SIMT_混合_函数调用层级.png)

## 核函数的定义与执行配置<a name="zh-cn_topic_0000002571578013_section156822920311"></a>

核函数是SIMD与SIMT混合编程的Device侧入口函数，负责协调整个算子的执行流程，包括VF的调度和调用。函数定义语法为：

```
__global__ __vector__ void kernel_name(__gm__ type* param1, __gm__ type* param2, ...);
```

关键修饰符说明如下：

<a name="zh-cn_topic_0000002571578013_table4811406341"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002571578013_row1785401349"><th class="cellrowborder" valign="top" width="19.101910191019105%" id="mcps1.1.4.1.1"><p id="zh-cn_topic_0000002571578013_p19200152183411"><a name="zh-cn_topic_0000002571578013_p19200152183411"></a><a name="zh-cn_topic_0000002571578013_p19200152183411"></a>修饰符</p>
</th>
<th class="cellrowborder" valign="top" width="47.56475647564757%" id="mcps1.1.4.1.2"><p id="zh-cn_topic_0000002571578013_p3200852103417"><a name="zh-cn_topic_0000002571578013_p3200852103417"></a><a name="zh-cn_topic_0000002571578013_p3200852103417"></a>作用</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.1.4.1.3"><p id="zh-cn_topic_0000002571578013_p1520010522341"><a name="zh-cn_topic_0000002571578013_p1520010522341"></a><a name="zh-cn_topic_0000002571578013_p1520010522341"></a>必需性</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002571578013_row18834063411"><td class="cellrowborder" valign="top" width="19.101910191019105%" headers="mcps1.1.4.1.1 "><pre class="screen" id="zh-cn_topic_0000002571578013_screen112004526349"><a name="zh-cn_topic_0000002571578013_screen112004526349"></a><a name="zh-cn_topic_0000002571578013_screen112004526349"></a>__global__</pre>
</td>
<td class="cellrowborder" valign="top" width="47.56475647564757%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002571578013_p520055213411"><a name="zh-cn_topic_0000002571578013_p520055213411"></a><a name="zh-cn_topic_0000002571578013_p520055213411"></a>标识核函数，表明可在Host侧通过&lt;&lt;&lt;...&gt;&gt;&gt;调用</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002571578013_p1520075215340"><a name="zh-cn_topic_0000002571578013_p1520075215340"></a><a name="zh-cn_topic_0000002571578013_p1520075215340"></a>必需</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row2817406345"><td class="cellrowborder" valign="top" width="19.101910191019105%" headers="mcps1.1.4.1.1 "><pre class="screen" id="zh-cn_topic_0000002571578013_screen112001152153412"><a name="zh-cn_topic_0000002571578013_screen112001152153412"></a><a name="zh-cn_topic_0000002571578013_screen112001152153412"></a>__vector__</pre>
</td>
<td class="cellrowborder" valign="top" width="47.56475647564757%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002571578013_p1120113524345"><a name="zh-cn_topic_0000002571578013_p1120113524345"></a><a name="zh-cn_topic_0000002571578013_p1120113524345"></a>标识函数是在Device侧AI Vector上执行</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002571578013_p1520175263410"><a name="zh-cn_topic_0000002571578013_p1520175263410"></a><a name="zh-cn_topic_0000002571578013_p1520175263410"></a>必需</p>
</td>
</tr>
</tbody>
</table>

核函数定义有以下几个约束：

-   返回值类型必须是void；
-   入参支持指针类型（需使用\_\_gm\_\_修饰）和Ascend C内置数据类型；
-   指针参数必须是指向Global Memory上的内存地址，使用\_\_gm\_\_修饰。

核函数的调用是通过<<<...\>\>\>内核调用符在Host侧调用，语法如下：

```
kernel_name<<<block_num, dyn_ub_size, stream>>>(args...);
```

内核调用符内的配置参数说明如下：

<a name="zh-cn_topic_0000002571578013_table942016184315"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002571578013_row1743620634313"><th class="cellrowborder" valign="top" width="14.09%" id="mcps1.1.5.1.1"><p id="zh-cn_topic_0000002571578013_p243616624318"><a name="zh-cn_topic_0000002571578013_p243616624318"></a><a name="zh-cn_topic_0000002571578013_p243616624318"></a>参数</p>
</th>
<th class="cellrowborder" valign="top" width="17.580000000000002%" id="mcps1.1.5.1.2"><p id="zh-cn_topic_0000002571578013_p9436186154312"><a name="zh-cn_topic_0000002571578013_p9436186154312"></a><a name="zh-cn_topic_0000002571578013_p9436186154312"></a>类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.1.5.1.3"><p id="zh-cn_topic_0000002571578013_p1343696124316"><a name="zh-cn_topic_0000002571578013_p1343696124316"></a><a name="zh-cn_topic_0000002571578013_p1343696124316"></a>说明</p>
</th>
<th class="cellrowborder" valign="top" width="43.33%" id="mcps1.1.5.1.4"><p id="zh-cn_topic_0000002571578013_p34361063433"><a name="zh-cn_topic_0000002571578013_p34361063433"></a><a name="zh-cn_topic_0000002571578013_p34361063433"></a>约束</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002571578013_row54361763432"><td class="cellrowborder" valign="top" width="14.09%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002571578013_p643626184318"><a name="zh-cn_topic_0000002571578013_p643626184318"></a><a name="zh-cn_topic_0000002571578013_p643626184318"></a>block_num</p>
</td>
<td class="cellrowborder" valign="top" width="17.580000000000002%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002571578013_p1443619614314"><a name="zh-cn_topic_0000002571578013_p1443619614314"></a><a name="zh-cn_topic_0000002571578013_p1443619614314"></a>uint32_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002571578013_p1643646154312"><a name="zh-cn_topic_0000002571578013_p1643646154312"></a><a name="zh-cn_topic_0000002571578013_p1643646154312"></a>设置核函数启用的核数</p>
</td>
<td class="cellrowborder" valign="top" width="43.33%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002571578013_p3436166134316"><a name="zh-cn_topic_0000002571578013_p3436166134316"></a><a name="zh-cn_topic_0000002571578013_p3436166134316"></a>取值范围[1, 65536]</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row184361465430"><td class="cellrowborder" valign="top" width="14.09%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002571578013_p2436186134320"><a name="zh-cn_topic_0000002571578013_p2436186134320"></a><a name="zh-cn_topic_0000002571578013_p2436186134320"></a>dyn_ub_size</p>
</td>
<td class="cellrowborder" valign="top" width="17.580000000000002%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002571578013_p20436168435"><a name="zh-cn_topic_0000002571578013_p20436168435"></a><a name="zh-cn_topic_0000002571578013_p20436168435"></a>uint32_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002571578013_p74363644313"><a name="zh-cn_topic_0000002571578013_p74363644313"></a><a name="zh-cn_topic_0000002571578013_p74363644313"></a>指定动态内存大小，单位为字节</p>
</td>
<td class="cellrowborder" valign="top" width="43.33%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002571578013_p19436136144312"><a name="zh-cn_topic_0000002571578013_p19436136144312"></a><a name="zh-cn_topic_0000002571578013_p19436136144312"></a>不超过最大可配置值：256KB - 8KB - 32KB - 静态内存</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row943610674310"><td class="cellrowborder" valign="top" width="14.09%" headers="mcps1.1.5.1.1 "><p id="zh-cn_topic_0000002571578013_p1143711684313"><a name="zh-cn_topic_0000002571578013_p1143711684313"></a><a name="zh-cn_topic_0000002571578013_p1143711684313"></a>stream</p>
</td>
<td class="cellrowborder" valign="top" width="17.580000000000002%" headers="mcps1.1.5.1.2 "><p id="zh-cn_topic_0000002571578013_p0437467438"><a name="zh-cn_topic_0000002571578013_p0437467438"></a><a name="zh-cn_topic_0000002571578013_p0437467438"></a>aclrtStream</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.1.5.1.3 "><p id="zh-cn_topic_0000002571578013_p743756114318"><a name="zh-cn_topic_0000002571578013_p743756114318"></a><a name="zh-cn_topic_0000002571578013_p743756114318"></a>用于维护异步操作执行顺序</p>
</td>
<td class="cellrowborder" valign="top" width="43.33%" headers="mcps1.1.5.1.4 "><p id="zh-cn_topic_0000002571578013_p144371561433"><a name="zh-cn_topic_0000002571578013_p144371561433"></a><a name="zh-cn_topic_0000002571578013_p144371561433"></a>无</p>
</td>
</tr>
</tbody>
</table>

## SIMT VF函数<a name="zh-cn_topic_0000002571578013_section1780955884616"></a>

SIMT VF函数用于实现线程级并行计算任务，处理不规则访问和复杂控制逻辑，函数内可使用SIMT内置变量：threadIdx、blockIdx、blockDim、gridDim。函数定义示例如下：

```
__simt_vf__ __launch_bounds__(MAX_THREAD_COUNT) inline void function_name(
    __gm__ type* gm_param, 
    __ubuf__ type* ubuf_param, 
    type scalar_param, ...);
```

SIMT VF函数定义中的关键修饰符说明如下：

<a name="zh-cn_topic_0000002571578013_table7661145014492"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002571578013_row2681250114917"><th class="cellrowborder" valign="top" width="30.509999999999998%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002571578013_p1168111503490"><a name="zh-cn_topic_0000002571578013_p1168111503490"></a><a name="zh-cn_topic_0000002571578013_p1168111503490"></a>修饰符</p>
</th>
<th class="cellrowborder" valign="top" width="69.49%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002571578013_p166813508492"><a name="zh-cn_topic_0000002571578013_p166813508492"></a><a name="zh-cn_topic_0000002571578013_p166813508492"></a>作用</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002571578013_row268125019492"><td class="cellrowborder" valign="top" width="30.509999999999998%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002571578013_p859861412317"><a name="zh-cn_topic_0000002571578013_p859861412317"></a><a name="zh-cn_topic_0000002571578013_p859861412317"></a>__simt_vf__</p>
</td>
<td class="cellrowborder" valign="top" width="69.49%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002571578013_p1368117503498"><a name="zh-cn_topic_0000002571578013_p1368117503498"></a><a name="zh-cn_topic_0000002571578013_p1368117503498"></a>函数标识符，标识SIMT VF函数</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row19681450154918"><td class="cellrowborder" valign="top" width="30.509999999999998%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002571578013_p1940119202314"><a name="zh-cn_topic_0000002571578013_p1940119202314"></a><a name="zh-cn_topic_0000002571578013_p1940119202314"></a>__launch_bounds__(N)</p>
</td>
<td class="cellrowborder" valign="top" width="69.49%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002571578013_p3682155012493"><a name="zh-cn_topic_0000002571578013_p3682155012493"></a><a name="zh-cn_topic_0000002571578013_p3682155012493"></a>指定最大线程数（可选，默认1024）</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row8682165014496"><td class="cellrowborder" valign="top" width="30.509999999999998%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002571578013_p2781142523110"><a name="zh-cn_topic_0000002571578013_p2781142523110"></a><a name="zh-cn_topic_0000002571578013_p2781142523110"></a>inline</p>
</td>
<td class="cellrowborder" valign="top" width="69.49%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002571578013_p1682115014910"><a name="zh-cn_topic_0000002571578013_p1682115014910"></a><a name="zh-cn_topic_0000002571578013_p1682115014910"></a>建议内联，实际是否内联由编译器决定</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row146823500499"><td class="cellrowborder" valign="top" width="30.509999999999998%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002571578013_p10181203283119"><a name="zh-cn_topic_0000002571578013_p10181203283119"></a><a name="zh-cn_topic_0000002571578013_p10181203283119"></a>__gm__</p>
</td>
<td class="cellrowborder" valign="top" width="69.49%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002571578013_p136829505495"><a name="zh-cn_topic_0000002571578013_p136829505495"></a><a name="zh-cn_topic_0000002571578013_p136829505495"></a>内存空间修饰符，标识内存空间为GM</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002571578013_row168295015495"><td class="cellrowborder" valign="top" width="30.509999999999998%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002571578013_p86781651183118"><a name="zh-cn_topic_0000002571578013_p86781651183118"></a><a name="zh-cn_topic_0000002571578013_p86781651183118"></a>__ubuf__</p>
</td>
<td class="cellrowborder" valign="top" width="69.49%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002571578013_p1682155016496"><a name="zh-cn_topic_0000002571578013_p1682155016496"></a><a name="zh-cn_topic_0000002571578013_p1682155016496"></a>内存空间修饰符，标识内存空间为UB</p>
</td>
</tr>
</tbody>
</table>

通过SIMT的[asc\_vf\_call](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/SIMD%E4%B8%8ESIMT%E6%B7%B7%E5%90%88%E7%BC%96%E7%A8%8B%E7%AE%80%E4%BB%8B/%E6%89%A9%E5%B1%95%E8%AF%AD%E6%B3%95/%E6%A0%B8%E5%87%BD%E6%95%B0%E9%85%8D%E7%BD%AE-147.md#asc_vf_call%E8%B0%83%E7%94%A8)接口在核函数或\_\_aicore\_\_函数中调用，调用示例如下：

```
asc_vf_call<function_name>(dim3(blockDim), arg1, arg2, ...);
```

SIMT VF函数有以下约束：

-   入参仅支持Ascend C的[内置数据类型](../../../语言扩展层/SIMD&SIMT-BuiltIn关键字.md#zh-cn_topic_0000002571575581_section1880403364916)（int32\_t、uint32\_t、float、half等）及其组成的指针、数组、结构体类型，且指针类型必须指向GM或者UB内存。
-   函数返回类型必须是void。
-   SIMT VF内只能调用\_\_simt\_callee\_\_函数或constexpr \_\_aicore\_\_函数。


## \_\_simt\_callee\_\_子函数<a name="zh-cn_topic_0000002571578013_section4680126514"></a>

\_\_simt\_callee\_\_子函数是SIMT VF函数内部调用的辅助函数，函数内部可以使用SIMT内置变量。定义示例如下：

```
__simt_callee__ return_type function_name(__gm__ type* gm_param, __ubuf__ type* ubuf_param, type scalar_param, ...);
```

调用示例如下：

```
return_type result = function_name(arg1, arg2, ...);
```

该函数有以下约束：

-   入参仅支持Ascend C的[内置数据类型](../../../语言扩展层/SIMD&SIMT-BuiltIn关键字.md#zh-cn_topic_0000002571575581_section1880403364916)（int32\_t、uint32\_t、float、half等）和指针类型。
-   函数返回值只能是Ascend C的[内置数据类型](../../../语言扩展层/SIMD&SIMT-BuiltIn关键字.md#zh-cn_topic_0000002571575581_section1880403364916)（int32\_t、uint32\_t、float、half等）及对应的指针类型。
-   函数内只能调用\_\_simt\_callee\_\_函数或constexpr \_\_aicore\_\_函数。
