# asc_mutex_execute_mode

<!-- md-trans-meta sourceCommit=5e55c38a4e567465079c4ca7387d07ab9e6347c5 translatedAt=2026-10-23T00:03:00.049Z pushedAt=2026-10-28T12:47:03.097Z nl=2 -->

`asc_mutex_execute_mode` selects the blocking or non-blocking execution mode of a mutex, and serves as the `mode` parameter type of the [asc_lock](../../sync/intra_core_sync/asc_lock.md) and [asc_unlock](../../sync/intra_core_sync/asc_unlock.md) APIs. The compatible type name `ascMutexExecuteMode` is an alias of this type.

The header file path is: `"c_api/defs/enum.h"`.

## Enum Definition

```cpp
enum asc_mutex_execute_mode {
    ASC_LOCK_BLOCK = 0,
    ASC_LOCK_NON_BLOCK = 1
};

using ascMutexExecuteMode = asc_mutex_execute_mode;
```

## Enumeration Value Details

**Table 1** Enumeration value description

| Enumeration Value | Value | Description |
| --- | --- | --- |
| ASC_LOCK_BLOCK | 0 | Blocks the execution of the pipeline specified by `pipe`. This is the default mode. |
| ASC_LOCK_NON_BLOCK | 1 | Does not block the execution of the pipeline. |
