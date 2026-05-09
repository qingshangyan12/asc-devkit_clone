---
name: aclnn-doc-gen
description: |
  生成华为 CANN ACLNN 算子 API 参考文档。当用户提到以下场景时必须使用此技能：
  - 生成 ACLNN 算子文档、生成 aclnn 文档、生成算子文档、写算子API文档
  - 分析 CANN 算子的芯片/产品差异、芯片差异分析、产品支持情况
  - 提取算子信息（参数、约束、公式）、提取算子信息、分析算子参数
  - 对比相关算子接口（V1 vs V2、WeightNz 变体）、接口对比
  - 生成公式分析报告、算子公式生成、推导算子计算公式
  - 用户提供算子目录路径，要求生成该算子的完整 API 参考文档
  - 用户提到 aclnn、ACLNN、CANN 算子相关文档任务
  - 用户给了一个算子代码目录，想了解该算子的功能、参数、约束
  即使只是问"这个算子支持哪些芯片"或"帮我分析这个算子的约束"，也应使用此技能。
  按 5 阶段流水线（extract → formula → compare → generate → review）执行，
  各阶段通过文件传递产出，无需代码驱动。
version: 1.0.0
metadata:
  author: tzh
  category: doc-generation
---

# ACLNN 文档生成技能

## 概述

生成华为 CANN ACLNN 算子 API 参考文档。主动探索代码库，按照 5 阶段流水线生成文档，各阶段通过文件传递产出。

> **路径基准说明**：本文件（SKILL.md）所在目录即为**技能根目录**。下文所有 `references/`、`scripts/`、`tasks/` 路径均以技能根目录为基准，**与用户提供的算子目录和项目根目录无关**。执行任何读文件或运行脚本操作前，必须先确定 SKILL.md 的绝对路径，并以此推算技能根目录的绝对路径，再拼出目标文件的完整路径。例如，若 SKILL.md 位于 `C:\Users\foo\.config\opencode\skills\aclnn-doc-gen\SKILL.md`，则 `references/quantization_introduction.md` 的完整路径为 `C:\Users\foo\.config\opencode\skills\aclnn-doc-gen\references\quantization_introduction.md`。

### 执行步骤

1. **加载领域知识** — 按顺序读取以下 4 个文件，**所有步骤之前执行，每次会话只读一次**（路径相对于技能根目录）：
   - `references/quantization_introduction.md` — 量化概念（pertensor/perchannel/pergroup 等模式、组合量化、静态/动态量化），在算子代码中大量出现，不理解会导致误解参数语义和约束逻辑
   - `references/数据类型.md` — ACL 数据类型与文档简写形式的对照表，参数 dtype 描述必须使用表中简写形式
   - `references/broadcast关系.md` — 广播规则与限制，理解参数间 broadcast 约束的基础
   - `references/sparse_mode参数说明.md` — sparseMode 各取值的语义、mask 矩阵形态及适用场景，涉及 sparseMode 参数时必须参考
2. **检测算子目录与接口** — **若用户已在请求中明确指定了接口名（如 `aclnnXxx`），跳过此步骤**，直接以用户指定的接口名和路径作为目标，进入步骤 4。否则运行脚本，读取输出（路径相对于技能根目录）：
   ```
   node scripts/detect-interfaces.js <用户提供的路径>
   ```
   脚本自动判断路径类型（算子目录或父目录），列出所有算子目录及其包含的接口。
3. **选择接口并确定路径变量** — **若用户已指定接口名，跳过交互选择，但仍必须确定以下变量**。否则：多接口时列出供用户选择（可多选），单接口直接使用
   - **目标接口名**：使用完整 ACLNN 接口名作为输出目录名和最终文档文件名前缀，例如 `aclnnGroupedMatmulV5`；若用户指定的是 `aclnnXxxGetWorkspaceSize`，统一规范化为对应执行接口名 `aclnnXxx`
4. **并行预处理** — **在同一次响应中同时发出以下 5 个 Bash 调用**，脚本间无依赖，并行运行。**每条命令必须携带 `--output` 参数**，脚本默认只输出到 stdout，不携带则不会写入任何文件：

   ```
   node scripts/extract-prototype.js <算子目录路径> <目标接口名> --output <算子目录>/outputs/<目标接口名>/0_prototype.md
   ```
   ```
   node scripts/extract-cross-dir.js <算子目录路径> <项目根目录> --output <算子目录>/outputs/<目标接口名>/0_cross_dir.md
   ```
   ```
   node scripts/extract-chip-evidence.js <算子目录路径> <项目根目录> --output <算子目录>/outputs/<目标接口名>/0_chip_evidence.md
   ```
   ```
   node scripts/extract-error-codes.js <算子目录路径> <项目根目录> --output <算子目录>/outputs/<目标接口名>/0_error_codes.md
   ```
   ```
   node scripts/extract-call-example.js <算子目录路径> <目标接口名> --output <算子目录>/outputs/<目标接口名>/0_call_example.md
   ```

   所有脚本完成后，`0_prototype.md`、`0_cross_dir.md`、`0_chip_evidence.md`、`0_error_codes.md`、`0_call_example.md` 均已落盘，后续各阶段直接读文件，无需重新运行任何脚本。若某脚本退出码非零，立即向输出目录写入对应的 `0_{产出名}_FAILED.md`（如 `0_cross_dir_FAILED.md`），内容包含：失败时间、执行的脚本命令、脚本错误输出摘要。
5. **按阶段执行** — 按以下顺序执行，阶段 2 与阶段 3 并行运行：
   a. 执行**阶段 1（extract）**：读取 `tasks/extract.md` → 探索算子代码 → 写入 `1_extract.md`
   b. 阶段 1 完成后，**在同一次响应中同时发出两个 Agent 调用**，并行执行阶段 2 和阶段 3：
      - **阶段 2（formula）子代理**：读取 `tasks/formula.md` → 读取 `0_cross_dir.md` 和 `1_extract.md` → 探索 `op_kernel/` → 写入 `2_formula.md`
      - **阶段 3（compare）子代理**：读取 `tasks/compare.md` → 读取 `1_extract.md` → 探索关联接口代码 → 写入 `3_compare.md`

       每个子代理 prompt 中必须注入：技能根目录绝对路径、算子目录绝对路径、目标接口名。两个子代理写入不同文件，无冲突。**等两者均完成后再进入阶段 4。**
   c. 执行**阶段 4（generate）**：读取 `tasks/generate.md` → 读取 `1_extract.md`、`2_formula.md`、`3_compare.md` → 写入 `4_generate.md`
   d. 执行**阶段 5（review）**：读取 `tasks/review.md` → 校验修正 → 只写入 `5_review.md`

   各阶段通用规则：
   1. 读取对应 `tasks/*.md` 获取该阶段的完整规则和输出格式（`tasks/` 在技能根目录下，不在算子目录下）
   2. 读取所需 `references/` 文件（按需加载，不预先全部读入；`references/` 在技能根目录下）
   3. 读取上游 `{算子目录}/outputs/{目标接口名}/` 产出文件；若发现依赖的上游产出缺失且存在对应 `_FAILED.md`，立即停止并向用户报告，不继续执行
   4. 写入本阶段产出到 `{算子目录}/outputs/{目标接口名}/`
   5. 若本阶段无法产出有效文件，写入 `{N}_{阶段名}_FAILED.md`，不写正常产出文件（内容格式见对应 `tasks/*.md` 末尾"失败处理"节）
6. **生成最终交付物** — review 完成后，按固定顺序执行：
   a. 读取 `{算子目录}/outputs/{目标接口名}/5_review.md`
   b. 读取 `{算子目录}/outputs/{目标接口名}/0_call_example.md`，按"调用示例拼接规则"追加为文档末尾 `## 调用示例` 章节
   c. 对已拼接调用示例的文档内容执行空格规范化（中文与中文/英文/数字之间不加空格，英文与英文之间保留 1 个空格，英文与数字之间保留 1 个空格）
   d. **仅在拼接调用示例和空格规范化均完成后**，写入最终交付物 `{算子目录}/outputs/{目标接口名}/{目标接口名}_YYYYMMDDHHMM.md`
7. **最终输出** — 最终交付物必须是已完成调用示例拼接和空格规范化后的文件，命名为 `{目标接口名}_YYYYMMDDHHMM.md`（YYYYMMDDHHMM 为执行时的日期和时间，如 `aclnnMegaMoe_202604271434.md`），写入 `{算子目录}/outputs/{目标接口名}/`

**核心原则**：先搜索，后推理；按优先级提取；记录证据来源；通过文件传递阶段产出。

---

## 接口选择

**若用户在请求中已明确指定接口名（如"为 aclnnXxx 生成文档"），直接使用该接口名，不运行 `detect-interfaces.js`，不向用户询问。**

否则，运行 `detect-interfaces.js` 后根据输出结果：

- **多个算子目录**：列出目录供用户选择（可多选），每个选中算子独立执行后续流程；只有一个目录时直接使用
- **多个接口**：列出接口名供用户选择（可多选），每个选中接口独立生成一份文档；只有一个接口时直接使用

---

## 6 阶段流水线

### 阶段间数据流

```
extract ──→ formula
   │
   └──────→ compare
               │
               └──→ generate ──→ review
```

### 跨阶段公共规则

**任务描述读取**（所有阶段）：每个阶段的输出格式完全由对应的 `tasks/*.md` 定义。执行前先读取该阶段的 YAML，输出严格遵循其结构。这是因为各阶段格式要求复杂且不同（如参数表 8 列 vs 3 列、公式块的 LaTeX 规范等），凭记忆复现极易出错。

**并行探索**（阶段 1、2）：算子代码量大、目录多，单线程逐文件读取容易超时或遗漏。启动多个子任务并行探索不同目录或计算路径可以有效避免这个问题。各阶段 task YAML 中有具体的并行分工方式。

**kernel 分支分析**（阶段 1、2、4）：`op_kernel/` 包含实际计算逻辑，其中大量 `if/else` 分支对应不同的数据类型组合、量化模式、分组策略等。不深入分析这些分支，产出的参数约束和公式只会是笼统的并集描述，无法区分不同场景下的实际行为。

**芯片标识替换**（所有阶段）：文档面向外部用户，内部芯片标识对他们无意义且违反保密要求。输出中不得出现代码层面芯片标识（如 `ascend910b`、`ASCEND910_95`），必须替换为技能根目录下 `references/chip_mapping.md` 中的对外产品型号。允许出现在文档中的产品型号见 `chip_mapping.md` 的"文档输出规范"。

**产品支持情况判断原则**（所有阶段）：产品支持由 3 条规则（AddConfig 注册、NpuArch/SocVersion 运行时分支、编译期架构宏）共同决定，任一规则命中即代表支持，三者互相补充不互相否定。不要以"AddConfig 未注册"为由否定 NpuArch/SocVersion/编译宏的证据——这些运行时代码分支说明代码确实为该架构提供了处理路径。`aclnn_*.cpp` 中的芯片代码是主要查找位置，但共享工具代码（通过 `#include` 引入的 `*_util.cpp`、`cube_util.cpp` 等）中的芯片分支同样有效——它们在运行时会被执行。

**跨目录引用追踪**（阶段 1、2、4）：CANN 算子代码通过 `#include "..."` 引用外部共享模块（共享 kernel 模板、公共宏、基类、tiling 策略等），还通过 `OP_TYPE_REGISTER(OpName)` 调用其他底层算子。这些外部文件中可能包含关键的校验逻辑、计算实现和芯片差异分支——不追踪会导致分析结果严重缺失。每个阶段开始代码分析前，先读取 `{算子目录}/outputs/{目标接口名}/0_cross_dir.md`（预生成报告），追踪到的外部文件与本算子目录内文件同等对待。

**代码分析清单**（阶段 1、2、3、4）：每个阶段报告末尾附 `## 代码分析清单` 章节，记录所有分析过的文件及分析原因（包括跨目录追踪到的外部文件，以加粗引用链标注来源）。这是阶段间质量保证的关键——后续阶段和 review 可以据此验证分析是否完整，避免遗漏。

**约束说明 HTML 缩进规则**（阶段 5、6）：约束说明使用 `<details>`/`<summary>` 折叠，缩进不一致会导致渲染错乱。第 2 层（产品区分）`<details>` 顶格，第 3 层（场景区分）缩进两格；`</summary>` 后紧跟空行，`</details>` 前保留空行。

**跳过无关文件**：扫描算子代码时，所有 `.md` 文件和 `tests/` 目录不读取。

**失败标志文件**（所有阶段）：当某阶段无法完成正常产出时（必需上游输入缺失、算子代码目录无法访问、脚本执行失败等），必须写入失败标志文件，**不写**不完整的正常产出。
- **命名**：`{N}_{阶段名}_FAILED.md`（如 `1_extract_FAILED.md`）；预处理脚本失败对应写 `0_{产出名}_FAILED.md`（如 `0_cross_dir_FAILED.md`）
- **位置**：`{算子目录}/outputs/{目标接口名}/`（与正常产出相同目录）
- **下游检查**：后续阶段启动前，先确认依赖的上游产出文件是否存在；若正常产出缺失且对应 `_FAILED.md` 存在，立即停止，向用户说明哪个上游阶段失败（附失败文件路径），不继续执行后续阶段
- **内容格式**：见各阶段 `tasks/*.md` 末尾"失败处理"节

### 各阶段任务

#### 阶段 1：extract（算子信息提取）

从算子代码中确定产品支持情况、分析芯片差异，并提取全面的结构化信息，输出包含六大章节的报告：算子名称、产品支持情况、功能说明与接口语义、参数列表（逐参数提取 dtype/format/shape/nonContiguous/emptyTensor/valueRange/broadcast/usageNotes 共 8 个维度，每个参数独立一节）、返回值错误码、约束信息（分跨产品公共约束和按产品/场景区分的约束两层）。参数列表和约束信息是报告核心。

- 先读取技能根目录下的 `tasks/extract.md`，按其中定义的执行流程、逐参数格式和提取优先级生成报告
- **读取**：`{算子目录}/outputs/{目标接口名}/0_chip_evidence.md`、`{算子目录}/outputs/{目标接口名}/0_error_codes.md`、`{算子目录}/outputs/{目标接口名}/0_cross_dir.md`
- **探索**：启动多个子任务并行完整读取 `op_api/`、`op_host/`、`op_tiling/`、`op_kernel/` 及追踪到的外部文件的所有代码，不可遗漏文件或跳过实现
- **输出**：`{算子目录}/outputs/{目标接口名}/1_extract.md`

---

#### 阶段 2：formula（公式生成）

根据算子计算逻辑生成计算公式和公式描述。输出固定结构：引导语 → `$$...$$` 公式块 → 逐公式 `说明：` → 统一变量映射 → 局部变量解释 → 证据摘要。简单算子一条公式，复合算子按"中间结果→后续结果→最终输出"拆为步骤公式。

- 先读取技能根目录下的 `tasks/formula.md`，按其中定义的输出结构和写作风格约束生成报告
- **读取**：`{算子目录}/outputs/{目标接口名}/1_extract.md`
- **探索**：启动多个子任务并行探索 `op_kernel/`、`op_host/aclnn_*.cpp`、`op_api/` 核心调用链及不同计算路径，包括追踪到的外部文件
- **输出**：`{算子目录}/outputs/{目标接口名}/2_formula.md`

---

#### 阶段 3：compare（接口对比）

对比当前接口与关联接口的差异，输出固定结构：当前接口 → 版本迭代对比（新增/删除参数、类型变化、新增能力）→ 格式变体对比（format/dtype/参数差异）→ 相近接口对比 → 面向文档的功能差异总结（1~3条）。无关联接口时各节写"无"。

- 先读取技能根目录下的 `tasks/compare.md`，按其中定义的输出格式生成报告
- **读取**：`{算子目录}/outputs/{目标接口名}/1_extract.md`
- **探索**：`op_api/`、`op_host/`、`references/` 中的关联接口代码
- **输出**：`{算子目录}/outputs/{目标接口名}/3_compare.md`

---

#### 阶段 4：generate（文档生成）

汇总各阶段产出生成完整 API 参考文档。文档章节固定为：`# aclnn{Name}` → `## 产品支持情况` → `## 功能说明` → `## 函数原型` → `## aclnn{Name}GetWorkspaceSize`（含 8 列 HTML 参数说明表 + HTML 返回值表）→ `## aclnn{Name}`（含 3 列 HTML 参数说明表）→ `## 约束说明`（4 层层级结构，使用 `<details>`/`<summary>` 折叠）。产品名加 `<term>` 标记，必须包含 `【需用户确认】`/`【需用户补齐】` 审阅标记。

- 先读取技能根目录下的 `tasks/generate.md`，按其中定义的章节顺序、HTML 表格规范和约束说明层级生成文档
- **读取**：`1_extract.md`、`2_formula.md`、`3_compare.md`
- **读取**：`{算子目录}/outputs/{目标接口名}/0_prototype.md`（函数原型，预处理阶段已生成）
- **输出**：`{算子目录}/outputs/{目标接口名}/4_generate.md`
- 公式以 `2_formula.md` 为唯一权威来源，不重新推导
- **不生成** `## 调用示例` 章节——该章节由脚本输出拼接而来（见下方"调用示例拼接规则"）
- 参数说明表写所有产品并集，芯片差异放约束说明章节
- 当阶段产出信息不足时，可主动探索 `op_api/`、`op_host/`、`op_kernel/`、`op_tiling/` 补充分析

---

#### 阶段 5：review（一致性校验）

对 `4_generate.md` 进行一致性校验和修正，**输出修正后的完整已审校底稿 `5_review.md`**。不是校验报告、不是 diff、不是对比表——是与 `4_generate.md` 结构相同但修正了不一致处的完整文档底稿。只修正不新增，保留原章节顺序、标题层级、审阅标记。

- 先读取技能根目录下的 `tasks/review.md`，按其中定义的 7 条校验规则逐条检查并修正
- **读取**：`1_extract.md`、`2_formula.md`、`4_generate.md`、技能根目录下的 `references/common_terminology.yml`、`references/terminology_dict.yml`
- **输出**：`{算子目录}/outputs/{目标接口名}/5_review.md`；最终交付物 `{目标接口名}_YYYYMMDDHHMM.md` 由主流程在 review 完成后基于 `5_review.md` 派生，完成调用示例拼接和空格规范化后再写入

---

## 脚本预生成

### 接口检测脚本

脚本自动判断路径类型，列出算子目录及接口：

```bash
node scripts/detect-interfaces.js <路径>
```

### 函数原型提取脚本

脚本从算子头文件中提取 `aclnnStatus` 函数签名并格式化为文档章节：

```bash
node scripts/extract-prototype.js <算子目录路径> [目标接口名]
```

### 调用示例提取脚本

脚本从 `examples/` 目录中提取代表性调用示例代码：

```bash
node scripts/extract-call-example.js <算子目录路径> [目标接口名]
```

### 芯片证据提取脚本

脚本自动执行跨目录引用追踪和 3 条规则的 grep 搜索，输出结构化的芯片标识匹配结果：

```bash
node scripts/extract-chip-evidence.js <算子目录路径> [项目根目录]
```

脚本输出包含：
- 规则1（AddConfig 注册声明）的所有匹配：芯片标识、文件路径、行号、代码片段
- 规则2（NpuArch/SocVersion 分支）的所有匹配
- 规则3（编译期架构宏）的所有匹配
- **命中标识汇总**：去重后的全部芯片/架构标识列表，供查映射表使用
- 未解析项：脚本无法自动追踪的 #include 和 OP_TYPE_REGISTER（需手动补充）
- 搜索范围：算子目录文件数 + 追踪到的外部文件列表

### 错误码提取脚本

脚本扫描算子代码（含跨目录追踪文件），输出实际出现的错误码列表：

```bash
node scripts/extract-error-codes.js <算子目录路径> [项目根目录]
```

脚本输出包含：
- **出现的错误码**：实际命中的错误码符号和数值码列表（4 个文档允许码的子集）
- 搜索范围：算子目录文件数 + 跨目录追踪外部文件数

模型在 extract 阶段读取此列表后，自行探索算子代码推导每个错误码的触发原因描述。

### 空格规范化脚本

脚本对已完成调用示例拼接的文档内容进行空格规范化，保护代码块、行内代码、HTML 标签、链接/URL、公式块以及数据类型等代码式 token。空格规范化必须在最终交付物写入之前完成，不得在向用户报告最终产物之后再执行：

```bash
node scripts/normalize-doc-spacing.js <最终文档路径> [--output <file>] [--in-place]
```

规范化规则：中文与中文之间不加空格，中文与英文之间不加空格，中文与数字之间不加空格，英文与英文之间保留 1 个空格，英文与数字之间保留 1 个空格。

## 调用示例拼接规则

`## 调用示例` 章节不由模型生成，`5_review.md` 中不得包含该章节。它由脚本输出先拼接到底稿末尾，再执行空格规范化，最后整体写入最终交付物 `{目标接口名}_YYYYMMDDHHMM.md`。这是因为调用示例应该是真实可编译的代码，直接从 `examples/` 提取比模型生成更可靠。

拼接步骤：

1. review 阶段完成后，读取 `{算子目录}/outputs/{目标接口名}/5_review.md` 作为最终交付物底稿
2. 读取 `{算子目录}/outputs/{目标接口名}/0_call_example.md` 获取示例代码
3. 如果底稿中已包含 `## 调用示例` 章节，先删除该章节及其内容
4. 在底稿末尾（约束说明章节之后）拼接以下内容：

```markdown
## 调用示例

示例代码如下，仅供参考，具体编译和执行过程请参考[编译与运行样例](../../../docs/zh/context/编译与运行样例.md)。

{脚本输出内容}
```

标题和引导句必须手动补上——脚本只输出代码块，不含这两部分。禁止在 `## 调用示例` 标题后直接放代码块。

---

## 阶段产出目录

所有产出（含最终文档）写入 `{算子目录}/outputs/{目标接口名}/`，其中 `{目标接口名}` 是完整 ACLNN 接口名，例如 `aclnnGroupedMatmulV5`：

```
{算子目录}/outputs/{目标接口名}/
├── 0_cross_dir.md              # 跨目录引用分析报告（预生成，落盘）
├── 0_cross_dir_FAILED.md       # ↑ 若 extract-cross-dir.js 失败时写入（与正常产出互斥）
├── 1_extract.md                # 产品支持、芯片差异、算子信息提取（参数、约束、错误码）
├── 1_extract_FAILED.md         # ↑ 若阶段1失败时写入（与正常产出互斥）
├── 2_formula.md                # 计算公式
├── 2_formula_FAILED.md         # ↑ 若阶段2失败时写入（与正常产出互斥）
├── 3_compare.md                # 接口对比
├── 3_compare_FAILED.md         # ↑ 若阶段3失败时写入（与正常产出互斥）
├── 4_generate.md               # 文档草稿
├── 4_generate_FAILED.md        # ↑ 若阶段4失败时写入（与正常产出互斥）
├── 5_review.md                 # 已审校底稿（不含调用示例）
├── 5_review_FAILED.md          # ↑ 若阶段5失败时写入（与正常产出互斥）
└── {目标接口名}_YYYYMMDDHHMM.md   # 最终交付物（已完成调用示例拼接和空格规范化）
```
