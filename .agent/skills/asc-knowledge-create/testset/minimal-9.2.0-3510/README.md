# C API 最小检索测试集：9.2.0 × 3510

本测试集覆盖 15 篇已写入 OKF slim 头的 C API 文档，筛选条件为 CANN 9.2.0、NPU 架构 3510。

覆盖能力包括：GM/UB 搬入搬出、对齐/非对齐与填充、Reg load/store、掩码计算、绝对值与除法、
half/FP8 转换、饱和控制和流水同步。`annotations.json` 保存语义标注，`include-briefs.json`
保存具体重载摘要，`index/` 是对应的 asc-search 派生索引。

`tasks.yaml` 是自然语言检索问题集，共 36 题，包含能力定位、流程组合、依赖查找，
以及产品、版本、数据类型、地址空间、参数和行为约束不满足的反向问题。
每题仅含 `id` 和 `prompt`，不提供答案、预期候选或正反标签。逐题独立执行，版本、架构和
检索范围以题目明确指定的条件为准；“已标注的 15 篇”指 `selection.json` 中的文档。
`queries.json` 保留原有 CLI 冒烟检查，两者用途不同。

示例冒烟查询：

```bash
python3 .agent/skills/asc-search/scripts/query.py \
  --index-dir .agent/skills/asc-knowledge-create/testset/minimal-9.2.0-3510/index \
  --operation absolute --json
python3 .agent/skills/asc-search/scripts/query.py \
  --index-dir .agent/skills/asc-knowledge-create/testset/minimal-9.2.0-3510/index \
  --convert half float --json
python3 .agent/skills/asc-search/scripts/query.py \
  --index-dir .agent/skills/asc-knowledge-create/testset/minimal-9.2.0-3510/index \
  --deps-of asc_copy_gm2ub_align_arch_3510 --json
```
