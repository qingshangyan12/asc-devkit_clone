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
- `DumpTensor`（`__gm__/__ubuf__/__cbuf__/__cc__/__ca__/__cb__`）
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
| Ascend910B3 (dav-2201) | 任务书矩阵 **PASS=34** | `artifacts/910b_taskbook_20260917_170022/` |
| Ascend950 (dav-3510) 主通路 | **PASS=49** | `artifacts/950_cube_matrix_20260916_152136/` |
| Ascend950 MX (`mx_only`) | **PASS=14**（含 mmad_mx s1–8） | `artifacts/950_taskbook_20260918_185853/` |
| Ascend950 全量 taskbook | **PASS=46** FAIL=0（含 **mmad:5–7** + **mmad_mx:5–8**） | `artifacts/950_taskbook_20260918_194602/` |
| P1-B fp8 SplitB | `766fa1fb`：3510 上 `SplitB` 改 `LoadData2DParamsV2` | 已随 `194602` 验证 |

```bash
export CUBE_MATRIX_MODE=mx_only   # 或 taskbook
bash scripts/950_run_taskbook.sh
```

SKIP（xlsx 豁免）：`PopStackBuffer`、`SetAddrWithOffset`。

## 账号与交付

| 项 | 链接 / 状态 |
|----|-------------|
| Fork / 分支 | `baibai_Uu/asc-devkit` @ `feature/cube-basic-api-pointerization` |
| 合入 PR | [MR #6176](https://gitcode.com/cann/asc-devkit/merge_requests/6176)（已去 WIP · mergeable） |
| 易用性 Issue | [#1733](https://gitcode.com/cann/asc-devkit/issues/1733) |
| 设计 Issue | [#1675](https://gitcode.com/cann/asc-devkit/issues/1675) |
| Ascend-CANN 协作者 | **已邀请**（Developer） |
