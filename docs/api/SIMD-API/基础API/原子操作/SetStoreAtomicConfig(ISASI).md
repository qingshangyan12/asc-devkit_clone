# SetStoreAtomicConfig\(ISASI\)<a name="ZH-CN_TOPIC_0000001790206710"></a>

## 产品支持情况<a name="section1550532418810"></a>

**表 1**  产品支持情况
| 产品 | 是否支持 |
|------|----------|
| <cann-filter npu_type = "950"><term>Ascend 950PR/Ascend 950DT<term> | √</cann-filter> |
| <cann-filter npu_type = "A3"><term>Atlas A3 训练系列产品/Atlas A3 推理系列产品<term> | √</cann-filter> |
| <cann-filter npu_type = "910b"><term>Atlas A2 训练系列产品/Atlas A2 推理系列产品<term> | √</cann-filter> |
| <cann-filter npu_type = "310b"><term>Atlas 200I/500 A2 推理产品<term> | √</cann-filter> |
| <cann-filter npu_type = "310p"><term>Atlas 推理系列产品 AI Core<term> | x</cann-filter> |
| <cann-filter npu_type = "310p"><term>Atlas 推理系列产品 Vector Core<term> | x</cann-filter> |
| <cann-filter npu_type = "910"><term>Atlas 训练系列产品<term> | x</cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为： "basic_api/kernel_operator_common_intf.h"。

SetStoreAtomicConfig接口，功能是：设置原子操作使能位与原子操作类型。

## 函数原型<a name="section620mcpsimp"></a>

```
template <AtomicDtype type, AtomicOp op>
__aicore__ inline void SetStoreAtomicConfig()
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| type | 输入 | 原子操作使能位，AtomicDtype枚举类的定义如下：<br><pre>enum class AtomicDtype {<br>    ATOMIC_NONE = 0,  // 无原子操作<br>    ATOMIC_F32,       // 使能原子操作，进行原子操作的数据类型为float<br>    ATOMIC_F16,       // 使能原子操作，进行原子操作的数据类型为half<br>    ATOMIC_S16,       // 使能原子操作，进行原子操作的数据类型为int16_t<br>    ATOMIC_S32,       // 使能原子操作，进行原子操作的数据类型为int32_t<br>    ATOMIC_S8,        // 使能原子操作，进行原子操作的数据类型为int8_t<br>    ATOMIC_BF16       // 使能原子操作，进行原子操作的数据类型为bfloat16_t<br>};</pre> |
| op | 输入 | 原子操作类型，仅当使能原子操作时有效（即"type"为非"ATOMIC_NONE"的场景），当前仅支持求和操作。<br><pre>enum class AtomicOp {<br>    ATOMIC_SUM = 0   // 求和操作<br>};</pre> |

## 数据类型

支持的数据类型为int8_t/int16_t/half/bfloat16_t/int32_t/float。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 流水类型

PIPE_S

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section837496171220"></a>

```
// 设置原子操作为求和操作，支持的数据类型为half
AscendC::SetStoreAtomicConfig<AscendC::AtomicDtype::ATOMIC_F16, AscendC::AtomicOp::ATOMIC_SUM>();
```
