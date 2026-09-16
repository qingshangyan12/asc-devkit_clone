# Ascend C Basic API CUBE Pointerization — Acceptance Notes (hongwei-2026)

## Repository

| Item | Value |
|------|------|
| Fork | https://gitcode.com/hongwei-2026/asc-devkit |
| Branch | `feat/basic-api-cube-ptr-hongwei-2026` |
| Tip | `a27c6100` (see branch HEAD) |
| Upstream PR | https://gitcode.com/cann/asc-devkit/merge_requests/6111 |
| Baseline | latest `master` (developed against `v9.1.0`) |
| Hardware / CANN | Ascend950PR / CANN 9.1.0 / `-DCMAKE_ASC_ARCHITECTURES=dav-3510` |
| Changed paths | `include/basic_api/`, `impl/basic_api/` |

Chinese: [community_task_cube_ptr_README.md](./community_task_cube_ptr_README.md)

## Invite Developer

Repo settings → Members → add **`Ascend-CANN`** as **Developer**.

## Environment

```bash
source /usr/local/Ascend/cann-9.1.0/asc/*/set_env.sh 2>/dev/null || true
export ASC=/usr/local/Ascend/cann-9.1.0/asc
cd ~/cube_basic_api_dev/asc-devkit
git checkout feat/basic-api-cube-ptr-hongwei-2026
```

**Required**: sync in-tree headers to system ASC before building examples:

```bash
cp -a include/basic_api/* "$ASC/include/basic_api/"
cp -a impl/basic_api/* "$ASC/impl/basic_api/"
```

## Core self-tests (hidevlab PASS)

| Scenario | Example | Result |
|----------|---------|--------|
| LoadData 2DV2 pointer | load_data_2dv2_l12l0 | PASS |
| Mmad pointer SCENARIO=1/2 | mmad | PASS |
| Fixpipe L0C→GM/L1/UB | fixpipe_l0c2gm / l1 / ub | PASS |
| LoadData 3DV2 | mmad_load3dv2 S4 | PASS |
| MmadMx FP8 | mmad_mx S3 | PASS |

## Known limits

1. Bare `__gm__*` Fixpipe/LoadData uses `cacheMode=0` (Issue #1686)
2. Sparse APIs under `__NPU_ARCH__==2201` are skipped on 950
3. Dump / SPM: no official pointerizable samples; ledger skip

## Links

- Design doc: competitions !1594
- Design archive Issue: https://gitcode.com/cann/asc-devkit/issues/1685
- Usability Issue: https://gitcode.com/cann/asc-devkit/issues/1686
- Signature ledger: [cube_ptr_signature_ledger_en.md](./cube_ptr_signature_ledger_en.md)
