# GM与UB数据搬运

GM与UB之间的数据搬运接口按方向、对齐条件和维度选择。概览仅列出公开符号；完整重载、参数单位和产品支持情况以单接口文档为准。

| 场景 | GM->UB | UB->GM | 说明 |
| --- | --- | --- | --- |
| 连续或分块搬运 | [asc_copy_gm2ub](../asc_copy_gm2ub/asc_copy_gm2ub.md) | [asc_copy_ub2gm](../asc_copy_ub2gm/asc_copy_ub2gm.md) | 连续重载使用按字节计量的`size`；分块重载使用`n_burst`、`len_burst`及gap或stride参数。 |
| 非对齐搬运 | [asc_copy_gm2ub_align](../asc_copy_gm2ub_align/asc_copy_gm2ub_align.md) | [asc_copy_ub2gm_align](../asc_copy_ub2gm_align/asc_copy_ub2gm_align.md) | 3510架构提供Padding、stride和L2 Cache控制相关重载，2201架构的重载不同。 |
| 多维搬运 | [asc_ndim_copy_gm2ub](../asc_ndim_copy_gm2ub.md) | 暂无对应公开接口 | 使用多维循环大小以及预先配置的源、目的stride和Padding信息。 |

## 多维搬运配置

调用`asc_ndim_copy_gm2ub`前，根据场景使用以下接口：

- [asc_set_ndim_loop_stride](../asc_set_ndim_loop_stride.md)
- [asc_set_ndim_pad_count](../asc_set_ndim_pad_count.md)
- [asc_set_ndim_pad_value](../asc_set_ndim_pad_value.md)

多维搬运通过上述配置接口和`asc_ndim_copy_gm2ub`的标量参数完成。L2 Cache策略通过该接口的`l2_cache_mode`参数配置。

## 使用注意事项

- `size`表示字节数，不能直接传入元素个数。
- 分块重载的参数单位随架构和接口变化，不应从其他搬运接口类推。
- 源、目的指针必须分别满足`__gm__`和`__ubuf__`限定符及单接口文档中的对齐要求。
- 异步搬运与后续计算或写回存在数据依赖时，应插入对应的流水同步。
