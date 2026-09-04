# CeilDiv

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
- Atlas 推理系列产品 AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品 Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"utils/base/sys_math.h"`。

计算两个整数a和b相除后向上取整的结果。该接口可在Host侧和AI Core侧使用。

## 函数原型

```cpp
template <typename T, typename U>
__host__ __aicore__ inline constexpr auto CeilDiv(T a, U b);
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 含义 |
| --- | --- |
| T | 参数a的数据类型，必须为整型。 |
| U | 参数b的数据类型，必须为整型。 |

**表2** 接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| a | 输入 | 被除数。 |
| b | 输入 | 除数。 |

## 返回值说明

返回两个整型数相除的向上取整结果。一般情况下，返回值类型由`decltype(T{} / U{})`推导得到。

## 约束说明

- T和U必须为整型，支持的数据类型：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t。
- 当b为0时，结果为0。
- 对于有符号输入，调用方需确保a >= 0且b > 0。

## 调用示例

```cpp
constexpr uint32_t repeatTimes = AscendC::CeilDiv(1025U, 256U);
static_assert(repeatTimes == 5U);
```
