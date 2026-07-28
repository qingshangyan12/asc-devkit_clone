# 基础 API 到 C API 的迁移记录

## 已迁移样例

| 原基础 API 样例 | C API 样例 | 迁移方式 | API 证据 |
| --- | --- | --- | --- |
| `00_data_movement/copy_ub2ub` | `00_data_movement/copy_ub2ub` | 逐结果等价重写 | `asc_copy_gm2ub_sync`、`asc_copy_ub2ub_sync`、`asc_copy_ub2gm_sync` |
| `00_data_movement/data_copy_gm2l1` | `00_data_movement/data_copy_gm2l1` | 直接 C API 重写 | `asc_copy_gm2l1_sync`、`asc_copy_l12ub_sync`、`asc_copy_ub2gm_sync` |
| `00_data_movement/data_copy_gm2ub_nddma` | `00_data_movement/data_copy_gm2ub_nddma` | 直接 C API 重写（二维固定值 Padding） | `asc_set_ndim_loop{0..4}_stride`、`asc_set_ndim_pad_value`、`asc_set_ndim_pad_count`、`asc_ndim_copy_gm2ub` |
| `00_data_movement/reg_auxscalar` | `00_data_movement/reg_auxscalar` | 直接 C API Reg 重写 | `asc_load`、`asc_add_scalar`、`asc_store`、`asc_create_mask_b16` |
| `01_memory_vector_compute/element_wise_arithmetic` | `01_memory_vector_compute/element_wise_arithmetic` | 功能等价重写 | `asc_copy_gm2ub_sync`、`asc_copy_ub2gm_sync`、`asc_sync_pipe` |

## 不确定项与处理

| 项目 | 证据 | 处理 |
| --- | --- | --- |
| `asc_leakyrelu` 在目标产品上的可用性 | `docs/zh/api/SIMD-API/C-API/vector_compute/asc_leakyrelu.md` 标示 Ascend 950PR/950DT 不支持；`impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_leakyrelu_impl.h` 却存在内部实现。 | 不调用该 C API。保留 LeakyReLU 功能，采用 UB 数组逐元素计算；GM↔UB 和流水线收敛仍使用文档化 C API。需要产品侧确认后，才可替换为 `asc_leakyrelu[_sync]`。 |
| C++ 管线/对象类接口 | `TensorAPI`、`TPipe`、`TQue`、`TBufPool`、`ListTensorDesc`、`DataCopyPad`、`CrossCoreWaitFlag` 等未找到 C API 等价文档。 | 对涉及这些接口的原样例按功能而非对象模型重写；每个样例会单独补充映射说明。 |
| `data_copy_gm2ub_nddma` 的其余四个场景 | 原样例还包括最近值 Padding、转置、广播和切片。`asc_ndim_copy_gm2ub` 文档包含这些模式，但原 `NdDmaLoopInfo` 字段到 C API 五维 stride/padding 寄存器的精确映射未在文档中给出。 | 先迁移文档可直接核对的二维固定值 Padding 场景；其余四个场景保留为待映射项，不猜测寄存器参数。 |

## API 证据日志

| API | 文档 | 声明 | 选用形式 | 目标 | 关键约束 |
| --- | --- | --- | --- | --- | --- |
| `asc_copy_gm2ub_sync` | `docs/zh/api/SIMD-API/C-API/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md` | `include/c_api/vector_datamove/vector_datamove.h` | `(__ubuf__ void*, __gm__ void*, uint32_t)` | `dav-3510`，half | 大小和起始地址按 32 字节对齐。 |
| `asc_copy_ub2gm_sync` | `docs/zh/api/SIMD-API/C-API/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md` | `include/c_api/vector_datamove/vector_datamove.h` | `(__gm__ void*, __ubuf__ void*, uint32_t)` | `dav-3510`，half | 大小和起始地址按 32 字节对齐。 |
| `asc_copy_ub2ub_sync` | `docs/zh/api/SIMD-API/C-API/vector_datamove/asc_copy_ub2ub.md` | `include/c_api/vector_datamove/vector_datamove.h` | `(__ubuf__ void*, __ubuf__ void*, uint32_t)` 和 `(uint16_t, uint16_t, uint16_t, uint16_t)` | `dav-3510`，int32 | 前 n 个数据搬运大小按 32 字节对齐；二维形式的长度、间隔单位为 32 字节 DataBlock。 |
| `asc_copy_gm2l1_sync` | `docs/zh/api/SIMD-API/C-API/cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md` | `include/c_api/cube_datamove/cube_datamove.h` | `(__cbuf__ void*, __gm__ void*, uint32_t)` | `dav-3510`，half | 连续搬运大小为 1024 字节，满足 32 字节 DataBlock 对齐。 |
| `asc_copy_l12ub_sync` | `docs/zh/api/SIMD-API/C-API/cube_datamove/asc_copy_l12ub.md` | `include/c_api/cube_datamove/cube_datamove.h` | `(__ubuf__ void*, __cbuf__ void*, bool, uint16_t, uint16_t, uint16_t, uint16_t)` | `dav-3510`，half | 该接口跨 Cube/Vector 核，kernel 入口不使用 `__cube__` 或 `__vector__` 修饰。 |
| `asc_ndim_copy_gm2ub` | `docs/zh/api/SIMD-API/C-API/vector_datamove/asc_ndim_copy_gm2ub.md` | `include/c_api/vector_datamove/vector_datamove.h` | `(__ubuf__ float*, __gm__ float*, 5 个 loop size, loop0 padding, mode, L2 mode)` | `dav-3510`，float | 维度 stride 和第 1 至 4 维 padding 必须先经对应 setter 写入。 |
| `asc_sync_notify` / `asc_sync_wait` | `docs/zh/api/SIMD-API/C-API/sync/asc_sync_notify.md`、`asc_sync_wait.md` | `include/c_api/sync/sync.h` | `(PIPE_MTE2, PIPE_MTE3, EVENT_ID0)` | `dav-3510` | 使 UB→GM 搬运等待多维 GM→UB 搬运完成。 |
| `asc_load` / `asc_store` | `docs/zh/api/SIMD-API/C-API/reg/reg_load/asc_load.md`、`reg_store/asc_store.md` | `include/c_api/reg_compute/reg_load.h`、`reg_store.h` | `vector_half` 与 `__ubuf__ half*` | `dav-3510`，half | 每次搬运 VL 个元素；连续搬运需要手动更新地址。 |
| `asc_add_scalar` | `docs/zh/api/SIMD-API/C-API/reg/arithmetic_compute/asc_add_scalar.md` | `include/c_api/reg_compute/reg_vector.h` | `(vector_half&, vector_half, half, vector_bool)` | `dav-3510`，half | mask 未覆盖的输出元素归零。 |
| `asc_create_mask_b16` / `asc_get_vf_len` | `docs/zh/api/SIMD-API/C-API/reg/reg_data_types/asc_create_mask.md`、`sys_var/asc_get_vf_len.md` | `include/c_api/reg_compute/reg_vector.h`、`sys_var/sys_var.h` | `PAT_ALL`，运行时 VL 查询 | `dav-3510`，half | b16 全掩码覆盖一个 VL；本样例长度为 VL 的整倍数。 |
| `asc_sync_pipe` | `docs/zh/api/SIMD-API/C-API/sync/asc_sync_pipe.md` | `include/c_api/sync/sync.h` | `(PIPE_ALL)` | `dav-3510` | `PIPE_ALL` 会等待全部流水，仅用作核结束收敛。 |
