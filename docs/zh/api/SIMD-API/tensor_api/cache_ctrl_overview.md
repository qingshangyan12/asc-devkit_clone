# cache_ctrl概述

Tensor API当前未提供完整的缓存控制能力。在使用Tensor API开发算子时，如需预加载Data Cache或Instruction Cache、维护Data Cache与Global Memory间的一致性，或使Data Cache失效，可配合使用C API提供的缓存控制接口。使用相关接口时，需要额外引入`c_api/cache_ctrl.h`，具体约束和产品支持情况以对应C API资料为准。

缓存控制相关C API如下表所示。

| 接口 | 用途 |
| :--- | :--- |
| [asc_datacache_preload](../c_api/cache_ctrl/asc_datacache_preload.md) | 将指定Global Memory地址的数据预加载到Data Cache中，每次预加载一个Cache Line。 |
| [asc_dcci](../c_api/cache_ctrl/asc_dcci.md) | 刷新Data Cache，维护Data Cache与Global Memory间的数据一致性。 |
| [asc_dci](../c_api/cache_ctrl/asc_dci.md) | 使整个Data Cache失效，不会将脏Cache Line写回Global Memory。 |
| [asc_icache_preload](../c_api/cache_ctrl/asc_icache_preload.md) | 将指定Global Memory地址的指令预加载到Instruction Cache中。 |
| [asc_get_icache_preload_status](../c_api/cache_ctrl/asc_get_icache_preload_status.md) | 查询Instruction Cache预加载状态。 |
