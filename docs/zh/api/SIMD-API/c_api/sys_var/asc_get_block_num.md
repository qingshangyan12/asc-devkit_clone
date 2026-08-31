# asc_get_block_num

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

获取当前kernel启动时配置的block（AI Core）数量，即任务下发时grid维度的规模，用于代码内部的多核逻辑控制等。该值由任务启动参数决定，运行期不可修改，常用于核间切分数据并行度的计算。本接口在AIC与AIV上均可调用，返回值含义一致。

## 函数原型

```c
__aicore__ inline int64_t asc_get_block_num()
```

## 参数说明

本接口无参数。

## 返回值说明

返回`int64_t`类型的当前kernel启动配置的block（AI Core）数量。返回值为正整数，具体规模由任务启动参数决定，同一kernel内多次调用返回值相同。

## 流水类型

PIPE_S

## 约束说明

- 该C API不触发硬件搬运或计算流水，返回值由kernel启动时配置决定，运行期不可修改，同一kernel内多次调用返回值相同。
- 本接口不支持纯SIMT编译模式，在该模式下包含对应头文件会触发编译错误。

## 调用示例

无
