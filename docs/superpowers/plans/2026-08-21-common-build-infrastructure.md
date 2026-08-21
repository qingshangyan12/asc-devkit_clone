# 公共构建基础能力实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**目标：** 将资源打包、注册和 Kernel 常量化编译中重复的进程执行、临时目录所有权和通用文件操作收敛到 `tools/build/common`，并保持三个模块现有行为不变。

**架构：** 公共层新增无业务语义的 `ProcessExecutor` 和 `TemporaryDirectory`，并复用已有 `FileUtils`。打包、注册和常量化编译继续保留自己的流程类、日志上下文和错误码映射，只把操作系统交互委托给公共层。

**技术栈：** C++11 公共源码、C++17 ACLRTC 调用方、POSIX `posix_spawn`/`poll`/`waitid`/`waitpid`、Boost.Filesystem、GoogleTest、MockCPP、CMake。

---

## 文件结构

- 新建 `tools/build/common/process_executor.h/.cpp`：无 Shell 的进程执行、环境构造、输出捕获、超时和进程组回收。
- 新建 `tools/build/common/temporary_directory.h/.cpp`：唯一目录创建、已有目录接管、移动所有权、立即清理和保留。
- 删除 `tools/build/common/process_runner.h/.cpp`：其能力由 `ProcessExecutor` 完整替代，不保留兼容接口。
- 删除 `tools/build/asc_compile_exporter/build_workspace.h/.cpp`：其能力由 `TemporaryDirectory` 替代。
- 修改 `tools/build/common/file_utils.cpp` 的调用方，不增加只服务单一流程的高层 API。
- 修改 `tools/aclrtc/specialization/compilation_plan_executor.cpp`：仅保留命令超时策略、诊断和 ACL 错误映射。
- 修改 `tools/aclrtc/specialization/kernel_compilation_workspace.h/.cpp`：使用公共目录所有权和已有文件工具。
- 修改 `tools/aclrtc/specialization/resource_registry.h/.cpp`：使用公共目录所有权管理临时根目录及单次物化目录。
- 修改 `tools/build/asc_compile_exporter/manifest_bundle_compiler.cpp`：使用公共进程执行和临时目录。
- 修改两个生产目标和两个 UT 目标的 CMake 源文件列表。

### 任务 1：记录基线行为

**文件：**
- 测试：`tests/tools/utils/asc_compile_exporter/test_process_runner.cpp`
- 测试：`tests/tools/utils/asc_compile_exporter/test_build_workspace.cpp`
- 测试：`tests/tools/aclrtc/test_compilation_plan_executor.cpp`
- 测试：`tests/tools/aclrtc/test_resource_registry.cpp`
- 测试：`tests/tools/aclrtc/test_kernel_compilation_workspace.cpp`

- [ ] **步骤 1：构建并运行现有打包测试**

执行：

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter -j8
```

预期：目标构建成功，现有 `ProcessRunner`、`BuildWorkspace`、Manifest 打包和发布测试全部通过。

- [ ] **步骤 2：构建并运行现有 ACLRTC 测试**

执行：

```bash
cmake --build build --target ascendc_ut_aclrtc -j8
```

预期：目标构建成功，CMake 当前发现的 ACLRTC 测试全部通过。

- [ ] **步骤 3：记录与环境无关的失败**

如果构建目录缺少依赖，执行：

```bash
cmake -S . -B build -DASCEND_CANN_PACKAGE_PATH=/usr/local/Ascend/cann-9.2.0
```

然后重新执行步骤 1、2。仅记录缺失工具链或头文件等环境问题，不修改生产代码绕过环境失败。

### 任务 2：新增公共 ProcessExecutor

**文件：**
- 新建：`tools/build/common/process_executor.h`
- 新建：`tools/build/common/process_executor.cpp`
- 新建：`tests/tools/utils/asc_compile_exporter/test_process_executor.cpp`
- 修改：`tests/tools/utils/CMakeLists.txt`

- [ ] **步骤 1：编写公共接口行为测试**

测试必须直接覆盖公共类型，不经过 pack 或 specialization：

```cpp
ProcessExecutionRequest request;
request.arguments = {"/usr/bin/printf", "0123456789"};
request.outputMode = ProcessOutputMode::CaptureCombined;
request.executionTimeout = std::chrono::seconds(1);
request.terminationGracePeriod = std::chrono::milliseconds(20);
request.capturedOutputByteLimit = 4U;

const ProcessExecutionResult result = ProcessExecutor::Execute(request);
EXPECT_TRUE(result.HasSuccessfulExit());
EXPECT_EQ(result.capturedOutput, "0123");
EXPECT_TRUE(result.capturedOutputTruncated);
```

同时增加以下用例：

- 空 argv 和非正超时返回 `InfrastructureFailure`；
- 参数中的 `$(...)` 保持普通字符串，不执行 Shell；
- 环境变量覆盖和移除生效；
- 非 0 退出、信号退出、exec 失败分别返回明确终止类型和 code；
- 超时后结束忽略 SIGTERM 的后代进程；
- `Inherit` 模式不创建捕获输出；
- 输出持续产生时仍按墙钟时间超时；
- 子进程 leader 先退出时仍等待输出管道关闭。

- [ ] **步骤 2：运行测试并确认 RED**

执行：

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter -j8
```

预期：编译失败，提示 `process_executor.h` 或 `ProcessExecutor` 尚不存在。

- [ ] **步骤 3：定义 C++11 公共接口**

在 `process_executor.h` 中定义：

```cpp
enum class ProcessOutputMode : uint32_t {
    Inherit,
    CaptureCombined,
};

enum class ProcessTermination : uint32_t {
    InfrastructureFailure,
    Exited,
    Signaled,
    TimedOut,
};

struct ProcessExecutionRequest {
    std::vector<std::string> arguments;
    std::vector<std::pair<std::string, std::string>> environmentOverrides;
    std::vector<std::string> removedEnvironmentVariables;
    ProcessOutputMode outputMode{ProcessOutputMode::Inherit};
    std::chrono::milliseconds executionTimeout{0};
    std::chrono::milliseconds terminationGracePeriod{0};
    uint64_t capturedOutputByteLimit{0U};
};

struct ProcessExecutionResult {
    ProcessTermination termination{ProcessTermination::InfrastructureFailure};
    int32_t terminationCode{0};
    bool capturedOutputTruncated{false};
    std::chrono::milliseconds elapsedTime{0};
    std::string capturedOutput;

    bool HasSuccessfulExit() const noexcept;
    const char* GetTerminationName() const noexcept;
};

class ProcessExecutor final {
public:
    ProcessExecutor() = delete;
    static ProcessExecutionResult Execute(const ProcessExecutionRequest& request);
};
```

- [ ] **步骤 4：实现最小公共执行能力**

从现有 `compilation_plan_executor.cpp` 移动并领域中立化以下实现：

- `ScopedFileDescriptor`；
- `CommandSpawnConfiguration`；
- 非阻塞 CLOEXEC 输出管道；
- 有界输出读取与 `poll` 等待；
- `waitid(..., WNOWAIT)` 观察 leader，最终 `waitpid` 回收；
- 进程组 SIGTERM、宽限期和 SIGKILL；
- `posix_spawn` argv/envp 构造。

环境数组在父进程中从 `::environ` 生成快照，按名称移除 `removedEnvironmentVariables`，再覆盖
`environmentOverrides`。不得在 fork 后调用 `setenv`，不得调用 `/bin/sh -c`。

- [ ] **步骤 5：运行公共执行器测试并确认 GREEN**

执行：

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter -j8
```

预期：新增 `ProcessExecutor` 测试全部通过，原打包测试仍通过。

- [ ] **步骤 6：提交公共执行器**

```bash
git add tools/build/common/process_executor.* tests/tools/utils/asc_compile_exporter/test_process_executor.cpp \
  tests/tools/utils/CMakeLists.txt
git commit -m "refactor(build): add common process executor"
```

### 任务 3：迁移 Kernel 常量化编译命令执行

**文件：**
- 修改：`tools/aclrtc/specialization/compilation_plan_executor.cpp`
- 修改：`tools/aclrtc/specialization/kernel_specialization_types.h`
- 修改：`tools/aclrtc/specialization/kernel_specialization_diagnostics.h`
- 修改：`tools/aclrtc/specialization/kernel_specialization_diagnostics.cpp`
- 修改：`tools/aclrtc/CMakeLists.txt`
- 修改：`tests/tools/aclrtc/CMakeLists.txt`
- 修改：`tests/tools/aclrtc/test_compilation_plan_executor.cpp`

- [ ] **步骤 1：将 specialization 测试改为断言公共结果类型**

删除 `CompilationProcessTermination` 和 `CompilationProcessResult` 的类型断言，改为：

```cpp
static_assert(
    std::is_same<std::underlying_type_t<ascendc::ProcessTermination>, uint32_t>::value,
    "ProcessTermination must have a stable width");
static_assert(
    std::is_same<decltype(ascendc::ProcessExecutionResult::terminationCode), int32_t>::value,
    "terminationCode must have a stable width");
```

保留现有 compile/link 错误映射、短超时、输出截断、后代进程终止和基础设施失败测试。

- [ ] **步骤 2：运行 ACLRTC 测试并确认 RED**

执行：

```bash
cmake --build build --target ascendc_ut_aclrtc -j8
```

预期：因 specialization 尚未使用公共结果类型而编译失败。

- [ ] **步骤 3：缩减 CompilationPlanExecutor**

删除本文件中的进程、管道、spawn、poll、wait 和 signal 实现。`ExecuteCompilationCommand` 只负责构造：

```cpp
ProcessExecutionRequest request;
request.arguments.push_back(compilationCommand.executablePath.string());
request.arguments.insert(
    request.arguments.end(), compilationCommand.commandArguments.begin(), compilationCommand.commandArguments.end());
request.outputMode = ProcessOutputMode::CaptureCombined;
request.executionTimeout = GetCommandExecutionTimeout(compilationCommand.commandKind);
request.terminationGracePeriod = executionLimits_.terminationGracePeriod;
request.capturedOutputByteLimit = executionLimits_.capturedOutputByteLimit;
processResult = ProcessExecutor::Execute(request);
```

然后保持当前规则：基础设施错误返回 `ACLRTC_ERROR_FAILURE`；正常命令失败按 command kind 映射为
`ACLRTC_ERROR_COMPILATION` 或 `ACLRTC_ERROR_LINKING`；诊断继续记录公共结果中的终止类型、code、耗时和
输出。

- [ ] **步骤 4：运行 ACLRTC 测试并确认 GREEN**

执行：

```bash
cmake --build build --target ascendc_ut_aclrtc -j8
```

预期：全部 ACLRTC 测试通过，超时测试使用毫秒级注入值，不发生真实分钟级等待。

- [ ] **步骤 5：提交 specialization 迁移**

```bash
git add tools/aclrtc/specialization tools/aclrtc/CMakeLists.txt tests/tools/aclrtc
git commit -m "refactor(aclrtc): use common process executor"
```

### 任务 4：迁移打包命令执行并删除 ProcessRunner

**文件：**
- 修改：`tools/build/asc_compile_exporter/manifest_bundle_compiler.cpp`
- 修改：`tools/build/asc_compile_exporter/CMakeLists.txt`
- 修改：`tests/tools/utils/asc_compile_exporter/test_asc_compile_exporter.cpp`
- 删除：`tests/tools/utils/asc_compile_exporter/test_process_runner.cpp`
- 删除：`tools/build/common/process_runner.h`
- 删除：`tools/build/common/process_runner.cpp`
- 修改：`tests/tools/utils/CMakeLists.txt`

- [ ] **步骤 1：增加 pack 环境与输出模式回归测试**

在公共执行器测试中验证以下请求：

```cpp
request.arguments = {"/bin/sh", "-c", "test -z \"${MAKEFLAGS+x}\" && test \"$BUNDLE_CXX\" = expected"};
request.removedEnvironmentVariables = {"MAKEFLAGS", "MFLAGS", "MAKEFILES", "GNUMAKEFLAGS"};
request.environmentOverrides = {{"BUNDLE_CXX", "expected"}};
request.outputMode = ProcessOutputMode::Inherit;
request.executionTimeout = std::chrono::minutes(30);
request.terminationGracePeriod = std::chrono::seconds(5);
```

该测试锁定旧 `ProcessRunner` 对 pack 的环境行为。

- [ ] **步骤 2：运行测试并确认新增断言通过公共层、pack 尚未迁移**

执行：

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter -j8
```

预期：公共测试通过；生产 `ManifestBundleCompiler` 仍引用 `ProcessRunner`。

- [ ] **步骤 3：迁移 ManifestBundleCompiler**

`BuildBundle` 使用 `ProcessExecutionRequest`，保持以下值：

- argv：`make -C <workDir> -f Makefile -B -j<jobs>`；
- 环境覆盖：`ASCENDC_BUNDLE_CXX`、`ASCENDC_BUNDLE_OUTPUT`；
- 环境移除：`MAKEFLAGS`、`MFLAGS`、`MAKEFILES`、`GNUMAKEFLAGS`；
- 输出模式：`Inherit`；
- 执行超时：30 分钟；
- 终止宽限：5 秒。

失败日志补充 `result.GetTerminationName()` 和 `result.terminationCode`，成功条件使用
`result.HasSuccessfulExit()`。

- [ ] **步骤 4：删除旧 ProcessRunner 及其测试**

从生产和 UT CMake 中删除 `process_runner.cpp`，删除头文件、实现和旧测试；端到端测试中的辅助编译命令
改用 `ProcessExecutor`。

- [ ] **步骤 5：运行打包与 ACLRTC 双回归**

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter ascendc_ut_aclrtc -j8
```

预期：两个测试目标全部通过。

- [ ] **步骤 6：提交 pack 迁移**

```bash
git add tools/build/common tools/build/asc_compile_exporter tests/tools/utils
git commit -m "refactor(pack): use common process executor"
```

### 任务 5：新增公共 TemporaryDirectory

**文件：**
- 新建：`tools/build/common/temporary_directory.h`
- 新建：`tools/build/common/temporary_directory.cpp`
- 新建：`tests/tools/utils/asc_compile_exporter/test_temporary_directory.cpp`
- 修改：`tests/tools/utils/CMakeLists.txt`

- [ ] **步骤 1：编写目录所有权测试**

核心测试使用如下接口：

```cpp
TemporaryDirectory directory;
ASSERT_TRUE(directory.CreateUniqueUnder(root_, ".common-test-%%%%%%%%"));
const std::string createdPath = directory.GetPath();
EXPECT_TRUE(FileUtils::IsDirectory(createdPath));

TemporaryDirectory movedDirectory(std::move(directory));
EXPECT_TRUE(directory.GetPath().empty());
EXPECT_EQ(movedDirectory.GetPath(), createdPath);
```

另行覆盖：父目录创建失败、同一对象重复创建、移动赋值、接管已有目录、`RemoveNow`、析构清理、
`RetainAfterDestruction` 和清理失败 warning 路径。

- [ ] **步骤 2：运行测试并确认 RED**

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter -j8
```

预期：编译失败，提示 `TemporaryDirectory` 尚不存在。

- [ ] **步骤 3：实现只允许移动的目录所有者**

接口固定为：

```cpp
class TemporaryDirectory final {
public:
    TemporaryDirectory() = default;
    ~TemporaryDirectory() noexcept;
    TemporaryDirectory(TemporaryDirectory&& other) noexcept;
    TemporaryDirectory& operator=(TemporaryDirectory&& other) noexcept;
    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;

    bool CreateUniqueUnder(const std::string& parentPath, const std::string& namePattern);
    bool AdoptExistingForCleanup(std::string directoryPath);
    bool RemoveNow() noexcept;
    void RetainAfterDestruction() noexcept;
    const std::string& GetPath() const noexcept;
};
```

`CreateUniqueUnder` 使用 `FileUtils::CreateDirectories`、`FileUtils::ResolveDirectory`、
`boost::filesystem::unique_path` 和单次 `create_directory`。不得把 TMPDIR、资源类型或 Manifest 语义放入
公共类。

- [ ] **步骤 4：运行目录测试并确认 GREEN**

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter -j8
```

预期：新增目录测试和原测试全部通过。

- [ ] **步骤 5：提交公共目录能力**

```bash
git add tools/build/common/temporary_directory.* \
  tests/tools/utils/asc_compile_exporter/test_temporary_directory.cpp tests/tools/utils/CMakeLists.txt
git commit -m "refactor(build): add common temporary directory"
```

### 任务 6：迁移 pack、register 和 specialization 的目录所有权

**文件：**
- 修改：`tools/build/asc_compile_exporter/manifest_bundle_compiler.cpp`
- 修改：`tools/build/asc_compile_exporter/CMakeLists.txt`
- 删除：`tools/build/asc_compile_exporter/build_workspace.h`
- 删除：`tools/build/asc_compile_exporter/build_workspace.cpp`
- 删除：`tests/tools/utils/asc_compile_exporter/test_build_workspace.cpp`
- 修改：`tools/aclrtc/specialization/resource_registry.h`
- 修改：`tools/aclrtc/specialization/resource_registry.cpp`
- 修改：`tools/aclrtc/specialization/kernel_compilation_workspace.h`
- 修改：`tools/aclrtc/specialization/kernel_compilation_workspace.cpp`
- 修改：`tools/aclrtc/CMakeLists.txt`
- 修改：`tests/tools/aclrtc/CMakeLists.txt`
- 修改：`tests/tools/aclrtc/test_resource_registry.cpp`
- 修改：`tests/tools/aclrtc/test_kernel_compilation_workspace.cpp`

- [ ] **步骤 1：先将 BuildWorkspace 测试改写为 TemporaryDirectory 调用方测试**

保留现有行为断言：未设置父目录时调用方选择 `$TMPDIR` 或 `/tmp`；默认析构删除；`--keep-temp` 对应
`RetainAfterDestruction`；跨文件系统发布使用目标父目录下的临时目录。

- [ ] **步骤 2：迁移 pack 并删除 BuildWorkspace**

在 `ManifestBundleCompiler::Compile` 和 `PublishBundleAcrossFileSystems` 中直接创建
`TemporaryDirectory`。TMPDIR 默认值选择保留在 pack 调用方：

```cpp
const char* configuredTemporaryRoot = std::getenv("TMPDIR");
const std::string temporaryRoot = configuredTemporaryRoot == nullptr || configuredTemporaryRoot[0] == '\0' ?
                                      "/tmp" : configuredTemporaryRoot;
TemporaryDirectory workspace;
if (!workspace.CreateUniqueUnder(temporaryRoot, ".manifest-generator-%%%%%%%%")) {
    return false;
}
if (request_.keepTemp) {
    workspace.RetainAfterDestruction();
}
```

- [ ] **步骤 3：运行 pack 测试并确认 GREEN**

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter -j8
```

预期：keep-temp 和跨文件系统发布测试全部通过。

- [ ] **步骤 4：迁移 ResourceRegistry 临时根目录**

将 `temporaryRoot_` 改为 `TemporaryDirectory`。首次使用时在调用方读取 TMPDIR，并通过
`CreateUniqueUnder(configuredRoot, "aclrtc-resource-%%%%%%%%")` 创建。开启保存 kernel meta 时调用
`RetainAfterDestruction`。

单次物化目录也使用局部 `TemporaryDirectory`：默认失败时自动清理；成功时调用
`RetainAfterDestruction` 后将路径返回给 `ResourceData`；保存 kernel meta 且物化失败时同样保留目录，
以满足现有失败现场测试。

- [ ] **步骤 5：迁移 KernelCompilationWorkspace 所有权**

用 `TemporaryDirectory ownedWorktree_` 替代 `worktreePath_` 和手写析构清理。构造时通过
`AdoptExistingForCleanup` 接管 Lookup 路径；保留策略调用 `RetainAfterDestruction`；发布前清理调用
`RemoveNow`。`GetWorktreePath` 固定按值返回由 `ownedWorktree_.GetPath()` 构造的
`boost::filesystem::path`，不保留第二份带清理语义的路径成员。

- [ ] **步骤 6：运行 register 和 specialization 测试并确认 GREEN**

```bash
cmake --build build --target ascendc_ut_aclrtc -j8
```

预期：Lookup 唯一路径、默认清理、保存 kernel meta、失败现场保留、发布前删除和删除失败测试全部通过。

- [ ] **步骤 7：提交目录所有权迁移**

```bash
git add tools/build/asc_compile_exporter tools/aclrtc tools/build/common tests/tools
git commit -m "refactor(build): share temporary directory ownership"
```

### 任务 7：复用 FileUtils 并删除剩余重复代码

**文件：**
- 修改：`tools/aclrtc/specialization/kernel_compilation_workspace.cpp`
- 修改：`tools/aclrtc/specialization/kernel_specialization_diagnostics.cpp`
- 修改：`tools/aclrtc/specialization/resource_registry.cpp`
- 测试：`tests/tools/aclrtc/test_kernel_compilation_workspace.cpp`
- 测试：`tests/tools/aclrtc/test_kernel_specialization_diagnostics.cpp`
- 测试：`tests/tools/aclrtc/test_resource_registry.cpp`

- [ ] **步骤 1：增加现有 FileUtils 复用后的行为断言**

保留以下测试：源码文件打开/读取失败、patch 写入/flush/close 失败时不替换原文件、诊断文件写入失败、
物化文件 finalize 失败。测试只断言模块行为，不依赖公共函数内部实现。

- [ ] **步骤 2：迁移已有完全相同的文件操作**

- `ReadSourceFileContents` 改用 `FileUtils::ReadRegularFile`，再将字节数组转换为字符串；
- 输出目录创建改用 `FileUtils::CreateDirectories`；
- patch 和诊断流的 flush/close 检查改用 `FileUtils::FinalizeOutput`；
- 有明确生命周期所有权的目录统一使用 `TemporaryDirectory`；一次性无所有权路径的尽力清理使用
  `FileUtils::RemoveAll`；
- `ResourceRegistry` 已使用的 `FinalizeOutput` 保持不变。

原子 patch、ELF 发布时序和跨文件系统资源包发布仍保留在领域模块，不新增只被一个调用方使用的
`FileUtils` 高层接口。

- [ ] **步骤 3：运行两个模块测试并确认 GREEN**

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter ascendc_ut_aclrtc -j8
```

预期：全部通过。

- [ ] **步骤 4：提交文件操作收敛**

```bash
git add tools/aclrtc/specialization tests/tools/aclrtc
git commit -m "refactor(aclrtc): reuse common file utilities"
```

### 任务 8：清理、格式化与完整回归

**文件：**
- 修改：本计划涉及的全部 C++、头文件和 CMake 文件
- 修改：`docs/superpowers/specs/2026-08-21-common-build-infrastructure-design.md`
- 修改：`docs/superpowers/plans/2026-08-21-common-build-infrastructure.md`

- [ ] **步骤 1：确认旧实现完全删除**

```bash
rg -n "ProcessRunner|BuildWorkspace|SpawnedToolProcess|CleanupPath" tools tests
```

预期：无旧类、旧进程实现或 `CleanupPath` 引用。

- [ ] **步骤 2：运行格式化**

```bash
pre-commit run clang-format --files \
  tools/build/common/process_executor.h tools/build/common/process_executor.cpp \
  tools/build/common/temporary_directory.h tools/build/common/temporary_directory.cpp \
  tools/build/asc_compile_exporter/manifest_bundle_compiler.cpp \
  tools/aclrtc/specialization/compilation_plan_executor.cpp \
  tools/aclrtc/specialization/kernel_compilation_workspace.cpp \
  tools/aclrtc/specialization/resource_registry.cpp
```

预期：通过；若工具改写文件，重新运行相关测试。

- [ ] **步骤 3：构建生产目标**

```bash
cmake --build build --target asc_compile_exporter acl_rtc -j8
```

预期：两个生产目标在 `-Werror` 下构建通过。

- [ ] **步骤 4：运行最终 UT**

```bash
cmake --build build --target ascendc_ut_asc_compile_exporter ascendc_ut_aclrtc -j8
```

预期：全部测试通过，没有真实分钟级等待，没有遗留后代进程。

- [ ] **步骤 5：检查符号、差异和工作区**

```bash
git diff --check
git status --short
nm -D --defined-only build/tools/aclrtc/libacl_rtc.so | rg -v " aclrtc"
```

预期：`git diff --check` 无输出；仅存在本次预期修改和用户原有未跟踪文件；`libacl_rtc.so` 未新增公共导出符号。

- [ ] **步骤 6：提交最终清理**

```bash
git add tools tests docs/superpowers
git commit -m "refactor(build): consolidate shared build infrastructure"
```
