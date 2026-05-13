# sync<a name="ZH-CN_TOPIC_0000002568712474"></a>

## 功能说明<a name="section618mcpsimp"></a>

同步组内所有线程。

## 函数原型<a name="section620mcpsimp"></a>

```
void sync() const
```

## 参数说明<a name="section0866173114710"></a>

无

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section134121647154719"></a>

```
coalesced_group active = coalesced_threads();
if (threadIdx.x % 2 == 0) {
    coalesced_group active = coalesced_threads();
    active.sync();
}
```

