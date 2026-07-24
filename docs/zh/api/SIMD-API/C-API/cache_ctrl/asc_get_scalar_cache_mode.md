# asc_get_scalar_cache_mode

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
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"c_api/cache_ctrl/cache_ctrl.h"`。

用于获取标量单元读取数据时的Scalar Cache模式。该接口通过读取CTRL寄存器的CTRL[19:16]比特位（READ模式）来获取当前标量单元读通道的缓存策略。

## 函数原型

```cpp
__aicore__ inline asc_load_l2_cache_mode asc_get_scalar_cache_mode()
```

## 参数说明

无

## 返回值说明

返回当前的Scalar Cache模式，类型为[asc_store_l2_cache_mode](../enum/asc_store_l2_cache_mode.md)。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
asc_load_l2_cache_mode mode = asc_get_scalar_cache_mode();
```
