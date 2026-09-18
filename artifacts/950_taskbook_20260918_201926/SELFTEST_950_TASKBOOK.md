# Ascend950 CUBE task-book self-test

- HEAD: b2f76f430
- ARCH: dav-3510
- MODE: taskbook
- STAMP: 20260918_201926

- PASS: **46**
- FAIL: 0

| case | scenario | arch | build | run | verify | note |
|---|---|---|---|---|---|---|
| mmad | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| mmad | 2 | dav-3510 | PASS | PASS | PASS | s2 |
| mmad | 3 | dav-3510 | PASS | PASS | PASS | s3 |
| mmad | 5 | dav-3510 | PASS | PASS | PASS | s5 |
| mmad | 6 | dav-3510 | PASS | PASS | PASS | s6 |
| mmad | 7 | dav-3510 | PASS | PASS | PASS | s7 |
| fixpipe_l0c2gm | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| fixpipe_l0c2gm | 2 | dav-3510 | PASS | PASS | PASS | s2 |
| fixpipe_l0c2gm | 3 | dav-3510 | PASS | PASS | PASS | s3 |
| fixpipe_l0c2gm | 4 | dav-3510 | PASS | PASS | PASS | s4 |
| fixpipe_l0c2gm | 5 | dav-3510 | PASS | PASS | PASS | s5 |
| fixpipe_l0c2gm | 6 | dav-3510 | PASS | PASS | PASS | s6 |
| fixpipe_l0c2gm | 7 | dav-3510 | PASS | PASS | PASS | s7 |
| load_data_2dv2_l12l0 | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| load_data_2dv2_l12l0 | 2 | dav-3510 | PASS | PASS | PASS | s2 |
| load_data_2dv2_l12l0 | 3 | dav-3510 | PASS | PASS | PASS | s3 |
| load_data_2dv2_l12l0 | 4 | dav-3510 | PASS | PASS | PASS | s4 |
| load_data_2dv2_l12l0 | 5 | dav-3510 | PASS | PASS | PASS | s5 |
| load_data_2dv2_l12l0 | 6 | dav-3510 | PASS | PASS | PASS | s6 |
| load_data_2dv2_l12l0 | 7 | dav-3510 | PASS | PASS | PASS | s7 |
| batch_matmul | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| mmad_gemv | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| mmad_unitflag | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| mmad | 1 | dav-3510 | PASS | PASS | PASS | s1_m1n1k16 |
| mmad | 1 | dav-3510 | PASS | PASS | PASS | s1_m32n32k32 |
| mmad | 1 | dav-3510 | PASS | PASS | PASS | s1_m1024n16k16 |
| mmad | 1 | dav-3510 | PASS | PASS | PASS | s1_m16n16k2048 |
| mmad_mx | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| mmad_mx | 2 | dav-3510 | PASS | PASS | PASS | s2 |
| mmad_mx | 3 | dav-3510 | PASS | PASS | PASS | s3 |
| mmad_mx | 4 | dav-3510 | PASS | PASS | PASS | s4 |
| mmad_mx | 5 | dav-3510 | PASS | PASS | PASS | s5 |
| mmad_mx | 6 | dav-3510 | PASS | PASS | PASS | s6 |
| mmad_mx | 7 | dav-3510 | PASS | PASS | PASS | s7 |
| mmad_mx | 8 | dav-3510 | PASS | PASS | PASS | s8 |
| load_data_2dmx_l12l0 | 1 | dav-3510 | PASS | PASS | PASS | s1 |
| load_data_2dmx_l12l0 | 2 | dav-3510 | PASS | PASS | PASS | s2 |
| load_data_2dmx_l12l0 | 3 | dav-3510 | PASS | PASS | PASS | s3 |
| load_data_2dmx_l12l0 | 4 | dav-3510 | PASS | PASS | PASS | s4 |
| load_data_2dmx_l12l0 | 5 | dav-3510 | PASS | PASS | PASS | s5 |
| load_data_2dmx_l12l0 | 6 | dav-3510 | PASS | PASS | PASS | s6 |
| dual/mmad | 1 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/fixpipe_l0c2gm | 1 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/mmad_mx | 1 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/mmad_mx | 3 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/load_data_2dmx_l12l0 | 1 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| PopStackBuffer | 0 | dav-3510 | SKIP | - | - | friend_stack_meta |
| SetAddrWithOffset | 0 | dav-3510 | SKIP | - | - | deprecated_member |