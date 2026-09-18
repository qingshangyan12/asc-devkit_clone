# Ascend910B3 CUBE task-book self-test

- HEAD: ba35e47a8
- ARCH: dav-2201
- STAMP: 20260918_125711
- MODE: taskbook

- verify PASS: **23**
- SKIP (documented): 8
- FAIL: 25

## Notes

- Shapes 1/32/1024/2048 applied as M/K/N equivalent dims; cube single-shot L0/L1 cannot hold square 1024^2.
- Dual-path rows () compare pointer vs Tensor output bins.
- MmadMx / 2dmx deferred to Ascend950 (dav-3510).

| case | scenario | arch | build | run | verify | note |
|---|---|---|---|---|---|---|
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1/mmad.asc:371:24: error: no member named 'disa |
| mmad | 2 | dav-2201 | BUILD_FAIL | - | - | s2|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s2/mmad.asc:371:24: error: no member named 'disa |
| mmad | 3 | dav-2201 | BUILD_FAIL | - | - | s3|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s3/mmad.asc:371:24: error: no member named 'disa |
| mmad | 4 | dav-2201 | BUILD_FAIL | - | - | s4|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s4/mmad.asc:371:24: error: no member named 'disa |
| fixpipe_l0c2gm | 1 | dav-2201 | PASS | PASS | PASS | s1 |
| fixpipe_l0c2gm | 2 | dav-2201 | PASS | PASS | PASS | s2 |
| fixpipe_l0c2gm | 4 | dav-2201 | PASS | PASS | PASS | s4 |
| fixpipe_l0c2gm | 5 | dav-2201 | PASS | PASS | PASS | s5 |
| fixpipe_l0c2gm | 6 | dav-2201 | PASS | PASS | PASS | s6 |
| fixpipe_l0c2gm | 7 | dav-2201 | PASS | PASS | PASS | s7 |
| load_data_l12l0 | 1 | dav-2201 | PASS | PASS | PASS | s1 |
| load_data_l12l0 | 6 | dav-2201 | PASS | PASS | PASS | s6 |
| load_data_l12l0 | 11 | dav-2201 | PASS | PASS | PASS | s11 |
| fixpipe_l0c2l1 | 1 | dav-2201 | PASS | PASS | PASS | s1 |
| mmad_load3dv2 | 1 | dav-2201 | PASS | PASS | PASS | s1 |
| mmad_unitflag | 1 | dav-2201 | PASS | PASS | PASS | s1 |
| batch_matmul | 1 | dav-2201 | PASS | PASS | PASS | s1 |
| mmad_gemv | 1 | dav-2201 | PASS | PASS | PASS | s1 |
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1_m1n1k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1_m1n1k16/mmad.asc:371:24: error: no member nam |
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1_m1n16k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1_m1n16k16/mmad.asc:371:24: error: no member na |
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1_m32n32k32|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1_m32n32k32/mmad.asc:371:24: error: no member n |
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1_m128n128k128|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1_m128n128k128/mmad.asc:371:24: error: no membe |
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1_m1024n16k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1_m1024n16k16/mmad.asc:371:24: error: no member |
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1_m16n16k1024|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1_m16n16k1024/mmad.asc:371:24: error: no member |
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1_m32n32k1024|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1_m32n32k1024/mmad.asc:371:24: error: no member |
| mmad | 1 | dav-2201 | BUILD_FAIL | - | - | s1_m16n16k2048|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s1_m16n16k2048/mmad.asc:371:24: error: no member |
| mmad | 2 | dav-2201 | BUILD_FAIL | - | - | s2_m1n16k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s2_m1n16k16/mmad.asc:371:24: error: no member na |
| mmad | 2 | dav-2201 | BUILD_FAIL | - | - | s2_m32n32k32|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s2_m32n32k32/mmad.asc:371:24: error: no member n |
| mmad | 2 | dav-2201 | BUILD_FAIL | - | - | s2_m1024n16k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s2_m1024n16k16/mmad.asc:371:24: error: no member |
| mmad | 2 | dav-2201 | BUILD_FAIL | - | - | s2_m16n16k1024|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s2_m16n16k1024/mmad.asc:371:24: error: no member |
| mmad | 3 | dav-2201 | BUILD_FAIL | - | - | s3_m1n16k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s3_m1n16k16/mmad.asc:371:24: error: no member na |
| mmad | 3 | dav-2201 | BUILD_FAIL | - | - | s3_m32n32k32|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s3_m32n32k32/mmad.asc:371:24: error: no member n |
| mmad | 3 | dav-2201 | BUILD_FAIL | - | - | s3_m1024n16k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s3_m1024n16k16/mmad.asc:371:24: error: no member |
| mmad | 3 | dav-2201 | BUILD_FAIL | - | - | s3_m16n16k512|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s3_m16n16k512/mmad.asc:371:24: error: no member |
| mmad | 4 | dav-2201 | BUILD_FAIL | - | - | s4_m1n1k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s4_m1n1k16/mmad.asc:371:24: error: no member nam |
| mmad | 4 | dav-2201 | BUILD_FAIL | - | - | s4_m32n32k32|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s4_m32n32k32/mmad.asc:371:24: error: no member n |
| mmad | 4 | dav-2201 | BUILD_FAIL | - | - | s4_m1024n16k16|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s4_m1024n16k16/mmad.asc:371:24: error: no member |
| mmad | 4 | dav-2201 | BUILD_FAIL | - | - | s4_m16n16k2048|/home/Temp_room/wj_cub_cube_ptr/runs_910b/taskbook_20260918_125711/mmad_s4_m16n16k2048/mmad.asc:371:24: error: no member |
| load_data_l12l0 | 1 | dav-2201 | PASS | PASS | PASS | s1_m32n32k32 |
| load_data_l12l0 | 1 | dav-2201 | PASS | PASS | PASS | s1_m16n16k1024 |
| fixpipe_l0c2gm | 1 | dav-2201 | PASS | PASS | PASS | s1_m32n32k32 |
| fixpipe_l0c2gm | 1 | dav-2201 | PASS | PASS | PASS | s1_m64n64k64 |
| fixpipe_l0c2gm | 1 | dav-2201 | PASS | PASS | PASS | s1_m1024n16k16 |
| fixpipe_l0c2gm | 1 | dav-2201 | PASS | PASS | FAIL | s1_m16n16k1024 |
| dual/mmad | 1 | dav-2201 | SKIP | - | - | no_ptr_output |
| dual/mmad | 1 | dav-2201 | SKIP | - | - | no_ptr_output |
| dual/mmad | 2 | dav-2201 | SKIP | - | - | no_ptr_output |
| dual/mmad | 4 | dav-2201 | SKIP | - | - | no_ptr_output |
| dual/fixpipe_l0c2gm | 1 | dav-2201 | PASS | PASS | PASS | ptr_eq_tensor_s1 |
| dual/load_data_l12l0 | 1 | dav-2201 | PASS | PASS | PASS | ptr_eq_tensor_s1 |
| sync/ib_set_wait | 0 | dav-2201 | PASS | PASS | PASS |  |
| sync/sync_all | 0 | dav-2201 | PASS | PASS | PASS |  |
| mmad_mx | 0 | dav-2201 | SKIP | - | - | dav-3510_only_on_950 |
| load_data_2dmx | 0 | dav-2201 | SKIP | - | - | dav-3510_only_on_950 |
| PopStackBuffer | 0 | dav-2201 | SKIP | - | - | friend_stack_meta_not_data_ptr |
| SetAddrWithOffset | 0 | dav-2201 | SKIP | - | - | deprecated_LocalTensor_member |