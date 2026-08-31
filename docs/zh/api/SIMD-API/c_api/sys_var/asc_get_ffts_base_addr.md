# asc_get_ffts_base_addr

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

获取核间同步寄存器（FFTS基地址寄存器）的基地址。需要在Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步寄存器的基地址，并作为参数传入[asc_set_ffts_base_addr](asc_set_ffts_base_addr.md)后使用。本接口仅读取当前核上下文中已配置的基地址值。

## 函数原型

```c
__aicore__ inline int64_t asc_get_ffts_base_addr()
```

## 参数说明

本接口无参数。

## 返回值说明

返回`int64_t`类型的核间同步寄存器基地址。

## 流水类型

PIPE_S

## 约束说明

- 本接口为只读查询接口，读取的是核间同步寄存器的基地址，调用方不可直接设置或修改返回的基地址值；基地址需通过[asc_set_ffts_base_addr](asc_set_ffts_base_addr.md)预先配置后方可用于核间同步。

<font color="red">

- 本接口不支持纯SIMT编译模式，在该模式下包含`sys_var.h`头文件会触发编译错误。

</font>

## 调用示例

```cpp
// Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr
uint64_t config = *(__gm__ uint64_t*)ffts_addr;
asc_set_ffts_base_addr(config);
int64_t ffts_base = asc_get_ffts_base_addr();
```
