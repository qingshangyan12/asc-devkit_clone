---
name: user_require_analysis
description: 理解用户的自然语言工程诉求，将其转化为 edit-yaml 技能可直接调用的标准化命令。作为用户意图与 YAML 配置修改之间的翻译层，提供需求分析、操作规划等全流程支持。
---

# 用户需求分析技能

## 一、技能概述

本技能（user_require_analysis）是华为 CANN CI/CD 体系下的**用户需求理解与转化层**，负责将用户的**自然语言工程诉求**转化为 edit-yaml 技能可以直接理解和执行的**标准化操作命令**。

本技能的核心价值在于：
- **降低使用门槛**：用户无需记忆复杂的 YAML 路径和脚本参数，使用自然语言即可表达需求
- **精确需求理解**：通过智能分析提取关键信息（文件路径、目标节点、操作类型、参数值）
- **标准化转化**：将多样化的自然语言表述统一为 edit-yaml 技能的标准命令格式
- **批量处理能力**：支持解析多个诉求，统一展示执行计划，一次性确认后批量执行

## 二、工作流程

### 2.1 标准操作流程

```
用户自然语言诉求
    ↓
【步骤1】需求分析：提取所有诉求的关键信息（文件、节点、操作、参数）
    ↓
【步骤2】操作规划：为每个诉求选择合适的 edit-yaml 工具及参数
    ↓
【步骤3】统一展示：展示完整的执行计划（包含所有步骤）
    ↓
【步骤4】用户确认：用户一次性确认所有操作
    ↓
【步骤5】批量执行：按计划依次调用 edit-yaml 技能执行
    ↓
【步骤6】结果反馈：向用户报告所有执行结果
```

### 2.2 流程详细说明

#### **步骤1：需求分析**
从用户的自然语言诉求中提取以下关键信息：

| 信息类型 | 提取内容 | 示例 |
|---------|---------|------|
| **目标文件** | YAML 文件路径或文件名 | `ci_config/rule.yaml` 或 `rule.yaml` |
| **操作类型** | 查找/修改/新增/删除/移动 | "修改 ut_check 为 false" → 修改操作 |
| **目标节点** | 节点路径或节点特征 | `CB.tasks.pkg.llt1` 或 "version 为 cann 的任务" |
| **配置项** | 要操作的键名 | `ut_check`、`version`、`enabled` |
| **目标值** | 要设置的值 | `false`、`cann_new`、`true` |
| **条件过滤** | 筛选条件 | "version 为 cann"、"status 为 active" |

#### **步骤2：操作规划**
根据分析结果，选择合适的 edit-yaml 工具：

| 自然语言诉求 | 对应的 edit-yaml 工具 |
|-------------|---------------------|
| "查找 xxx 文件中 yyy 的任务" | `find_yaml_node.py` |
| "修改 aaa 为 bbb"（节点内已存在的键） | `modify_node_key_value.py` |
| "新增 aaa 配置项"（简单键值） | `add_node_key_value.py` |
| "添加新节点 yyy"（嵌套结构） | `add_node.py` |
| "把 xxx 移动到 yyy" | `move_yaml_node.py` |
| "重命名节点 xxx" | `modify_node_name.py` |

#### **步骤3：统一展示执行计划**
将所有解析后的操作统一展示给用户：

```
【执行计划总览】

📋 原始诉求：
  诉求1：{用户的第一个自然语言描述}
  诉求2：{用户的第二个自然语言描述}
  ...

🔍 解析结果（共 N 个操作）：

┌─────────────────────────────────────────
│ 操作 1/3：修改配置项
├─────────────────────────────────────────
│ • 操作文件：ci_config/rule.yaml
│ • 目标节点：CB.tasks.pkg 下 version 为 cann 的任务
│ • 操作类型：修改键值
│ • 配置项：ut_check
│ • 目标值：false
└─────────────────────────────────────────

┌─────────────────────────────────────────
│ 操作 2/3：新增配置项
├─────────────────────────────────────────
│ • 操作文件：ci_config/rule.yaml
│ • 目标节点：CB.tasks.compile
│ • 操作类型：新增节点
│ • 节点名称：new_task
│ • 节点内容：{version: cann, enabled: true}
└─────────────────────────────────────────

┌─────────────────────────────────────────
│ 操作 3/3：移动节点
├─────────────────────────────────────────
│ • 操作文件：ci_config/rule.yaml
│ • 移动节点：CB.tasks.llt1
│ • 目标路径：CB.tasks.pkg
│ • 操作类型：移动节点
└─────────────────────────────────────────

🛠️ 执行命令预览：
  命令1：查找ci_config/rule.yaml下CB.tasks.pkg中version为cann的任务，并修改ut_check为false
  命令2：在ci_config/rule.yaml的CB.tasks.compile节点下添加新任务new_task，内容为version:cann,enabled:true
  命令3：移动ci_config/rule.yaml中的CB.tasks.llt1节点到CB.tasks.pkg下

⚠️ 影响范围：
  • 预计影响节点数：约 5-10 个节点
  • 自动创建备份文件：是
  • 操作类型：批量修改

❓ 请确认是否执行以上所有操作？
  [确认] - 执行所有操作
  [取消] - 取消操作
```

#### **步骤4：用户确认**
用户一次性确认所有操作：
- **确认**：按计划依次执行所有操作
- **取消**：放弃所有操作

#### **步骤5：批量执行**
用户确认后，按计划依次通过 Skill 工具调用 edit-yaml 技能执行：

```python
# 依次执行每个操作
Skill(skill="edit-yaml", args="{命令1}")
Skill(skill="edit-yaml", args="{命令2}")
Skill(skill="edit-yaml", args="{命令3}")
```

#### **步骤6：结果反馈**
向用户报告所有执行结果：

```
【执行结果总览】

✅ 执行完成

📊 执行摘要：
  • 总操作数：3
  • 成功：3
  • 失败：0

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
操作 1/3：修改配置项
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ 成功
  • 修改节点数：5
  • 修改内容：ut_check: true → false
  • 备份文件：ci_config/rule.yaml.backup_20250131_123456

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
操作 2/3：新增配置项
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ 成功
  • 新增节点：CB.tasks.compile.new_task
  • 节点内容：{version: cann, enabled: true}

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
操作 3/3：移动节点
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ 成功
  • 移动节点：CB.tasks.llt1 → CB.tasks.pkg.llt1
  • 备份文件：ci_config/rule.yaml.backup_20250131_123500

💡 后续操作建议：
  [1] 查看修改后的文件内容
  [2] 执行 YAML 语法检查
  [3] 提交代码到仓库
```

## 三、自然语言诉求识别模式

### 3.1 文件路径识别

| 用户表述 | 提取的文件路径 | 处理方式 |
|---------|--------------|---------|
| "rule.yaml 文件" | `ci_config/rule.yaml` | 自动添加 `ci_config/` 前缀 |
| "在 ci_config/rule.yaml 中" | `ci_config/rule.yaml` | 直接使用 |
| "配置文件 rule.yaml" | `ci_config/rule.yaml` | 自动添加路径前缀 |
| "所有 yaml 文件" | `ci_config/**/*.yaml` | 执行搜索操作 |

### 3.2 节点路径识别

| 用户表述 | 提取的节点路径 | 处理方式 |
|---------|--------------|---------|
| "CB.tasks.pkg 下的任务" | `CB.tasks.pkg` | 作为根路径 |
| "llt1 这个任务" | 需要查找 | 使用 `find_yaml_node.py` 搜索 |
| "version 为 cann 的任务" | 查找条件 | 使用 `-k version -v cann` 查找 |
| "pkg 下的所有编译任务" | `CB.tasks.pkg` + 条件 | 组合路径和条件 |

### 3.3 操作类型识别

#### **修改操作**

| 用户表述 | 操作类型 | 工具选择 |
|---------|---------|---------|
| "修改 ut_check 为 false" | 修改键值 | `modify_node_key_value.py` |
| "把 version 改成 cann_new" | 修改键值 | `modify_node_key_value.py` |
| "将 enabled 设置为 true" | 修改键值 | `modify_node_key_value.py` |
| "更新 retry_count 为 5" | 修改键值 | `modify_node_key_value.py` |

#### **新增操作**

| 用户表述 | 操作类型 | 工具选择 |
|---------|---------|---------|
| "新增 enabled 配置项" | 新增键值 | `add_node_key_value.py` |
| "添加 new_field 字段" | 新增键值 | `add_node_key_value.py` |
| "增加一个新的任务节点" | 新增节点 | `add_node.py` |
| "创建新的子任务" | 新增节点 | `add_node.py` |

#### **查找操作**

| 用户表述 | 操作类型 | 工具选择 |
|---------|---------|---------|
| "查找所有 version 为 cann 的任务" | 查找节点 | `find_yaml_node.py` |
| "搜索 ut_check 为 true 的配置" | 查找节点 | `find_yaml_node.py` |
| "找出 pkg 目录下的所有任务" | 查找节点 | `find_yaml_node.py` |

#### **移动操作**

| 用户表述 | 操作类型 | 工具选择 |
|---------|---------|---------|
| "把 llt1 移动到 pkg 下" | 移动节点 | `move_yaml_node.py` |
| "将 compile 下的任务迁移到 pkg" | 移动节点 | `move_yaml_node.py` |
| "重组任务结构到新路径" | 移动节点 | `move_yaml_node.py` |

#### **重命名操作**

| 用户表述 | 操作类型 | 工具选择 |
|---------|---------|---------|
| "重命名 llt1 为 llt1_new" | 重命名节点 | `modify_node_name.py` |
| "把任务名改为新名称" | 重命名节点 | `modify_node_name.py` |
| "修改节点名称后缀" | 重命名节点 | `modify_node_name.py` |

### 3.4 参数值识别

| 用户表述 | 提取的值 | 值类型 |
|---------|---------|--------|
| "设置为 true"、"启用"、"开启" | `true` | 布尔值 |
| "设置为 false"、"禁用"、"关闭" | `false` | 布尔值 |
| "设置为 cann" | `cann` | 字符串 |
| "数量改为 5"、"重试3次" | `5`、`3` | 整数 |
| "比例设为 0.5" | `0.5` | 浮点数 |

## 四、标准转化规则

### 4.1 修改操作转化

**用户诉求示例：**
> "修改 rule.yaml 中 CB.tasks.pkg 下 version 为 cann 的任务，将 ut_check 设置为 false"

**分析过程：**
1. 文件路径：`ci_config/rule.yaml`
2. 查找条件：`-r CB.tasks.pkg -k version -v cann`
3. 操作类型：修改键值
4. 修改目标：`-k ut_check -v false -t bool`

**转化后的命令：**
```bash
# 步骤1：查找节点
python .claude/skills/edit-yaml/scripts/find_yaml_node.py -f ci_config/rule.yaml -r CB.tasks.pkg -k version -v cann > nodes.txt

# 步骤2：修改键值
python .claude/skills/edit-yaml/scripts/modify_node_key_value.py -f ci_config/rule.yaml --nodes-file nodes.txt -k ut_check -v false
```

**传递给 edit-yaml 的参数：**
```
查找ci_config/rule.yaml下CB.tasks.pkg中version为cann的任务，并修改ut_check为false
```

### 4.2 新增操作转化

**用户诉求示例：**
> "在 rule.yaml 中 CB.tasks.compile 下添加一个新任务 new_task，version 为 cann，enabled 为 true"

**分析过程：**
1. 文件路径：`ci_config/rule.yaml`
2. 父节点：`CB.tasks.compile`
3. 新节点名：`new_task`
4. 节点内容：`{version: cann, enabled: true}`

**转化后的命令：**
```bash
python .claude/skills/edit-yaml/scripts/add_node.py -f ci_config/rule.yaml \
  -p CB.tasks.compile -n "new_task: {version: cann, enabled: true}"
```

**传递给 edit-yaml 的参数：**
```
在ci_config/rule.yaml的CB.tasks.compile节点下添加新任务new_task，内容为version:cann, enabled:true
```

### 4.3 查找操作转化

**用户诉求示例：**
> "查找 rule.yaml 中所有 version 为 cann_eco_debug 的任务"

**分析过程：**
1. 文件路径：`ci_config/rule.yaml`
2. 查找条件：`-k version -v cann_eco_debug`
3. 默认根节点：`CB`

**转化后的命令：**
```bash
python .claude/skills/edit-yaml/scripts/find_yaml_node.py -f ci_config/rule.yaml -k version -v cann_eco_debug
```

**传递给 edit-yaml 的参数：**
```
查找ci_config/rule.yaml下CB中version为cann_eco_debug的任务
```

### 4.4 移动操作转化

**用户诉求示例：**
> "把 rule.yaml 中 CB.tasks.compile 下的 llt1 和 llt2 任务移动到 CB.tasks.pkg 下"

**分析过程：**
1. 文件路径：`ci_config/rule.yaml`
2. 移动节点：`CB.tasks.compile.llt1`、`CB.tasks.compile.llt2`
3. 目标路径：`CB.tasks.pkg`

**转化后的命令：**
```bash
python .claude/skills/edit-yaml/scripts/move_yaml_node.py -f ci_config/rule.yaml \
  -n CB.tasks.compile.llt1 CB.tasks.compile.llt2 -t CB.tasks.pkg
```

**传递给 edit-yaml 的参数：**
```
移动ci_config/rule.yaml中的CB.tasks.compile.llt1和CB.tasks.compile.llt2节点到CB.tasks.pkg下
```

### 4.5 重命名操作转化

**用户诉求示例：**
> "将 rule.yaml 中 CB.tasks.pkg 下的 llt1 重命名为 llt1_test"

**分析过程：**
1. 文件路径：`ci_config/rule.yaml`
2. 原节点：`CB.tasks.pkg.llt1`
3. 新节点名：`CB.tasks.pkg.llt1_test`

**转化后的命令：**
```bash
python .claude/skills/edit-yaml/scripts/modify_node_name.py -f ci_config/rule.yaml \
  -m CB.tasks.pkg.llt1=CB.tasks.pkg.llt1_test
```

**传递给 edit-yaml 的参数：**
```
重命名ci_config/rule.yaml中的CB.tasks.pkg.llt1节点为CB.tasks.pkg.llt1_test
```

## 五、复杂场景处理

### 5.1 多步骤操作

**用户诉求示例：**
> "找出 rule.yaml 中所有 version 为 cann 的任务，然后批量修改它们的 ut_check 为 false，再新增一个 enabled 字段设置为 true"

**处理方式：**
1. 拆分为多个独立操作
2. 统一展示所有操作的计划
3. 用户一次性确认后依次执行

**展示格式：**
```
【执行计划总览】

📋 原始诉求：
  找出 rule.yaml 中所有 version 为 cann 的任务，然后批量修改它们的 ut_check 为 false，再新增一个 enabled 字段设置为 true

🔍 解析结果（共 3 个操作）：

┌─────────────────────────────────────────
│ 操作 1/3：查找目标节点
├─────────────────────────────────────────
│ • 操作文件：ci_config/rule.yaml
│ • 查找条件：CB 下 version 为 cann 的任务
│ • 操作类型：查找节点
└─────────────────────────────────────────

┌─────────────────────────────────────────
│ 操作 2/3：批量修改配置
├─────────────────────────────────────────
│ • 操作文件：ci_config/rule.yaml
│ • 目标节点：操作1查找到的所有节点
│ • 操作类型：修改键值
│ • 配置项：ut_check
│ • 目标值：false
└─────────────────────────────────────────

┌─────────────────────────────────────────
│ 操作 3/3：批量新增配置
├─────────────────────────────────────────
│ • 操作文件：ci_config/rule.yaml
│ • 目标节点：操作1查找到的所有节点
│ • 操作类型：新增键值
│ • 配置项：enabled
│ • 目标值：true
└─────────────────────────────────────────

❓ 请确认是否执行以上所有操作？
```

### 5.2 条件不明确的诉求

**用户诉求示例：**
> "修改 pkg 下的任务"

**处理方式：**
当诉求信息不足时，通过 AskUserQuestion 工具询问用户：

```python
AskUserQuestion(questions=[
    {
        "question": "请问要修改 pkg 下的哪些任务？",
        "header": "任务筛选",
        "options": [
            {"label": "所有任务", "description": "修改 CB.tasks.pkg 下的所有任务节点"},
            {"label": "version 为 cann 的任务", "description": "仅修改 version 字段为 cann 的任务"},
            {"label": "特定任务名称", "description": "指定具体的任务名称（如 llt1、llt2）"}
        ],
        "multiSelect": false
    },
    {
        "question": "要修改什么配置项？",
        "header": "配置项",
        "options": [
            {"label": "ut_check", "description": "单元测试检查开关"},
            {"label": "enabled", "description": "任务启用状态"},
            {"label": "自定义", "description": "手动输入配置项名称和值"}
        ],
        "multiSelect": false
    }
])
```

### 5.3 文件路径不明确

**用户诉求示例：**
> "修改配置文件中的 ut_check"

**处理方式：**
1. 先搜索可能的目标文件
2. 向用户展示候选文件列表
3. 等待用户选择确认

```bash
# 搜索 ci_config 下的 yaml 文件
find ci_config -name "*.yaml" -type f
```

```
【候选文件列表】

找到以下 YAML 配置文件：
  [1] ci_config/rule.yaml (主要配置文件)
  [2] ci_config/custom/rule_custom.yaml (自定义配置)
  [3] ci_config/test/rule_test.yaml (测试配置)

❓ 请选择要修改的文件？
```

### 5.4 节点路径不存在

**处理方式：**
当用户指定的节点路径不存在时：

1. 向用户报告查找失败
2. 提供可能的相似路径建议
3. 建议使用查找功能定位正确路径

```
【查找失败】

❌ 在 CB.tasks.pkg 下未找到名为 llt99 的节点

💡 建议：
  [1] 查看所有可用节点：使用 "查找 CB.tasks.pkg 下的所有任务"
  [2] 搜索相似节点：使用 "查找名称包含 llt 的任务"
  [3] 确认节点路径是否正确
```

## 六、与 edit-yaml 技能的集成

### 6.1 调用方式

本技能分析完成后，**必须**通过 Skill 工具调用 edit-yaml 技能：

```python
# 正确的调用方式
Skill(skill="edit-yaml", args="{标准化命令}")

# ❌ 错误：禁止直接使用 Bash 工具调用 edit-yaml 的脚本
# Bash(command="python .claude/skills/edit-yaml/scripts/xxx.py ...")

# ❌ 错误：禁止使用 Read + Edit 工具直接修改 YAML 文件
# Read(file_path="ci_config/rule.yaml")
# Edit(file_path="ci_config/rule.yaml", ...)
```

### 6.2 参数传递格式

传递给 edit-yaml 的参数应遵循以下格式：

| edit-yaml 工具 | 参数格式示例 |
|---------------|-------------|
| `find_yaml_node.py` | `查找ci_config/rule.yaml下CB.tasks.pkg中version为cann的任务` |
| `modify_node_key_value.py` | `查找ci_config/rule.yaml下CB.tasks.pkg中version为cann的任务，并修改ut_check为false` |
| `add_node_key_value.py` | `在ci_config/rule.yaml的CB.tasks.pkg.llt1节点中添加enabled:true` |
| `add_node.py` | `在ci_config/rule.yaml的CB.tasks.pkg节点下添加新任务new_task，内容为version:cann,enabled:true` |
| `move_yaml_node.py` | `移动ci_config/rule.yaml中的CB.tasks.llt1节点到CB.tasks.pkg下` |
| `modify_node_name.py` | `重命名ci_config/rule.yaml中的CB.tasks.pkg.llt1节点为CB.tasks.pkg.llt1_test` |

### 6.3 批量执行流程

对于多个操作，按以下流程执行：

```python
# 1. 解析所有诉求
operations = parse_all_requirements(user_input)

# 2. 统一展示执行计划
display_execution_plan(operations)

# 3. 用户确认
if user_confirms():
    # 4. 批量执行
    results = []
    for op in operations:
        result = Skill(skill="edit-yaml", args=op.command)
        results.append(result)

    # 5. 汇总展示结果
    display_results_summary(results)
```

## 七、Agent 核心职责

1. **精确理解用户意图**：从多样化的自然语言表述中准确提取操作意图
2. **标准化转化**：将用户诉求转化为 edit-yaml 技能可执行的标准命令
3. **批量解析能力**：支持解析多个诉求，统一规划执行计划
4. **统一展示确认**：解析完成后，统一展示所有操作的执行计划，一次性确认
5. **批量调用执行**：确认后依次通过 Skill 工具调用 edit-yaml 执行所有操作
6. **结果汇总反馈**：汇总展示所有执行结果和后续建议

## 八、注意事项

1. **优先使用本技能**：所有用户的自然语言诉求都应先通过本技能分析，再调用 edit-yaml 执行
2. **禁止直接操作**：禁止绕过本技能直接使用 Bash 或 Read+Edit 工具操作 YAML 文件
3. **统一确认机制**：解析完所有诉求后，统一展示执行计划并一次性确认，不再逐个确认
4. **错误预防**：当操作可能影响大量节点时，在执行计划中特别提醒
5. **可追溯性**：将分析过程和执行的命令记录到项目目录下的 cd.json 文件中

## 九、典型使用示例

### 示例1：单操作执行

**用户诉求：**
> "修改 rule.yaml 中 CB.tasks.pkg 下所有 version 为 cann 的任务，把 ut_check 设置为 false"

**本技能的处理流程：**
1. 分析诉求，提取关键信息
2. 展示执行计划（1个操作）
3. 用户确认后，调用 edit-yaml 技能执行
4. 展示执行结果

### 示例2：多步骤批量操作

**用户诉求：**
> "找出所有 version 为 cann2 的 CB.tasks.pkg 任务，将后缀修改为 cpp"

**本技能的处理流程：**
1. 分析诉求，识别为2步操作（查找+重命名）
2. 展示完整执行计划：
   - 操作1：查找 CB.tasks.pkg 下 version 为 cann2 的任务
   - 操作2：将查找到的任务名称添加 _cpp 后缀
3. 用户一次性确认所有操作
4. 依次调用 edit-yaml 执行
5. 汇总展示执行结果

### 示例3：不明确诉求处理

**用户诉求：**
> "修改 pkg 下的配置"

**本技能的处理流程：**
1. 识别信息不足
2. 通过 AskUserQuestion 工具询问详细信息
3. 收集完整信息后，展示执行计划
4. 用户确认后执行

## 十、维护与更新

- 技能名称：user_require_analysis
- 技能路径：`.claude/skills/user_require_analysis/`
- 文档位置：`.claude/skills/user_require_analysis/SKILL.md`
- 创建日期：2025-01-31
- 依赖技能：edit-yaml

## 十一、问题反馈

如遇到问题或需要新增功能，请联系 CI/CD 团队。
