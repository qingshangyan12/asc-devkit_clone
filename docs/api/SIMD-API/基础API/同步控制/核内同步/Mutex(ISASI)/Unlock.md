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

```cpp
template <pipe_t pipe>
static __aicore__ inline void Unlock(MutexID id)
```

## 参数说明<a name="section74061251191017"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| pipe | 模板参数，表示流水类别。<br>支持的流水类型为PIPE_S/PIPE_M/PIPE_V/PIPE_MTE1/PIPE_MTE2/PIPE_MTE3/PIPE_FIX。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| id | 输入 | 进行流水同步管理的MutexID。<ul><li><a href=\"TPipe-TQue编程实践参考.md\">TPipe和TQue编程实践</a>中，MutexID需要通过<a href=\"zh-cn_topic_0000002534327182.md\">AllocMutexID/ReleaseMutexID</a>进行申请释放。</li><li><a href=\"静态Tensor编程实践参考.md\">静态Tensor编程实践</a>中，此时性能最优，MutexID由开发者自行管理，建议使用0-27，28-31为系统内部规划预留，不建议使用。</li></ul> |

## 约束说明<a name="section184751024101111"></a>

参考[Lock约束说明](Lock.md#section184751024101111)。

## 调用示例<a name="section123275308128"></a>

参考[Lock调用示例](Lock.md#section123275308128)。
