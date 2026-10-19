# M510（npu_arch_5102）capi 测试用例集

本目录是 **M510 系列芯片** 的 Ascend C C-API（capi）单元测试。设计目标：**最大化复用 3510（npu_arch_3510）用例**，仅对 M510 与 3510 的已知差异单独标注。

> 配套文档：`docs/zh/api/SIMD-API/c_api/rfc_capi_m510_adaptation.md`

## 1. 编译期标识

M510 在编译工程中识别为 `__NPU_ARCH__ == 5102`（耦合架构 `PRE_COMPILE_KERNEL_MIX_COUPLING`，核心宏 `__DAV_M200__` / `__DAV_M200_VEC__`）。capi 侧目录命名沿用现有数字约定：**`npu_arch_5102`**。

## 2. 用例状态约定

| 状态 | 含义 |
|------|------|
| **Reuse** | 直接克隆 3510 同名用例（API/指令一致），证明复用可行 |
| **Adapt** | 需按 M510 差异改写（cube 耦合路径、expdiff 策略） |
| **Drop** | 3510 专属、M510 首期不提供（如 fp4 MX） |

## 3. 完整测试矩阵（对齐 npu_arch_3510）

| 类别（子目录） | 3510 用例 | M510 状态 | 说明 |
|----------------|-----------|-----------|------|
| `atomic/` | `test_set_atomic_add_int16.cpp`、`test_set_atomic_add_bfloat.cpp`、`test_set_atomic_min_bfloat.cpp`、`test_set_atomic_instr.cpp`、`test_set_atomic_max_int16.cpp`、`test_set_atomic_max_bfloat.cpp`、`test_set_atomic_add_int32.cpp`、`test_set_atomic_add_float16.cpp`、`test_set_atomic_add_int.cpp`、`test_set_atomic_min_float.cpp`、`test_set_atomic_max_int8.cpp`、`test_set_atomic_max_int32.cpp`、`test_disable_dma_atomic.cpp`、`test_set_atomic_add_int8.cpp`、`test_set_atomic_max_float.cpp`、`test_set_atomic_max_int.cpp`、`test_get_store_atomic_config.cpp`、`test_set_st_atomic_cfg.cpp`、`test_set_atomic_none.cpp`、`test_set_atomic_min_float16.cpp`、`test_set_atomic_min_int16.cpp`、`test_set_atomic_min_int.cpp`、`test_set_atomic_min_int8.cpp`、`test_set_atomic_min_int32.cpp` | Reuse | 原子类 API 与 3510 一致 |
| `cube_compute/` | `test_enable_fp8.cpp`、`test_enable_hf32.cpp`、`test_set_fp32_mode.cpp`、`test_mmad.cpp`、`test_set_hf32_round_mode.cpp`、`test_enable_hf32_trans.cpp`、`test_mmad_mx.cpp`、`test_enable_hif8.cpp`、`test_mmad_mx_sync.cpp`、`test_mmad_bias.cpp`、`test_set_mm_transform.cpp` | Reuse（共享 mmad 子集：`test_mmad.cpp`、`test_mmad_bias.cpp`、`test_set_mm_transform.cpp`、`test_set_fp32_mode.cpp`、`test_set_hf32_round_mode.cpp` 等） | M510 支持 mmad/fixpipe/l0c2gm 等共享算子 |
| `cube_compute/` | `test_mmad_mx.cpp`、`test_mmad_mx_bias.cpp`、`test_mmad_mx_sync.cpp` | Drop | fp4 MX 为 3510 专属，M510 首期不支持 |
| `cube_datamove/` | `test_asc_copy_gm2l1_align.cpp`、`test_asc_copy_gm2l1.cpp`、`test_asc_copy_gm2l1_dn2nz.cpp`、`test_asc_set_l0c2gm_nz2nd.cpp`、`test_copy_l0c2l1_instr.cpp`、`test_asc_set_l0c2gm_lrelu_alpha.cpp`、`test_asc_copy_l12l0b.cpp`、`test_asc_copy_gm2l1_nd2nz.cpp`、`test_asc_copy_l12l0a.cpp`、`test_asc_copy_l12ub.cpp`、`test_asc_copy_l0c2ub.cpp`、`test_asc_copy_l12fb.cpp`、`test_asc_set_l0c2gm_config.cpp`、`test_asc_copy_l12bt.cpp`、`test_asc_copy_l0c2gm.cpp`、`test_set_gm2l1_lp2_stride.cpp`、`test_set_gm2l1_lp1_stride.cpp`、`test_set_gm2l1_pad.cpp`、`test_set_l1_3d_fmatrix.cpp`、`test_fill_l1_value.cpp`、`test_set_l13d_rpt.cpp`、`test_set_gm2l1_lp_size.cpp`、`test_copy_l12l0.cpp`、`test_copy_l12l0b_trans.cpp`、`test_copy_l12l0a_mx.cpp`、`test_set_l0c_copy_prequant.cpp` | Adapt | GM→L0A、L1→L0A transpose、InitL0A、v1 LoadData 等路径 M510 不支持（见 `impl/basic_api/dav_m510/kernel_operator_mm_impl.h`）；需改为桩化/预期报错用例 |
| `cache_ctrl/` | `test_asc_data_cache_preload.cpp`、`test_dci.cpp`、`test_icache_preload.cpp`、`test_dcci.cpp`、`test_asc_scalar_cache_mode.cpp`、`test_get_icache_preload_status.cpp` | Reuse（需核对具体指令在 M510 是否一致） | |
| `misc/` | `test_asc_init.cpp`、`TestAscNopCAPI.cpp` | Reuse | |
| `scalar_compute/` | `test_asc_atomic_inc.cpp`、`test_asc_atomic_or.cpp`、`test_asc_load_dev.cpp`、`test_asc_popc.cpp`、`test_asc_atomic_xor.cpp`、`test_asc_clear_nthbit.cpp`、`test_asc_atomic_min.cpp`、`test_asc_atomic_add.cpp`、`test_asc_atomic_max.cpp`、`test_asc_float2int32.cpp`、`test_asc_atomic_sub.cpp`、`test_asc_float2half.cpp`、`test_asc_clz.cpp`、`test_asc_atomic_exch.cpp`、`test_asc_atomic_and.cpp`、`test_asc_atomic_dec.cpp`、`test_asc_atomic_cas.cpp` | Reuse | M510 走通用 scalar 路径（`dav_m510` 无 scalar 专属文件） |
| `sync/` | `test_asc_lock.cpp`、`test_asc_sync_block_arrive.cpp`、`test_asc_unlock.cpp`、`test_asc_sync_intra_arrive.cpp`、`test_asc_sync_inter_arrive.cpp`、`test_asc_sync_inter_wait.cpp`、`test_asc_sync_data_barrier.cpp`、`test_asc_sync_intra_wait.cpp`、`test_asc_sync_pipe.cpp`、`test_asc_sync_subblock_arrive.cpp`、`test_asc_sync_subblock_wait.cpp`、`test_asc_sync_block_wait.cpp` | Reuse | |
| `sys_var/` | 见 3510 `sys_var/` 全量 | Reuse | |
| `vector_compute/` | `test_return_vconv.cpp`（vconv 重载部分） | Reuse | `vcvt` 等 vector 指令 M510 共享 |
| `vector_compute/` | `test_return_vconv.cpp` 中的 `ExpSubHalfToFloatUsesVexpdif` / `ExpSubUsesVexpdif` | Adapt/Drop | M510 无 `vexpdif` 专用指令，按 RFC §4.5 决策；当前用例已拆出到 `test_exp_sub_m510_policy.cpp` 占位 |
| `vector_datamove/` | 见 3510 `vector_datamove/` 全量 | Reuse | |
| `reg_compute/` | 见 3510 `reg_compute/` 对应 | Reuse | reg 类实现共享 |

## 4. 本目录已落地用例（可直接克隆验证复用）

- `atomic/test_set_atomic_add_int16.cpp` —— Reuse（3510 克隆）
- `sync/test_asc_sync_block_wait.cpp` —— Reuse（3510 克隆）
- `sys_var/test_get_block_idx.cpp` —— Reuse（3510 克隆）
- `cube_compute/test_mmad.cpp` —— Reuse（3510 克隆，验证 cube 共享子集）
- `vector_compute/test_return_vconv.cpp` —— Reuse（克隆 3510，已移除 expdiff 两个用例）
- `vector_compute/test_exp_sub_m510_policy.cpp` —— Adapt 占位（按 §4.5 决策回填）

## 5. 如何接入与运行

依赖 RFC §7.1 的 CMake patch（`tests/api/c_api/CMakeLists.txt` 增加 `ascendm510_AIC` / `ascendm510_AIV`），并依赖外部 `tikicpulib::ascendm510` CPU 模型库就绪。

```bash
# 在 asc-devkit 构建目录下，编译并运行 M510 capi 单测（需 tikicpulib::ascendm510 就绪）
cmake --build . --target ascendc_ut_c_api_ascendm510_AIC ascendc_ut_c_api_ascendm510_AIV
ctest -R ascendc_ut_c_api_ascendm510
```

## 6. 编写新用例的约定

1. 复用 3510 用例：直接复制 `npu_arch_3510/<cat>/<name>.cpp` 到 `npu_arch_5102/<cat>/`，如 API/指令一致则无需修改。
2. 桩底层指令用 `MOCKER_CPP(<instr>, <sig>).times(1).will(invoke(<stub>))`，并对 `asc_*` 包装函数做参数/返回类型断言。
3. AIC/AIV 区分通过 `g_coreType = C_API_AIC_TYPE / C_API_AIV_TYPE`（cube 用例设 AIC，vector 用例设 AIV）。
4. M510 差异用例命名加 `_m510` 后缀，并在文件头注释标注差异来源（引用 `impl/basic_api/dav_m510/...` 或 RFC 章节）。
