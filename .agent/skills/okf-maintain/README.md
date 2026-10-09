# OKF Maintain README

这是建设侧 skill `okf-maintain` 的全貌说明：它是干什么的、怎么用、每个文件的作用、账本长什么样。

## 1. 这个 skill 是干什么的

### 1.1 背景

`okf-build` 把 C API 文档建成三类"头"（文档 front matter、include Doxygen、样例 marker），
`okf-search` 消费这些头建索引。但源内容会变：文档被编辑、Release Notes 出新版本、词表换版、
代码被改动——头不会自动跟着变，于是产生**漂移**。`okf-maintain` 就是给**指定文档**做体检、
出报告并修复：

- 只查你指定的文档/目录，不做全仓审计；
- 确定性漂移（结构可推导）自动修；语义漂移按正文真实含义重写；
- include 注解与样例 marker 任何时刻都能从现行源重算，可直接幂等刷新。

### 1.2 三类头怎么算漂移

| 头 | 漂移判定 | 修复方式 |
| --- | --- | --- |
| 文档头 | profile/字段、okftype 重分类、version 重推导、tag 合法性、依赖目标、正文哈希与证据复检 | okftype/version 自动原位刷新；语义字段走标注管线 |
| include 头 | 对指定文档重推注解，与现有头做 diff | `update --write` 幂等写入 |
| examples 头 | 指定文档声明的 API 命中的 featured 样例，重算调用点与 marker | `update --write` 幂等写入 |

报告按四级定级：`DET`（自动可修）、`SEM-HIGH`（语义失效，必改）、`SEM-LOW`（建议复核）、
`INFO`（提示，不阻断）。分类与判定细节见 [references/drift.md](references/drift.md)。

### 1.3 边界

- 只维护存量头，不首次建头（首次用 `okf-build`）、不建索引/检索（用 `okf-search`）。
- 不自动删除既有内容；孤儿注解/marker 不处理，文档自身问题登记 okf-build 的 document-issues。
- 依赖同仓 okf-build 的脚本与配置，两个 skill 需一起分发。

## 2. 怎么快速使用

前提：本地有 asc-devkit 仓库、Python 3（依赖 PyYAML）。从本 skill 目录执行。

### 2.1 体检

```bash
python3 scripts/maintain.py check --devkit-root /path/to/asc-devkit \
  --document docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md
# 全量（较慢）：不传 --document/--source-dir
# 快速确定性检查：--checks doc
```

输出：屏幕摘要 + `tmp/maintain/<ts>/drift-report.md|json`；退出码 0=干净（仅 INFO），1=发现漂移。

### 2.2 确定性修复

```bash
python3 scripts/maintain.py update --devkit-root /path/to/asc-devkit \
  --document docs/.../asc_add.md            # dry-run
python3 scripts/maintain.py update --devkit-root /path/to/asc-devkit \
  --document docs/.../asc_add.md --write    # 落盘
```

`update` 只处理 DET：okftype/version 原位替换（正文字节不变），include/marker 幂等写入；
语义漂移留给标注管线。

### 2.3 语义修复与账本

```bash
# 走 okf-build 标注管线重写语义字段（prepare → 写 annotations → validate → write）
python3 ../okf-build/scripts/controller.py prepare --devkit-root <repo> \
  --document <漂移文档> --run-dir tmp/maintain/<ts>/annotate
# Agent 按 annotation-contract.md 写 annotations/*.json（正文逐字证据）
python3 ../okf-build/scripts/controller.py validate --run-dir tmp/maintain/<ts>/annotate
python3 ../okf-build/scripts/controller.py write --run-dir tmp/maintain/<ts>/annotate --write

# 刷新账本（记录新正文哈希与新证据）
python3 scripts/maintain.py baseline --from-run tmp/maintain/<ts>/annotate
```

首次使用先 `baseline` 建账（`--from-run` 可读取历史标注工件作为证据）。

## 3. 内部实现方案

### 3.1 数据流

```text
指定文档 ─┬─ 文档头检查：重分类/重推导/词表/依赖 + 账本（正文哈希、证据复检）
          ├─ include 检查：重推注解 → 与现有头 diff
          └─ examples 检查：featured 命中 → 重算调用点 → marker diff
                    │
                    ▼
             漂移报告（DET/SEM/INFO）
                    │
        DET ── update --write（原位/幂等）
        SEM ── okf-build 标注管线（逐字证据）→ baseline 刷新账本
```

### 3.2 目录与文件作用

```text
okf-maintain/
├── SKILL.md              # 给 AI 的入口：触发场景、命令、修复流程与约束
├── README.md             # 本文件：给人的全貌说明
├── agents/openai.yaml    # 客户端 UI 元数据
├── references/
│   ├── README.md         # reference 导航
│   └── drift.md          # 漂移分类/定级、账本格式、check/update/baseline 流程
├── scripts/maintain.py   # 主 CLI（check/update/baseline），引用 okf-build 脚本
├── state/c_api.json      # 随仓提交的漂移账本（每篇正文哈希 + 头快照 + 证据）
├── tests/test_maintain.py # 纯函数 + 合成仓漂移用例
└── tmp/                  # 运行期报告（不提交）
```

### 3.3 账本（state/c_api.json）

| 键 | 含义 |
| --- | --- |
| `configs.*_sha256` | 词表、version-overrides、profile registry、type registry 的指纹，用于发现配置换版 |
| `documents[rel].body_sha256` | 正文（front matter 之后）哈希，判断头写入后正文是否变化 |
| `documents[rel].header` | 记入时的头值快照，发现绕过管线的手工修改 |
| `documents[rel].evidence` | description/tag/dependencies 的正文逐字证据，用于正文变化后的证据复检 |
| `documents[rel].recorded_at` | 记录时间 |

`evidence` 只在能拿到标注工件时写入（`baseline --from-run`）；没有证据时正文一旦变化
报告为 `doc.body_changed_unverified`（建议人工复核），不会误判为仍有效。

## 4. 测试

```bash
python3 tests/test_maintain.py
```

覆盖：front matter 原位刷新、账本证据提取、version/okftype 漂移往返、正文变化证据存活/失效、
include 注解漂移往返、样例 marker 漂移往返、`baseline --from-run` 记录证据。
测试在临时目录复制真实仓文件为夹具，不写入真实文件。
