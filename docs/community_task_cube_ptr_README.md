# Ascend C Basic API CUBE 指针化 — 验收复现说明（hongwei-2026）

## 仓库信息

| 项 | 内容 |
|----|------|
| Fork | https://gitcode.com/hongwei-2026/asc-devkit |
| 分支 | `feat/basic-api-cube-ptr-hongwei-2026` |
| tip | `a27c6100`（以分支 HEAD 为准） |
| 上游 PR | https://gitcode.com/cann/asc-devkit/merge_requests/6111 |
| 基线 | 最新 `master`（开发时对照 `v9.1.0`） |
| 硬件 / CANN | Ascend950PR / CANN 9.1.0 / `-DCMAKE_ASC_ARCHITECTURES=dav-3510` |
| 变更目录 | `include/basic_api/`、`impl/basic_api/` |

English: [community_task_cube_ptr_README_en.md](./community_task_cube_ptr_README_en.md)

## 邀请开发者

仓库设置 → 成员管理 → 添加 **`Ascend-CANN`**，角色 **Developer**。

## 环境

```bash
source /usr/local/Ascend/cann-9.1.0/asc/*/set_env.sh 2>/dev/null || true
export ASC=/usr/local/Ascend/cann-9.1.0/asc
cd ~/cube_basic_api_dev/asc-devkit
git checkout feat/basic-api-cube-ptr-hongwei-2026
```

**必须**：样例构建前把仓内头同步到系统 ASC：

```bash
cp -a include/basic_api/* "$ASC/include/basic_api/"
cp -a impl/basic_api/* "$ASC/impl/basic_api/"
```

## 核心自测（hidevlab 已通过）

| 场景 | 样例 | 结果 |
|------|------|------|
| LoadData 2DV2 指针 | load_data_2dv2_l12l0 | PASS |
| Mmad 指针 SCENARIO=1/2 | mmad | PASS |
| Fixpipe L0C→GM/L1/UB | fixpipe_l0c2gm / l1 / ub | PASS |
| LoadData 3DV2 | mmad_load3dv2 S4 | PASS |
| MmadMx FP8 | mmad_mx S3 | PASS |

## 已知限制

1. 裸 `__gm__*` Fixpipe/LoadData：`cacheMode=0`（Issue #1686）
2. Sparse 等 `__NPU_ARCH__==2201` 接口：950 登记跳过
3. Dump / SPM：无官方可改指针样例，台账跳过

## 关联

- 设计文档 competitions !1594
- 设计归档 Issue：https://gitcode.com/cann/asc-devkit/issues/1685
- 易用性 Issue：https://gitcode.com/cann/asc-devkit/issues/1686
- 签名台账：[cube_ptr_signature_ledger.md](./cube_ptr_signature_ledger.md)
