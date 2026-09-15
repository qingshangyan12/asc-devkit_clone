# 比较运算符

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
- Atlas 推理系列产品 AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品 Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

比较运算符对源操作数执行逐元素比较，并返回比较结果。支持reg_tensor与reg_tensor、reg_tensor与标量、标量与reg_tensor三种操作数组合。

| 运算符 | 计算规则 |
| --- | --- |
| `operator==` | `dst_i = (src0_i == src1_i)` |
| `operator!=` | `dst_i = (src0_i != src1_i)` |
| `operator<` | `dst_i = (src0_i < src1_i)` |
| `operator<=` | `dst_i = (src0_i <= src1_i)` |
| `operator>` | `dst_i = (src0_i > src1_i)` |
| `operator>=` | `dst_i = (src0_i >= src1_i)` |

## 函数原型

<a id="operator-eq"></a>
### operator==

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator==(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator==(
    const reg_tensor<T>& src0, const T& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator==(
    const T& src0, const reg_tensor<T>& src1)
```

<a id="operator-ne"></a>
### operator!=

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator!=(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator!=(
    const reg_tensor<T>& src0, const T& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator!=(
    const T& src0, const reg_tensor<T>& src1)
```

<a id="operator-lt"></a>
### operator<

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<(
    const reg_tensor<T>& src0, const T& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<(
    const T& src0, const reg_tensor<T>& src1)
```

<a id="operator-le"></a>
### operator<=

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<=(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<=(
    const reg_tensor<T>& src0, const T& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator<=(
    const T& src0, const reg_tensor<T>& src1)
```

<a id="operator-gt"></a>
### operator>

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>(
    const reg_tensor<T>& src0, const T& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>(
    const T& src0, const reg_tensor<T>& src1)
```

<a id="operator-ge"></a>
### operator>=

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>=(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>=(
    const reg_tensor<T>& src0, const T& src1)

template <typename T>
__simd_callee__ inline reg_tensor<bool> operator>=(
    const T& src0, const reg_tensor<T>& src1)
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| src0 | 输入 | 第一个源操作数，类型为reg_tensor&lt;T&gt;或标量T。类型为reg_tensor&lt;T&gt;时，其mask用于控制参与计算的元素。 |
| src1 | 输入 | 第二个源操作数，类型为reg_tensor&lt;T&gt;或标量T。当src0为标量时，src1.mask用于控制参与计算的元素；当src0为reg_tensor&lt;T&gt;时，src1.mask不参与计算。 |

## 数据类型

两个源操作数的数据类型必须相同。支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。

## 返回值说明

返回类型为reg_tensor&lt;bool&gt;。返回值的reg保存逐元素比较结果：对应位置为1表示比较结果成立，为0表示比较结果不成立。

## 约束说明

- 用于控制计算的mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- reg_tensor与reg_tensor形式使用src0.mask；reg_tensor与标量形式使用reg_tensor操作数的mask。
- 输入mask对应位置为0时，返回结果的对应位置为0。

## 调用示例

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void compare_example(InputTensor input, OutputTensor output)
{
    using asc::te::experimental::operator>=;
    auto src = asc::te::experimental::load(input, asc::te::make_coord(0))
                   .with_mask(asc::te::experimental::all_mask<float>());
    auto condition = src >= 0.0f;
    auto zero = asc::te::experimental::fill(0.0f, asc::te::experimental::all_mask<float>());
    auto dst = asc::te::experimental::select(condition, src, zero);
    asc::te::experimental::store(output, asc::te::make_coord(0), dst);
}
```
