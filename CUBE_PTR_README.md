# Ascend C Basic API 指针化扩展（CUBE）

## 变更说明

为 CUBE（矩阵 / ISASI）类 Basic API 增加硬件指针入参重载，使 `__ca__` / `__cb__` / `__cc__` / `__cbuf__` / `__gm__` / `__ubuf__` 裸指针可与原有 `LocalTensor` / `GlobalTensor` 双轨调用同一套 API。底层仍复用既有 `*Cal` / `*Impl`，不改变数值语义。

## 新增 / 修改文件

| 路径 | 说明 |
|------|------|
| `include/basic_api/kernel_operator_ptr_utils.h` | `GetUnderlyingPtr` / `ElemType` |
| `impl/basic_api/kernel_operator_mm_ptr_impl.h` | LoadData* / Mmad* / InitConstValue / LoadImageToLocal / LoadDataUnzip |
| `impl/basic_api/kernel_operator_fixpipe_ptr_impl.h` | Fixpipe / SetFixPipeConfig / SetFixPipeAddr |
| `impl/basic_api/kernel_operator_{dump,block_sync,determine_compute_sync,common}_ptr_impl.h` | Dump / Sync / Determine / InitOutput |
| `examples/.../03_matrix_compute/**`、`05_sync_control/**` | 样例指针路径调用（DataCopy 仍用 Tensor） |

## 覆盖 API（样例实测调用）

- `LoadData`（含 MX）/ `LoadDataWithStride` / `LoadDataWithTranspose` / `LoadDataUnzip`
- `Mmad` / `MmadMx` / `MmadWithSparse`
- `Fixpipe`（含 Arch3510 `ExtractCacheMode`）/ `SetFixPipeConfig` / `SetFixPipeAddr`
- `InitConstValue` / `InitOutput`
- `DumpTensor`（`__cbuf__` 等）
- `IBSet` / `IBWait` / Soft `SyncAll` / `InitDetermineComputeWorkspace` / `WaitPreBlock` / `NotifyNextBlock`

官方范围以 [discussions/27](https://gitcode.com/cann/asc-devkit/discussions/27) xlsx 为准（60 签名 / 19 名；PopStackBuffer / SetAddrWithOffset 预期豁免）。

## 调用示例

```cpp
AscendC::LoadData((__ca__ half*)a2.GetPhyAddr(), (__cbuf__ half*)a1.GetPhyAddr(), load2dParams);
AscendC::Mmad((__cc__ float*)c.GetPhyAddr(), (__ca__ half*)a2.GetPhyAddr(),
              (__cb__ half*)b2.GetPhyAddr(), mmadParams);
AscendC::Fixpipe((__gm__ float*)cGm.GetPhyAddr(), (__cc__ float*)c.GetPhyAddr(), fixpipeParams);
```

## 自测

| 平台 | 结果 | 产物 |
|------|------|------|
| Ascend910B3 (dav-2201) | 任务书矩阵 GREEN | `artifacts/910b_taskbook_20260917_170022/` |
| Ascend950 (dav-3510) 主通路 | 49/49 GREEN（不含 MX） | `artifacts/950_cube_matrix*` |
| Ascend950 MX | 进行中（verify 曾 FAIL；已修 deps 硬失败 + Fixpipe cacheMode + dual Tensor 诊断） | 等 `coord/ascend950` outbox |

```bash
# 950 一次性 bootstrap：见仓库外 docs/950_USER_ONE_SHOT.md
export CUBE_MATRIX_MODE=mx_only   # 或 taskbook
bash scripts/950_run_taskbook.sh
```

## 账号

Fork：`baibai_Uu/asc-devkit`（private）  
**验收分支：** `feature/cube-basic-api-pointerization`  
合入目标：`cann/asc-devkit` master（旧 MR #6066 closed，待 MX 绿后重开）
