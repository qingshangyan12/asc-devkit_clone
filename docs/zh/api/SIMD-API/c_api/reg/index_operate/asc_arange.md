# asc_arange

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

以传入的`value`为起始值，生成递增/递减的索引，并将生成的索引保存在`dst`中。算法逻辑表示如下：
```cpp
// 递增
{value, value + 1, value + 2, ... value + VL_T - 2, value + VL_T - 1}
// 递减
{value + VL_T - 1, value + VL_T - 2, value + VL_T - 3, ... value + 1, value}
```

以int16_t数据类型，起始值`value=10`为例：
递增索引为{10, 11, 12, 13, ... 135, 136, 137}，递减索引为{137, 136, 135, 134, ... 12, 11, 10}。

## 函数原型

### 递增模式

#### 模板原型（占位符形式）
```c
__simd_callee__ inline void asc_arange(vector_<dtype>& dst,
                                       <dtype> value)
```

dtype支持的数据类型为`int8_t`、`int16_t`、`half`、`int32_t`、`float`

#### 典型示例

```c
// 示例：以 half 标量为基值生成递增序列
__simd_callee__ inline void asc_arange(vector_half& dst,
                                       half value)
```

### 递减模式

#### 模板原型（占位符形式）
```c
__simd_callee__ inline void asc_arange_descend(vector_<dtype>& dst,
                                               <dtype> value)
```

dtype支持的数据类型为`int8_t`、`int16_t`、`half`、`int32_t`、`float`

#### 典型示例

```c
// 示例：以 half 标量为基值生成递减序列
__simd_callee__ inline void asc_arange_descend(vector_half& dst,
                                               half value)
```

## 参数说明

**表1** 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数（矢量数据寄存器）。 |
| value     | 输入    | 源操作数（标量），dtype须与dst一致。作为递增序列的起点，序列第0个元素等于value，后续元素按1递增/递减。取值范围为该dtype的可表示范围。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- dst与value的dtype必须完全一致（int8_t、int16_t、half、int32_t、float之一），否则触发异常。
- 本接口无掩码寄存器参数，目的矢量寄存器的全部元素均参与计算并写入结果，不存在清零或保留原值的位置。
- 整型dtype（int8_t、int16_t、int32_t）结果在超出该dtype可表示范围时回绕（wrap-around），不触发异常。例如int8_t取value=127时，序列前128个元素依次为127、−128、−127、…、0；value=−128时，序列前128个元素依次为−128、−127、…、−1、0。

## 调用示例

```cpp
__simd_vf__ inline void arange_vf(__ubuf__ int8_t* dst_addr, int8_t value, uint32_t count, int32_t one_repeat_size, uint16_t repeat_time)
{
    vector_int8_t dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b8(count);
        asc_arange(dst, value);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
