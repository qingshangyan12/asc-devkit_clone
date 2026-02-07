---
name: new-yaml-node
description: YAML节点构造技能 - 根据任务名自动生成符合规范的YAML配置节点，支持YAML文件结构分析和规则提取
---

# YAML节点构造技能

## 一、技能概述

### 1.1 技能定位

本技能（new-yaml-node）为华为 CANN CI/CD 体系下 **ci_config** 仓库提供 **YAML 节点构造和结构分析** 能力，核心支撑基于任务名自动生成符合规范的 YAML 配置节点。

### 1.2 核心价值

- **自动化节点构造**：根据任务名自动生成符合规范的 YAML 节点结构
- **规则映射**：提供节点命名规则与节点内容的映射关系
- **结构分析**：支持对现有 YAML 文件进行结构分析和规则提取
- **标准化输出**：确保生成的节点符合 CI/CD 配置规范
- **减少错误**：避免手动构造节点时的拼写错误和格式问题

### 1.3 核心脚本

| 脚本名称 | 功能说明 | 典型用途 |
|---------|---------|---------|
| **create_node.py** | 根据节点名称自动构造节点内容 | 新建标准化任务节点 |
| **analyze_yaml_structure.py** | 分析YAML文件结构和规则 | 提取现有配置规则、验证命名规范 |

## 二、快速开始

### 2.1 三大典型使用场景

**场景1：根据任务名创建新节点**
```bash
# 最简单的用法 - 只提供节点名
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py -n pkg_common_utils

# 指定目标文件
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py \
  -n test_api_smoke -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml

# 预览模式（不实际写入）
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py \
  -n compile_core -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --dry-run
```

**场景2：分析YAML文件结构**
```bash
# 基本分析
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml

# 类型分布分析
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --type-distribution

# 命名规范检查
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --check-naming
```

**场景3：与edit-yaml技能配合使用**
```bash
# 1. 使用create_node.py生成节点
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py \
  -n pkg_new_feature > node_template.yaml

# 2. 使用edit-yaml技能的add_node.py添加节点
python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/add_node.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml \
  -p CB.tasks.pkg \
  --content-file node_template.yaml
```

### 2.2 典型工作流程

```
【需求】创建新的pkg任务节点
    ↓
【步骤1】使用create_node.py生成节点
  python create_node.py -n pkg_new_feature -f ci_config/rule.yaml
    ↓
【步骤2】验证生成的节点内容
  检查version、enabled、timeout等必需配置项
    ↓
【步骤3】调整节点内容（可选）
  根据实际需求修改target、env等配置项
    ↓
【步骤4】验证YAML格式
  python fix_non_standard_yaml.py -f ci_config/rule.yaml
    ↓
【完成】节点已添加到YAML文件
```

## 三、节点命名规则

### 3.1 命名规范

**基本格式：**
```
<任务类型>_<任务名称>_<可选后缀>
```

**任务类型（必需）：**
| 类型 | 说明 | 示例 |
|-----|------|------|
| `pkg` | 打包任务 | pkg_common_utils |
| `compile` | 编译任务 | compile_core_module |
| `test` | 测试任务 | test_api_function |
| `llt` | 链接测试任务 | llt_memory_check |
| `deploy` | 部署任务 | deploy_online_prod |
| `misc` | 其他任务 | misc_custom_task |

**任务名称（必需）：**
- 使用小写字母和数字
- 不能以数字开头
- 使用下划线分隔多个单词
- 长度建议：3-20个字符

**可选后缀：**
| 后缀 | 说明 | 示例 |
|-----|------|------|
| `_test` | 测试版本 | pkg_utils_test |
| `_prod` | 生产版本 | deploy_service_prod |
| `_bak` | 备份版本 | compile_module_bak |
| `_v<版本号>` | 版本标识 | test_smoke_v1 |
| `smoke` | 烟雾测试 | test_api_smoke |
| `perf` | 性能测试 | test_benchmark_perf |

### 3.2 命名示例

```yaml
# ✓ 合法命名
pkg_common_compile
test_smoke_v1
llt_memory_check
deploy_online_prod
compile_core_module_test

# ✗ 非法命名
PkgCompile           # 未使用下划线
pkg                  # 缺少任务名称
1st_test             # 以数字开头
pkg-my-task          # 使用连字符
PKG_COMMON_UTILS     # 使用大写字母
```

### 3.3 节点路径规则

**完整路径格式：**
```
CB.tasks.<任务类型>.<完整节点名>
```

**路径示例：**
```yaml
CB.tasks.pkg.pkg_common_compile
CB.tasks.test.test_smoke_v1
CB.tasks.llt.llt_memory_check
CB.tasks.compile.compile_core_module
```

## 四、节点内容映射规则

### 4.1 通用配置项

所有节点类型必须包含的通用配置项：

```yaml
<节点名>:
  version: cann              # 版本号
  enabled: true             # 是否启用
  timeout: 3600             # 超时时间（秒）
```

**配置项说明：**
| 配置项 | 类型 | 默认值 | 说明 |
|-------|------|--------|------|
| `version` | string | `cann` | 版本标识 |
| `enabled` | boolean | `true` | 是否启用该任务 |
| `timeout` | integer | `3600` | 超时时间（秒），不同类型默认值不同 |

### 4.2 各类型节点规则

#### 4.2.1 pkg 类型节点

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
- `target`: 打包目标列表（.so/.a文件）

**典型示例：**
```yaml
pkg_common_utils:
  version: cann
  enabled: true
  timeout: 3600
  type: pkg
  target:
    - libcommon.so
    - libcommon.a
  env:
    BUILD_TYPE: release
```

#### 4.2.2 compile 类型节点

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

**典型示例：**
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

#### 4.2.3 test 类型节点

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

**典型示例：**
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

#### 4.2.4 llt 类型节点

**任务名特征：** 以 `llt_` 开头

**默认内容模板：**
```yaml
llt_<任务名>:
  version: cann
  enabled: true
  timeout: 2400
  type: llt
  test_suite:
    - <测试套件1>
  dependency:
    - <依赖任务>
  ut_check: true
```

**必需配置项：**
- `type`: 必须为 `llt`
- `test_suite`: 测试套件列表

**典型示例：**
```yaml
llt_memory_check:
  version: cann
  enabled: true
  timeout: 2400
  type: llt
  test_suite:
    - memory_leak_test
    - memory_corruption_test
  dependency:
    - compile_core_module
  ut_check: true
```

#### 4.2.5 deploy 类型节点

**任务名特征：** 以 `deploy_` 开头

**默认内容模板：**
```yaml
deploy_<任务名>:
  version: cann
  enabled: true
  timeout: 1800
  type: deploy
  target:
    - <部署目标1>
  env:
    DEPLOY_ENV: prod
```

**必需配置项：**
- `type`: 必须为 `deploy`
- `target`: 部署目标列表

**典型示例：**
```yaml
deploy_online_prod:
  version: cann
  enabled: true
  timeout: 1800
  type: deploy
  target:
    - online_server
  env:
    DEPLOY_ENV: production
    ROLLBACK_ENABLED: true
```

### 4.3 智能内容推断规则

create_node.py 脚本根据任务名自动推断节点类型和内容：

| 任务名前缀 | 推断类型 | 默认timeout | 默认必需配置项 |
|-----------|---------|------------|---------------|
| `pkg_` | pkg | 3600 | type, target |
| `compile_` | compile | 7200 | type, source |
| `test_` | test | 1800 | type, case |
| `llt_` | llt | 2400 | type, test_suite |
| `deploy_` | deploy | 1800 | type, target |
| 其他 | misc | 3600 | type |

## 五、核心脚本使用

### 5.1 create_node.py - 节点构造

#### 5.1.1 基本用法

```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py [选项]
```

#### 5.1.2 参数说明

| 参数 | 简写 | 说明 | 必需 | 示例 |
|-----|------|------|------|------|
| `--node-name` | `-n` | 节点名称 | ✓ | `-n pkg_common_utils` |
| `--file` | `-f` | YAML文件路径 | ✗ | `-f ci_config/rule.yaml` |
| `--dry-run` | | 预览模式，不实际写入 | ✗ | `--dry-run` |
| `--help` | `-h` | 显示帮助信息 | ✗ | `-h` |

**参数说明：**
- `-n/--node-name`（必需）：节点名称，必须符合命名规范
- `-f/--file`（可选）：目标YAML文件路径，不指定则输出到标准输出
- `--dry-run`（可选）：预览模式，显示将要生成的节点内容但不写入文件

#### 5.1.3 使用示例

**示例1：输出到标准输出**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py -n pkg_common_utils
```

输出：
```yaml
pkg_common_utils:
  version: cann
  enabled: true
  timeout: 3600
  type: pkg
  target:
    - libcommon_utils.so
    - libcommon_utils.a
```

**示例2：直接写入YAML文件**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py \
  -n test_api_smoke -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml
```

**示例3：预览模式**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py \
  -n compile_core -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --dry-run
```

### 5.2 analyze_yaml_structure.py - 结构分析

#### 5.2.1 基本用法

```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py [选项]
```

#### 5.2.2 参数说明

| 参数 | 简写 | 说明 | 必需 | 示例 |
|-----|------|------|------|------|
| `--file` | `-f` | YAML文件路径 | ✓ | `-f ci_config/rule.yaml` |
| `--type-distribution` | | 显示节点类型分布 | ✗ | `--type-distribution` |
| `--check-naming` | | 检查命名规范 | ✗ | `--check-naming` |
| `--check-config` | | 检查配置完整性 | ✗ | `--check-config` |
| `--extract-rules` | | 提取内容映射规则 | ✗ | `--extract-rules` |
| `--summary` | | 显示摘要信息 | ✗ | `--summary` |
| `--help` | `-h` | 显示帮助信息 | ✗ | `-h` |

**分析模式说明：**

1. **默认模式**：基本结构分析
2. `--type-distribution`：显示各类型节点数量和占比
3. `--check-naming`：检查所有节点命名是否符合规范
4. `--check-config`：检查节点配置项是否完整
5. `--extract-rules`：提取节点名称与内容的映射规则
6. `--summary`：显示文件摘要信息

#### 5.2.3 使用示例

**示例1：基本分析**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml
```

**示例2：类型分布分析**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --type-distribution
```

输出：
```
节点类型分布报告：
=================
pkg:     15 (30%)
compile: 12 (24%)
test:    10 (20%)
llt:      8 (16%)
deploy:   5 (10%)
总计:    50 个节点
```

**示例3：命名规范检查**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --check-naming
```

输出：
```
命名规范检查报告：
==================
✓ 符合规范的节点: 48 个
✗ 不符合规范的节点: 2 个

不规范节点详情：
1. PkgCompile (CB.tasks.pkg)
   - 问题: 使用了大写字母
   - 建议: pkg_compile

2. 1st_test (CB.tasks.test)
   - 问题: 以数字开头
   - 建议: first_test
```

**示例4：配置完整性检查**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --check-config
```

输出：
```
配置完整性检查报告：
====================
pkg类型节点:
  ✓ pkg_common_utils: 配置完整
  ✗ pkg_missing_target: 缺少必需配置项 [target]
  ...

test类型节点:
  ✓ test_api_smoke: 配置完整
  ✗ test_no_cases: 缺少必需配置项 [case]
  ...
```

**示例5：提取映射规则**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --extract-rules
```

输出：
```
节点内容映射规则：
================
pkg类型节点规则:
  - 通用配置项: version, enabled, timeout
  - 必需配置项: type, target
  - 典型配置项: env

compile类型节点规则:
  - 通用配置项: version, enabled, timeout
  - 必需配置项: type, source
  - 典型配置项: include, flags
  ...
```

**示例6：组合使用多种分析模式**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml \
  --type-distribution \
  --check-naming \
  --summary
```

## 六、与其他技能配合

### 6.1 与 edit-yaml 技能配合

**典型工作流程：**

1. **使用 new_yaml_node 生成节点模板**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py \
  -n pkg_new_feature > node_template.yaml
```

2. **编辑节点内容（可选）**
```bash
# 手动调整或使用脚本修改
vi node_template.yaml
```

3. **使用 edit-yaml 技能添加节点**
```bash
# 方式1：使用add_node.py从文件添加
python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/add_node.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml \
  -p CB.tasks.pkg \
  --content-file node_template.yaml

# 方式2：使用add_node.py直接指定内容
python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/add_node.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml \
  -p CB.tasks.pkg \
  -n "pkg_new_feature: {version: cann, enabled: true, timeout: 3600, type: pkg}"
```

4. **验证和修复格式**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/fix_non_standard_yaml.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml
```

### 6.2 完整工作流示例

**场景：批量创建多个新节点**

```bash
# 步骤1：创建节点模板
for nodename in pkg_utils compile_core test_api; do
  python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py \
    -n $nodename > ${nodename}_template.yaml
done

# 步骤2：使用edit-yaml添加到YAML文件
python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/add_node.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml \
  -p CB.tasks.pkg \
  --content-file pkg_utils_template.yaml

python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/add_node.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml \
  -p CB.tasks.compile \
  --content-file compile_core_template.yaml

python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/add_node.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml \
  -p CB.tasks.test \
  --content-file test_api_template.yaml

# 步骤3：验证YAML格式
python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/fix_non_standard_yaml.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml

# 步骤4：分析新添加的节点
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml --check-naming --check-config
```

## 七、最佳实践

### 7.1 节点命名建议

**推荐做法：**
- ✓ 使用描述性的任务名称，见名知意
  - `pkg_common_utils` ✓
  - `test_api_smoke` ✓
  - `compile_core_module` ✓

- ✓ 使用下划线分隔单词
  - `pkg_common_utils` ✓
  - `test_api_function_v2` ✓

- ✓ 添加有意义的后缀
  - `test_smoke` ✓
  - `deploy_prod` ✓
  - `compile_test` ✓

**避免做法：**
- ✗ 使用模糊的名称
  - `pkg_task1` ✗
  - `test_abc` ✗

- ✗ 使用过长的名称
  - `pkg_common_utilities_for_computation_module` ✗ (太长)

- ✗ 混用命名风格
  - `PkgCommonUtils` ✗
  - `pkg-common-utils` ✗

### 7.2 内容配置建议

**推荐做法：**
- ✓ 确保所有必需配置项都存在
- ✓ 使用合理的timeout值（根据任务类型调整）
- ✓ 设置明确的enabled状态
- ✓ 提供完整的依赖关系

**示例：**
```yaml
test_api_integration:
  version: cann
  enabled: true          # 明确启用状态
  timeout: 3600         # 合理的超时时间（1小时）
  type: test
  case:
    - test_api_001
    - test_api_002
  dependency:
    - compile_api        # 明确的依赖关系
    - pkg_lib_depend     # 明确的依赖关系
  ut_check: true
```

**避免做法：**
- ✗ 缺少必需配置项
  ```yaml
  test_api_bad:
    version: cann
    # 缺少 type, case 等必需配置
  ```

- ✗ timeout设置不合理
  ```yaml
  compile_large_module:
    timeout: 60  # 1分钟对于编译任务太短
  ```

- ✗ 依赖关系不明确
  ```yaml
  test_api:
    # 缺少 dependency 配置
  ```

### 7.3 批量操作建议

**批量创建节点：**
```bash
# 1. 创建节点列表文件
cat > nodes_list.txt << EOF
pkg_common_utils
pkg_core_tools
compile_base_module
test_api_smoke
test_unit_basic
EOF

# 2. 批量生成节点
while read nodename; do
  python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py \
    -n "$nodename" -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml
done < nodes_list.txt
```

**批量验证节点：**
```bash
# 一次运行多种分析
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f $CLAUDE_PROJECT_DIR/ci_config/rule.yaml \
  --check-naming \
  --check-config \
  --type-distribution \
  --summary
```

## 八、常见问题（FAQ）

### Q1：如何为已存在的节点类型添加新的映射规则？

**答：** 修改 `create_node.py` 脚本中的 `TYPE_TEMPLATES` 字典：

```python
TYPE_TEMPLATES = {
    'pkg': {
        'timeout': 3600,
        'type': 'pkg',
        'target': ['<target_name>.so', '<target_name>.a']
    },
    # 添加新的类型
    'custom_type': {
        'timeout': 1800,
        'type': 'custom_type',
        'custom_field': ['<default_value>']
    }
}
```

### Q2：节点构造后如何验证正确性？

**答：** 使用三种验证方法：

1. **YAML语法验证**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/fix_non_standard_yaml.py \
  -f ci_config/rule.yaml
```

2. **命名规范检查**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f ci_config/rule.yaml --check-naming
```

3. **配置完整性检查**
```bash
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f ci_config/rule.yaml --check-config
```

### Q3：如何处理不符合命名规范的现有节点？

**答：** 使用 edit-yaml 技能的 modify_node_name.py：

```bash
# 1. 先分析找出不规范的节点
python $CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py \
  -f ci_config/rule.yaml --check-naming

# 2. 使用modify_node_name.py批量重命名
python $CLAUDE_PROJECT_DIR/.claude/skills/edit-yaml/scripts/modify_node_name.py \
  -f ci_config/rule.yaml \
  -m "PkgCompile=pkg_compile"
```

### Q4：智能推断规则是否可以自定义？

**答：** 可以，修改 `create_node.py` 中的以下部分：

1. **修改类型推断规则**
```python
def get_type_from_nodename(nodename: str) -> str:
    # 添加自定义前缀规则
    if nodename.startswith('custom_'):
        return 'custom'
    # ... 其他规则
```

2. **修改内容模板**
```python
TYPE_TEMPLATES = {
    # 修改或添加类型模板
}
```

3. **修改必需配置项**
```python
REQUIRED_FIELDS = {
    'custom': ['type', 'custom_field'],
    # ...
}
```

### Q5：为什么有些节点被识别为 misc 类型？

**答：** 当节点名称不符合任何已知类型前缀（pkg/compile/test/llt/deploy）时，会被识别为 `misc` 类型。解决方法：

1. **使用标准前缀**
```bash
# 不推荐
misc_mytasks

# 推荐
pkg_mytasks  # 如果是打包任务
compile_mytasks  # 如果是编译任务
```

2. **或添加自定义类型**
修改 `create_node.py` 中的类型识别规则。

### Q6：如何修改必需配置项的定义？

**答：** 修改 `create_node.py` 中的 `REQUIRED_FIELDS` 字典：

```python
REQUIRED_FIELDS = {
    'pkg': ['type', 'target'],
    'compile': ['type', 'source'],
    'test': ['type', 'case'],
    # 添加或修改必需字段
    'custom_type': ['type', 'custom_field1', 'custom_field2']
}
```

### Q7：如何添加新的节点类型？

**答：** 三个步骤：

1. **在 TYPE_TEMPLATES 中添加模板**
```python
TYPE_TEMPLATES = {
    # ... 现有类型
    'new_type': {
        'timeout': 1800,
        'type': 'new_type',
        'field1': ['<default_value1>'],
        'field2': ['<default_value2>']
    }
}
```

2. **在 REQUIRED_FIELDS 中添加必需字段**
```python
REQUIRED_FIELDS = {
    # ... 现有类型
    'new_type': ['type', 'field1']
}
```

3. **在 get_type_from_nodename 中添加识别规则**
```python
def get_type_from_nodename(nodename: str) -> str:
    if nodename.startswith('newtype_'):
        return 'new_type'
    # ... 其他规则
```

## 九、附录

### 9.1 依赖要求

**Python 版本：**
- Python 3.6 或更高版本

**Python 模块：**
```bash
# 必需模块
pyyaml                   # YAML解析

# 可选模块
typing                   # 类型提示（Python 3.5+内置）
```

**安装依赖：**
```bash
pip install pyyaml
```

### 9.2 注意事项

1. **编码问题**
   - 确保YAML文件使用UTF-8编码
   - 避免使用BOM（Byte Order Mark）

2. **节点识别**
   - 节点名称必须符合命名规范
   - 节点路径必须存在（如 CB.tasks.pkg）

3. **类型推断**
   - 基于节点名前缀自动推断类型
   - 无法识别的类型默认为 misc

4. **必需配置项**
   - 每种类型有特定的必需配置项
   - 缺少必需配置项会在验证时报错

5. **文件写入**
   - 直接写入YAML文件会覆盖原文件
   - 建议先使用 --dry-run 预览
   - 操作前建议备份原文件

### 9.3 维护信息

**脚本位置：**
- `$CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/create_node.py`
- `$CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/scripts/analyze_yaml_structure.py`

**技能文档：**
- `$CLAUDE_PROJECT_DIR/.claude/skills/new_yaml_node/SKILL.md`

**维护者：**
- Claude Code Agent System

**更新频率：**
- 根据实际使用情况和需求更新

**相关技能：**
- **edit-yaml**：YAML文件编辑技能
- **git-code-process**：代码版本管理技能

**反馈渠道：**
- 如有问题或建议，请通过项目issue反馈
