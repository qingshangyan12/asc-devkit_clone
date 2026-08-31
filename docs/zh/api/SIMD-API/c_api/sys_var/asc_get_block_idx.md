# asc_get_block_idx

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

读取当前正在运行本kernel的block在所有启动block中的索引。block索引由运行时在kernel启动时为每个block分配，用于标识同一kernel的不同block实例，取值范围为[0, 总block数−1]，总block数由kernel启动配置决定。调用方可据此区分各block并计算各block负责处理的数据分片偏移。本接口可在AIC与AIV上调用，返回当前block的全局索引。

## 函数原型

```c
__aicore__ inline int64_t asc_get_block_idx()
```

## 参数说明

本接口无参数。

## 返回值说明

返回类型为`int64_t`，表示当前block的索引。该值为非负整数，取值范围为[0, 总block数−1]，总block数由kernel启动配置决定。同一kernel的不同block实例返回不同的索引值。

## 流水类型

PIPE_S

## 约束说明

- 本接口不触发硬件访存动作，仅读取运行时分配的block索引。
- block索引由运行时在kernel启动时分配，本接口仅读取当前block的索引值，调用方不可设置或修改该值。
- 返回值为非负整数，取值范围[0, 总block数−1]，总block数由kernel启动配置决定；调用方应按非负整数解析使用。
- 本接口可在AIC与AIV上调用，返回值反映当前执行本kernel的block的全局索引，与所在核类型无关。

## 调用示例

无
