# Ascend C Basic API CUBE 指针化 — 验收复现说明（hongwei-2026）

## 仓库信息

| 项 | 内容 |
|----|------|
| Fork | https://gitcode.com/hongwei-2026/asc-devkit |
| 分支 | `feat/basic-api-cube-ptr-hongwei-2026` |
| tip | `5b68f538`（以分支 HEAD 为准） |
| 上游 PR | https://gitcode.com/cann/asc-devkit/merge_requests/6109 |
| 基线 tag | `v9.1.0` |
| 硬件 / CANN | Ascend950PR / CANN 9.1.0 / `-DCMAKE_ASC_ARCHITECTURES=dav-3510` |
| 变更目录 | `include/basic_api/`、`impl/basic_api/` |

## 邀请开发者

仓库设置 → 成员管理 → 添加 **`Ascend-CANN`**，角色 **Developer**。  
（API 曾返回「用户已被添加」；若成员列表未见，请在网页再确认一次。）

## 环境

```bash
source /usr/local/Ascend/cann-9.1.0/asc/*/set_env.sh 2>/dev/null || true
export ASC=/usr/local/Ascend/cann-9.1.0/asc
cd ~/cube_basic_api_dev/asc-devkit   # 或本 fork clone
git checkout feat/basic-api-cube-ptr-hongwei-2026
```

**必须**：样例构建前把仓内头同步到系统 ASC（否则仍用旧 Tensor-only 头）：

```bash
cp -a include/basic_api/* "$ASC/include/basic_api/"
cp -a impl/basic_api/* "$ASC/impl/basic_api/"
```

或使用交付脚本：

```bash
bash verify_cube_ptr.sh --sync
bash verify_cube_ptr.sh --run CORE
```

## 核心自测（已在 hidevlab 通过）

| 场景 | 样例 | 结果 |
|------|------|------|
| LoadData 2DV2 指针 | load_data_2dv2_l12l0 | PASS |
| Mmad 指针 SCENARIO=1 | mmad | PASS |
| Fixpipe L0C→GM 指针 S1 | fixpipe_l0c2gm | PASS |

详细表与截图见交付包 `deliverables/results/`、`deliverables/screenshots/`。

## 已知限制

1. 裸 `__gm__*` Fixpipe/LoadData：`cacheMode=0`（见 Issue #1686）
2. Sparse 等 `__NPU_ARCH__==2201` 接口：950 登记跳过
3. 3D / MX / SPM / Dump：部分签名仍在扩展台账中，见 `GAP_REMAINING.md`

## 关联

- 设计文档 competitions !1594
- 设计归档 Issue：https://gitcode.com/cann/asc-devkit/issues/1685
- 易用性 Issue：https://gitcode.com/cann/asc-devkit/issues/1686
