# 基础 API 到 C API 的迁移记录

## 已迁移样例

| 原基础 API 样例 | C API 样例 | 迁移方式 | API 证据 |
| --- | --- | --- | --- |
| `00_data_movement/copy_ub2ub` | `00_data_movement/copy_ub2ub` | 逐结果等价重写 | `asc_copy_gm2ub_sync`、`asc_copy_ub2ub_sync`、`asc_copy_ub2gm_sync` |
| `00_data_movement/data_copy_gm2l1` | `00_data_movement/data_copy_gm2l1` | 完整场景等价重写（Nz、ND、DN、Bias、Vector Quant） | `asc_copy_gm2l1`、`asc_copy_gm2l1_{nd2nz,dn2nz}`、`asc_copy_l12l0{a,b}_transpose`、`asc_copy_l12bt`、`asc_mmad`、`asc_copy_l12fb`、`asc_copy_l0c2gm` |
| `00_data_movement/data_copy_gm2ub_nddma` | `00_data_movement/data_copy_gm2ub_nddma` | 直接 C API 重写（二维固定值 Padding） | `asc_set_ndim_loop{0..4}_stride`、`asc_set_ndim_pad_value`、`asc_set_ndim_pad_count`、`asc_ndim_copy_gm2ub` |
| `00_data_movement/reg_auxscalar` | `00_data_movement/reg_auxscalar` | 直接 C API Reg 重写 | `asc_load`、`asc_add_scalar`、`asc_store`、`asc_create_mask_b16` |
| `01_memory_vector_compute/element_wise_arithmetic` | `01_memory_vector_compute/element_wise_arithmetic` | 功能等价重写 | `asc_copy_gm2ub_sync`、`asc_copy_ub2gm_sync`、`asc_sync_pipe` |
| `04_memory_management/tensor_allocation`（场景4） | `02_reg_vector_compute/vf_add` | VF Add场景等价重写 | `asc_get_vf_len`、`asc_create_mask_b32`、`asc_load`、`asc_add`、`asc_store`、`asc_copy_gm2ub_sync`、`asc_copy_ub2gm_sync` |

## 不确定项与处理

| 项目 | 证据 | 处理 |
| --- | --- | --- |
| `asc_leakyrelu` 在目标产品上的可用性 | `docs/zh/api/SIMD-API/C-API/vector_compute/asc_leakyrelu.md` 标示 Ascend 950PR/950DT 不支持；`impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_leakyrelu_impl.h` 却存在内部实现。 | 不调用该 C API。保留 LeakyReLU 功能，采用 UB 数组逐元素计算；GM↔UB 和流水线收敛仍使用文档化 C API。需要产品侧确认后，才可替换为 `asc_leakyrelu[_sync]`。 |
| C++ 管线/对象类接口 | `TensorAPI`、`TPipe`、`TQue`、`TBufPool`、`ListTensorDesc`、`DataCopyPad`、`CrossCoreWaitFlag` 等未找到 C API 等价文档。 | 对涉及这些接口的原样例按功能而非对象模型重写；每个样例会单独补充映射说明。 |
| `data_copy_gm2ub_nddma` 的其余四个场景 | 原样例还包括最近值 Padding、转置、广播和切片。`asc_ndim_copy_gm2ub` 文档包含这些模式，但原 `NdDmaLoopInfo` 字段到 C API 五维 stride/padding 寄存器的精确映射未在文档中给出。 | 先迁移文档可直接核对的二维固定值 Padding 场景；其余四个场景保留为待映射项，不猜测寄存器参数。 |
| `data_copy_gm2l1` 的 Bias 到 BTable 参数 | C API 文档和 `03_c_api/03_matrix_compute/mmad/mmad_s1.h` 均使用`asc_copy_l12bt`的16B目标地址单位；原基础 API 通过`SplitBias`隐式管理该布局。 | Bias样例固定使用BTable地址0，按32B DataBlock计算并传入1024B Bias；该换算已静态核对，未在当前机器执行。 |
| `tensor_allocation` 场景4的 `asc_vf_call<AddVF>` | C API未提供基础 API `asc_vf_call` 的直接等价封装；C API Reg接口声明为`__simd_callee__`，现有C API Add样例直接从`__simd_vf__`辅助函数调用。 | 在`vf_add`中保留`__simd_vf__`分层，直接顺序调用`asc_load`、`asc_add`、`asc_store`。`dav-3510`的VL为256B，1024个float正好是16个VL，不需尾块掩码。 |

## API 证据日志

| API | 文档 | 声明 | 选用形式 | 目标 | 关键约束 |
| --- | --- | --- | --- | --- | --- |
| `asc_copy_gm2ub_sync` | `docs/zh/api/SIMD-API/C-API/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md` | `include/c_api/vector_datamove/vector_datamove.h` | `(__ubuf__ void*, __gm__ void*, uint32_t)` | `dav-3510`，half | 大小和起始地址按 32 字节对齐。 |
| `asc_copy_ub2gm_sync` | `docs/zh/api/SIMD-API/C-API/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md` | `include/c_api/vector_datamove/vector_datamove.h` | `(__gm__ void*, __ubuf__ void*, uint32_t)` | `dav-3510`，half | 大小和起始地址按 32 字节对齐。 |
| `asc_copy_ub2ub_sync` | `docs/zh/api/SIMD-API/C-API/vector_datamove/asc_copy_ub2ub.md` | `include/c_api/vector_datamove/vector_datamove.h` | `(__ubuf__ void*, __ubuf__ void*, uint32_t)` 和 `(uint16_t, uint16_t, uint16_t, uint16_t)` | `dav-3510`，int32 | 前 n 个数据搬运大小按 32 字节对齐；二维形式的长度、间隔单位为 32 字节 DataBlock。 |
| `asc_copy_gm2l1` / `asc_copy_gm2l1_nd2nz` / `asc_copy_gm2l1_dn2nz` | `docs/zh/api/SIMD-API/C-API/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md`、`asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md`、`asc_copy_gm2l1_dn2nz/asc_copy_gm2l1_dn2nz_arch_3510.md` | `include/c_api/cube_datamove/cube_datamove.h` | 连续Nz、ND2NZ、DN2NZ | `dav-3510`，half/float | 场景1直接搬运Nz；场景2/4/5为ND2NZ；场景3为DN2NZ；Bias按一行float ND2NZ。 |
| `asc_copy_l12l0a` / `asc_copy_l12l0b_transpose` / `asc_copy_l12bt` | 对应 `cube_datamove` C API 文档 | `include/c_api/cube_datamove/cube_datamove.h` | L1→L0A、L1 Nz→L0B Zn、L1→BTable | `dav-3510` | Bias先进入L1，再以BTable地址0供矩阵乘使用。 |
| `asc_mmad` / `asc_copy_l0c2gm` / `asc_copy_l12fb` | 对应 `cube_compute`、`cube_datamove` C API 文档 | `include/c_api/cube_compute/cube_compute.h`、`include/c_api/cube_datamove/cube_datamove.h` | half×half→float、float/Vector Quant输出 | `dav-3510` | 场景4设置`c_matrix_source`；场景5用2048B量化参数，经Fixpipe Buffer提供给`VQF322B8_PRE`。 |
| `asc_ndim_copy_gm2ub` | `docs/zh/api/SIMD-API/C-API/vector_datamove/asc_ndim_copy_gm2ub.md` | `include/c_api/vector_datamove/vector_datamove.h` | `(__ubuf__ float*, __gm__ float*, 5 个 loop size, loop0 padding, mode, L2 mode)` | `dav-3510`，float | 维度 stride 和第 1 至 4 维 padding 必须先经对应 setter 写入。 |
| `asc_sync_notify` / `asc_sync_wait` | `docs/zh/api/SIMD-API/C-API/sync/asc_sync_notify.md`、`asc_sync_wait.md` | `include/c_api/sync/sync.h` | `(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)` | `dav-3510` | 使 UB→GM 搬运等待多维 GM→UB 搬运完成。 |
| `asc_load` / `asc_store` | `docs/zh/api/SIMD-API/C-API/reg/reg_load/asc_load.md`、`reg_store/asc_store.md` | `include/c_api/reg_compute/reg_load.h`、`reg_store.h` | `vector_half` 与 `__ubuf__ half*` | `dav-3510`，half | 每次搬运 VL 个元素；连续搬运需要手动更新地址。 |
| `asc_add_scalar` | `docs/zh/api/SIMD-API/C-API/reg/arithmetic_compute/asc_add_scalar.md` | `include/c_api/reg_compute/reg_vector.h` | `(vector_half&, vector_half, half, vector_bool)` | `dav-3510`，half | mask 未覆盖的输出元素归零。 |
| `asc_create_mask_b16` / `asc_get_vf_len` | `docs/zh/api/SIMD-API/C-API/reg/reg_data_types/asc_create_mask.md`、`sys_var/asc_get_vf_len.md` | `include/c_api/reg_compute/reg_vector.h`、`sys_var/sys_var.h` | `PAT_ALL`，运行时 VL 查询 | `dav-3510`，half | b16 全掩码覆盖一个 VL；本样例长度为 VL 的整倍数。 |
| `asc_sync_pipe` | `docs/zh/api/SIMD-API/C-API/sync/asc_sync_pipe.md` | `include/c_api/sync/sync.h` | `(PIPE_ALL)` | `dav-3510` | `PIPE_ALL` 会等待全部流水，仅用作核结束收敛。 |
| `asc_load` / `asc_add` / `asc_store` | `docs/zh/api/SIMD-API/C-API/reg/reg_load/asc_load.md`、`reg/arithmetic_compute/asc_add.md`、`reg/reg_store/asc_store.md` | `include/c_api/reg_compute/reg_load.h`、`reg_vector.h`、`reg_store.h` | `vector_float`与`__ubuf__ float*` | `dav-3510`，float | 每轮处理一个VL；Add使用b32全掩码；样例长度1024能被float VL整除。 |
| `asc_create_mask_b32` / `asc_get_vf_len` | `docs/zh/api/SIMD-API/C-API/reg/reg_data_types/asc_create_mask.md`、`sys_var/asc_get_vf_len.md` | `include/c_api/reg_compute/reg_vector.h`、`sys_var/sys_var.h` | `PAT_ALL`、无参数 | `dav-3510`，float | `asc_get_vf_len()`为256B，float VL为64元素。 |
