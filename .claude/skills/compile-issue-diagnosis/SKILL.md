---
name: compile-issue-diagnosis
description: 编译问题定位技能，自动解析编译日志，定位出问题的target和.o文件，分析编译错误原因，给出修复建议。
---

# 编译问题定位技能

## 一、技能概述

本技能（compile-issue-diagnosis）为华为 CANN CI/CD 体系提供**自动化编译问题诊断**能力，通过智能解析编译日志，快速定位编译失败的根本原因，并提供精准的修复建议。

**核心价值：**
- **快速定位**：自动从海量日志中提取关键错误信息
- **精准诊断**：通过分析编译命令和错误信息，定位根本原因
- **智能建议**：基于错误类型和历史经验，提供修复方案
- **效率提升**：减少人工排查时间，提高问题解决效率
- **知识积累**：记录常见问题和解决方案，形成知识库

## 二、重要操作原则

### 2.1 ⚠️ 强制要求

**关键原则：编译问题诊断必须基于完整的编译日志**

- **日志完整性**：必须提供完整的编译日志，包含从开始到失败的全部输出
- **上下文保留**：保留错误前后的编译输出，避免截断关键信息
- **多轮分析**：复杂问题可能需要多次分析，逐步深入
- **人工验证**：AI 提供的建议需要人工验证后执行
- **知识反馈**：将有效的解决方案反馈到知识库

### 2.2 核心能力

| 能力类型 | 功能说明 | 典型使用场景 |
|---------|---------|-------------|
| **日志解析** | 解析编译日志，提取错误信息 | 编译失败后快速定位问题 |
| **Target 定位** | 识别失败的编译 target | 确定问题模块 |
| **源码定位** | 查找 .o 文件对应的源代码 | 定位问题文件 |
| **命令提取** | 提取完整的编译命令 | 复现和分析 |
| **错误分析** | 分析错误类型和原因 | 理解问题本质 |
| **修复建议** | 提供针对性的修复方案 | 快速解决问题 |

## 三、诊断流程

### 3.1 完整诊断流程图

```
【步骤1：获取编译日志】
    ├─ 读取编译日志文件
    ├─ 验证日志完整性
    └─ 提取编译失败标志

【步骤2：解析日志信息】
    ├─ 识别失败的 target
    ├─ 定位出问题的 .o 文件
    ├─ 提取编译命令
    └─ 获取错误信息

【步骤3：定位源代码】
    ├─ 根据 target 查找代码仓
    ├─ 找到对应的源文件
    ├─ 分析编译逻辑
    └─ 理解依赖关系

【步骤4：分析错误原因】
    ├─ 分类错误类型
    │   ├─ 语法错误
    │   ├─ 链接错误
    │   ├─ 依赖错误
    │   ├─ 类型错误
    │   └─ 宏定义错误
    ├─ 分析错误上下文
    └─ 定位根本原因

【步骤5：给出修复建议】
    ├─ 根据错误类型提供方案
    ├─ 参考历史解决方案
    ├─ 提供代码修改示例
    └─ 标注注意事项
```

### 3.2 错误类型分类

| 错误类型 | 特征 | 常见原因 | 典型案例 |
|---------|------|---------|---------|
| **语法错误** | `error: expected ';'` | 代码语法问题 | 缺少分号、括号不匹配 |
| **链接错误** | `undefined reference` | 符号未定义 | 缺少链接库、函数未实现 |
| **依赖错误** | `fatal error: xxx.h: No such file` | 头文件缺失 | 路径错误、模块未编译 |
| **类型错误** | `cannot convert 'A*' to 'B*'` | 类型不匹配 | 指针类型转换错误 |
| **宏定义错误** | `error: macro "XXX" passed 3 arguments` | 宏参数不匹配 | 宏定义与使用不一致 |
| **模板错误** | `template argument deduction` | 模板推导失败 | 模板参数不匹配 |

## 四、使用方法

### 4.1 基本用法

```bash
# 分析编译日志文件
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log

# 分析指定 target 的编译失败
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log -t libbase.so

# 详细模式（显示完整诊断过程）
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log -v

# 只定位问题源文件
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/find_target_source.py -t libbase.so

# 分析编译错误并给出建议
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/analyze_compile_error.py -f compile.log
```

### 4.2 完整诊断流程示例

**场景：编译 libbase.so 失败，出现 undefined reference 错误**

```bash
# 步骤1：解析编译日志，定位问题
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log -v

# 输出：
# [√] 解析编译日志: compile.log
# [X] 发现编译错误
#
# 问题概览:
#   Target: libbase.so (Shared Library)
#   出错文件: base_utils.o:245
#   错误类型: 链接错误
#   错误信息: undefined reference to 'get_config_value'

# 步骤2：查找失败 target 的源代码
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/find_target_source.py -t libbase.so

# 输出：
# [√] 查找 Target: libbase.so
#   [*] 查找源文件...
#   [√] 找到 15 个源文件
#       - src/base/utils.cpp [C++]
#       - src/base/config.cpp [C++]
#       ...
#   [*] 查找编译配置...
#   [√] 找到 1 个配置文件
#       - Android.mk: base/Android.mk

# 步骤3：分析错误原因，获取修复建议
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/analyze_compile_error.py -f compile.log

# 输出：
# ============================================================
# 编译错误分析报告
# ============================================================
#
# [1] 链接错误-符号未定义
#     根本原因: 符号 'get_config_value' 未定义
#     修复建议:
#       - 添加链接库: target_link_libraries(base PRIVATE config)
#       - 检查函数实现是否存在
#     示例:
#       grep -r 'get_config_value' --include='*.cpp'
```

### 4.3 典型案例

#### 案例1：链接错误

**错误信息：**
```
undefined reference to 'get_config_value(char const*)'
```

**诊断步骤：**
```bash
# 1. 解析日志
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log

# 2. 查找源代码
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/find_target_source.py -t libbase.so

# 3. 分析错误原因
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/analyze_compile_error.py -f compile.log

# 输出：
# 根本原因: 符号 'get_config_value' 未定义，可能缺少链接库
# 修复建议:
#   - 添加链接库: target_link_libraries(base PRIVATE config)
```

**修复方案：**
```cmake
# 在 CMakeLists.txt 中添加
target_link_libraries(base PRIVATE config)
```

或在 Android.mk 中：
```makefile
LOCAL_SHARED_LIBRARIES += libconfig
```

---

#### 案例2：头文件缺失

**错误信息：**
```
fatal error: 'utils/helper.h' file not found
```

**诊断步骤：**
```bash
# 1. 分析日志
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log

# 2. 查找头文件
find . -name "helper.h"

# 3. 分析原因
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/analyze_compile_error.py -f compile.log

# 输出：
# 根本原因: 文件 'utils/helper.h' 不存在或路径错误
# 修复建议:
#   - 检查 include 路径配置
#   - 验证模块是否已编译
```

**修复方案：**
```cmake
# 在 CMakeLists.txt 中添加 include 路径
include_directories(${CMAKE_SOURCE_DIR}/utils)
```

或在 Android.mk 中：
```makefile
LOCAL_C_INCLUDES += $(LOCAL_PATH)/utils
```

### 4.4 命令行参数

#### parse_compile_log.py

| 参数 | 说明 | 必需 |
|------|------|------|
| `-f, --file` | 编译日志文件路径 | 是 |
| `-t, --target` | 指定分析的 target 名称 | 否 |
| `-v, --verbose` | 显示详细诊断过程 | 否 |
| `-o, --output` | 输出诊断报告到文件 | 否 |

#### find_target_source.py

| 参数 | 说明 | 必需 |
|------|------|------|
| `-t, --target` | Target 名称 | 是 |
| `-r, --repo` | 代码仓路径（默认自动查找） | 否 |
| `-b, --branch` | 分支名称（默认 master） | 否 |

#### analyze_compile_error.py

| 参数 | 说明 | 必需 |
|------|------|------|
| `-f, --file` | 编译日志文件路径 | 是 |
| `--error-type` | 指定错误类型进行深入分析 | 否 |
| `--suggest` | 强制生成修复建议 | 否 |

## 五、诊断输出格式

### 5.1 诊断报告结构

```markdown
# 编译问题诊断报告

## 1. 问题概览
- 失败的 Target: libbase.so
- 出错的 .o 文件: base_utils.o
- 错误类型: 链接错误
- 错误代码: undefined reference to 'get_config_value'

## 2. 错误详情
### 编译命令
arm-linux-gnueabihf-g++ -shared -fPIC ... base_utils.cpp -o base_utils.o

### 错误信息
base_utils.o: In function 'Utils::getConfig()':
base_utils.cpp:245: undefined reference to 'get_config_value(char const*)'

### 回溯信息
#0  Utils::getConfig() at base_utils.cpp:245
#1  Base::init() at base.cpp:102

## 3. 源码定位
### 文件位置
代码仓: component/base
源文件: src/base/utils.cpp
函数位置: 第 240-250 行

### 相关代码
```cpp
char* Utils::getConfig(const char* key) {
    // Line 240-250
    return get_config_value(key);  // <-- Line 245: 错误发生处
}
```

## 4. 问题分析
### 错误类型
链接错误 - 符号未定义

### 根本原因
函数 `get_config_value()` 在代码中调用，但未找到实现。
可能原因：
1. 缺少链接库 libconfig.so
2. 函数实现未被编译
3. 链接顺序错误

## 5. 修复建议
### 方案1：添加缺失的链接库（推荐）
在 Android.mk 或 CMakeLists.txt 中添加：
```cmake
target_link_libraries(base PRIVATE config)
```

### 方案2：检查函数实现
确认 get_config_value() 函数是否存在：
```bash
grep -r "get_config_value" --include="*.cpp" --include="*.c"
```

### 方案3：调整链接顺序
将 libconfig 放在 base 之后：
```makefile
LOCAL_SHARED_LIBRARIES := \
    libbase \
    libconfig  # 移到最后
```

## 6. 验证步骤
1. 应用修复方案
2. 清理旧编译产物：make clean
3. 重新编译：make libbase.so
4. 检查编译日志确认错误已解决

## 7. 相关信息
- 编译时间: 2025-02-06 14:30:25
- 编译环境: GCC 9.3.0
- Target 类型: Shared Library
- 依赖模块: libconfig.so, libutils.so
```

### 5.2 简洁输出格式（默认模式）

```bash
$ python parse_compile_log.py -f compile.log

[√] 解析编译日志: compile.log
[X] 发现编译错误

问题概览:
  Target: libbase.so
  出错文件: base_utils.o:245
  错误类型: 链接错误
  错误信息: undefined reference to 'get_config_value'

源码位置:
  代码仓: component/base
  文件: src/base/utils.cpp:245

修复建议:
  添加链接库: target_link_libraries(base PRIVATE config)
```

## 六、子代理配置

### 6.1 子代理说明

编译问题诊断技能配合专用的子代理使用，子代理配置文件位于：
```
.claude/subagents/compile-diagnosis-agent.json
```

### 6.2 子代理能力

| 能力 | 说明 | 使用场景 |
|------|------|---------|
| **日志分析** | 深度分析编译日志，提取隐含信息 | 复杂编译问题 |
| **代码搜索** | 在代码仓中搜索相关代码 | 定位函数定义、查找依赖 |
| **模式匹配** | 识别编译错误模式 | 快速判断错误类型 |
| **知识推理** | 基于历史知识推断问题原因 | 新问题分析 |
| **多轮对话** | 交互式诊断，逐步深入 | 复杂问题排查 |

### 6.3 子代理工作流程

```
用户提交编译日志
    ↓
[子代理] 预处理：验证日志格式
    ↓
[子代理] 解析：提取错误信息
    ↓
[子代理] 定位：查找源代码
    ↓
[子代理] 分析：判断错误类型
    ↓
[子代理] 推理：分析根本原因
    ↓
[子代理] 生成：提供修复建议
    ↓
用户验证方案
    ↓
[可选] 反馈调整建议
```

### 6.4 子代理使用示例

**示例1：链接错误诊断**

```
用户: 编译失败了，undefined reference to 'get_config_value'

Agent: [检测到编译错误，激活 compile-diagnosis 子代理]

[子代理] 开始分析...
[子代理] 步骤1：收集编译日志
[子代理] 步骤2：解析日志信息
  - 识别错误类型：链接错误-符号未定义
  - 提取符号名称：get_config_value
  - 定位出错位置：base_utils.o:245

[子代理] 步骤3：定位源代码
  - 查找 target: libbase.so
  - 定位源文件: src/base/utils.cpp
  - 分析编译配置: base/Android.mk

[子代理] 步骤4：分析错误原因
  - 错误类型：链接错误
  - 根本原因：函数 get_config_value() 未定义
  - 可能原因：缺少链接库 libconfig.so

[子代理] 步骤5：生成修复建议
  - 方案1：添加链接库（推荐）
    在 Android.mk 中添加：LOCAL_SHARED_LIBRARIES += libconfig
    或在 CMakeLists.txt 中添加：target_link_libraries(base PRIVATE config)

  - 方案2：检查函数实现
    验证 get_config_value() 函数是否存在
    grep -r 'get_config_value' --include='*.cpp'

[子代理] 分析完成！
```

**示例2：头文件缺失诊断**

```
用户: fatal error: 'utils/helper.h' file not found

Agent: [检测到编译错误，激活 compile-diagnosis 子代理]

[子代理] 开始分析...
[子代理] 步骤1：分析错误信息
  - 错误类型：致命错误-文件缺失
  - 缺失文件：utils/helper.h

[子代理] 步骤2：定位问题
  - 查找源文件：确认 helper.h 是否存在
  - 检查 include 路径：验证头文件搜索路径配置

[子代理] 步骤3：生成修复建议
  - 方案1：添加 include 路径
    CMakeLists.txt: include_directories(${CMAKE_SOURCE_DIR}/utils)
    Android.mk: LOCAL_C_INCLUDES += $(LOCAL_PATH)/utils

  - 方案2：检查模块编译
    确认 utils 模块是否已编译
    make utils

[子代理] 分析完成！
```

## 七、最佳实践

### 7.1 日志收集

**完整的编译日志应包含：**
```bash
# 推荐方式：重定向完整输出
make libbase.so > compile.log 2>&1

# 或者使用脚本记录
#!/bin/bash
LOG_FILE="compile_$(date +%Y%m%d_%H%M%S).log"
make "$@" 2>&1 | tee $LOG_FILE
```

**日志内容要求：**
- ✓ 完整的编译命令
- ✓ 所有警告和错误信息
- ✓ 编译环境信息（编译器版本、路径等）
- ✓ Target 依赖关系
- ✗ 不要截断输出
- ✗ 不要过滤错误信息

### 7.2 日志收集技巧

```bash
# 完整记录编译日志
make libbase.so > compile_$(date +%Y%m%d_%H%M%S).log 2>&1

# 使用 tee 同时显示和保存
make libbase.so 2>&1 | tee compile.log
```

### 7.3 快速定位技巧

```bash
# 搜索错误关键词
grep -i "error:" compile.log

# 查找 undefined reference
grep "undefined reference" compile.log

# 查找 fatal error
grep "fatal error:" compile.log

# 统计错误数量
grep -c "error:" compile.log
```

### 7.4 问题诊断

**快速诊断步骤：**
1. **提取关键词**：从错误信息中提取关键符号
2. **查找定义**：在代码仓中搜索符号定义
3. **分析依赖**：检查 target 的依赖关系
4. **验证环境**：确认编译环境和工具链

**复杂问题排查：**
```bash
# 步骤1：分析编译日志
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log

# 步骤2：定位源代码
cd <代码仓>
find . -name "<出错的源文件>"

# 步骤3：查找符号定义
grep -r "符号名" --include="*.h" --include="*.cpp"

# 步骤4：分析编译配置
cat Android.mk  # 或 CMakeLists.txt

# 步骤5：查看相关代码
vi <出错的源文件>
```

### 7.5 修复验证

**验证清单：**
- [ ] 修改代码或配置
- [ ] 清理旧编译产物
- [ ] 重新编译
- [ ] 检查错误是否解决
- [ ] 验证功能正常
- [ ] 检查是否引入新问题

**常用验证命令：**
```bash
# 清理编译产物
make clean
rm -rf out/

# 重新编译
make libbase.so -j8

# 查看编译结果
ls -lh out/target/product/<device>/lib/libbase.so
```

### 7.6 知识积累

**记录问题解决方案：**
1. 创建问题记录文档
2. 记录错误特征和解决方案
3. 标注适用场景
4. 更新知识库

**示例记录格式：**
```markdown
## 问题：undefined reference to 'get_config_value'

### 错误特征
- 错误类型：链接错误
- 错误信息：undefined reference to 'get_config_value(char const*)'
- 出现场景：编译 libbase.so 时

### 根本原因
函数 `get_config_value()` 在 libbase 中调用，但未链接 libconfig 库

### 解决方案
在 Android.mk 中添加：
```makefile
LOCAL_SHARED_LIBRARIES += libconfig
```

### 适用场景
- 所有依赖 libconfig 的模块
- 类似的符号未定义错误

### 注意事项
- 链接库顺序：被依赖的库放在后面
- 静态库和动态库混用时的注意事项
```

## 八、常见问题

### Q1：如何获取完整的编译日志？

**回答：**

编译日志需要包含完整的编译输出，包括标准输出和标准错误。

**推荐方法：**
```bash
# 方法1：重定向所有输出
make libbase.so > compile.log 2>&1

# 方法2：使用 tee 同时显示和保存
make libbase.so 2>&1 | tee compile.log

# 方法3：使用脚本
#!/bin/bash
LOG_FILE="compile_$(date +%Y%m%d_%H%M%S).log"
make "$@" 2>&1 | tee $LOG_FILE
```

**注意事项：**
- 不要使用 `| grep error` 过滤日志
- 不要使用 `> /dev/null` 丢弃输出
- 保留编译器版本信息
- 保留完整的命令行

### Q2：诊断不准确怎么办？

**回答：**

诊断结果不准确可能由以下原因导致：

**原因1：日志不完整**
- 确保提供了完整的编译日志
- 检查日志是否被截断
- 包含错误前后的上下文

**原因2：多错误叠加**
- 编译可能存在多个错误
- 先解决第一个错误，重新编译
- 某些错误可能是连锁反应

**原因3：环境问题**
- 检查编译工具链版本
- 确认依赖库是否完整
- 验证环境变量配置

**改进方法：**
1. 使用详细模式：`-v` 参数
2. 指定具体的 target：`-t` 参数
3. 结合子代理多轮分析
4. 人工验证 AI 建议的可行性

### Q3：找不到源代码文件怎么办？

**回答：**

源代码文件找不到可能有以下原因：

**原因1：代码仓路径不正确**
- 确认代码仓路径配置
- 检查是否在正确的分支
- 验证代码是否已同步

**原因2：文件路径映射错误**
- 编译输出中的路径可能是相对路径
- 需要根据编译目录转换路径
- 检查 out/ 目录结构

**解决方法：**
```bash
# 手动查找源文件
find <代码仓> -name "<文件名>"

# 查找所有 .o 文件对应的源文件
find <代码仓> -name "*.cpp" -o -name "*.c"

# 检查编译映射文件
cat out/.../module-info.json
```

### Q4：修复建议不适用怎么办？

**回答：**

AI 提供的修复建议可能不完全适用，需要根据实际情况调整。

**调整方法：**
1. **理解建议的本质**：抓住建议的核心思路
2. **结合实际环境**：考虑项目的具体配置
3. **参考类似问题**：搜索历史问题和解决方案
4. **咨询团队成员**：复杂问题需要集体讨论

**改进建议质量：**
- 提供更详细的上下文信息
- 包含相关的配置文件
- 说明已经尝试的方法
- 提供错误前后的修改内容

### Q6：脚本无法执行怎么办？

**回答：**

脚本没有执行权限时会导致无法直接运行。

**检查方法：**
```bash
# 检查脚本权限
ls -la $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/
```

**解决方法：**
```bash
# 添加执行权限
chmod +x $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/*.py
```

### Q7：如何避免编译错误？

**回答：**

预防编译错误的最佳实践：

**代码编写阶段：**
- 遵循代码规范
- 使用静态分析工具
- 及时编译检查
- 编写单元测试

**提交前检查：**
```bash
# 静态分析
cppcheck src/

# 语法检查
gcc -fsyntax-only *.cpp

# 完整编译
make clean
make -j8

# 运行测试
make test
```

**CI/CD 集成：**
- 预提交编译检查
- 自动化编译验证
- 代码审查检查
- 持续集成编译


**回答：**

脚本没有执行权限时会导致无法直接运行。

**检查方法：**
```bash
# 检查脚本权限
ls -la $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/
```

**解决方法：**
```bash
# 添加执行权限
chmod +x $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/*.py

# 验证权限
ls -la $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/
# 应该看到 -rwxr-xr-x (可执行权限)
```

### Q8：找不到 Python 解释器怎么办？

**回答：**

系统可能未安装 Python 或路径不在 PATH 中。

**检查方法：**
```bash
# 检查 Python 版本
python3 --version
```

**解决方法：**
```bash
# 方法1：使用 python3
python3 $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log

# 方法2：使用完整路径
/usr/bin/python3 $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log
```

### Q9：日志文件编码问题怎么办？

**回答：**

编译日志可能包含非 UTF-8 编码的字符，导致读取错误。

**解决方法：**
```bash
# 方法1：转换编码
iconv -f GBK -t UTF-8 compile.log > compile_utf8.log

# 方法2：脚本已内置编码容错（errors='ignore'）
python $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/parse_compile_log.py -f compile.log

# 方法3：在 Git Bash 中运行（推荐）
# Git Bash 默认使用 UTF-8 编码
```

## 九、维护信息

- **技能名称**：compile-issue-diagnosis
- **技能路径**：`$CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/`
- **脚本位置**：`$CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/`
- **子代理配置**：`$CLAUDE_PROJECT_DIR/.claude/agents/compile-diagnosis-agent.md`
- **文档位置**：`$CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/SKILL.md`
- **更新日期**：2025-02-06

### 脚本文件

| 脚本名称 | 功能说明 |
|---------|---------|
| **parse_compile_log.py** | 解析编译日志，提取错误信息 |
| **find_target_source.py** | 查找 target 对应的源代码文件 |
| **analyze_compile_error.py** | 分析编译错误原因，给出修复建议 |

### 脚本权限

确保所有脚本具有执行权限：

```bash
# 检查权限
ls -la $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/

# 添加执行权限（如果缺失）
chmod +x $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/*.py

# 验证权限
ls -la $CLAUDE_PROJECT_DIR/.claude/skills/compile-issue-diagnosis/scripts/
# 应该看到 -rwxr-xr-x (可执行权限)
```

### 环境要求

- **Python 版本**：Python 3.x
- **依赖模块**：无特殊依赖（仅使用标准库）
- **推荐环境**：Git Bash 或支持 UTF-8 的终端

| 脚本名称 | 功能说明 |
|---------|---------|
| **parse_compile_log.py** | 解析编译日志，提取错误信息 |
| **find_target_source.py** | 查找 target 对应的源代码文件 |
| **analyze_compile_error.py** | 分析编译错误原因，给出修复建议 |

## 十、相关技能

- **edit-yaml**：YAML 配置文件修改技能（修改编译配置）
- **user_require_analysis**：用户需求分析技能
- **git-code-process**：Git 代码处理技能（提交修复代码）

**典型工作流：**
```
编译失败
  → compile-issue-diagnosis（诊断问题）✅ 本技能
  → edit-yaml（修改配置）
  → git-code-process（提交修复）
```
