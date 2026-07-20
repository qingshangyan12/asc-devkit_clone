# 头文件按文档目录功能分类重构方案

---

## 一、重构原则

将现有头文件按 `docs/zh/api/` 的 **文档功能目录** 进行归类合并，每个功能目录生成一个合并后的头文件，命名规则：

```
kernel_operator_{功能分类英文名}_intf.h
```

**合并后文件 = 该功能目录下所有关联的现有头文件内容的聚合。**

---

## 二、基础API → include/basic_api/ 重构

### 2.1 Memory矢量计算（`Memory矢量计算/`）

共 19 个现有头文件 → 合并为 15 个功能头文件

| 序号 | 文档子目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|----------|--------------|-----------------|:---:|
| 1 | **基础算术/** | `kernel_operator_memory_basic_arithmetic_intf.h` | `kernel_operator_vec_binary_intf.h`<br>`kernel_operator_vec_binary_scalar_intf.h`<br>`kernel_operator_vec_unary_intf.h`<br>`kernel_operator_vec_ternary_scalar_intf.h` | 4 |
| 2 | **复合计算/** | `kernel_operator_memory_compound_compute_intf.h` | `kernel_operator_vec_brcb_intf.h` | 1 |
| 3 | **归约计算/** | `kernel_operator_memory_reduce_intf.h` | `kernel_operator_vec_reduce_intf.h` | 1 |
| 4 | **比较与选择/** | `kernel_operator_memory_compare_select_intf.h` | `kernel_operator_vec_cmpsel_intf.h` | 1 |
| 5 | **逻辑计算/** | `kernel_operator_memory_logic_compute_intf.h` | （部分逻辑计算接口内嵌于 cmpsel/binary 中，独立声明） | 1 |
| 6 | **离散与聚合/** | `kernel_operator_memory_gather_scatter_intf.h` | `kernel_operator_vec_gather_intf.h`<br>`kernel_operator_vec_gather_mask_intf.h`<br>`kernel_operator_vec_scatter_intf.h` | 3 |
| 7 | **数据填充/** | `kernel_operator_memory_data_fill_intf.h` | `kernel_operator_vec_duplicate_intf.h` | 1 |
| 8 | **数据排布转换/** | `kernel_operator_memory_data_rearrange_intf.h` | `kernel_operator_vec_transpose_intf.h` | 1 |
| 9 | **数据重排（ISASI）/** | `kernel_operator_memory_data_reorder_intf.h` | `kernel_operator_vec_mulcast_intf.h`<br>`kernel_operator_vec_bilinearinterpolation_intf.h` | 2 |
| 10 | **排序组合（ISASI）/** | `kernel_operator_memory_sort_combine_intf.h` | `kernel_operator_proposal_intf.h` | 1 |
| 11 | **掩码操作/** | `kernel_operator_memory_mask_intf.h` | `kernel_operator_vec_vpadding_intf.h` | 1 |
| 12 | **类型转换/** | `kernel_operator_memory_type_convert_intf.h` | `kernel_operator_vec_vconv_intf.h` | 1 |
| 13 | **索引操作/** | `kernel_operator_memory_index_intf.h` | `kernel_operator_vec_createvecindex_intf.h` | 1 |
| 14 | **Vector逻辑架构/** | `kernel_operator_memory_vector_intf.h` | `kernel_vec_intf.h` | 1 |
| — | **SIMD计算说明/** | （无对应头文件，为接口说明文档） | — | — |

> 原有 19 个头文件归入 14 个功能头文件（"逻辑计算"和"SIMD计算说明"无内联文件映射）

---

### 2.2 Reg矢量计算（`Reg矢量计算/`）

共 22 个现有头文件（`include/basic_api/reg_compute/`下） → 合并为 21 个功能头文件

| 序号 | 文档子目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|----------|--------------|-----------------|:---:|
| 1 | **基础算术/** | `kernel_operator_reg_basic_arithmetic_intf.h` | `kernel_reg_compute_vec_binary_intf.h`<br>`kernel_reg_compute_vec_binary_scalar_intf.h`<br>`kernel_reg_compute_vec_unary_intf.h`<br>`kernel_reg_compute_vec_ternary_scalar_intf.h` | 4 |
| 2 | **复合计算/** | `kernel_operator_reg_compound_compute_intf.h` | `kernel_reg_compute_vec_fused_intf.h` | 1 |
| 3 | **归约计算/** | `kernel_operator_reg_reduce_intf.h` | `kernel_reg_compute_vec_reduce_intf.h` | 1 |
| 4 | **比较与选择/** | `kernel_operator_reg_compare_select_intf.h` | `kernel_reg_compute_vec_cmpsel_intf.h` | 1 |
| 5 | **逻辑计算/** | `kernel_operator_reg_logic_compute_intf.h` | （逻辑计算接口内嵌） | 1 |
| 6 | **离散操作/** | `kernel_operator_reg_scatter_gather_intf.h` | `kernel_reg_compute_gather_mask_intf.h` | 1 |
| 7 | **数据压缩/** | `kernel_operator_reg_data_compress_intf.h` | `kernel_reg_compute_pack_intf.h` | 1 |
| 8 | **数据填充/** | `kernel_operator_reg_data_fill_intf.h` | `kernel_reg_compute_vec_duplicate_intf.h`<br>`kernel_reg_compute_vec_arange_intf.h` | 2 |
| 9 | **数据重排/** | `kernel_operator_reg_data_rearrange_intf.h` | `kernel_reg_compute_copy_intf.h`<br>`kernel_reg_compute_datacopy_intf.h` | 2 |
| 10 | **类型转换/** | `kernel_operator_reg_type_convert_intf.h` | `kernel_reg_compute_vec_vconv_intf.h` | 1 |
| 11 | **直方图计算/** | `kernel_operator_reg_histogram_intf.h` | `kernel_reg_compute_histograms_intf.h` | 1 |
| 12 | **MaskReg计算/** | `kernel_operator_reg_maskreg_intf.h` | `kernel_reg_compute_maskreg_intf.h` | 1 |
| 13 | **Reg数据搬入/** | `kernel_operator_reg_data_load_intf.h` | （reg_load 相关接口，当前在 reg_compute 中部分覆盖） | 1 |
| 14 | **Reg数据搬出/** | `kernel_operator_reg_data_store_intf.h` | （reg_store 相关接口） | 1 |
| 15 | **同步控制/** | `kernel_operator_reg_sync_intf.h` | `kernel_reg_compute_membar_intf.h` | 1 |
| 16 | **系统变量访问/** | `kernel_operator_reg_sys_var_intf.h` | （sys_var 相关接口） | 1 |
| 17 | **索引操作/** | `kernel_operator_reg_index_intf.h` | （索引相关接口） | 1 |
| 18 | **VF调用/** | `kernel_operator_reg_vf_call_intf.h` | （VF 调用接口） | 1 |
| 19 | **寄存器数据类型/** | `kernel_operator_reg_data_types_intf.h` | `kernel_reg_compute_struct_intf.h`<br>`kernel_reg_compute_addrreg_intf.h` | 2 |
| 20 | **辅助数据类型/** | `kernel_operator_reg_aux_types_intf.h` | `kernel_reg_compute_common_intf.h`<br>`kernel_reg_compute_utils.h` | 2 |
| 21 | **关键特性说明/** | `kernel_operator_reg_features_intf.h` | `kernel_reg_compute_intf.h` | 1 |

> 原有 22 个头文件归入 21 个功能头文件

---

### 2.3 矩阵计算（`矩阵计算（ISASI）/` + `矩阵计算（TensorAPI）/`）

文档中两个目录共享同一组头文件

| 序号 | 文档子目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|----------|--------------|-----------------|:---:|
| 1 | **Mmad计算/** | `kernel_operator_mmad_compute_intf.h` | `kernel_cube_intf.h` | 1 |
| 2 | **矩阵搬入/** | `kernel_operator_matrix_load_intf.h` | —（cube 搬入相关 API 内嵌） | — |
| 3 | **矩阵搬出/** | `kernel_operator_matrix_store_intf.h` | —（cube 搬出相关 API 内嵌） | — |
| 4 | **矩阵乘/** | `kernel_operator_matmul_intf.h` | `kernel_operator_mm_intf.h`<br>`kernel_operator_gemm_intf.h`<br>`kernel_operator_mm_bitmode_intf.h` | 3 |
| 5 | **卷积/** | `kernel_operator_conv2d_intf.h` | `kernel_operator_conv2d_intf.h`<br>`kernel_operator_fixpipe_intf.h` | 2 |

> 共 6 个现有头文件 → 合并为 5 个功能头文件（矩阵搬入/搬出接口内嵌于 cube_intf）

---

### 2.4 原子操作（`原子操作/`）

| 序号 | 文档子目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|----------|--------------|-----------------|:---:|
| 1 | **原子操作/** | `kernel_operator_atomic_ops_intf.h` | `kernel_operator_atomic_intf.h`<br>`kernel_operator_set_atomic_intf.h` | 2 |

---

### 2.5 同步控制（`同步控制/`）

| 序号 | 文档子目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|----------|--------------|-----------------|:---:|
| 1 | **核内同步/** | `kernel_operator_inner_sync_intf.h` | `kernel_operator_determine_compute_sync_intf.h` | 1 |
| 2 | **核间同步/** | `kernel_operator_inter_core_sync_intf.h` | `kernel_operator_block_sync_intf.h` | 1 |

---

### 2.6 Cube分组管理（`Cube分组管理（ISASI）/`）

| 序号 | 文档子目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|----------|--------------|-----------------|:---:|
| 1 | **Cube分组管理/** | `kernel_operator_cube_group_intf.h` | `core_mng/roc/kernel_operator_cube_group_intf.h`<br>`core_mng/roc/kernel_operator_group_barrier_intf.h` | 2 |

---

### 2.7 其他基础API功能目录

| 序号 | 文档目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|---------|--------------|-----------------|:---:|
| 1 | **Kernel-Tiling/** | `kernel_operator_tiling_intf.h` | `kernel_basic_intf.h`<br>`kernel_operator_intf.h`<br>`kernel_operator_common_intf.h` | 3 |
| 2 | **工具接口/** | `kernel_operator_utils_intf.h` | `kernel_operator_utils_intf.h`<br>`kernel_operator_limits_intf.h` | 2 |
| 3 | **数据搬运导览/** | `kernel_operator_data_copy_intf.h` | `kernel_operator_data_copy_intf.h` | 1 |
| 4 | **数据结构/** | `kernel_operator_data_struct_intf.h` | `kernel_type.h`<br>`kernel_base_types.h`<br>`kernel_common.h`<br>`kernel_tensor.h`<br>`kernel_operator_list_tensor_intf.h`<br>`kernel_struct_aipp.h`<br>`kernel_struct_binary.h`<br>`kernel_struct_brcb.h`<br>`kernel_struct_conv2d.h`<br>`kernel_struct_data_copy.h`<br>`kernel_struct_fixpipe.h`<br>`kernel_struct_gather.h`<br>`kernel_struct_mm.h`<br>`kernel_struct_proposal.h`<br>`kernel_struct_transpose.h`<br>`kernel_struct_unary.h`<br>`kernel_struct_vdeq.h` | 17 |
| 5 | **标量计算/** | `kernel_operator_scalar_intf.h` | `kernel_operator_scalar_intf.h` | 1 |
| 6 | **特殊寄存器访问/** | `kernel_operator_sys_register_intf.h` | `kernel_operator_sys_var_intf.h` | 1 |
| 7 | **缓存控制/** | `kernel_operator_cache_intf.h` | `kernel_operator_cache_intf.h` | 1 |
| 8 | **调试接口/** | `kernel_operator_debug_intf.h` | `kernel_operator_dump_tensor_intf.h`<br>`kernel_prof_trace_intf.h` | 2 |
| 9 | **资源管理/** | `kernel_operator_resource_intf.h` | `kernel_tpipe.h`<br>`kernel_operator_swap_mem_intf.h` | 2 |

---

### 2.8 基础API重构后清单

| 序号 | 新头文件名 | 合并来源文件数 |
|:---:|-----------|:---:|
| 1 | `kernel_operator_memory_basic_arithmetic_intf.h` | 4 |
| 2 | `kernel_operator_memory_compound_compute_intf.h` | 1 |
| 3 | `kernel_operator_memory_reduce_intf.h` | 1 |
| 4 | `kernel_operator_memory_compare_select_intf.h` | 1 |
| 5 | `kernel_operator_memory_logic_compute_intf.h` | — |
| 6 | `kernel_operator_memory_gather_scatter_intf.h` | 3 |
| 7 | `kernel_operator_memory_data_fill_intf.h` | 1 |
| 8 | `kernel_operator_memory_data_rearrange_intf.h` | 1 |
| 9 | `kernel_operator_memory_data_reorder_intf.h` | 2 |
| 10 | `kernel_operator_memory_sort_combine_intf.h` | 1 |
| 11 | `kernel_operator_memory_mask_intf.h` | 1 |
| 12 | `kernel_operator_memory_type_convert_intf.h` | 1 |
| 13 | `kernel_operator_memory_index_intf.h` | 1 |
| 14 | `kernel_operator_memory_vector_intf.h` | 1 |
| 15 | `kernel_operator_reg_basic_arithmetic_intf.h` | 4 |
| 16 | `kernel_operator_reg_compound_compute_intf.h` | 1 |
| 17 | `kernel_operator_reg_reduce_intf.h` | 1 |
| 18 | `kernel_operator_reg_compare_select_intf.h` | 1 |
| 19 | `kernel_operator_reg_logic_compute_intf.h` | — |
| 20 | `kernel_operator_reg_scatter_gather_intf.h` | 1 |
| 21 | `kernel_operator_reg_data_compress_intf.h` | 1 |
| 22 | `kernel_operator_reg_data_fill_intf.h` | 2 |
| 23 | `kernel_operator_reg_data_rearrange_intf.h` | 2 |
| 24 | `kernel_operator_reg_type_convert_intf.h` | 1 |
| 25 | `kernel_operator_reg_histogram_intf.h` | 1 |
| 26 | `kernel_operator_reg_maskreg_intf.h` | 1 |
| 27 | `kernel_operator_reg_data_load_intf.h` | — |
| 28 | `kernel_operator_reg_data_store_intf.h` | — |
| 29 | `kernel_operator_reg_sync_intf.h` | 1 |
| 30 | `kernel_operator_reg_sys_var_intf.h` | — |
| 31 | `kernel_operator_reg_index_intf.h` | — |
| 32 | `kernel_operator_reg_vf_call_intf.h` | — |
| 33 | `kernel_operator_reg_data_types_intf.h` | 2 |
| 34 | `kernel_operator_reg_aux_types_intf.h` | 2 |
| 35 | `kernel_operator_reg_features_intf.h` | 1 |
| 36 | `kernel_operator_mmad_compute_intf.h` | 1 |
| 37 | `kernel_operator_matrix_load_intf.h` | — |
| 38 | `kernel_operator_matrix_store_intf.h` | — |
| 39 | `kernel_operator_matmul_intf.h` | 3 |
| 40 | `kernel_operator_conv2d_intf.h` | 2 |
| 41 | `kernel_operator_atomic_ops_intf.h` | 2 |
| 42 | `kernel_operator_inner_sync_intf.h` | 1 |
| 43 | `kernel_operator_inter_core_sync_intf.h` | 1 |
| 44 | `kernel_operator_cube_group_intf.h` | 2 |
| 45 | `kernel_operator_tiling_intf.h` | 3 |
| 46 | `kernel_operator_utils_intf.h` | 2 |
| 47 | `kernel_operator_data_copy_intf.h` | 1 |
| 48 | `kernel_operator_data_struct_intf.h` | 17 |
| 49 | `kernel_operator_scalar_intf.h` | 1 |
| 50 | `kernel_operator_sys_register_intf.h` | 1 |
| 51 | `kernel_operator_cache_intf.h` | 1 |
| 52 | `kernel_operator_debug_intf.h` | 2 |
| 53 | `kernel_operator_resource_intf.h` | 2 |

**基础API重构前后：55 个分散头文件 → 53 个按功能合并的头文件**

---

## 三、C-API → include/c_api/ 重构

`docs/zh/api/SIMD-API/C-API/` 下共 12 个功能目录，`include/c_api/` 下也是 12 个子目录，**天然对齐**。

| 序号 | 文档目录 / C-API 子目录 | 新头文件名 | 合并自现有头文件 |
|:---:|------------------------|-----------|----------------|
| 1 | **vector_compute/** | `kernel_operator_c_vector_compute_intf.h` | `include/c_api/vector_compute/vector_compute.h` |
| 2 | **vector_datamove/** | `kernel_operator_c_vector_datamove_intf.h` | `include/c_api/vector_datamove/vector_datamove.h` |
| 3 | **cube_compute/** | `kernel_operator_c_cube_compute_intf.h` | `include/c_api/cube_compute/cube_compute.h` |
| 4 | **cube_datamove/** | `kernel_operator_c_cube_datamove_intf.h` | `include/c_api/cube_datamove/cube_datamove.h` |
| 5 | **scalar_compute/** | `kernel_operator_c_scalar_compute_intf.h` | `include/c_api/scalar_compute/scalar_compute.h` |
| 6 | **reg_compute/** | `kernel_operator_c_reg_compute_intf.h` | `reg_load.h`<br>`reg_store.h`<br>`reg_vector.h`<br>`reg_convert.h` |
| 7 | **simd_atomic/** | `kernel_operator_c_atomic_intf.h` | `include/c_api/atomic/atomic.h` |
| 8 | **sync/** | `kernel_operator_c_sync_intf.h` | `include/c_api/sync/sync.h` |
| 9 | **cache_ctrl/** | `kernel_operator_c_cache_ctrl_intf.h` | `include/c_api/cache_ctrl/cache_ctrl.h` |
| 10 | **sys_var/** | `kernel_operator_c_sys_var_intf.h` | `include/c_api/sys_var/sys_var.h` |
| 11 | **misc/** | `kernel_operator_c_misc_intf.h` | `include/c_api/misc/misc.h` |
| 12 | **struct/** + **utils/** | `kernel_operator_c_utils_intf.h` | `include/c_api/utils/union.h` |

**C-API 重构前后：~15 个分散头文件 → 12 个按功能合并的头文件**

---

## 四、高阶API → include/adv_api/ 重构

### 4.1 数学计算（`数学计算/`）

| 序号 | 功能分组 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|---------|--------------|-----------------|:---:|
| 1 | **三角函数/** | `kernel_operator_trigonometric_intf.h` | `kernel_operator_sin_intf.h`<br>`kernel_operator_cos_intf.h`<br>`kernel_operator_tan_intf.h`<br>`kernel_operator_asin_intf.h`<br>`kernel_operator_acos_intf.h`<br>`kernel_operator_atan_intf.h` | 6 |
| 2 | **双曲函数/** | `kernel_operator_hyperbolic_intf.h` | `kernel_operator_sinh_intf.h`<br>`kernel_operator_cosh_intf.h`<br>`kernel_operator_tanh_intf.h`<br>`kernel_operator_asinh_intf.h`<br>`kernel_operator_acosh_intf.h`<br>`kernel_operator_atanh_intf.h` | 6 |
| 3 | **指数对数/** | `kernel_operator_exponential_intf.h` | `kernel_operator_exp_intf.h`<br>`kernel_operator_log_intf.h`<br>`kernel_operator_power_intf.h`<br>`kernel_operator_sqrt_intf.h` | 4 |
| 4 | **取整取余/** | `kernel_operator_rounding_intf.h` | `kernel_operator_ceil_intf.h`<br>`kernel_operator_floor_intf.h`<br>`kernel_operator_round_intf.h`<br>`kernel_operator_trunc_intf.h`<br>`kernel_operator_frac_intf.h`<br>`kernel_operator_fmod_intf.h`<br>`kernel_operator_rint_intf.h` | 7 |
| 5 | **符号与截断/** | `kernel_operator_sign_clamp_intf.h` | `kernel_operator_sign_intf.h`<br>`kernel_operator_clamp_intf.h` | 2 |
| 6 | **特殊函数/** | `kernel_operator_special_intf.h` | `kernel_operator_erf_intf.h`<br>`kernel_operator_erfc_intf.h`<br>`kernel_operator_lgamma_intf.h`<br>`kernel_operator_digamma_intf.h` | 4 |
| 7 | **混合乘加/** | `kernel_operator_fused_math_intf.h` | `kernel_operator_axpy_intf.h`<br>`kernel_operator_fma_intf.h`<br>`kernel_operator_hypot_intf.h`<br>`kernel_operator_sincos_intf.h` | 4 |
| 8 | **位运算/** | `kernel_operator_bitwise_intf.h` | `kernel_operator_bitwise_and_intf.h`<br>`kernel_operator_bitwise_or_intf.h`<br>`kernel_operator_bitwise_not_intf.h`<br>`kernel_operator_bitwise_xor_intf.h`<br>`kernel_operator_xor_intf.h` | 5 |
| 9 | **逻辑运算/** | `kernel_operator_logical_intf.h` | `kernel_operator_logical_and_intf.h`<br>`kernel_operator_logical_ands_intf.h`<br>`kernel_operator_logical_not_intf.h`<br>`kernel_operator_logical_or_intf.h`<br>`kernel_operator_logical_ors_intf.h`<br>`kernel_operator_logical_xor_intf.h` | 6 |
| 10 | **谓词判断/** | `kernel_operator_predicate_intf.h` | `kernel_operator_is_finite_intf.h`<br>`kernel_operator_is_inf_intf.h`<br>`kernel_operator_is_nan_intf.h` | 3 |
| 11 | **条件选择/** | `kernel_operator_where_intf.h` | `kernel_operator_where_intf.h`<br>`kernel_operator_cumsum_intf.h` | 2 |
| 12 | **随机数/** | `kernel_operator_random_intf.h` | `kernel_operator_philox_intf.h` | 1 |

> 数学计算共 ~39 个现有头文件 → 12 个功能头文件

---

### 4.2 激活函数（`激活函数/`）

| 序号 | 功能分组 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|---------|--------------|-----------------|:---:|
| 1 | **Sigmoid/Tanh/** | `kernel_operator_sigmoid_intf.h` | `kernel_operator_sigmoid_intf.h` | 1 |
| 2 | **ReLU/GELU/** | `kernel_operator_relu_gelu_intf.h` | `kernel_operator_gelu_intf.h`<br>`kernel_operator_reglu_intf.h`<br>`kernel_operator_geglu_intf.h`<br>`kernel_operator_swiglu_intf.h` | 4 |
| 3 | **SiLU/Swish/** | `kernel_operator_silu_swish_intf.h` | `kernel_operator_silu_intf.h`<br>`kernel_operator_swish_intf.h` | 2 |
| 4 | **SoftMax/** | `kernel_operator_softmax_intf.h` | `kernel_operator_softmax_intf.h`<br>`kernel_operator_softmax_flash_intf.h`<br>`kernel_operator_softmax_flashv2_intf.h`<br>`kernel_operator_softmax_grad_intf.h`<br>`kernel_operator_simple_softmax_intf.h`<br>`kernel_operator_logsoftmax_intf.h` | 6 |

> 激活函数共 13 个现有头文件 → 4 个功能头文件

---

### 4.3 归一化操作（`归一化操作/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_normalization_intf.h` | `kernel_operator_batchnorm_intf.h`<br>`kernel_operator_layernorm_intf.h`<br>`kernel_operator_layernormgrad_intf.h`<br>`kernel_operator_layernormgradbeta_intf.h`<br>`kernel_operator_rmsnorm_intf.h`<br>`kernel_operator_deepnorm_intf.h`<br>`kernel_operator_welfordfinalize_intf.h` | 7 |

---

### 4.4 归约操作（`归约操作/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_reduce_intf.h` | `kernel_operator_sum_intf.h`<br>`kernel_operator_mean_intf.h` | 2 |

---

### 4.5 量化操作（`量化操作/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_quantization_intf.h` | `kernel_operator_quantize_intf.h`<br>`kernel_operator_dequantize_intf.h`<br>`kernel_operator_ascend_quant_intf.h`<br>`kernel_operator_ascend_dequant_intf.h`<br>`kernel_operator_ascend_antiquant_intf.h`<br>`kernel_operator_antiquantize_intf.h` | 6 |

---

### 4.6 卷积计算（`卷积计算/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_conv3d_intf.h` | `conv/conv3d/conv3d_intf.h`<br>`conv/conv3d/conv3d_api.h` | 2 |
| 2 | `kernel_operator_conv3d_backprop_intf.h` | `kernel_operator_conv3d_bp_filter_*.h`<br>`kernel_operator_conv3d_bp_input_*.h` | 2 |

---

### 4.7 矩阵计算（`矩阵计算/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_matmul_intf.h` | `matmul/matmul_intf.h` | 1 |

---

### 4.8 排序操作（`排序操作/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_sort_intf.h` | `kernel_operator_topk_intf.h` | 1 |

---

### 4.9 张量变换（`张量变换/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_transpose_intf.h` | `kernel_operator_confusion_transpose_intf.h` | 1 |

---

### 4.10 数据过滤（`数据过滤/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_filter_intf.h` | `kernel_operator_dropout_intf.h`<br>`kernel_operator_selectwithbytesmask_intf.h` | 2 |

---

### 4.11 填充操作（`pad/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_pad_intf.h` | `kernel_operator_pad_intf.h` | 1 |

---

### 4.12 索引计算（`索引计算/`）

| 序号 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|--------------|-----------------|:---:|
| 1 | `kernel_operator_index_intf.h` | `kernel_operator_arithprogression_intf.h` | 1 |

---

### 4.13 高阶API重构后清单

| 序号 | 新头文件名 | 合并来源文件数 |
|:---:|-----------|:---:|
| 1 | `kernel_operator_trigonometric_intf.h` | 6 |
| 2 | `kernel_operator_hyperbolic_intf.h` | 6 |
| 3 | `kernel_operator_exponential_intf.h` | 4 |
| 4 | `kernel_operator_rounding_intf.h` | 7 |
| 5 | `kernel_operator_sign_clamp_intf.h` | 2 |
| 6 | `kernel_operator_special_intf.h` | 4 |
| 7 | `kernel_operator_fused_math_intf.h` | 4 |
| 8 | `kernel_operator_bitwise_intf.h` | 5 |
| 9 | `kernel_operator_logical_intf.h` | 6 |
| 10 | `kernel_operator_predicate_intf.h` | 3 |
| 11 | `kernel_operator_where_intf.h` | 2 |
| 12 | `kernel_operator_random_intf.h` | 1 |
| 13 | `kernel_operator_sigmoid_intf.h` | 1 |
| 14 | `kernel_operator_relu_gelu_intf.h` | 4 |
| 15 | `kernel_operator_silu_swish_intf.h` | 2 |
| 16 | `kernel_operator_softmax_intf.h` | 6 |
| 17 | `kernel_operator_normalization_intf.h` | 7 |
| 18 | `kernel_operator_reduce_intf.h` | 2 |
| 19 | `kernel_operator_quantization_intf.h` | 6 |
| 20 | `kernel_operator_conv3d_intf.h` | 2 |
| 21 | `kernel_operator_conv3d_backprop_intf.h` | 2 |
| 22 | `kernel_operator_matmul_intf.h` | 1 |
| 23 | `kernel_operator_sort_intf.h` | 1 |
| 24 | `kernel_operator_transpose_intf.h` | 1 |
| 25 | `kernel_operator_filter_intf.h` | 2 |
| 26 | `kernel_operator_pad_intf.h` | 1 |
| 27 | `kernel_operator_index_intf.h` | 1 |

**高阶API重构前后：~70 个分散头文件 → 27 个功能头文件**

---

## 五、SIMT-API → include/simt_api/ 重构

`docs/zh/api/SIMT-API/` 共 10 个功能目录

| 序号 | 文档目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|---------|--------------|-----------------|:---:|
| 1 | **数学函数/** | `kernel_operator_simt_math_intf.h` | `include/simt_api/math_functions.h`<br>`include/simt_api/cpp/kernel_simt_math_intf.h`<br>`include/simt_api/cpp/kernel_simt_transcendental_intf.h`<br>`include/simt_api/cpp/kernel_simt_bessel_intf.h` | 4 |
| 2 | **原子操作/** | `kernel_operator_simt_atomic_intf.h` | `include/simt_api/device_atomic_functions.h`<br>`include/simt_api/cpp/kernel_simt_atomic_intf.h` | 2 |
| 3 | **同步与内存栅栏/** | `kernel_operator_simt_sync_intf.h` | `include/simt_api/device_sync_functions.h` | 1 |
| 4 | **协作组/** | `kernel_operator_simt_cooperative_groups_intf.h` | `include/simt_api/cooperative_groups.h` | 1 |
| 5 | **Warp函数/** | `kernel_operator_simt_warp_intf.h` | `include/simt_api/device_warp_functions.h`<br>`include/simt_api/cpp/kernel_simt_warp_level_intf.h` | 2 |
| 6 | **访存函数/** | `kernel_operator_simt_memory_intf.h` | `include/simt_api/device_functions.h` | 1 |
| 7 | **数据类型/** | `kernel_operator_simt_types_intf.h` | `include/simt_api/device_types.h`<br>`include/simt_api/asc_bf16.h`<br>`include/simt_api/asc_fp16.h`<br>`include/simt_api/asc_fp8.h`<br>`include/simt_api/math_constants.h` | 5 |
| 8 | **地址空间谓词函数/** | `kernel_operator_simt_address_pred_intf.h` | `include/simt_api/common_functions.h`（部分） | 1 |
| 9 | **地址空间转换函数/** | `kernel_operator_simt_address_cast_intf.h` | `include/simt_api/common_functions.h`（部分） | 1 |
| 10 | **通用接口/** | `kernel_operator_simt_common_intf.h` | `include/simt_api/cpp/kernel_simt_common_intf.h`<br>`include/simt_api/cpp/kernel_simt_utils.h`<br>`include/simt_api/cpp/kernel_simt_cmp_intf.h`<br>`include/simt_api/cpp/kernel_simt_cast_intf.h`<br>`include/simt_api/cpp/kernel_simt_print_intf.h`<br>`include/simt_api/cpp/kernel_simt_intf.h`<br>`include/simt_api/vector_functions.h` | 7 |

**SIMT-API 重构前后：~25 个分散头文件 → 10 个功能头文件**

---

## 六、Utils-API → include/utils/ 重构

`docs/zh/api/Utils-API/` 共 10 个功能目录

| 序号 | 文档目录 | 新合并头文件名 | 合并自现有头文件 | 数量 |
|:---:|---------|--------------|-----------------|:---:|
| 1 | **CPP标准库/** | `kernel_operator_utils_cpp_std_intf.h` | `include/utils/std/algorithm.h`<br>`include/utils/std/cmath.h`<br>`include/utils/std/tuple.h`<br>`include/utils/std/type_traits.h`<br>`include/utils/std/utility.h` | 5 |
| 2 | **平台信息获取/** | `kernel_operator_utils_platform_intf.h` | `include/utils/tiling/platform/platform_ascendc.h` | 1 |
| 3 | **调测接口/** | `kernel_operator_utils_debug_intf.h` | `include/utils/debug/asc_assert.h`<br>`include/utils/debug/asc_dump.h`<br>`include/utils/debug/asc_printf.h`<br>`include/utils/debug/asc_time.h` | 4 |
| 4 | **Tiling调测/** | `kernel_operator_utils_tiling_debug_intf.h` | `include/utils/context/context_builder.h`<br>`include/utils/context/context_ascendc_log.h` | 2 |
| 5 | **Tiling模板编程/** | `kernel_operator_utils_tiling_template_intf.h` | `include/utils/tiling/template_argument.h` | 1 |
| 6 | **Tiling下沉/** | `kernel_operator_utils_tiling_offload_intf.h` | —（独立功能） | — |
| 7 | **Tiling数据结构注册/** | `kernel_operator_utils_tiling_data_intf.h` | —（独立功能） | — |
| 8 | **原型注册与管理/** | `kernel_operator_utils_op_registry_intf.h` | —（独立功能，配套 impl/） | — |
| 9 | **RTC/** | `kernel_operator_utils_rtc_intf.h` | —（独立功能） | — |
| 10 | **SuperKernel/** | `kernel_operator_utils_superkernel_intf.h` | —（独立功能） | — |
| 11 | **日志/** | `kernel_operator_utils_log_intf.h` | `include/utils/log/asc_cpu_log.h` | 1 |
| 12 | **基础常量与宏/** | `kernel_operator_utils_base_intf.h` | `include/utils/base/sys_constants.h`<br>`include/utils/base/sys_macros.h`<br>`include/utils/common_types.h` | 3 |

**Utils-API 重构前后：~15 个分散头文件 → 12 个功能头文件**

---

## 七、重构前后总量对照

| API 层级 | 重构前头文件数 | 重构后头文件数 | 压缩比 |
|----------|:---:|:---:|:---:|
| **基础API** | 55 | 53 | 1.0x（分散→归类清晰） |
| **C-API** | 15 | 12 | 1.3x |
| **高阶API** | ~70 | 27 | 2.6x |
| **SIMT-API** | ~25 | 10 | 2.5x |
| **Utils-API** | ~15 | 12 | 1.3x |
| **总计** | **~180** | **114** | **1.6x** |

---

## 八、重命名规则总结

1. **前缀统一**：全部使用 `kernel_operator_` 前缀
2. **层级标识**：基础API(Memory/Reg/C-API)在名称中保留 `_c_` 或 `_memory_`/`_reg_` 区分；SIMT-API 使用 `_simt_`；Utils-API 使用 `_utils_`
3. **功能命名**：取文档功能目录的英文译名，如"基础算术"→ `basic_arithmetic`，"激活函数"→（拆分合并为 `sigmoid`/`relu_gelu`/`silu_swish`/`softmax` 等）
4. **后缀统一**：全部以 `_intf.h` 结尾

---

## 九、典型重构示例

**例：Memory矢量计算 / 基础算术 目录**

```
重构前（4个独立头文件）:
├── kernel_operator_vec_binary_intf.h        // 二元运算: Add, Sub, Mul, Div...
├── kernel_operator_vec_binary_scalar_intf.h // 标量二元: Adds, Muls...
├── kernel_operator_vec_unary_intf.h         // 一元运算: Neg, Abs, Sqrt, Rsqrt...
└── kernel_operator_vec_ternary_scalar_intf.h // 三元运算: Select, Fma...

重构后（1个合并头文件）:
└── kernel_operator_memory_basic_arithmetic_intf.h  ← 聚合上述4个文件全部声明
```