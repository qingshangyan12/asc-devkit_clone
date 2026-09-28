# mula

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/composite_compute.h"`。

将`src0`与`src1`逐元素相乘，并累加到`dst`：

$$
result_i = src0_i \times src1_i + dst_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> mula(
    reg_tensor<T>& dst,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1)
```

## 参数说明

**表1**模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表2**参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输入/输出 | 累加初值和结果。接口直接更新`dst.reg`，`dst.mask`控制参与计算的元素。 |
| src0 | 输入 | 第一个乘数。`src0.mask`不参与本次计算。 |
| src1 | 输入 | 第二个乘数。`src1.mask`不参与本次计算。 |

## 数据类型

三个操作数与返回值的数据类型必须一致，支持int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。

## 返回值说明

返回`reg_tensor<T>`，保存`src0 * src1 + dst`的结果，返回值的`mask`与`dst.mask`相同。

## 约束说明

- `dst.mask`需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- `dst.mask`对应位置为`0`时，返回值`reg`的对应位置置零。

## 调用示例

```cpp
auto mask = asc::te::experimental::all_mask<int32_t>();
auto dst = asc::te::experimental::load(input0).with_mask(mask);
auto src0 = asc::te::experimental::load(input1);
auto src1 = asc::te::experimental::load(input2);
auto result = asc::te::experimental::mula(dst, src0, src1);
asc::te::experimental::store(output, result);
```
