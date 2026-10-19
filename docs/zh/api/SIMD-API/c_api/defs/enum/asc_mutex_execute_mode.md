---
okftype: reference.enum
description: "定义互斥锁阻塞或非阻塞执行模式枚举，作为asc_lock和asc_unlock接口的mode参数类型，ASC_LOCK_BLOCK为默认阻塞模式，ASC_LOCK_NON_BLOCK不阻塞流水执行。"
tag:
  - roles: [configuration]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_lock.md
    kind: configures
    condition: "作为锁接口的mode参数类型"
  - path: docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_unlock.md
    kind: configures
    condition: "作为解锁接口的mode参数类型"
version: ">=9.0.0"
---
# asc_mutex_execute_mode

`asc_mutex_execute_mode`用于选择互斥锁的阻塞或非阻塞执行模式，作为[asc_lock](../../sync/intra_core_sync/asc_lock.md)和[asc_unlock](../../sync/intra_core_sync/asc_unlock.md)接口的`mode`参数类型。兼容类型名`ascMutexExecuteMode`是该类型的别名。

头文件路径为：`"c_api/defs/enum.h"`。

## 枚举具体定义

```cpp
enum asc_mutex_execute_mode {
    ASC_LOCK_BLOCK = 0,
    ASC_LOCK_NON_BLOCK = 1
};

using ascMutexExecuteMode = asc_mutex_execute_mode;
```

## 枚举值详解

**表1** 枚举值说明

| 枚举值 | 取值 | 说明 |
| --- | --- | --- |
| ASC_LOCK_BLOCK | 0 | 阻塞`pipe`指定流水的执行，为默认模式。 |
| ASC_LOCK_NON_BLOCK | 1 | 不阻塞流水的执行。 |
