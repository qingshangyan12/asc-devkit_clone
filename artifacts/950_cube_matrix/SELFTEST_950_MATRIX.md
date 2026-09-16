# Ascend950 CUBE self-test matrix

- HEAD: 2b3830d17
- ARCH: dav-3510
- STAMP: 20260916_125043

- verify PASS: **0**
- FAIL: 5

| case | scenario | arch | build | run | verify | note |
|---|---|---|---|---|---|---|
| mmad | 1 | dav-3510 | PASS | RUN_FAIL | - | exit=0 |
| fixpipe_l0c2gm | 1 | dav-3510 | PASS | RUN_FAIL | - | exit=0 |
| load_data_l12l0 | 1 | dav-3510 | BUILD_FAIL | - | - |  |
| batch_matmul | 1 | dav-3510 | PASS | RUN_FAIL | - | exit=0 |
| mmad_gemv | 1 | dav-3510 | PASS | RUN_FAIL | - | exit=0 |