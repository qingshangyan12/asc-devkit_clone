# thread\_rank<a name="ZH-CN_TOPIC_0000002568552824"></a>

## 功能说明<a name="section618mcpsimp"></a>

获取当前线程在组内的排名。

## 函数原型<a name="section620mcpsimp"></a>

```
unsigned long long thread_rank() const
```

## 参数说明<a name="section0866173114710"></a>

无

## 返回值说明<a name="section640mcpsimp"></a>

当前线程在组内的排名。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section134121647154719"></a>

```
coalesced_group active = coalesced_threads();
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    unsigned long long rank = active.thread_rank();
}
```

示例代码中的条件分支将一个warp中所有线程id是偶数的线程组成coalesced\_group协作组，组内各线程thread\_rank接口返回结果如下图所示。

**图 1**  coalesced\_group\_rank<a name="fig788024554320"></a>  
![](figures/coalesced_group_rank.png "coalesced_group_rank")

