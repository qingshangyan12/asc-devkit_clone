# Tensor API 接口对外/删除整理

> 标注说明：
> - **放出去**：当前在 `impl/tensor_api/` 内部实现中，需要移到 `include/tensor_api/` 对外公共头文件中
> - **删除**：当前代码中标记为废弃，后续版本需要删除
> - **资料删除**：内部实现细节，不需要对外暴露，后续可删除
> - **已完成判定**：接口声明或前置声明直接位于 `include/tensor_api/` 公共头中；仅由公共头间接包含 `impl/tensor_api/` 实现文件不视为已完成

## 一、需要放出去的接口（impl → include）

### 1.1 类型别名（layout_definition.h）

| 标识符 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|---------|---------|------|------|
| `shape` | `impl/tensor_api/tensor/layout_definition.h:37` | `include/tensor_api/tensor/layout.h` | Shape 类型 | ✅ 已完成 |
| `stride` | `impl/tensor_api/tensor/layout_definition.h:40` | `include/tensor_api/tensor/layout.h` | Stride 类型 | ✅ 已完成 |
| `coord` | `impl/tensor_api/tensor/layout_definition.h:43` | `include/tensor_api/tensor/layout.h` | Coord 类型 | ✅ 已完成 |
| `layout` | `impl/tensor_api/tensor/layout_definition.h:46` | `include/tensor_api/tensor/layout.h` | Layout 主结构体 | ✅ 已完成 |
| `get_layout_pattern` | `impl/tensor_api/tensor/layout_definition.h:203` | `include/tensor_api/tensor/layout.h` | 获取 layout pattern | ✅ 已完成 |


### 1.2 Layout Pattern（layout_pattern.h）

| 标识符 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|---------|---------|------|------|
| `zn_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:31` | `include/tensor_api/tensor/layout_type.h` | ZN layout pattern | ✅ 已完成 |
| `zz_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:32` | `include/tensor_api/tensor/layout_type.h` | ZZ layout pattern | ✅ 已完成 |
| `nn_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:33` | `include/tensor_api/tensor/layout_type.h` | NN layout pattern | ✅ 已完成 |
| `nz_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:34` | `include/tensor_api/tensor/layout_type.h` | NZ layout pattern | ✅ 已完成 |
| `nd_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:35` | `include/tensor_api/tensor/layout_type.h` | ND layout pattern | ✅ 已完成 |
| `dn_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:36` | `include/tensor_api/tensor/layout_type.h` | DN layout pattern | ✅ 已完成 |
| `nd_ext_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:37` | `include/tensor_api/tensor/layout_type.h` | ND Ext layout pattern | ✅ 已完成 |
| `dn_ext_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:38` | `include/tensor_api/tensor/layout_type.h` | DN Ext layout pattern | ✅ 已完成 |
| `scalea_nd_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:39` | `include/tensor_api/tensor/layout_type.h` | ScaleA ND layout pattern | ✅ 已完成 |
| `scalea_dn_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:40` | `include/tensor_api/tensor/layout_type.h` | ScaleA DN layout pattern | ✅ 已完成 |
| `scaleb_nd_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:41` | `include/tensor_api/tensor/layout_type.h` | ScaleB ND layout pattern | ✅ 已完成 |
| `scaleb_dn_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:42` | `include/tensor_api/tensor/layout_type.h` | ScaleB DN layout pattern | ✅ 已完成 |
| `nchw_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:43` | `include/tensor_api/tensor/layout_type.h` | NCHW layout pattern | ✅ 已完成 |
| `nhwc_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:44` | `include/tensor_api/tensor/layout_type.h` | NHWC layout pattern | ✅ 已完成 |
| `nc1hwc0_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:45` | `include/tensor_api/tensor/layout_type.h` | NC1HWC0 layout pattern | ✅ 已完成 |
| `ncdhw_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:46` | `include/tensor_api/tensor/layout_type.h` | NCDHW layout pattern | ✅ 已完成 |
| `ndc1hwc0_layout_ptn` | `impl/tensor_api/tensor/layout_pattern.h:47` | `include/tensor_api/tensor/layout_type.h` | NDC1HWC0 layout pattern | ✅ 已完成 |
| `make_pattern_layout` | `impl/tensor_api/tensor/layout_pattern.h:45` | `include/tensor_api/tensor/layout_interface.h` | 创建 pattern layout（注：使用 `make_layout` 可无需指定 layout_pattern 和 layout_trait） | ✅ 已完成 |


### 1.3 Layout Trait（layout_frame.h）

| 标识符 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|---------|---------|------|------|
| `layout_trait` | `impl/tensor_api/tensor/layout_frame.h:43` | `include/tensor_api/tensor/layout.h` | Layout trait 结构体 | ⬜ |
| `layout_trait_default` | `impl/tensor_api/tensor/layout_frame.h:48` | `include/tensor_api/tensor/layout.h` | 默认 layout trait | ⬜ |
| `frame_layout_format` | `impl/tensor_api/tensor/layout_frame.h:190` | `include/tensor_api/tensor/layout.h` | Frame layout format 结构体 | ⬜ |
| `make_frame_layout` (2个重载) | `impl/tensor_api/tensor/layout_frame.h:169,184` | `include/tensor_api/tensor/layout.h` | 创建 frame layout | ✅ 已完成 |


### 1.4 Layout 方法函数（layout_method.h）

| 函数 | 当前位置 | 目标位置 | 说明 | 状态 |
|------|---------|---------|------|------|
| `make_shape` | `impl/tensor_api/tensor/layout_method.h:35` | `include/tensor_api/tensor/layout.h` | 创建 shape | ✅ 已完成 |
| `make_stride` | `impl/tensor_api/tensor/layout_method.h:42` | `include/tensor_api/tensor/layout.h` | 创建 stride | ✅ 已完成 |
| `make_coord` | `impl/tensor_api/tensor/layout_method.h:54` | `include/tensor_api/tensor/layout.h` | 创建 coord | ✅ 已完成 |
| `make_layout` (shape+stride) | `impl/tensor_api/tensor/layout_method.h:60` | `include/tensor_api/tensor/layout.h` | 创建 layout | ✅ 已完成 |
| `make_layout` (仅shape) | `impl/tensor_api/tensor/layout_method.h:150` | `include/tensor_api/tensor/layout.h` | 创建 layout | ✅ 已完成 |
| `get_shape` | `impl/tensor_api/tensor/layout_method.h:162` | `include/tensor_api/tensor/layout.h` | 获取 shape | ⬜ |
| `get_stride` | `impl/tensor_api/tensor/layout_method.h:174` | `include/tensor_api/tensor/layout.h` | 获取 stride | ⬜ |
| `coshape` | `impl/tensor_api/tensor/layout_method.h:209` | `include/tensor_api/tensor/layout.h` | 获取 coshape | ⬜ |
| `cosize` | `impl/tensor_api/tensor/layout_method.h:218` | `include/tensor_api/tensor/layout.h` | 获取 cosize | ✅ 已完成 |
| `rank` | `impl/tensor_api/tensor/layout_method.h:224` | `include/tensor_api/tensor/layout.h` | 获取 rank | ✅ 已完成 |
| `select` | `impl/tensor_api/tensor/layout_method.h:230` | `include/tensor_api/tensor/layout.h` | select 操作 | ✅ 已完成 |
| `get` (layout) | `impl/tensor_api/tensor/layout_method.h:236` | `include/tensor_api/tensor/layout.h` | 获取 layout 属性 | ✅ 已完成 |
| `size` | `impl/tensor_api/tensor/layout_method.h:242` | `include/tensor_api/tensor/layout.h` | 获取 size | ✅ 已完成 |
| `capacity` | `impl/tensor_api/tensor/layout_method.h:248` | `include/tensor_api/tensor/layout.h` | 获取 capacity | ✅ 已完成 |
| `slice` | `impl/tensor_api/tensor/layout_method.h:254` | `include/tensor_api/tensor/layout.h` | tensor 切片 | ✅ 已完成 |
| `crd2idx` (layout) | `impl/tensor_api/tensor/coord_index.h:80` | `include/tensor_api/tensor/layout.h` | 坐标转索引 | ✅ 已完成 |
| `get` (tuple, variadic) | `impl/tensor_api/tensor/tuple_impl.h:112` | `include/tensor_api/tensor/layout.h` | 获取 tuple 元素，支持单索引和多索引递归 | ✅ 已完成 |


### 1.5 Tensor/Pointer（tensor_impl.h / pointer_pattern.h）

| 标识符 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|---------|---------|------|------|
| `squeeze` | `impl/tensor_api/tensor/tensor_impl.h:377` | `include/tensor_api/tensor/tensor.h` | 挤压维度 | ✅ 已完成 |
| `make_mem_ptr` (3个重载) | `impl/tensor_api/tensor/pointer_pattern.h:57,64,70` | `include/tensor_api/tensor/pointer.h` | 创建内存指针 | ✅ 已完成 |
| `get_mem_location` | `impl/tensor_api/tensor/pointer_mem_impl.h:36` | `include/tensor_api/tensor/pointer.h` | 获取内存位置 | ⬜ |


### 1.6 常量与类型（constant_impl.h）

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `c0_element` | 变量模板 | `impl/tensor_api/utils/constant_impl.h:204` | `include/tensor_api/utils/utils.h` | C0 元素数 | ⬜ |
| `_0` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:32` | `include/tensor_api/utils/utils.h` | 零值常量 | ⬜ |
| `_1` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:33` | `include/tensor_api/utils/utils.h` | 整数常量 1 | ⬜ |
| `_2` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:34` | `include/tensor_api/utils/utils.h` | 整数常量 2 | ⬜ |
| `_3` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:35` | `include/tensor_api/utils/utils.h` | 整数常量 3 | ⬜ |
| `_4` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:36` | `include/tensor_api/utils/utils.h` | 整数常量 4 | ⬜ |
| `_5` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:37` | `include/tensor_api/utils/utils.h` | 整数常量 5 | ⬜ |
| `_6` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:38` | `include/tensor_api/utils/utils.h` | 整数常量 6 | ⬜ |
| `_7` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:39` | `include/tensor_api/utils/utils.h` | 整数常量 7 | ⬜ |
| `_8` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:40` | `include/tensor_api/utils/utils.h` | 整数常量 8 | ⬜ |
| `_9` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:41` | `include/tensor_api/utils/utils.h` | 整数常量 9 | ⬜ |
| `_10` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:42` | `include/tensor_api/utils/utils.h` | 整数常量 10 | ⬜ |
| `_16` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:43` | `include/tensor_api/utils/utils.h` | 整数常量 16 | ⬜ |
| `_24` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:44` | `include/tensor_api/utils/utils.h` | 整数常量 24 | ⬜ |
| `_32` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:45` | `include/tensor_api/utils/utils.h` | 整数常量 32 | ⬜ |
| `_64` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:46` | `include/tensor_api/utils/utils.h` | 整数常量 64 | ⬜ |
| `_128` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:47` | `include/tensor_api/utils/utils.h` | 整数常量 128 | ⬜ |
| `_256` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:48` | `include/tensor_api/utils/utils.h` | 整数常量 256 | ⬜ |
| `_512` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:49` | `include/tensor_api/utils/utils.h` | 整数常量 512 | ⬜ |
| `_1024` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:50` | `include/tensor_api/utils/utils.h` | 整数常量 1024 | ⬜ |
| `_2048` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:51` | `include/tensor_api/utils/utils.h` | 整数常量 2048 | ⬜ |
| `_4096` | 类型别名 | `impl/tensor_api/utils/constant_impl.h:52` | `include/tensor_api/utils/utils.h` | 整数常量 4096 | ⬜ |
| `keep_mark` | struct | `impl/tensor_api/utils/constant_impl.h:55` | `include/tensor_api/utils/utils.h` | 占位标记类型 | ⬜ |
| `_` | 常量 | `impl/tensor_api/utils/constant_impl.h:58` | `include/tensor_api/utils/utils.h` | 占位标记常量 | ⬜ |
| `location` 命名空间 | namespace | `impl/tensor_api/utils/constant_impl.h:89` | `include/tensor_api/utils/utils.h` | Location 类型定义 | ⬜ |
| `invalid` | struct | `impl/tensor_api/utils/constant_impl.h:90` | `include/tensor_api/utils/utils.h` | 无效 location 标记 | ⬜ |
| `gm` | struct | `impl/tensor_api/utils/constant_impl.h:91` | `include/tensor_api/utils/utils.h` | Global Memory location | ⬜ |
| `ub` | struct | `impl/tensor_api/utils/constant_impl.h:92` | `include/tensor_api/utils/utils.h` | Unified Buffer location | ⬜ |
| `l1` | struct | `impl/tensor_api/utils/constant_impl.h:93` | `include/tensor_api/utils/utils.h` | L1 location | ⬜ |
| `l0a` | struct | `impl/tensor_api/utils/constant_impl.h:94` | `include/tensor_api/utils/utils.h` | L0A location | ⬜ |
| `l0b` | struct | `impl/tensor_api/utils/constant_impl.h:95` | `include/tensor_api/utils/utils.h` | L0B location | ⬜ |
| `l0scalea` | struct | `impl/tensor_api/utils/constant_impl.h:96` | `include/tensor_api/utils/utils.h` | L0ScaleA location | ⬜ |
| `l0scaleb` | struct | `impl/tensor_api/utils/constant_impl.h:97` | `include/tensor_api/utils/utils.h` | L0ScaleB location | ⬜ |
| `l0c` | struct | `impl/tensor_api/utils/constant_impl.h:98` | `include/tensor_api/utils/utils.h` | L0C location | ⬜ |
| `bias` | struct | `impl/tensor_api/utils/constant_impl.h:99` | `include/tensor_api/utils/utils.h` | Bias location | ⬜ |
| `fixbuf` | struct | `impl/tensor_api/utils/constant_impl.h:100` | `include/tensor_api/utils/utils.h` | Fixbuf location | ⬜ |
| `ssbuf` | struct | `impl/tensor_api/utils/constant_impl.h:101` | `include/tensor_api/utils/utils.h` | SSBuf location | ⬜ |


### 1.7 Copy 操作类型与 Trait（arch/cube + arch/vector）

#### Cube Copy 操作

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `copy_gm_to_l1` | struct | `impl/tensor_api/arch/cube/gm_to_l1/copy.h:37` | `include/tensor_api/arch/cube/copy_op.h` | GM→L1 操作类型 | ✅ 已完成 |
| `copy_gm_to_l1_trait` | struct | `impl/tensor_api/arch/cube/gm_to_l1/copy_impl/instruction.h:32` | `include/tensor_api/arch/cube/copy_op.h` | GM→L1 trait | ✅ 已完成 |
| `copy_gm_to_l1_trait_default` | struct | `impl/tensor_api/arch/cube/gm_to_l1/copy.h:32` | `include/tensor_api/arch/cube/copy_op.h` | GM→L1 默认 trait | ✅ 已完成 |
| `copy_l1_to_ub` | struct | `impl/tensor_api/arch/cube/l1_to_ub/copy.h:39` | `include/tensor_api/arch/cube/copy_op.h` | L1→UB 操作类型 | ✅ 已完成 |
| `copy_l1_to_ub_trait` | struct | `impl/tensor_api/arch/cube/l1_to_ub/copy_impl/instruction.h:30` | `include/tensor_api/arch/cube/copy_op.h` | L1→UB trait | ✅ 已完成 |
| `copy_l1_to_ub_trait_default` | struct | `impl/tensor_api/arch/cube/l1_to_ub/copy.h:34` | `include/tensor_api/arch/cube/copy_op.h` | L1→UB 默认 trait | ✅ 已完成 |
| `copy_l1_to_l0a` | struct | `impl/tensor_api/arch/cube/l1_to_l0a/copy.h:37` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0A 操作类型（base_tensor） | ✅ 已完成 |
| `copy_l1_to_l0a_trait` | struct | `impl/tensor_api/arch/cube/l1_to_l0a/copy_impl/instruction.h:30` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0A trait | ✅ 已完成 |
| `copy_l1_to_l0a_trait_default` | struct | `impl/tensor_api/arch/cube/l1_to_l0a/copy.h:32` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0A 默认 trait | ✅ 已完成 |
| `copy_l1_to_l0b` | struct | `impl/tensor_api/arch/cube/l1_to_l0b/copy.h:37` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0B 操作类型 | ✅ 已完成 |
| `copy_l1_to_l0b_trait` | struct | `impl/tensor_api/arch/cube/l1_to_l0b/copy_impl/instruction.h:30` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0B trait | ✅ 已完成 |
| `copy_l1_to_l0b_trait_default` | struct | `impl/tensor_api/arch/cube/l1_to_l0b/copy.h:32` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0B 默认 trait | ✅ 已完成 |
| `copy_l1_to_l0scalea` | struct | `impl/tensor_api/arch/cube/l1_to_l0scalea/copy.h:37` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0ScaleA 操作类型 | ✅ 已完成 |
| `copy_l1_to_l0scalea_trait` | struct | `impl/tensor_api/arch/cube/l1_to_l0scalea/copy_impl/instruction.h:30` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0ScaleA trait | ✅ 已完成 |
| `copy_l1_to_l0scalea_trait_default` | struct | `impl/tensor_api/arch/cube/l1_to_l0scalea/copy.h:32` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0ScaleA 默认 trait | ✅ 已完成 |
| `copy_l1_to_l0scaleb` | struct | `impl/tensor_api/arch/cube/l1_to_l0scaleb/copy.h:37` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0ScaleB 操作类型 | ✅ 已完成 |
| `copy_l1_to_l0scaleb_trait` | struct | `impl/tensor_api/arch/cube/l1_to_l0scaleb/copy_impl/instruction.h:30` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0ScaleB trait | ✅ 已完成 |
| `copy_l1_to_l0scaleb_trait_default` | struct | `impl/tensor_api/arch/cube/l1_to_l0scaleb/copy.h:32` | `include/tensor_api/arch/cube/copy_op.h` | L1→L0ScaleB 默认 trait | ✅ 已完成 |
| `copy_l1_to_fixbuf` | struct | `impl/tensor_api/arch/cube/l1_to_fb/copy.h:37` | `include/tensor_api/arch/cube/copy_op.h` | L1→Fixbuf 操作类型 | ✅ 已完成 |
| `copy_l1_to_fixbuf_trait` | struct | `impl/tensor_api/arch/cube/l1_to_fb/copy_impl/instruction.h:30` | `include/tensor_api/arch/cube/copy_op.h` | L1→Fixbuf trait | ✅ 已完成 |
| `copy_l1_to_fixbuf_trait_default` | struct | `impl/tensor_api/arch/cube/l1_to_fb/copy.h:32` | `include/tensor_api/arch/cube/copy_op.h` | L1→Fixbuf 默认 trait | ✅ 已完成 |
| `copy_l1_to_biastable` | struct | `impl/tensor_api/arch/cube/l1_to_bt/copy.h:37` | `include/tensor_api/arch/cube/copy_op.h` | L1→Biastable 操作类型 | ✅ 已完成 |
| `copy_l1_to_biastable_trait` | struct | `impl/tensor_api/arch/cube/l1_to_bt/copy_impl/instruction.h:30` | `include/tensor_api/arch/cube/copy_op.h` | L1→Biastable trait | ✅ 已完成 |
| `copy_l1_to_biastable_trait_default` | struct | `impl/tensor_api/arch/cube/l1_to_bt/copy.h:32` | `include/tensor_api/arch/cube/copy_op.h` | L1→Biastable 默认 trait | ✅ 已完成 |
| `copy_l0c_to_gm` | struct | `impl/tensor_api/arch/cube/l0c_to_gm/copy.h:39` | `include/tensor_api/arch/cube/copy_op.h` | L0C→GM 操作类型（base_tensor） | ✅ 已完成 |
| `copy_l0c_to_gm_trait` | struct | `impl/tensor_api/arch/cube/l0c_to_gm/copy_impl/data_copy.h:33` | `include/tensor_api/arch/cube/copy_op.h` | L0C→GM trait | ✅ 已完成 |
| `copy_l0c_to_gm_trait_default` | struct | `impl/tensor_api/arch/cube/l0c_to_gm/copy.h:34` | `include/tensor_api/arch/cube/copy_op.h` | L0C→GM 默认 trait | ✅ 已完成 |
| `copy_l0c_to_ub` | struct | `impl/tensor_api/arch/cube/l0c_to_ub/copy.h:39` | `include/tensor_api/arch/cube/copy_op.h` | L0C→UB 操作类型（base_tensor） | ✅ 已完成 |
| `copy_l0c_to_ub_trait` | struct | `impl/tensor_api/arch/cube/l0c_to_ub/copy_impl/data_copy.h:33` | `include/tensor_api/arch/cube/copy_op.h` | L0C→UB trait | ✅ 已完成 |
| `copy_l0c_to_ub_trait_default` | struct | `impl/tensor_api/arch/cube/l0c_to_ub/copy.h:34` | `include/tensor_api/arch/cube/copy_op.h` | L0C→UB 默认 trait | ✅ 已完成 |
| `copy_l0c_to_l1` | struct | `impl/tensor_api/arch/cube/l0c_to_l1/copy.h:39` | `include/tensor_api/arch/cube/copy_op.h` | L0C→L1 操作类型（base_tensor） | ✅ 已完成 |
| `copy_l0c_to_l1_trait` | struct | `impl/tensor_api/arch/cube/l0c_to_l1/copy_impl/data_copy.h:33` | `include/tensor_api/arch/cube/copy_op.h` | L0C→L1 trait | ✅ 已完成 |
| `copy_l0c_to_l1_trait_default` | struct | `impl/tensor_api/arch/cube/l0c_to_l1/copy.h:34` | `include/tensor_api/arch/cube/copy_op.h` | L0C→L1 默认 trait | ✅ 已完成 |


#### Vector Copy 操作

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `copy_gm_to_ub` | struct | `impl/tensor_api/arch/vector/gm_to_ub/copy.h:41` | `include/tensor_api/arch/vector/copy_op.h` | GM→UB 操作类型（base_tensor） | ✅ 已完成 |
| `copy_gm_to_ub_trait` | struct | `impl/tensor_api/arch/vector/gm_to_ub/copy_impl/data_copy.h:32` | `include/tensor_api/arch/vector/copy_op.h` | GM→UB trait | ✅ 已完成 |
| `copy_gm_to_ub_trait_default` | struct | `impl/tensor_api/arch/vector/gm_to_ub/copy.h:34` | `include/tensor_api/arch/vector/copy_op.h` | GM→UB 默认 trait | ✅ 已完成 |
| `copy_ub_to_gm` | struct | `impl/tensor_api/arch/vector/ub_to_gm/copy.h:38` | `include/tensor_api/arch/vector/copy_op.h` | UB→GM 操作类型 | ✅ 已完成 |
| `copy_ub_to_gm_trait` | struct | `impl/tensor_api/arch/vector/ub_to_gm/copy_impl/data_copy.h:32` | `include/tensor_api/arch/vector/copy_op.h` | UB→GM trait | ✅ 已完成 |
| `copy_ub_to_gm_trait_default` | struct | `impl/tensor_api/arch/vector/ub_to_gm/copy.h:33` | `include/tensor_api/arch/vector/copy_op.h` | UB→GM 默认 trait | ✅ 已完成 |
| `copy_ub_to_l1` | struct | `impl/tensor_api/arch/vector/ub_to_l1/copy.h:38` | `include/tensor_api/arch/vector/copy_op.h` | UB→L1 操作类型 | ✅ 已完成 |
| `copy_ub_to_l1_trait` | struct | `impl/tensor_api/arch/vector/ub_to_l1/copy_impl/data_copy.h:31` | `include/tensor_api/arch/vector/copy_op.h` | UB→L1 trait | ✅ 已完成 |
| `copy_ub_to_l1_trait_default` | struct | `impl/tensor_api/arch/vector/ub_to_l1/copy.h:33` | `include/tensor_api/arch/vector/copy_op.h` | UB→L1 默认 trait | ✅ 已完成 |
| `copy_ub_to_ub` | struct | `impl/tensor_api/arch/vector/ub_to_ub/copy.h:39` | `include/tensor_api/arch/vector/copy_op.h` | UB→UB 操作类型 | ✅ 已完成 |
| `copy_ub_to_ub_trait` | struct | `impl/tensor_api/arch/vector/ub_to_ub/copy_impl/data_copy.h:32` | `include/tensor_api/arch/vector/copy_op.h` | UB→UB trait | ✅ 已完成 |
| `copy_ub_to_ub_trait_default` | struct | `impl/tensor_api/arch/vector/ub_to_ub/copy.h:34` | `include/tensor_api/arch/vector/copy_op.h` | UB→UB 默认 trait | ✅ 已完成 |


### 1.8 Mmad 操作类型（arch/cube/mmad）

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `mmad_operation` | struct | `impl/tensor_api/arch/cube/mmad/mmad.h:41` | `include/tensor_api/arch/cube/mmad_op.h` | Mmad 操作类型 | ✅ 已完成 |
| `mmad_trait_default` | struct | `impl/tensor_api/arch/cube/mmad/mmad.h:36` | `include/tensor_api/arch/cube/mmad_op.h` | Mmad 默认 trait | ✅ 已完成 |
| `mmad_traits` | struct | `impl/tensor_api/atom/mmad_traits_impl.h:31` | `include/tensor_api/atom/cube/mmad_atom.h` | Mmad traits 主模板 | ✅ 已完成 |
| `mmad_atom` | struct | `impl/tensor_api/atom/mmad_atom_impl.h:32` | `include/tensor_api/atom/cube/mmad_atom.h` | Mmad atom 主模板 | ✅ 已完成 |
| `copy_traits` | struct | `impl/tensor_api/atom/copy_traits_impl.h:31` | `include/tensor_api/atom/cube/copy_atom.h` | Copy traits 主模板 | ✅ 已完成 |
| `copy_atom` | struct | `impl/tensor_api/atom/copy_atom_impl.h:49` | `include/tensor_api/atom/cube/copy_atom.h` | Copy atom 主模板 | ✅ 已完成 |


### 1.9 C_API 辅助函数（instruction.h）

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `set_mte2_nz_para` | 函数 | `impl/tensor_api/arch/cube/gm_to_l1/copy_impl/instruction.h:34` | `include/tensor_api/arch/cube/copy_op.h` | 算子直接使用 c_api，设置 MTE2 NZ 参数 | ⬜ |
| `get_cache_mode` | 函数 | `impl/tensor_api/tensor/engine_impl.h:52` | `include/tensor_api/tensor/tensor.h` | 获取 cache mode（view_engine 成员函数） | ⬜ |
| `make_batch_pattern_layout` | 函数 | `impl/tensor_api/tensor/layout_pattern.h:53` | 无需对外 | Batch Pattern Layout内部辅助函数 | ✅ 已确认（保留内部实现，不对外展示） |

### 1.11 Tensor 结构体（tensor_impl.h / engine_impl.h）

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `base_tensor` | struct | `impl/tensor_api/tensor/tensor_impl.h:45` | `include/tensor_api/tensor/tensor.h` | Tensor 基类（含 element_type/layout_type/engine_type） | ⬜ |
| `global_tensor` | struct | `impl/tensor_api/tensor/tensor_impl.h:271` | `include/tensor_api/tensor/tensor.h` | Global Memory tensor | ⬜ |
| `local_tensor` | struct | `impl/tensor_api/tensor/tensor_impl.h:293` | `include/tensor_api/tensor/tensor.h` | Local Memory tensor | ⬜ |
| `make_tensor_result` | struct | `impl/tensor_api/tensor/tensor_impl.h:303` | `include/tensor_api/tensor/tensor.h` | make_tensor 返回类型选择 | ⬜ |
| `make_tensor` | 函数 | `impl/tensor_api/tensor/tensor_impl.h:348` | `include/tensor_api/tensor/tensor.h` | 创建 tensor | ✅ 已完成 |
| `squeeze` | 函数 | `impl/tensor_api/tensor/tensor_impl.h:377` | `include/tensor_api/tensor/tensor.h` | 挤压维度 | ✅ 已完成 |

### 1.12 Layout 结构体（layout_definition.h / layout_frame.h）

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `layout` | struct | `impl/tensor_api/tensor/layout_definition.h:46` | `include/tensor_api/tensor/layout.h` | Layout 主结构体 | ✅ 已完成 |
| `shape` | 类型别名 | `impl/tensor_api/tensor/layout_definition.h:37` | `include/tensor_api/tensor/layout.h` | Shape 类型 | ✅ 已完成 |
| `stride` | 类型别名 | `impl/tensor_api/tensor/layout_definition.h:40` | `include/tensor_api/tensor/layout.h` | Stride 类型 | ✅ 已完成 |
| `coord` | 类型别名 | `impl/tensor_api/tensor/layout_definition.h:43` | `include/tensor_api/tensor/layout.h` | Coord 类型 | ✅ 已完成 |
| `get_layout_pattern` | 类型别名 | `impl/tensor_api/tensor/layout_definition.h:203` | `include/tensor_api/tensor/layout.h` | 获取 layout pattern | ✅ 已完成 |
| `is_layout` | struct | `impl/tensor_api/tensor/layout_definition.h:205` | `include/tensor_api/tensor/layout.h` | layout 类型判断 trait | ✅ 已完成 |
| `is_layout_v` | 变量模板 | `impl/tensor_api/tensor/layout_definition.h:212` | `include/tensor_api/tensor/layout.h` | is_layout 的 _v 版本 | ✅ 已完成 |
| `layout_trait` | struct | `impl/tensor_api/tensor/layout_frame.h:43` | `include/tensor_api/tensor/layout.h` | Layout trait 结构体 | ⬜ |
| `layout_trait_default` | struct | `impl/tensor_api/tensor/layout_frame.h:48` | `include/tensor_api/tensor/layout.h` | 默认 layout trait | ⬜ |
| `frame_layout_format` | struct | `impl/tensor_api/tensor/layout_frame.h:190` | `include/tensor_api/tensor/layout.h` | Frame layout format 结构体 | ⬜ |


### 1.13 Pointer 结构体（pointer_pattern.h / pointer_mem_impl.h）

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `make_mem_ptr` (3个重载) | 函数 | `impl/tensor_api/tensor/pointer_pattern.h:57,64,70` | `include/tensor_api/tensor/pointer.h` | 创建内存指针 | ✅ 已完成 |
| `get_mem_location` | 类型别名 | `impl/tensor_api/tensor/pointer_mem_impl.h:36` | `include/tensor_api/tensor/pointer.h` | 获取内存位置 | ⬜ |


### 1.14 Layout Pattern 结构体（layout_pattern.h）

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `zn_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:31` | `include/tensor_api/tensor/layout_type.h` | ZN layout pattern | ✅ 已完成 |
| `zz_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:32` | `include/tensor_api/tensor/layout_type.h` | ZZ layout pattern | ✅ 已完成 |
| `nn_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:33` | `include/tensor_api/tensor/layout_type.h` | NN layout pattern | ✅ 已完成 |
| `nz_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:34` | `include/tensor_api/tensor/layout_type.h` | NZ layout pattern | ✅ 已完成 |
| `nd_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:35` | `include/tensor_api/tensor/layout_type.h` | ND layout pattern | ✅ 已完成 |
| `dn_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:36` | `include/tensor_api/tensor/layout_type.h` | DN layout pattern | ✅ 已完成 |
| `nd_ext_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:37` | `include/tensor_api/tensor/layout_type.h` | ND Ext layout pattern | ✅ 已完成 |
| `dn_ext_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:38` | `include/tensor_api/tensor/layout_type.h` | DN Ext layout pattern | ✅ 已完成 |
| `scalea_nd_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:39` | `include/tensor_api/tensor/layout_type.h` | ScaleA ND layout pattern | ✅ 已完成 |
| `scalea_dn_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:40` | `include/tensor_api/tensor/layout_type.h` | ScaleA DN layout pattern | ✅ 已完成 |
| `scaleb_nd_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:41` | `include/tensor_api/tensor/layout_type.h` | ScaleB ND layout pattern | ✅ 已完成 |
| `scaleb_dn_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:42` | `include/tensor_api/tensor/layout_type.h` | ScaleB DN layout pattern | ✅ 已完成 |
| `nchw_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:259` | `include/tensor_api/tensor/layout_type.h` | NCHW layout pattern | ✅ 已完成 |
| `nhwc_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:260` | `include/tensor_api/tensor/layout_type.h` | NHWC layout pattern | ✅ 已完成 |
| `nc1hwc0_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:261` | `include/tensor_api/tensor/layout_type.h` | NC1HWC0 layout pattern | ✅ 已完成 |
| `ncdhw_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:271` | `include/tensor_api/tensor/layout_type.h` | NCDHW layout pattern | ✅ 已完成 |
| `ndc1hwc0_layout_ptn` | struct | `impl/tensor_api/tensor/layout_pattern.h:272` | `include/tensor_api/tensor/layout_type.h` | NDC1HWC0 layout pattern | ✅ 已完成 |
| `make_pattern_layout` | 函数 | `impl/tensor_api/tensor/layout_pattern.h:45` | `include/tensor_api/tensor/layout_interface.h` | 创建 pattern layout | ✅ 已完成 |

## 二、需要删除的接口

### 2.0 对外暂定（待确认）

| 标识符 | 类型 | 当前位置 | 目标位置 | 说明 | 状态 |
|--------|------|---------|---------|------|------|
| `iter_adaptor` | struct | `impl/tensor_api/tensor/pointer_adaptor_impl.h` | `include/tensor_api/tensor/pointer.h`（暂定） | 迭代器适配器，是否对外暂定 | ⬜ |


### 2.1 类型/函数删除

| 标识符 | 当前位置 | 说明 | 状态 |
|--------|---------|------|------|
| `tile_type` | `impl/tensor_api/tensor/layout_definition.h:43` | Tile 类型，不再使用 | ✅ 已完成 |
| `make_tile` | `impl/tensor_api/tensor/layout_method.h:48` | Tile 创建函数，不再使用 | ✅ 已完成 |
| `layout_trait_scale` | `impl/tensor_api/tensor/layout_frame.h:50` | Scale trait，内部实现 | ✅ 已完成 |
| `layout_trait_fp4` | `impl/tensor_api/tensor/layout_frame.h:52` | FP4 trait，内部实现 | ✅ 已完成 |
| `get_shape` (非const引用) | `impl/tensor_api/tensor/layout_method.h:168` | 非 const 重载，保留 const 版本即可 | ✅ 已完成 |
| `get_stride` (非const引用) | `impl/tensor_api/tensor/layout_method.h:180` | 非 const 重载，保留 const 版本即可 | ✅ 已完成 |
| `get` (整个tuple) | `impl/tensor_api/tensor/tuple_impl.h:128` | 获取整个 tuple 的重载 | ✅ 已完成 |
| `copy_traits::with()` (无参) | `impl/tensor_api/atom/copy_traits_impl.h:39` | 无参 with 方法 | ✅ 已完成 |
| `mmad_traits::with()` (无参) | `impl/tensor_api/atom/mmad_traits_impl.h:38` | 无参 with 方法 | ✅ 已完成 |
| `using Std::Int` | `impl/tensor_api/utils/constant_impl.h:31` | Std::Int using 声明 | ✅ 已完成 |
| `view_engine` | `impl/tensor_api/tensor/engine_impl.h:31` | 资料删除，内部实现 | ✅ 已确认（保留内部实现，不对外展示） |


### 2.2 整个文件删除

| 文件 | 说明 | 状态 |
|------|------|------|
| `impl/tensor_api/utils/macro_impl.h` | 保留文件 | ✅ 已确认保留 |

## 三、已完成的重构

| 改动 | 文件 | 说明 | 状态 |
|------|------|------|------|
| `layout_type::rank` 成员变量 → `rank_size` | `impl/tensor_api/tensor/layout_definition.h` | 避免和成员函数 `rank()` 冲突 | ✅ 已完成 |
| `layout_type::get_rank()` → `rank()` | `impl/tensor_api/tensor/layout_definition.h` | 成员函数改名，去掉 get_ 前缀 | ✅ 已完成 |
| `layout_method.h` 中 `get_rank` 调用 → `rank` | `impl/tensor_api/tensor/layout_method.h` | 同步更新调用方 | ✅ 已完成 |
| 所有 `::rank` 引用 → `::rank_size` | `impl/tensor_api/` + `tests/`（37处） | 同步更新所有引用 | ✅ 已完成 |
| `global_tensor` 补充 `get_cache_mode()` | `impl/tensor_api/tensor/tensor_impl.h` | global_tensor 结构体中补充 get_cache_mode 成员函数，转发到 engine().get_cache_mode() | ✅ 已完成 |
| `get` 单索引/多索引重载合并 | `impl/tensor_api/tensor/tuple_impl.h` | 两个 get 重载合并为一个 variadic 模板，通过 `if constexpr (sizeof...(Is) == 0)` 区分 | ✅ 已完成 |

## 四、汇总统计

| 类别 | 数量 |
|------|------|
| 放出去的类型别名 | 7 |
| 放出去的 struct 结构体 | 78 |
| 放出去的函数 | 18 |
| 放出去的变量模板/常量 | 5 |
| 放出去的 namespace | 1 |
| **放出去合计** | **130** |
| 当前已完成（按唯一标识符统计） | 76 |
| 当前待完成（按唯一标识符统计） | 69 |
| 对外暂定 | 1 |
| 删除的类型/函数 | 10 |
| 仅资料不对外展示（保留内部实现） | 1 |
| 删除的整个文件 | 1 |
| **已处理合计** | **12** |

struct arch_version {       // 暂定命名v3510或V3510
    static constexpr uint32_t v3510 = 3510;
    static constexpr uint32_t v2201 = 2201;
    static constexpr uint32_t V3510 = 3510;
    static constexpr uint32_t V2201 = 2201;
};
