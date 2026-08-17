# atomic概述

Tensor API当前未提供完整的原子操作配置能力。当多个核向同一Global Memory区域写入结果，或者需要对单个Global Memory地址执行原子更新时，可配合使用C API提供的原子操作接口。使用随路原子操作接口时，需要额外引入`c_api/atomic.h`；使用Scalar原子操作接口时，需要额外引入`c_api/scalar_compute.h`。具体数据类型、约束和产品支持情况以对应C API资料为准。

## 随路原子操作

随路原子操作会影响后续目的地址为Global Memory的数据搬运，可与Tensor API的L0C Buffer到Global Memory搬运配合使用。完成原子搬运后，应根据后续流程要求恢复原子操作状态。

| 接口 | 用途 |
| :--- | :--- |
| [asc_set_atomic_add](../c_api/simd_atomic/asc_set_atomic_add.md) | 对后续搬运到Global Memory的数据使能原子累加。 |
| [asc_set_atomic_max](../c_api/simd_atomic/asc_set_atomic_max.md) | 对后续搬运到Global Memory的数据使能原子求最大值。 |
| [asc_set_atomic_min](../c_api/simd_atomic/asc_set_atomic_min.md) | 对后续搬运到Global Memory的数据使能原子求最小值。 |
| [asc_set_atomic_none](../c_api/simd_atomic/asc_set_atomic_none.md) | 关闭随路原子操作。 |
| [asc_set_store_atomic_config_v1](../c_api/simd_atomic/asc_set_store_atomic_config_v1.md) | 设置原子操作使能状态和原子操作类型。 |
| [asc_get_store_atomic_config](../c_api/simd_atomic/asc_get_store_atomic_config.md) | 获取当前原子操作使能状态和原子操作类型。 |

## Scalar原子操作

Scalar原子操作只更新指定Global Memory地址，不会改变后续数据搬运指令的原子操作状态。

| 接口 | 用途 |
| :--- | :--- |
| [asc_atomic_add](../c_api/scalar_compute/asc_atomic_add.md) | 对指定Global Memory地址执行原子加。 |
| [asc_atomic_sub](../c_api/scalar_compute/asc_atomic_sub.md) | 对指定Global Memory地址执行原子减。 |
| [asc_atomic_max](../c_api/scalar_compute/asc_atomic_max.md) | 对指定Global Memory地址执行原子求最大值。 |
| [asc_atomic_min](../c_api/scalar_compute/asc_atomic_min.md) | 对指定Global Memory地址执行原子求最小值。 |
| [asc_atomic_and](../c_api/scalar_compute/asc_atomic_and.md) | 对指定Global Memory地址执行原子与。 |
| [asc_atomic_or](../c_api/scalar_compute/asc_atomic_or.md) | 对指定Global Memory地址执行原子或。 |
| [asc_atomic_xor](../c_api/scalar_compute/asc_atomic_xor.md) | 对指定Global Memory地址执行原子异或。 |
| [asc_atomic_exch](../c_api/scalar_compute/asc_atomic_exch.md) | 对指定Global Memory地址执行原子交换。 |
| [asc_atomic_cas](../c_api/scalar_compute/asc_atomic_cas.md) | 对指定Global Memory地址执行原子比较交换。 |
| [asc_atomic_inc](../c_api/scalar_compute/asc_atomic_inc.md) | 对指定Global Memory计数器执行原子递增。 |
| [asc_atomic_dec](../c_api/scalar_compute/asc_atomic_dec.md) | 对指定Global Memory计数器执行原子递减。 |
