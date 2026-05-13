# all<a name="ZH-CN_TOPIC_0000002599192059"></a>

## 功能说明<a name="section618mcpsimp"></a>

判断是否所有组内线程的输入均不为0。

## 函数原型<a name="section620mcpsimp"></a>

```
int all(int predicate) const
```

## 参数说明<a name="section0866173114710"></a>

**表 1**  参数说明

<a name="table1051842613457"></a>
<table><thead align="left"><tr id="row125181826114515"><th class="cellrowborder" valign="top" width="11.91%" id="mcps1.2.4.1.1"><p id="p251872684515"><a name="p251872684515"></a><a name="p251872684515"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="11.04%" id="mcps1.2.4.1.2"><p id="p12518172614451"><a name="p12518172614451"></a><a name="p12518172614451"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.05%" id="mcps1.2.4.1.3"><p id="p1651812664519"><a name="p1651812664519"></a><a name="p1651812664519"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1551814266452"><td class="cellrowborder" valign="top" width="11.91%" headers="mcps1.2.4.1.1 "><p id="p11467118193716"><a name="p11467118193716"></a><a name="p11467118193716"></a>predicate</p>
</td>
<td class="cellrowborder" valign="top" width="11.04%" headers="mcps1.2.4.1.2 "><p id="p74679185371"><a name="p74679185371"></a><a name="p74679185371"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.05%" headers="mcps1.2.4.1.3 "><p id="p1646721873712"><a name="p1646721873712"></a><a name="p1646721873712"></a>操作数。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

当组内所有线程的输入均不为0，返回1，否则返回0。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section134121647154719"></a>

```
coalesced_group active = coalesced_threads();
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    uint32_t result = active.all(1); // 返回1
}
```

