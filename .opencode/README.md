# asc-devkit OpenCode 配置

本目录维护 `asc-devkit` 仓内的本地 skill、安装脚本和 skill 级测试。它面向两类读者：

- 人类使用者：安装、卸载、查看 skill 能力和运行测试。
- AI Agent：按仓内事实源维护 skill、引用 reference、生成或补齐 API UT。

## 给人类使用者

### 当前内置 skill

| Skill | 用途 | 入口 |
|------|------|------|
| `asc-api-ut-gen` | 生成或补齐 `tests/api/` 下的 AscendC API UT | `skills/asc-api-ut-gen/SKILL.md` |
| `asc-npu-arch` | 维护芯片、架构、`SocVersion`、`__NPU_ARCH__` 和 dtype 事实 | `skills/asc-npu-arch/SKILL.md` |

当前 `.opencode` 不维护设计文档生成、API 实现生成、示例工程生成等 skill。

### 安装

默认安装到 `${CODEX_HOME:-$HOME/.codex}/skills`：

```bash
bash .opencode/install-skills.sh
```

显式安装目标：

```bash
bash .opencode/install-skills.sh --codex
bash .opencode/install-skills.sh --claude
bash .opencode/install-skills.sh --all
```

安装方式是创建软链接，不复制 skill 内容。安装后重启对应工具，才能重新发现 `$asc-api-ut-gen` 和 `$asc-npu-arch`。

### 卸载

默认卸载 Codex 用户目录中的软链接：

```bash
bash .opencode/install-skills.sh --uninstall
```

按目标卸载：

```bash
bash .opencode/install-skills.sh --uninstall --codex
bash .opencode/install-skills.sh --uninstall --claude
bash .opencode/install-skills.sh --uninstall --all
```

卸载只删除指向本仓 `.opencode/skills/<skill-name>` 的软链接。普通文件或目录不会被删除；同名软链接如果指向其他位置，需要显式加 `--force`。

### 测试

运行全部 skill 测试：

```bash
bash .opencode/tests/skills/run-all.sh
```

单独测试：

```bash
bash .opencode/tests/skills/asc-api-ut-gen/test.sh
bash .opencode/tests/skills/asc-npu-arch/test.sh
bash .opencode/tests/skills/install/test.sh
```

## 给 AI Agent

### 维护边界

- `.opencode/skills` 是本仓 skill 的唯一事实来源。
- 不直接编辑 `.claude/skills`；需要 Claude Code 兼容链接时，通过 `install-skills.sh --claude` 生成。
- 不在本仓维护 `.agents/plugins`、`plugins/ascendc-api-generator` 或外部安装目录中的 skill 副本。
- 当前 `asc-api-ut-gen` skill 忽略 Tensor API，不维护 `include/experimental/tensor_api/`、`impl/experimental/tensor_api/`、`tests/api/tensor_api/` 的生成规则。

### 事实源顺序

| 事实类型 | 首选来源 |
|---------|----------|
| 芯片、架构、`SocVersion`、`__NPU_ARCH__`、dtype | `skills/asc-npu-arch/references/npu-arch-facts.json` |
| 架构和 dtype 人工说明 | `skills/asc-npu-arch/references/npu-arch-guide.md` |
| API 类型、目录和 UT 目录映射 | `skills/asc-api-ut-gen/references/foundations/api-directory-map.md` |
| API 类型限制和生成器约束 | `skills/asc-api-ut-gen/references/foundations/generation-constraints.json` |
| UT 生成流程、覆盖率和报告要求 | `skills/asc-api-ut-gen/references/workflows/` |

脚本需要消费芯片或 dtype 事实时，读取结构化 JSON，不解析 Markdown 表格，也不要在脚本中复制完整事实表。

### 修改规则

- 修改 skill 正文、reference、脚本或测试后，至少运行相关 skill 的 `test.sh`。
- 修改安装脚本后，运行 `bash .opencode/tests/skills/install/test.sh`。
- 跨 skill 或通用事实变更后，运行 `bash .opencode/tests/skills/run-all.sh`。
- 新增 reference 时，同步更新对应 `references/README.md` 或 skill 的参考索引。
- 新增芯片时，优先更新 `asc-npu-arch` 的结构化事实；只有 API 类型限制变化时，才更新 `asc-api-ut-gen` 的生成约束。

### UT 任务入口

处理 API UT 任务时：

1. 先确认目标 API、API 类型、目标芯片和是否需要构建验证。
2. 读取 `asc-npu-arch` 确认芯片、架构和 dtype 边界。
3. 读取 `asc-api-ut-gen` 的 API guide、目录映射和工作流。
4. 生成或修改 UT 后，报告 token 消耗、总耗时、当前覆盖率；无法获取时说明原因。
5. 不声称未执行的构建、UT 或覆盖率验证已经通过。

