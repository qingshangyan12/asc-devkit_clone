---
okftype: reference.api
title: asc_get_block_num（废弃）
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/deprecated_interface/asc_get_block_num_deprecated.md
description: 已废弃的核数查询接口：获取当前任务配置的核数用于代码内部的多核逻辑控制并返回int64_t结果，运行于标量流水，请使用内置变量block_num替代。
tag:
  - api_surfaces: [c_api]
  - roles: [query]
  - families: [system]
  - execution_models: [scalar]
  - traits: [deprecated]
signature:
  - override_id: asc_get_block_num__z
archs: ['2201', '3510']
header: [/include/c_api/utils/sys_var.h]
dependencies: []
examples: []
timestamp: '2026-09-28'
---
# asc_get_block_num（废弃）

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
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

头文件路径为：`"c_api/utils/sys_var.h"`。

**该接口已废弃，请使用[内置变量block_num](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#内置变量)替代。**

获取当前任务配置的核数，用于代码内部的多核逻辑控制等。建议在代码中直接使用内置变量`block_num`：

```cpp
uint32_t blockLength = C_API_TOTAL_LENGTH / block_num;  // 替代 asc_get_block_num()
```

### 迁移说明

内置变量`block_num`的值为当前任务配置的核数，与原接口返回值一致，替代时直接使用即可。使用前请参考[内置变量说明](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#内置变量)确认适用场景。

## 函数原型

```cpp
__aicore__ inline int64_t asc_get_block_num()
```

## 参数说明

无

## 返回值说明

当前任务配置的核数。

## 流水类型

`PIPE_S`

## 约束说明

无

## 调用示例

```cpp
uint32_t blockLength = C_API_TOTAL_LENGTH / block_num;  // 获取当前任务配置的核数，替代 asc_get_block_num()
```