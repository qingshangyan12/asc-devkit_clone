# group\_dim<a name="ZH-CN_TOPIC_0000002568712466"></a>

## 函数功能<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section36583473819"></a>

获取当前线程块的线程配置，与接口dim\_threads功能相同。

## 函数原型<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section13230182415108"></a>

```
static dim3 group_dim()
```

## 参数说明<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section75395119104"></a>

无

## 返回值说明<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section25791320141317"></a>

当前线程块的线程配置。

## 约束说明<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section19165124931511"></a>

无

## 调用示例<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section320753512363"></a>

```
thread_block g = this_thread_block();
dim3 block_dim = g.group_dim();
```

