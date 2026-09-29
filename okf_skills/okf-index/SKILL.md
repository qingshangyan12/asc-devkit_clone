---
name: okf-index
description: 按 API surface 构建相互隔离的持久化检索索引实例（当前 c_api 全量 838 篇：倒排视图 + 依赖图 + 缺口登记），并提供统一查询 CLI（--surface，默认 c_api）。每个 surface 独立目录 index/<surface>/、独立构建/漂移检测/缺口登记；构建为确定性纯派生，校验不过不出索引。
---

# OKF 索引建设

本 Skill 是阶段 4 交付物：把阶段 3 生成的 front matter 组织成持久化索引，支撑阶段 5
`okf-retrieve` 的算子需求检索。索引是 front matter 的**纯派生数据**，不承载独立知识；
一切内容以文档 YAML 头为唯一依据，文档与索引不一致时以重建解决，不手改 `index.json`。
**Surface 隔离**：每个 API surface 一个独立索引实例（`index/<surface>/`），文档、视图、
依赖图、缺口登记与漂移检测互不共享；一个 surface 的变更不影响其他 surface 的索引产物。

## 资产结构

```
okf-index/
  scripts/build_index.py        # 构建 + 校验 + 漂移检测（--check）；--scope 选择 surface
  scripts/query.py              # 查询 CLI（唯一检索入口）；--surface 默认 c_api
  index/c_api/index.json        # c_api 实例主索引：documents + 9 视图 + 依赖图 + scope（构建产物）
  index/c_api/gaps.json         # c_api 实例缺口登记：范围外缺口 + 关系缺口（人工维护、构建校验）
  index/c_api/build-meta.json   # c_api 实例源 commit、文档 sha256、词表/缺口指纹（构建产物）
  index/c_api/概览.md            # c_api 实例人读概览（构建产物）
  tests/test_index.py           # surface 注册表 + 确定性 + 视图正确性 + 20 题 dry-run
  tests/dryrun.yaml             # c_api 实例 20 题查询清单 + extended 新能力面查询
```

## Surface 注册表与隔离规则

- surface 在 `scripts/build_index.py` 的 `SCOPES` 注册表登记（scope 根目录、根导航页
  文件名）；当前仅 `c_api`。新 surface 接入按 `okf-maintain` 接入清单校准分类/导航
  规则后在注册表登记。
- 每个 surface 一套 `index/<surface>/{index.json,gaps.json,build-meta.json,概览.md}`；
  构建、`--check` 漂移检测、缺口登记、dryrun 清单均按 surface 独立执行。
- `index.json` 的 `meta.surface` 记录实例归属（schema `okf-index/2`）。
- 未登记的 surface：构建与查询直接报错并列出已注册项，不回退到其他实例。

## 复用依赖

`build_index.py` 直接 import `okf-yaml-generate/scripts/controller.py` 的
`split_front_matter / taxonomy / ALLOWED_FIELDS / DEPENDENCY_KINDS`，不复制解析与词表规则。
controller.py 路径变更时本 skill 同步调整 import（阶段 6 可收敛为共享模块）。

## 构建流程

1. 文档 front matter 变化后运行：`python3 scripts/build_index.py --scope c_api`
   （仓库根执行；`--scope` 默认 c_api，产物写入 `index/<scope>/`）。
2. 构建内联全量校验，任一失败即拒绝出索引：
   - 字段无越界（空 `examples`/`signature` 键允许缺省）、okftype 合法、tag 值全部出自冻结词表；
   - signature 头文件存在于 `include/`、examples 路径存在、依赖目标存在（允许指向范围外文档，打 `out_of_scope` 标记）；
   - `requires` 边无环（`requires` + `configures` 对偶边属合法语义，不判环）；
   - gaps.json 的 hint/evidence 路径全部存在。
3. 家族上卷：架构变体页（`_arch_*`）的 tags/signature/deps/examples 上卷到家族主页，
   视图只索引主文档（710 个，含 29 个 deprecated 散置架构页——其 family 无 index 主页承接，
   标记 standalone_primary 独立参与检索），变体页经 `--api` 家族卡可达。
4. 构建确定性：输出不含时钟，同输入两次构建字节一致；`build-meta.json` 记录 838 个文档
   sha256 用于陈旧检测。`meta.source_commit`/`commit_date` 为信息性字段（记录构建时 HEAD，
   提交索引后天然滞后一个提交），漂移比较时排除。
5. 漂移检测：`python3 scripts/build_index.py --scope c_api --check`（文档改动未重建索引时退出码 1）。

## 查询接口

`query.py` 过滤维度间 AND、逗号值内 OR；独立查询返回结构化卡片：

```
--surface c_api                 # 选择 API surface 索引实例（默认 c_api；--index-dir 可显式指定目录）
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

`tests/dryrun.yaml`（c_api 实例清单）以 ≤2 次查询/题复现测试集全部 20 题：required 全覆盖（家族等价展开）、
替代组每组 ≥1 成员、E 类原缺口能力文档可检索（缺口登记已清空）。该清单是 `okf-retrieve`
检索策略的基线示范，阶段 5 可优化但不得低于此覆盖；`extended` 小节抽验 cube 搬运、
matrix_compute、defs 类型页、fp8/fp4 转换对、deprecated 家族卡等新能力面。
新 surface 接入时另行编制其实例的 dryrun 清单。
运行：`python3 -m unittest discover -s okf-skills/okf-index/tests`。

## 缺口登记规则（gaps.json）

- gaps.json 是 surface 实例级文件（`index/<surface>/gaps.json`），只登记该 surface 范围的缺口；
  只收录有结构化证据的条目：范围内文档正文/依赖指向范围外，或源码声明存在而文档不在 scope；
  每条 hint/evidence 必须是真实存在的仓库路径（构建强校验），禁止为凑查询结果编造。
- 初始 3 条范围外缺口（流水线事件同步、矢量寄存器加载/存储、浮点 floor）对应的
  sync/、reg_compute/ 等目录已随 c_api 全量纳入索引范围，登记已清空（scope_out_gaps=[]）；
  `--gap` 在登记为空时返回「无登记缺口」。E 类评测任务相应改为断言能力文档可检索。
- 阶段 3 的 1 条依赖目标歧义（asc_get_cmp_mask「需和Compare操作配合使用」未链接具体接口）
  登记为 `relation_gaps`，检索该 API 时应提示配合对象待确认。
- 阶段 7 评测须单列「缺口登记对 E 类的贡献」，复核是否存在过拟合。

## 维护边界

- 文档新增/修改/删除后：重跑对应 surface 的 `build_index.py --scope <surface>` 并提交索引 diff；
  `test_committed_index_fresh` 会在索引陈旧时失败，这是预期的漂移报警。
- gaps.json 由人工按证据增补（每个 surface 独立），构建只做存在性校验；查询零重建成本。
- 新 API 文档范围接入时在 `SCOPES` 注册表登记 scope 并校准 classify 分类/导航规则与该
  surface 的 dryrun 查询清单，按 `okf-maintain` 接入清单编排；不影响既有 surface 实例。
