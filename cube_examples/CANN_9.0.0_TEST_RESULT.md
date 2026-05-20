=== cube_examples can-9.0.0-beta.2 编译测试结果 ===

构建结果: 0/20 通过, 20/20 失败

--- 根因 ---
API 签名在两个版本间有重大变化:

9.0.0-beta.2                 → 9.1.0-beta.1
───────────────────────────────────────────────────────────────
asc_copy_gm2l1_sync          asc_copy_gm2l1_sync
  (dst,src,size)   简单线性    (dst,src,m_start,k_start,...) 2D分形

asc_copy_l12l0a_sync         asc_copy_l12l0a_sync
  (dst,src,start_idx,repeat,  (dst,src,m_start,k_start,
   src_stride,dst_gap)   旧    m_step,k_step,src_s,dst_s)  新

asc_copy_l12l0b_sync         同上

asc_mmad_sync                 asc_mmad_sync
  k_direction_align           disable_gemv

--- 9.0.0 缺失的 API (编译报 use of undeclared identifier) ---
- asc_copy_l0c2gm_sync       (9.0.0只有异步版 asc_copy_l0c2gm)
- asc_set_gm2l1_nz_para      (ND→NZ配置API)
- asc_copy_l12gm             (L1→GM搬运)
- asc_copy_l12l0a_transpose_sync (L1→L0A转置)
- asc_copy_l12l0b_transpose_sync (L1→L0B转置)
- asc_fill_l1                (存在但可能编译宏不同)

--- 结论 ---
当前重构代码仅兼容 cann-9.1.0-beta.1 (或更新版本)。
9.0.0-beta.2 的 API 接口设计不同，需要适配不同的参数集。
