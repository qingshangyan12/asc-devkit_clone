# Ascend950 CUBE task-book self-test

- HEAD: feacf4c83
- ARCH: dav-3510
- MODE: sync_only
- STAMP: 20260918_215653

- PASS: **0**
- FAIL: 2

| case | scenario | arch | build | run | verify | note |
|---|---|---|---|---|---|---|
| sync/ib_set_wait | 0 | dav-3510 | PASS | TIMEOUT | SKIP | to=90s |
| sync/sync_all | 0 | dav-3510 | PASS | TIMEOUT | SKIP | to=90s |
| mmad | 4 | dav-3510 | SKIP | - | - | int4_dav-2201_only |
| PopStackBuffer | 0 | dav-3510 | SKIP | - | - | friend_stack_meta |
| SetAddrWithOffset | 0 | dav-3510 | SKIP | - | - | deprecated_member |