# madd

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

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

将`dst`与`src0`逐元素相乘，再与`src1`相加：

$$
result_i = dst_i \times src0_i + src1_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> madd(
    const reg_tensor<T>& dst,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `dst` | 输入 | 第一个乘数。`dst.mask`控制参与计算的元素。 |
| `src0` | 输入 | 第二个乘数。`src0.mask`不参与本次计算。 |
| `src1` | 输入 | 加数。`src1.mask`不参与本次计算。 |

## 数据类型

三个操作数与返回值的数据类型必须一致，支持half、bfloat16_t、float。

## 返回值说明

返回`reg_tensor<T>`，保存`dst * src0 + src1`的结果，返回值的`mask`与`dst.mask`相同。

## 约束说明

调用前必须初始化`dst.mask`。`dst.mask`为0的对应结果位置置零。

## 调用示例

```cpp
auto mask = asc::te::experimental::all_mask<float>();
auto dst = asc::te::experimental::load(input0).with_mask(mask);
auto src0 = asc::te::experimental::load(input1);
auto src1 = asc::te::experimental::load(input2);
auto result = asc::te::experimental::madd(dst, src0, src1);
asc::te::experimental::store(output, result);
```
