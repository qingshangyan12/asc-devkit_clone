# meta\_group\_size<a name="ZH-CN_TOPIC_0000002568712476"></a>

## 功能说明<a name="section618mcpsimp"></a>

获取父组大小，coalesced\_group 的父组大小固定为1。

## 函数原型<a name="section620mcpsimp"></a>

```
unsigned long long meta_group_size() const
```

## 参数说明<a name="section0866173114710"></a>

无

## 返回值说明<a name="section640mcpsimp"></a>

返回1。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section134121647154719"></a>

```
coalesced_group active = coalesced_threads();
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    unsigned long long thread_num = active.meta_group_size(); // 返回1
}
```

