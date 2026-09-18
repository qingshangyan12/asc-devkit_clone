# Ascend910B3 CUBE self-test matrix

- HEAD: 2eb83bfe3
- ARCH: dav-2201
- STAMP: 20260916_125444
- MODE: smoke

- verify PASS: **11**
- FAIL: 0

| case | scenario | arch | build | run | verify | note |
|---|---|---|---|---|---|---|
| mmad | 1 | dav-2201 | PASS | PASS | PASS |  |
| fixpipe_l0c2gm | 1 | dav-2201 | PASS | PASS | PASS |  |
| load_data_l12l0 | 1 | dav-2201 | PASS | PASS | PASS |  |
| batch_matmul | 1 | dav-2201 | PASS | PASS | PASS |  |
| mmad_gemv | 1 | dav-2201 | PASS | PASS | PASS |  |
| fixpipe_l0c2l1 | 1 | dav-2201 | PASS | PASS | PASS |  |
| mmad_load3dv2 | 1 | dav-2201 | PASS | PASS | PASS |  |
| mmad_unitflag | 1 | dav-2201 | PASS | PASS | PASS |  |
| mmad_with_sparse | 1 | dav-2201 | PASS | PASS | PASS |  |
| sync/ib_set_wait | 0 | dav-2201 | PASS | PASS | PASS |  |
| sync/sync_all | 0 | dav-2201 | PASS | PASS | PASS |  |