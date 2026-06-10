# AllocEventID

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :---: |
| Ascend 950PR/Ascend 950DT | √ |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ |
| Atlas 200I/500 A2 推理产品 | x |
| Atlas 推理系列产品AI Core | √ |
| Atlas 推理系列产品Vector Core | x |
| Atlas 训练系列产品 | √ |
| Kirin X90 | √ |
| Kirin 9030 | √ |

## 功能说明

用于申请HardEvent（硬件类型同步事件）的TEventID。调用该接口后，会占用申请到的TEventID，直至调用[ReleaseEventID](ReleaseEventID.md)释放。该接口为AscendC命名空间下的全局接口，独立于TPipe和TQue对象使用。

## 函数原型

```cpp
template <HardEvent evt>
__aicore__ inline TEventID AllocEventID()
```

## 参数说明

**表 1** 模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| evt | HardEvent硬件同步类型。该类型的具体说明请参考[SetFlag/WaitFlag(ISASI)](SetFlag-WaitFlag(ISASI).md)中同步类型的说明。 |

## 返回值说明

TEventID

## 约束说明

- TEventID有数量限制，使用结束后应及时调用ReleaseEventID释放，防止TEventID耗尽。
- AllocEventID和ReleaseEventID需成对使用，ReleaseEventID传入的TEventID需由相同HardEvent类型的AllocEventID申请获得。

## 调用示例

```cpp
AscendC::TEventID eventID = AscendC::AllocEventID<AscendC::HardEvent::V_S>();
AscendC::SetFlag<AscendC::HardEvent::V_S>(eventID);
// 需要等待PIPE_V流水完成后再执行的PIPE_S流水操作。
AscendC::WaitFlag<AscendC::HardEvent::V_S>(eventID);
AscendC::ReleaseEventID<AscendC::HardEvent::V_S>(eventID);
```
