# CUBE Pointerization Signature Ledger (Jessicaruan · 950 / dav-3510)

Canonical tip: umbrella PR / `feat/cube-ptr-pr4-gm-bitmode`.  
Approach: explicit AS pointer overloads → `*Cal`/`*Impl` (not `GetUnderlyingPtr` primary).

Markers: `P`=pointer overload in code · `T`=device BIT-EXACT · `S`=arch skip · `W`=code only · `D`=Cal still Tensor-bound

Chinese: [cube_ptr_signature_ledger.md](./cube_ptr_signature_ledger.md)

## P+T (device-proven)

LoadData 2D/2DV2 (+ BitMode) · GM→L1 smoke · 3DV2/Stride · Mmad ±bias (+ BitMode) · MmadMx (+ BitMode) · Fixpipe→GM/L1/UB  
Evidence IDs E1–E11 + mcmp E1/E7 + neg 5/5 in plaza IT zip.

## P/W (no PASS claimed)

Transpose · BroadCastVecToMM · Fill/InitConstValue

## S/D

Sparse (2201) · GM→L0 on 3510 · Dump/SPM Tensor-bound · bare GM cacheMode=0 (#1744)

## Historical wave PRs (CI green; review umbrella)

#6177 #6196 #6218 #6266 #6268
