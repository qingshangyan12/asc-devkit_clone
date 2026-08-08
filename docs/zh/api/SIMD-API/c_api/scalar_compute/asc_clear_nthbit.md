# asc_clear_nthbit

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
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

对一个64位无符号整数执行位清除操作，将`bits`中第`idx`位清零，其余位保持不变。返回修改后的64位无符号整数。

本接口为标量位运算，运行在标量流水上，AIC与AIV均可调用，行为一致。

## 函数原型

```c
__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits,
                                            int64_t idx)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `bits` | 输入 | 64位无符号整数。取值范围为[0，2^64−1]。 |
| `idx` | 输入 | 位索引。实际位索引按`(idx % 64 + 64) % 64`计算，范围为[0，63]。 |

## 返回值说明

返回位清零后的结果（uint64_t类型）。

## 流水类型

PIPE_S

## 约束说明

当`idx`大于63或小于0时，实际清除的位索引按`(idx % 64 + 64) % 64`计算。

## 调用示例

```cpp
uint64_t bits = 0x7;
int64_t idx = 0x2;
uint64_t res = asc_clear_nthbit(bits, idx);
```
