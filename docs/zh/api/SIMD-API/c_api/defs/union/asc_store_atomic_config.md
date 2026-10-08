---
okftype: reference.union
description: "包含原子操作启用位与原子操作类型的联合体，供asc_get_store_atomic_config接口使用，启用位按float、half、int16_t、int32_t、int8_t、bfloat16_t等数据类型开启原子操作，操作类型当前支持求和操作，默认值均为0。"
tag:
  - roles: [configuration]
  - traits: [atomic]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/atomic/scalar_atomic/asc_get_store_atomic_config.md
    kind: related
version: 9.0.0-
---
# asc_store_atomic_config

asc_store_atomic_config参数包含原子操作启用位与原子操作类型，用于[asc_get_store_atomic_config](../../atomic/scalar_atomic/asc_get_store_atomic_config.md)接口使用。

## 结构体具体定义

```cpp
constexpr uint64_t ASC_STORE_ATOMIC_DEFAULT_VALUE = 0x0000000000000000;
union asc_store_atomic_config {
    uint64_t config = ASC_STORE_ATOMIC_DEFAULT_VALUE;
    struct {
        uint64_t atomic_type : 3;
        uint64_t atomic_op : 2;
        uint64_t reserved1 : 59;
    };
};
```

## 字段详解

**表1** 字段说明

|字段名|字段含义|
|----------|----------|
| atomic_type | 原子操作启用位，默认值：0。<br>0：无原子操作。<br>1：开启原子操作，进行原子操作的数据类型为float。<br>2：开启原子操作，进行原子操作的数据类型为half。<br>3：开启原子操作，进行原子操作的数据类型为int16_t。<br>4：开启原子操作，进行原子操作的数据类型为int32_t。<br>5：开启原子操作，进行原子操作的数据类型为int8_t。<br>6：开启原子操作，进行原子操作的数据类型为bfloat16_t。<br>其余值无具体含义。|
| atomic_op | 原子操作类型，默认值：0。<br>0：求和操作。<br>其余值无具体含义。 |
