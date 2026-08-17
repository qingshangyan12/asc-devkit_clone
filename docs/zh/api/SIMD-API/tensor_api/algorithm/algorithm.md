# Algorithm

Tensor API的Algorithm层通过`copy`和`mmad`提供数据搬运和矩阵计算的统一调用入口。调用前通常需要先选择Arch层的Operation和Trait，并通过`make_copy`或`make_mmad`构造Atom；需要配置运行时参数时，使用Atom的`with`成员绑定Params。

## Operation、Atom和Trait

Tensor API将Operation和Traits组合为Atom，并通过Atom提供数据搬运和矩阵计算能力：

- Operation是一个C++结构体，用于封装特定通路的指令级数据搬运或矩阵计算行为。
- Traits是一个与Operation关联的模板特化结构体，为Operation提供所必须的信息。
- Params描述运行时参数，通过Atom的`with`成员函数绑定。
- Atom保存Operation、Trait和可选的Params，顶层`copy`或`mmad`接口使用对应Atom执行数据搬运或矩阵计算。

使用Default Trait且不需要运行时参数的通路，可以仅传入Operation构造Atom：

```cpp
auto atom = make_copy(copy_gm_to_l1{});
copy(atom, dst, src);
```

对于需要运行时参数的通路，先调用`with`生成绑定了参数的新Atom：

```cpp
l0c_to_gm_params params;
auto atom = make_copy(copy_l0c_to_gm{}).with(params);
copy(atom, dst, src);
```

`mmad`采用相同的Atom构造方式。使用Default Trait时，可以仅传入`mmad_operation`，再通过`with`绑定矩阵计算所需的`mmad_params`：

```cpp
mmad_params params(m, n, k, unit_flag_mode::disable, true);
auto atom = make_mmad(mmad_operation{}).with(params);
mmad(atom, c, a, b);
```

### copy_traits

`copy_traits`保存Operation、Trait，以及绑定运行时参数后使用的Operation和Trait类型信息，其定义如下：

```cpp
template <typename CopyOperation, typename... CopyOpArgs>
struct copy_traits {};

template <typename CopyOperation, typename CopyTrait,
    typename CopyOperationWith, typename CopyTraitWith>
struct copy_traits<CopyOperation, CopyTrait, CopyOperationWith, CopyTraitWith> {
    using trait_type = typename CopyTrait::trait_type;
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
| trait_type | 通路使用的具体Trait类型。 |
| default_trait | Trait包装类型提供的默认Trait值。 |
| with | 绑定运行时参数，返回包含对应Operation和Trait的`copy_traits`对象。 |
| copy_unpack | 展开已绑定参数并调用Operation的静态`copy`成员，完成数据搬运。 |

### copy_atom

`copy_atom`继承对应的`copy_traits`，保存搬运配置并提供`call`和`with`成员函数，其定义如下：

```cpp
template <typename... Args>
struct copy_atom;

template <typename CopyOperation>
struct copy_atom<CopyOperation> : copy_atom<copy_traits<CopyOperation>> {};

template <typename... Args>
struct copy_atom<copy_traits<Args...>> : copy_traits<Args...> {
    using copy_traits_type = copy_traits<Args...>;
    using trait_type = typename copy_traits_type::trait_type;
    static constexpr const trait_type default_trait = copy_traits_type::default_trait;

    template <const trait_type& traits = default_trait, typename... Params>
    __aicore__ inline void call(const Params&... params) const;

    template <typename... TraitArgs>
    __aicore__ inline auto with(TraitArgs&&... args) const;
};
```

| 成员 | 描述 |
| :--- | :--- |
| copy_traits_type | `copy_traits`类型。 |
| trait_type | 通路使用的具体Trait类型。 |
| default_trait | 通路使用的默认Trait值。 |
| call | 使用Atom中的Operation执行数据搬运。 |
| with | 绑定运行时参数并返回新的Atom，不修改原Atom。 |

### mmad_traits

`mmad_traits`保存Operation、Trait，以及绑定运行时参数后使用的Operation和Trait类型信息，其定义如下：

```cpp
template <typename MmadOperation, typename... MmadOpArgs>
struct mmad_traits {};

template <typename MmadOperation, typename MmadTrait,
    typename MmadOperationWith, typename MmadTraitWith>
struct mmad_traits<MmadOperation, MmadTrait, MmadOperationWith, MmadTraitWith> {
    using trait_type = typename MmadTrait::trait_type;
    static constexpr const trait_type default_trait = MmadTrait::value;

    template <typename Params>
    __aicore__ inline constexpr mmad_traits<MmadOperationWith, MmadTraitWith>
        with(const Params& params) const;

    template <const trait_type& trait = default_trait, typename... Args>
    __aicore__ inline void mmad_unpack(const Args&... args) const;
};
```

| 成员 | 描述 |
| :--- | :--- |
| trait_type | 矩阵计算使用的具体Trait类型。 |
| default_trait | Trait包装类型提供的默认Trait值。 |
| with | 绑定运行时参数，返回包含对应Operation和Trait的`mmad_traits`对象。 |
| mmad_unpack | 展开已绑定参数并调用Operation的静态`mmad`成员，完成矩阵计算。 |

### mmad_atom

`mmad_atom`继承对应的`mmad_traits`，保存矩阵计算配置并提供`call`和`with`成员函数，其定义如下：

```cpp
template <typename... Args>
struct mmad_atom;

template <typename MmadOperation>
struct mmad_atom<MmadOperation> : mmad_atom<mmad_traits<MmadOperation>> {};

template <typename MmadOperation, typename... Args>
struct mmad_atom<mmad_traits<MmadOperation, Args...>> : mmad_traits<MmadOperation, Args...> {
    using mmad_traits_type = mmad_traits<MmadOperation, Args...>;
    using trait_type = typename mmad_traits_type::trait_type;
    static constexpr const trait_type default_trait = mmad_traits_type::default_trait;

    template <const trait_type& traits = default_trait, typename... Params>
    __aicore__ inline void call(const Params&... params) const;

    template <typename... TraitArgs>
    __aicore__ inline auto with(TraitArgs&&... args) const;
};
```

| 成员 | 描述 |
| :--- | :--- |
| mmad_traits_type | `mmad_traits`类型。 |
| trait_type | 矩阵计算使用的具体Trait类型。 |
| default_trait | 矩阵计算使用的默认Trait值。 |
| call | 使用Atom中的Operation执行矩阵计算。 |
| with | 绑定运行时参数并返回新的Atom，不修改原Atom。 |

### Operation和Traits关系列表

Operation接收编译期特性Trait和运行时参数Params，完成对应物理通路的数据搬运或者矩阵计算。

| 功能 | Operation | Traits | Trait | Params |
| :--- | :--- | :--- | :--- | :--- |
| [Global Memory到Unified Buffer](../data_move/copy_gm_to_ub.md) | `copy_gm_to_ub` | `copy_traits<copy_gm_to_ub, ...>` | `gm_to_ub_trait` | `gm_to_ub_params` |
| [Unified Buffer到Global Memory](../data_move/copy_ub_to_gm.md) | `copy_ub_to_gm` | `copy_traits<copy_ub_to_gm, ...>` | `ub_to_gm_trait` | 无 |
| [Global Memory到L1 Buffer](../matrix_compute/cube_compute_load/copy_gm_to_l1.md) | `copy_gm_to_l1` | `copy_traits<copy_gm_to_l1, ...>` | `gm_to_l1_trait` | 无 |
| [Unified Buffer到L1 Buffer](../data_move/copy_ub_to_l1.md) | `copy_ub_to_l1` | `copy_traits<copy_ub_to_l1, ...>` | `ub_to_l1_trait` | 无 |
| [L1 Buffer到Unified Buffer](../matrix_compute/cube_compute_load/copy_l1_to_ub.md) | `copy_l1_to_ub` | `copy_traits<copy_l1_to_ub, ...>` | `l1_to_ub_trait` | 无 |
| [L1 Buffer到L0A Buffer](../matrix_compute/cube_compute_load/copy_l1_to_l0a.md) | `copy_l1_to_l0a` | `copy_traits<copy_l1_to_l0a, ...>` | `l1_to_l0a_trait` | `img2col_params<PadType>`，仅Img2Col场景使用 |
| [L1 Buffer到L0B Buffer](../matrix_compute/cube_compute_load/copy_l1_to_l0b.md) | `copy_l1_to_l0b` | `copy_traits<copy_l1_to_l0b, ...>` | `l1_to_l0b_trait` | 无 |
| [L1 Buffer到L0ScaleA Buffer](../matrix_compute/cube_compute_load/copy_l1_to_l0scalea.md) | `copy_l1_to_l0scalea` | `copy_traits<copy_l1_to_l0scalea, ...>` | `l1_to_l0scalea_trait` | 无 |
| [L1 Buffer到L0ScaleB Buffer](../matrix_compute/cube_compute_load/copy_l1_to_l0scaleb.md) | `copy_l1_to_l0scaleb` | `copy_traits<copy_l1_to_l0scaleb, ...>` | `l1_to_l0scaleb_trait` | 无 |
| [L1 Buffer到Fixpipe Buffer](../matrix_compute/cube_compute_load/copy_l1_to_fixbuf.md) | `copy_l1_to_fixbuf` | `copy_traits<copy_l1_to_fixbuf, ...>` | `l1_to_fixbuf_trait` | 无 |
| [L1 Buffer到BiasTable Buffer](../matrix_compute/cube_compute_load/copy_l1_to_biastable.md) | `copy_l1_to_biastable` | `copy_traits<copy_l1_to_biastable, ...>` | `l1_to_biastable_trait` | 无 |
| [L0C Buffer到Global Memory](../matrix_compute/cube_compute_store/copy_l0c_to_gm.md) | `copy_l0c_to_gm` | `copy_traits<copy_l0c_to_gm, ...>` | `l0c_to_gm_trait` | `l0c_to_gm_params` |
| [L0C Buffer到Unified Buffer](../matrix_compute/cube_compute_store/copy_l0c_to_ub.md) | `copy_l0c_to_ub` | `copy_traits<copy_l0c_to_ub, ...>` | `l0c_to_ub_trait` | `l0c_to_ub_params` |
| [L0C Buffer到L1 Buffer](../matrix_compute/cube_compute_store/copy_l0c_to_l1.md) | `copy_l0c_to_l1` | `copy_traits<copy_l0c_to_l1, ...>` | `l0c_to_l1_trait` | `l0c_to_l1_params` |
| [Unified Buffer到Unified Buffer](../data_move/copy_ub_to_ub.md) | `copy_ub_to_ub` | `copy_traits<copy_ub_to_ub, ...>` | `ub_to_ub_trait` | 无 |
| [矩阵乘计算](../matrix_compute/mmad_compute/mmad.md) | `mmad_operation` | `mmad_traits<mmad_operation, ...>` | `mmad_trait` | `mmad_params` |
