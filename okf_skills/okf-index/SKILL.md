---
name: okf-index
description: 将 c_api 全量 800 篇 OKF YAML 头构建为可复用的持久化检索索引（倒排视图 + 依赖图 + 缺口登记），并提供统一查询 CLI。构建为确定性纯派生，提交 git 后日常检索无需重建；校验不过不出索引。
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
  tests/test_index.py      # 确定性 + 视图正确性 + 20 题 dry-run + 新能力面抽验
  tests/dryrun.yaml        # 20 题查询清单 + extended 新能力面查询（阶段5检索策略基线）
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
   视图只索引主文档（710 个，含 29 个 deprecated 散置架构页——其 family 无 index 主页承接，
   标记 standalone_primary 独立参与检索），变体页经 `--api` 家族卡可达。
4. 构建确定性：输出不含时钟，同输入两次构建字节一致；`build-meta.json` 记录 800 个文档
   sha256 用于陈旧检测。`meta.source_commit`/`commit_date` 为信息性字段（记录构建时 HEAD，
   提交索引后天然滞后一个提交），漂移比较时排除。
5. 漂移检测：`python3 scripts/build_index.py --check`（文档改动未重建索引时退出码 1）。

## 查询接口

`query.py` 过滤维度间 AND、逗号值内 OR；独立查询返回结构化卡片：

```
--stage in,out,compute          # 分阶段候选（roles+locations 判定；含 gm↔L0A/L1 等 cube 搬运与 UB↔矢量寄存器 load/store）
--operation divide,gather       # operations 维度（含 matrix_multiply_accumulate、synchronization 等）
--category vector_compute,defs  # 一级目录分类维度（子目录不再是独立分类）
--dtype half,f32,fp8_e4m3fn     # dtype 别名归一（f32→float、e4m3→fp8_e4m3fn 等）
--kw 非对齐,填充                 # title+description 子串
--convert half int4,bfloat16 float  # 类型转换对（含 fp8/hifloat8/fp4x2 新 token）
--api asc_copy_gm2ub            # 家族卡：主页+架构变体+同族成员+分类页（deprecated 散置家族可查）
--deps-of <api|path> [--kind K] [--reverse]   # 依赖图遍历（C 类）
--gap 同步,寄存器               # 缺口登记检索（scope_out_gaps 为空时优雅返回「无登记缺口」）
--json                          # 机器可读输出
```

输出统一为 `path | API 名 | 命中原因`，对齐评测答案格式「候选 + 未覆盖说明」。

## 20 题 dry-run（阶段 5 验收锚点）

`tests/dryrun.yaml` 以 ≤2 次查询/题复现测试集全部 20 题：required 全覆盖（家族等价展开）、
替代组每组 ≥1 成员、E 类原缺口能力文档可检索（缺口登记已清空）。该清单是 `okf-retrieve`
检索策略的基线示范，阶段 5 可优化但不得低于此覆盖；`extended` 小节抽验 cube 搬运、
matrix_compute、defs 类型页、fp8/fp4 转换对、deprecated 家族卡等新能力面。
运行：`python3 -m unittest discover -s okf-skills/okf-index/tests`。

## 缺口登记规则（gaps.json）

- 只收录有结构化证据的条目：范围内文档正文/依赖指向范围外，或源码声明存在而文档不在 scope；
  每条 hint/evidence 必须是真实存在的仓库路径（构建强校验），禁止为凑查询结果编造。
- 初始 3 条范围外缺口（流水线事件同步、矢量寄存器加载/存储、浮点 floor）对应的
  sync/、reg_compute/ 等目录已随 c_api 全量纳入索引范围，登记已清空（scope_out_gaps=[]）；
  `--gap` 在登记为空时返回「无登记缺口」。E 类评测任务相应改为断言能力文档可检索。
- 阶段 3 的 1 条依赖目标歧义（asc_get_cmp_mask「需和Compare操作配合使用」未链接具体接口）
  登记为 `relation_gaps`，检索该 API 时应提示配合对象待确认。
- 阶段 7 评测须单列「缺口登记对 E 类的贡献」，复核是否存在过拟合。

## 维护边界

- 文档新增/修改/删除后：重跑 `build_index.py` 并提交索引 diff；`test_committed_index_fresh`
  会在索引陈旧时失败，这是预期的漂移报警。
- gaps.json 由人工按证据增补，构建只做存在性校验；查询零重建成本。
- 新目录并入 scope 时须同步校准 classify 的分类/导航规则与 dryrun 查询清单；
  新 API 文档范围的全量接入（含本 skill 索引段的参数化改造）按 `okf-maintain`
  的接入清单编排。
