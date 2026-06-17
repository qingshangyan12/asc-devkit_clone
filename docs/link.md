# 文档问题批量审阅清单

- 报告: `run:2026-06-18T01-40-26`
- Run ID: `2026-06-18T01-40-26`
- 日期: `2026-06-18`
- 开始时间: `2026-06-18T04:20:16.630199+08:00`
- 导出时间: `2026-06-18T11:22:59+08:00`
- 导出负责人筛选: `全部`
- 问题总数: `168`

## 级别统计

- `error`: 18
- `info`: 66
- `warning`: 84

## 给二次审阅 agent 的任务

逐项验证下面的 finding：如果问题真实存在，直接修正文档；如果是误报，说明验证方法和误报原因。修复后重新运行 asc-doc-checker 复核。

## Findings

### 1. `0a3729a99da27f73`

- Target: `asc-devkit`
- 文件: `docs/guide/index.md`
- 行号: `316`
- 章节: `无`
- 级别: `error`
- 规则: `link.broken_relative`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 目标不存在: 算子实践参考/SIMT算子性能优化/内存访问/使用短向量类型提升效率.md
- 建议: 优先修复该 finding 指出的文档错误后重新审查。

### 2. `28b658e6642e1119`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/内存访问/避免UB的Bank冲突.md`
- 行号: `45-79`
- 章节: `【反例】/【正例】`
- 级别: `error`
- 规则: `content.simt.bank_count_mismatch`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第45行称列方向访问集中于"两个bank"约16线程，但依据文档自身4-bank-per-row模型（第7、43行），32线程列方向应落在4个bank、每bank约8线程；第79行对比段沿用"16线程"同样有误
- 建议: 第45行改为"四个bank，每个bank约8个线程"；第79行对比改为"每个bank约8个线程访问"

### 3. `31d68de52135b298`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/执行配置/线程块数量配置优化.md`
- 行号: `14`
- 章节: `线程块数量配置优化`
- 级别: `error`
- 规则: `content.markdown.alert_syntax`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: GitHub告警语法格式错误：`[!NOTE]`与后续文字"说明"写在同一行，不符合规范语法，可能导致渲染失败
- 建议: 将`> [!NOTE]说明`改为`> [!NOTE]`独占一行，"说明"作为正文内容放到下一行：`> 说明`

### 4. `4f3d8ecef688d7cb`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/核函数.md`
- 行号: `46`
- 章节: `无`
- 级别: `error`
- 规则: `link.broken_anchor`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 目标文件中未找到锚点: ./内存层级.md#共享内存unified-buffer
- 建议: 优先修复该 finding 指出的文档错误后重新审查。

### 5. `4880223c93f0339b`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/语言扩展层/SIMT-BuiltIn关键字.md`
- 行号: `52-66`
- 章节: `内置结构体 / 内置变量`
- 级别: `error`
- 规则: `content.markdown.duplicate_anchor`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "内置结构体"和"内置变量"两个章节使用了相同的锚点ID section13165113520576，会导致页面内链接跳转错误
- 建议: 为"内置变量"章节分配唯一的锚点ID，例如 section_builtin_var

### 6. `cf534bafafc9f5cb`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/语言扩展层/SIMT语言扩展层C-API.md`
- 行号: `5`
- 章节: `SIMT语言扩展层C API`
- 级别: `error`
- 规则: `content.simt.wrong_term`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "同步与内存栅栏"类别描述中使用"内存管理"一词，语义错误。内存栅栏（memory fence/barrier）用于控制内存序与可见性，与内存管理（分配/释放）无关，易误导读者。
- 建议: 将"提供内存管理与同步接口"改为"提供内存栅栏与同步接口"。

### 7. `44bc95d0c0901ba0`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/抽象硬件架构.md`
- 行号: `12`
- 章节: `无`
- 级别: `error`
- 规则: `link.broken_anchor`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 目标文件中未找到锚点: 核函数与VF函数.md#zh-cn_topic_0000002571578013_section2067153312577
- 建议: 优先修复该 finding 指出的文档错误后重新审查。

### 8. `88f01e5506b7c657`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/核函数与VF函数.md`
- 行号: `8-24`
- 章节: `核函数与VF函数`
- 级别: `error`
- 规则: `content.simt.incomplete_coverage`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 表格引入6种函数类型但正文仅详细说明3种，缺少__simd_vf__（SIMD VF函数）、__simd_callee__子函数、__callee__公共函数的定义、调用语法及约束章节
- 建议: 补充SIMD VF函数、__simd_callee__子函数、__callee__公共函数的独立章节，包含定义语法、调用方式、修饰符说明及约束条件

### 9. `84f04183133582fd`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/概述.md`
- 行号: `32`
- 章节: `指南概览`
- 级别: `error`
- 规则: `content.link.broken_relative`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 相对链接 ../../../语言扩展层/SIMD与SIMT混合编程BuiltIn关键字.md 解析到不存在的目录 docs/guide/编程指南/高级编程/语言扩展层/，目标文件实际位于 docs/guide/编程指南/语言扩展层/
- 建议: 将链接路径修改为 ../../../../语言扩展层/SIMD与SIMT混合编程BuiltIn关键字.md（向上4级）

### 10. `d3731dd0e602a1df`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/编程示例.md`
- 行号: `3`
- 章节: `编程示例`
- 级别: `error`
- 规则: `content.link.broken_target`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 链接目标目录不存在：URL路径 03_fusion_operation/gather_adds_simt_simd_hybrid 在远程仓库中为空或不存在，该样例已迁移至 05_simd_simt_hybrid 目录
- 建议: 将URL中的 03_fusion_operation 替换为 05_simd_simt_hybrid，即 https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/00_introduction/05_simd_simt_hybrid/gather_adds_simt_simd_hybrid

### 11. `bb8fa5a5a827a22e`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/编程示例.md`
- 行号: `98`
- 章节: `配置核函数启动`
- 级别: `error`
- 规则: `content.simt.wrong_config_value`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: dyn_ub_size=2048字节，但核函数内通过LocalMemAllocator分配了两个1024 float（合计8192字节），动态UB空间不足
- 建议: 将dyn_ub_size修改为不小于8192的值，例如constexpr uint32_t dyn_ub_size = 8192;

### 12. `0c8c56a6f9586e2c`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/01_assert/README.md`
- 行号: `5-27`
- 章节: `概述, 算子描述`
- 级别: `error`
- 规则: `content.markdown.inline_code_backticks`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 行内代码 assert() 使用了三反引号（```）而非单反引号（`），三反引号是围栏代码块语法，会导致渲染异常
- 建议: 将 ```assert()``` 替换为 `assert()`，共2处（第5行、第27行）

### 13. `666966dc882389aa`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/02_clock/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `error`
- 规则: `content.markdown.inline_code`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 行内代码使用了三个反引号（```clock()```），应使用单个反引号，三个反引号会触发代码块渲染
- 建议: 将 ```clock()``` 改为 `clock()`

### 14. `d5832ad228c00684`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/02_clock/README.md`
- 行号: `42-43`
- 章节: `算子描述`
- 级别: `error`
- 规则: `content.simt.shape_mismatch`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 算子规格表中 index shape [8192] 和 output shape [8192] 与实际代码 clock.asc 中 index_total_length=48*256=12288 不一致
- 建议: 将 index 和 output 的 shape 从 [8192] 修改为 [12288]

### 15. `632b9e0e68e467e7`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/02_clock/README.md`
- 行号: `108-109`
- 章节: `编译运行`
- 级别: `error`
- 规则: `content.simt.output_mismatch`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 预期输出 "test pass!" 与实际代码 clock.asc 第60行输出的 "[Success] Case accuracy is verification passed." 不一致
- 建议: 将预期输出修改为 "[Success] Case accuracy is verification passed."

### 16. `17465e4b5f31c2f0`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md`
- 行号: `71-78`
- 章节: `算子描述`
- 级别: `error`
- 规则: `content.markdown.malformed_html`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: HTML表格中存在4个多余的</tr>闭合标签（第71、75、76、78行），各<tr>已在同行闭合，这些标签属于冗余
- 建议: 删除第71、75、76、78行的多余</tr>标签

### 17. `49d5da396d65adc8`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/01_separate_compile/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `error`
- 规则: `content.simt.wrong_function_name`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 概述中称 func.asc 定义了 func_a 函数，但实际源文件 gather_compute.asc 中定义的函数名为 gather_element，不存在 func_a
- 建议: 将 func_a 改为 gather_element，同时将 func.asc 改为 gather_compute.asc

### 18. `a0f2cb91901b28c7`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `error`
- 规则: `content.lowlevel.placeholder`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 文本中存在未替换的占位符 <<<>>>，读者无法理解具体指代内容
- 建议: 将 <<<>>> 替换为实际的技术术语或 API 名称，例如具体的直调接口名称

### 19. `403e1d0789b485ca`

- Target: `asc-devkit`
- 文件: `docs/guide/index.md`
- 行号: `25`
- 章节: `无章节`
- 级别: `warning`
- 规则: `content.link.misleading_text`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 链接文本"基于指针的C语言编程"指向文件"基于语言扩展层C-API编程.md"，文本与目标文件名语义不匹配，可能误导读者
- 建议: 将链接文本改为"基于语言扩展层C API编程"，或将目标改为与文本匹配的文件

### 20. `bad34b20695ae752`

- Target: `asc-devkit`
- 文件: `docs/guide/index.md`
- 行号: `88`
- 章节: `无章节`
- 级别: `warning`
- 规则: `content.link.misleading_text`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 链接文本"高级编程"指向"硬件实现/硬件实现.md"，但同级目录存在"高级编程.md"，目标与文本不匹配
- 建议: 将目标路径改为"./编程指南/高级编程/高级编程.md"

### 21. `141f0dc074e4a94e`

- Target: `asc-devkit`
- 文件: `docs/guide/index.md`
- 行号: `310`
- 章节: `无章节`
- 级别: `warning`
- 规则: `content.link.misleading_text`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 链接文本"SIMT算子性能优化"指向"内存访问/访存合并.md"，但同级目录存在"SIMT算子性能优化.md"，目标与文本不匹配
- 建议: 将目标路径改为"算子实践参考/SIMT算子性能优化/SIMT算子性能优化.md"

### 22. `436395f512f9b45f`

- Target: `asc-devkit`
- 文件: `docs/guide/入门教程/快速入门/基于SIMT编程/Gather算子快速入门.md`
- 行号: `56`
- 章节: `Host端代码实现`
- 级别: `warning`
- 规则: `content.markdown.missing_backticks`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第56行<<<>>>未使用反引号包裹，与第28行`<<<...>>>`写法不一致，可能被Markdown渲染器误解析
- 建议: 将<<<>>>改为用反引号包裹：`<<<>>>`，与第28行保持一致

### 23. `8a7c6c294c8a9301`

- Target: `asc-devkit`
- 文件: `docs/guide/入门教程/快速入门/基于SIMT编程/HelloWorld.md`
- 行号: `25`
- 章节: `Hello World`
- 级别: `warning`
- 规则: `content.markdown.unformatted_code`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: <<<>>>语法糖符号未使用反引号包裹为行内代码，可能被Markdown渲染器误解析为HTML标签导致内容不可见
- 建议: 将<<<>>>改为`<<<>>>`，使用反引号包裹为行内代码格式

### 24. `f042399b78635037`

- Target: `asc-devkit`
- 文件: `docs/guide/入门教程/快速入门/基于SIMT编程/HelloWorld.md`
- 行号: `18`
- 章节: `Device端Kernel实现`
- 级别: `warning`
- 规则: `content.simt.printf_format`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: printf中使用的%lu格式说明符对应unsigned long(64位)，但dim3成员(blockIdx.x等)为无符号整数(uint32_t/unsigned int, 32位)，格式不匹配
- 建议: 将%lu替换为%u，与unsigned int类型匹配

### 25. `d73b770e1ba2efaf`

- Target: `asc-devkit`
- 文件: `docs/guide/入门教程/快速入门/基于SIMT编程/HelloWorld.md`
- 行号: `48`
- 章节: `算子编译与运行`
- 级别: `warning`
- 规则: `content.simt.output_description`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 描述称打印"核号"，但实际printf输出的是blockIdx(线程块索引)和threadIdx(线程索引)，并非AI Core硬件核号，表述不准确
- 建议: 将"核号"改为"线程块索引和线程索引"，与实际输出内容一致

### 26. `857360d2c14e43c2`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子实现/算子实现.md`
- 行号: `296`
- 章节: `Kernel侧算子实现`
- 级别: `warning`
- 规则: `content.simt.misleading_comment`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 完整核函数代码注释"enables coalesced memory access"具有误导性。内存合并是指同一Warp内多个线程同时访问连续地址，而非单线程内的顺序访问；Gather操作中不同线程通过index数组获取的行索引可能不连续，跨线程访问通常无法合并。
- 建议: 将注释修改为"// Single thread processes an entire row (all columns)"，移除"enables coalesced memory access"部分，或改为更准确的描述如"sequential access within a thread"。

### 27. `0d27d6332dd159b7`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/内存访问/DCache访问优化.md`
- 行号: `9`
- 章节: `SIMT编程模式下提供以下访存函数`
- 级别: `warning`
- 规则: `content.lowlevel.awkward_wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: asc_ldcg 描述"加载数据时从Global Memory加载"属于同义反复，未像 asc_stcg 那样明确指出"不经过DCache缓存"的关键特征，三个 API 描述风格不一致
- 建议: 改为"加载数据时绕过DCache，直接从Global Memory读取"，与 asc_stcg 的描述风格保持一致

### 28. `6be17718b488c4c3`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/内存访问/短向量类型提升效率.md`
- 行号: `40-41`
- 章节: `使用短向量类型提升效率`
- 级别: `warning`
- 规则: `content.markdown.missing_blank_line`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 正例章节中图片（第40行）与代码块（第41行）之间缺少空行，与反例章节（第13-15行有空行）结构不一致
- 建议: 在第40行图片与第41行代码块之间添加一个空行，与反例章节保持一致

### 29. `8f1ec4b2b35fc93a`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/执行配置/合理配置线程数避免寄存器溢出.md`
- 行号: `5`
- 章节: `【描述】`
- 级别: `warning`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "在SIMT编程的程序中"措辞冗余（"编程"与"程序"语义重叠），且"合理配置线程数避免寄存器溢出"缺少连接词，语句不流畅
- 建议: 将"在SIMT编程的程序中应合理配置线程数避免寄存器溢出"改为"在SIMT程序中应合理配置线程数以避免寄存器溢出"

### 30. `0a06c9fa2e93175a`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/执行配置/线程块数量配置优化.md`
- 行号: `126-132`
- 章节: `小数据量场景`
- 级别: `warning`
- 规则: `content.markdown.table_indent`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 表格存在2个空格的缩进，与文档中其他表格（均无缩进）不一致，可能导致部分渲染器无法正确解析
- 建议: 移除第126-132行表格每行开头的2个空格，使其与文档中其他表格对齐

### 31. `8078b19101087450`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md`
- 行号: `86`
- 章节: `共享内存大小的限制`
- 级别: `warning`
- 规则: `content.lowlevel.terminology`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 同一章节内术语不一致：第86行使用"DataCache"（无空格），而其余位置均使用"Data Cache"（有空格）。
- 建议: 将第86行的"DataCache"统一修改为"Data Cache"。

### 32. `a506b67fb5026681`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/同步机制.md`
- 行号: `23`
- 章节: `同步机制`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code_formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: asc_threadfence_block() 函数名在功能说明中以纯文本呈现，未使用反引号行内代码格式，与同目录其他文档（如概述.md）的API命名格式不一致
- 建议: 将 asc_threadfence_block() 改为使用反引号包裹的行内代码格式

### 33. `10d8b1dbb75e3b41`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/抽象硬件架构.md`
- 行号: `15`
- 章节: `抽象硬件架构`
- 级别: `warning`
- 规则: `content.simt.data_cache_description`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: Data Cache被描述为"Unified Buffer中预留的一部分空间"，其中"预留"一词与内存层级文档中专指8KB编译器预留空间的"预留空间"术语冲突，易造成读者混淆
- 建议: 将"预留"改为"划出"或"专门分配"，避免与内存层级文档中的"预留空间"术语产生歧义

### 34. `794a89c7d736cac2`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/核函数.md`
- 行号: `56`
- 章节: `Grid与Thread索引内置变量`
- 级别: `warning`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: blockIdx条目使用半角冒号":"，而threadIdx、blockDim、gridDim条目均使用全角冒号"："，标点不一致
- 建议: 将"blockIdx:"改为"blockIdx："以与其他条目保持一致

### 35. `34bb9f99dcad29e4`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/核函数.md`
- 行号: `57`
- 章节: `Grid与Thread索引内置变量`
- 级别: `warning`
- 规则: `content.lowlevel.terminology`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 使用"内核启动"，而文档其他位置（如L44、L55）均使用"核函数启动"或"核函数配置"，术语不一致
- 建议: 将"内核启动"改为"核函数启动"

### 36. `5767ea3293eb57d4`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/核函数.md`
- 行号: `67`
- 章节: `边界检查`
- 级别: `warning`
- 规则: `content.markdown.code_fence`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: C++代码语句"int global_index = blockIdx.x * blockDim.x + threadIdx.x;"未使用代码围栏包裹，无法正确渲染为代码
- 建议: 用```代码块包裹该语句

### 37. `c1c4dd27ccd56d3d`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/概述.md`
- 行号: `36-41`
- 章节: `SIMT编程内存层级体系`
- 级别: `warning`
- 规则: `content.simt.terminology_inconsistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 正文第36行使用"共享内存"，但第41行表格使用"Unified Buffer"指代同一内存类型，术语不一致
- 建议: 将表格中"Unified Buffer"改为"共享内存"，与正文及内存层级.md保持一致

### 38. `8315d0537b11c276`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/概述.md`
- 行号: `52`
- 章节: `SIMT编程接口特点`
- 级别: `warning`
- 规则: `content.simt.builtin_var_category`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 将blockDim和gridDim与blockIdx、threadIdx统称为"线程索引变量"不准确，blockDim和gridDim是维度变量
- 建议: 改为"提供`__global__`修饰符以及`blockIdx`、`threadIdx`等线程索引变量和`blockDim`、`gridDim`等维度变量"

### 39. `d6b5874ec7d3d37b`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/线程架构.md`
- 行号: `80`
- 章节: `配置最大线程数`
- 级别: `warning`
- 规则: `content.simt.terminology`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "栈溢出（stack spill）"术语混淆："栈溢出"通常指 stack overflow，而 stack spill 指寄存器溢出到栈空间，两者是不同概念；同句中还使用了"爆栈"（也是 stack overflow），导致概念混淆
- 建议: 将"栈溢出（stack spill）"改为"寄存器溢出（stack spill）"或"spill到栈空间"，与后文"爆栈"（stack overflow）区分开

### 40. `84093206db12851d`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/线程架构.md`
- 行号: `61`
- 章节: `Warp执行机制`
- 级别: `warning`
- 规则: `content.simt.warp_statement`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "同一Warp内的线程相互独立，彼此不能存在依赖关系"表述过于绝对，Warp内线程可通过共享内存进行数据交互，存在隐式依赖
- 建议: 建议修改为"同一Warp内的线程在执行上相互独立，不应通过显式同步机制建立依赖关系"或类似更准确的表述

### 41. `efd26dffa8733d70`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/线程架构.md`
- 行号: `25`
- 章节: `线程层次结构`
- 级别: `warning`
- 规则: `content.lowlevel.awkward_wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 句子结构不通顺：主语"基于SIMT编程模型的程序"与谓语"在AIV核上执行多个结构相同的线程块"衔接不当
- 建议: 建议改为"基于SIMT编程模型的程序在AIV核上执行时，会启动多个结构相同的线程块，执行的总线程数等于…"

### 42. `fc392e85fbf8ec2d`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/编程示例.md`
- 行号: `3`
- 章节: `编程示例`
- 级别: `warning`
- 规则: `content.link.empty_destination`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 链接"语法限制"的目标文件仅含一级标题，无任何实质内容（15字节），读者无法获取有效信息
- 建议: 补充语法限制.md的实质内容，或将链接暂时移除/标记为待补充

### 43. `fb033f982c4919a2`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/语言扩展层/SIMT-BuiltIn关键字.md`
- 行号: `132-155`
- 章节: `内置数据类型`
- 级别: `warning`
- 规则: `content.simt.type_width_contradiction`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 标量表称unsigned long为64 bit，但短向量表中ulong系列标注为32 bit，同一文档内unsigned long位宽描述矛盾
- 建议: 统一unsigned long的位宽：若为32 bit，标量表中uint64_t应改为unsigned long long；若为64 bit，短向量表中ulong应改为64 bit

### 44. `1cb3a9aa6eafac29`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/语言扩展层/SIMT-BuiltIn关键字.md`
- 行号: `56`
- 章节: `内置结构体`
- 级别: `warning`
- 规则: `content.lowlevel.incomplete_sentence`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: dim3描述"用于指定和获取线程网格（Grid）、线程块（Thread Block）在x、y、z维度上的内置结构体"缺少中心词，语义不完整
- 建议: 改为"用于指定和获取线程网格（Grid）、线程块（Thread Block）在x、y、z维度上大小信息的内置结构体"

### 45. `32b54d4b1fafaee6`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/语言扩展层/SIMT语言扩展层C-API.md`
- 行号: `13`
- 章节: `SIMT语言扩展层C API`
- 级别: `warning`
- 规则: `content.link.misleading_text`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-link-checker`
- 子模式: `default`
- 问题: 链接文本为"调测接口"（分类名称），但目标指向 printf-290.md（printf 单个函数页面），语义不匹配
- 建议: 将链接目标改为调测接口分类索引页：https://gitcode.com/cann/asc-devkit/blob/master/docs/api/Utils-API/调测接口/调测接口.md

### 46. `2c68f2874bb72093`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md`
- 行号: `70`
- 章节: `访问UB内存`
- 级别: `warning`
- 规则: `content.simt.missing_sync_guidance`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 指出Vector、Scalar、MTE三条流水并行执行需要同步，但未提供任何同步机制或API说明
- 建议: 补充UB访问场景下可用的同步原语或接口（如barrier、fence等），或给出指向相关API文档的链接

### 47. `9a115b93ca9f1f6b`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md`
- 行号: `79-81`
- 章节: `访问GM内存`
- 级别: `warning`
- 规则: `content.simt.missing_mainscalar_solution`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 描述了MainScalar读写GM时的缓存一致性问题，但未提供解决方案，与SIMT VF部分给出volatile/asc_dcci_single/asc_dcci_entire方案不对等
- 建议: 补充MainScalar场景下确保缓存一致性的方法，如volatile、cache flush接口或其他机制；若MainScalar无对应方案，应明确说明并给出替代建议

### 48. `4c4bef7e4a0f2ccf`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/核函数与VF函数.md`
- 行号: `10-43`
- 章节: `核函数的定义与执行配置`
- 级别: `warning`
- 规则: `content.simt.modifier_inconsistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 表格列出核函数修饰符为__global__ __aicore__，但定义语法和修饰符说明仅展示__global__ __vector__，两者不一致
- 建议: 将表格修饰符改为__global__ __vector__以与定义 section 一致，或在表格和定义 section 中同时说明两种修饰符的适用场景及区别

### 49. `ceb9bca929ea08d1`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/核函数与VF函数.md`
- 行号: `10-122`
- 章节: `核函数与VF函数`
- 级别: `warning`
- 规则: `content.lowlevel.inconsistent_code_formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 修饰符格式混用三种写法：反引号内联代码、转义下划线、原始双下划线（渲染为粗体），全文多处不一致
- 建议: 统一使用反引号内联代码格式（如`__simt_vf__`）包裹所有修饰符和API名称，避免使用转义下划线或裸双下划线

### 50. `91d00c45e3f3af29`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/概述.md`
- 行号: `24`
- 章节: `指南概览`
- 级别: `warning`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 同一句中"介绍"重复出现："介绍了SIMD与SIMT混合编程场景的硬件架构，介绍VF（Vector Function）运行机制..."，前后时态不一致且用词冗余。
- 建议: 将第二个"介绍"替换为"以及"或合并句式，例如："介绍SIMD与SIMT混合编程场景的硬件架构，以及VF（Vector Function）运行机制、SIMT VF、SIMD VF等概念。"

### 51. `a516e33cd13e78e2`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/编程示例.md`
- 行号: `82-84`
- 章节: `调用VF函数`
- 级别: `warning`
- 规则: `content.simt.incomplete_example`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: asc_vf_call<simd_adds>调用了SIMD VF函数simd_adds，但全文未展示其定义，读者无法了解SIMD侧的完整实现
- 建议: 补充simd_adds的函数定义代码段，或在调用处添加说明指向外部示例链接

### 52. `adf9c82591374085`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/00_quickstart/hello_world_simt/README.md`
- 行号: `61`
- 章节: `执行结果`
- 级别: `warning`
- 规则: `content.simt.terminology`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 术语"AIV Core"不规范，相邻SIMT文档统一使用"AI Core"（英文）或"AIV核"（中文），"AIV Core"为混用缩写与英文的错误组合
- 建议: 将"AIV Core"改为"AI Core"

### 53. `45585db12caceded`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/00_quickstart/hello_world_simt/README.md`
- 行号: `61`
- 章节: `执行结果`
- 级别: `warning`
- 规则: `content.lowlevel.redundant_word`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "成功"在同一句中重复出现："说明核函数已成功在AIV Core上成功执行"
- 建议: 删除其中一个"成功"，例如改为"说明核函数已在AI Core上成功执行"

### 54. `0c1b2acf2ce3f1d9`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/basic_gather/gather_1d/README.md`
- 行号: `21`
- 章节: `目录结构`
- 级别: `warning`
- 规则: `content.markdown.tree_inconsistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 目录树第21行使用ASCII管道符|，而第19-20行使用Unicode制表符│，字符不一致导致树形结构视觉不对齐
- 建议: 将第21行的|替换为│，与上方行保持一致

### 55. `3da0011a70043f54`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/basic_gather/gather_2d/README.md`
- 行号: `86`
- 章节: `编译运行`
- 级别: `warning`
- 规则: `content.lowlevel.grammar`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 执行结果示例文本 "Case accuracy is verification passed" 语法错误，"is verification passed" 不符合英语语法
- 建议: 修改为 "Case accuracy verification passed" 或 "Case accuracy is verified"

### 56. `880fb5201005d2da`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/general_gather/README.md`
- 行号: `41`
- 章节: `算子描述 > gather算子`
- 级别: `warning`
- 规则: `content.simt.questionable_constraint`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: index的shape约束"m < M"过于严格，典型gather操作允许m>=M（含重复索引或收集全部行）
- 建议: 确认是否为硬性限制；若非必要，移除"m < M"约束或改为不限制m与M的关系

### 57. `27d7ec0ed32a42e3`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/general_gather/README.md`
- 行号: `117-118`
- 章节: `算子描述 > gather_v2算子`
- 级别: `warning`
- 规则: `content.markdown.missing_blank_line`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "</table>"与后续列表项"- 约束说明："之间缺少空行，可能导致列表渲染异常
- 建议: 在</table>与- 约束说明：之间添加一个空行

### 58. `9a9a707ef1918a86`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/general_gather/README.md`
- 行号: `56`
- 章节: `算子描述 > gather算子 > 调用实现`
- 级别: `warning`
- 规则: `content.markdown.unescaped_angle_brackets`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 内核调用符<<<>>>未用反引号包裹，尖括号可能被解析为HTML标签导致渲染异常（第136行同样存在）
- 建议: 将<<<>>>改为用反引号包裹的行内代码形式

### 59. `ed90def87c206066`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: <<<>>>未使用反引号包裹为行内代码，与第11行同元素的格式不一致
- 建议: 将第5行的<<<>>>改为`<<<>>>`，与第11行保持一致

### 60. `2f0348a4a2196614`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/00_printf/README.md`
- 行号: `27`
- 章节: `算子描述`
- 级别: `warning`
- 规则: `content.lowlevel.grammar`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "实现对算子执行过程中打印每个线程的变量信息"句式杂糅，介词"对"使用不当，语法不通顺
- 建议: 改为"实现在算子执行过程中打印每个线程的变量信息"或"实现对算子执行过程中每个线程变量信息的打印"

### 61. `a6b195ea6a0a3ddd`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/01_assert/README.md`
- 行号: `27`
- 章节: `算子描述`
- 级别: `warning`
- 规则: `content.lowlevel.awkward_wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "实现对算子执行过程中断言的调试" 语法生硬，"对...断言的调试" 结构不自然
- 建议: 改为 "实现算子执行过程中的断言调试" 或 "对算子执行过程进行断言调试"

### 62. `803aca44019266a3`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/03_profiling/README.md`
- 行号: `1-8`
- 章节: `全文`
- 级别: `warning`
- 规则: `content.lowlevel.spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 文档中多处英文词汇与相邻中文之间缺少空格（如Profiling样例、通过Profiling工具、Gather算子、msProf工具等），影响可读性
- 建议: 在所有英文词汇与相邻中文之间添加空格，建议对全文统一处理中英文间距

### 63. `8ab45f7d733a32f7`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/03_profiling/msProf/README.md`
- 行号: `41`
- 章节: `性能数据说明`
- 级别: `warning`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文文本中使用了半角逗号："文件夹,性能数据"，应使用全角逗号
- 建议: 将半角逗号 "," 改为全角逗号 "，"

### 64. `d822cd5fcaeaecc5`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/03_profiling/msProf/README.md`
- 行号: `43-54`
- 章节: `性能数据说明`
- 级别: `warning`
- 规则: `content.markdown.code_block_language`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 目录树结构使用了```bash语言标记，内容为文件目录树而非bash命令，语言标记具有误导性
- 建议: 将```bash改为```text

### 65. `2786277bbf4ee9f6`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/07_simulator/README.md`
- 行号: `59`
- 章节: `编译运行`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第59行存在多处中文文本与行内代码反引号之间的多余空格，包括"说明："后、"${install_path}"前后、"至"后及"/usr/local/Ascend"后
- 建议: 去除中文与反引号之间的空格，改为：**说明：**`${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

### 66. `0e90c9bf67a0d109`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/09_log/README.md`
- 行号: `23`
- 章节: `目录结构介绍`
- 级别: `warning`
- 规则: `content.markdown.tree_consistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 目录树第23行使用ASCII管道符"|"而非Unicode制表符"│"，与上方行不一致
- 建议: 将第23行的"|"替换为"│"，保持目录树制表符统一

### 67. `9551445e0f50e1c4`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/09_log/README.md`
- 行号: `118`
- 章节: `编译选项说明`
- 级别: `warning`
- 规则: `content.lowlevel.terminology_consistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 同一单元格内"NPU 运行"有空格而"NPU仿真"无空格，术语空格使用不一致
- 建议: 统一写法，建议改为"NPU运行、NPU仿真"或"NPU 运行、NPU 仿真"，保持一致

### 68. `1d76b4a7e93b6d2c`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/00_framework/00_pytorch/torch_library_roll/README.md`
- 行号: `45`
- 章节: `样例描述`
- 级别: `warning`
- 规则: `content.markdown.extra_closing_tag`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: HTML表格第44行（样例类型行）已正常闭合，第45行存在多余的孤立</tr>闭合标签，无对应<tr>开标签
- 建议: 删除第45行的多余</tr>标签

### 69. `6a353ebb9a92be14`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/00_framework/README.md`
- 行号: `7-11`
- 章节: `样例列表`
- 级别: `warning`
- 规则: `content.lowlevel.heading_mismatch`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "样例列表"标题暗示多个样例，但表格中仅列出一个样例（00_pytorch），标题与内容不匹配。
- 建议: 将标题改为"样例说明"或补充更多样例条目。

### 70. `25c4189b0746d794`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md`
- 行号: `86`
- 章节: `算子描述`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "<<<>>>"未使用行内代码反引号包裹，尖括号可能被Markdown解析器当作HTML标签导致渲染异常
- 建议: 将<<<>>>改为用反引号包裹的行内代码

### 71. `ce1bbf24e46f5a03`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md`
- 行号: `66`
- 章节: `算子描述`
- 级别: `warning`
- 规则: `content.lowlevel.typo`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "key为int64_t类型数"中"类型数"表述有误，疑为"类型"或"类型的数"的笔误
- 建议: 改为"key为int64_t类型"或"key为int64_t类型的数"

### 72. `6220b7b6b83fa06f`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence/README.md`
- 行号: `5-81`
- 章节: `概述 / 样例描述`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 文档中多处中文与内联代码反引号之间存在多余空格（第5、60、81行），如"一维 `float` 输入"应改为"一维`float`输入"。
- 建议: 全文统一检查并去除中文与内联代码反引号之间的空格，保持"中文`code`中文"的紧凑格式。

### 73. `2d9ff426b28dd000`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/02_atomic_operation/README.md`
- 行号: `5`
- 章节: `## 概述`
- 级别: `warning`
- 规则: `content.simt.incomplete_api_coverage`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 概述称展示SIMT原子操作接口的使用方法，但样例仅覆盖asc_atomic_add()一个接口，而原子操作目录下共有11个API（asc_atomic_sub/max/min/cas/exch/and/or/xor/inc/dec），其余10个接口无对应样例
- 建议: 补充其他原子操作API的示例，或将概述改为明确说明当前仅展示asc_atomic_add()接口的使用方法

### 74. `9277fc7c7018cd78`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/02_atomic_operation/histogram/README.md`
- 行号: `44`
- 章节: `数据切分`
- 级别: `warning`
- 规则: `content.simt.data_type`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "遍历`uint32`输入"表述误导，样例输入数据类型为uint8而非uint32，此处实际含义是每次以uint32方式读取4字节
- 建议: 改为"按grid-stride遍历输入，每次以`uint32`方式读取4个字节"

### 75. `2b68e51a699779e8`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/02_atomic_operation/histogram/README.md`
- 行号: `5-64`
- 章节: `概述 / 阶段一`
- 级别: `warning`
- 规则: `content.markdown.html_tag`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第5行和第64行使用了非标准HTML标签</br>，<br>是空元素，不存在闭合标签形式
- 建议: 将</br>替换为<br>或<br/>

### 76. `9cc03126428c2461`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/02_atomic_operation/histogram/README.md`
- 行号: `36-37`
- 章节: `样例描述`
- 级别: `warning`
- 规则: `content.markdown.table_structure`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 样例输出所在行（rowspan=2）仅有1个单元格，缺少name/shape/data type/format表头单元格，导致该行4列渲染为空白，与样例输入行结构不一致
- 建议: 补齐表头单元格，或去掉rowspan="2"将样例输出合并为单行5列

### 77. `3b1a4e148c3ff0b9`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/02_atomic_operation/histogram/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `warning`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "Ascend C 的SIMT的"连续使用两个"的"字，语句拗口
- 建议: 改为"Ascend C SIMT的`asc_atomic_add()`接口"

### 78. `3877ae474eb0370f`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/03_warp_instruction/sobel_warp_shfl/README.md`
- 行号: `40-110`
- 章节: `样例实现`
- 级别: `warning`
- 规则: `content.simt.naming_mismatch`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 公式中变量名(middle_left/bottom_left等)与代码中变量名(mid_left/bot_left等)不一致，读者对照时易混淆
- 建议: 统一公式与代码中的变量命名，或在公式后补充缩写对照说明

### 79. `c3831ec4e70ca4f4`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/03_warp_instruction/sobel_warp_shfl/README.md`
- 行号: `50-122`
- 章节: `样例功能`
- 级别: `warning`
- 规则: `content.simt.threshold_comparison`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 文本描述"超过阈值"语义为严格大于(>)，但代码使用>=，strength恰好等于200时行为存在歧义
- 建议: 将"超过阈值"改为"不小于阈值"或"大于等于阈值"，与代码>=保持一致

### 80. `18ca206e77ba8499`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/00_basic_compile/README.md`
- 行号: `37-131`
- 章节: `异构编译介绍 / 编译运行`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 文档中多处中文与行内代码反引号之间存在多余空格（第37、41、85、102、131行），如"Host侧 `.cpp`"、"将 `.asc`"、"设置 `CMAKE_ASC_ENABLE_SIMT`"等
- 建议: 统一去除中文与行内代码反引号之间的空格，全文排查并修正

### 81. `d0a43b26d424cda6`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/01_separate_compile/README.md`
- 行号: `5-54`
- 章节: `概述 / bisheng命令行编译`
- 级别: `warning`
- 规则: `content.simt.filename_inconsistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 概述和bisheng命令行编译章节使用 func.asc、kernel.asc、host.cpp 等通用名称，与目录结构和编译运行章节中的实际文件名 gather_compute.asc、gather_kernel.asc、main.cpp 不一致
- 建议: 统一使用实际文件名，或在通用示例处明确标注其为示意名称

### 82. `cd15179a4f63f077`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/01_separate_compile/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `warning`
- 规则: `content.markdown.unescaped_kernel_launch`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `张顺为`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: <<<>>> 内核启动语法在正文中出现两次均未使用行内代码格式包裹，尖括号可能被渲染器解释为 HTML 标签导致内容丢失
- 建议: 将 <<<>>> 改为行内代码格式 `<<<>>>`

### 83. `170b3057a13ca6ea`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/02_dynamic_library_compile/README.md`
- 行号: `44`
- 章节: `bisheng命令行编译`
- 级别: `warning`
- 规则: `content.simt.inconsistent_example`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: bisheng示例命令使用kernel.asc/libkernel.so，但项目实际文件为gather_kernel.asc，同文档CMake示例也使用真实文件名，内部不一致
- 建议: 将第44行命令改为 bisheng -shared gather_kernel.asc -o libgather_kernel.so -fPIC --npu-arch=dav-3510 --enable-simt，与CMake示例和编译运行章节保持一致

### 84. `310fc0464d9df3b7`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/02_dynamic_library_compile/README.md`
- 行号: `37-102`
- 章节: `动态库编译介绍`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `刘熠`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 全文约11处中文与行内代码反引号之间存在多余空格，如Host侧 `.cpp`、`.asc` 混合文件等
- 建议: 统一去除中文与反引号之间的空格，对全文做一次统一修正

### 85. `c16163af0822bbf2`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/03_static_library_compile/README.md`
- 行号: `44-58`
- 章节: `bisheng命令行编译 / CMake方式编译`
- 级别: `warning`
- 规则: `content.simt.example_filename_mismatch`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 示例命令使用kernel.asc/libkernel.a，但项目实际文件为gather_kernel.asc，"具体编译命令如下"的措辞会让读者直接复制命令导致找不到文件
- 建议: 将示例中的kernel.asc改为gather_kernel.asc，libkernel.a改为libgather_kernel.a，与项目目录结构及编译运行小节保持一致

### 86. `a7a48a16d8d0a97f`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/aligned_types/README.md`
- 行号: `115`
- 章节: `性能指标说明`
- 级别: `warning`
- 规则: `content.simt.wrong_term`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: aiv_total_cycles 描述中使用了"CPU cycle"，但该指标在 NPU Vector Core 上采集，不是主机 CPU 周期
- 建议: 将"CPU cycle"改为"NPU cycle"或"AI Core cycle"

### 87. `a52bb9d6c45fe943`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/aligned_types/README.md`
- 行号: `106`
- 章节: `调用实现`
- 级别: `warning`
- 规则: `content.markdown.missing_backticks`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: <<<>>>未使用反引号包裹，可能被 Markdown 渲染器误解析为 HTML 标签，且与第59行写法不一致
- 建议: 将 <<<>>> 改为 `<<<>>>`

### 88. `c75d61198c3547c2`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/aligned_types/README.md`
- 行号: `208`
- 章节: `性能数据获取`
- 级别: `warning`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `snt`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文句子中使用了英文逗号"文件夹,性能数据"，应为中文逗号
- 建议: 将英文逗号 "," 改为中文逗号 "，"

### 89. `47ca10669502697d`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `73`
- 章节: `1. 数据加载特点`
- 级别: `warning`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 句末缺少中文句号："不应让这类数据与热点数据竞争DCache空间"后无标点
- 建议: 在句末添加中文句号"。"

### 90. `14c9f8e3ebf01cce`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `232`
- 章节: `编译运行`
- 级别: `warning`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中英文冒号混用："0:基线版本"使用半角冒号，"1：数据缓存优化版本"使用全角冒号
- 建议: 统一使用全角冒号："0：基线版本，1：数据缓存优化版本"

### 91. `3c5cc6e6fff830ff`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `248`
- 章节: `性能分析`
- 级别: `warning`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文文本中使用了半角逗号："文件夹,性能数据文件夹结构示例如下"
- 建议: 将半角逗号","改为全角逗号"，"

### 92. `daa9f0aa0266ab54`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `136-140`
- 章节: `Case 0: 基线版本`
- 级别: `warning`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `白雪眼`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 列举英文词时使用全角逗号造成断句歧义："加载input，output时"易被误读为两个分句
- 建议: 将"input，output"改为"input、output"或"input和output"以消除歧义

### 93. `cbba4995bfdae22a`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice/README.md`
- 行号: `341-370`
- 章节: `编译运行 / 性能分析`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文文本与行内代码反引号之间存在多余空格，文档中有多处此类问题（如第341行、第370行）
- 建议: 统一去除中文与行内代码反引号之间的空格，例如将`使用 \`msprof\` 工具`改为`使用\`msprof\`工具`，并全文排查同类问题

### 94. `fc843995e8340c73`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice/README.md`
- 行号: `376`
- 章节: `性能分析`
- 级别: `warning`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文句子中使用了半角逗号","，同一句中其他逗号均为全角"，"，标点不一致
- 建议: 将第376行"文件夹,性能数据"中的半角逗号改为全角逗号"，"

### 95. `9aa9c59a718ba87b`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add/README.md`
- 行号: `58`
- 章节: `性能指标说明`
- 级别: `warning`
- 规则: `content.simt.terminology`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: aiv_total_cycles 释义中使用"CPU周期"，但该指标描述的是 AI Vector Core 执行周期，不是 CPU 周期；line 190 同样存在此问题
- 建议: 将"CPU周期"改为"AI Core周期"或直接写"周期（Cycle）"

### 96. `f06455be9c4dea6e`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add/README.md`
- 行号: `260`
- 章节: `性能数据获取`
- 级别: `warning`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `xuyiyang`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文文本之间使用了 ASCII 逗号","而非中文逗号"，"
- 建议: 将"文件夹,性能数据"改为"文件夹，性能数据"

### 97. `1f256c69ae19bc84`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/README.md`
- 行号: `1`
- 章节: `Execution Configuration Optimizations样例介绍`
- 级别: `warning`
- 规则: `content.lowlevel.mixed_language`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 标题中英文直接拼接无空格分隔："Execution Configuration Optimizations样例介绍"，影响可读性
- 建议: 在中英文之间添加空格，如 "Execution Configuration Optimizations 样例介绍"，或统一使用中文标题

### 98. `f8dbfb2051513b79`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/README.md`
- 行号: `11`
- 章节: `样例列表`
- 级别: `warning`
- 规则: `content.lowlevel.formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: \_\_launch_bounds\_\_ 使用反斜杠转义下划线来避免 Markdown 粗体解析，方式脆弱且不规范
- 建议: 改用行内代码反引号包裹：`__launch_bounds__`

### 99. `f8ebacfa50cf3e30`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `warning`
- 规则: `content.simt.terminology`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `祁星凯`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "设置最大线程块的优化案例"中"最大线程块"表述模糊，可理解为最大线程块数量或每块最大线程数，结合 __launch_bounds__ 实际指每块最大线程数
- 建议: 改为"设置每块最大线程数的优化案例"或"设置线程块最大线程数的优化案例"以消除歧义

### 100. `8a6839e76d428a4b`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/max_thread_config/sincos_compute/README.md`
- 行号: `13`
- 章节: `支持的CANN软件版本`
- 级别: `warning`
- 规则: `content.lowlevel.formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 列表项中使用了不必要的反斜杠转义：\>= CANN 9.1.0，反斜杠在某些渲染器中可能显示为字面字符
- 建议: 移除反斜杠，改为：- >= CANN 9.1.0

### 101. `c062a8614a1979fe`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/max_thread_config/sincos_compute/README.md`
- 行号: `49-162`
- 章节: `sincos计算说明 / 样例实现说明`
- 级别: `warning`
- 规则: `content.simt.missing_prerequisite`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `lishangfan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 代码示例中使用了 PER_THREAD_LOOP 和 THREADS_PER_BLOCK 常量但未定义其值，读者需查阅源文件才能理解参数含义
- 建议: 在sincos计算说明章节补充常量定义，例如：PER_THREAD_LOOP=16（每线程处理16个输入），THREADS_PER_BLOCK=512（每Block 512线程）

### 102. `d74fec2f5bffcac0`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/README.md`
- 行号: `4`
- 章节: `样例列表`
- 级别: `warning`
- 规则: `content.lowlevel.inline_code_formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `louyuxuan`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: <<<>>>未使用反引号包裹为行内代码，Markdown渲染时尖括号可能被解析为HTML标签导致显示异常
- 建议: 将<<<>>>改为`<<<>>>`，与00_introduction/README.md中的格式保持一致

### 103. `410097b36696e85d`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/内存访问/DCache访问优化.md`
- 行号: `67`
- 章节: `正例`
- 级别: `info`
- 规则: `content.lowlevel.redundant_wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "不需要占用DCache空间，避免输入数据对DCache的占用"前后语义重复
- 建议: 删除其中一个分句，例如改为"不需要占用DCache空间"即可

### 104. `f7153c233a12d5ab`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/内存访问/DCache访问优化.md`
- 行号: `17-46`
- 章节: `反例 / 正例`
- 级别: `info`
- 规则: `content.markdown.missing_code_language`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 两处代码块（line 17、line 46）均未指定语言标签，代码为 C++/CUDA 但缺少语法高亮标识
- 建议: 将 ``` 改为 ```cpp

### 105. `33acded3678db5bb`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/内存访问/短向量类型提升效率.md`
- 行号: `53`
- 章节: `使用短向量类型提升效率`
- 级别: `info`
- 规则: `content.lowlevel.code_comment_wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 代码注释"短向量循环处理偶数对"中"偶数对"表述不清晰，易产生歧义
- 建议: 建议改为"短向量循环处理所有完整元素对"或"短向量循环处理成对元素"

### 106. `cf07a2c4c0f67e29`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/内存访问/通过类型对齐提升搬运效率.md`
- 行号: `9`
- 章节: `通过类型对齐提升搬运效率`
- 级别: `info`
- 规则: `content.markdown.code_lang`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第一个代码块（第9行）缺少语言标签，文档中全部四个代码块均未指定语言，无法获得语法高亮
- 建议: 为所有代码块添加 cpp 语言标签，即将 ``` 改为 ```cpp

### 107. `c9652a1772a215ca`

- Target: `asc-devkit`
- 文件: `docs/guide/算子实践参考/SIMT算子性能优化/内存访问/通过类型对齐提升搬运效率.md`
- 行号: `39`
- 章节: `通过类型对齐提升搬运效率`
- 级别: `info`
- 规则: `content.markdown.table_separator`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第一张表格分隔行第二列 |:--------------: | 缺少冒号前的空格，与第二张表格（第55行）的 | :--------------: | 写法不一致
- 建议: 将第39行改为 | :---------------: | :--------------: |，保持与第55行格式一致

### 108. `775b5491008c1645`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/同步机制.md`
- 行号: `23`
- 章节: `同步机制`
- 级别: `info`
- 规则: `content.lowlevel.inconsistent_span`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 功能说明文本被多个无class/style属性的<span>标签碎片化包裹，属于编辑器残留，增加HTML噪音
- 建议: 移除所有无意义的<span>标签，保留纯文本内容

### 109. `5f143bc9042a8ad0`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/概述.md`
- 行号: `49`
- 章节: `SIMT编程接口特点`
- 级别: `info`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "具体接口可以查询"中"查询"用词不当，应为"查阅"
- 建议: 将"查询"改为"查阅"

### 110. `733ca6b6d700d495`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/线程架构.md`
- 行号: `63`
- 章节: `Warp执行机制`
- 级别: `info`
- 规则: `content.lowlevel.extra_space`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 数字与量词之间存在多余空格："不足32 个线程"中"32"与"个"之间不应有空格
- 建议: 删除"32"与"个"之间的空格，改为"不足32个线程"

### 111. `e815267c0e0fa079`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/线程架构.md`
- 行号: `16`
- 章节: `线程层次结构`
- 级别: `info`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 列表最后一项以分号"；"结尾，而文档中其他列表末项以句号"。"结尾，标点不一致
- 建议: 将第16行末尾的"；"改为"。"，与其他列表保持一致

### 112. `3cc2466d184566ae`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/编程示例.md`
- 行号: `5`
- 章节: `编程示例`
- 级别: `info`
- 规则: `content.lowlevel.terminology`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 将形状为100000*128的数据称为"二维向量"不准确，"向量"通常指一维数据
- 建议: 将"二维向量"改为"二维张量"或"二维矩阵"

### 113. `af6e7f6f5de2c981`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/编程模型/AI-Core-SIMT编程/编程示例.md`
- 行号: `62`
- 章节: `编程示例`
- 级别: `info`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "每行数据由核函数完成处理"表述模糊，易误解为每行需一次核函数调用
- 建议: 改为"每行数据由核函数中的一个线程完成处理"

### 114. `0d108fbed40f7d02`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/语言扩展层/SIMT-BuiltIn关键字.md`
- 行号: `143-163`
- 章节: `内置数据类型`
- 级别: `info`
- 规则: `content.lowlevel.missing_table_caption`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 短向量类型列表（元素数据类型与Vector X2/X3/X4对应关系表）缺少表编号，与文档中其他表格的编号体系不一致
- 建议: 为该表补充表编号，并相应调整后续表编号顺序

### 115. `ec30d72eb5ac86a7`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/语言扩展层/SIMT-BuiltIn关键字.md`
- 行号: `318`
- 章节: `核函数配置`
- 级别: `info`
- 规则: `content.lowlevel.ascii_semicolon`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文语境下使用了ASCII分号";"而非中文分号"；"，与同列表第319行的中文分号不一致
- 建议: 将第318行末尾的ASCII分号";"改为中文分号"；"

### 116. `e805047285f7f4bc`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/核函数与VF函数.md`
- 行号: `95`
- 章节: `SIMT VF函数`
- 级别: `info`
- 规则: `content.lowlevel.missing_inline_code`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: NOTE块中asc_vf_call未使用反引号包裹，与文档其他位置的写法不一致
- 建议: 将第95行的asc_vf_call改为`asc_vf_call`

### 117. `36081601666f1e11`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/编程示例.md`
- 行号: `110`
- 章节: `配置核函数启动`
- 级别: `info`
- 规则: `content.lowlevel.inconsistent_table_value`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 参数表格中num_blocks和dyn_ub_size给出了具体数值，但stream的"本样例取值"列仅写了变量名stream，风格不一致
- 建议: 将stream的取值改为描述性文字如"用户创建的ACL流对象"，或给出具体示例值

### 118. `7d5bd1dcd1ed2784`

- Target: `asc-devkit`
- 文件: `docs/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/编程示例.md`
- 行号: `9-11`
- 章节: `场景说明`
- 级别: `info`
- 规则: `content.markdown.misleading_code_language`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: output[i] = input[index[i]] + 1是数学公式（缺少分号），但使用了cpp语言标签，第129-131行同样存在此问题
- 建议: 将代码块语言标签改为text，或补充分号使其成为合法C++语句

### 119. `f706f26947f78102`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/00_quickstart/hello_world_simt/README.md`
- 行号: `61`
- 章节: `执行结果`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 内联代码与中文之间存在多余空格，涉及`[blockIdx (X/2)]`和`[threadIdx (Y/32)]`两处闭合反引号后与中文"表示"之间的空格
- 建议: 移除闭合反引号后的空格，改为`[blockIdx (X/2)]`表示和`[threadIdx (Y/32)]`表示

### 120. `81dba53cd98c437f`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/basic_gather/gather_1d/README.md`
- 行号: `66`
- 章节: `编译运行`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 行内代码与相邻中文之间存在多余空格，如`${install_path}` 为、至 `/usr/local/Ascend` 下
- 建议: 去除中文与行内代码反引号之间的空格，改为`${install_path}`为、至`/usr/local/Ascend`下

### 121. `e30705370f10fe11`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/basic_gather/gather_1d/README.md`
- 行号: `44-48`
- 章节: `算子描述`
- 级别: `info`
- 规则: `content.lowlevel.list_marker_inconsistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 数据切分子列表使用*作为列表标记，而文档其余所有列表均使用-，风格不统一
- 建议: 将第44-48行的*统一替换为-

### 122. `c04aa7118db00ab8`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/basic_gather/gather_2d/README.md`
- 行号: `21`
- 章节: `目录结构介绍`
- 级别: `info`
- 规则: `content.markdown.tree_format`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 目录结构树中使用了半角管道符 "|"，与上文 box-drawing 字符 "│" (U+2502) 不一致
- 建议: 将第21行的 "|" 替换为 "│" (U+2502)，保持目录树格式统一

### 123. `47524eaef48da486`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/00_introduction/01_gather/general_gather/README.md`
- 行号: `148`
- 章节: `编译运行`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文与行内代码反引号之间存在多余空格（`${install_path}` 为、至 `/usr/local/Ascend` 下）
- 建议: 全文统一去除中文与行内代码反引号之间的空格

### 124. `e22bce94ea9d9e39`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/01_assert/README.md`
- 行号: `1`
- 章节: `标题`
- 级别: `info`
- 规则: `content.lowlevel.redundant_wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 标题中 "assert断言" 中英文语义重复，assert 即断言
- 建议: 改为 "SIMT assert功能实现样例" 或 "SIMT 断言功能实现样例"

### 125. `c5eabc3953a02fc8`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/03_profiling/msProf/README.md`
- 行号: `56-69`
- 章节: `msProf工具介绍/编译运行`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文文本与inline code反引号之间存在多余空格，共3处（L56、L69两处）
- 建议: 全文统一去除中文与inline code反引号之间的空格，如改为"打开`visualize_data.bin`文件"

### 126. `f1038da87f92e441`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/09_log/README.md`
- 行号: `47-113`
- 章节: `全文多处`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文文本与行内代码反引号之间存在多余空格，全文约10处（如第47、50、80、92、106、113行）
- 建议: 统一去除中文与行内代码反引号之间的空格，例如将"仅由 `thread 0` 触发"改为"仅由`thread 0`触发"

### 127. `4439d49a34468d6c`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/09_log/README.md`
- 行号: `121`
- 章节: `执行结果`
- 级别: `info`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "以非打屏模式下为例"措辞冗余，"模式下"与"为例"搭配不当
- 建议: 改为"以非打屏模式为例"

### 128. `f055b8003cb3d1c2`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/README.md`
- 行号: `1`
- 章节: `Utilities样例介绍`
- 级别: `info`
- 规则: `content.lowlevel.mixed_lang_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 标题中 "Utilities样例介绍" 中英文之间缺少空格，影响可读性
- 建议: 修改为 "# Utilities 样例介绍"，与英文版标题风格保持一致

### 129. `d3aed13cf3606678`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/01_utilities/README.md`
- 行号: `9`
- 章节: `样例列表`
- 级别: `info`
- 规则: `content.lowlevel.redundant_wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "assert断言" 语义重复，assert 即断言，属于冗余表述
- 建议: 精简为 "assert功能" 或 "断言功能"，避免同义重复

### 130. `9c40afcd6d720957`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/00_framework/README.md`
- 行号: `1`
- 章节: `Framework样例介绍`
- 级别: `info`
- 规则: `content.lowlevel.mixed_language`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 一级标题"Framework样例介绍"中英文混排缺少空格，影响可读性。
- 建议: 改为"Framework 样例介绍"或"框架样例介绍"。

### 131. `7417a5dd744c0400`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/README.md`
- 行号: `11`
- 章节: `样例列表`
- 级别: `info`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 表格描述"展示SIMT算子处理复杂业务逻辑能力"缺少结构助词"的"，语句略显生硬
- 建议: 修改为"展示SIMT算子处理复杂业务逻辑的能力"

### 132. `55be543307ba877a`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md`
- 行号: `28`
- 章节: `背景知识`
- 级别: `info`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第二条无序列表"用于存储键值对（key-value pair）"末尾缺少句号，与第一条列表项（第27行以句号结尾）格式不一致
- 建议: 在"（key-value pair）"后补充句号

### 133. `16105d8cb5d0c240`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md`
- 行号: `30-113`
- 章节: `背景知识/编译运行`
- 级别: `info`
- 规则: `content.lowlevel.spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中英文之间存在多余空格：第30行"hash table 中"、第113行"NPU 架构"，共2处
- 建议: 删除中英文之间的空格，改为"hash table中"和"NPU架构"

### 134. `992cf56336879a99`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md`
- 行号: `97`
- 章节: `编译运行`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 行内代码与中文之间存在多余空格："${install_path}"后与"为"之间、"至"与"/usr/local/Ascend"前后均有空格
- 建议: 删除行内代码反引号与相邻中文字符之间的空格，如改为"`${install_path}`为"和"至`/usr/local/Ascend`下"

### 135. `b776132310cec820`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md`
- 行号: `70`
- 章节: `算子描述`
- 级别: `info`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "算子类型(OpType)"使用了英文括号，中文语境应使用中文括号
- 建议: 改为"算子类型（OpType）"

### 136. `57530342bbef5d1e`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md`
- 行号: `116`
- 章节: `编译运行`
- 级别: `info`
- 规则: `content.markdown.code_block_language`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 输出示例代码块未声明语言类型
- 建议: 添加语言标识，如```text

### 137. `692bf77336fb6a36`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence/README.md`
- 行号: `107`
- 章节: `样例实现`
- 级别: `info`
- 规则: `content.lowlevel.consistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 图号引用格式不一致：第107行使用"图 1"（含空格），而第113、126、132行使用"图1""图2"（无空格）。
- 建议: 统一为无空格格式，将第107行"图 1"改为"图1"。

### 138. `f0563d57840d2cd8`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/02_atomic_operation/histogram/README.md`
- 行号: `64`
- 章节: `阶段一`
- 级别: `info`
- 规则: `content.lowlevel.punctuation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文句号"。"与"其中"之间存在一个多余空格
- 建议: 删除句号后的空格："……局部直方图。其中`asc_atomic_add`……"

### 139. `3f9e17a36e806eb6`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/02_atomic_operation/histogram/README.md`
- 行号: `82`
- 章节: `编译运行`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文文字与行内代码反引号之间存在多余空格，共2处：`${install_path}` 为、至 `/usr/local/Ascend` 下
- 建议: 去除中文与反引号之间的空格：`${install_path}`为、至`/usr/local/Ascend`下

### 140. `371517605ebbc66f`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/03_warp_instruction/sobel_warp_shfl/README.md`
- 行号: `5-113`
- 章节: `SIMT实现Sobel边缘检测样例`
- 级别: `info`
- 规则: `content.lowlevel.spacing_inconsistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "3×3"前后空格使用不一致：第5、31、50行有空格，第81、113行无空格
- 建议: 全文统一"3×3"前后的空格风格，推荐统一不加空格

### 141. `bcfc2d2060aa5fac`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/01_api_features/README.md`
- 行号: `11-14`
- 章节: `算子开发样例`
- 级别: `info`
- 规则: `content.lowlevel.consistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 表格中目录链接尾斜杠不一致：00_memory_access 和 03_warp_instruction 无尾斜杠，01_sync_instruction/ 和 02_atomic_operation/ 带尾斜杠
- 建议: 统一所有目录链接的尾斜杠格式，建议全部去掉或全部保留尾斜杠

### 142. `c839d9b1aad20fd9`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/00_basic_compile/README.md`
- 行号: `5-11`
- 章节: `概述`
- 级别: `info`
- 规则: `content.lowlevel.numbering`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 图编号格式不一致：正文中为"图 1"（有空格），图注标题为"图1"（无空格）
- 建议: 统一图编号格式，建议全部使用"图1"（无空格）或"图 1"（有空格）

### 143. `74d5e5ae9edb01de`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/00_basic_compile/README.md`
- 行号: `59`
- 章节: `CMake方式编译`
- 级别: `info`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "使用CMake来更简便地使用毕昇编译器编译"中"使用"重复出现两次，语句拗口
- 建议: 改为"借助CMake可更简便地编译Ascend C SIMT算子"或类似表述，避免重复"使用"

### 144. `13ffe85f3368bf44`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/01_separate_compile/README.md`
- 行号: `5-144`
- 章节: `全文`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 全文存在多处中文文本与行内代码反引号之间添加空格的情况，如"其中 `func.asc` 为"、"使用 `extern` 关键字"等
- 建议: 对全文进行统一排查，去除中文与行内代码反引号之间的空格

### 145. `489c83e9d3666d05`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/01_separate_compile/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `info`
- 规则: `content.lowlevel.inconsistent_formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 同一段落中文件名有时使用行内代码格式（如 `func.asc`），有时未使用（如"调用func.asc中的"），格式不统一
- 建议: 统一为所有文件名添加行内代码反引号格式

### 146. `71b22fd969eba55b`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/03_static_library_compile/README.md`
- 行号: `37-71`
- 章节: `静态库编译介绍 / 编译运行`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 全文多处中文与行内代码反引号之间存在多余空格（第37、41、47、53、71行等），如"Host侧 `.cpp` 通过"
- 建议: 全文统一排查，删除中文文字与行内代码反引号之间的空格，如改为"Host侧`.cpp`通过"

### 147. `2a4289d83a766f40`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/03_static_library_compile/README.md`
- 行号: `5-11`
- 章节: `概述`
- 级别: `info`
- 规则: `content.lowlevel.figure_reference_inconsistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 正文引用"图 1"（有空格）与图片标题"图1"（无空格）格式不一致
- 建议: 统一为"图1"或"图 1"，保持引用与标题格式一致

### 148. `e81346335cd5a135`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/06_compile/03_static_library_compile/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `info`
- 规则: `content.lowlevel.inconsistent_code_formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 概述中.asc、.cpp、.a未使用行内代码反引号包裹，而文档其余部分均使用了反引号，格式不统一
- 建议: 将概述中的.asc、.cpp、.a用反引号包裹，与全文其他章节保持一致

### 149. `4ae702df3247a9f4`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/README.md`
- 行号: `1`
- 章节: `# Features样例介绍`
- 级别: `info`
- 规则: `content.lowlevel.mixed_lang_heading`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 标题中英文单词"Features"与中文"样例介绍"之间缺少空格，影响可读性
- 建议: 改为"# Features 样例介绍"，在英文和中文之间添加空格

### 150. `a673d17de20d76d2`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/02_features/README.md`
- 行号: `12`
- 章节: `样例列表`
- 级别: `info`
- 规则: `content.lowlevel.inconsistent_link_format`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 链接"./01_api_features/"末尾带斜杠，而其他目录链接（./00_framework、./06_compile）不带斜杠，格式不一致
- 建议: 统一去掉末尾斜杠，改为"./01_api_features"

### 151. `8aed293be1054325`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/aligned_types/README.md`
- 行号: `87-225`
- 章节: `样例实现 / 性能数据获取`
- 级别: `info`
- 规则: `content.markdown.code_lang`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第87行和第225行的代码块缺少语言标识符，与文档中其他代码块（cpp/bash）风格不一致
- 建议: 第87行改为 ```cpp，第225行改为 ```bash

### 152. `131e3174e4fe0f9b`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/aligned_types/README.md`
- 行号: `197`
- 章节: `执行结果`
- 级别: `info`
- 规则: `content.lowlevel.grammar`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 预期输出 "Case accuracy is verification passed." 存在英文语法错误
- 建议: 若为程序实际输出则保持不变，但建议确认程序端是否已修正该输出文案

### 153. `182ce4351063b70f`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `179-190`
- 章节: `Case 1: 数据缓存优化版本 / 性能对比总结`
- 级别: `info`
- 规则: `content.simt.calculation`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 百分比计算偏差：1533/5064约为30.27%，四舍五入应为30.3%，文档写为30.2%，两处均存在此问题
- 建议: 将"约30.2%"修改为"约30.3%"

### 154. `9dc74ea3b46ac65e`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `123-167`
- 章节: `Case 0 / Case 1 关键代码`
- 级别: `info`
- 规则: `content.simt.example`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 代码片段中使用了sign变量但未展示其定义，读者无法理解该变量来源
- 建议: 在代码片段中补充sign变量的计算逻辑，或在代码前添加说明文字解释sign的含义

### 155. `56652ff2d56b6d63`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `251-260`
- 章节: `性能分析`
- 级别: `info`
- 规则: `content.lowlevel.formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 目录树格式与文档内第一处目录树不一致：此处├──后无空格，而第18-22行├──后有空格
- 建议: 在├──和└──后统一添加空格，与第18-22行保持一致

### 156. `45523e87cea0df98`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `251-260`
- 章节: `性能分析`
- 级别: `info`
- 规则: `content.lowlevel.formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 目录树注释风格与文档内第一处目录树不一致：此处使用#注释，而第19-21行使用//注释
- 建议: 统一目录树中的注释风格，建议全部使用//或全部使用#

### 157. `76467b3b9aff59b1`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/README.md`
- 行号: `182`
- 章节: `Case 1: 数据缓存优化版本`
- 级别: `info`
- 规则: `content.markdown.spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 水平分隔线---前缺少空行，直接紧跟列表项，部分Markdown解析器可能渲染异常
- 建议: 在---前添加一个空行

### 158. `252ac57fe3ad1752`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice/README.md`
- 行号: `109`
- 章节: `性能指标说明`
- 级别: `info`
- 规则: `content.lowlevel.wording`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: "均展示的为"句式杂糅，语法不通顺
- 建议: 改为"本例中其余指标所展示的均为所有block上性能指标的平均值"或"本例中其余指标均为所有block上性能指标的平均值"

### 159. `5cf0ed142e0135bc`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice/README.md`
- 行号: `310`
- 章节: `性能对比总结`
- 级别: `info`
- 规则: `content.simt.incomplete_perf_scope`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 标题"Ascend 950PR性能数据"仅提及950PR，但文档支持的产品包含950PR和950DT，读者无法判断数据适用范围
- 建议: 明确说明性能数据是否同时适用于Ascend 950DT，或补充950DT的性能数据

### 160. `e304a527c53d3b09`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add/README.md`
- 行号: `213-235`
- 章节: `编译运行`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 文档中多处中文文本与 inline code 反引号之间存在多余空格（lines 213, 227, 235）
- 建议: 统一全文，消除中文与反引号之间的空格，如"执行`rm CMakeCache.txt`后"

### 161. `aba877f37c2e864c`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add/README.md`
- 行号: `173-181`
- 章节: `Case 2: half2类型数据处理`
- 级别: `info`
- 规则: `content.lowlevel.parallel_structure`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 原理说明中 Case 1 与 Case 2 的并列条目格式不一致：Case 1 用冒号分隔，Case 2 用句号且句式不同
- 建议: 统一两条目的句式结构，如均使用"Case N（描述）：具体说明"格式

### 162. `10681e2405ff4d2a`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add/README.md`
- 行号: `199`
- 章节: `优化要点总结`
- 级别: `info`
- 规则: `content.lowlevel.numbering`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 适用场景列使用了"1、"编号但无后续"2、"条目，编号多余或缺失内容
- 建议: 若仅有一项，去掉"1、"前缀；若有更多场景，补充后续条目

### 163. `37cd13a3e4e057f6`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add/README.md`
- 行号: `277`
- 章节: `性能数据获取`
- 级别: `info`
- 规则: `content.markdown.code_block_language`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 最后一个代码块缺少语言标识符，与文档中其他代码块（bash/cpp/text）不一致
- 建议: 将 ``` 改为 ```bash

### 164. `ba2508f13c083c62`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/README.md`
- 行号: `5`
- 章节: `概述`
- 级别: `info`
- 规则: `content.lowlevel.formatting`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 内核启动语法符号 <<<>>> 未使用行内代码反引号包裹，易与普通文本混淆
- 建议: 将 <<<>>> 改为 `<<<>>>`

### 165. `a61a72c2e4138ea0`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/grid_dim_config/README.md`
- 行号: `263`
- 章节: `编译运行`
- 级别: `info`
- 规则: `content.lowlevel.inline_code_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 中文与行内代码反引号之间存在空格：`安装至 \`/usr/local/Ascend\` 下`
- 建议: 去掉中文与行内代码之间的空格，改为`安装至\`/usr/local/Ascend\`下`

### 166. `0eb1add6aee75057`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/grid_dim_config/README.md`
- 行号: `251`
- 章节: `调优建议`
- 级别: `info`
- 规则: `content.lowlevel.formatting_consistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 工具名称`msprof`在第251行未使用行内代码格式，而在第289行使用了行内代码格式，格式不一致
- 建议: 统一将第251行的msprof改为行内代码格式`msprof`

### 167. `96dd0c7260b0b2ed`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/01_execution_conf_optimizations/grid_dim_config/README.md`
- 行号: `297-307`
- 章节: `性能分析`
- 级别: `info`
- 规则: `content.markdown.directory_tree_consistency`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 第二处目录树（第297-307行）`├──`后无空格且注释用`#`，与第一处目录树（第17-22行）`├──`后有空格且注释用`//`的风格不一致
- 建议: 统一目录树格式：在`├──`后添加空格，并统一注释符号

### 168. `e15cbf310e7723b3`

- Target: `asc-devkit`
- 文件: `examples/03_simt_api/03_best_practices/README.md`
- 行号: `1`
- 章节: `# BestPractices样例介绍`
- 级别: `info`
- 规则: `content.lowlevel.mixed_script_spacing`
- 诊断: `文档内容问题` / `document_content`
- 处理状态: `open`
- 负责人: `未分配`
- Skill: `asc-simt-doc-quality`
- 子模式: `default`
- 问题: 标题中英文 "BestPractices" 与中文 "样例介绍" 直接拼接无空格，且与同级文档标题风格不一致（00_introduction用纯中文，02_features同样无空格）
- 建议: 统一为 "Best Practices 样例介绍" 或纯中文 "最佳实践样例介绍"，并与同级文档保持一致

