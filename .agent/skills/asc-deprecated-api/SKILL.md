---
name: asc-deprecated-api
description: AscendC API 废弃与迁移技能。当用户要求废弃、弃用、迁移、替换或删除 AscendC 基础 API、高阶 API、C API、SIMT API、工具类 API，整理 `_deprecated.md` 文档或废弃接口目录，补充 `[[deprecated]]` 编译告警，检查替代接口、兼容性、目录链接、测试与删除门禁时使用。
---

# AscendC Deprecated API

把 API 废弃视为兼容性迁移，而不是文件删除。先确认生命周期阶段，再同步处理声明、实现、文档、导航、样例、测试和发布信息。

## Required Inputs

开始修改前解析以下信息：

- `DEVKIT_PATH`：目标 asc-devkit 仓根目录。
- `TARGET_API`：待废弃的符号及全部相关重载。
- `API_FAMILY`：基础 API、高阶 API、C API、SIMT API、AI CPU API 或工具类 API。
- `DEPRECATION_STAGE`：仅整理已有废弃文档、首次标记废弃，或最终删除。
- `REPLACEMENT`：替代接口或明确的迁移组合；若没有替代方案，记录原因和继续使用的风险。
- `SUPPORT_POLICY`：废弃起始版本、预计删除版本和兼容范围；无法从需求或仓库获得时标记为待确认。

从当前 workspace 向上查找包含 `CMakeLists.txt`、`include/`、`impl/` 和 `docs/` 的仓根。不要假定私人路径、CANN 安装位置或目标 SoC。

## Lifecycle Decision

先把任务归入一个阶段，阶段不明确时采用影响最小的“标记废弃”阶段，不执行删除。

| 阶段 | 允许的动作 | 禁止的动作 |
|---|---|---|
| 文档整理 | 移动废弃文档、修复导航、补充替代说明 | 改变声明、实现或运行行为 |
| 标记废弃 | 保留接口，实现编译期告警，补齐替代方案、文档和测试 | 删除符号、缩减既有产品支持 |
| 最终删除 | 在明确版本策略和删除授权下移除声明、实现、测试及打包入口 | 仅凭“已废弃”字样直接删除 |

如果请求同时包含多个阶段，拆成可独立评审的变更。优先先完成标记和迁移，再在后续版本删除。

## Workflow

### 1. Build An Evidence Inventory

使用 `rg` 查找符号、文件名和替代接口，覆盖：

- `include/` 中的公开声明、所有重载、宏和别名。
- `impl/` 中各架构实现及条件编译分支。
- `docs/zh/api/README.md`、API 分类页、单接口文档和 `docs/zh/api/appendix/deprecated_interface.md`。
- `examples/`、`tests/`、构建脚本、打包清单和生成器。
- 英文文档或其他语言镜像；只修改任务要求覆盖的语言，但报告未同步项。

记录 `symbol -> declaration -> implementation -> docs -> tests/examples -> replacement`。同名重载必须逐个分类，不能用一次命中代表全部覆盖。

### 2. Validate The Replacement Contract

核对替代方案是否覆盖原接口的：

- 数据类型、tensor position、同步语义、流水类型和副作用。
- 产品与架构支持范围。
- 参数默认值、舍入/饱和模式、原子状态和地址对齐约束。
- 性能特征；组合替代时给出完整调用顺序。

若替代方案会改变语义或缩小支持范围，不要写成无条件“请使用 X 替代”。明确差异、迁移前提和无等价替代的场景。

涉及芯片、`NpuArch`、`__NPU_ARCH__` 或 dtype 支持时读取 [`../asc-npu-arch/SKILL.md`](../asc-npu-arch/SKILL.md)。涉及新增 kernel 调用或修改实现时遵守 [`../asc-ar-dev/SKILL.md`](../asc-ar-dev/SKILL.md) 的 API 文档核验门禁。

### 3. Preserve Source Compatibility

首次标记废弃时：

1. 保留原公开签名和实现。
2. 在用户可见声明处添加仓库现有形式的 `[[deprecated("NOTICE: ... is deprecated. Please use ... instead.")]]`。
3. 让告警准确指向可用替代接口；组合替代时简述组合并让文档承载细节。
4. 检查声明和定义是否重复携带属性，避免宏展开、条件编译或不同架构产生不一致声明。
5. 保留既有 UT，必要时新增“旧接口仍可编译并保持行为”的兼容测试。

不要为了消除内部告警而静默改写对外行为。先迁移仓内调用者；若构建启用 `-Werror`，使用项目认可的局部策略验证废弃调用，不要全局关闭告警。

### 4. Move Documentation Into The Deprecated Surface

遵循当前 API 家族的既有目录。对于已采用集中废弃目录的 C API：

1. 将独立文档命名为 `<api>_deprecated.md` 并放入 `docs/zh/api/SIMD-API/c_api/deprecated_interface/`。
2. 标题写成 `# <api>（废弃）`。
3. 保留原产品支持、函数原型、参数、流水类型和约束，不因废弃擅自改成“不支持”。
4. 在功能说明开头写清废弃状态、替代接口和迁移方式。
5. 从正常功能分类的目录和列表中移除旧入口，避免同一接口在“正常”和“废弃”分类重复展示。
6. 将入口加入 `deprecated_interface/deprecated_interface.md`、API 总目录、API 列表和 `appendix/deprecated_interface.md`。
7. 更新所有相对链接、锚点和引用旧路径的页面。

如果接口原本只是另一篇文档中的重载，先拆成独立废弃文档，再从原文档删除该重载。详细迁移映射和文档模板见 [`references/c-api-documentation-pattern.md`](references/c-api-documentation-pattern.md)。

仅做文档整理时，不补写代码中不存在的 `[[deprecated]]`，也不声称 API 已从二进制或源码删除。

### 5. Apply The Removal Gate

只有同时满足以下条件才最终删除接口：

- 需求明确授权删除，并给出适用版本或兼容窗口。
- 替代方案已发布且覆盖目标产品范围，或已接受无替代方案的影响。
- 仓内调用者、样例和生成代码已经迁移。
- ABI/API 兼容性影响已评审；导出符号、打包清单和工具生成物已纳入范围。
- 删除后的文档不再提供可调用原型，但保留必要的迁移或发布记录。
- 相关构建、UT、文档链接检查和回归验证可执行。

任一条件不满足时，停留在废弃阶段并报告缺口。

### 6. Verify

按影响面从窄到宽验证：

- `git diff --check` 无空白错误。
- 文档内无旧路径和重复导航，新增相对链接均存在。
- 公共声明产生预期废弃告警，替代接口不产生该告警。
- 旧接口兼容测试和替代接口功能测试通过。
- 删除阶段确认目标符号、实现、测试调用和打包引用均已处理。
- 运行仓库已有的文档规范检查；需要专项 Markdown 规范检查时使用 `asc-doc-checker`。

不要声称未执行的编译、UT、覆盖率或硬件验证已经通过。

## Output Contract

交付时按以下顺序报告：

```md
## 废弃阶段
## 兼容策略
## 替代方案
## 改动清单
## 验证结果
## 遗留风险
```

明确区分“文档已归档”“源码已标记废弃”和“接口已删除”，并列出仍需确认的版本、架构或替代语义。

