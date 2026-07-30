# 搬运类样例迁移状态

| 原基础 API 样例 | 状态 | 说明 |
| --- | --- | --- |
| `copy_ub2ub` | 已迁移 | C API 连续与二维 UB 搬运。 |
| `data_copy_gm2ub_nddma` | 部分迁移 | 已迁移可由 C API 文档直接核对的二维固定值 Padding。 |
| `data_copy_gm2l1` | 已迁移 | 使用 C API 的 GM→L1、L1→UB 与 UB→GM 通路，提供可回写观察的最小端到端样例。 |
| `data_copy_l0c2gm` | 已有等价样例 | 见仓内 `examples/02_simd_c_api/03_c_api/00_data_movement/data_copy_l0c2gm`；后续纳入本迁移目录。 |
| `data_copy_ub2l1` | 已有等价样例 | 见仓内 `examples/02_simd_c_api/03_c_api/00_data_movement/data_copy_ub2l1`；后续纳入本迁移目录。 |
| `data_copy_gm2ub_slice` | 待迁移 | 需要核对基础 `SliceInfo` 到 C API ndim 配置的映射。 |
| `data_copy_pad_gm2ub_ub2gm` | 待迁移 | C API 无 `DataCopyPad` 同名接口，计划用 ndim Padding 组合重写。 |
| `reg_auxscalar` | 已迁移 | C API `asc_loadalign`、`asc_add_scalar`、`asc_storealign`及b16全掩码。 |
| `broadcast_ub2l0c` | 待迁移 | 需要核对 UB 到 L0C 广播的 C API 等价接口。 |
| `copy_in_tensor_api` / `copy_out_tensor_api` | 待迁移 | TensorAPI 无 C API 对应，需按矩阵数据通路重写。 |
| `reg_*` | 待迁移 | 依次使用 C API reg load/store、gather、scatter 和 unaligned 接口重写。 |

所有状态均只代表静态迁移进度，不代表已经在设备上验证。
