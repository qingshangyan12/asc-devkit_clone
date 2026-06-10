# FetchEventID

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

根据HardEvent（硬件类型同步事件）获取对应的可用TEventID。该接口不会占用TEventID，适用于临时使用事件ID的场景。该接口为AscendC命名空间下的全局接口，独立于TPipe和TQue对象使用。

## 函数原型

```cpp
template <HardEvent evt>
__aicore__ inline TEventID FetchEventID()
```

## 参数说明

**表 1** 模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| evt | HardEvent硬件同步类型。该类型的具体说明请参考[SetFlag/WaitFlag(ISASI)](SetFlag-WaitFlag(ISASI).md)中同步类型的说明。 |

## 返回值说明

TEventID

## 约束说明

相比[AllocEventID](AllocEventID.md)，FetchEventID仅获取当前可用的TEventID，不会占用该ID。复杂场景下需由开发者自行保证ID使用正确。例如相同流水连续调用SetFlag/WaitFlag时，如果两次传入的ID都通过FetchEventID获取，可能因ID相同导致程序卡死等未定义行为，此时推荐使用AllocEventID。

## 调用示例

```cpp
AscendC::TEventID eventID = AscendC::FetchEventID<AscendC::HardEvent::V_S>();
AscendC::SetFlag<AscendC::HardEvent::V_S>(eventID);
AscendC::WaitFlag<AscendC::HardEvent::V_S>(eventID);
```
