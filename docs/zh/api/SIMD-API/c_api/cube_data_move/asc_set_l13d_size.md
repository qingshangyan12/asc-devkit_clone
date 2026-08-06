# asc_set_l13d_size

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

设置3D格式分形矩阵搬运（img2col展开）的L1 Buffer边界值，作为后续3D格式分形矩阵搬运接口在L1 Buffer侧的容量上限。配置写入后作用于后续调用的3D格式分形矩阵搬运接口，约束Feature Map（由`fm_w × fm_h × channel_size`描述）占用的字节数不可超过本接口设置的边界值。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

3D格式分形矩阵搬运场景下，L1 Buffer侧容量上限需通过本接口配置；Feature Map属性需通过[asc_set_l13d_fmatrix](asc_set_l13d_fmatrix.md)、[asc_set_l13d_fmatrix_b](asc_set_l13d_fmatrix_b.md)配置，填充值需通过[asc_set_l13d_padding](asc_set_l13d_padding.md)配置，repeat控制参数需通过[asc_set_l13d_rpt](asc_set_l13d_rpt.md)配置，各setter需配合使用后再调用3D格式分形矩阵搬运接口执行搬运。

## 函数原型

```cpp
__aicore__ inline void asc_set_l13d_size(uint64_t value)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `value` | 输入 | L1 Buffer边界值，作为3D格式分形矩阵搬运接口在L1 Buffer侧的容量上限，约束Feature Map字节数`fm_w × fm_h × channel_size × sizeof(dtype)`不超过该值。取值范围：[0, 2^64−1]。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需配合[asc_copy_l12l0a](asc_copy_l12l0a/asc_copy_l12l0a.md)/[asc_copy_l12l0b](asc_copy_l12l0b/asc_copy_l12l0b.md)的3D格式搬运接口使用。
- 如果使用asc_set_l13d_size接口设置了边界值，[asc_copy_l12l0a](asc_copy_l12l0a/asc_copy_l12l0a.md)/[asc_copy_l12l0b](asc_copy_l12l0b/asc_copy_l12l0b.md)的3D格式搬运接口的L1 Buffer初始地址要在设置的边界内（value）。
- value为0表示不设置边界，可使用整个L1 Buffer。
- 配合3D格式搬运接口使用时，非0边界值不能小于1024；设置为1~1023时，行为未定义。
- 操作数地址对齐约束请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。

### value参数约束

- value取值范围为[0, 2^32−1]，超出取值范围的值会被截断，导致后续搬运结果不符合预期。


## 调用示例

```cpp
uint64_t value = 1024;
asc_set_l13d_size(value);    // 设置L1 Buffer的边界值为1024（单位：字节）
```
