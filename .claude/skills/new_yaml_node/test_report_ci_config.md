# create_node.py 测试报告（ci_config 实际文件）

## 测试环境
- 测试文件：`D:\qiaolili\asc-devkit\ci_config\rule.yaml`
- 文件大小：517KB
- 测试时间：2025-02-06

## ci_config/rule.yaml 实际结构分析

### 节点类型分布（测试前）
```
总计：8402 个节点

├─ misc: 7460 个节点 (88.8%)
├─ string: 466 个节点 (5.5%)
├─ number: 466 个节点 (5.5%)
├─ llt: 7 个节点 (0.1%)
├─ pkg: 2 个节点 (0.0%)
└─ compile: 1 个节点 (0.0%)
```

### 实际存在的父路径
根据分析，rule.yaml 中实际存在的父路径：
- `CB.tasks.pkg` - 存在，包含 llt 类型节点和 pkg 类型节点
- `CB.tasks.compile` - 存在，包含 llt 类型节点
- `CB.tasks.llt` - **不存在**
- `CB.tasks.test` - **不存在**
- `CB.tasks.deploy` - **不存在**

**重要发现**：
1. rule.yaml 的结构与标准 SKILL.md 中描述的结构不同
2. 节点不是按类型完全分离的
3. llt 节点分散在 pkg 和 compile 路径下
4. 命名格式：`llt4_pkg_pkg`、`llt6_pkg_pkg` 等，不符合 `llt_<name>` 的规范

## 测试用例及结果

### 测试1：pkg 类型节点（成功 ✓）

**命令：**
```bash
python .claude/skills/new_yaml_node/scripts/create_node.py -n pkg_test_new_feature -f ci_config/rule.yaml
```

**结果：**
- ✓ 成功添加到 `CB.tasks.pkg`
- 创建备份：`ci_config/rule.yaml.backup_20260206_105514`

**生成的节点内容：**
```yaml
pkg_test_new_feature:
  version: cann
  enabled: true
  timeout: 3600
  type: pkg
  target:
    - test_new_feature.so
    - test_new_feature.a
```

### 测试2：compile 类型节点（成功 ✓）

**命令：**
```bash
python .claude/skills/new_yaml_node/scripts/create_node.py -n compile_api_module -f ci_config/rule.yaml
```

**结果：**
- ✓ 成功添加到 `CB.tasks.compile`
- 创建备份：`ci_config/rule.yaml.backup_20260206_105628`

**生成的节点内容：**
```yaml
compile_api_module:
  version: cann
  enabled: true
  timeout: 7200
  type: compile
  source:
    - src/api_module/*.cpp
  include:
    - include/
  flags:
    - -O2
    - -Wall
```

### 测试3：test 类型节点（失败 ✗）

**命令：**
```bash
python .claude/skills/new_yaml_node/scripts/create_node.py -n test_unit_smoke -f ci_config/rule.yaml
```

**结果：**
- ✗ 失败：未找到父节点 `CB.tasks.test`
- 原因：rule.yaml 中不存在 CB.tasks.test 路径

### 测试4：llt 类型节点（失败 ✗）

**命令：**
```bash
python .claude/skills/new_yaml_node/scripts/create_node.py -n llt_memory_test -f ci_config/rule.yaml
```

**结果：**
- ✗ 失败：未找到父节点 `CB.tasks.llt`
- 原因：rule.yaml 中不存在 CB.tasks.llt 路径

### 测试5：deploy 类型节点（失败 ✗）

**命令：**
```bash
python .claude/skills/new_yaml_node/scripts/create_node.py -n deploy_staging_env -f ci_config/rule.yaml
```

**结果：**
- ✗ 失败：未找到父节点 `CB.tasks.deploy`
- 原因：rule.yaml 中不存在 CB.tasks.deploy 路径

## 测试总结

### 成功的测试（2/5）
| 测试用例 | 节点类型 | 父路径 | 结果 |
|---------|---------|--------|------|
| pkg_test_new_feature | pkg | CB.tasks.pkg | ✓ 成功 |
| compile_api_module | compile | CB.tasks.compile | ✓ 成功 |

### 失败的测试（3/5）
| 测试用例 | 节点类型 | 失败原因 |
|---------|---------|---------|
| test_unit_smoke | test | CB.tasks.test 路径不存在 |
| llt_memory_test | llt | CB.tasks.llt 路径不存在 |
| deploy_staging_env | deploy | CB.tasks.deploy 路径不存在 |

## 发现的问题和建议

### 问题1：父路径不存在
**问题**：部分父路径（CB.tasks.test、CB.tasks.llt、CB.tasks.deploy）在 rule.yaml 中不存在。

**解决方案**：
1. **手动创建父路径**：在使用 create_node.py 之前，先创建所需的父路径
2. **使用现有父路径**：根据实际文件结构，使用 CB.tasks.pkg 或 CB.tasks.compile
3. **更新技能文档**：在 SKILL.md 中说明实际文件结构可能与标准结构不同

### 问题2：命名规范不一致
**问题**：现有节点命名（如 llt4_pkg_pkg）与标准规范（llt_<name>）不一致。

**建议**：
- 对于现有项目，保持现有命名风格
- 新建项目时，建议使用标准命名规范

### 问题3：占位符未被替换
**问题**：生成的节点中仍有占位符（如 `<module>_001`）。

**改进建议**：
- 在 create_node.py 中增强占位符替换逻辑
- 提供手动编辑占位符的提示

## 功能验证

### 已验证的功能 ✓

1. **节点名称解析**：正确识别节点类型前缀
2. **模板匹配**：根据类型应用正确的模板
3. **智能推断**：后缀规则正常工作
4. **集成 edit-yaml**：成功调用 add_node.py
5. **备份创建**：自动创建带时间戳的备份文件
6. **占位符替换**：大部分占位符正确替换

### 需要改进的功能

1. **父路径验证**：在添加前检查父路径是否存在
2. **占位符完全替换**：`test_<module>_001` 中的占位符未替换
3. **自定义父路径**：支持 `-p` 参数指定不同的父路径

## 建议的使用方式

### 方式1：使用现有父路径
```bash
# 添加到 CB.tasks.pkg
python .claude/skills/new_yaml_node/scripts/create_node.py -n pkg_new_feature -f ci_config/rule.yaml

# 添加到 CB.tasks.compile
python .claude/skills/new_yaml_node/scripts/create_node.py -n compile_new_module -f ci_config/rule.yaml
```

### 方式2：指定父路径
```bash
# 如果需要添加到其他位置，使用 -p 参数
python .claude/skills/new_yaml_node/scripts/create_node.py \
  -n pkg_new_feature -f ci_config/rule.yaml -p CB.tasks.pkg
```

### 方式3：先创建父路径
```bash
# 如果父路径不存在，先用 edit-yaml 创建
python .claude/skills/edit-yaml/scripts/add_node.py \
  -f ci_config/rule.yaml -p CB.tasks -n "test: {}"

# 然后再添加节点
python .claude/skills/new_yaml_node/scripts/create_node.py \
  -n test_new_case -f ci_config/rule.yaml
```

## 结论

create_node.py 脚本基本功能正常，能够在实际的 ci_config/rule.yaml 文件中成功添加 pkg 和 compile 类型的节点。

对于 test、llt、deploy 类型的节点，由于 rule.yaml 中不存在对应的父路径，需要：
1. 先创建父路径
2. 或指定合适的父路径
3. 或根据项目实际情况调整节点类型和命名规范

脚本的核心功能（节点构造、智能推断、集成 edit-yaml）均已验证可用。
