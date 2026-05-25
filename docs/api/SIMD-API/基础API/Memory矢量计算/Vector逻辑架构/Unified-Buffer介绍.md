# Unified Buffer介绍<a name="ZH-CN_TOPIC_0000002574022839"></a>

## Unified Buffer地址对齐约束<a name="zh-cn_topic_0000002524627718_section796754519912"></a>

**表 1**  不同存储单元的对齐要求

<a name="zh-cn_topic_0000002524627718_table16278354141117"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002524627718_row1827835418116"><th class="cellrowborder" valign="top" width="24.15%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002524627718_p1927845481114"><a name="zh-cn_topic_0000002524627718_p1927845481114"></a><a name="zh-cn_topic_0000002524627718_p1927845481114"></a>存储单元</p>
</th>
<th class="cellrowborder" valign="top" width="75.85%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002524627718_p15278165413113"><a name="zh-cn_topic_0000002524627718_p15278165413113"></a><a name="zh-cn_topic_0000002524627718_p15278165413113"></a>对齐要求</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002524627718_row5989518428"><td class="cellrowborder" valign="top" width="24.15%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002524627718_p12786547114"><a name="zh-cn_topic_0000002524627718_p12786547114"></a><a name="zh-cn_topic_0000002524627718_p12786547114"></a><span id="zh-cn_topic_0000002524627718_ph64037249135"><a name="zh-cn_topic_0000002524627718_ph64037249135"></a><a name="zh-cn_topic_0000002524627718_ph64037249135"></a>Global Memory</span></p>
</td>
<td class="cellrowborder" valign="top" width="75.85%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002524627718_p112781554101116"><a name="zh-cn_topic_0000002524627718_p112781554101116"></a><a name="zh-cn_topic_0000002524627718_p112781554101116"></a>Byte对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002524627718_row1427810549118"><td class="cellrowborder" valign="top" width="24.15%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002524627718_p152784546112"><a name="zh-cn_topic_0000002524627718_p152784546112"></a><a name="zh-cn_topic_0000002524627718_p152784546112"></a><span id="zh-cn_topic_0000002524627718_ph0136142113115"><a name="zh-cn_topic_0000002524627718_ph0136142113115"></a><a name="zh-cn_topic_0000002524627718_ph0136142113115"></a>Unified Buffer</span></p>
</td>
<td class="cellrowborder" valign="top" width="75.85%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002524627718_p104523511813"><a name="zh-cn_topic_0000002524627718_p104523511813"></a><a name="zh-cn_topic_0000002524627718_p104523511813"></a>32Byte对齐。</p>
</td>
</tr>
</tbody>
</table>

## Unified Buffer地址重叠约束<a name="zh-cn_topic_0000002524627718_section668772811100"></a>

使用基础API的Tensor高维切分计算接口时，为了节省地址空间，开发者可以定义一个Tensor，供源操作数与目的操作数同时使用（即地址重叠）。使用时需要注意以下约束：

-   单次迭代内：源操作数与目的操作数必须100%完全重叠，不支持部分重叠。
-   多次迭代间：不支持前序迭代的目的操作数与后序迭代的源操作数重叠。例如，第N次迭代的目的操作数是第N+1次的源操作数（如下图所示）。在这种情况下，第N次迭代可能会改写覆盖源操作数的数值，导致无法得到预期结果。特别地，对于部分双目计算类的API（Add、Sub、Mul、Max、Min、AddRelu、SubRelu），当数据类型为half、int32\_t、float时，支持前序迭代的目的操作数与后序迭代的源操作数重叠：仅针对目的操作数和第二个源操作数重叠的情况，且src1RepStride或者dstRepStride必须为0。

**图 1**  地址重叠示例（不支持）<a name="zh-cn_topic_0000002524627718_fig12654416181216"></a>  
![](figures/地址重叠示例（不支持）.png "地址重叠示例（不支持）")

>![](public_sys-resources/icon-note.gif) **说明：** 
>本节所述地址重叠通用约束适用于一般情况，API参考中如有额外特殊说明的，则以具体API中的说明为准。
>API中没有描述地址重叠约束的，视为不支持Tensor高维切分计算的地址重叠，地址重叠时计算结果可能不满足预期。

