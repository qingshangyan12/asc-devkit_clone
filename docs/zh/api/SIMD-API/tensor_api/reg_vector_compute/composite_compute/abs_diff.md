# abs_diff

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

逐元素计算两个输入之差的绝对值：

$$
dst_i = |src0_i - src1_i|
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> abs_diff(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0` | 输入 | 被减数，同时提供执行mask。 |
| `src1` | 输入 | 减数。 |

## 支持的数据类型

| 模板参数 | 支持的数据类型 |
| --- | --- |
| `T` | `half`、`float` |

## 返回值说明

返回`reg_tensor<T>`，有效元素保存差值的绝对值，结果mask等于`src0.mask`。

## 约束说明

- 两个输入寄存器的元素类型必须一致。
- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。

## 调用示例

```cpp
auto mask = asc::te::all_mask<float>();
auto src0 = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
auto src1 = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);
auto result = asc::te::experimental::abs_diff(src0, src1);
```
