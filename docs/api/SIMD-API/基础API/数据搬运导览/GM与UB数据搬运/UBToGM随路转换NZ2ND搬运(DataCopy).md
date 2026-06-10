# UB->GM随路转换NZ2ND搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002391805265"></a>

该接口为软仿接口，从易用性角度出发进行设计，支持在从Unified Buffer到Global Memory的数据搬运过程中进行NZ到ND格式的转换。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- Unified Buffer -> Global Memory
    - VECOUT -> GM
    - <cann-filter npu-type="310p">CO2 -> GM（仅Atlas 推理系列产品AI Core支持）</cann-filter>

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[UB->GM随路转换NZ2ND搬运(DataCopy)](../../Memory矢量计算/数据搬运/GM与UB数据搬运/UBToGM随路转换NZ2ND搬运(DataCopy).md)。

src和dst分别为源操作数和目的操作数；intriParams为Nz2NdParamsFull类型的搬运参数，配置NZ到ND格式转换信息。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const Nz2NdParamsFull& intriParams)
```
