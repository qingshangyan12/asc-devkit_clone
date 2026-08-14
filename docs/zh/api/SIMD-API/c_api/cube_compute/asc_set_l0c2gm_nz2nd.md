# asc_set_l0c2gm_nz2nd

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

本接口用于配置L0C Buffer搬出接口启用Nz格式转换为ND格式时所需的相关参数。

## 函数原型

```c
__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num,
                                            uint64_t src_nd_stride,
                                            uint64_t dst_nd_stride)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| nd_num | 输入 | ND矩阵的数量，取值范围为[1, 65535]。 |
| src_nd_stride | 输入 | 以分形大小为单位的源步长，源相邻Nz矩阵的偏移（头与头）。当nd_num配置为1时，src_nd_stride不生效，配置为0即可。取值范围为[0, 65535]，单位为C0_Size（16*sizeof(T)，T为L0C Buffer上数据的类型）。 |
| dst_nd_stride | 输入 | 目的相邻ND矩阵的偏移（头与头）。取值范围为[0, 65535]，单位为element。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 调用L0C Buffer搬出接口并且需要进行随路Nz格式转换为ND格式之前，必须先调用本接口配置格式转换参数。

## 调用示例

```cpp
uint64_t nd_num = 2;
uint64_t src_nd_stride = 2;
uint64_t dst_nd_stride = 1;
asc_set_l0c2gm_nz2nd(nd_num, src_nd_stride, dst_nd_stride);
```
