---
name: edit-yaml
description: 采用流式读取、递归遍历、命令行参数解析等功能高效完成大型yaml文件修改。适用于对大型yaml文件进行配置项搜索、配置项增加、配置项修改、配置项删除、配置项注释/解注释及格式标准化等操作,或当用户指示"查找/搜索/修改xxx.yaml文件"时使用。
---

# YAML文件修改技能

## 一、技能概述

本技能（edit-yaml）为华为 CANN CI/CD 体系下 **asc-devkit** 仓库的工程需求开发提供标准化指导，核心支撑该仓库内 YAML 配置文件的**搜索、查找、新增、修改、删除、语法检查与修复**等全维度操作。

所有操作均基于**asc-devkit**代码仓的**ci_config/**路径下存储的工程全量 YAML 配置文件。

## 二、重要操作原则

### 2.1 ⚠️ 强制要求

**关键原则：所有YAML配置文件修改必须优先使用edit-yaml技能提供的标准化工具**

- **强制要求**：任何涉及YAML文件的修改操作，**必须**使用本技能提供的标准化Python脚本工具；
- **禁止直接操作**：禁止使用Read+Edit工具组合直接修改YAML文件，除非本技能工具无法满足需求且经过评估确认；
- **禁止直接调用脚本**：禁止直接使用Bash工具调用edit-yaml技能的Python脚本，必须通过Skill工具调用edit-yaml技能；
- **工具优先级**：Skill工具(edit-yaml技能) > Bash工具(脚本) > Read+Edit工具；
- **可回溯要求**：将执行过程中使用的脚本命令都保存到项目目录下的cd.json文件中，重新开启claude时清空。

### 2.2 核心工具集

| 工具名称 | 功能说明 | 典型使用场景 |
|---------|---------|-------------|
| **find_yaml_node.py** | 查找YAML文件中符合指定条件的节点 | 搜索包含特定key:value的节点 |
| **modify_node_name.py** | 批量修改节点名称 | 重命名节点、修改任务名称后缀 |
| **modify_node_key_value.py** | **修改已有键值**：批量修改节点内的key:value键值对 | 节点内已存在某个 key，需要修改其 value |
| **add_node_key_value.py** | **新增简单键值**：在指定节点内批量添加新的key:value键值对 | 在已存在节点内新增简单的 key:value 配置项 |
| **add_node.py** | **新增嵌套节点**：在指定父节点路径下批量添加新的子节点（支持嵌套） | 创建新的子节点或包含多层嵌套结构的复杂配置|
| **move_yaml_node.py** | 批量移动节点到指定父节点路径下 | 重组任务结构、迁移节点位置 |
| **fix_non_standard_yaml.py** | 校验和修复YAML文件格式 | 修复非标准YAML语法 |

**技术特性（适用于所有工具）：**
- **流式读取**：支持大型 YAML 文件，避免内存溢出
- **递归遍历**：高效定位和修改深层嵌套节点
- **自动备份**：默认创建带时间戳的备份文件
- **安全恢复**：保存失败时自动从备份恢复
- **多文档支持**：支持 YAML 多文档格式（--- 分隔）
- **类型推断**：自动识别布尔值、整数、浮点数和字符串

## 三、详细操作指南

### 3.1 查找节点：find_yaml_node.py

查找YAML文件中符合指定条件的节点。

**命令格式：**
```bash
# 推荐方式：使用命令行参数
python .claude/skills/edit-yaml/scripts/find_yaml_node.py -f ${yaml_path} -r target_root -k target_key -v target_value

# 使用示例
python .claude/skills/edit-yaml/scripts/find_yaml_node.py -f ci_config/rule.yaml -r CB.pkg -k version -v cann
```

**参数说明：**
- `--file, -f`: YAML 文件路径（必需）
- `--root, -r`: 要查找的根节点路径，支持多级（如CB.pkg.compile），默认是CB
- `--key, -k`: 要匹配的键名，默认version
- `--value, -v`: 要匹配的值，默认cann_eco_debug

### 3.2 修改节点名称：modify_node_name.py

批量修改节点名称。

**命令格式：**
```bash
# 批量修改（3个及以上节点，使用映射文件）
python .claude/skills/edit-yaml/scripts/modify_node_name.py -f ${yaml_path} --mappings-file mappings.txt

# 少量修改（1-2个节点，使用命令行参数）
python .claude/skills/edit-yaml/scripts/modify_node_name.py -f ${yaml_path} -m CB.tasks.pkg.llt1=CB.tasks.pkg.llt1_test
```

**映射文件格式（每行一个映射）：**
```bash
CB.tasks.pkg.llt5 -> CB.tasks.pkg.llt5_pkg
CB.tasks.pkg.llt1_newllt = CB.tasks.pkg.llt1_pkg
```

**参数说明：**
- `--file, -f`: YAML 文件路径（必需）
- `--mappings, -m`: 节点映射关系（使用 `->` 或 `=` 分隔）
- `--mappings-file`: 包含节点映射关系的文件
- `--no-backup`: 不创建备份文件（谨慎使用）

**重要说明：**
- 对于批量修改（3个及以上节点），**必须优先使用 `--mappings-file` 参数**
- 仅在1-2个节点的少量修改时，才使用命令行 `-m` 参数

### 3.3 修改键值对：modify_node_key_value.py

批量修改节点内的key:value键值对。

**命令格式：**
```bash
# 批量修改（3个及以上节点）
python .claude/skills/edit-yaml/scripts/find_yaml_node.py -f ${yaml_path} -r CB -k version -v cann > nodes.txt
python .claude/skills/edit-yaml/scripts/modify_node_key_value.py -f ${yaml_path} --nodes-file nodes.txt -k version -v cann_new

# 少量修改（1-2个节点）
python .claude/skills/edit-yaml/scripts/modify_node_key_value.py -f ${yaml_path} -n CB.tasks.pkg.llt1_new -k ut_check -v false
```

**参数说明：**
- `--file, -f`: YAML 文件路径（必需）
- `--nodes, -n`: 要修改的节点路径列表（空格分隔）
- `--nodes-file`: 包含要修改的节点路径列表的文件
- `--key, -k`: 要修改的键名（必需）
- `--value, -v`: 新的值（必需）
- `--type, -t`: 值的类型（auto/bool/int/float/str，默认为 auto）
- `--no-backup`: 不创建备份文件（谨慎使用）

### 3.4 添加键值对：add_node_key_value.py

在节点内批量添加新的key:value键值对。

**命令格式：**
```bash
# 批量添加（3个及以上节点）
python .claude/skills/edit-yaml/scripts/find_yaml_node.py -f ${yaml_path} -r CB -k version -v cann > nodes.txt
python .claude/skills/edit-yaml/scripts/add_node_key_value.py -f ${yaml_path} --nodes-file nodes.txt -k enabled -v true

# 少量添加（1-2个节点）
python .claude/skills/edit-yaml/scripts/add_node_key_value.py -f ${yaml_path} -n CB.tasks.pkg.llt1 -k retry_count -v 3 -t int
```

**参数说明：**
- `--file, -f`: YAML 文件路径（必需）
- `--nodes, -n`: 要添加的节点路径列表（空格分隔）
- `--nodes-file`: 包含节点路径列表的文件
- `--key, -k`: 要添加的键名（必需）
- `--value, -v`: 要添加的值（必需）
- `--type, -t`: 值的类型（auto/bool/int/float/str，默认为 auto）
- `--skip`: 如果键已存在，跳过该键（默认覆盖已存在的键）
- `--no-backup`: 不创建备份文件（谨慎使用）

### 3.5 添加节点：add_node.py

在指定父节点路径下批量添加新的子节点（支持嵌套）。

**命令格式：**
```bash
# 批量添加节点（3个及以上父节点）
python .claude/skills/edit-yaml/scripts/find_yaml_node.py -f ${yaml_path} -r CB.tasks -k type -v pkg > parents.txt
python .claude/skills/edit-yaml/scripts/add_node.py -f ${yaml_path} --parents-file parents.txt -n "new_task: {version: cann}"

# 少量添加节点（1-2个父节点）
python .claude/skills/edit-yaml/scripts/add_node.py -f ${yaml_path} -p CB.tasks.pkg -n "new_task: {version: cann, enabled: true}"

# 添加嵌套节点
python .claude/skills/edit-yaml/scripts/add_node.py -f ${yaml_path} -p CB.tasks.pkg -n "new_sub_node.level1.level2: {key: value}"
```

**参数说明：**
- `--file, -f`: YAML 文件路径（必需）
- `--parents, -p`: 父节点路径列表（空格分隔）
- `--parents-file`: 包含父节点路径列表的文件
- `--node, -n`: 要添加的新节点规格（必需）
  - 格式1：`node_name`（创建空节点）
  - 格式2：`node_name: content`（创建带内容的节点）
  - 支持嵌套节点：`parent.child.grandchild`
- `--skip`: 如果节点已存在，跳过该节点（默认覆盖已存在的节点）
- `--no-backup`: 不创建备份文件（谨慎使用）

### 3.6 移动节点：move_yaml_node.py

批量移动节点到指定父节点路径下。

**命令格式：**
```bash
# 批量移动（3个及以上节点）
python .claude/skills/edit-yaml/scripts/find_yaml_node.py -f ${yaml_path} -r CB -k version -v cann > nodes.txt
python .claude/skills/edit-yaml/scripts/move_yaml_node.py -f ${yaml_path} --nodes-file nodes.txt -t CB.tasks.pkg

# 少量移动（1-2个节点）
python .claude/skills/edit-yaml/scripts/move_yaml_node.py -f ${yaml_path} -n CB.tasks.llt1 -t CB.tasks.compile

# 移动并替换后缀（替换模式）
python .claude/skills/edit-yaml/scripts/move_yaml_node.py -f ${yaml_path} -n CB.tasks.llt1_test -t CB.tasks.pkg --suffix prod
# 结果：CB.tasks.llt1_test -> CB.tasks.pkg.llt1_prod

# 移动并添加后缀（添加模式）
python .claude/skills/edit-yaml/scripts/move_yaml_node.py -f ${yaml_path} -n CB.tasks.llt1 -t CB.tasks.pkg --suffix test --addmode
# 结果：CB.tasks.llt1 -> CB.tasks.pkg.llt1_test
```

**参数说明：**
- `--file, -f`: YAML 文件路径（必需）
- `--nodes, -n`: 要移动的节点路径列表（空格分隔）
- `--nodes-file`: 包含节点路径列表的文件
- `--target, -t`: 目标父节点路径（必需）
- `--suffix`: 指定任务名后缀（可选）
- `--addmode`: 后缀添加模式（true=直接添加后缀，false=替换最后一个"_"后的部分，默认为false）
- `--no-backup`: 不创建备份文件（谨慎使用）

**注意事项：**
- 移动节点会从原位置删除节点
- **如果目标路径下已存在同名节点，操作会直接覆盖该节点（会给出警告提示）**
- 使用 `--suffix` 参数时，默认为替换模式（替换最后一个"_"后的部分），使用 `--addmode` 可切换为添加模式

### 3.7 修复YAML格式：fix_non_standard_yaml.py

校验和修复YAML文件格式。

**命令格式：**
```bash
# 仅检查语法问题
python .claude/skills/edit-yaml/scripts/fix_non_standard_yaml.py ci_config/rule.yaml

# 检查并自动修复（创建备份）
python .claude/skills/edit-yaml/scripts/fix_non_standard_yaml.py ci_config/rule.yaml --fix
```

**检测与修复的问题：**
1. **键值对格式错误**: `key:value` → `key: value`（冒号后缺少空格）
2. **Tab 字符混用**: 检测并提示使用 Tab 字符缩进的行
3. **行尾多余空格**: 自动清理行尾多余空格

## 四、依赖要求

- Python 3.x
- PyYAML 模块（用于验证修复后的文件）

**安装依赖：**
```bash
pip3 install PyYAML
```

## 五、注意事项

1. **备份机制**: 所有修改工具默认都会创建带时间戳的备份文件，格式为 `原文件名.backup_YYYYMMDD_HHMMSS`
2. **批量操作**: 对于3个及以上节点的批量操作，必须使用文件参数（`--nodes-file`、`--mappings-file`等）
3. **批量添加键值**: add_node_key_value.py 默认会覆盖已存在的键。如需跳过已存在的键值，请使用 --skip 参数
4. **批量添加节点**: add_node.py 默认会覆盖已存在的节点。如需跳过已存在的节点，请使用 --skip 参数
5. **⚠️ 路径查找失败处理**: 当使用 find_yaml_node.py 查找指定路径的节点失败时，**禁止**在未确认路径正确性的情况下，执行移动、修改等破坏性操作
6. **节点重命名**: 对于3个及以上节点的批量修改，必须使用 `--mappings-file` 参数创建映射文件；仅在1-2个节点的少量修改时才使用命令行 `-m` 参数

## 六、核心定义

- **名称后缀**：指配置项名称中**最后一个下划线（_）** 之后的字符段
- **修改后缀**：将原名称后缀修改为新的名称后缀（默认模式）
- **增加后缀**：在原名称后增加**下划线（_）**后，增加新的名称后缀（使用 --addmode）

## 七、维护信息

- 脚本位置: `.claude/skills/edit-yaml/scripts/`
- 文档位置: `.claude/skills/edit-yaml/SKILL.md`
- 更新日期: 2025-01-31
