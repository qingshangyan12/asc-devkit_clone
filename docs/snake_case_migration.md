# Tensor API snake_case 风格改造修改清单

## 概述

将 Tensor API 的代码风格从 PascalCase/camelCase 改造为 snake_case，同时保持原有大驼峰形式的向后兼容性。

- **改造范围**：`impl/tensor_api/`、`include/tensor_api/`、`tests/api/tensor_api/`
- **修改文件数**：135 个（impl 96 个，include 4 个，tests 35 个）
- **代码变更**：+4806 行 / -4041 行
- **验证结果**：编译通过，1464 个单元测试全部 PASSED（AIC 1166 + AIV 298）

## 改造原则

| 改造对象 | 改造前 | 改造后 | 兼容性 |
|----------|--------|--------|--------|
| 自由函数（命名空间作用域） | `MakeTensor` | `make_tensor` | 保留 PascalCase wrapper 调用 snake_case |
| 结构体成员函数 | `Shape()`、`Engine()` | 保持不变 | — |
| 结构体成员函数内局部变量 | `sliceEngine` | `slice_engine` | 无需兼容（内部实现） |
| 结构体成员函数内参数 | `const DstLayout& dstLayout` | `const DstLayout& dst_layout` | 无需兼容（内部实现） |
| 结构体/枚举类名 | `BaseTensor`、`Location::GM` | 保持不变 | — |
| 结构体成员变量 | `FixpipeParams::unitFlag` | 保持不变 | — |
| 模板类型参数 | `typename Capacity` | 保持不变 | — |

## 一、自由函数 PascalCase → snake_case

### 1.1 公共 API（include/tensor_api/）

以下函数的前向声明和定义全部改为 snake_case，并在 impl 文件中生成 PascalCase 兼容 wrapper：

| 文件 | PascalCase | snake_case |
|------|------------|------------|
| `include/tensor_api/algorithm/copy.h` | `Copy` | `copy` |
| `include/tensor_api/algorithm/copy.h` | `MakeCopy` | `make_copy` |
| `include/tensor_api/algorithm/mmad.h` | `Mmad` | `mmad` |
| `include/tensor_api/algorithm/mmad.h` | `MakeMmad` | `make_mmad` |
| `include/tensor_api/tensor/tensor.h` | `MakeTensor` | `make_tensor` |
| `include/tensor_api/tensor/layout.h` | `MakeShape` | `make_shape` |
| `include/tensor_api/tensor/layout.h` | `MakeStride` | `make_stride` |
| `include/tensor_api/tensor/layout.h` | `MakeTile` | `make_tile` |
| `include/tensor_api/tensor/layout.h` | `MakeCoord` | `make_coord` |
| `include/tensor_api/tensor/layout.h` | `MakeLayout` | `make_layout` |
| `include/tensor_api/tensor/layout.h` | `Crd2Idx` | `crd2idx` |
| `include/tensor_api/tensor/layout.h` | `Get` | `get` |
| `include/tensor_api/tensor/layout.h` | `Cosize` | `cosize` |
| `include/tensor_api/tensor/layout.h` | `Rank` | `rank` |
| `include/tensor_api/tensor/layout.h` | `Select` | `select` |
| `include/tensor_api/tensor/layout.h` | `Size` | `size` |
| `include/tensor_api/tensor/layout.h` | `Capacity` | `capacity` |
| `include/tensor_api/tensor/layout.h` | `Slice` | `slice` |
| `include/tensor_api/tensor/layout.h` | `MakeFrameLayout` | `make_frame_layout` |
| `include/tensor_api/tensor/layout.h` | `Squeeze` | `squeeze` |

### 1.2 内部实现函数（impl/tensor_api/）

| PascalCase | snake_case | 所在文件 |
|------------|------------|----------|
| `Transform` | `transform` | `algorithm/transform_impl.h` |
| `MakeCoordLayout` | `make_coord_layout` | `tensor/layout_impl.h` |
| `MakeSlicePatternLayout` | `make_slice_pattern_layout` | `tensor/layout_impl.h` |
| `MakeSameShapeSliceLayout` | `make_same_shape_slice_layout` | `tensor/layout_impl.h` |
| `MakeFourDimSliceLayout` | `make_four_dim_slice_layout` | `tensor/layout_impl.h` |
| `MakeFlatBatchSliceLayoutImpl` | `make_flat_batch_slice_layout_impl` | `tensor/layout_impl.h` |
| `MakeFlatBatchSliceLayout` | `make_flat_batch_slice_layout` | `tensor/layout_impl.h` |
| `MakeSliceLayout` | `make_slice_layout` | `tensor/layout_impl.h` |
| `RemoveBatchDim` | `remove_batch_dim` | `tensor/layout_impl.h` |
| `MakeSqueezedLayout` | `make_squeezed_layout` | `tensor/layout_impl.h` |
| `SqueezeLayout` | `squeeze_layout` | `tensor/layout_impl.h` |
| `SqueezeRecTupleLevel` | `squeeze_rec_tuple_level` | `tensor/layout_impl.h` |
| `SqueezeRecNode` | `squeeze_rec_node` | `tensor/layout_impl.h` |
| `SqueezeUnwrap` | `squeeze_unwrap` | `tensor/layout_impl.h` |
| `SqueezeRecTupleLevelImpl` | `squeeze_rec_tuple_level_impl` | `tensor/layout_impl.h` |
| `MakeSingleBatchSubTensor` | `make_single_batch_sub_tensor` | `tensor/tensor_impl.h` |
| `MakeFractalShape` | `make_fractal_shape` | `tensor/layout_fractal.h` |
| `BatchStrideAt` | `batch_stride_at` | `tensor/layout_frame.h` |
| `MakeFlatBatchLayout` | `make_flat_batch_layout` | `tensor/layout_frame.h` |
| `MakeMultiBatchFrameLayoutImpl` | `make_multi_batch_frame_layout_impl` | `tensor/layout_frame.h` |
| `MakeMultiBatchFrameLayout` | `make_multi_batch_frame_layout` | `tensor/layout_frame.h` |
| `MakePatternLayout` | `make_pattern_layout` | `tensor/layout_pattern.h` |
| `MakeBatchPatternLayout` | `make_batch_pattern_layout` | `tensor/layout_pattern.h` |
| `TupleApply` | `tuple_apply` | `tensor/tuple_impl.h` |
| `TransformApply` | `transform_apply` | `tensor/tuple_impl.h` |
| `TransformTupleApply` | `transform_tuple_apply` | `tensor/tuple_impl.h` |
| `TransformTupleApplyImpl` | `transform_tuple_apply_impl` | `tensor/tuple_impl.h` |
| `GetTuple` | `get_tuple` | `tensor/tuple_impl.h` |
| `GetRank` | `get_rank` | `tensor/tuple_impl.h` |
| `TupleSize` | `tuple_size` | `tensor/tuple_impl.h` |
| `SelectTuple` | `select_tuple` | `tensor/tuple_impl.h` |
| `GetValue` | `get_value` | `tensor/tuple_impl.h` |
| `GetMax` | `get_max` | `tensor/tuple_impl.h` |
| `GetCapacity` | `get_capacity` | `tensor/tuple_impl.h` |
| `GetCapacityImpl` | `get_capacity_impl` | `tensor/tuple_impl.h` |
| `SelectBySeqImpl` | `select_by_seq_impl` | `tensor/tuple_impl.h` |
| `SelectBySeq` | `select_by_seq` | `tensor/tuple_impl.h` |
| `Crd2IdxTTT` | `crd2idx_ttt` | `tensor/coord_index.h` |
| `Crd2IdxITT` | `crd2idx_itt` | `tensor/coord_index.h` |
| `MakeMemPtr` | `make_mem_ptr` | `tensor/pointer_pattern.h` |
| `MakeLocationMemPtr` | `make_location_mem_ptr` | `tensor/pointer_mem_impl.h` |
| `BuildStrideRowImpl` | `build_stride_row_impl` | `tensor/layout_method.h` |
| `BuildStrideColImpl` | `build_stride_col_impl` | `tensor/layout_method.h` |
| `ComputeStride` | `compute_stride` | `tensor/layout_method.h` |
| `BuildFlatStrideImpl` | `build_flat_stride_impl` | `tensor/layout_method.h` |
| `ComputeFlatStride` | `compute_flat_stride` | `tensor/layout_method.h` |
| `GetShape` | `get_shape` | `tensor/layout_method.h` |
| `GetStride` | `get_stride` | `tensor/layout_method.h` |
| `Coshape` | `coshape` | `tensor/layout_method.h` |
| `GetElement` | `get_element` | `arch/utils/arch_utils.h` |
| `GetTotalColumnShape` | `get_total_column_shape` | `arch/utils/arch_utils.h` |
| `GetTotalRowShape` | `get_total_row_shape` | `arch/utils/arch_utils.h` |
| `RunGmToL1Batched` | `run_gm_to_l1_batched` | `arch/cube/gm_to_l1/copy_impl/copy_common.h` |
| `SetMTE2NzPara` | `set_mte2_nz_para` | `arch/cube/gm_to_l1/copy_impl/instruction.h` |
| `AllocFbTempBuf` | `alloc_fb_temp_buf` | `arch/cube/utils/l0c2out_utils.h` |
| `SetFpc` | `set_fpc` | `arch/cube/utils/l0c2out_utils.h` |
| `InsertSync` | `insert_sync` | `arch/cube/utils/l0c2out_utils.h` |
| `CheckOneLayoutPattern` | `check_one_layout_pattern` | `arch/utils/check_format.h` |
| `CheckLayoutPattern` | `check_layout_pattern` | `arch/utils/check_format.h` |
| `EmitSetRegister` | `emit_set_register` | `arch/cube/utils/l0c2out_utils.h` |
| `SetRegisterImpl` | `set_register_impl` | `arch/cube/utils/l0c2out_utils.h` |
| `SetCtrlForhifloat8` | `set_ctrl_for_hifloat8` | `arch/cube/mmad/mmad_impl/instruction.h` |
| `ResetCtrlForhifloat8` | `reset_ctrl_for_hifloat8` | `arch/cube/mmad/mmad_impl/instruction.h` |

### 1.3 PascalCase 兼容 wrapper

在每个 impl 文件的 header guard `#endif` 之前（namespace 闭合之后）插入 PascalCase wrapper，共 **19 个文件、104 个 wrapper**：

```cpp
// 示例：impl/tensor_api/algorithm/copy_impl.h
namespace AscendC {
namespace Te {

template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src)
{
    copy<AtomType, DstTensor, SrcTensor>(atomCopy, dst, src);
}

} // namespace Te
} // namespace AscendC
```

包含 wrapper 的文件清单：

| 文件 | wrapper 数量 |
|------|-------------|
| `impl/tensor_api/tensor/tuple_impl.h` | 22 |
| `impl/tensor_api/tensor/layout_method.h` | 19 |
| `impl/tensor_api/tensor/layout_impl.h` | 16 |
| `impl/tensor_api/arch/cube/utils/l0c2out_utils.h` | 7 |
| `impl/tensor_api/tensor/layout_frame.h` | 6 |
| `impl/tensor_api/algorithm/copy_impl.h` | 4 |
| `impl/tensor_api/algorithm/mmad_impl.h` | 4 |
| `impl/tensor_api/tensor/tensor_impl.h` | 4 |
| `impl/tensor_api/tensor/coord_index.h` | 4 |
| `impl/tensor_api/arch/utils/arch_utils.h` | 4 |
| `impl/tensor_api/tensor/pointer_pattern.h` | 3 |
| `impl/tensor_api/tensor/layout_fractal.h` | 2 |
| `impl/tensor_api/tensor/layout_pattern.h` | 2 |
| `impl/tensor_api/arch/utils/check_format.h` | 2 |
| `impl/tensor_api/arch/cube/mmad/mmad_impl/instruction.h` | 2 |
| `impl/tensor_api/algorithm/transform_impl.h` | 1 |
| `impl/tensor_api/tensor/pointer_mem_impl.h` | 1 |
| `impl/tensor_api/arch/cube/gm_to_l1/copy_impl/copy_common.h` | 1 |
| `impl/tensor_api/arch/cube/gm_to_l1/copy_impl/instruction.h` | 1 |

## 二、结构体成员函数内变量 camelCase → snake_case

对结构体/类的成员函数（`__aicore__`、`__simd_callee__`、`__simd_vf__` 装饰的缩进函数）内部的局部变量和参数名进行 snake_case 改造，涉及 **64 个文件**。

### 2.1 改造的变量类型

- `auto` 声明的局部变量：`auto sliceEngine = ...` → `auto slice_engine = ...`
- 类型声明的局部变量：`uint8_t unitFlag = ...` → `uint8_t unit_flag = ...`
- `const T&` 参数：`const DstLayout& dstLayout` → `const DstLayout& dst_layout`
- `decltype(auto)` 声明的局部变量

### 2.2 典型变量改名示例

| 改造前 | 改造后 | 说明 |
|--------|--------|------|
| `sliceEngine` | `slice_engine` | tensor slicing 引擎 |
| `coordLayout` | `coord_layout` | 坐标 layout |
| `subEngine` | `sub_engine` | 子 tensor 引擎 |
| `subLayout` | `sub_layout` | 子 tensor layout |
| `batchStride` | `batch_stride` | batch 步长 |
| `batchIdx` | `batch_idx` | batch 索引 |
| `newLayout` | `new_layout` | 新 layout |
| `dstLayout` | `dst_layout` | 目的 layout |
| `srcLayout` | `src_layout` | 源 layout |
| `dstNoBatchLayout` | `dst_no_batch_layout` | 去 batch 的目的 layout |
| `srcNoBatchLayout` | `src_no_batch_layout` | 去 batch 的源 layout |
| `copyParams` | `copy_params` | copy 参数 |
| `dstCur` | `dst_cur` | 目的当前指针 |
| `unitFlag` | `unit_flag` | unit flag（局部变量） |
| `cacheMode` | `cache_mode` | cache 模式 |
| `reluEn` | `relu_en` | relu 使能 |
| `nz2ndEn` | `nz2nd_en` | nz2nd 使能 |
| `nz2dnEn` | `nz2dn_en` | nz2dn 使能 |
| `blockCount` | `block_count` | block 数量 |
| `blockLen` | `block_len` | block 长度 |
| `mStep` | `m_step` | M 步长 |
| `kStep` | `k_step` | K 步长 |
| `srcStride` | `src_stride` | 源步长 |
| `dstStride` | `dst_stride` | 目的步长 |
| `innerRow` | `inner_row` | 内部行 |
| `innerCol` | `inner_col` | 内部列 |
| `realRow` | `real_row` | 实际行 |
| `realCol` | `real_col` | 实际列 |
| `realShape` | `real_shape` | 实际形状 |
| `coordShape` | `coord_shape` | 坐标形状 |
| `fractalShape` | `fractal_shape` | fractal 形状 |
| `noTrans` | `no_trans` | 是否不转置 |
| `tailParam` | `tail_param` | 尾部参数 |
| `mainLoopParam` | `main_loop_param` | 主循环参数 |

### 2.3 不改名的变量

以下类型的标识符**保持不变**：

- **结构体成员变量访问**：`params.unitFlag`、`trait.enableRelu`、`trait.dualDstCtl`（通过 `.->::` 访问的成员）
- **模板类型参数**：`typename Capacity`、`typename Tuple`、`typename LayoutType`
- **已知类型名**：`Engine`、`Layout`、`Location`、`Shape`、`Stride` 等

## 三、测试文件更新

`tests/api/tensor_api/` 下 **35 个测试文件**的调用站点全部更新为 snake_case：

### 3.1 基础结构测试（basic_struct/）

| 文件 | 主要改动 |
|------|----------|
| `layout/test_tensor_api_frame.cpp` | `MakeFrameLayout` → `make_frame_layout` 等 |
| `layout/test_tensor_api_get.cpp` | `Get` → `get`、`GetShape` → `get_shape` 等 |
| `layout/test_tensor_api_layout.cpp` | `MakeLayout` → `make_layout`、`MakeShape` → `make_shape` 等 |
| `layout/test_tensor_api_metrics.cpp` | `Size` → `size`、`Capacity` → `capacity` 等 |
| `layout/test_tensor_api_select.cpp` | `Select` → `select` 等 |
| `layout/test_tensor_api_squeeze.cpp` | `Squeeze` → `squeeze` 等 |
| `pointer/test_tensor_api_engine.cpp` | `MakeMemPtr` → `make_mem_ptr` 等 |
| `pointer/test_tensor_api_ptr.cpp` | `MakeMemPtr` → `make_mem_ptr` 等 |
| `pointer/test_tensor_api_ptr_offset.cpp` | `MakeMemPtr` → `make_mem_ptr` 等 |
| `tensor/test_tensor_api_l2cache.cpp` | `MakeTensor` → `make_tensor` 等 |
| `tensor/test_tensor_api_slice.cpp` | `Slice` → `slice`、`MakeCoord` → `make_coord` 等 |
| `tensor/test_tensor_api_tensor.cpp` | `MakeTensor` → `make_tensor`、`MakeCoord` → `make_coord` 等 |

### 3.2 NPU 架构测试（npu_arch_3510/）

AIC（13 个文件）和 AIV（10 个文件）的 copy/mmad/transform 测试全部更新调用站点为 snake_case。

## 四、技术实现细节

### 4.1 正则匹配规则

自由函数改名使用以下正则，确保只匹配函数调用/定义：

```
(?<!template )(?<![.>:])\b(FuncName)\b(?=\s*[(<])
```

- `(?<!template )`：跳过 `template Get`（显式模板调用中的模板名）
- `(?<![.>:])`：跳过 `.Get`、`->Get`、`::Get`（成员函数调用）
- `(?=\s*[(<])`：只匹配后接 `(` 或 `<` 的标识符（避免误改类型名如 `typename Capacity`）

### 4.2 结构体成员函数识别

通过以下条件识别结构体成员函数（不修改其函数名）：

- 行首为空格或 Tab（缩进）
- 行内容以 `__aicore__`、`__simd_callee__`、`__simd_vf__` 开头
- 包含 `inline` 关键字

### 4.3 wrapper 生成规则

- 可推导的 `typename` 模板参数不显式传递（避免引用类型重载歧义）
- 未命名的参数自动生成 `_arg0`、`_arg1` 等名称
- 默认值参数（`typename = Std::enable_if_t<...>`）和 C++ 关键字（`typename`、`class`）跳过

### 4.4 变量改名安全性

- 使用 negative lookbehind `(?<![.>:])` 避免误改结构体成员变量访问
- 排除 C++ 关键字和已知类型名
- 按变量名长度降序替换，避免部分匹配

## 五、兼容性保证

### 5.1 大驼峰函数直接调用 snake 函数

原有 PascalCase 调用代码**无需修改**即可继续工作，因为每个 impl 文件末尾生成了对应的 PascalCase wrapper：

```cpp
// 用户原有代码（仍然可用）
auto copyAtom = MakeCopy(CopyGM2L1{}, CopyGM2L1TraitDefault{});
Copy(copyAtom, dstTensor, srcTensor);

// 新代码（推荐使用 snake_case）
auto copy_atom = make_copy(CopyGM2L1{}, CopyGM2L1TraitDefault{});
copy(copy_atom, dst_tensor, src_tensor);
```

### 5.2 结构体成员函数不受影响

结构体的成员函数名保持不变：

```cpp
// 以下调用方式完全不变
tensor.Shape();
tensor.Engine();
tensor.Data();
tensor.Slice(coord, info);
layout.Shape();
layout.Stride();
layout.Capacity();
```

### 5.3 结构体/枚举类名不受影响

```cpp
// 以下类型名保持不变
BaseTensor<EngineType, LayoutType>
GlobalTensor<TensorAttribute<Engine, Layout>>
LocalTensor<TensorAttribute<Engine, Layout>>
Location::GM, Location::L1, Location::L0A
CopyGM2L1, CopyL12L0A, CopyL0C2GM
NDLayoutPtn, NZLayoutPtn, ZZLayoutPtn
FixpipeParams, MmadParams, MmadTrait
```
