# CLAUDE.md
本文件为 Claude Code (claude.ai/code) 在**ci_config/**路径下的**ci_config**代码仓库中，实现工程需求开发及自动化上库流程提供标准化操作指导。

## 一、项目概述
本项目隶属于华为 CANN CI/CD 工程体系，核心目标是在**ci_config/**路径下的**ci_config**代码仓库中，完成工程需求开发落地及代码自动化上库全流程。
其中**ci_config**代码仓为华为 CANN CI/CD 工程核心配置仓，**ci_config/**路径下存储了该工程全量的 YAML 配置文件，是所有操作的核心载体。

## 二、项目目录结构
- `ci_config/`：华为 CANN CI/CD 工程项目核心配置仓，**ci_config/**路径下包含了工程全量 YAML 配置文件（必需）

## 三、核心职责
1. **优先使用user_require_analysis技能**：所有用户自然语言工程诉求，**必须优先使用`user_require_analysis`技能**进行需求理解和转化，该技能将自然语言诉求转化为 edit-yaml 技能可执行的标准命令；
2. **使用edit-yaml技能执行**：在`user_require_analysis`技能完成需求分析并经用户确认后，通过`edit-yaml`技能实现 CANN CI/CD 工程 YAML 配置文件的搜索、查找、修改等操作，完成工程需求开发落地；
3. **使用git-code-process技能执行**：进行**ci_config**代码仓的版本管理，将修改后的配置文件标准化上传至 CodeHub。

## 四、重要操作原则
**⚠️ 关键原则：所有用户自然语言诉求必须优先使用user_require_analysis技能，所有YAML配置文件修改必须优先使用edit-yaml技能，所有代码提交上库必须优先使用git-code-process技能**

### 4.1 代码上库优先级

**强制要求：代码提交上库必须优先使用git-code-process技能**

- **第一优先级：git-code-process 技能**
  - 所有涉及代码提交、上库的操作，**必须**优先通过 `git-code-process` 技能执行；
  - 该技能提供标准化的 Git 操作流程：git stash → git pull → git stash pop → git add → git commit → repo upload；
  - 内置冲突预防机制，确保代码基于最新基线；
  - 支持规范化的提交信息格式。

- **禁止直接操作**：
  - 禁止绕过 git-code-process 技能直接使用 Bash 工具执行 git 命令；
  - 禁止跳过 stash + pull 机制直接提交代码。

- **工具优先级**：Skill工具(git-code-process技能) > Bash工具(git命令) > 手动操作。

### 4.2 YAML修改优先级
- **第一优先级：user_require_analysis 技能**
  - 用户使用自然语言描述工程需求时，**必须**优先通过`user_require_analysis`技能进行需求分析和转化；
  - 该技能将自然语言诉求转化为 edit-yaml 技能可执行的标准命令；
  - 支持批量解析多个诉求，统一展示执行计划，一次性确认后批量执行。

- **第二优先级：edit-yaml 技能**
  - 任何涉及YAML文件的修改操作（包括但不限于节点名称修改、键值对修改、配置项新增/删除等），**必须**通过`Skill工具`调用`edit-yaml`技能及其提供的标准化脚本工具；
  - `user_require_analysis`技能分析完成后，自动通过 Skill 工具调用 `edit-yaml` 技能执行。

- **禁止直接操作**：
  - 禁止使用Read+Edit工具组合直接修改YAML文件，除非edit-yaml技能无法满足需求且经过评估确认；
  - 禁止直接使用Bash工具调用edit-yaml技能的Python脚本，必须通过Skill工具调用edit-yaml技能。

- **工具优先级**：Skill工具(user_require_analysis技能) > Skill工具(edit-yaml技能) > Bash工具(脚本) > Read+Edit工具；
- **可回溯要求**：将执行过程中使用的脚本命令都保存到项目目录下的cd.json内，重新开启claude时清空。

### 4.3 edit-yaml技能核心工具集

| 工具名称 | 功能说明 | 典型使用场景 |
|---------|---------|-------------|
| **find_yaml_node.py** | 查找YAML文件中符合指定条件的节点 | 搜索包含特定key:value的节点 |
| **modify_node_name.py** | 批量修改节点名称 | 重命名节点、修改任务名称后缀 |
| **modify_node_key_value.py** | **修改已有键值**：批量修改节点内的key:value键值对 | 节点内已存在某个 key，需要修改其 value |
| **add_node_key_value.py** | **新增简单键值**：在指定节点内批量添加新的key:value键值对 | 在已存在节点内新增简单的 key:value 配置项 |
| **add_node.py** | **新增嵌套节点**：在指定父节点路径下批量添加新的子节点（支持嵌套） | 创建新的子节点或包含多层嵌套结构的复杂配置|
| **move_yaml_node.py** | 批量移动节点到指定父节点路径下 | 重组任务结构、迁移节点位置 |
| **fix_non_standard_yaml.py** | 校验和修复YAML文件格式 | 修复非标准YAML语法 |

**重要说明：**
- **move_yaml_node.py**：移动节点会从原位置删除节点，目标位置已存在同名节点时直接覆盖（给出警告提示）
- **add_node_key_value.py**：默认会覆盖已存在的键，如需跳过已存在的键值，请使用 `--skip` 参数
- **add_node.py**：默认会覆盖已存在的节点，如需跳过已存在的节点，请使用 `--skip` 参数

## 五、专属可用技能
### 5.1 user_require_analysis
- **核心定位**：用户自然语言诉求与 YAML 配置修改之间的**智能翻译层**；
- **核心能力**：自然语言理解、标准化转化为 edit-yaml 命令、批量解析与统一展示、自动调用执行；
- **适用场景**：用户使用自然语言描述 YAML 配置修改需求时；
- **调用方式**：用户直接使用自然语言描述需求，Agent 自动调用本技能进行分析和转化。

### 5.2 edit-yaml
- **核心能力**：承接用户工程需求，全覆盖 YAML 配置文件的**搜索、查找、修改、新增**等核心操作；
- **适用场景**：所有涉及 YAML 配置文件内容检索、定位、调整的工程需求开发环节；
- **调用方式**：由 user_require_analysis 技能分析完成后自动调用，或用户直接使用标准化命令调用。
- **详细说明**：参见 `.claude/skills/edit-yaml/SKILL.md`

### 5.3 git-code-process
- **核心能力**：工程需求开发完成后，通过 Git 命令完成**代码提交、推送、上库**等标准化流程；
- **适用场景**：YAML 配置文件修改完成后的自动化上库环节。

## 六、标准化操作典型流程
1. **目录校验与切换**：确认当前工作目录为`ci_config`仓根目录，若否，执行`cd ci_config/`命令切换至目标路径；
2. **用户需求分析**：
   - 用户使用自然语言描述工程需求；
   - Agent 自动调用`user_require_analysis`技能进行需求分析，转化为 edit-yaml 技能可执行的标准命令；
   - **统一展示执行计划**：将解析出的所有操作统一展示给用户；
   - **一次性确认**：用户一次性确认所有操作；
3. **YAML 配置开发**：
   - 用户确认后，`user_require_analysis`技能自动通过 Skill 工具依次调用`edit-yaml`技能；
   - `edit-yaml`技能执行具体操作：目标 YAML 文件的搜索、查找、修改（或新增）；
4. **命令记录**：将执行过程中使用的脚本命令保存到项目目录下的cd.json文件中；
5. **自动化代码上库**：YAML 配置文件修改完成后，使用`git-code-process`技能执行标准化 Git 操作，完成代码仓的版本更新与上库。

## 七、skills修改原则
1. **保持代码风格统一**：确认所有skills的python脚本，markdown文件风格统一；
2. **删除死代码**：删除未使用的变量、函数。
