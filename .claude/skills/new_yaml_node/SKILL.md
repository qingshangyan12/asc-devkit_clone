---
name: new-yaml-node
description: 描述yaml节点的命令规则和节点内容的映射规则，用于根据任务名构造新的yaml节点。同时，可针对特定yaml文件进行结构分析。
---

# YAML节点构造技能

## 一、技能概述

本技能（new-yaml-node）为华为 CANN CI/CD 体系下 **ci_config** 仓库提供 **YAML 节点构造和结构分析** 能力，核心支撑基于任务名自动生成符合规范的 YAML 配置节点。

本技能的核心价值在于：
- **自动化节点构造**：根据任务名自动生成符合规范的 YAML 节点结构
- **规则映射**：提供节点命令规则与节点内容的映射关系
- **结构分析**：支持对现有 YAML 文件进行结构分析和规则提取
- **标准化输出**：确保生成的节点符合 CI/CD 配置规范
- **减少错误**：避免手动构造节点时的拼写错误和格式问题

## 二、核心脚本

本技能提供两个核心脚本：

| 脚本名称 | 功能说明 |
|---------|---------|
| **create_node.py** | 根据节点名称自动构造节点内容并添加到 YAML 文件 |
| **analyze_yaml_structure.py** | YAML 文件结构分析工具，提取节点命名和内容映射规则 |

## 三、重要操作原则

### 3.1 ⚠️ 强制要求

**关键原则：节点构造必须遵循标准化规则**

- **强制要求**：所有新增 YAML 节点必须遵循本技能定义的命名规则和内容映射规则；
- **规则一致性**：节点命名规则必须与现有 CI/CD 配置保持一致；
- **内容完整性**：生成的节点必须包含必需的配置项，符合最小完整性要求；
- **格式规范性**：节点格式必须符合 YAML 语法规范和 CI/CD 配置要求；
- **可验证性**：生成的节点应能通过 YAML 语法检查和 CI/CD 配置验证。

### 3.2 核心能力

| 能力类型 | 功能说明 | 典型使用场景 |
|---------|---------|-------------|
| **节点命名规则** | 定义符合规范的 YAML 节点命名规则 | 新建任务节点时的命名指导 |
| **内容映射规则** | 定义节点名称与节点内容的映射关系 | 根据任务名自动填充节点内容 |
| **节点构造** | 根据任务名自动生成完整的 YAML 节点 | 批量创建标准化任务节点 |
| **结构分析** | 分析现有 YAML 文件的节点结构和规则 | 提取现有配置的构造规则 |
| **规则验证** | 验证节点命名和内容是否符合规范 | 生成节点后的质量检查 |

**技术特性：**
- **规则引擎**：内置节点命名和内容映射规则引擎
- **模板系统**：提供常用节点类型的内容模板
- **智能推断**：根据任务名自动推断节点类型和必需配置项
- **批量处理**：支持批量生成多个节点
- **格式检查**：自动检查生成节点的格式正确性

## 四、YAML 节点命名规则

### 4.1 节点命名规范

**基本格式：**
```
<任务类型>_<任务名称>_<可选后缀>
```

**命名规则：**
1. **任务类型（必需）**
   - `pkg`：打包任务
   - `compile`：编译任务
   - `test`：测试任务
   - `llt`：链接测试任务
   - `deploy`：部署任务
   - `misc`：其他任务

2. **任务名称（必需）**
   - 使用小写字母和数字
   - 不能以数字开头
   - 使用下划线分隔多个单词
   - 长度建议：3-20 个字符

3. **可选后缀（可选）**
   - `_test`：测试版本
   - `_prod`：生产版本
   - `_bak`：备份版本
   - `_v<版本号>`：版本标识
   - `smoke`：烟雾测试
   - `perf/performance`：性能测试

**命名示例：**
```yaml
# 合法命名
pkg_common_compile
test_smoke_v1
llt_memory_check
deploy_online_prod

# 非法命名
PkgCompile           # ❌ 未使用下划线
pkg                  # ❌ 缺少任务名称
1st_test             # ❌ 以数字开头
pkg-my-task          # ❌ 使用连字符
```

### 4.2 节点路径规则

**完整路径格式：**
```
CB.tasks.<任务类型>.<完整节点名>
```

**路径示例：**
```yaml
CB.tasks.pkg.pkg_common_compile
CB.tasks.test.test_smoke_v1
CB.tasks.llt.llt_memory_check
```

## 五、节点内容映射规则

### 5.1 通用配置项

所有节点类型必须包含的通用配置项：

```yaml
<节点名>:
  version: cann              # 版本号
  enabled: true             # 是否启用
  timeout: 3600             # 超时时间（秒）
```

### 5.2 各类型节点映射规则

#### 5.2.1 pkg 类型节点

**任务名特征：** 以 `pkg_` 开头

**默认内容模板：**
```yaml
pkg_<任务名>:
  version: cann
  enabled: true
  timeout: 3600
  type: pkg
  target:
    - <目标1>
    - <目标2>
  env:
    KEY: value
```

**必需配置项：**
- `type`: 必须为 `pkg`
- `target`: 打包目标列表

**示例：**
```yaml
pkg_common_compile:
  version: cann
  enabled: true
  timeout: 3600
  type: pkg
  target:
    - libcommon.so
    - libcommon.a
```

#### 5.2.2 compile 类型节点

**任务名特征：** 以 `compile_` 开头

**默认内容模板：**
```yaml
compile_<任务名>:
  version: cann
  enabled: true
  timeout: 7200
  type: compile
  source:
    - <源文件1>
    - <源文件2>
  include:
    - <头文件路径>
  flags:
    - <编译选项>
```

**必需配置项：**
- `type`: 必须为 `compile`
- `source`: 源文件列表

**示例：**
```yaml
compile_core_module:
  version: cann
  enabled: true
  timeout: 7200
  type: compile
  source:
    - src/core/*.cpp
  include:
    - include/
  flags:
    - -O2
    - -Wall
```

#### 5.2.3 test 类型节点

**任务名特征：** 以 `test_` 开头

**默认内容模板：**
```yaml
test_<任务名>:
  version: cann
  enabled: true
  timeout: 1800
  type: test
  case:
    - <测试用例1>
    - <测试用例2>
  dependency:
    - <依赖任务>
  ut_check: true
```

**必需配置项：**
- `type`: 必须为 `test`
- `case`: 测试用例列表

**示例：**
```yaml
test_api_function:
  version: cann
  enabled: true
  timeout: 1800
  type: test
  case:
    - test_api_001
    - test_api_002
  dependency:
    - compile_api_module
  ut_check: true
```

#### 5.2.4 llt 类型节点

**任务名特征：** 以 `llt_` 开头

**默认内容模板：**
```yaml
llt_<任务名>:
  version: cann
  enabled: true
  timeout: 5400
  type: llt
  module:
    - <模块1>
    - <模块2>
  coverage:
    - <覆盖率目标>
```

**必需配置项：**
- `type`: 必须为 `llt`
- `module`: 测试模块列表

**示例：**
```yaml
llt_memory_leak:
  version: cann
  enabled: true
  timeout: 5400
  type: llt
  module:
    - memory_alloc
    - memory_free
  coverage:
    - line: 80
    - branch: 70
```

#### 5.2.5 deploy 类型节点

**任务名特征：** 以 `deploy_` 开头

**默认内容模板：**
```yaml
deploy_<任务名>:
  version: cann
  enabled: true
  timeout: 2400
  type: deploy
  target:
    host: <目标主机>
    path: <部署路径>
  pre_check:
    - <前置检查>
  post_check:
    - <后置检查>
```

**必需配置项：**
- `type`: 必须为 `deploy`
- `target`: 部署目标信息

**示例：**
```yaml
deploy_online_prod:
  version: cann
  enabled: true
  timeout: 2400
  type: deploy
  target:
    host: 192.168.1.100
    path: /opt/cann/deploy
  pre_check:
    - disk_space_check
    - network_check
  post_check:
    - service_status_check
```

### 5.3 智能内容推断规则

**基于任务名的智能推断：**

1. **包含 `smoke` 的任务名**
   - 自动添加 `smoke_test: true` 配置项

2. **包含 `performance` 或 `perf` 的任务名**
   - 自动添加性能测试相关配置项
   - 延长默认 timeout 时间

3. **包含 `v<数字>` 后缀的任务名**
   - 自动提取版本号到 `version` 配置项

4. **包含 `prod` 后缀的任务名**
   - 设置 `env: production`
   - 启用更严格的检查

## 六、节点构造

### 6.1 节点构造流程

```
【步骤1】任务名称解析
    ├─ 提取任务类型（pkg/compile/test/llt/deploy）
    ├─ 提取任务名称
    └─ 识别可选后缀

【步骤2】规则匹配
    ├─ 匹配任务类型对应的内容模板
    ├─ 应用智能推断规则
    └─ 确定必需配置项

【步骤3】内容生成
    ├─ 填充通用配置项
    ├─ 添加类型特定配置项
    └─ 应用智能推断结果

【步骤4】格式验证
    ├─ YAML 语法检查
    ├─ 必需配置项完整性检查
    └─ 命名规范检查

【步骤5】节点输出
    └─ 输出符合规范的 YAML 节点
```

### 6.2 使用 create_node.py 构造节点

**功能特性：**

1. **智能解析节点名称**
   - 自动识别节点类型（pkg/compile/test/llt/deploy）
   - 提取节点名称和后缀信息

2. **自动生成节点内容**
   - 根据节点类型匹配对应的模板
   - 应用智能推断规则（smoke、performance、prod等后缀）
   - 自动填充通用配置项

3. **集成 edit-yaml 技能**
   - 直接调用 add_node.py 将节点添加到 YAML 文件
   - 支持预览模式（--dry-run）

#### 6.2.1 基本用法

```bash
# 构造节点（仅显示，不添加到文件）
python .claude/skills/new_yaml_node/scripts/create_node.py -n pkg_common_utils

# 构造节点并添加到 YAML 文件
python .claude/skills/new_yaml_node/scripts/create_node.py -n pkg_common_utils -f ci_config/rule.yaml

# 预览模式（不实际添加）
python .claude/skills/new_yaml_node/scripts/create_node.py -n test_api_smoke -f ci_config/rule.yaml --dry-run
```

#### 6.2.2 命令行参数

| 参数 | 说明 | 必需 |
|------|------|------|
| `-n, --name` | 节点名称 | 是 |
| `-f, --file` | YAML 文件路径 | 否（添加到文件时必需） |
| `-p, --parent` | 父路径（可选，默认自动推断） | 否 |
| `--dry-run` | 预览模式，不实际添加 | 否 |
| `-v, --verbose` | 显示详细信息 | 否 |

#### 6.2.3 智能推断规则详解

**1. Smoke 测试**
```bash
python create_node.py -n test_api_smoke
# 自动添加：smoke_test: true
```

**2. 性能测试**
```bash
python create_node.py -n test_performance_test
# 自动调整：
# - timeout: 7200（增加超时时间）
# - performance_test: true
```

**3. 生产环境**
```bash
python create_node.py -n deploy_online_prod
# 自动添加：env: production
```

**4. 版本标识**
```bash
python create_node.py -n pkg_module_v2
# 自动添加：build_version: 2
```

#### 6.2.4 输出示例

**pkg 类型节点：**
```yaml
pkg_common_utils:
  version: cann
  enabled: true
  timeout: 3600
  type: pkg
  target:
    - common_utils.so
    - common_utils.a
```

**test 类型节点（带 smoke 后缀）：**
```yaml
test_api_smoke:
  version: cann
  enabled: true
  timeout: 3600
  type: test
  case:
    - test_api_smoke_001
  dependency:
    - compile_api_smoke
  ut_check: true
  smoke_test: true
```

**compile 类型节点：**
```yaml
compile_core_module:
  version: cann
  enabled: true
  timeout: 7200
  type: compile
  source:
    - src/core_module/*.cpp
  include:
    - include/
  flags:
    - -O2
    - -Wall
```

#### 6.2.5 与 edit-yaml 技能的集成

create_node.py 通过调用 edit-yaml 的 add_node.py 将节点添加到 YAML 文件：

```
用户输入节点名称
    ↓
create_node.py 解析名称并生成节点内容
    ↓
调用 add_node.py 将节点添加到 YAML 文件
    ↓
返回添加结果
```

**自动执行的命令示例：**
```bash
python .claude/skills/edit-yaml/scripts/add_node.py \
  -f ci_config/rule.yaml \
  -p CB.tasks.pkg \
  -n "pkg_common_utils: {version: cann, enabled: true, timeout: 3600, type: pkg, target: [common_utils.so, common_utils.a]}"
```

#### 6.2.6 完整工作流程示例

```bash
# 步骤1：预览节点内容
python .claude/skills/new_yaml_node/scripts/create_node.py -n pkg_new_feature

# 步骤2：添加到测试文件验证
python .claude/skills/new_yaml_node/scripts/create_node.py -n pkg_new_feature -f test_example.yaml

# 步骤3：验证添加结果
python .claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py -f test_example.yaml --type-distribution
```

## 七、YAML 文件结构分析

### 7.1 结构分析功能

本技能支持对现有 YAML 文件进行结构分析，提取以下信息：

1. **节点类型分布**
   - 统计各类型节点的数量
   - 分析节点类型占比

2. **命名规范分析**
   - 检查节点命名是否符合规范
   - 识别命名模式

3. **配置项完整性**
   - 检查各节点是否包含必需配置项
   - 识别缺失的配置项

4. **内容规则提取**
   - 从现有节点中提取内容模式
   - 生成规则映射建议

### 7.2 使用 analyze_yaml_structure.py

#### 7.2.1 基本用法

```bash
# 完整分析（包含所有功能）
python .claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py -f ci_config/rule.yaml

# 仅分析节点类型分布
python .claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py -f ci_config/rule.yaml --type-distribution

# 仅检查命名规范
python .claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py -f ci_config/rule.yaml --check-naming

# 仅检查配置项完整性
python .claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py -f ci_config/rule.yaml --check-config

# 仅提取内容映射规则
python .claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py -f ci_config/rule.yaml --extract-rules

# 仅输出总结报告
python .claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py -f ci_config/rule.yaml --summary
```

#### 7.2.2 命令行参数

| 参数 | 说明 | 必需 |
|------|------|------|
| `-f, --file` | YAML 文件路径 | 是 |
| `--type-distribution` | 仅输出节点类型分布 | 否 |
| `--check-naming` | 仅检查命名规范 | 否 |
| `--check-config` | 仅检查配置项完整性 | 否 |
| `--extract-rules` | 仅提取内容映射规则 | 否 |
| `--summary` | 仅输出总结报告 | 否 |

### 7.3 分析结果示例

#### 7.3.1 节点类型分布报告

```
============================================================
节点类型分布统计
============================================================

总计：21 个节点

├─ pkg: 4 个节点 (19.0%)
├─ test: 4 个节点 (19.0%)
├─ compile: 3 个节点 (14.3%)
├─ llt: 3 个节点 (14.3%)
└─ deploy: 2 个节点 (9.5%)
```

#### 7.3.2 命名规范检查报告

```
============================================================
命名规范检查
============================================================

命名规范检查结果：
[√] 符合规范：9 个节点 (42.9%)
[X] 不符合规范：12 个节点 (57.1%)

不规范节点列表：

1. Pkg_Module
   路径: CB.tasks.pkg.Pkg_Module
   问题:
   - 未使用小写字母和下划线
   - 缺少类型前缀（pkg/compile/test/llt/deploy）
```

#### 7.3.3 配置项完整性报告

```
============================================================
配置项完整性分析
============================================================

[X] 2 个节点配置不完整（共检查 21 个节点）

1. pkg_utils (类型: pkg)
   - 建议项缺失: enabled
2. compile_helper (类型: compile)
   - 必需项缺失: source
   - 建议项缺失: timeout
```

#### 7.3.4 内容映射规则报告

```
============================================================
节点内容映射规则提取
============================================================

### pkg 类型节点规则
------------------------------------------------------------

基于 4 个 pkg 节点的统计：

配置项                  出现次数      占比        必需性
------------------------------------------------------------
version              3            75.0%    可选
type                 3            75.0%    可选
target               3            75.0%    可选
enabled              2            50.0%    可选
timeout              2            50.0%    可选

推荐内容模板：
pkg_common_compile_<任务名>:
  version: cann
  enabled: true
  timeout: 3600
  type: <type_值>
  target: [libcommon.so]
```

### 7.4 5种分析模式详解

**analyze_yaml_structure.py** 提供5种分析模式：

1. **完整分析（默认）**：执行所有分析功能
2. **节点类型分布**：统计各类型节点的数量和占比
3. **命名规范检查**：检查节点命名是否符合规范
4. **配置项完整性检查**：检查必需和推荐配置项
5. **内容规则提取**：从现有节点提取配置项使用频率和推荐模板

## 八、与其他技能的配合

### 8.1 与 edit-yaml 技能配合

**典型工作流：**
```
用户需求
  ↓
new_yaml_node（构造节点内容）
  ↓
edit-yaml（将节点添加到 YAML 文件）
```

**使用场景：**
1. 使用 new_yaml_node 根据任务名生成节点内容
2. 使用 edit-yaml 的 `add_node.py` 将节点添加到 YAML 文件

**示例：**
```bash
# 步骤1：使用 new_yaml_node 生成节点
Skill(skill="new_yaml_node", args="构造任务名为 pkg_common_utils 的节点")

# 步骤2：使用 edit-yaml 添加节点
python .claude/skills/edit-yaml/scripts/add_node.py \
  -f ci_config/rule.yaml \
  -p CB.tasks.pkg \
  -n "pkg_common_utils: {version: cann, enabled: true, timeout: 3600, type: pkg, target: [libcommon_utils.so]}"
```

### 8.2 与 user_require_analysis 技能配合

**典型工作流：**
```
用户自然语言需求
  ↓
user_require_analysis（分析需求）
  ↓
new_yaml_node（构造节点）
  ↓
edit-yaml（添加节点）
```

**使用场景：**
用户描述："创建一个名为 pkg_common_utils 的打包任务"

1. user_require_analysis 解析需求，识别出：
   - 任务类型：pkg
   - 任务名：common_utils
   - 操作：创建新节点

2. new_yaml_node 根据任务名生成节点内容

3. edit-yaml 将节点添加到 YAML 文件

## 九、最佳实践

### 9.1 节点命名建议

- **语义化命名**：使用能清晰表达任务功能的名称
- **避免缩写**：除非是广泛认知的缩写（如 api、utils）
- **保持一致性**：相同类型的任务使用相同的命名模式
- **版本标识**：使用后缀区分不同版本

**良好示例：**
```yaml
pkg_common_utils          # ✅ 清晰表达功能和类型
test_api_smoke            # ✅ 明确测试类型和范围
compile_core_module_v2    # ✅ 包含版本信息
```

**避免示例：**
```yaml
pkg_cu                    # ❌ 缩写不清晰
t1                        # ❌ 无意义的名称
pkg_test                  # ❌ 类型混淆
```

### 9.2 内容配置建议

- **保持简洁**：只包含必需的配置项
- **合理设置超时**：根据任务复杂度设置合适的 timeout
- **明确依赖关系**：正确配置 dependency 避免执行顺序错误
- **环境隔离**：使用 env 配置项隔离不同环境

### 9.3 批量构造建议

- **统一规划**：批量构造前先规划所有节点
- **分批验证**：每批节点生成后立即验证
- **版本控制**：批量修改前做好版本备份
- **渐进式添加**：先添加基础节点，再添加复杂节点

## 十、依赖要求

### 10.1 Python 环境

- Python 3.x

### 10.2 Python 模块

- PyYAML 模块

### 10.3 安装依赖

```bash
pip3 install PyYAML
```

## 十一、注意事项

### 11.1 编码问题

Windows 控制台可能显示中文乱码，但功能正常。建议在 Git Bash 或支持 UTF-8 的终端中运行。

### 11.2 节点识别

脚本会递归遍历 YAML 文件中的所有字典节点，包括非任务节点（如 CB、tasks 等）。分析时需要根据实际情况判断哪些是真正的任务节点。

### 11.3 类型推断

节点类型根据节点名称前缀或内容中的 `type` 字段推断。如果没有明确的类型标识，会被归类为 `misc` 类型。

### 11.4 必需配置项

每种类型节点定义了必需配置项，但这些规则可能需要根据实际项目需求调整。

### 11.5 父路径验证

在使用 create_node.py 添加节点前，需确认目标父路径存在。如果父路径不存在，需要先创建。

## 十二、测试示例

### 12.1 使用测试文件验证

使用测试文件验证脚本功能：

```bash
# 进入技能目录
cd .claude/skills/new_yaml_node

# 运行完整分析
python scripts/analyze_yaml_structure.py -f test_example.yaml

# 测试单个功能
python scripts/analyze_yaml_structure.py -f test_example.yaml --type-distribution
python scripts/analyze_yaml_structure.py -f test_example.yaml --check-naming
python scripts/analyze_yaml_structure.py -f test_example.yaml --check-config
python scripts/analyze_yaml_structure.py -f test_example.yaml --extract-rules
```

## 十三、常见问题

### Q1：如何为已存在的节点类型添加新的映射规则？

根据现有节点的命名和内容模式，在对应类型的映射规则中添加新的规则条目。

### Q2：节点构造后如何验证正确性？

使用以下方法验证：
1. YAML 语法检查
2. 必需配置项完整性检查
3. 与现有同类节点对比
4. CI/CD 配置验证工具

### Q3：如何处理不符合命名规范的现有节点？

1. 使用结构分析功能识别不规范节点
2. 评估影响范围
3. 制定重命名计划
4. 使用 edit-yaml 的 modify_node_name.py 进行批量修改

### Q4：智能推断规则是否可以自定义？

是的，可以根据项目需求自定义智能推断规则，修改规则配置文件。

### Q5：为什么有些节点被识别为 misc 类型？

这通常是因为节点名称没有明确的类型前缀（pkg/compile/test/llt/deploy），或者节点内容中没有 `type` 字段。

### Q6：如何修改必需配置项的定义？

编辑 `analyze_yaml_structure.py` 脚本中的 `required_keys` 字典（约第 200-209 行），根据实际需求添加或修改配置项。

### Q7：如何添加新的节点类型？

1. 在 `required_keys` 中添加新类型的必需配置项
2. 在 `_infer_node_type` 方法中添加类型识别规则（如果需要）
3. 在 create_node.py 的 TYPE_TEMPLATES 中添加新类型的模板

## 十四、维护信息

- **技能名称**：new-yaml-node
- **技能路径**：`.claude/skills/new_yaml_node/`
- **脚本位置**：`.claude/skills/new_yaml_node/scripts/`
- **文档位置**：`.claude/skills/new_yaml_node/SKILL.md`
- **更新日期**：2025-02-06

### 脚本文件

| 脚本名称 | 功能说明 |
|---------|---------|
| **create_node.py** | 根据节点名称构造节点并添加到 YAML 文件 |
| **analyze_yaml_structure.py** | YAML 文件结构分析工具 |

## 十五、相关技能

- **edit-yaml**：YAML 配置文件修改技能
- **user_require_analysis**：用户需求分析技能

**典型工作流：**
```
用户需求
  → user_require_analysis（分析需求）
  → new_yaml_node（构造节点）✅ 本技能
  → edit-yaml（添加节点）
```
