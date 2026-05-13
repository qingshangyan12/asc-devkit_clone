# meta\_group\_rank<a name="ZH-CN_TOPIC_0000002599312027"></a>

## 功能说明<a name="section618mcpsimp"></a>

获取当前协作组在其父组的排名，coalesced\_group 的排名固定为0。

## 函数原型<a name="section620mcpsimp"></a>

```
unsigned long long meta_group_rank() const
```

## 参数说明<a name="section0866173114710"></a>

无

## 返回值说明<a name="section640mcpsimp"></a>

返回0。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section134121647154719"></a>

```
coalesced_group active = coalesced_threads();
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    unsigned long long thread_num = active.meta_group_rank(); // 返回0
}
```

