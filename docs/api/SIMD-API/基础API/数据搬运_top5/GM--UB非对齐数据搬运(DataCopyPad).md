# GM-\>UB非对齐数据搬运\(DataCopyPad\)<a name="ZH-CN_TOPIC_0000002543262912"></a>

DataCopyPad能够实现将数据从Global Memory非对齐搬运至Unified Buffer，可以根据开发者的需要自行填充数据。

src和dst分别为源操作数和目的操作数；dataCopyParams为搬运参数；padParams为数据填充参数。

搬运的数据用于[矢量计算](Memory矢量计算.md#ZH-CN_TOPIC_0000002525764374)，接口具体介绍请参考：[GM-\>UB非对齐数据搬运\(DataCopyPad\)](GM--UB非对齐数据搬运(DataCopyPad)-10.md#ZH-CN_TOPIC_0000002544407954)

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```
template <typename T> 
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams) 
```

