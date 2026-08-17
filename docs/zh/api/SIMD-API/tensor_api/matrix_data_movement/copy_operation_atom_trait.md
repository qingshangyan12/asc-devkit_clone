# Copy Operation、Atom和Trait

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

Tensor API通过Operation、Trait和Atom组合数据搬运能力：

- Operation标识源存储位置、目的存储位置及其对应的搬运通路。
- Trait描述编译期确定的静态搬运特性，Default Trait提供通路默认配置。
- Params描述运行时搬运参数，通过`copy_atom::with`绑定。
- `copy_atom`保存Operation、Trait和Params，顶层`copy`接口使用该Atom执行搬运。

不需要运行时参数的通路，可直接构造Atom并执行搬运：

```cpp
auto atom = make_copy(copy_gm_to_l1{}, gm_to_l1_trait_default{});
copy(atom, dst, src);
```

需要运行时参数的通路，先通过`with`生成绑定参数的新Atom：

```cpp
l0c_to_gm_params params;
auto atom = make_copy(copy_l0c_to_gm{}, l0c_to_gm_trait_default{}).with(params);
copy(atom, dst, src);
```

## make_copy

`make_copy`根据Operation和可选的Trait构造`copy_atom`。省略Trait参数时，接口使用Operation对应的Default Trait。

```cpp
template <typename CopyOperation>
__aicore__ inline constexpr auto make_copy(const CopyOperation& operation)

template <typename CopyOperation, typename CopyTrait>
__aicore__ inline constexpr auto make_copy(
    const CopyOperation& operation, const CopyTrait& trait)
```

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| operation | 输入 | 数据搬运Operation对象。 |
| trait | 输入 | 可选的Trait包装对象，需要提供`trait_type`和`value`成员。 |

`make_copy`返回`copy_atom`对象。

## copy_traits

`copy_traits`保存Operation、Trait及绑定运行时参数后需要使用的Operation和Trait类型。其公开定义如下：

```cpp
template <typename CopyOperation, typename... CopyOpArgs>
struct copy_traits {};

template <typename CopyOperation, typename CopyTrait,
    typename CopyOperationWith, typename CopyTraitWith>
struct copy_traits<CopyOperation, CopyTrait, CopyOperationWith, CopyTraitWith> {
    using trait_type = get_trait_member_type_t<CopyTrait>;
    static constexpr const trait_type default_trait = CopyTrait::value;

    template <typename Params>
    __aicore__ inline constexpr copy_traits<CopyOperationWith, CopyTraitWith>
        with(const Params& params) const;

    template <const trait_type& trait = default_trait, typename... Args>
    __aicore__ inline void copy_unpack(const Args&... args) const;
};
```

| 成员 | 描述 |
| :--- | :--- |
| trait_type | 当前通路使用的具体Trait类型。 |
| default_trait | 当前Trait包装类型提供的默认Trait值。 |
| with | 绑定运行时参数，返回包含对应With Operation和With Trait的`copy_traits`对象。 |
| copy_unpack | 展开已绑定参数并调用Operation的静态`copy`成员。该成员由`copy_atom::call`调用。 |

## copy_atom

`copy_atom`继承对应的`copy_traits`，保存搬运配置并提供`call`和`with`成员。其公开定义如下：

```cpp
template <typename... Args>
struct copy_atom;

template <typename CopyOperation>
struct copy_atom<CopyOperation> : copy_atom<copy_traits<CopyOperation>> {};

template <typename... Args>
struct copy_atom<copy_traits<Args...>> : copy_traits<Args...> {
    using copy_traits_type = copy_traits<Args...>;
    using trait_type = get_trait_member_type_t<copy_traits_type>;
    static constexpr const trait_type default_trait = copy_traits_type::default_trait;

    template <const trait_type& traits = default_trait, typename... Params>
    __aicore__ inline void call(const Params&... params) const;

    template <typename... TraitArgs>
    __aicore__ inline auto with(TraitArgs&&... args) const;
};
```

| 成员 | 描述 |
| :--- | :--- |
| copy_traits_type | 当前Atom继承的`copy_traits`类型。 |
| trait_type | 当前通路使用的具体Trait类型。 |
| default_trait | 当前通路使用的默认Trait值。 |
| call | 使用Atom中的Operation、Trait和运行时参数执行分发，通常由顶层`copy`接口调用。 |
| with | 绑定运行时参数并返回新的Atom，不修改原Atom。 |

## 矩阵搬运Operation和Trait列表

每个Operation公开一个静态`copy`成员，负责将Trait、Tensor及运行时参数分发到对应通路。本节仅列出矩阵计算相关搬运Operation，不包含Vector侧搬运Operation。Operation的public定义、具体Trait字段、Default Trait和Params均在对应通路资料中说明。

| 搬运方向 | Operation | Trait | Default Trait | Params | 详细资料 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Global Memory到L1 Buffer | `copy_gm_to_l1` | `gm_to_l1_trait` | `gm_to_l1_trait_default` | 无 | [查看](cube_compute_load/copy_gm_to_l1.md) |
| L1 Buffer到Unified Buffer | `copy_l1_to_ub` | `l1_to_ub_trait` | `l1_to_ub_trait_default` | 无 | [查看](cube_compute_load/copy_l1_to_ub.md) |
| L1 Buffer到L0A Buffer | `copy_l1_to_l0a` | `l1_to_l0a_trait` | `l1_to_l0a_trait_default` | `img2col_params<PadType>`，仅Img2Col场景使用 | [查看](cube_compute_load/copy_l1_to_l0a.md) |
| L1 Buffer到L0B Buffer | `copy_l1_to_l0b` | `l1_to_l0b_trait` | `l1_to_l0b_trait_default` | 无 | [查看](cube_compute_load/copy_l1_to_l0b.md) |
| L1 Buffer到L0ScaleA Buffer | `copy_l1_to_l0scalea` | `l1_to_l0scalea_trait` | `l1_to_l0scalea_trait_default` | 无 | [查看](cube_compute_load/copy_l1_to_l0scalea.md) |
| L1 Buffer到L0ScaleB Buffer | `copy_l1_to_l0scaleb` | `l1_to_l0scaleb_trait` | `l1_to_l0scaleb_trait_default` | 无 | [查看](cube_compute_load/copy_l1_to_l0scaleb.md) |
| L1 Buffer到Fixpipe Buffer | `copy_l1_to_fixbuf` | `l1_to_fixbuf_trait` | `l1_to_fixbuf_trait_default` | 无 | [查看](cube_compute_load/copy_l1_to_fixbuf.md) |
| L1 Buffer到BiasTable Buffer | `copy_l1_to_biastable` | `l1_to_biastable_trait` | `l1_to_biastable_trait_default` | 无 | [查看](cube_compute_load/copy_l1_to_biastable.md) |
| L0C Buffer到Global Memory | `copy_l0c_to_gm` | `l0c_to_gm_trait` | `l0c_to_gm_trait_default` | `l0c_to_gm_params` | [查看](cube_compute_store/copy_l0c_to_gm.md) |
| L0C Buffer到Unified Buffer | `copy_l0c_to_ub` | `l0c_to_ub_trait` | `l0c_to_ub_trait_default` | `l0c_to_ub_params` | [查看](cube_compute_store/copy_l0c_to_ub.md) |
| L0C Buffer到L1 Buffer | `copy_l0c_to_l1` | `l0c_to_l1_trait` | `l0c_to_l1_trait_default` | `l0c_to_l1_params` | [查看](cube_compute_store/copy_l0c_to_l1.md) |

## 约束说明

- Operation、Trait和Params必须属于同一数据搬运通路。
- Trait是编译期配置，传入`make_copy`的自定义Trait包装类型需要提供`trait_type`和编译期常量`value`。
- Params是运行时配置，只能绑定到支持该Params类型的Operation。
- Atom只保存搬运配置，不替代源张量、目的张量、Layout、数据类型和搬运范围的合法性检查。
