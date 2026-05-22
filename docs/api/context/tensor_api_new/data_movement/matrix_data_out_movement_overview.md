# 矩阵计算的搬出总体说明

矩阵计算的搬出是AscendC编程框架中用于数据搬运的核心接口系列，主要实现L0C与Global Memory、UB之间的数据高效传输。该接口系列提供了多种数据搬运模式，能够满足矩阵计算中左右矩阵等数据的搬运需求，通常在Mmad接口后配合使用。

矩阵计算的搬出接口支持多种随路搬运场景，包括**随路量化、随路Relu、随路格式转换\(NZ2ND\)、随路通道拆分\(channel split\)、随路通道合并\(channel merge\)**。针对Ascend 950PR/Ascend 950DT，还支持NZ2DN随路格式转换和**UB双目标模式。**

针对Ascend 950PR/Ascend 950DT：

**图 1**  fixpipe整体流程图<a name="zh-cn_topic_0000002542682989_fig7998719163213"></a>  
![](figures/fixpipe整体流程图-66.png "fixpipe整体流程图-66")

**数据通路与存储层级**

矩阵计算的搬出接口支持多种数据通路，每种通路对应特定的存储层级和访问特性。

针对Ascend 950PR/Ascend 950DT：

<a name="zh-cn_topic_0000002542682989_table135162019133315"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002542682989_row1516151923315"><th class="cellrowborder" valign="top" width="16.76832316768323%" id="mcps1.1.6.1.1"><p id="zh-cn_topic_0000002542682989_p10516161973315"><a name="zh-cn_topic_0000002542682989_p10516161973315"></a><a name="zh-cn_topic_0000002542682989_p10516161973315"></a>源位置</p>
</th>
<th class="cellrowborder" valign="top" width="13.638636136386362%" id="mcps1.1.6.1.2"><p id="zh-cn_topic_0000002542682989_p35161219113313"><a name="zh-cn_topic_0000002542682989_p35161219113313"></a><a name="zh-cn_topic_0000002542682989_p35161219113313"></a>源地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="11.868813118688132%" id="mcps1.1.6.1.3"><p id="zh-cn_topic_0000002542682989_p65165199339"><a name="zh-cn_topic_0000002542682989_p65165199339"></a><a name="zh-cn_topic_0000002542682989_p65165199339"></a>目的位置</p>
</th>
<th class="cellrowborder" valign="top" width="17.908209179082093%" id="mcps1.1.6.1.4"><p id="zh-cn_topic_0000002542682989_p3516101917339"><a name="zh-cn_topic_0000002542682989_p3516101917339"></a><a name="zh-cn_topic_0000002542682989_p3516101917339"></a>目的地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="39.81601839816019%" id="mcps1.1.6.1.5"><p id="zh-cn_topic_0000002542682989_p65161919103318"><a name="zh-cn_topic_0000002542682989_p65161919103318"></a><a name="zh-cn_topic_0000002542682989_p65161919103318"></a>典型应用场景</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002542682989_row05161919103316"><td class="cellrowborder" valign="top" width="16.76832316768323%" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002542682989_p1516161912332"><a name="zh-cn_topic_0000002542682989_p1516161912332"></a><a name="zh-cn_topic_0000002542682989_p1516161912332"></a>L0C</p>
</td>
<td class="cellrowborder" valign="top" width="13.638636136386362%" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002542682989_p5516819163316"><a name="zh-cn_topic_0000002542682989_p5516819163316"></a><a name="zh-cn_topic_0000002542682989_p5516819163316"></a>64字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688132%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002542682989_p251661913333"><a name="zh-cn_topic_0000002542682989_p251661913333"></a><a name="zh-cn_topic_0000002542682989_p251661913333"></a>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="17.908209179082093%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0000002542682989_p205162019133317"><a name="zh-cn_topic_0000002542682989_p205162019133317"></a><a name="zh-cn_topic_0000002542682989_p205162019133317"></a>1字节/32字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.81601839816019%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0000002542682989_p10516171918334"><a name="zh-cn_topic_0000002542682989_p10516171918334"></a><a name="zh-cn_topic_0000002542682989_p10516171918334"></a>从L0C搬出数据至GM（NZ2NZ/NZ2ND/NZ2DN）</p>

</td>
</tr>
<tr id="zh-cn_topic_0000002542682989_row1351731917338"><td class="cellrowborder" valign="top" width="16.76832316768323%" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002542682989_p145171119153317"><a name="zh-cn_topic_0000002542682989_p145171119153317"></a><a name="zh-cn_topic_0000002542682989_p145171119153317"></a>L0C</p>
</td>
<td class="cellrowborder" valign="top" width="13.638636136386362%" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002542682989_p1751716197333"><a name="zh-cn_topic_0000002542682989_p1751716197333"></a><a name="zh-cn_topic_0000002542682989_p1751716197333"></a>64字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688132%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002542682989_p2517519183313"><a name="zh-cn_topic_0000002542682989_p2517519183313"></a><a name="zh-cn_topic_0000002542682989_p2517519183313"></a>UB</p>
</td>
<td class="cellrowborder" valign="top" width="17.908209179082093%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0000002542682989_p9517519183316"><a name="zh-cn_topic_0000002542682989_p9517519183316"></a><a name="zh-cn_topic_0000002542682989_p9517519183316"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.81601839816019%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0000002542682989_p1851711916332"><a name="zh-cn_topic_0000002542682989_p1851711916332"></a><a name="zh-cn_topic_0000002542682989_p1851711916332"></a>从L0C搬出数据至UB(NZ2NZ/NZ2ND/NZ2DN)</p>
</td>
</tr>
</tbody>
</table>

**接口分类与功能场景**

针对Ascend 950PR/Ascend 950DT：

<a name="zh-cn_topic_0000002542682989_table4531205110133"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002542682989_row45451951101311"><th class="cellrowborder" valign="top" width="7.3999999999999995%" id="mcps1.1.6.1.1"><p id="zh-cn_topic_0000002542682989_p8545145181318"><a name="zh-cn_topic_0000002542682989_p8545145181318"></a><a name="zh-cn_topic_0000002542682989_p8545145181318"></a>接口类别</p>
</th>
<th class="cellrowborder" valign="top" width="12.75%" id="mcps1.1.6.1.2"><p id="zh-cn_topic_0000002542682989_p554525131315"><a name="zh-cn_topic_0000002542682989_p554525131315"></a><a name="zh-cn_topic_0000002542682989_p554525131315"></a>主要功能</p>
</th>
<th class="cellrowborder" valign="top" width="30.099999999999998%" id="mcps1.1.6.1.3"><p id="zh-cn_topic_0000002542682989_p1954575191311"><a name="zh-cn_topic_0000002542682989_p1954575191311"></a><a name="zh-cn_topic_0000002542682989_p1954575191311"></a>支持的随路特性能力</p>
</th>
<th class="cellrowborder" valign="top" width="36.49%" id="mcps1.1.6.1.4"><p id="zh-cn_topic_0000002542682989_p124605811328"><a name="zh-cn_topic_0000002542682989_p124605811328"></a><a name="zh-cn_topic_0000002542682989_p124605811328"></a>搬出支持的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="13.26%" id="mcps1.1.6.1.5"><p id="zh-cn_topic_0000002542682989_p16620125204610"><a name="zh-cn_topic_0000002542682989_p16620125204610"></a><a name="zh-cn_topic_0000002542682989_p16620125204610"></a>流水类型</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002542682989_row105453518135"><td class="cellrowborder" valign="top" width="7.3999999999999995%" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002542682989_p1954517514134"><a name="zh-cn_topic_0000002542682989_p1954517514134"></a><a name="zh-cn_topic_0000002542682989_p1954517514134"></a>L0C到GM</p>
</td>
<td class="cellrowborder" valign="top" width="12.75%" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002542682989_p12545351121319"><a name="zh-cn_topic_0000002542682989_p12545351121319"></a><a name="zh-cn_topic_0000002542682989_p12545351121319"></a>实现L0C到GM的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="30.099999999999998%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002542682989_p2085013304224"><a name="zh-cn_topic_0000002542682989_p2085013304224"></a><a name="zh-cn_topic_0000002542682989_p2085013304224"></a><a href="quant_pre.md">随路量化</a>(Quant_PRE)、<a href="relu_pre.md">随路Relu</a>(ReLU_PRE)、<a href="int8_int4_channel_merge.md">Int8/Int4 Channel Merge</a>、NZ2ND、NZ2NZ、NZ2DN、<a href="f32_channel_split.md">F32 Channel Split</a></p>
</td>
<td class="cellrowborder" valign="top" width="36.49%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0000002542682989_p13461258183219"><a name="zh-cn_topic_0000002542682989_p13461258183219"></a><a name="zh-cn_topic_0000002542682989_p13461258183219"></a>float、int32_t、half、bfloat16_t、uint8_t/int8_t、fp8_e4m3fn_t、hifloat8_t</p>
</td>
<td class="cellrowborder" valign="top" width="13.26%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0000002542682989_p156201752104620"><a name="zh-cn_topic_0000002542682989_p156201752104620"></a><a name="zh-cn_topic_0000002542682989_p156201752104620"></a>PIPE_FIX</p>

</td>
</tr>
<tr id="zh-cn_topic_0000002542682989_row1718919141820"><td class="cellrowborder" valign="top" width="7.3999999999999995%" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002542682989_p12718171920183"><a name="zh-cn_topic_0000002542682989_p12718171920183"></a><a name="zh-cn_topic_0000002542682989_p12718171920183"></a>L0C到UB</p>
</td>
<td class="cellrowborder" valign="top" width="12.75%" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002542682989_p2718161991815"><a name="zh-cn_topic_0000002542682989_p2718161991815"></a><a name="zh-cn_topic_0000002542682989_p2718161991815"></a>实现L0C到UB的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="30.099999999999998%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002542682989_p13559738204515"><a name="zh-cn_topic_0000002542682989_p13559738204515"></a><a name="zh-cn_topic_0000002542682989_p13559738204515"></a><a href="quant_pre.md">随路量化</a>(Quant_PRE)、<a href="relu_pre.md">随路Relu</a>(ReLU_PRE)、<a href="int8_int4_channel_merge.md">Int8/Int4 Channel Merge</a>、NZ2NZ、NZ2ND、NZ2DN、<a href="f32_channel_split.md">F32 Channel Split</a>、<a href="l0c_to_ub_dual_dst.md">UB双目标模式</a></p>
</td>
<td class="cellrowborder" valign="top" width="36.49%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0000002542682989_p10718131916184"><a name="zh-cn_topic_0000002542682989_p10718131916184"></a><a name="zh-cn_topic_0000002542682989_p10718131916184"></a>float、int32_t、half、bfloat16_t、uint8_t/int8_t、fp8_e4m3fn_t、hifloat8_t</p>
</td>
<td class="cellrowborder" valign="top" width="13.26%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0000002542682989_p10718131918186"><a name="zh-cn_topic_0000002542682989_p10718131918186"></a><a name="zh-cn_topic_0000002542682989_p10718131918186"></a>PIPE_FIX</p>
</td>
</tr>
</tbody>
</table>
