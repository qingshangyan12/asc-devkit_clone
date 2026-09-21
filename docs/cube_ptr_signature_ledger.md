# CUBE 指针化签名台账（Jessicaruan · 950 / dav-3510）

任务：`09-AscendC-Basic-API-CUBE` · **合入以伞 PR / `feat/cube-ptr-pr4-gm-bitmode` tip 为准**  
路线：显式 AS 指针重载 → `*Cal`/`*Impl`（**不用** `GetUnderlyingPtr` 主路径）

标记：`P`=代码已加指针重载 · `T`=950 真机 BIT-EXACT（或注明烟测）· `S`=架构 SKIP · `W`=代码有/真机未跑 · `D`=底层仍吃 Tensor

English: [cube_ptr_signature_ledger_en.md](./cube_ptr_signature_ledger_en.md)

## 已覆盖并真机钉死（P+T）

| 族 | 路径 | 证据 |
|----|------|------|
| LoadData 2D/2DV2 L1→L0 | `__cbuf__`→`__ca__/__cb__` | E1 BIT-EXACT + mcmp |
| LoadData 2DV2 BitMode | 同上 + BitMode 参数 | E9 |
| LoadData GM→L1 | `__gm__`→`__cbuf__`（cacheMode=0） | E10 烟测 EXIT 0 |
| LoadData 3DV2 / WithStride | L1→L0 + Mmad | E7 + mcmp |
| Mmad ± `__biasbuf__` | `__cc__/__ca__/__cb__` | E2/E6 |
| Mmad BitMode + bias | | E8 |
| MmadMx ± / BitMode | 3510 | E5/E11 |
| Fixpipe Arch3510 | L0C→GM / L1 / UB | E2/E3/E4 |

## 代码已有、真机仍 W（不标 PASS）

| 族 | 状态 |
|----|------|
| LoadDataWithTranspose | P / W |
| BroadCastVecToMM | P / W |
| Fill / InitConstValue | P / W |

## 登记 SKIP / 缓做

| 项 | 标记 | 原因 |
|----|------|------|
| MmadWithSparse / LoadData Sparse | S/D | 2201 only 或 Cal 仍吃 Tensor |
| GM→L0 LoadData | S | 3510 Cal `NOT_SUPPORT`（不暴露） |
| DumpTensor 指针 | D | Impl 依赖 Position/Shape |
| SPM 部分 | D | TPipe 生命周期 |
| 裸 GM 非 0 cacheMode | S | 无 ExtractCacheMode → 文档约定 0（#1744） |

## 负向编译

错误硬件地址空间限定符：**5/5 expect-fail**（IT `negative_compile/`）。

## 历史分波 PR（CI 绿，请审伞 PR）

#6177 #6196 #6218 #6266 #6268 → 由伞 PR 承接合入。
