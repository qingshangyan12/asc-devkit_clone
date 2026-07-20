# 基础API 头文件按文档目录结构重新划分

---

## 一、划分前（当前状态）

当前 `include/basic_api/` 下头文件呈扁平化分布，仅按子模块（`core_mng/`、`op_frame/`、`reg_compute/`）做了粗粒度划分入口，其余 ~50 个 .h 文件全部平铺在顶层。

```
include/basic_api/
├── core_mng/roc/
│   ├── kernel_operator_cube_group_intf.h
│   └── kernel_operator_group_barrier_intf.h
├── op_frame/
│   └── elemwise_frame.h
├── reg_compute/
│   ├── kernel_reg_compute_addrreg_intf.h
│   ├── kernel_reg_compute_common_intf.h
│   ├── kernel_reg_compute_copy_intf.h
│   ├── kernel_reg_compute_datacopy_intf.h
│   ├── kernel_reg_compute_gather_mask_intf.h
│   ├── kernel_reg_compute_histograms_intf.h
│   ├── kernel_reg_compute_intf.h
│   ├── kernel_reg_compute_maskreg_intf.h
│   ├── kernel_reg_compute_membar_intf.h
│   ├── kernel_reg_compute_pack_intf.h
│   ├── kernel_reg_compute_struct_intf.h
│   ├── kernel_reg_compute_utils.h
│   ├── kernel_reg_compute_vec_arange_intf.h
│   ├── kernel_reg_compute_vec_binary_intf.h
│   ├── kernel_reg_compute_vec_binary_scalar_intf.h
│   ├── kernel_reg_compute_vec_cmpsel_intf.h
│   ├── kernel_reg_compute_vec_duplicate_intf.h
│   ├── kernel_reg_compute_vec_fused_intf.h
│   ├── kernel_reg_compute_vec_reduce_intf.h
│   ├── kernel_reg_compute_vec_ternary_scalar_intf.h
│   ├── kernel_reg_compute_vec_unary_intf.h
│   └── kernel_reg_compute_vec_vconv_intf.h     ← 3个子目录已分类
│                                                 ★ 以下 ~50 个 .h 全部平铺在顶层 ★
├── kernel_base_types.h
├── kernel_basic_intf.h
├── kernel_common.h
├── kernel_cube_intf.h
├── kernel_operator_atomic_intf.h
├── kernel_operator_block_sync_intf.h
├── kernel_operator_cache_intf.h
├── kernel_operator_common_intf.h
├── kernel_operator_conv2d_intf.h
├── kernel_operator_data_copy_intf.h
├── kernel_operator_determine_compute_sync_intf.h
├── kernel_operator_dump_tensor_intf.h
├── kernel_operator_fixpipe_intf.h
├── kernel_operator_gemm_intf.h
├── kernel_operator_intf.h
├── kernel_operator_limits_intf.h
├── kernel_operator_list_tensor_intf.h
├── kernel_operator_mm_bitmode_intf.h
├── kernel_operator_mm_intf.h
├── kernel_operator_proposal_intf.h
├── kernel_operator_scalar_intf.h
├── kernel_operator_set_atomic_intf.h
├── kernel_operator_swap_mem_intf.h
├── kernel_operator_sys_var_intf.h
├── kernel_operator_utils_intf.h
├── kernel_operator_vec_bilinearinterpolation_intf.h
├── kernel_operator_vec_binary_intf.h
├── kernel_operator_vec_binary_scalar_intf.h
├── kernel_operator_vec_brcb_intf.h
├── kernel_operator_vec_cmpsel_intf.h
├── kernel_operator_vec_createvecindex_intf.h
├── kernel_operator_vec_duplicate_intf.h
├── kernel_operator_vec_gather_intf.h
├── kernel_operator_vec_gather_mask_intf.h
├── kernel_operator_vec_mulcast_intf.h
├── kernel_operator_vec_reduce_intf.h
├── kernel_operator_vec_scatter_intf.h
├── kernel_operator_vec_ternary_scalar_intf.h
├── kernel_operator_vec_transpose_intf.h
├── kernel_operator_vec_unary_intf.h
├── kernel_operator_vec_vconv_intf.h
├── kernel_operator_vec_vpadding_intf.h
├── kernel_prof_trace_intf.h
├── kernel_struct_aipp.h
├── kernel_struct_binary.h
├── kernel_struct_brcb.h
├── kernel_struct_conv2d.h
├── kernel_struct_data_copy.h
├── kernel_struct_fixpipe.h
├── kernel_struct_gather.h
├── kernel_struct_mm.h
├── kernel_struct_proposal.h
├── kernel_struct_transpose.h
├── kernel_struct_unary.h
├── kernel_struct_vdeq.h
├── kernel_tensor.h
├── kernel_tpipe.h
├── kernel_type.h
└── kernel_vec_intf.h
```

---

## 二、划分后（按文档目录结构归类）

将上述头文件按 `docs/zh/api/SIMD-API/基础API/` 的 **16 个一级文档目录** 归类，保持头文件原文件名不变，仅做逻辑分组。

### 2.1 各文档目录对应的头文件清单

| 序号 | 文档目录 | 归入的头文件 | 数量 |
|:---:|---------|-------------|:---:|
| 1 | **Cube分组管理（ISASI）/** | `core_mng/roc/kernel_operator_cube_group_intf.h`<br>`core_mng/roc/kernel_operator_group_barrier_intf.h` | 2 |
| 2 | **Kernel-Tiling/** | `kernel_basic_intf.h`<br>`kernel_operator_intf.h`<br>`kernel_operator_common_intf.h` | 3 |
| 3 | **Memory矢量计算/** | `kernel_vec_intf.h`<br>`kernel_operator_vec_binary_intf.h`<br>`kernel_operator_vec_binary_scalar_intf.h`<br>`kernel_operator_vec_unary_intf.h`<br>`kernel_operator_vec_brcb_intf.h`<br>`kernel_operator_vec_cmpsel_intf.h`<br>`kernel_operator_vec_createvecindex_intf.h`<br>`kernel_operator_vec_duplicate_intf.h`<br>`kernel_operator_vec_gather_intf.h`<br>`kernel_operator_vec_gather_mask_intf.h`<br>`kernel_operator_vec_mulcast_intf.h`<br>`kernel_operator_vec_reduce_intf.h`<br>`kernel_operator_vec_scatter_intf.h`<br>`kernel_operator_vec_ternary_scalar_intf.h`<br>`kernel_operator_vec_transpose_intf.h`<br>`kernel_operator_vec_vconv_intf.h`<br>`kernel_operator_vec_vpadding_intf.h`<br>`kernel_operator_vec_bilinearinterpolation_intf.h`<br>`kernel_operator_proposal_intf.h` | 19 |
| 4 | **Reg矢量计算/** | `reg_compute/kernel_reg_compute_intf.h`<br>`reg_compute/kernel_reg_compute_common_intf.h`<br>`reg_compute/kernel_reg_compute_utils.h`<br>`reg_compute/kernel_reg_compute_struct_intf.h`<br>`reg_compute/kernel_reg_compute_addrreg_intf.h`<br>`reg_compute/kernel_reg_compute_maskreg_intf.h`<br>`reg_compute/kernel_reg_compute_membar_intf.h`<br>`reg_compute/kernel_reg_compute_copy_intf.h`<br>`reg_compute/kernel_reg_compute_datacopy_intf.h`<br>`reg_compute/kernel_reg_compute_gather_mask_intf.h`<br>`reg_compute/kernel_reg_compute_histograms_intf.h`<br>`reg_compute/kernel_reg_compute_pack_intf.h`<br>`reg_compute/kernel_reg_compute_vec_arange_intf.h`<br>`reg_compute/kernel_reg_compute_vec_binary_intf.h`<br>`reg_compute/kernel_reg_compute_vec_binary_scalar_intf.h`<br>`reg_compute/kernel_reg_compute_vec_cmpsel_intf.h`<br>`reg_compute/kernel_reg_compute_vec_duplicate_intf.h`<br>`reg_compute/kernel_reg_compute_vec_fused_intf.h`<br>`reg_compute/kernel_reg_compute_vec_reduce_intf.h`<br>`reg_compute/kernel_reg_compute_vec_ternary_scalar_intf.h`<br>`reg_compute/kernel_reg_compute_vec_unary_intf.h`<br>`reg_compute/kernel_reg_compute_vec_vconv_intf.h` | 22 |
| 5 | **原子操作/** | `kernel_operator_atomic_intf.h`<br>`kernel_operator_set_atomic_intf.h` | 2 |
| 6 | **同步控制/** | `kernel_operator_block_sync_intf.h`<br>`kernel_operator_determine_compute_sync_intf.h` | 2 |
| 7 | **工具接口/** | `kernel_operator_utils_intf.h`<br>`kernel_operator_limits_intf.h` | 2 |
| 8 | **数据搬运导览/** | `kernel_operator_data_copy_intf.h` | 1 |
| 9 | **数据结构/** | `kernel_type.h`<br>`kernel_base_types.h`<br>`kernel_common.h`<br>`kernel_tensor.h`<br>`kernel_operator_list_tensor_intf.h`<br>`kernel_struct_aipp.h`<br>`kernel_struct_binary.h`<br>`kernel_struct_brcb.h`<br>`kernel_struct_conv2d.h`<br>`kernel_struct_data_copy.h`<br>`kernel_struct_fixpipe.h`<br>`kernel_struct_gather.h`<br>`kernel_struct_mm.h`<br>`kernel_struct_proposal.h`<br>`kernel_struct_transpose.h`<br>`kernel_struct_unary.h`<br>`kernel_struct_vdeq.h` | 17 |
| 10 | **标量计算/** | `kernel_operator_scalar_intf.h` | 1 |
| 11 | **特殊寄存器访问/** | `kernel_operator_sys_var_intf.h` | 1 |
| 12 | **矩阵计算（ISASI）/ + 矩阵计算（TensorAPI）/** | `kernel_cube_intf.h`<br>`kernel_operator_mm_intf.h`<br>`kernel_operator_gemm_intf.h`<br>`kernel_operator_mm_bitmode_intf.h`<br>`kernel_operator_conv2d_intf.h`<br>`kernel_operator_fixpipe_intf.h` | 6 |
| 13 | **缓存控制/** | `kernel_operator_cache_intf.h` | 1 |
| 14 | **调试接口/** | `kernel_operator_dump_tensor_intf.h`<br>`kernel_prof_trace_intf.h` | 2 |
| 15 | **资源管理/** | `kernel_tpipe.h`<br>`kernel_operator_swap_mem_intf.h` | 2 |
| 16 | **op_frame（文档缺失，暂独立）** | `op_frame/elemwise_frame.h` | 1 |

| 总计 | | |
|------|------|------|
| **文件数** | | **55（含子目录）** |
| **文档覆盖** | 15/16 个文档域有对应头文件 | **1 个缺文档**（op_frame） |

---

## 三、划分后虚拟目录树

```
include/basic_api/                           ← 重新编排的虚拟目录结构
│
├── 01-Cube分组管理（ISASI）/                   ← 对应文档: Cube分组管理（ISASI）/
│   ├── kernel_operator_cube_group_intf.h
│   └── kernel_operator_group_barrier_intf.h
│
├── 02-Kernel-Tiling/                         ← 对应文档: Kernel-Tiling/
│   ├── kernel_basic_intf.h
│   ├── kernel_operator_intf.h
│   └── kernel_operator_common_intf.h
│
├── 03-Memory矢量计算/                         ← 对应文档: Memory矢量计算/
│   ├── kernel_vec_intf.h
│   ├── kernel_operator_vec_binary_intf.h
│   ├── kernel_operator_vec_binary_scalar_intf.h
│   ├── kernel_operator_vec_unary_intf.h
│   ├── kernel_operator_vec_brcb_intf.h
│   ├── kernel_operator_vec_cmpsel_intf.h
│   ├── kernel_operator_vec_createvecindex_intf.h
│   ├── kernel_operator_vec_duplicate_intf.h
│   ├── kernel_operator_vec_gather_intf.h
│   ├── kernel_operator_vec_gather_mask_intf.h
│   ├── kernel_operator_vec_mulcast_intf.h
│   ├── kernel_operator_vec_reduce_intf.h
│   ├── kernel_operator_vec_scatter_intf.h
│   ├── kernel_operator_vec_ternary_scalar_intf.h
│   ├── kernel_operator_vec_transpose_intf.h
│   ├── kernel_operator_vec_vconv_intf.h
│   ├── kernel_operator_vec_vpadding_intf.h
│   ├── kernel_operator_vec_bilinearinterpolation_intf.h
│   └── kernel_operator_proposal_intf.h
│
├── 04-Reg矢量计算/                            ← 对应文档: Reg矢量计算/
│   ├── kernel_reg_compute_intf.h
│   ├── kernel_reg_compute_common_intf.h
│   ├── kernel_reg_compute_utils.h
│   ├── kernel_reg_compute_struct_intf.h
│   ├── kernel_reg_compute_addrreg_intf.h
│   ├── kernel_reg_compute_maskreg_intf.h
│   ├── kernel_reg_compute_membar_intf.h
│   ├── kernel_reg_compute_copy_intf.h
│   ├── kernel_reg_compute_datacopy_intf.h
│   ├── kernel_reg_compute_gather_mask_intf.h
│   ├── kernel_reg_compute_histograms_intf.h
│   ├── kernel_reg_compute_pack_intf.h
│   ├── kernel_reg_compute_vec_arange_intf.h
│   ├── kernel_reg_compute_vec_binary_intf.h
│   ├── kernel_reg_compute_vec_binary_scalar_intf.h
│   ├── kernel_reg_compute_vec_cmpsel_intf.h
│   ├── kernel_reg_compute_vec_duplicate_intf.h
│   ├── kernel_reg_compute_vec_fused_intf.h
│   ├── kernel_reg_compute_vec_reduce_intf.h
│   ├── kernel_reg_compute_vec_ternary_scalar_intf.h
│   ├── kernel_reg_compute_vec_unary_intf.h
│   └── kernel_reg_compute_vec_vconv_intf.h
│
├── 05-原子操作/                               ← 对应文档: 原子操作/
│   ├── kernel_operator_atomic_intf.h
│   └── kernel_operator_set_atomic_intf.h
│
├── 06-同步控制/                               ← 对应文档: 同步控制/
│   ├── kernel_operator_block_sync_intf.h
│   └── kernel_operator_determine_compute_sync_intf.h
│
├── 07-工具接口/                               ← 对应文档: 工具接口/
│   ├── kernel_operator_utils_intf.h
│   └── kernel_operator_limits_intf.h
│
├── 08-数据搬运导览/                           ← 对应文档: 数据搬运导览/
│   └── kernel_operator_data_copy_intf.h
│
├── 09-数据结构/                               ← 对应文档: 数据结构/
│   ├── kernel_type.h
│   ├── kernel_base_types.h
│   ├── kernel_common.h
│   ├── kernel_tensor.h
│   ├── kernel_operator_list_tensor_intf.h
│   ├── kernel_struct_aipp.h
│   ├── kernel_struct_binary.h
│   ├── kernel_struct_brcb.h
│   ├── kernel_struct_conv2d.h
│   ├── kernel_struct_data_copy.h
│   ├── kernel_struct_fixpipe.h
│   ├── kernel_struct_gather.h
│   ├── kernel_struct_mm.h
│   ├── kernel_struct_proposal.h
│   ├── kernel_struct_transpose.h
│   ├── kernel_struct_unary.h
│   └── kernel_struct_vdeq.h
│
├── 10-标量计算/                              ← 对应文档: 标量计算/
│   └── kernel_operator_scalar_intf.h
│
├── 11-特殊寄存器访问/                         ← 对应文档: 特殊寄存器访问/
│   └── kernel_operator_sys_var_intf.h
│
├── 12-矩阵计算/                               ← 对应文档: 矩阵计算（ISASI）+ 矩阵计算（TensorAPI）
│   ├── kernel_cube_intf.h
│   ├── kernel_operator_mm_intf.h
│   ├── kernel_operator_gemm_intf.h
│   ├── kernel_operator_mm_bitmode_intf.h
│   ├── kernel_operator_conv2d_intf.h
│   └── kernel_operator_fixpipe_intf.h
│
├── 13-缓存控制/                               ← 对应文档: 缓存控制/
│   └── kernel_operator_cache_intf.h
│
├── 14-调试接口/                               ← 对应文档: 调试接口/
│   ├── kernel_operator_dump_tensor_intf.h
│   └── kernel_prof_trace_intf.h
│
├── 15-资源管理/                               ← 对应文档: 资源管理/
│   ├── kernel_tpipe.h
│   └── kernel_operator_swap_mem_intf.h
│
└── 16-op_frame（无对应文档）                   ← 缺失文档，暂独立
    └── elemwise_frame.h
```

---

## 四、与文档目录结构对比

| 文档目录 | 划入头文件数 | 状态 |
|----------|:---:|:---:|
| Cube分组管理（ISASI）/ | 2 | ✅ 一致 |
| Kernel-Tiling/ | 3 | ✅ 一致 |
| Memory矢量计算/ | 19 | ✅ 一致 |
| Reg矢量计算/ | 22 | ✅ 一致 |
| 原子操作/ | 2 | ✅ 一致 |
| 同步控制/ | 2 | ✅ 一致 |
| 工具接口/ | 2 | ✅ 一致 |
| 数据搬运导览/ | 1 | ✅ 一致 |
| 数据结构/ | 17 | ✅ 一致 |
| 标量计算/ | 1 | ✅ 一致 |
| 特殊寄存器访问/ | 1 | ✅ 一致 |
| 矩阵计算（ISASI）/ + 矩阵计算（TensorAPI）/ | 6 | ✅ 一致（合并为一目录） |
| 缓存控制/ | 1 | ✅ 一致 |
| 调试接口/ | 1 | ✅ 一致 |
| 资源管理/ | 2 | ✅ 一致 |
| (op_frame 缺文档) | 1 | ❌ 需补充文档 |

**划分原则说明：**
- 文档中 `矩阵计算（ISASI）/` 和 `矩阵计算（TensorAPI）/` 是两个独立目录，但对应同一组头文件（ISASI API 和 TensorAPI 共享声明），合并为 `矩阵计算/`
- `kernel_prof_trace_intf.h` 按功能归入 `调试接口/`（性能统计相关），与文档 `调试接口/性能统计/` 吻合
- `kernel_operator_proposal_intf.h` 归入 `Memory矢量计算/`（文档中 `排序组合（ISASI）/` 子章节使用）
