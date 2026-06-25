# Unlock<a name="ZH-CN_TOPIC_0000002406681009"></a>

## 产品支持情况<a name="section73648168211"></a>

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明<a name="section54681522111017"></a>

头文件路径为： "basic\_api/kernel\_common.h"

直到前置当前流水指令退出后，根据MutexID释放对应Mutex。

## 函数原型<a name="section1568410468104"></a>

```
template <pipe_t pipe>
static __aicore__ inline void Unlock(MutexID id)
```

## 参数说明<a name="section74061251191017"></a>

**表 1**  模板参数说明

<a name="table1965616488585"></a>
<table><thead align="left"><tr id="row865644816588"><th class="cellrowborder" valign="top" width="18.29%" id="mcps1.2.3.1.1"><p id="p765612483583"><a name="p765612483583"></a><a name="p765612483583"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="81.71000000000001%" id="mcps1.2.3.1.2"><p id="p1657204895812"><a name="p1657204895812"></a><a name="p1657204895812"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row865704865817"><td class="cellrowborder" valign="top" width="18.29%" headers="mcps1.2.3.1.1 "><p id="p8657048165815"><a name="p8657048165815"></a><a name="p8657048165815"></a>pipe</p>
</td>
<td class="cellrowborder" valign="top" width="81.71000000000001%" headers="mcps1.2.3.1.2 "><p id="p1465784819589"><a name="p1465784819589"></a><a name="p1465784819589"></a>模板参数，表示流水类别。</p>
<p id="p12962145013811"><a name="p12962145013811"></a><a name="p12962145013811"></a>支持的流水类型为PIPE_S/PIPE_M/PIPE_V/PIPE_MTE1/PIPE_MTE2/PIPE_MTE3/PIPE_FIX。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="table103102222579"></a>
<table><thead align="left"><tr id="row631032216571"><th class="cellrowborder" valign="top" width="18.54%" id="mcps1.2.4.1.1"><p id="p231082295719"><a name="p231082295719"></a><a name="p231082295719"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="10.05%" id="mcps1.2.4.1.2"><p id="p2031014221576"><a name="p2031014221576"></a><a name="p2031014221576"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="71.41%" id="mcps1.2.4.1.3"><p id="p103112022105713"><a name="p103112022105713"></a><a name="p103112022105713"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row83111722195716"><td class="cellrowborder" valign="top" width="18.54%" headers="mcps1.2.4.1.1 "><p id="p131119221575"><a name="p131119221575"></a><a name="p131119221575"></a>id</p>
</td>
<td class="cellrowborder" valign="top" width="10.05%" headers="mcps1.2.4.1.2 "><p id="p63110229572"><a name="p63110229572"></a><a name="p63110229572"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.41%" headers="mcps1.2.4.1.3 "><p id="p9311822145713"><a name="p9311822145713"></a><a name="p9311822145713"></a>进行流水同步管理的MutexID。</p>
<a name="ul097173018484"></a><a name="ul097173018484"></a><ul id="ul097173018484"><li><a href="TPipe-TQue编程实践参考.md">TPipe和TQue编程实践</a>中，MutexID需要通过<span id="ph0971103024815"><a name="ph0971103024815"></a><a name="ph0971103024815"></a><a href="zh-cn_topic_0000002534327182.md">AllocMutexID/ReleaseMutexID</a></span>进行申请释放。</li><li><a href="静态Tensor编程实践参考.md">静态Tensor编程实践</a>中，此时性能最优，MutexID由开发者自行管理，建议使用0-27，28-31为系统内部规划预留，不建议使用。</li></ul>
</td>
</tr>
</tbody>
</table>

## API映射关系<a name="section1870615163113"></a>

与built-in接口映射关系：

Unlock接口是在built-in接口（rls\_buf）的基础上进行抽象封装实现的，其对应的底层built-in接口为:

```
void rls_buf(pipe_t pipe, uint64_t buf_id, bool mode)
```

## 约束说明<a name="section184751024101111"></a>

参考[Lock约束说明](Lock.md#section184751024101111)。

## 调用示例<a name="section123275308128"></a>

参考[Lock调用示例](Lock.md#section123275308128)。
