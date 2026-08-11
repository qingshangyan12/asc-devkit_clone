# Tensor API Copy/Mmad 函数参数与成员变量统计

## 〇、目录结构与头文件清单

### 对外公共头文件（include/tensor_api/）

| 文件 | 职责 | 主要内容 |
|------|------|----------|
| `tensor.h` | 总入口 | 聚合 include 所有公共头 + legacy 兼容层 |
| `algorithm/copy.h` | Copy 公共接口 | `copy()` / `make_copy()` 前向声明 |
| `algorithm/mmad.h` | Mmad 公共接口 | `mmad()` / `make_mmad()` 前向声明 |
| `arch/arch.h` | 架构入口 | 聚合 include cube + vector arch |
| `arch/cube/copy_op.h` | Cube Copy 操作类型 | `copy_gm_to_l1` / `copy_l0c_to_gm` 等 struct 前向声明 + trait 前向声明 |
| `arch/cube/mmad_op.h` | Cube Mmad 操作类型 | `mmad_operation` / `mmad_op_with` 前向声明 |
| `arch/vector/copy_op.h` | Vector Copy 操作类型 | `copy_gm_to_ub` / `copy_ub_to_gm` 等 struct 前向声明 + trait 前向声明 |
| `atom/atom.h` | Atom 入口 | 聚合 include copy_atom + mmad_atom |
| `atom/cube/copy_atom.h` | Copy Atom 公共类型 | `copy_atom` / `copy_traits` 前向声明 |
| `atom/cube/mmad_atom.h` | Mmad Atom 公共类型 | `mmad_atom` / `mmad_traits` 前向声明 |
| `tensor/layout.h` | Layout 公共接口 | `make_shape` / `make_stride` / `make_coord` / `make_layout` / `get` / `size` / `capacity` / `rank` / `select` / `cosize` / `coshape` / `slice` / `squeeze` / `make_frame_layout` / `crd2idx` |
| `tensor/pointer.h` | Pointer 公共接口 | `make_mem_ptr` 前向声明 |
| `tensor/tensor.h` | Tensor 公共接口 | `make_tensor` 前向声明 |
| `utils/utils.h` | 参数结构体 + 枚举 | `cache_mode` / `mmad_type` / `round_mode` / `dual_dst_mode` / `mmad_trait` / `mmad_params` / `fixpipe_params` / `copy_gm_to_ub_params` / `img2col_params` |

### 对内实现头文件（impl/tensor_api/）

| 目录 | 职责 | 主要文件 |
|------|------|----------|
| `algorithm/` | 算法分发层 | `copy_impl.h`（copy/make_copy 实现 + copy_trait_adapter + normalize_copy_trait）<br>`mmad_impl.h`（mmad/make_mmad 实现） |
| `arch/cube/gm_to_l1/` | GM→L1 通路 | `copy.h`（入口类）<br>`routing.h`（路由分发）<br>`copy_impl/instruction.h`（c_api 封装 + trait 定义）<br>`copy_impl/dn2nz.h` / `nd2nz.h` / `nd2nd.h` / `nz2nz.h` / `zn2zn.h` 等（格式转换实现）<br>`copy_impl/nc1hwc02nc1hwc0.h` / `nchw2nc1hwc0.h` / `nhwc2nc1hwc0.h` / `ncdhw2ndc1hwc0.h`（卷积通路）<br>`copy_impl/scalea_*.h` / `scaleb_*.h`（Scale 通路） |
| `arch/cube/l0c_to_gm/` | L0C→GM 通路 | `copy.h`（入口类 + trait 定义）<br>`routing.h`（路由分发）<br>`copy_impl/instruction.h`（c_api 封装）<br>`copy_impl/data_copy.h`（数据搬运实现）<br>`copy_impl/nz2nchw.h` / `nz2nhwc.h` / `nz2ncdhw.h`（卷积通路） |
| `arch/cube/l0c_to_ub/` | L0C→UB 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` / `copy_impl/data_copy.h` |
| `arch/cube/l0c_to_l1/` | L0C→L1 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` / `copy_impl/data_copy.h` |
| `arch/cube/l1_to_ub/` | L1→UB 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` |
| `arch/cube/l1_to_l0a/` | L1→L0A 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h`（含 img2col）<br>`copy_impl/zn2nz.h` / `nz2nz.h` / `img2col.h` / `img2col3d.h` / `zn2nzb8b4.h` |
| `arch/cube/l1_to_l0b/` | L1→L0B 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h`<br>`copy_impl/zn2zn.h` / `nz2zn.h` / `nz2znb8b4.h` |
| `arch/cube/l1_to_l0scalea/` | L1→L0ScaleA 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` / `copy_impl/scalea.h` |
| `arch/cube/l1_to_l0scaleb/` | L1→L0ScaleB 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` / `copy_impl/scaleb.h` |
| `arch/cube/l1_to_fb/` | L1→Fixbuf 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` |
| `arch/cube/l1_to_bt/` | L1→Biastable 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` / `copy_impl/nd2nd.h` |
| `arch/cube/mmad/` | Mmad 通路 | `mmad.h`（入口类 + trait 定义）<br>`mmad_impl/instruction.h`（c_api 封装）<br>`mmad_impl/mmad.h`（普通 Mmad）<br>`mmad_impl/mmad_with_bias.h`（带 Bias Mmad）<br>`routing.h`（路由分发） |
| `arch/cube/utils/` | Cube 工具 | `l0c2out_utils.h`（set_register / alloc_fixbuf_temp_buf / set_fpc / insert_sync / get_quant_mode） |
| `arch/vector/gm_to_ub/` | GM→UB 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` / `copy_impl/data_copy.h`<br>`copy_impl/dn2dn.h` / `nd2nd.h` / `nz2nz.h` / `zn2zn.h` |
| `arch/vector/ub_to_gm/` | UB→GM 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h` / `copy_impl/data_copy.h`<br>`copy_impl/dn2dn.h` / `nd2nd.h` / `nz2nz.h` |
| `arch/vector/ub_to_l1/` | UB→L1 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h`<br>`copy_impl/dn2dn.h` / `nd2nd.h` / `nz2nz.h` / `zn2zn.h` |
| `arch/vector/ub_to_ub/` | UB→UB 通路 | `copy.h` / `routing.h` / `copy_impl/instruction.h`<br>`copy_impl/dn2dn.h` / `nd2nd.h` / `nz2nz.h` / `zn2zn.h` |
| `arch/utils/` | Arch 工具 | `arch_utils.h`（get_element / get_total_column_shape / get_total_row_shape）<br>`check_data_type.h`（数据类型校验）<br>`check_format.h`（格式校验）<br>`is_format.h`（格式判断 + attr_info 命名空间） |
| `atom/` | Atom 实现 | `copy_atom_impl.h`（copy_atom 特化 + call/with）<br>`copy_traits_impl.h`（copy_traits 特化 + copy_unpack/with）<br>`mmad_atom_impl.h`（mmad_atom 特化 + call/with）<br>`mmad_traits_impl.h`（mmad_traits 特化 + mmad_unpack/with）<br>`cube/copy_gm2l1.h` / `copy_l0c2gm.h` / `copy_l12l0a.h` 等（各通路 trait 绑定）<br>`vector/copy_gm2ub.h` / `copy_ub2gm.h` 等（Vector 通路 trait 绑定） |
| `experimental/` | 实验性功能 | `reg/load_mode.h` / `reg/reg_tensor_impl.h` / `reg/mask_impl.h`（寄存器 tensor） |
| `legacy/` | PascalCase 兼容层 | `legacy_utils.h`（PascalCase 类型 + normalize 函数）<br>`legacy_type.h`（类型别名）<br>`legacy_atom.h`（CopyTraits/CopyAtom/MmadTraits/MmadAtom + normalize_copy_trait + has_copy_method）<br>`legacy.h`（PascalCase wrapper 函数） |
| `tensor/` | Tensor/Layout 实现 | `tensor_impl.h`（base_tensor/global_tensor/local_tensor）<br>`engine_impl.h`（view_engine/const_view_engine）<br>`layout_definition.h`（layout_type/shape_type/stride_type/coord_type/tile_type）<br>`layout_frame.h`（frame_layout_format/layout_trait）<br>`layout_pattern.h`（各 layout pattern）<br>`layout_method.h`（get_shape/get_stride/coshape/compute_stride 等）<br>`layout_impl.h`（make_coord_layout/make_slice_layout 等）<br>`layout_infer.h`（layout 推导）<br>`layout_fractal.h`（fractal layout）<br>`layout_size.h`（layout size 计算）<br>`pointer_pattern.h`（pointer pattern）<br>`pointer_mem_impl.h`（hardware_mem_ptr）<br>`pointer_adaptor_impl.h`（iter_adaptor）<br>`coord_index.h`（crd2idx）<br>`tuple_impl.h`（tuple helper） |
| `utils/` | 工具实现 | `utils_impl.h`（normalize 函数模板 + is_quant/is_copy_quant）<br>`constant_impl.h`（location/copy_mode/arch_version/C0_ELEMENT 等）<br>`location_impl.h`（location 类型映射）<br>`map_impl.h`（tuple_map）<br>`extra_impl.h`（void_t/get_trait_member_type/iter_ele/iter_ref/iter_val）<br>`macro_impl.h`（宏定义） |

## 〇.一、对外公共模板参数

### Copy 公共接口模板参数

| 函数/类 | 模板参数 | 说明 |
|---------|---------|------|
| `copy()` | `typename AtomType, typename DstTensor, typename SrcTensor` | 基本拷贝 |
| `copy()` (quant) | `typename AtomType, typename DstTensor, typename SrcTensor, typename QuantParam, enable_if_t<is_copy_quant_param_v<QuantParam>, int>` | 带 quant 拷贝 |
| `make_copy()` | `typename CopyOperationType` | 创建 atom（无 trait） |
| `make_copy()` | `typename CopyOperationType, typename CopyTraitType` | 创建 atom（带 trait） |
| `copy_atom<CopyOperation>` | `typename CopyOperation` | atom 1参数特化 |
| `copy_traits<Args...>` | `typename... Args` | traits variadic 主模板 |

### Mmad 公共接口模板参数

| 函数/类 | 模板参数 | 说明 |
|---------|---------|------|
| `mmad()` | `typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor` | 基本矩阵乘 |
| `mmad()` (bias) | `typename AtomType, typename DstTensor, typename FmTensor, typename FilterTensor, typename BiasTensor, enable_if_t<is_attr_tensor_v<BiasTensor>, int>` | 带 bias 矩阵乘 |
| `make_mmad()` | `typename MmadOperationType` | 创建 atom（无 trait） |
| `make_mmad()` | `typename MmadOperationType, typename MmadTraitType` | 创建 atom（带 trait） |
| `mmad_atom<MmadOperationType>` | `typename MmadOperationType` | atom 1参数特化 |
| `mmad_traits<MmadOperationType, Args...>` | `typename MmadOperationType, typename... MmadOpArgs` | traits variadic 主模板 |

### Layout/Pointer/Tensor 公共接口模板参数

| 函数/类 | 模板参数 | 说明 |
|---------|---------|------|
| `make_shape()` | `typename... Ts` | 创建 shape |
| `make_stride()` | `typename... Ts` | 创建 stride |
| `make_coord()` | `typename... Ts` | 创建 coord |
| `make_layout()` | `typename T, typename U` | 创建 layout（shape + stride） |
| `make_layout()` | `typename ShapeType` | 创建 layout（仅 shape） |
| `crd2idx()` | `typename T, typename U, typename S` | 坐标转索引（coord + shape + stride） |
| `crd2idx()` | `typename T, typename LayoutType, enable_if_t<is_layout_v<LayoutType>>` | 坐标转索引（coord + layout） |
| `get()` | `size_t I, typename Tuple, enable_if_t<is_tuple_v<...>>` | 获取 tuple 元素 |
| `get()` | `size_t I0, size_t I1, size_t... Is, typename Tuple, enable_if_t<...>` | 多级获取 |
| `get()` | `typename Tuple, enable_if_t<...>` | 获取整个 tuple |
| `get()` (layout) | `size_t... Is, typename LayoutType, enable_if_t<is_layout_v<...>>` | 获取 layout 属性 |
| `size()` | `size_t... Is, typename LayoutType, enable_if_t<is_layout_v<...>>` | layout size |
| `capacity()` | `size_t... Is, typename LayoutType, enable_if_t<is_layout_v<...>>` | layout capacity |
| `rank()` | `size_t... Is, typename LayoutType, enable_if_t<is_layout_v<...>>` | layout rank |
| `select()` | `size_t... Is, typename LayoutType, enable_if_t<is_layout_v<...>>` | layout select |
| `coshape()` | `size_t... Is, typename LayoutType, enable_if_t<is_layout_v<...>>` | layout coshape |
| `cosize()` | `size_t... Is, typename LayoutType, enable_if_t<is_layout_v<...>>` | layout cosize |
| `slice()` | `typename Tensor, typename CoordType, typename Info` | tensor 切片 |
| `squeeze()` | `size_t... SqueezeDims, typename T, enable_if_t<...>` | 挤压维度 |
| `squeeze()` | `typename Pattern, typename T, enable_if_t<...>` | 按模式挤压 |
| `make_frame_layout()` | `typename LayoutPattern, typename TraitType, typename... Args` | 创建 frame layout |
| `make_frame_layout()` | `typename LayoutPattern, size_t C0Element, typename... Args` | 创建 frame layout（指定 C0） |
| `make_mem_ptr()` | `typename PtrPattern, typename DataType, typename Addr, enable_make_ptr_by_trait<...>` | 按 trait 创建指针 |
| `make_mem_ptr()` | `typename PtrPattern, typename Iterator, enable_make_hardware_ptr<...>` | 按硬件创建指针 |
| `make_mem_ptr()` | `typename Iterator, enable_make_ptr_by_iter<...>` | 按迭代器创建指针 |
| `make_tensor()` | `typename Iterator, typename... Args` | 创建 tensor |

## 〇.二、对内实现模板参数

### Copy 通路 instruction 层模板参数

| 通路 | 模板参数 | 说明 |
|------|---------|------|
| GM→L1 (align) | `typename T` | 数据类型 |
| GM→L1 (nd2nz/dn2nz) | `typename T, typename U, typename... Params` | 数据类型 + tensor + 参数 |
| L0C→GM | `QuantMode_t quant_pre, typename T, typename U, typename... Params` | 量化模式 + tensor + 参数 |
| L0C→GM (batch) | `const copy_l0c_to_gm_trait& trait, QuantMode_t quant_pre, typename T, typename U, typename V` | trait + 量化 + tensor |
| L0C→UB | `QuantMode_t quant_pre, typename T, typename U, typename... Params` | 量化模式 + tensor + 参数 |
| L0C→L1 | `QuantMode_t quant_pre, typename T, typename U, typename... Params` | 量化模式 + tensor + 参数 |
| L1→L0A | `bool transpose, typename T, typename U, typename... Params` | 转置 + tensor + 参数 |
| L1→L0A (img2col) | `typename T, typename U, typename... Params` | tensor + 参数 |
| L1→L0B | `bool transpose, typename T, typename U, typename... Params` | 转置 + tensor + 参数 |
| L1→L0ScaleA/B | `typename U, typename... Params` | tensor + 参数 |
| L1→UB/Fixbuf/Biastable | `typename T, typename U, typename... Params` | tensor + 参数 |
| GM→UB/UB→GM/UB→L1/UB→UB | `typename T, typename U, typename... Params` | tensor + 参数 |

### Copy 通路 copy.h 入口层模板参数

| 通路 | 模板参数 | 说明 |
|------|---------|------|
| GM→L1 | `const copy_gm_to_l1_trait& trait, typename T, typename U` | trait + tensor |
| L0C→GM | `const copy_l0c_to_gm_trait& trait, typename T, typename U` | trait + tensor |
| L0C→UB | `const copy_l0c_to_ub_trait& trait, typename T, typename U` | trait + tensor |
| L0C→L1 | `const copy_l0c_to_l1_trait& trait, typename T, typename U` | trait + tensor |
| L1→L0A | `const copy_l1_to_l0a_trait& trait, typename T, typename U` | trait + tensor |
| L1→L0B | `const copy_l1_to_l0b_trait& trait, typename T, typename U` | trait + tensor |
| L1→L0ScaleA | `const copy_l1_to_l0scalea_trait& trait, typename T, typename U` | trait + tensor |
| L1→L0ScaleB | `const copy_l1_to_l0scaleb_trait& trait, typename T, typename U` | trait + tensor |
| L1→UB | `const copy_l1_to_ub_trait& trait, typename T, typename U` | trait + tensor |
| L1→Fixbuf | `const copy_l1_to_fixbuf_trait& trait, typename T, typename U` | trait + tensor |
| L1→Biastable | `const copy_l1_to_biastable_trait& trait, typename T, typename U` | trait + tensor |
| GM→UB | `const copy_gm_to_ub_trait& trait, typename T, typename U` | trait + tensor |
| UB→GM | `const copy_ub_to_gm_trait& trait, typename T, typename U` | trait + tensor |
| UB→L1 | `const copy_ub_to_l1_trait& trait, typename T, typename U` | trait + tensor |
| UB→UB | `const copy_ub_to_ub_trait& trait, typename T, typename U` | trait + tensor |

### Routing 层模板参数

| 参数 | 类型 | 说明 |
|------|------|------|
| `version` | `uint32_t` | 架构版本号 |
| `DstLayoutPattern` | `typename` | 目的 layout pattern |
| `SrcLayoutPattern` | `typename` | 源 layout pattern |
| `CopyMode` | `typename`（部分通路） | Copy 模式 |
| `BiasPos` | `typename`（mmad routing） | Bias 位置 |

### Mmad 通路模板参数

| 层 | 模板参数 | 说明 |
|----|---------|------|
| mmad.h 入口 | `typename Tp, const Tp& traits, typename... Args` | trait 类型 + trait 值 + tensor |
| mmad_impl (无bias) | `const mmad_trait& trait, typename T, typename U, typename S, typename Params` | trait + L0C/L0A/L0B tensor + params |
| mmad_impl (带bias) | `const mmad_trait& trait, typename T, typename U, typename S, typename V, typename Params` | trait + L0C/L0A/L0B/Bias tensor + params |
| instruction | `typename T` | 数据类型 |

### Atom/Traits 层模板参数

| 类 | 模板参数 | 说明 |
|----|---------|------|
| `copy_traits<CopyOp, Traits, CopyOpWith, TraitsWith>` | 4个类型参数 | 操作 + trait + with操作 + with trait |
| `copy_atom<copy_traits<Args...>>` | variadic | atom 特化 |
| `mmad_traits<MmadOp, MmadTraitsType, MmadOpWith, MmadTraitsWith>` | 4个类型参数 | 操作 + trait + with操作 + with trait |
| `mmad_atom<mmad_traits<Args...>>` | variadic | atom 特化 |
| `copy_unpack` / `mmad_unpack` | `const TraitType& trait, typename... Args` | trait 值 + tensor |
| `call` | `const TraitType& trait, typename... Params` | trait 值 + tensor |
| `with` | `typename... TraitsArgs` | 参数绑定 |

### Legacy 层模板参数

| 类/函数 | 模板参数 | 说明 |
|---------|---------|------|
| `CopyTraits<Args...>` | `typename... Args` | 独立 PascalCase 可特化模板 |
| `CopyAtom<Args...>` | `typename... Args` | 独立 PascalCase atom |
| `MmadTraits<Args...>` | `typename... Args` | 独立 PascalCase 可特化模板 |
| `MmadAtom<Args...>` | `typename... Args` | 独立 PascalCase atom |
| `has_copy_method` | `typename AlwaysVoid, typename Op, typename Trait, const Trait& T, typename... Args` | SFINAE 检测 copy 方法 |
| `normalize_copy_trait` | `typename Trait` | 类型转换模板 |
| `normalize_mmad_trait` | `typename Trait` | 类型转换模板 |
| `normalize_mmad_params` | `typename Params` | 参数转换模板 |
| `normalize_fixpipe_params` | `typename Params` | 参数转换模板 |
| `normalize_copy_gm_to_ub_params` | `typename Params` | 参数转换模板 |
| `normalize_img2col_params` | `typename PadType` | 参数转换模板 |
| `Copy()` | `typename AtomType, typename DstTensor, typename SrcTensor` | PascalCase copy |
| `MakeCopy()` | `typename CopyOperationType` | PascalCase make_copy |
| `Mmad()` | `typename... Args, typename DstTensor, typename FmTensor, typename FilterTensor` | PascalCase mmad |
| `MakeMmad()` | `typename MmadOperationType` | PascalCase make_mmad |

## 一、枚举类型

| 枚举名 | 定义位置 | 类型 | 枚举值 |
|--------|---------|------|--------|
| `cache_mode` | `include/tensor_api/utils/utils.h` | `enum class : uint8_t` | `normal=0`, `disable=4`, `last=5`, `persistent=6` |
| `mmad_type` | `include/tensor_api/utils/utils.h` | `enum class : uint8_t` | `normal=0`, `mx=1` |
| `round_mode` | `include/tensor_api/utils/utils.h` | `enum class : uint8_t` | `default_round=0`, `hybrid=1` |
| `dual_dst_mode` | `include/tensor_api/utils/utils.h` | `enum : uint8_t` | `disable=0`, `split_m=1`, `split_n=2` |

## 二、Trait 结构体（控制搬运/计算行为）

### 2.1 Mmad Trait

| 结构体 | 定义位置 | 成员变量 | 类型 | 默认值 | 说明 |
|--------|---------|----------|------|--------|------|
| `mmad_trait` | `include/tensor_api/utils/utils.h` | `fm_offset` | `int32_t` | `0` | fm 偏移 |
| | | `k_direction_align` | `bool` | `false` | K 方向对齐 |
| | | `cmatrix_source` | `bool` | `false` | C矩阵来源 |
| | | `disable_gemv` | `bool` | `true` | 禁用 GEMV |
| | | `mmad_type` | `mmad_type` | `normal` | Mmad 类型 |

### 2.2 L0C→GM/UB/L1 Trait

| 结构体 | 定义位置 | 成员变量 | 类型 | 默认值 | 说明 |
|--------|---------|----------|------|--------|------|
| `copy_l0c_to_gm_trait` | `impl/.../l0c_to_gm/copy_impl/data_copy.h` | `round_mode` | `round_mode` | `default_round` | 舍入模式 |
| | | `enable_relu` | `bool` | `false` | 使能 ReLU |
| | | `enable_channel_split` | `bool` | `false` | 使能通道拆分 |
| `copy_l0c_to_ub_trait` | `impl/.../l0c_to_ub/copy_impl/data_copy.h` | `round_mode` | `round_mode` | `default_round` | 舍入模式 |
| | | `enable_relu` | `bool` | `false` | 使能 ReLU |
| | | `enable_channel_split` | `bool` | `false` | 使能通道拆分 |
| | | `dual_dst_ctl` | `dual_dst_mode` | `disable` | 双目输出模式 |
| `copy_l0c_to_l1_trait` | `impl/.../l0c_to_l1/copy_impl/data_copy.h` | `round_mode` | `round_mode` | `default_round` | 舍入模式 |
| | | `enable_relu` | `bool` | `false` | 使能 ReLU |
| | | `enable_channel_split` | `bool` | `false` | 使能通道拆分 |

### 2.3 其他 Copy Trait（空结构体，仅用于类型标识）

| 结构体 | 定义位置 | 成员变量 | 说明 |
|--------|---------|----------|------|
| `copy_gm_to_l1_trait` | `impl/.../gm_to_l1/copy_impl/instruction.h` | 无 | GM→L1 搬运标识 |
| `copy_l1_to_ub_trait` | `impl/.../l1_to_ub/copy_impl/instruction.h` | 无 | L1→UB 搬运标识 |
| `copy_l1_to_l0a_trait` | `impl/.../l1_to_l0a/copy_impl/instruction.h` | 无 | L1→L0A 搬运标识 |
| `copy_l1_to_l0b_trait` | `impl/.../l1_to_l0b/copy_impl/instruction.h` | 无 | L1→L0B 搬运标识 |
| `copy_l1_to_l0scalea_trait` | `impl/.../l1_to_l0scalea/copy_impl/instruction.h` | 无 | L1→L0ScaleA 搬运标识 |
| `copy_l1_to_l0scaleb_trait` | `impl/.../l1_to_l0scaleb/copy_impl/instruction.h` | 无 | L1→L0ScaleB 搬运标识 |
| `copy_l1_to_fixbuf_trait` | `impl/.../l1_to_fb/copy_impl/instruction.h` | 无 | L1→Fixbuf 搬运标识 |
| `copy_l1_to_biastable_trait` | `impl/.../l1_to_bt/copy_impl/instruction.h` | 无 | L1→Biastable 搬运标识 |
| `copy_gm_to_ub_trait` | `impl/.../gm_to_ub/copy_impl/data_copy.h` | 无 | GM→UB 搬运标识 |
| `copy_ub_to_gm_trait` | `impl/.../ub_to_gm/copy_impl/data_copy.h` | 无 | UB→GM 搬运标识 |
| `copy_ub_to_l1_trait` | `impl/.../ub_to_l1/copy_impl/data_copy.h` | 无 | UB→L1 搬运标识 |
| `copy_ub_to_ub_trait` | `impl/.../ub_to_ub/copy_impl/data_copy.h` | 无 | UB→UB 搬运标识 |

## 三、参数结构体（运行时参数，通过 `.with(params)` 绑定）

### 3.1 Mmad 参数

| 结构体 | 定义位置 | 成员变量 | 类型 | 默认值 | 说明 |
|--------|---------|----------|------|--------|------|
| `mmad_params` | `include/tensor_api/utils/utils.h` | `m` | `uint16_t` | `0` | M 维度大小 |
| | | `n` | `uint16_t` | `0` | N 维度大小 |
| | | `k` | `uint16_t` | `0` | K 维度大小 |
| | | `unit_flag` | `uint8_t` | `0` | 单元标志 |
| | | `cmatrix_init_val` | `bool` | `false` | C矩阵初始值 |

### 3.2 Copy/L0C 输出参数

| 结构体 | 定义位置 | 成员变量 | 类型 | 默认值 | 说明 |
|--------|---------|----------|------|--------|------|
| `fixpipe_params` | `include/tensor_api/utils/utils.h` | `unit_flag` | `uint8_t` | `0` | 单元标志 |
| | | `sub_block_id` | `bool` | `false` | 子块 ID |
| `copy_gm_to_ub_params` | `include/tensor_api/utils/utils.h` | `left_padding_count` | `uint8_t` | `0` | 左填充计数 |
| | | `right_padding_count` | `uint8_t` | `0` | 右填充计数 |
| | | `enable_constant_pad` | `bool` | `true` | 使能常量填充 |

### 3.3 Img2Col 参数

| 结构体 | 定义位置 | 成员变量 | 类型 | 默认值 | 说明 |
|--------|---------|----------|------|--------|------|
| `img2col_params<T>` | `include/tensor_api/utils/utils.h` | `m_extension` | `uint16_t` | `0` | M 扩展 |
| | | `k_extension` | `uint16_t` | `0` | K 扩展 |
| | | `m_start_pt` | `uint16_t` | `0` | M 起始点 |
| | | `k_start_pt` | `uint16_t` | `0` | K 起始点 |
| | | `pad_list[4]` | `uint8_t[4]` | `{0,0,0,0}` | 填充列表 |
| | | `stride_w` | `uint8_t` | `1` | 宽度步长 |
| | | `stride_h` | `uint8_t` | `1` | 高度步长 |
| | | `filter_w` | `uint8_t` | `1` | 卷积核宽 |
| | | `filter_h` | `uint8_t` | `1` | 卷积核高 |
| | | `dilation_filter_w` | `uint8_t` | `1` | 膨胀宽 |
| | | `dilation_filter_h` | `uint8_t` | `1` | 膨胀高 |
| | | `filter_size_w` | `bool` | `false` | 卷积核大小宽 |
| | | `filter_size_h` | `bool` | `false` | 卷积核大小高 |
| | | `transpose` | `bool` | `false` | 转置 |
| | | `f_matrix_ctrl` | `bool` | `false` | F矩阵控制 |
| | | `pad_value` | `T` | `0` | 填充值 |

## 四、Trait Default 别名

| 别名 | 实际类型 | 定义位置 |
|------|---------|---------|
| `mmad_trait_default` | `mmad_trait` (带 `DEFAULT_MMAD_TRAIT` 值) | `impl/.../mmad/mmad.h` |
| `copy_gm_to_l1_trait_default` | `copy_gm_to_l1_trait` | `impl/.../gm_to_l1/copy.h` |
| `copy_l1_to_ub_trait_default` | `copy_l1_to_ub_trait` | `impl/.../l1_to_ub/copy.h` |
| `copy_l1_to_l0a_trait_default` | `copy_l1_to_l0a_trait` | `impl/.../l1_to_l0a/copy.h` |
| `copy_l1_to_l0b_trait_default` | `copy_l1_to_l0b_trait` | `impl/.../l1_to_l0b/copy.h` |
| `copy_l1_to_l0scalea_trait_default` | `copy_l1_to_l0scalea_trait` | `impl/.../l1_to_l0scalea/copy.h` |
| `copy_l1_to_l0scaleb_trait_default` | `copy_l1_to_l0scaleb_trait` | `impl/.../l1_to_l0scaleb/copy.h` |
| `copy_l1_to_fixbuf_trait_default` | `copy_l1_to_fixbuf_trait` | `impl/.../l1_to_fb/copy.h` |
| `copy_l1_to_biastable_trait_default` | `copy_l1_to_biastable_trait` | `impl/.../l1_to_bt/copy.h` |
| `copy_l0c_to_gm_trait_default` | `copy_l0c_to_gm_trait` | `impl/.../l0c_to_gm/copy.h` |
| `copy_l0c_to_ub_trait_default` | `copy_l0c_to_ub_trait` | `impl/.../l0c_to_ub/copy.h` |
| `copy_l0c_to_l1_trait_default` | `copy_l0c_to_l1_trait` | `impl/.../l0c_to_l1/copy.h` |
| `copy_gm_to_ub_trait_default` | `copy_gm_to_ub_trait` | `impl/.../gm_to_ub/copy.h` |
| `copy_ub_to_gm_trait_default` | `copy_ub_to_gm_trait` | `impl/.../ub_to_gm/copy.h` |
| `copy_ub_to_l1_trait_default` | `copy_ub_to_l1_trait` | `impl/.../ub_to_l1/copy.h` |
| `copy_ub_to_ub_trait_default` | `copy_ub_to_ub_trait` | `impl/.../ub_to_ub/copy.h` |

## 五、Atom/Traits 结构体方法

### 5.1 CopyTraits 方法

| 方法 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| `with()` | 无 | `CopyTraits<CopyOpWith, TraitsWith>` | 返回带参数绑定的 traits |
| `with(params)` | `fixpipe_params` 或 `copy_gm_to_ub_params` | `CopyTraits<CopyOpWith, TraitsWith>` | 绑定运行时参数 |
| `copy_unpack(trait, args...)` | `const TraitType& trait`, tensor 参数 | `void` | 内部分发调用 |
| `call(args...)` | tensor 参数 | `void` | 对外调用入口 |

### 5.2 CopyAtom 方法

| 方法 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| `call(dst, src)` | 目的 tensor, 源 tensor | `void` | 执行 Copy |
| `call(dst, src, quant)` | 目的 tensor, 源 tensor, quant 参数 | `void` | 执行带 quant 的 Copy |
| `with(args...)` | `fixpipe_params` 等 | `CopyAtom<...>` | 返回带参数的 atom |

### 5.3 MmadTraits 方法

| 方法 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| `with()` | 无 | `MmadTraits<MmadOpWith, MmadTraitsWith>` | 返回带参数绑定的 traits |
| `with(params)` | `mmad_params` | `MmadTraits<MmadOpWith, MmadTraitsWith>` | 绑定运行时参数 |
| `mmad_unpack(trait, args...)` | `const TraitType& trait`, tensor 参数 | `void` | 内部分发调用 |
| `call(args...)` | tensor 参数 | `void` | 对外调用入口 |

### 5.4 MmadAtom 方法

| 方法 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| `call(dst, fm, filter)` | L0C tensor, L0A tensor, L0B tensor | `void` | 执行 Mmad |
| `call(dst, fm, filter, bias)` | L0C tensor, L0A tensor, L0B tensor, bias tensor | `void` | 执行带 bias 的 Mmad |
| `with(params)` | `mmad_params` | `MmadAtom<...>` | 返回带参数的 atom |

## 六、公共 API 函数

### 6.1 Copy 接口

| 函数 | 参数 | 说明 |
|------|------|------|
| `copy(atom, dst, src)` | `copy_atom`, 目的 tensor, 源 tensor | 执行 Copy |
| `copy(atom, dst, src, quant)` | `copy_atom`, 目的 tensor, 源 tensor, quant | 执行带 quant 的 Copy |
| `make_copy(operation)` | copy 操作类型 | 创建 atom |
| `make_copy(operation, trait)` | copy 操作类型, trait 类型 | 创建带 trait 的 atom |

### 6.2 Mmad 接口

| 函数 | 参数 | 说明 |
|------|------|------|
| `mmad(atom, dst, fm, filter)` | `mmad_atom`, L0C tensor, L0A tensor, L0B tensor | 执行 Mmad |
| `mmad(atom, dst, fm, filter, bias)` | `mmad_atom`, L0C tensor, L0A tensor, L0B tensor, bias tensor | 执行带 bias 的 Mmad |
| `make_mmad(operation)` | mmad 操作类型 | 创建 atom |
| `make_mmad(operation, trait)` | mmad 操作类型, trait 类型 | 创建带 trait 的 atom |

## 七、PascalCase 兼容别名（legacy.h）

| PascalCase | snake_case | 说明 |
|-----------|-----------|------|
| `Copy` | `copy` | Copy 函数 |
| `MakeCopy` | `make_copy` | MakeCopy 函数 |
| `Mmad` | `mmad` | Mmad 函数 |
| `MakeMmad` | `make_mmad` | MakeMmad 函数 |
| `CopyTraits` | `copy_traits` | 独立可特化模板 |
| `CopyAtom` | `copy_atom` | 独立 atom 模板 |
| `MmadTraits` | `mmad_traits` | 别名模板 |
| `MmadAtom` | `mmad_atom` | 别名模板 |
| `MmadTrait` | `mmad_trait` | PascalCase trait（legacy_utils.h 独立定义） |
| `MmadParams` | `mmad_params` | PascalCase 参数（legacy_utils.h 独立定义） |
| `FixpipeParams` | `fixpipe_params` | PascalCase 参数（legacy_utils.h 独立定义） |
| `CopyL0C2GMTrait` | `copy_l0c_to_gm_trait` | PascalCase trait（legacy_utils.h 独立定义） |
| `CopyL0C2UBTrait` | `copy_l0c_to_ub_trait` | PascalCase trait（legacy_utils.h 独立定义） |
| `CopyL0C2L1Trait` | `copy_l0c_to_l1_trait` | PascalCase trait（legacy_utils.h 独立定义） |
| `MmadType` | `mmad_type` | PascalCase 枚举（legacy_utils.h 独立定义） |
| `RoundMode` | `round_mode` | PascalCase 枚举（legacy_utils.h 独立定义） |
| `DualDstMode` | `dual_dst_mode` | PascalCase 枚举（legacy_utils.h 独立定义） |

## 八、Instruction 层计算参数（各通路 c_api 调用参数）

### 8.1 GM→L1 通路

| c_api 函数 | 参数名 | 类型 | 说明 |
|-----------|--------|------|------|
| `asc_copy_gm2l1_align` | `dst` | `__cbuf__ T*` | 目的地址（L1） |
| | `src` | `__gm__ T*` | 源地址（GM） |
| | `block_count` | `uint32_t` | block 数量 |
| | `block_len` | `uint32_t` | block 长度 |
| | `left_padding_cnt` | `uint8_t` | 左填充计数 |
| | `right_padding_cnt` | `uint8_t` | 右填充计数 |
| | `cache_mode` | `uint8_t` | cache 模式 |
| | `src_stride` | `uint64_t` | 源步长 |
| | `dst_stride` | `uint32_t` | 目的步长 |
| `asc_copy_gm2l1_nd2nz` | `dst` | `__cbuf__ T*` | 目的地址（L1） |
| | `src` | `__gm__ T*` | 源地址（GM） |
| | `loop1_src_stride` | `uint64_t` | loop1 源步长 |
| | `cache_mode` | `uint8_t` | cache 模式 |
| | `nd_num` | `uint16_t` | ND 数量 |
| | `loop2_dst_stride` | `uint16_t` | loop2 目的步长 |
| | `loop3_dst_stride` | `uint16_t` | loop3 目的步长 |
| | `loop4_dst_stride` | `uint16_t` | loop4 目的步长 |
| | `n_value` | `uint16_t` | N 值 |
| | `d_value` | `uint32_t` | D 值 |
| | `loop4_src_stride` | `uint64_t` | loop4 源步长 |
| | `enable_small_c0` | `bool` | 使能小 C0 |
| `asc_copy_gm2l1_dn2nz` | `dst` | `__cbuf__ T*` | 目的地址（L1） |
| | `src` | `__gm__ T*` | 源地址（GM） |
| | `loop1_src_stride` | `uint64_t` | loop1 源步长 |
| | `cache_mode` | `uint8_t` | cache 模式 |
| | `dn_num` | `uint16_t` | DN 数量 |
| | `loop2_dst_stride` | `uint16_t` | loop2 目的步长 |
| | `loop3_dst_stride` | `uint16_t` | loop3 目的步长 |
| | `loop4_dst_stride` | `uint16_t` | loop4 目的步长 |
| | `n_value` | `uint16_t` | N 值 |
| | `d_value` | `uint32_t` | D 值 |
| | `loop4_src_stride` | `uint64_t` | loop4 源步长 |
| | `enable_small_c0` | `bool` | 使能小 C0 |
| `asc_set_gm2l1_nz_para` | `para` | `uint64_t` | MTE2 NZ 参数 |

### 8.2 L0C→GM/UB/L1 通路

| c_api 函数 | 参数名 | 类型 | 说明 |
|-----------|--------|------|------|
| `asc_copy_l0c2gm` | `dst` | `__gm__ T*` | 目的地址（GM） |
| | `src` | `__cc__ U*` | 源地址（L0C） |
| | `n_size` | `uint16_t` | N 维度大小 |
| | `m_size` | `uint16_t` | M 维度大小 |
| | `dst_stride` | `uint32_t` | 目的步长 |
| | `src_stride` | `uint16_t` | 源步长 |
| | `cache_mode` | `uint8_t` | cache 模式 |
| | `unit_flag` | `uint8_t` | 单元标志 |
| | `quant_pre` | `uint64_t` | 量化前置 |
| | `relu_en` | `uint8_t` | ReLU 使能 |
| | `is_channel_split` | `bool` | 通道拆分 |
| | `nz2nd_en` | `bool` | NZ→ND 使能 |
| | `nz2dn_en` | `bool` | NZ→DN 使能 |
| `asc_copy_l0c2ub` | `dst` | `__ubuf__ T*` | 目的地址（UB） |
| | `src` | `__cc__ U*` | 源地址（L0C） |
| | `n_size` | `uint16_t` | N 维度大小 |
| | `m_size` | `uint16_t` | M 维度大小 |
| | `dst_stride` | `uint32_t` | 目的步长 |
| | `src_stride` | `uint16_t` | 源步长 |
| | `dual_dst_ctl` | `uint8_t` | 双目输出控制 |
| | `sub_block_id` | `bool` | 子块 ID |
| | `unit_flag` | `uint8_t` | 单元标志 |
| | `quant_pre` | `uint64_t` | 量化前置 |
| | `relu_en` | `uint8_t` | ReLU 使能 |
| | `is_channel_split` | `bool` | 通道拆分 |
| | `nz2nd_en` | `bool` | NZ→ND 使能 |
| | `nz2dn_en` | `bool` | NZ→DN 使能 |
| `asc_copy_l0c2l1` | `dst` | `__cbuf__ T*` | 目的地址（L1） |
| | `src` | `__cc__ U*` | 源地址（L0C） |
| | `n_size` | `uint16_t` | N 维度大小 |
| | `m_size` | `uint16_t` | M 维度大小 |
| | `dst_stride` | `uint32_t` | 目的步长 |
| | `src_stride` | `uint16_t` | 源步长 |
| | `unit_flag` | `uint8_t` | 单元标志 |
| | `quant_pre` | `uint64_t` | 量化前置 |
| | `relu_en` | `uint8_t` | ReLU 使能 |
| | `is_channel_split` | `bool` | 通道拆分 |

### 8.3 L1→L0A/L0B 通路

| c_api 函数 | 参数名 | 类型 | 说明 |
|-----------|--------|------|------|
| `asc_copy_l12l0a` | `dst` | `__ca__ T*` | 目的地址（L0A） |
| | `src` | `__cbuf__ T*` | 源地址（L1） |
| | `m_start_position` | `uint16_t` | M 起始位置 |
| | `k_start_position` | `uint16_t` | K 起始位置 |
| | `m_step` | `uint8_t` | M 步长 |
| | `k_step` | `uint8_t` | K 步长 |
| | `src_stride` | `int16_t` | 源步长 |
| | `dst_stride` | `uint16_t` | 目的步长 |
| `asc_copy_l12l0a_transpose` | 同上 | 同上 | 转置版本 |
| `asc_copy_l12l0a` (img2col) | `dst` | `__ca__ T*` | 目的地址（L0A） |
| | `src` | `__cbuf__ T*` | 源地址（L1） |
| | `k_extension` | `uint16_t` | K 扩展 |
| | `m_extension` | `uint16_t` | M 扩展 |
| | `k_start_pt` | `uint16_t` | K 起始点 |
| | `m_start_pt` | `uint16_t` | M 起始点 |
| | `stride_w` | `uint8_t` | 宽度步长 |
| | `stride_h` | `uint8_t` | 高度步长 |
| | `filter_w` | `uint8_t` | 卷积核宽 |
| | `filter_h` | `uint8_t` | 卷积核高 |
| | `dilation_filter_w` | `uint8_t` | 膨胀宽 |
| | `dilation_filter_h` | `uint8_t` | 膨胀高 |
| | `filter_size_w` | `bool` | 卷积核大小宽 |
| | `filter_size_h` | `bool` | 卷积核大小高 |
| | `transpose` | `bool` | 转置 |
| | `f_matrix_ctrl` | `bool` | F矩阵控制 |
| | `channel_size` | `uint16_t` | 通道大小 |
| `asc_copy_l12l0b` | 同 `asc_copy_l12l0a` | 同上 | L1→L0B 版本 |
| `asc_copy_l12l0b_transpose` | 同上 | 同上 | 转置版本 |

### 8.4 L1→L0ScaleA/L0ScaleB 通路

| c_api 函数 | 参数名 | 类型 | 说明 |
|-----------|--------|------|------|
| `asc_copy_l12l0a_mx` | `mx_dst_addr` | `uint64_t` | MX 目的地址 |
| | `src` | `__cbuf__ T*` | 源地址（L1） |
| | `m_start_position` | `uint16_t` | M 起始位置 |
| | `k_start_position` | `uint16_t` | K 起始位置 |
| | `m_step` | `uint8_t` | M 步长 |
| | `k_step` | `uint8_t` | K 步长 |
| | `src_stride` | `int16_t` | 源步长 |
| | `dst_stride` | `uint16_t` | 目的步长 |
| `asc_copy_l12l0b_mx` | 同上 | 同上 | L0ScaleB 版本 |

### 8.5 L1→UB/Fixbuf/Biastable 通路

| c_api 函数 | 参数名 | 类型 | 说明 |
|-----------|--------|------|------|
| `asc_copy_l12ub` | `dst` | `__ubuf__ T*` | 目的地址（UB） |
| | `src` | `__cbuf__ T*` | 源地址（L1） |
| | `block_count` | `uint16_t` | block 数量 |
| | `block_len` | `uint16_t` | block 长度 |
| | `src_stride` | `uint16_t` | 源步长 |
| | `dst_stride` | `uint16_t` | 目的步长 |
| `asc_copy_l12fb` | `dst` | `__fbuf__ T*` | 目的地址（Fixbuf） |
| | `src` | `__cbuf__ T*` | 源地址（L1） |
| | `block_count` | `uint16_t` | block 数量 |
| | `block_len` | `uint16_t` | block 长度 |
| | `src_stride` | `uint16_t` | 源步长 |
| | `dst_stride` | `uint16_t` | 目的步长 |
| `asc_copy_l12bt` | `dst` | `uint64_t` | 目的地址（Biastable） |
| | `src` | `__cbuf__ T*` | 源地址（L1） |
| | `conv_control` | `bool` | 转换控制 |
| | `block_count` | `uint16_t` | block 数量 |
| | `block_len` | `uint16_t` | block 长度 |
| | `src_stride` | `uint16_t` | 源步长 |
| | `dst_stride` | `uint16_t` | 目的步长 |

### 8.6 GM→UB / UB→GM / UB→L1 / UB→UB 通路

| c_api 函数 | 参数名 | 类型 | 说明 |
|-----------|--------|------|------|
| `asc_copy_gm2ub_align` | `dst` | `__ubuf__ T*` | 目的地址（UB） |
| | `src` | `__gm__ T*` | 源地址（GM） |
| | `block_count` | `uint16_t` | block 数量 |
| | `block_len` | `uint32_t` | block 长度 |
| | `left_padding_count` | `uint8_t` | 左填充计数 |
| | `right_padding_count` | `uint8_t` | 右填充计数 |
| | `enable_constant_pad` | `bool` | 使能常量填充 |
| | `cache_mode` | `asc_load_l2_cache_mode` | L2 cache 模式 |
| | `src_stride` | `int64_t` | 源步长 |
| | `dst_stride` | `int64_t` | 目的步长 |
| `asc_copy_ub2gm_align` | `dst` | `__gm__ T*` | 目的地址（GM） |
| | `src` | `__ubuf__ T*` | 源地址（UB） |
| | `block_count` | `uint16_t` | block 数量 |
| | `block_len` | `uint32_t` | block 长度 |
| | `cache_mode` | `asc_store_l2_cache_mode` | L2 cache 模式 |
| | `dst_stride` | `int64_t` | 目的步长 |
| | `src_stride` | `uint32_t` | 源步长 |
| `asc_copy_ub2l1` | `dst` | `__cbuf__ void*` | 目的地址（L1） |
| | `src` | `__ubuf__ void*` | 源地址（UB） |
| | `block_count` | `uint16_t` | block 数量 |
| | `block_len` | `uint16_t` | block 长度 |
| | `src_stride` | `uint16_t` | 源步长 |
| | `dst_stride` | `uint16_t` | 目的步长 |
| `asc_copy_ub2ub` | `dst` | `__ubuf__ void*` | 目的地址（UB） |
| | `src` | `__ubuf__ void*` | 源地址（UB） |
| | `block_count` | `uint16_t` | block 数量 |
| | `block_len` | `uint32_t` | block 长度 |
| | `src_stride` | `int64_t` | 源步长 |
| | `dst_stride` | `int64_t` | 目的步长 |

### 8.7 Mmad 通路

| c_api 函数 | 参数名 | 类型 | 说明 |
|-----------|--------|------|------|
| `asc_mmad` | `dst` | `__cc__ T*` | 目的地址（L0C） |
| | `fm` | `__ca__ T*` | FM 地址（L0A） |
| | `filter` | `__cb__ T*` | Filter 地址（L0B） |
| | `m` | `uint16_t` | M 维度 |
| | `k` | `uint16_t` | K 维度 |
| | `n` | `uint16_t` | N 维度 |
| | `unit_flag` | `uint8_t` | 单元标志 |
| | `disable_gemv` | `bool` | 禁用 GEMV |
| | `cmatrix_source` | `bool` | C矩阵来源 |
| | `cmatrix_init_val` | `bool` | C矩阵初始值 |
| `asc_mmad` (带bias) | 同上 + `bias` | `__cc__ T*` / `__biasbuf__ T*` | Bias 地址 |
| `asc_mmad_mx` | 同 `asc_mmad` | 同上 | MX 版本 |
| `asc_mmad_mx` (带bias) | 同上 + `bias` | 同上 | MX 带 bias 版本 |
| `asc_set_ctrl` | `config` | `uint64_t` | 控制寄存器配置 |
| `asc_get_ctrl` | 无 | 返回 `uint64_t` | 获取控制寄存器 |

### 8.8 L1→L0A img2col 辅助函数

| c_api 函数 | 参数名 | 类型 | 说明 |
|-----------|--------|------|------|
| `asc_set_l13d_fmatrix` | `config` | `asc_l13d_fmatrix_config` | F矩阵配置（含 l1_height/l1_width/padding 参数） |
| `asc_set_l13d_padding` | `pad_value` | `T` | 填充值 |
| `asc_set_l13d_rpt` | `config` | `asc_load3d_v2_config` | 重复配置（含 rpt_stride/rpt_time/rpt_mode） |

## 九、参数汇总（按参数名去重）

| 参数名 | 类型 | 出现的通路 | 说明 |
|--------|------|-----------|------|
| `dst` | 各地址空间指针 | 所有 copy 通路 | 目的地址 |
| `src` | 各地址空间指针 | 所有 copy 通路 | 源地址 |
| `fm` | `__ca__ T*` | mmad | FM 地址（L0A） |
| `filter` | `__cb__ T*` | mmad | Filter 地址（L0B） |
| `bias` | `__cc__/__biasbuf__ T*` | mmad | Bias 地址 |
| `block_count` | `uint16_t/uint32_t` | gm_to_l1, l1_to_ub, l1_to_fb, l1_to_bt, gm_to_ub, ub_to_gm, ub_to_l1, ub_to_ub | block 数量 |
| `block_len` | `uint16_t/uint32_t` | 同上 | block 长度 |
| `src_stride` | `uint16_t/uint32_t/uint64_t/int16_t/int64_t` | 所有 copy 通路 | 源步长 |
| `dst_stride` | `uint16_t/uint32_t/int64_t` | 所有 copy 通路 | 目的步长 |
| `cache_mode` | `uint8_t/asc_load_l2_cache_mode/asc_store_l2_cache_mode` | gm_to_l1, l0c_to_gm, gm_to_ub, ub_to_gm | cache 模式 |
| `n_size` | `uint16_t` | l0c_to_gm, l0c_to_ub, l0c_to_l1 | N 维度大小 |
| `m_size` | `uint16_t` | l0c_to_gm, l0c_to_ub, l0c_to_l1 | M 维度大小 |
| `unit_flag` | `uint8_t` | l0c_to_gm, l0c_to_ub, l0c_to_l1, mmad | 单元标志 |
| `relu_en` | `uint8_t/bool` | l0c_to_gm, l0c_to_ub, l0c_to_l1 | ReLU 使能 |
| `is_channel_split` | `bool` | l0c_to_gm, l0c_to_ub, l0c_to_l1 | 通道拆分 |
| `nz2nd_en` | `bool` | l0c_to_gm, l0c_to_ub | NZ→ND 使能 |
| `nz2dn_en` | `bool` | l0c_to_gm, l0c_to_ub | NZ→DN 使能 |
| `quant_pre` | `uint64_t/QuantMode_t` | l0c_to_gm, l0c_to_ub, l0c_to_l1 | 量化前置 |
| `dual_dst_ctl` | `uint8_t/dual_dst_mode` | l0c_to_ub | 双目输出控制 |
| `sub_block_id` | `bool` | l0c_to_ub | 子块 ID |
| `conv_control` | `bool` | l1_to_bt | 转换控制 |
| `m_start_position` | `uint16_t` | l1_to_l0a, l1_to_l0b, l1_to_l0scalea, l1_to_l0scaleb | M 起始位置 |
| `k_start_position` | `uint16_t` | l1_to_l0a, l1_to_l0b, l1_to_l0scalea, l1_to_l0scaleb | K 起始位置 |
| `m_step` | `uint8_t` | l1_to_l0a, l1_to_l0b, l1_to_l0scalea, l1_to_l0scaleb | M 步长 |
| `k_step` | `uint8_t` | l1_to_l0a, l1_to_l0b, l1_to_l0scalea, l1_to_l0scaleb | K 步长 |
| `mx_dst_addr` | `uint64_t` | l1_to_l0scalea, l1_to_l0scaleb | MX 目的地址 |
| `m` | `uint16_t` | mmad | M 维度 |
| `n` | `uint16_t` | mmad | N 维度 |
| `k` | `uint16_t` | mmad | K 维度 |
| `disable_gemv` | `bool` | mmad | 禁用 GEMV |
| `cmatrix_source` | `bool` | mmad | C矩阵来源 |
| `cmatrix_init_val` | `bool` | mmad | C矩阵初始值 |
| `nd_num` | `uint16_t` | gm_to_l1 (nd2nz) | ND 数量 |
| `dn_num` | `uint16_t` | gm_to_l1 (dn2nz) | DN 数量 |
| `loop1_src_stride` | `uint64_t` | gm_to_l1 (nd2nz/dn2nz) | loop1 源步长 |
| `loop2_dst_stride` | `uint16_t` | gm_to_l1 (nd2nz/dn2nz) | loop2 目的步长 |
| `loop3_dst_stride` | `uint16_t` | gm_to_l1 (nd2nz/dn2nz) | loop3 目的步长 |
| `loop4_dst_stride` | `uint16_t` | gm_to_l1 (nd2nz/dn2nz) | loop4 目的步长 |
| `loop4_src_stride` | `uint64_t` | gm_to_l1 (nd2nz/dn2nz) | loop4 源步长 |
| `n_value` | `uint16_t` | gm_to_l1 (nd2nz/dn2nz) | N 值 |
| `d_value` | `uint32_t` | gm_to_l1 (nd2nz/dn2nz) | D 值 |
| `enable_small_c0` | `bool` | gm_to_l1 (nd2nz/dn2nz) | 使能小 C0 |
| `left_padding_cnt` | `uint8_t` | gm_to_l1 (align) | 左填充计数 |
| `right_padding_cnt` | `uint8_t` | gm_to_l1 (align) | 右填充计数 |
| `left_padding_count` | `uint8_t` | gm_to_ub | 左填充计数 |
| `right_padding_count` | `uint8_t` | gm_to_ub | 右填充计数 |
| `enable_constant_pad` | `bool` | gm_to_ub | 使能常量填充 |
| `m_extension` | `uint16_t` | l1_to_l0a (img2col) | M 扩展 |
| `k_extension` | `uint16_t` | l1_to_l0a (img2col) | K 扩展 |
| `m_start_pt` | `uint16_t` | l1_to_l0a (img2col) | M 起始点 |
| `k_start_pt` | `uint16_t` | l1_to_l0a (img2col) | K 起始点 |
| `stride_w` | `uint8_t` | l1_to_l0a (img2col) | 宽度步长 |
| `stride_h` | `uint8_t` | l1_to_l0a (img2col) | 高度步长 |
| `filter_w` | `uint8_t` | l1_to_l0a (img2col) | 卷积核宽 |
| `filter_h` | `uint8_t` | l1_to_l0a (img2col) | 卷积核高 |
| `dilation_filter_w` | `uint8_t` | l1_to_l0a (img2col) | 膨胀宽 |
| `dilation_filter_h` | `uint8_t` | l1_to_l0a (img2col) | 膨胀高 |
| `filter_size_w` | `bool` | l1_to_l0a (img2col) | 卷积核大小宽 |
| `filter_size_h` | `bool` | l1_to_l0a (img2col) | 卷积核大小高 |
| `transpose` | `bool` | l1_to_l0a, l1_to_l0b | 转置 |
| `f_matrix_ctrl` | `bool` | l1_to_l0a (img2col) | F矩阵控制 |
| `channel_size` | `uint16_t` | l1_to_l0a (img2col) | 通道大小 |
| `pad_value` | `T` | l1_to_l0a (img2col) | 填充值 |
