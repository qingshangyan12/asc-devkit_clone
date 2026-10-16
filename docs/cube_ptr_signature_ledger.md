# CUBE 指针化签名台账（hongwei-2026）

任务：`09-AscendC-Basic-API-CUBE` · tip 以 fork 分支 HEAD 为准  
硬件主路径：Ascend950PR / CANN 9.1.0 / `dav-3510`

English: [cube_ptr_signature_ledger_en.md](./cube_ptr_signature_ledger_en.md)

说明：官方腾讯文档共 **21 API / 61 签名**；本表按源码入口归档改造状态。  
`P`=已加指针重载 · `T`=保留 Tensor · `S`=架构跳过 · `D`=底层 Cal 仍吃 Tensor/依赖 Position · `N`=未覆盖

## 已覆盖（P）

| 族 | 指针路径 | 标记 |
|----|----------|------|
| GetUnderlyingPtr | utils | 三刀起 |
| LoadData 2D L1→L0A/B | `__ca__/__cb__`+`__cbuf__` | EXPAND |
| LoadData 2DV2 L1→L0 | 同上 | 三刀 |
| LoadData 2DV2 GM→L0/L1 | +`__gm__` cacheMode=0 | EXPAND |
| LoadDataWithTranspose V1/V2 | L1→L0 | EXPAND |
| LoadData 3DV1/V2/WithStride/V2Pro | L0A/L0B/UB + L1 | **BGROUP** |
| Mmad ±bias | `__cc__/__ca__/__cb__` | 三刀+EXPAND |
| MmadMx ±bias | 同上 | **BGROUP** |
| Fixpipe V220 / Arch3510 | GM/L1/UB ±ws | EXPAND |
| Fill / InitConstValue | `__ca__/__cb__/__cbuf__` | **BGROUP** |
| BroadCastVecToMM | `__cc__`+`__ubuf__` | **BGROUP** |
| LoadDataUnzip | L1/L0A/L0B + GM | **BGROUP** |
| MmadWithSparse | 2201 only | **BGROUP** / 950=S |

## 登记跳过 / 暂缓

| 项 | 状态 | 原因 |
|----|------|------|
| LoadDataWithSparse 指针 | D | Cal 仍吃 LocalTensor |
| LoadData 2D MX | N/D | 三操作数+scale，需单独样例 |
| BitMode LoadData/Mmad | D | NTTP TPosition |
| DumpTensor 指针 | D | Impl 依赖 Tensor Position/Shape |
| SPM Init/Write/Read | D | TPipe 成员，非自由函数 |
| SetFlag/WaitFlag 等 sync | N | 无 Tensor 操作数 / 非本册搬运语义 |
| 非 0 cacheMode 裸 GM | S | 无 ExtractCacheMode → cacheMode=0 |

## 实测（hidevlab）

| ID | 结果 |
|----|------|
| C-01~C-06 核心三刀 | PASS |
| E-Fixpipe L1/UB | PASS |
| B-Load3DV2 S4 / MX S3 | PASS |

## 关联

- PR：https://gitcode.com/cann/asc-devkit/merge_requests/6111
- Issue：#1685 #1686
