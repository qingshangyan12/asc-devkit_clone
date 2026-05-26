# UB-\>GM非对齐数据搬运\(DataCopyPad\)<a name="ZH-CN_TOPIC_0000002574022811"></a>

DataCopyPad能够实现将数据从Unified Buffer非对齐搬运至Global Memory。

src和dst分别为源操作数和目的操作数；dataCopyParams为搬运参数。

搬运的数据用于[矢量计算](Memory矢量计算.md#ZH-CN_TOPIC_0000002525764374)，接口具体介绍请参考：[UB-\>GM非对齐数据搬运\(DataCopyPad\)](UB--GM非对齐数据搬运(DataCopyPad)-11.md#ZH-CN_TOPIC_0000002574928191)

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```
template <typename T> 
__aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams) 
```

