---
okftype: reference.type.cpp
title: asc_hf32_round_mode
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/enum/asc_hf32_round_mode.md
description: 表示Mmad计算开启HF32模式时FP32舍入到HF32的舍入模式管理策略的枚举，在调用asc_set_hf32_round_mode时使用。
tag:
  - api_surfaces: [c_api]
  - roles: [configuration]
  - traits: [rounding_mode]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/cube_compute/asc_set_hf32_round_mode.md
    kind: related
examples: []
headers: [/include/c_api/defs/enum.h]
declaration:
  symbol: asc_hf32_round_mode
  cpp: "enum class asc_hf32_round_mode : uint8_t {\n    NEAREST_AWAY = 0,\n    NEAREST_EVEN\
    \ = 1,\n    NEAREST_ZERO = NEAREST_AWAY  // Compatible with the old name; the\
    \ actual behavior is equivalent to NEAREST_AWAY.\n};"
  members:
    - name: NEAREST_AWAY
      value: '0'
      description: FP32将以向最接近的值舍入，平局时远离零的方式舍入为HF32。
    - name: NEAREST_EVEN
      value: '1'
      description: FP32将以向最接近的值舍入，平局时向偶数舍入的方式舍入为HF32。
    - name: NEAREST_ZERO
      value: NEAREST_AWAY
      description: 用于兼容旧版实现，不推荐使用，实际行为等同于`NEAREST_AWAY`。
  underlying_type: uint8_t
timestamp: '2026-09-23'
---
# asc_hf32_round_mode

asc_hf32_round_mode用于Mmad计算开启HF32模式时由FP32舍入到HF32的舍入模式管理策略，在调用[asc_set_hf32_round_mode](../../cube_compute/asc_set_hf32_round_mode.md)时使用。

## 枚举类具体定义

```cpp
enum class asc_hf32_round_mode : uint8_t {
    NEAREST_AWAY = 0,
    NEAREST_EVEN = 1,
    NEAREST_ZERO = NEAREST_AWAY  // Compatible with the old name; the actual behavior is equivalent to NEAREST_AWAY.
};
```

## 枚举值详解

|枚举值|HF32 舍入模式管理策略|
|------|-----------|
| NEAREST_AWAY | FP32将以向最接近的值舍入，平局时远离零的方式舍入为HF32。 |
| NEAREST_EVEN | FP32将以向最接近的值舍入，平局时向偶数舍入的方式舍入为HF32。 |
| NEAREST_ZERO | 用于兼容旧版实现，不推荐使用，实际行为等同于`NEAREST_AWAY`。 |
