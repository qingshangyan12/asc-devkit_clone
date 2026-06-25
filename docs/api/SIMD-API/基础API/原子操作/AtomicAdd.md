# AtomicAdd<a name="ZH-CN_TOPIC_0000002598784427"></a>

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明<a name="section163431416121118"></a>

头文件路径为：`"basic_api/kernel_operator_atomic_intf.h"`。

该接口用于在指定GM地址上进行原子加操作，将输入值原子加到address指向的GM地址上，返回该地址修改前的值。

计算公式如下：

$$result = Add(old\_value, input\_value)$$

## 函数原型<a name="section1171112415119"></a>

```cpp
template <typename T>
__aicore__ inline T AtomicAdd(__gm__ T *address, T value)
```

## 参数说明<a name="section086962912112"></a>

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| address | 输入 | 输入GM的地址。 |
| value | 输入 | 标量值，支持数据类型和address指向的数据类型一致。 |

## 数据类型<a name="section832119018128"></a>

address、value、返回值的数据类型相同，支持的数据类型为int32_t、uint32_t、float、int64_t、uint64_t。

## 返回值说明<a name="section97313820116"></a>

GM地址上做原子操作前的数据。

## 约束说明<a name="section24727811123"></a>

- 原子操作涉及标量计算，如果标量计算单元和搬运单元\(MTE2/MTE3\)在读写GM时存在数据依赖，开发者需要根据实际情况插入同步。
- Scalar原子操作会绕过DCache，需要调用[DataCacheCleanAndInvalid](../缓存控制/DataCacheCleanAndInvalid.md)接口确保GM与DCache的一致性。

## 调用示例<a name="section191505489122"></a>

```cpp
extern "C" __global__ __aicore__ void atomic_simple_kernel(__gm__ uint8_t* dst, uint32_t dataSize)
{
    // ...
    dst_global.SetGlobalBuffer(reinterpret_cast<__gm__ T *>(dst_gm), dataSize);
    LocalTensor<T> dstLocal = inQueueX.AllocTensor<T>();
    int32_t value = 1;
    int32_t a = AscendC::AtomicAdd(reinterpret_cast<__gm__ int32_t *>(dst), value);
    // 先执行完原子操作之后才能进行搬运操作，有数据依赖，需要手动插入MTE2等待Scalar的同步。
    event_t eventIdSToMte2 = static_cast<event_t>(GetTPipePtr()->AllocEventID<HardEvent::S_MTE2>());
    SetFlag<HardEvent::S_MTE2>(eventIdSToMte2);
    WaitFlag<HardEvent::S_MTE2>(eventIdSToMte2);
    DataCopy(dstLocal, dst_global, dataSize);
    // ...
}
```

假设上述函数在3个核上执行，核1、核2、核3依次调度，结果示例如下：

```text
原GM数据dst: [1,1,1,1,1,...,1]
核1：
原子计算后GM数据dst: [2,1,1,1,1,...,1]
返回值 a: 1
核2：
原子计算后GM数据dst: [3,1,1,1,1,...,1]
返回值 a: 2
核3：
原子计算后GM数据dst: [4,1,1,1,1,...,1]
返回值 a: 3
```
