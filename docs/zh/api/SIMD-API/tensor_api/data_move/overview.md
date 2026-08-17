# 数据搬运概述

Tensor API数据搬运接口以Tensor为操作对象。目的Tensor参数位于源Tensor参数之前，接口根据两端存储位置和Layout Pattern选择搬运通路。完整搬运直接传入源Tensor和目的Tensor；区域搬运还可以传入目的坐标、源坐标和搬运区域大小。

```cpp
auto atom = make_copy(copy_gm_to_ub{}, gm_to_ub_trait_default{});
copy(atom, ub_tensor, gm_tensor);

copy(atom, ub_tensor, gm_tensor, dst_coord, src_coord, copy_shape);
```

使用数据搬运接口时需要满足以下通用约束：

- 源Tensor和目的Tensor的存储位置必须与Operation描述的通路一致。
- 两端Layout Pattern组合必须由对应通路支持；没有明确说明格式转换时，源和目的应使用相同数据排布。
- 源和目的元素类型及元素数量需要满足对应接口要求，目的Tensor容量不能小于实际写入范围。
- 坐标及搬运区域的形状需要与Tensor的形状结构匹配，且搬运区域不能越界。
- 接口不会自动处理所有跨流水线依赖，复用片上Buffer前需要按执行流程完成同步。

硬件数据通路和通用搬运约束可参考[基础API数据搬运导览](../../basic_api/data_move_guide/data_move_guide.md)。
