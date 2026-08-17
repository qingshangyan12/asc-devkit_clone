# Algorithm

Tensor API的Algorithm层提供`copy`和`mmad`统一调用入口。调用前通常先选择Arch层的Operation和Trait，并通过`make_copy`或`make_mmad`构造Atom；需要运行时参数时，使用Atom的`with`成员绑定Params。

```cpp
auto copy_atom = make_copy(copy_gm_to_l1{}, gm_to_l1_trait_default{});
copy(copy_atom, l1_tensor, gm_tensor);

mmad_params params{m, n, k, unit_flag_mode::disable, true};
auto atom = make_mmad(mmad_operation{}, mmad_trait_default{}).with(params);
mmad(atom, c_tensor, a_tensor, b_tensor);
```

输出Tensor参数位于输入Tensor参数之前。Operation决定计算或搬运通路，Trait描述编译期静态配置，Params描述运行时配置，Atom保存这些配置并由Algorithm接口发起调用。

- [Copy Operation、Atom和Trait](copy_operation_atom_trait.md)
- [Mmad Operation、Atom和Trait](../matrix_compute/mmad_compute/mmad_operation_atom_trait.md)
- [数据搬运接口](../data_move/data_move.md)
- [mmad接口](../matrix_compute/mmad_compute/mmad.md)

Operation、Trait、Params和Tensor必须属于同一受支持通路。接口不会自动修改Tensor的存储位置或Layout，也不会自动插入所有跨流水线同步。
