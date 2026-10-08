# API 文档问题记录

本文件记录 API 文档自身的问题，不记录头文件、解析器或生成器实现缺陷。

| 编号 | 问题类型 | 文档位置 | 实际内容 | 期望内容 | 影响 | 根因 | 处理建议 | 状态 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DOC-001 | 函数原型限定符与头文件不一致 | `docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transpose.md:59,71` | 文档原型使用 `__simd_callee__` | 应与真实声明一致，使用 `__aicore__` | YAML 生成器无法将 `asc_transpose` 映射到头文件，导致 `signature.headers` 为空 | 文档原型未同步真实头文件声明 | 修改占位符原型和典型示例中的限定符，然后重新生成并校验 YAML | 待修复 |
| DOC-002 | 头文件声明缺失 Doxygen 注解 | `include/c_api/cube_datamove/cube_datamove.h`（`asc_copy_l12fb` 两处声明） | 声明无 `@override_id/@archs` 及 Doxygen 块，正文亦无 npu 标记 | 补 `@override_id asc_copy_l12fb__*` 与 `@archs`（变体页证据为 2201+3510） | 基础页 `asc_copy_l12fb.md` archs unknown 进 unresolved（保守保留，索引结果正确） | 头文件注解缺失 | 头文件补注解，或 derive-overrides 登记 archs 裁决 | 待修复 |
| DOC-003 | 接口语义与命名/同族接口冲突 | `docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_log.md:54,57` | 正文与公式为「按元素取自然对数」「dst_i = ln(src_i)」 | `asc_log` 命名暗示 log10/log2，且与 `asc_ln`（同为自然对数）语义重复 | 检索「常用对数/Log2」时无法区分；两个 API 文档语义完全相同 | 疑似文档复制 `asc_ln` 未改（或接口实现即为 ln） | 与接口实现方确认真实底数后修订正文；标注本轮已按正文标 `natural_log` | 待修复 |
| DOC-004 | include 无对应声明（改名/删除） | `cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md`、`deprecated_interface/asc_copy_gm2l1_{highdim,sync_2d,sync_highdim_split}_arch_3510_deprecated.md`、`deprecated_interface/asc_copy_l12l0{a,b}_sync_{2d,3d}_arch_3510_deprecated.md`、`reg_compute/vf_call/asc_vf_call.md`（共 8 篇） | 家族名在 include/c_api 无匹配声明 | 声明应存在或文档应指向现行名称 | 8 篇进 unresolved「no declaration found」（npu 兜底后正确门控） | 接口改名/删除后头文件与文档未同步 | 核对现行接口名后修订文档或头文件 | 待修复 |

## 问题分类

| 类型 | 说明 |
| --- | --- |
| 函数原型不一致 | 文档中的函数限定符、参数、返回值或类型与头文件声明不同 |
| 头文件路径错误 | 文档声明的头文件路径不存在或指向错误文件 |
| 参数说明不一致 | 参数名、方向、类型或约束与真实声明不一致 |
| 返回值说明错误 | 文档描述与函数真实返回类型或行为不一致 |
| 示例调用不一致 | 示例中的调用签名、参数或 API 名称与文档原型不一致 |
| 产品支持信息错误 | 文档中的产品、架构或支持状态与有效证据不一致 |

## 记录要求

每条问题应记录：文档路径和行号、实际内容、期望内容、影响、证据、根因、处理建议和当前状态。
