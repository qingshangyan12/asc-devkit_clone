# 全量轮执行问题登记（等待修复）

记录执行过程中发现的预料之外问题。规则：非阻塞问题只登记不修复，保持标注流水线推进；
阻塞问题除外（需当场解决并在下表补记处理方式）。

| 编号 | 类别 | 描述 | 影响 | 状态 |
| --- | --- | --- | --- | --- |
| ISS-001 | 推导缺口 | `asc_copy_l12fb` 基础页声明（cube_datamove.h）无 @override_id/@archs/Doxygen，正文无 npu 标记 → archs unknown 进 unresolved（保守保留，索引结果正确）；同族 _arch_3510/_arch_2201 变体页正常 | 1 条 unresolved 噪音；根因是头文件注解缺失 | 待修复（document-issues DOC-002） |
| ISS-002 | 推导缺口 | 8 篇文档家族名在 include/c_api 无匹配声明（highdim_split 家族、l12l0a/b_sync_2d/3d 废弃变体、asc_vf_call）→ unresolved「no declaration found」，npu 兜底后门控正确 | 8 条 unresolved 噪音；根因是接口改名/删除后头文件与文档未同步 | 待修复（document-issues DOC-004） |
| ISS-003 | 标注一致性 | 跨批次判例漂移已在整合 pass 修复：纯转换类 elementwise 统一移除（28 处）、rounding_mode.md 依赖 kind 统一 constrained_by（13 处）、requires 环 4 处破环（后置/配对方向降级 related） | 已消除 | 已修复 |
| ISS-004 | 推导缺口 | 废弃双架构接口（如 asc_copy_gm2ub_align_sync，声明 @archs 2201+3510）的 `_arch_2201` 变体页按声明并集门控漏入 A5 索引（5 篇） | 已修复：变体页文档级 archs 收敛为页面架构 | 已修复 |
| ISS-005 | 文档源缺陷 | asc_log 正文/公式为自然对数，与 asc_ln 语义重复，且与命名暗示不符 | 检索歧义；标注按正文 natural_log | 待修复（document-issues DOC-003） |

## 本轮已修复（执行前修复，非本表范围）

- `derive_signatures` 在 `mode=="none"`（include 无匹配声明）时提前 return，跳过正文 npu 标记兜底 → 22 篇带 npu 证据的文档被误判 unknown
- 家族名未剥离纯 `_deprecated` 后缀（如 `asc_set_gm2ub_pad_deprecated.md` 查不到存在的声明）
- include_db 不解析对象式宏别名（`#define asc_mem_bar mem_bar` 等 4 个）
- `conversion_pair` 用原始 stem 解析，`_arch_NNNN`/`_deprecated` 后缀页丢失转换对
