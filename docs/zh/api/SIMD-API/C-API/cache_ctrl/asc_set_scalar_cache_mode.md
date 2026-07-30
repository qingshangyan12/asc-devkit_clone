# asc_set_scalar_cache_mode

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

用于设置标量单元的Scalar Cache模式。通过参数类型区分读通道和写通道：
- 传入`asc_load_l2_cache_mode`类型参数：设置读通道缓存模式，对应CTRL寄存器CTRL[19:16]比特位。
- 传入`asc_store_l2_cache_mode`类型参数：设置写通道缓存模式，对应CTRL寄存器CTRL[23:20]比特位。

## 函数原型

```cpp
// 设置读通道Scalar Cache模式（CTRL[19:16]）
__aicore__ inline void asc_set_scalar_cache_mode(asc_load_l2_cache_mode cache_mode)

// 设置写通道Scalar Cache模式（CTRL[23:20]）
__aicore__ inline void asc_set_scalar_cache_mode(asc_store_l2_cache_mode cache_mode)
```

## 参数说明

表1 读通道参数说明（asc_load_l2_cache_mode）

|参数名|输入/输出|描述|
| :------ | :---  | :------------ |
|cache_mode   |输入   |读通道缓存管理策略。取值参考[asc_load_l2_cache_mode](../enum/asc_load_l2_cache_mode.md)。默认值为`asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM`。注意：`NOTALLOC_CLEAN`和`NOTALLOC_DROP`不被支持，传入这两个值时接口不执行任何操作。|

表2 写通道参数说明（asc_store_l2_cache_mode）

|参数名|输入/输出|描述|
| :------ | :---  | :------------ |
|cache_mode   |输入   |写通道缓存管理策略。取值参考[asc_store_l2_cache_mode](../enum/asc_store_l2_cache_mode.md)。默认值为`asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM`。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 读通道（`asc_load_l2_cache_mode`）仅支持`NORMAL_FIRST_VICTIM`、`NORMAL_LAST_VICTIM`、`NORMAL_PERSISTENT`、`NOTALLOC_KEEP`四个枚举值，`NOTALLOC_CLEAN`和`NOTALLOC_DROP`不被支持。
- 写通道（`asc_store_l2_cache_mode`）的全部4个枚举值均受支持。

## 调用示例

```cpp
// 设置读通道Scalar Cache为驻留模式
asc_set_scalar_cache_mode(asc_load_l2_cache_mode::NORMAL_PERSISTENT);

// 设置写通道Scalar Cache为不分配Cache Line
asc_set_scalar_cache_mode(asc_store_l2_cache_mode::NOTALLOC_CLEAN);
```
