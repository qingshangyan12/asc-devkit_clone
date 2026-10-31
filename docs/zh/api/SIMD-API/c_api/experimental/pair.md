# 双返回值结构体

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/experimental/pair.h"`。

本结构体为试验接口，在后续版本中可能会调整或移除，不保证后续兼容性。编译时需定义`CANN_ASC_USE_EXPERIMENTAL`宏，并显式包含指定的试验头文件。

双返回值结构体用于通过函数返回值承载两个寄存器结果，包含`first`和`second`两个成员。多个接口可以共用同一种结构体，成员保存的具体结果由对应接口定义。

成员使用的寄存器类型请参见[寄存器类型定义](../defs/type/data_type_definition.md)。

## 结构体定义

以下采用占位符形式说明结构体的命名和成员类型，`<dtype0>`、`<dtype1>`表示类型名称。

```c
struct asc_<dtype0>_<dtype1>_pair {
    vector_<dtype0> first;
    vector_<dtype1> second;
};
```

### dtype0和dtype1数据类型支持情况

| dtype0 | dtype1 |
| --- | --- |
| `bool` | `bool` |
| `int4x2_t` | `int4x2_t` |
| `int8_t` | `int8_t` |
| `uint8_t` | `uint8_t` |
| `fp4x2_e2m1_t` | `fp4x2_e2m1_t` |
| `fp4x2_e1m2_t` | `fp4x2_e1m2_t` |
| `hifloat8_t` | `hifloat8_t` |
| `fp8_e8m0_t` | `fp8_e8m0_t` |
| `fp8_e5m2_t` | `fp8_e5m2_t` |
| `fp8_e4m3fn_t` | `fp8_e4m3fn_t` |
| `int16_t` | `int16_t` |
| `uint16_t` | `uint16_t` |
| `half` | `half` |
| `bfloat16_t` | `bfloat16_t` |
| `int32_t` | `bool` |
| `int32_t` | `int32_t` |
| `uint32_t` | `bool` |
| `uint32_t` | `uint32_t` |
| `float` | `float` |

### 结构体典型示例

```c
struct asc_int32_t_int32_t_pair {
    vector_int32_t first;
    vector_int32_t second;
};
```

## 字段说明

| 字段名 | 数据类型 | 字段含义 |
| --- | --- | --- |
| first | `vector_<dtype0>` | 第一个返回结果，具体含义由对应接口的返回值说明定义。当`dtype0`为`bool`时，类型为`vector_bool`。 |
| second | `vector_<dtype1>` | 第二个返回结果，具体含义由对应接口的返回值说明定义。当`dtype1`为`bool`时，类型为`vector_bool`。 |

## 约束说明

- 使用该类结构体前，需要包含头文件：`#include "c_api/experimental/pair.h"`。
- 使用前需要增加编译选项`-DCANN_ASC_USE_EXPERIMENTAL`开启试验特性。
