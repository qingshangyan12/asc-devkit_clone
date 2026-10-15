# C API 最小检索测试集：9.2.0 × 3510

本测试集覆盖 15 篇已写入 OKF slim 头的 C API 文档，筛选条件为 CANN 9.2.0、NPU 架构 3510。

覆盖能力包括：GM/UB 搬入搬出、对齐/非对齐与填充、Reg load/store、掩码计算、绝对值与除法、
half/FP8 转换、饱和控制和流水同步。`annotations.json` 保存语义标注，`include-briefs.json`
保存具体重载摘要，`index/` 是对应的 asc-search 派生索引。

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
