# CUBE Pointerization Signature Ledger (hongwei-2026)

Task: `09-AscendC-Basic-API-CUBE` · tip follows fork branch HEAD  
Primary path: Ascend950PR / CANN 9.1.0 / `dav-3510`

Chinese: [cube_ptr_signature_ledger.md](./cube_ptr_signature_ledger.md)

Legend: `P`=pointer overload added · `T`=Tensor kept · `S`=arch skip · `D`=Cal still needs Tensor/Position · `N`=not covered

## Covered (P)

| Family | Pointer path | Tag |
|--------|--------------|-----|
| GetUnderlyingPtr | utils | core |
| LoadData 2D L1→L0A/B | `__ca__/__cb__` + `__cbuf__` | EXPAND |
| LoadData 2DV2 L1→L0 | same | core |
| LoadData 2DV2 GM→L0/L1 | + `__gm__` cacheMode=0 | EXPAND |
| LoadDataWithTranspose V1/V2 | L1→L0 | EXPAND |
| LoadData 3DV1/V2/WithStride/V2Pro | L0A/L0B/UB + L1 | **BGROUP** |
| Mmad ±bias | `__cc__/__ca__/__cb__` | core+EXPAND |
| MmadMx ±bias | same | **BGROUP** |
| Fixpipe V220 / Arch3510 | GM/L1/UB ±ws | EXPAND |
| Fill / InitConstValue | `__ca__/__cb__/__cbuf__` | **BGROUP** |
| BroadCastVecToMM | `__cc__` + `__ubuf__` | **BGROUP** |
| LoadDataUnzip | L1/L0A/L0B + GM | **BGROUP** |
| MmadWithSparse | 2201 only | **BGROUP** / 950=S |

## Skipped / deferred

| Item | Status | Reason |
|------|--------|--------|
| LoadDataWithSparse pointer | D | Cal still takes LocalTensor |
| LoadData 2D MX | N/D | three operands + scale |
| BitMode LoadData/Mmad | D | NTTP TPosition |
| DumpTensor pointer | D | Impl needs Tensor Position/Shape |
| SPM Init/Write/Read | D | TPipe members, not free functions |
| SetFlag/WaitFlag sync | N | no Tensor operands |
| Non-zero cacheMode bare GM | S | no ExtractCacheMode → cacheMode=0 |

## hidevlab results

| ID | Result |
|----|--------|
| C-01~C-06 core | PASS |
| E-Fixpipe L1/UB | PASS |
| B-Load3DV2 S4 / MX S3 | PASS |

## Links

- PR: https://gitcode.com/cann/asc-devkit/merge_requests/6111
- Issues: #1685 #1686
