# Ascend950 CUBE task-book self-test

- HEAD: baf8eb4dc
- ARCH: dav-3510
- MODE: mx_only
- STAMP: 20260918_172757

- PASS: **10**
- FAIL: 0

| case | scenario | arch | build | run | verify | note |
|---|---|---|---|---|---|---|
| mmad_mx | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| mmad_mx | 2 | dav-3510 | PASS | PASS | PASS | s2 |
| mmad_mx | 3 | dav-3510 | PASS | PASS | PASS | s3 |
| mmad_mx | 4 | dav-3510 | PASS | PASS | PASS | s4 |
| load_data_2dmx_l12l0 | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| load_data_2dmx_l12l0 | 2 | dav-3510 | PASS | PASS | PASS | s2 |
| load_data_2dmx_l12l0 | 3 | dav-3510 | PASS | PASS | PASS | s3 |
| load_data_2dmx_l12l0 | 4 | dav-3510 | PASS | PASS | PASS | s4 |
| load_data_2dmx_l12l0 | 5 | dav-3510 | PASS | PASS | PASS | s5 |
| load_data_2dmx_l12l0 | 6 | dav-3510 | PASS | PASS | PASS | s6 |
| dual/mmad_mx | 1 | dav-3510 | SKIP | - | - | stock_tensor_LoadDataMx_incomplete_on_beta3 |
| dual/load_data_2dmx_l12l0 | 1 | dav-3510 | SKIP | - | - | stock_tensor_LoadDataMx_incomplete_on_beta3 |
| PopStackBuffer | 0 | dav-3510 | SKIP | - | - | friend_stack_meta |
| SetAddrWithOffset | 0 | dav-3510 | SKIP | - | - | deprecated_member |