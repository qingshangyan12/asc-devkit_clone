# SetMMColumnMajor/SetMMRowMajor<a name="ZH-CN_TOPIC_0000002538231190"></a>

## 产品支持情况<a name="zh-cn_topic_0000002547203267_section1550532418810"></a>

<a name="zh-cn_topic_0000002547203267_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002547203267_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002547203267_p1883113061818"><a name="zh-cn_topic_0000002547203267_p1883113061818"></a><a name="zh-cn_topic_0000002547203267_p1883113061818"></a><span id="zh-cn_topic_0000002547203267_ph20833205312295"><a name="zh-cn_topic_0000002547203267_ph20833205312295"></a><a name="zh-cn_topic_0000002547203267_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002547203267_p783113012187"><a name="zh-cn_topic_0000002547203267_p783113012187"></a><a name="zh-cn_topic_0000002547203267_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002547203267_row296617817486"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002547203267_p17301775812"><a name="zh-cn_topic_0000002547203267_p17301775812"></a><a name="zh-cn_topic_0000002547203267_p17301775812"></a><span id="zh-cn_topic_0000002547203267_ph2272194216543"><a name="zh-cn_topic_0000002547203267_ph2272194216543"></a><a name="zh-cn_topic_0000002547203267_ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002547203267_p37256491200"><a name="zh-cn_topic_0000002547203267_p37256491200"></a><a name="zh-cn_topic_0000002547203267_p37256491200"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 函数原型<a name="zh-cn_topic_0000002547203267_section620mcpsimp"></a>

控制Mmad 优先通过M/N的哪个方向。

```
__aicore__ inline void SetMMColumnMajor() // 代表CUBE将首先通过M方向，然后通过N方向生成结果
__aicore__ inline void SetMMRowMajor()   // 代表CUBE将首先通过N方向，然后通过M方向产生结果
```

## 参数说明<a name="zh-cn_topic_0000002547203267_section622mcpsimp"></a>

无

## 返回值说明<a name="zh-cn_topic_0000002547203267_section640mcpsimp"></a>

无

## API映射关系<a name="zh-cn_topic_0000002547203267_section633mcpsimp"></a>

SetMMColumnMajor/SetMMRowMajor 接口是在built-in接口（set\_ctrl）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
void set_ctrl(uint64_t config);
```

## 约束说明<a name="zh-cn_topic_0000002547203267_section16716132384114"></a>

无

## 调用示例<a name="zh-cn_topic_0000002547203267_section837496171220"></a>

```
AscendC::SetMMRowMajor();
```

