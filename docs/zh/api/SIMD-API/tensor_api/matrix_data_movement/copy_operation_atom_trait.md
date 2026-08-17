# Copy Operation、Atom和Default Trait

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

需要包含的头文件为：#include "tensor_api/tensor.h"。

`copy_operation`用于标识数据搬运通路，`trait_default`用于提供该通路的默认静态配置，`copy_atom`组合Operation和Trait并保存通过`with`绑定的运行时参数。用户通过`make_copy`构造Atom，再调用`copy`执行搬运。

推荐调用流程如下：

```cpp
auto atom = make_copy(copy_gm_to_l1{}, gm_to_l1_trait_default{});
copy(atom, dst, src);
```

如果通路具有运行时参数，使用`with`生成绑定参数后的新Atom：

```cpp
l0c_to_gm_params params;
auto atom = make_copy(copy_l0c_to_gm{}, l0c_to_gm_trait_default{}).with(params);
copy(atom, dst, src);
```

## copy_operation

每种`copy_operation`均公开同一形式的静态`copy`成员。该成员负责将Atom携带的Trait和参数分发到对应搬运实现，通常由`copy_atom`内部调用，用户直接调用数据搬运时使用顶层`copy`接口。

以下以`copy_gm_to_l1`为例，仅展示Operation中的public内容，不展示内部private实现。其他`copy_operation`具有相同形式的public成员。

```cpp
struct copy_gm_to_l1 {
public:
    template <typename Trait, const Trait& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};
```

| 数据搬运方向 | copy_operation |
| :--- | :--- |
| Global Memory到L1 Buffer | `copy_gm_to_l1` |
| Global Memory到Unified Buffer | `copy_gm_to_ub` |
| L1 Buffer到Unified Buffer | `copy_l1_to_ub` |
| Unified Buffer到L1 Buffer | `copy_ub_to_l1` |
| Unified Buffer到Global Memory | `copy_ub_to_gm` |
| Unified Buffer到Unified Buffer | `copy_ub_to_ub` |
| L1 Buffer到L0A Buffer | `copy_l1_to_l0a` |
| L1 Buffer到L0B Buffer | `copy_l1_to_l0b` |
| L1 Buffer到L0ScaleA Buffer | `copy_l1_to_l0scalea` |
| L1 Buffer到L0ScaleB Buffer | `copy_l1_to_l0scaleb` |
| L1 Buffer到Fixpipe Buffer | `copy_l1_to_fixbuf` |
| L1 Buffer到BiasTable Buffer | `copy_l1_to_biastable` |
| L0C Buffer到Global Memory | `copy_l0c_to_gm` |
| L0C Buffer到Unified Buffer | `copy_l0c_to_ub` |
| L0C Buffer到L1 Buffer | `copy_l0c_to_l1` |

## copy_atom

`copy_atom`由`make_copy`返回。用户可使用的主要成员函数如下：

```cpp
template <const trait_type& traits = default_trait, typename... Params>
__aicore__ inline void call(const Params&... params) const;

template <typename... TraitArgs>
__aicore__ inline auto with(TraitArgs&&... args) const;
```

| 成员函数 | 描述 |
| :--- | :--- |
| `call` | 使用`copy_atom`保存的`copy_operation`、`trait`和运行时参数执行分发。通常由顶层`copy`接口调用。 |
| `with` | 绑定运行时参数并返回新的`copy_atom`，不修改原`copy_atom`。运行时参数类型由具体通路决定。 |

## trait_default

`trait_default`保存对应通路的默认trait类型和默认值。以Global Memory到L1 Buffer通路为例，其定义如下：

```cpp
struct gm_to_l1_trait_default {
    using trait_type = gm_to_l1_trait;
    static constexpr const trait_type value = {};
};
```

| copy_operation | trait | trait_default |
| :--- | :--- | :--- |
| `copy_gm_to_l1` | `gm_to_l1_trait` | `gm_to_l1_trait_default` |
| `copy_gm_to_ub` | `gm_to_ub_trait` | `gm_to_ub_trait_default` |
| `copy_l1_to_ub` | `l1_to_ub_trait` | `l1_to_ub_trait_default` |
| `copy_ub_to_l1` | `ub_to_l1_trait` | `ub_to_l1_trait_default` |
| `copy_ub_to_gm` | `ub_to_gm_trait` | `ub_to_gm_trait_default` |
| `copy_ub_to_ub` | `ub_to_ub_trait` | `ub_to_ub_trait_default` |
| `copy_l1_to_l0a` | `l1_to_l0a_trait` | `l1_to_l0a_trait_default` |
| `copy_l1_to_l0b` | `l1_to_l0b_trait` | `l1_to_l0b_trait_default` |
| `copy_l1_to_l0scalea` | `l1_to_l0scalea_trait` | `l1_to_l0scalea_trait_default` |
| `copy_l1_to_l0scaleb` | `l1_to_l0scaleb_trait` | `l1_to_l0scaleb_trait_default` |
| `copy_l1_to_fixbuf` | `l1_to_fixbuf_trait` | `l1_to_fixbuf_trait_default` |
| `copy_l1_to_biastable` | `l1_to_biastable_trait` | `l1_to_biastable_trait_default` |
| `copy_l0c_to_gm` | `l0c_to_gm_trait` | `l0c_to_gm_trait_default` |
| `copy_l0c_to_ub` | `l0c_to_ub_trait` | `l0c_to_ub_trait_default` |
| `copy_l0c_to_l1` | `l0c_to_l1_trait` | `l0c_to_l1_trait_default` |

省略`make_copy`的第二个参数时，接口会自动选择operation对应的trait_default。需要修改静态搬运特性时，用户可自定义包含`trait_type`和`value`成员的trait包装类型，并将其作为`make_copy`的第二个参数。

## 约束说明

- `copy_operation`、`trait`和运行时参数必须属于同一数据搬运通路。
- `with`绑定的参数需要满足对应通路资料中的类型和取值约束。
- `copy_atom`对象只保存配置，不替代目的Tensor、源Tensor及其Layout合法性检查。
