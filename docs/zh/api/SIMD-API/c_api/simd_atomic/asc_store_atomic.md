# asc_store_atomic

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

## 功能说明

根据[asc_set_store_atomic_config_v1](./asc_set_store_atomic_config_v1.md)设置的原子操作配置，将输入标量与指定GM地址中的数据进行原子运算，并将结果写回该GM地址。

## 函数原型

```cpp
__aicore__ inline void asc_store_atomic(__gm__ <dtype>* addr, <dtype> value)
```

### dtype支持数据类型

`dtype`取值为：`float`、`half`、`int16_t`、`int32_t`、`int8_t`、`bfloat16_t`。

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| addr | 输入/输出 | 原子操作的目标GM地址。 |
| value | 输入 | 参与原子操作的输入标量。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 调用前必须通过`asc_set_store_atomic_config_v1`设置原子操作配置，且`type`对应的数据类型必须与`addr`和`value`的数据类型一致。
- 当前仅支持`op`为0，即求和操作。
- `addr`起始地址须按`dtype`的字节数对齐，并且须落在GM可访问地址空间内。
- 该接口执行前不会对GM中的数据清零，开发者需根据需要初始化目标数据。
- 使用完成后，建议通过[asc_set_atomic_none](./asc_set_atomic_none.md)清空原子操作状态，以免影响后续相关指令。

## 调用示例

```cpp
__gm__ float* dst = ...;
float value = 1.0f;

asc_set_store_atomic_config_v1(1, 0);
asc_store_atomic(dst, value);
asc_set_atomic_none();
```
