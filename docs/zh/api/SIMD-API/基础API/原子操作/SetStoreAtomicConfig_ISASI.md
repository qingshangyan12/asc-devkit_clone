# SetStoreAtomicConfig\(ISASI\)<a name="ZH-CN_TOPIC_0000001790206710"></a>

## 产品支持情况<a name="section1550532418810"></a>

<!-- npu="950" id8 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id8 -->
<!-- npu="A3" id9 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id9 -->
<!-- npu="910b" id10 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id10 -->
<!-- npu="310b" id11 -->
- Atlas 200I/500 A2 推理产品：支持
<!-- end id11 -->
<!-- npu="310p" id12 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id12 -->
<!-- npu="310p" id13 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id13 -->
<!-- npu="910" id14 -->
- Atlas 训练系列产品：不支持
<!-- end id14 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_common_intf.h"`。

设置原子操作开启位与原子操作类型。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <AtomicDtype type, AtomicOp op>
__aicore__ inline void SetStoreAtomicConfig();
```

## 参数说明<a name="section622mcpsimp"></a>

<a name="table622mcpsimp"></a>**表1**  模板参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| type | 输入 | 原子操作开启位，类型为AtomicDtype。支持ATOMIC_NONE（关闭原子操作）、ATOMIC_F32（float）、ATOMIC_F16（half）、ATOMIC_S16（int16_t）、ATOMIC_S32（int32_t）、ATOMIC_S8（int8_t）、ATOMIC_BF16（bfloat16_t）。 |
| op | 输入 | 原子操作类型，类型为AtomicOp。仅当type不为ATOMIC_NONE时有效，当前仅支持ATOMIC_SUM（求和操作）。 |

## 数据类型

支持的数据类型为int8_t、int16_t、half、bfloat16_t、int32_t、float。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section837496171220"></a>

```cpp
// 设置原子操作为求和操作，支持的数据类型为half
AscendC::SetStoreAtomicConfig<AscendC::AtomicDtype::ATOMIC_F16, AscendC::AtomicOp::ATOMIC_SUM>();
```
