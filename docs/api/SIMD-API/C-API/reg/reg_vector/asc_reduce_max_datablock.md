# asc_reduce_max_datablock

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
<!-- npu="x90" id8 -->
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

找出DataBlock中的最大值，并将最终的计算结果依次保存在dst中的最低位。

## 函数原型

```cpp
__simd_callee__ inline void asc_reduce_max_datablock(vector_half& dst, vector_half src, vector_bool mask)
__simd_callee__ inline void asc_reduce_max_datablock(vector_float& dst, vector_float src, vector_bool mask)
__simd_callee__ inline void asc_reduce_max_datablock(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
__simd_callee__ inline void asc_reduce_max_datablock(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
__simd_callee__ inline void asc_reduce_max_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
__simd_callee__ inline void asc_reduce_max_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
```

## 参数说明

**表1**  参数说明


| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器），用于保存DataBlock内归约得到的最大值，结果位于最低位。 |
| `src` | 输入 | 源操作数（矢量数据寄存器），待归约的输入数据。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义](../reg数据类型定义.md)。


## 数据类型支持


| 输入A类型 | 输入B类型/标量类型 | 输出类型 |
|---|---|---|
| half | bool | half |
| float | bool | float |
| uint16_t | bool | uint16_t |
| int16_t | bool | int16_t |
| uint32_t | bool | uint32_t |
| int32_t | bool | int32_t |


## 返回值说明

无返回值。

## 约束说明

- max(-0, +0) = +0；如果DataBlock中存在nan，将该数据类型的nan写入dst对应位置。
- 异常和边界值处理：当某DataBlock内所有元素均不参与计算（mask全为0）时，将该数据类型的最小值写入dst对应位置；当存在多个最大值时，将第一个最大值保存在dst中。

## 理论性能


| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| int16_t | int16_t | 128 |
| uint16_t | uint16_t | 128 |
| half | half | 128 |
| int32_t | int32_t | 64 |
| uint32_t | uint32_t | 64 |
| float | float | 64 |


## 调用示例

```cpp
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_reduce_max_datablock(dst, src, mask);
```