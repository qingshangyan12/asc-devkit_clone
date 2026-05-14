# shfl\_up<a name="ZH-CN_TOPIC_0000002568552826"></a>

## 功能说明<a name="section618mcpsimp"></a>

获取组内当前线程向前偏移delta的线程的数据。

## 函数原型<a name="section620mcpsimp"></a>

```
T shfl_up(T var, int delta) const
```

## 参数说明<a name="section0866173114710"></a>

**表 1**  参数说明

<a name="zh-cn_topic_0235751031_table33761356"></a>
<table><thead align="left"><tr id="zh-cn_topic_0235751031_row27598891"><th class="cellrowborder" valign="top" width="11.91%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="11.04%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.05%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row42461942101815"><td class="cellrowborder" valign="top" width="11.91%" headers="mcps1.2.4.1.1 "><p id="p284425844311"><a name="p284425844311"></a><a name="p284425844311"></a>var</p>
</td>
<td class="cellrowborder" valign="top" width="11.04%" headers="mcps1.2.4.1.2 "><p id="p158449584436"><a name="p158449584436"></a><a name="p158449584436"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.05%" headers="mcps1.2.4.1.3 "><p id="p6389114616514"><a name="p6389114616514"></a><a name="p6389114616514"></a>线程用于交换的输入操作数。</p>
</td>
</tr>
<tr id="row1678544713120"><td class="cellrowborder" valign="top" width="11.91%" headers="mcps1.2.4.1.1 "><p id="p9785247183110"><a name="p9785247183110"></a><a name="p9785247183110"></a>delta</p>
</td>
<td class="cellrowborder" valign="top" width="11.04%" headers="mcps1.2.4.1.2 "><p id="p7785547123115"><a name="p7785547123115"></a><a name="p7785547123115"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.05%" headers="mcps1.2.4.1.3 "><p id="p27852472313"><a name="p27852472313"></a><a name="p27852472313"></a>期望获取的var值所在线程相对当前线程的向前偏移值。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

组内指定线程的var值。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section134121647154719"></a>

```
coalesced_group active = coalesced_threads();
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    uint32_t result = active.shfl_up(threadIdx.x + 100, 2);
}
```

示例代码中的条件分支将一个warp中所有线程id是偶数的线程组成coalesced\_group协作组，组内各线程shfl\_up接口返回结果如下图所示。

**图 1**  shfl\_up结果示意图<a name="fig206831456169"></a>  
![](../../../figures/shfl_up结果示意图.png "shfl_up结果示意图")

