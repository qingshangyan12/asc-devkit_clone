# CUBE 指针化 · 签名台账（Jessicaruan · 950/dav-3510）

> 显式硬件指针重载 → `*Cal`/`*Impl`；不做 `GetUnderlyingPtr` 主路径。  
> `P`=已指针化 · `T`=真机 BIT-EXACT · `S`=950 SKIP · `W`=待真机

## A · 代表性（#6177 + #6196）

| 接口 | 状态 | 样例 |
|------|------|------|
| LoadData 2D/2DV2 L1→L0 | P+T | `load_data_2dv2_l12l0` S1 |
| Mmad ± biasbuf | P+T（无 bias 路径） | 同上链路 |
| Fixpipe L0C→GM V220/Arch3510 | P+T（Arch3510） | 同上链路 |

## B · Wave1（本 PR）

| 接口 | 状态 | 备注 |
|------|------|------|
| Fixpipe L0C→L1/UB V220+Arch3510 | P / W | 声明落在对应 arch `#if`，防 2002 host-check |
| LoadDataWithTranspose V1 L0A+L0B | P / W | 3510 上 L0A Cal=`NOT_SUPPORT`（与 Tensor 一致） |
| LoadDataWithTranspose V2 | P / W | **仅 L0B**（禁 L0A，避 hongwei P1） |
| MmadMx ± biasbuf | P / W | 仅 `__NPU_ARCH__==3510` |
| BroadCastVecToMM | P / W | 编译覆盖 |
| Fill / InitConstValue → L0A/L0B/L1 | P / W | 编译覆盖 |

## SKIP（报告写明）

| 项 | 原因 |
|----|------|
| Sparse / SetFixPipeAddr / Dump Local | 2201-only / 3510 NOT_SUPPORT / R1 |
| Fixpipe M300/M310 | 非 3510 |

## 链接

- PR-0 https://gitcode.com/cann/asc-devkit/pull/6177
- PR-1 https://gitcode.com/cann/asc-devkit/pull/6196
- 设计 #1711 · 易用性 #1744
