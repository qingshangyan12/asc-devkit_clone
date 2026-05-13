# num\_threads<a name="ZH-CN_TOPIC_0000002599312017"></a>

## 函数功能<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section36583473819"></a>

获取组内线程总数。

## 函数原型<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section13230182415108"></a>

```
static unsigned int num_threads()
```

## 参数说明<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section75395119104"></a>

无

## 返回值说明<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section25791320141317"></a>

组内线程总数。

## 约束说明<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section19165124931511"></a>

无

## 调用示例<a name="zh-cn_topic_0000001586674626_zh-cn_topic_0000001340362562_section320753512363"></a>

```
thread_block g = this_thread_block();
unsigned int thread_num = g.num_threads();
```

