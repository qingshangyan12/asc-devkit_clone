# Ascend C Basic API CUBE Pointerization — Repro Notes (Jessicaruan)

| Item | Value |
|------|-------|
| Account | Jessicaruan (`gcw_DmEcQNNx`) |
| Fork | https://gitcode.com/gcw_DmEcQNNx/asc-devkit |
| **Canonical PR** | (fill after open) |
| Full tip | `feat/cube-ptr-pr4-gm-bitmode` (PR0–4 cumulative) |
| HW / CANN | Ascend950PR / 9.1.0 / `dav-3510` |

Chinese: [community_task_cube_ptr_README.md](./community_task_cube_ptr_README.md)

## Approach

Explicit hardware address-space pointer overloads → existing `*Cal`/`*Impl`.  
**Not** a type-erasing `GetUnderlyingPtr` as the primary path. Tensor overloads kept.

## Sync headers before building samples

```bash
cp -a include/basic_api/.  "$ASC/include/basic_api/"
cp -a impl/basic_api/.     "$ASC/impl/basic_api/"
```

## Device evidence (hidevlab)

E1–E11: MODE0 vs MODE1 **BIT-EXACT** on representative LoadData / Mmad / Fixpipe / MmadMx paths;  
BitMode covered (E8/E9/E11); GM→L1 smoke (E10); msobjdump match on E1/E7;  
negative address-space compile **5/5** expect-fail. Full logs in the plaza IT zip.

## Limits

- Bare `__gm__*`: `cacheMode=0` (Issue #1744)  
- Sparse / some Dump-SPM: arch or Tensor-dependent → ledger SKIP/defer

## Links

Design !1657 · Issue #1711 · Usability #1744 · Ledger: [cube_ptr_signature_ledger_en.md](./cube_ptr_signature_ledger_en.md)
