# Ascend950 CUBE task-book self-test

- HEAD: 7f0d5c4f2
- ARCH: dav-3510
- MODE: taskbook
- STAMP: 20260918_204124

- PASS: **69**
- FAIL: 9

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
| mmad | 2 | dav-3510 | PASS | PASS | PASS | s2_m1n1k16 |
| mmad | 2 | dav-3510 | PASS | PASS | PASS | s2_m32n32k32 |
| mmad | 2 | dav-3510 | PASS | PASS | PASS | s2_m1024n16k16 |
| mmad | 2 | dav-3510 | PASS | PASS | FAIL | s2_m16n16k2048 |
| mmad | 3 | dav-3510 | PASS | PASS | FAIL | s3_m1n1k16 |
| mmad | 3 | dav-3510 | PASS | PASS | PASS | s3_m32n32k32 |
| mmad | 3 | dav-3510 | PASS | PASS | PASS | s3_m1024n16k16 |
| mmad | 3 | dav-3510 | PASS | PASS | FAIL | s3_m16n16k2048 |
| mmad | 5 | dav-3510 | PASS | PASS | PASS | s5_m1n1k16 |
| mmad | 5 | dav-3510 | PASS | PASS | PASS | s5_m32n32k32 |
| mmad | 5 | dav-3510 | PASS | PASS | PASS | s5_m1024n16k16 |
| mmad | 5 | dav-3510 | PASS | PASS | FAIL | s5_m16n16k2048 |
| mmad | 6 | dav-3510 | PASS | PASS | PASS | s6_m1n1k16 |
| mmad | 6 | dav-3510 | PASS | PASS | PASS | s6_m32n32k32 |
| mmad | 6 | dav-3510 | PASS | PASS | PASS | s6_m1024n16k16 |
| mmad | 6 | dav-3510 | PASS | PASS | PASS | s6_m16n16k2048 |
| mmad | 7 | dav-3510 | PASS | PASS | PASS | s7_m1n1k16 |
| mmad | 7 | dav-3510 | PASS | PASS | PASS | s7_m32n32k32 |
| mmad | 7 | dav-3510 | PASS | PASS | PASS | s7_m1024n16k16 |
| mmad | 7 | dav-3510 | PASS | PASS | PASS | s7_m16n16k2048 |
| fixpipe_l0c2gm | 1 | dav-3510 | PASS | PASS | PASS | s1_m32n32k32 |
| fixpipe_l0c2gm | 1 | dav-3510 | PASS | PASS | PASS | s1_m1024n16k16 |
| load_data_2dv2_l12l0 | 1 | dav-3510 | PASS | PASS | PASS | s1_m32n32k32 |
| load_data_2dv2_l12l0 | 1 | dav-3510 | PASS | PASS | PASS | s1_m16n16k1024 |
| mmad_mx | 1 | dav-3510 | PASS | PASS | FAIL | s1_m32n32k32 |
| mmad_mx | 1 | dav-3510 | PASS | PASS | FAIL | s1_m16n16k1024 |
| load_data_2dmx_l12l0 | 1 | dav-3510 | PASS | PASS | FAIL | s1_m32n32k32 |
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
| dual/mmad | 5 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/mmad | 6 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/fixpipe_l0c2gm | 1 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/load_data_2dv2_l12l0 | 1 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/mmad_mx | 1 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/mmad_mx | 3 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| dual/load_data_2dmx_l12l0 | 1 | dav-3510 | PASS | PASS | PASS | ptr_eq_tensor=PASS;tensor_vs_golden=PASS |
| sync/ib_set_wait | 0 | dav-3510 | BUILD_FAIL | - | - | /home/Temp_room/wj_cub_cube_ptr_950/runs_950/taskbook_20260918_204124/sync_ib_set_wait/ib_set_wait.asc:50:9: error: no m |
| sync/sync_all | 0 | dav-3510 | BUILD_FAIL | - | - | /home/Temp_room/wj_cub_cube_ptr_950/runs_950/taskbook_20260918_204124/sync_sync_all/sync_all.asc:57:9: error: no matchin |
| mmad | 4 | dav-3510 | SKIP | - | - | int4_dav-2201_only |
| PopStackBuffer | 0 | dav-3510 | SKIP | - | - | friend_stack_meta |
| SetAddrWithOffset | 0 | dav-3510 | SKIP | - | - | deprecated_member |