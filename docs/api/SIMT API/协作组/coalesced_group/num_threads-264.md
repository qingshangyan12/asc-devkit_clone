# num\_threads<a name="ZH-CN_TOPIC_0000002599312025"></a>

## 功能说明<a name="section618mcpsimp"></a>

获取组内线程总数。

## 函数原型<a name="section620mcpsimp"></a>

```
unsigned long long num_threads() const
```

## 参数说明<a name="section0866173114710"></a>

无

## 返回值说明<a name="section640mcpsimp"></a>

组内线程总数。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section134121647154719"></a>

```
coalesced_group active = coalesced_threads();
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    unsigned long long thread_num = active.num_threads(); // 返回16，一个warp中共有16个偶数id的线程
}
```

