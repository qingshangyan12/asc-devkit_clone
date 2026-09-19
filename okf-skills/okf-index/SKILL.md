---
name: okf-index
description: 将范围内 140 篇 OKF YAML 头构建为可复用的持久化检索索引（倒排视图 + 依赖图 + 缺口登记），并提供统一查询 CLI。构建为确定性纯派生，提交 git 后日常检索无需重建；校验不过不出索引。
---

# OKF 索引建设

本 Skill 是阶段 4 交付物：把阶段 3 生成的 front matter 组织成持久化索引，支撑阶段 5
`okf-retrieve` 的算子需求检索。索引是 front matter 的**纯派生数据**，不承载独立知识；
一切内容以文档 YAML 头为唯一依据，文档与索引不一致时以重建解决，不手改 `index.json`。

## 资产结构

```
okf-index/
  scripts/build_index.py   # 构建 + 校验 + 漂移检测（--check）
  scripts/query.py         # 查询 CLI（唯一检索入口）
  index/index.json         # 主索引：documents + 9 视图 + 依赖图 + scope（构建产物）
  index/gaps.json          # 缺口登记：范围外能力缺口 + 关系缺口（人工维护、构建校验）
  index/build-meta.json    # 源 commit、文档 sha256、词表/缺口指纹（构建产物）
  index/概览.md             # 人读概览（构建产物）
  tests/test_index.py      # 确定性 + 视图正确性 + 20 题 dry-run
  tests/dryrun.yaml        # 20 题查询清单：阶段5检索策略基线
```

## 复用依赖

`build_index.py` 直接 import `okf-yaml-generate/scripts/controller.py` 的
`split_front_matter / taxonomy / ALLOWED_FIELDS / DEPENDENCY_KINDS`，不复制解析与词表规则。
controller.py 路径变更时本 skill 同步调整 import（阶段 6 可收敛为共享模块）。

## 构建流程

1. 文档 front matter 变化后运行：`python3 scripts/build_index.py`（仓库根执行）。
2. 构建内联全量校验，任一失败即拒绝出索引：
   - 字段无越界（空 `examples`/`signature` 键允许缺省）、okftype 合法、tag 值全部出自冻结词表；
   - signature 头文件存在于 `include/`、examples 路径存在、依赖目标存在（允许指向范围外文档，打 `out_of_scope` 标记）；
   - `requires` 边无环（`requires` + `configures` 对偶边属合法语义，不判环）；
   - gaps.json 的 hint/evidence 路径全部存在。
3. 家族上卷：架构变体页（`_arch_*`）的 tags/signature/deps/examples 上卷到家族主页，
   视图只索引主文档（119 个），变体页经 `--api` 家族卡可达。
4. 构建确定性：输出不含时钟，同输入两次构建字节一致；`build-meta.json` 记录 140 个文档
   sha256 用于陈旧检测。`meta.source_commit`/`commit_date` 为信息性字段（记录构建时 HEAD，
   提交索引后天然滞后一个提交），漂移比较时排除。
5. 漂移检测：`python3 scripts/build_index.py --check`（文档改动未重建索引时退出码 1）。

## 查询接口

`query.py` 过滤维度间 AND、逗号值内 OR；独立查询返回结构化卡片：

```
--stage in,out,compute          # 分阶段候选（roles+locations 判定）
--operation divide,gather       # operations 维度
--category vector_compare       # 目录分类维度
--dtype half,f32                # dtype 别名归一（f32→float 等）
--kw 非对齐,填充                 # title+description 子串
--convert half int4,int32       # 类型转换对（B 类）
--api asc_copy_gm2ub            # 家族卡：主页+架构变体+分类页
--deps-of <api|path> [--kind K] [--reverse]   # 依赖图遍历（C 类）
--gap 同步,寄存器               # 范围外缺口登记，逗号多关键词（E 类）
--json                          # 机器可读输出
```

输出统一为 `path | API 名 | 命中原因`，对齐评测答案格式「候选 + 未覆盖说明」。

## 20 题 dry-run（阶段 5 验收锚点）

`tests/dryrun.yaml` 以 ≤2 次查询/题复现测试集全部 20 题：required 全覆盖（家族等价展开）、
替代组每组 ≥1 成员、E 类缺口命中。该清单是 `okf-retrieve` 检索策略的基线示范，
阶段 5 可优化但不得低于此覆盖。运行：`python3 -m unittest discover -s okf-skills/okf-index/tests`。

## 缺口登记规则（gaps.json）

- 只收录有结构化证据的条目：范围内文档正文/依赖指向范围外，或源码声明存在而文档不在 scope；
  每条 hint/evidence 必须是真实存在的仓库路径（构建强校验），禁止为凑查询结果编造。
- 初始 3 条范围外缺口（流水线事件同步、矢量寄存器加载/存储、浮点 floor）来自阶段 1-3 固化发现；
  E 类查询命中时必须如实输出「范围内无文档」+ 范围外线索，不得改写为范围内文档。
- 阶段 3 的 5 条 `report_unresolved` 复核结论：4 条样例 marker 非唯一属内部记账问题，
  不是知识缺口；1 条依赖目标歧义（asc_get_cmp_mask「需和Compare操作配合使用」未链接具体接口）
  登记为 `relation_gaps`，检索该 API 时应提示配合对象待确认。
- 阶段 7 评测须单列「缺口登记对 E 类的贡献」，复核是否存在过拟合。

## 维护边界

- 文档新增/修改/删除后：重跑 `build_index.py` 并提交索引 diff；`test_committed_index_fresh`
  会在索引陈旧时失败，这是预期的漂移报警。
- gaps.json 由人工按证据增补，构建只做存在性校验；查询零重建成本。
- 范围外目录（sync/、reg_compute/ 等）不因检索命中缺口而自动纳入建设范围。
