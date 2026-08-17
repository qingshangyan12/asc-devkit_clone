# Mmad Operation、Atom和Default Trait

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

`mmad_operation`标识矩阵乘计算，`mmad_trait_default`提供默认静态配置，`mmad_atom`组合Operation和Trait并保存通过`with`绑定的`mmad_params`。用户通过`make_mmad`构造Atom，再调用`mmad`执行矩阵计算。

```cpp
mmad_params params(m, n, k, unit_flag_mode::disable, true);
auto atom = make_mmad(mmad_operation{}, mmad_trait_default{}).with(params);
mmad(atom, c, a, b);
```

## mmad_operation

`mmad_operation`公开的成员如下。`mmad`静态成员负责将Atom携带的Trait和参数分发到矩阵计算实现，通常由`mmad_atom`内部调用，用户执行矩阵计算时使用顶层`mmad`接口。

以下仅展示`mmad_operation`中的public内容，不展示内部private实现：

```cpp
struct mmad_operation {
public:
    template <typename Trait, const Trait& traits, typename... Args>
    __aicore__ inline static void mmad(const Args&... args);
};
```

## mmad_atom

`mmad_atom`由`make_mmad`返回。用户可使用的主要成员函数如下：

```cpp
template <const trait_type& traits = default_trait, typename... Params>
__aicore__ inline void call(const Params&... params) const;

template <typename... TraitArgs>
__aicore__ inline auto with(TraitArgs&&... args) const;
```

| 成员函数 | 描述 |
| :--- | :--- |
| `call` | 使用Atom保存的Operation、Trait和运行时参数执行分发。通常由顶层`mmad`接口调用。 |
| `with` | 绑定`mmad_params`并返回新的Atom，不修改原Atom。 |

## Default Trait

`mmad_trait_default`保存默认`mmad_trait`类型和默认值。

```cpp
struct mmad_trait_default {
    using trait_type = mmad_trait;
    static constexpr const trait_type value = {};
};
```

省略`make_mmad`的第二个参数时，接口自动使用`mmad_trait_default`。需要配置MX、GEMV或BT Buffer初始化等静态特性时，用户可自定义包含`trait_type`和`value`成员的Trait包装类型，并将其作为`make_mmad`的第二个参数。`mmad_trait`各成员的含义和约束参见[mmad](mmad.md)。

## 约束说明

- `with`绑定的对象必须为`mmad_params`。
- 自定义Trait的`trait_type`必须为`mmad_trait`，`value`必须是编译期可用的`mmad_trait`对象。
- Atom对象只保存计算配置，不替代输入、输出Tensor及其Layout合法性检查。
