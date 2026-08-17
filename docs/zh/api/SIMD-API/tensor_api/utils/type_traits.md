# 类型判断与提取工具

> [!NOTE]说明
>本接口为试验接口，在后续版本中可能会调整或改进，不保证后续兼容性。请开发者在使用过程中关注后续版本更新。

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
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

需要包含的头文件为：`#include "tensor_api/tensor.h"`。

类型判断工具用于在编译期判断输入是否为Tensor API支持的Layout、Tensor、Coord、Shape或量化参数类型；类型提取工具用于从Default Trait或带地址空间的Pointer类型中提取对应类型。当前Tensor类型判断工具的接口名为`is_attr_tensor_v`。

## 原型定义

```cpp
template <typename Layout>
constexpr bool is_layout_v = is_layout<Std::remove_cvref_t<Layout>>::value;

template <typename Tensor>
constexpr bool is_attr_tensor_v = is_attr_tensor<Std::remove_cvref_t<Tensor>>::value;

template <typename Coord>
constexpr bool is_valid_coord_v;

template <typename CopyShape>
constexpr bool is_valid_shape_v;

template <typename Quant>
constexpr bool is_valid_quant_v;

template <typename Trait>
using get_trait_member_type_t = typename get_trait_member_type<Trait>::type;

template <typename Pointer>
using get_attribute_element_type =
    typename location_type_combo::location_type_to_type::template get<Pointer>;
```

## 工具说明

| 工具 | 输入 | 结果 |
| :--- | :--- | :--- |
| `is_layout_v` | 待判断的类型。 | 输入去除const、volatile和引用后为`layout`类型时返回true，否则返回false。 |
| `is_attr_tensor_v` | 待判断的类型。 | 输入去除const、volatile和引用后为`global_tensor`或`local_tensor`类型时返回true，否则返回false。 |
| `is_valid_coord_v` | 搬运接口的坐标类型。 | 输入为Tuple类型或`zero_coord_type`时返回true，否则返回false。 |
| `is_valid_shape_v` | 搬运区域的Shape类型。 | 输入去除const、volatile和引用后为Tuple类型时返回true，否则返回false。 |
| `is_valid_quant_v` | 搬运接口的量化参数类型。 | 输入为Tensor API Tensor，或为可转换成`uint64_t`的标量类型时返回true，否则返回false。 |
| `get_trait_member_type_t` | 包含`trait_type`成员类型的Default Trait或Traits类型。 | 提取其对应的Trait类型。 |
| `get_attribute_element_type` | 带硬件地址空间属性的Pointer类型。 | 提取Pointer指向的基础元素类型；不支持的Pointer类型对应`void`。 |

上述`*_v`工具均为编译期布尔常量，不会产生运行时指令。`get_layout_pattern`的类型提取能力参见[get_layout_pattern](get_layout_pattern.md)。

## 返回值说明

- `is_layout_v`、`is_attr_tensor_v`、`is_valid_coord_v`、`is_valid_shape_v`和`is_valid_quant_v`返回编译期布尔值。
- `get_trait_member_type_t`和`get_attribute_element_type`为类型别名，不返回运行时数值。

## 约束说明

- 使用类型判断结果进行模板分发时，应通过`if constexpr`、`static_assert`或`Std::enable_if_t`在编译期使用。
- `get_trait_member_type_t`的输入类型需要提供`trait_type`成员类型。
- `get_attribute_element_type`仅用于Tensor API支持的硬件地址空间Pointer类型。

## 调用示例

```cpp
using namespace asc::te;

auto tensor_layout = make_layout(make_shape(_8{}, _16{}), make_stride(_16{}, _1{}));
static_assert(is_layout_v<decltype(tensor_layout)>);

auto tensor = make_tensor(make_mem_ptr<location::gm, float>(0), tensor_layout);
static_assert(is_attr_tensor_v<decltype(tensor)>);

auto coord = make_coord(0, 0);
auto shape = make_shape(8, 16);
static_assert(is_valid_coord_v<decltype(coord)>);
static_assert(is_valid_shape_v<decltype(shape)>);
static_assert(is_valid_quant_v<uint64_t>);

using trait = get_trait_member_type_t<l1_to_l0a_trait_default>;
static_assert(Std::is_same_v<trait, l1_to_l0a_trait>);

using element_type = get_attribute_element_type<__gm__ float*>;
static_assert(Std::is_same_v<element_type, float>);
```
