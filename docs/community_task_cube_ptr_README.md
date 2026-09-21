# Ascend C Basic API CUBE 指针化 — 验收复现说明（Jessicaruan）

## 仓库信息

| 项 | 内容 |
|----|------|
| 账号 | Jessicaruan（`gcw_DmEcQNNx`） |
| Fork | https://gitcode.com/gcw_DmEcQNNx/asc-devkit |
| **合入唯一入口（伞 PR）** | （开 PR 后填）`cann/asc-devkit#????` |
| 全集 tip 分支 | `feat/cube-ptr-pr4-gm-bitmode`（PR0–4 累加；伞分支在其上加 docs） |
| 历史分波（已 CI 绿，评审请看伞 PR） | #6177 #6196 #6218 #6266 #6268 |
| 基线 | CANN 9.1.0 · 对照 `master` |
| 硬件 | Ascend950PR · `-DCMAKE_ASC_ARCHITECTURES=dav-3510` |
| 变更目录 | `include/basic_api/`、`impl/basic_api/`（+ 本 docs） |

English: [community_task_cube_ptr_README_en.md](./community_task_cube_ptr_README_en.md)

## 方法（与任务书一致）

- **显式硬件地址空间指针重载** → 直调既有 `*Cal` / `*Impl`，不改数值语义。  
- **不**采用抹掉地址空间的统一 `GetUnderlyingPtr` 作为主路径（保留 `__gm__`/`__cbuf__`/`__ca__`/`__cb__`/`__cc__`/`__ubuf__` 限定符）。  
- Tensor 原重载保留；950 上 MODE0（Tensor）与 MODE1（指针）对拍。

## 环境同步（复现前必须）

```bash
source /usr/local/Ascend/cann-9.1.0/asc/*/set_env.sh 2>/dev/null || true
export ASC=/usr/local/Ascend/cann-9.1.0/asc
# checkout 伞分支或 feat/cube-ptr-pr4-gm-bitmode 后：
cp -a include/basic_api/.  "$ASC/include/basic_api/"
cp -a impl/basic_api/.     "$ASC/impl/basic_api/"
```

## 真机自测摘要（hidevlab · 证据在 IT zip）

| ID | 覆盖 | 结论 |
|----|------|------|
| E1 | LoadData 2DV2 L1→L0 | MODE0/1 **BIT-EXACT** + `.aicore.o` 一致 |
| E2 | Mmad + Fixpipe L0C→GM | **BIT-EXACT** |
| E3/E4 | Fixpipe →L1 / →UB | **BIT-EXACT** |
| E5 | MmadMx FP8 | **BIT-EXACT** |
| E6 | Mmad + `__biasbuf__` | **BIT-EXACT** |
| E7 | LoadDataWithStride 3DV2 | **BIT-EXACT** + `.aicore.o` 一致 |
| E8 | Mmad **BitMode** + bias | **BIT-EXACT** |
| E9 | LoadData 2DV2 **BitMode** | **BIT-EXACT** |
| E10 | LoadData GM→L1 | 烟测 EXIT 0（3510 不暴露 GM→L0） |
| E11 | MmadMx **BitMode** | **BIT-EXACT** |
| Neg | 错误地址空间限定符 | **5/5 expect-fail** |

完整 bin/txt/mcmp：任务广场 IT 包 `Jessicaruan_CUBE_ptr_IT_deliverables.zip`。

## 已知限制

1. 裸 `__gm__*`：`cacheMode=0`（Issue #1744；同类 #1686）  
2. Sparse 等仅 `__NPU_ARCH__==2201`：950 台账 SKIP  
3. Dump / 部分 SPM：底层仍依赖 Tensor Position → 登记缓做，不标 PASS

## 关联

- 设计：competitions !1657 · Issue #1711  
- 易用性：#1744 · 讨论区 #27  
- 签名台账：[cube_ptr_signature_ledger.md](./cube_ptr_signature_ledger.md)
