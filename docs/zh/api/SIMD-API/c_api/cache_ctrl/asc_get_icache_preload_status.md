# asc_get_icache_preload_status

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

获取ICache的Preload的状态。

该接口为调试接口，在[asc_icache_preload](./asc_icache_preload.md)后调用，用于获取ICache的PreLoad的状态。当返回值为0时，说明ICache的PreLoad已完成；当返回值为1时，说明ICache的PreLoad未完成。

<font color="red">

头文件路径：`"c_api/cache_ctrl/cache_ctrl.h"`。

调试接口，获取ICache预取状态。返回值为只读状态字段：返回`0`表示ICache预取空闲（上一次预取已完成），返回`1`表示ICache预取正在进行中（尚未完成）。

发起ICache预取（`asc_icache_preload`）后需通过本接口轮询预取是否完成，便于调试与性能分析时确认预取阶段是否已结束。

</font>

## 函数原型

```cpp
__aicore__ inline int64_t asc_get_icache_preload_status()
```

## 参数说明

无

## 返回值说明

int64_t类型，0表示ICache的PreLoad已完成（空闲），1表示ICache的PreLoad未完成（忙）。

<font color="red">

返回`int64_t`类型的ICache预取状态值，仅有效位取值如下：

- 返回`0`：ICache预取空闲，上一次预取已完成。
- 返回`1`：ICache预取正在进行中，尚未完成。

</font>

## 流水类型

PIPE_S

## 约束说明

无

<font color="red">

### 通用约束

- 本接口仅用于调试与性能分析场景下的状态查询，不影响预取本身的执行流程。

</font>

## 调用示例

```cpp
int64_t status = asc_get_icache_preload_status();
```
