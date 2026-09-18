# Ascend950 CUBE task-book self-test

- HEAD: b47799bcc
- ARCH: dav-3510
- MODE: sync_only
- STAMP: 20260918_221359

- PASS: **2**
- FAIL: 0

| case | scenario | arch | build | run | verify | note |
|---|---|---|---|---|---|---|
| sync/ib_set_wait | 0 | dav-3510 | PASS | PASS | PASS |  |
| sync/sync_all | 0 | dav-3510 | PASS | PASS | PASS |  |
| mmad | 4 | dav-3510 | SKIP | - | - | int4_dav-2201_only |
| PopStackBuffer | 0 | dav-3510 | SKIP | - | - | friend_stack_meta |
| SetAddrWithOffset | 0 | dav-3510 | SKIP | - | - | deprecated_member |