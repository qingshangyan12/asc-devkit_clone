# continuous_computeAPI<a name="ZH-CN_TOPIC_0000002491670826"></a>

continuous_computeAPI，支持Tensor前n个数据计算。针对源操作数的连续n个数据进行计算并连续写入目的操作数，解决一维tensor的continuous_compute问题。

```
Add(dst, src1, src2, n);
```

下图以矢量加法为例，展示了**continuous_computeAPI**的特点。

**图1** **continuous_computeAPI**<a name="zh-cn_topic_0000001762058545_fig6847134062319"></a>  
![](../../../../figures/continuous_computeAPI.png "continuous_computeAPI")
