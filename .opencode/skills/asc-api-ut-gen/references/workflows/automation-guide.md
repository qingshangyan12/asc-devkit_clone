# 自动化验证流程

## 1. UT 生成后的必执行流程

**生成 UT 文件后，必须自动执行以下验证流程：**

```
生成 UT 文件
    │
    ├─ Phase 1: 环境准备
    │   └─ source {CANN_PATH}/set_env.sh
    │
    ├─ Phase 2: 编译验证
    │   ├─ cd {ASC_DEVKIT_PATH}
    │   ├─ bash build.sh --basic_test_one -j8
    │   └─ 检查编译结果
    │       ├─ 成功 → 进入 Phase 3
    │       └─ 失败 → 分析错误 → 修复 UT → 重试 Phase 2
    │
    ├─ Phase 3: 测试执行
    │   ├─ cd {ASC_DEVKIT_PATH}/build/tests/api/basic_api
    │   ├─ ./可执行文件 --gtest_filter="*{API_NAME}*"
    │   └─ 检查测试结果
    │       ├─ 通过 → 进入 Phase 4
    │       └─ 失败 → 分析原因 → 修复 UT → 重试 Phase 2
    │
    └─ Phase 4: 输出报告
        └─ 输出最终验证报告（含 token、耗时、当前覆盖率）
```

---

## 2. 编译验证详细步骤

各 API 场景 guide 不再重复维护编译命令。生成 UT 后统一回到本节，根据 API 类型、目标芯片和本地 `build.sh` 参数选择实际命令。

### 2.1 编译命令

```bash
# Step 5: 编译验证（自动执行）

# 5.1 设置 CANN 环境
source {CANN_PATH}/set_env.sh

# 5.2 进入 asc-devkit 目录
cd {ASC_DEVKIT_PATH}

# 5.3 根据测试类型选择编译命令
# AIV 测试（大部分 Basic API）
bash build.sh --basic_test_one -j8

# Advanced API 测试
bash build.sh --adv_test -j8

# 全部测试
bash build.sh -t -j8
```

### 2.2 编译结果检查

```bash
# 成功标志
# Build succeeded

# 失败处理
# 根据错误信息修复 UT 文件
```

### 2.3 编译错误处理

| 错误类型 | 示例 | 修复方法 |
|---------|------|---------|
| 头文件未找到 | `fatal error: kernel_operator.h: No such file` | 检查 include 路径 |
| 类型不支持 | `static_assert failed: SupportType` | 检查数据类型是否在支持列表 |
| 未定义引用 | `undefined reference to vadd` | 检查架构是否正确设置 |
| 模板参数错误 | `template argument deduction failed` | 检查函数调用参数类型 |
| **参数结构初始化失败** | `no matching constructor for initialization of 'FixpipeParamsV220'` | **使用成员赋值替代花括号初始化** |

---

## 3. 测试执行详细步骤

### 3.1 测试执行命令

```bash
# Step 6: 测试执行（自动执行）

# 6.1 进入测试可执行文件目录
cd {ASC_DEVKIT_PATH}/build/tests/api/basic_api

# 6.2 执行测试（使用 gtest 过滤）
# 格式: ./可执行文件 --gtest_filter="*{API_NAME}*"
./ascendc_ut_basic_api_ascend910B1_AIV --gtest_filter="*Add*"

# 6.3 检查测试结果
# [  PASSED  ] X tests. → 成功
# [  FAILED  ] X tests. → 失败，需分析原因

# 6.4 详细输出模式（调试用）
./ascendc_ut_basic_api_ascend910B1_AIV --gtest_filter="*Add*" --gtest_output=xml:test_result.xml
```

### 3.2 测试失败处理

| 失败类型 | 可能原因 | 修复方法 |
|---------|---------|---------|
| 断言失败 | 期望值与实际值不匹配 | 检查测试逻辑和数据初始化 |
| 段错误 | 内存访问越界 | 检查数据大小和对齐 |
| 超时 | 死循环或性能问题 | 检查循环条件 |
| 设备错误 | NPU 设备状态异常 | 检查 SetGCoreType 设置 |
| **核心转储（Core Dump）** | **Stub 函数参数数量不匹配** | **使用类封装，参考已有成功测试** |
| **测试配置崩溃** | **QuantMode 与类型组合错误** | **查表确认类型组合正确性** |

---

## 4. 自动修复机制

### 4.1 自动修复流程

```
编译/测试失败
    │
    ├─ Step 1: 错误分析
    │   ├─ 解析错误信息
    │   ├─ 定位错误位置
    │   └─ 确定错误类型
    │
    ├─ Step 2: 自动修复
    │   ├─ 类型错误 → 修正数据类型
    │   ├─ 参数错误 → 调整参数配置
    │   ├─ 头文件缺失 → 添加 include
    │   ├─ 约束违反 → 调整测试数据
    │   └─ 结构初始化错误 → 改用成员赋值
    │
    ├─ Step 3: 重新验证
    │   └─ 自动重新编译和测试
    │
    └─ Step 4: 达到最大重试次数
        └─ 输出错误报告，请求用户介入
```

### 4.2 自动修复示例

```
[错误] 编译失败: no matching constructor for initialization of 'FixpipeParamsV220'
[分析] 检测到参数结构体不支持花括号初始化
[修复] 自动改用成员赋值方式
[重试] 重新编译...
[结果] ✓ 编译成功
```

---

## 5. 验证报告模板

生成 UT 后的最终报告必须包含运行指标：

- **Token 消耗**：如果调用环境能提供 LLM usage，记录 prompt / completion / total；如果是本地脚本生成或客户端未暴露 usage，写明 `未提供` 或 `不适用`，不能填 0 冒充真实消耗。
- **耗时**：记录从开始分析/生成到最终报告输出的总耗时；如可拆分，额外记录生成、编译、测试和覆盖率复查耗时。
- **当前覆盖率**：优先记录 `build/cov_report` 中当前 Lines / Functions；若未生成、未刷新或未传入报告路径，必须写明原因，不得声称覆盖率已达标。

```
╔══════════════════════════════════════════════════════════════╗
║              API UT 验证报告                                  ║
╠══════════════════════════════════════════════════════════════╣
║ API 名称:       {API_NAME}                                    ║
║ 架构:           {ARCH} (NPU_ARCH={ARCH_CODE})                 ║
║ 测试文件:       {TEST_FILE_PATH}                              ║
║ 支持数据类型:   {DATA_TYPES}                                  ║
║ Token 消耗:     prompt={PROMPT_TOKENS}, completion={COMPLETION_TOKENS}, total={TOTAL_TOKENS} ║
║ 总耗时:         {TOTAL_ELAPSED}s                               ║
║ 当前覆盖率:     Lines={COVERAGE_LINES}, Functions={COVERAGE_FUNCTIONS} ║
║ 覆盖率来源:     {COVERAGE_SOURCE_OR_UNAVAILABLE_REASON}        ║
╠══════════════════════════════════════════════════════════════╣
║ 编译状态:       [成功/失败]                                    ║
║ 编译命令:       bash build.sh --basic_test_one -j8            ║
║ 编译耗时:       {COMPILE_TIME}s                               ║
╠══════════════════════════════════════════════════════════════╣
║ 测试状态:       [PASSED/FAILED]                               ║
║ 测试命令:       ./可执行文件 --gtest_filter="*{API}*"         ║
║ 测试用例数:     {TEST_COUNT}                                  ║
║ 通过用例数:     {PASSED_COUNT}                                ║
║ 失败用例数:     {FAILED_COUNT}                                ║
║ 测试耗时:       {TEST_TIME}s                                  ║
╠══════════════════════════════════════════════════════════════╣
║ 问题记录:                                                     ║
║ {ISSUES}                                                      ║
╚══════════════════════════════════════════════════════════════╝
```

---

## 6. 自动化执行检查清单

生成 UT 后，**必须按顺序执行**：

### 6.1 编译阶段

- [ ] **Step 5.1**: 设置 CANN 环境
- [ ] **Step 5.2**: 执行编译 `bash build.sh --basic_test_one -j8`
- [ ] **Step 5.3**: 检查编译结果
  - 成功 → 继续 Step 6
  - 失败 → 分析错误 → 修复 UT → 重新编译

### 6.2 测试阶段

- [ ] **Step 6.1**: 执行测试 `./可执行文件 --gtest_filter="*{API}*"`
- [ ] **Step 6.2**: 检查测试结果
  - 通过 → 输出成功报告
  - 失败 → 分析原因 → 修复 UT → 重新编译和测试
- [ ] **Step 6.3**: 重新读取当前覆盖率；若无法读取，记录原因
- [ ] **Step 6.4**: 输出最终验证报告，包含 token 消耗、总耗时和当前覆盖率
