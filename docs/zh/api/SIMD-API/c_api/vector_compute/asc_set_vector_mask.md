# asc_set_vector_mask

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

某些矢量计算接口需要提前设置Mask，提示哪些数据需要参与计算。
单独使用该API时可引入`c_api/vector_compute/compute/vector_mask_config.h`，整体使用C API时可引入`c_api/asc_simd.h`。
该API用于设置Mask，使用前需要先调用[asc_set_mask_count](asc_set_mask_count.md)和[asc_set_mask_norm](asc_set_mask_norm.md)设置Mask模式。在不同的模式下Mask的含义如下：
- Normal模式用于高维切分计算类API。该模式下，Mask参数用来控制单次迭代内参与计算的元素。可以按位控制哪些元素参与计算：bit位的值为1表示参与计算，0表示不参与。分为mask_high(高位Mask)和mask_low（低位Mask）。参数取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，mask_high，mask_low∈[0, 2^64-1]，并且不同时为0；当操作数为32位时，mask_high为0，mask_low∈(0, 2^64-1]；当操作数为64位时，mask_high为0，mask_low∈(0, 2^32-1]。
- Counter模式用于前n个数据计算类API。该模式下，Mask参数用于控制整个计算过程中参与计算的元素个数。前n个数据计算类接口中已经实现了Normal模式和Counter模式的转换，用户不需要进行自行设置。

## 函数原型

```cpp
__aicore__ inline void asc_set_vector_mask(uint64_t mask_high, uint64_t mask_low)
```

## 参数说明

**表1** 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|mask_high|输入|Normal模式：高位mask值。<br>Counter模式：需要置0，本入参不生效。|
|mask_low|输入|Normal模式：低位mask值。<br>Counter模式：整个矢量计算过程中，参与计算的元素个数。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

需先调用[asc_set_mask_count](asc_set_mask_count.md)或[asc_set_mask_norm](asc_set_mask_norm.md)设置Mask模式。

<!-- npu="950" id8 -->
- Ascend 950PR/Ascend 950DT：配置Reg矢量计算时，可通过[asc_get_mask_spr](../reg/reg_data_types/asc_get_mask_spr.md)接口中的`asc_get_mask_spr_b16`或`asc_get_mask_spr_b32`读入掩码寄存器。
<!-- end id8 -->

## 调用示例

请参考[asc_set_mask_count](asc_set_mask_count.md#调用示例)或[asc_set_mask_norm](asc_set_mask_norm.md#调用示例)。
