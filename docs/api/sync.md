# sync<a name="ZH-CN_TOPIC_0000002568552812"></a>

## 功能说明<a name="section618mcpsimp"></a>

同步线程块内所有线程，所有线程都执行到改同步点位置才能继续执行。

## 函数原型<a name="section620mcpsimp"></a>

```
static void sync()
```

## 参数说明<a name="section0866173114710"></a>

无

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

必须保证线程块所有线程都能调用到此接口，否则会导致卡死。

## 调用示例<a name="section134121647154719"></a>

```
thread_block g = this_thread_block();
g.sync();
```

