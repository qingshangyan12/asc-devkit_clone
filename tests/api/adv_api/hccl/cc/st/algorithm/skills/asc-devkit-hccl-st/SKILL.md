---
name: asc-devkit-hccl-st
description: asc-devkit 仓 HCCL 集合通信算法 ST 的开发、迁移、adapter/Checker 设计和运行手册。任务涉及 AICPU Host展开与Task DAG、CCU Program/RuntimeWorld、MC2 Host/Kernel/KFC三校验域、sim_world、hccl_proxy、semantics_check、AllGather/ReduceScatter/AllToAllV、CCU Capture/TaskArg/KFC builder、缺失符号或单case/全量回归时使用。
---

# asc-devkit HCCL ST

## 目标

使用本 skill 在用户指定 asc-devkit worktree 的 `tests/api/adv_api/hccl/cc/st/algorithm` 中：

- 理解 AICPU `SimTaskQueue -> DAG` 与 CCU `Program -> RuntimeWorld/KFC` 两条证据链。
- 迁移或新增集合通信算法用例。
- 把 Stub、Proxy、Simulator 和 Checker 放到正确目录。
- 区分公共校验与 collective 专用语义校验。
- 编译并可靠运行全量、suite 或单个 GTest case。
- 准确说明 Host 仿真能力边界，不把逻辑 IR 校验描述成真实微码或硬件验证。

## 工作前检查

仓库代码和当前 CMake 是最终事实来源。用户可能在 `asc-devkit` 或功能 worktree（例如 `asc-devkit-ccu-kfc`）工作，先从用户指定仓库解析路径，不要固定切到基线仓：

```bash
cd <用户指定的 asc-devkit worktree>
git status --short
sed -n '1,180p' tests/api/adv_api/hccl/cc/st/algorithm/README.md
sed -n '1,140p' tests/api/adv_api/hccl/cc/st/algorithm/CMakeLists.txt
sed -n '1,100p' tests/api/adv_api/hccl/cc/st/algorithm/testcase/CMakeLists.txt
```

不要覆盖工作区已有改动。新增或修改 CCU ST 时还要读取当前算法对应文件；不要假设历史 README 或算法文件仍存在：

```text
utils/src/ccu/ccu_*_adapter.h
utils/src/hccl_proxy/ccu_st_types.h
utils/src/sim_world/ccu_sim_world.h
utils/src/sim_world/ccu_kfc_sim_world.h
utils/src/hccl_verifier/ccu_checker.h
utils/src/hccl_verifier/semantics_check/ccu_*_semantics_checker.h
```

## 按任务加载参考资料

- 分析目录职责、CMake 目标或完整调用链：读 [references/framework-and-flow.md](references/framework-and-flow.md)。
- 新增算法、adapter、原语、场景或 testcase：读 [references/adding-testcases.md](references/adding-testcases.md)，按 `Capture/Prepare/BuildKfcTaskArgs` 职责落函数。
- 判断 Checker 应放哪里、检查哪些内容：读 [references/checker-design.md](references/checker-design.md)。
- 构建、列出用例、运行 suite/单 case、排障：读 [references/running-tests.md](references/running-tests.md)。
- 适配 AllToAllV 普通/MC2 CCU 用例：读 [references/ccu-alltoallv.md](references/ccu-alltoallv.md)。
- 检查 Register、Program 和 TaskArg 两阶段桥接：读 [references/ccu-register-and-ir.md](references/ccu-register-and-ir.md)。
- 设计 MC2 tiling/KFC/消息专项 ST：读 [references/mc2-st.md](references/mc2-st.md)。
- 排查静态库、weak fallback、dlsym 和缺失符号：读 [references/linking-troubleshooting.md](references/linking-troubleshooting.md)。

## 核心分层规则

按所有权放代码，不要把一个算法的整套 ST 堆在 `utils/src/ccu`：

| 目录 | 所有权 |
|---|---|
| `testcase/` | GTest 场景、输入构造、正反例断言 |
| `utils/src/ccu/` | 真实 CCU kernel 的薄 ABI 适配、taskArg/KFC 参数准备 |
| `utils/src/hccl_depends_stub/` | 外部依赖和 `AscendC::ccu` 原语捕获 Stub |
| `utils/src/hccl_proxy/` | HCCL API/注册/Channel/TaskArg 代理及公共 CCU IR |
| `utils/src/sim_world/` | AICPU 虚拟环境/Task 模型；CCU `RuntimeWorld` 与 KFC 控制面解释执行 |
| `utils/src/hccl_verifier/` | 跨 collective 的公共结构和机制 Checker |
| `utils/src/hccl_verifier/semantics_check/` | AllGather、ReduceScatter 等 collective 专用语义 Checker |

公共 `ccu_checker.*` 不得 include 某个算法头，也不应出现 `AllGather`、`ReduceScatter`、固定 XN ID 或固定 TaskArg 下标。算法专用内容放到对应 `ccu_<collective>_semantics_checker.*`。

## 实施顺序

1. 明确测试的是 AICPU Task 编排，还是 CCU 的 Host、Kernel Device、KFC Device 校验域。
2. 找到产品算法、selector、executor、template、kernel 和参数准备代码。
3. AICPU 画出 `HCOMM -> TaskStub -> TaskQueue -> DAG`；CCU 列出 `LoadArg -> 普通 taskArgs -> KFC message/startup` ABI 表。
4. 新 CCU collective 依次实现 `CaptureXxxKernel`、`PrepareXxxTaskArgs`、`BuildXxxKfcTaskArgs`，再实现 Checker 和三类 testcase；内部原语变化但 ABI 不变时不要改 adapter。
5. 为正确输入添加正例，为本次算法的错误 IR、参数、资源或协议添加定向反例；不要扩展未交付原语的拒绝反例。
6. 更新对应 CMake 源文件列表和 include 路径。
7. 先增量编译，再直接运行测试二进制。
8. 执行 `git diff --check`，搜索旧接口和错误归属。
9. 汇报通过数量，并明确未覆盖的真实物理资源、Translator、驱动和硬件路径。

## 完成标准

- 代码位于正确 ST 模块，而非仅能链接通过。
- 公共 Checker 与 collective 语义 Checker 解耦。
- 普通 `PrepareXxxTaskArgs` 与 KFC builder 生成同一 ABI。
- Runtime expectation 在 Launch 前独立生成，不从 simulator 输出反推。
- 正例验证行为，反例证明 Checker 确实能拒绝错误。
- 新增源文件已进入正确静态库：模型能力进入 `hccl-model-proxy`，校验进入 `hccl-verifier`。
- 直接执行 GTest 二进制取得明确的 `[ PASSED ]` 统计。
- `git diff --check` 无错误，且没有误改用户的其他文件。

## 当前 ST 基线

当前实现包括 AICPU 与 MC2 CCU：

- `ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC`：AllGather 15、ReduceScatter 22，共 37 case。
- `ascendc_ut_adv_api_hccl_ccu_mc2_host`：MC2 Host 资源路径。
- `ascendc_ut_adv_api_hccl_ccu_mc2_device`：普通 Kernel Device 与 KFC Device。
- 最近一次 AllToAllV 基线为 Host 9、Kernel 8、KFC 10，共 27 case。

当前源码合计基线为 64 case。

数字会随代码变化。汇报前必须用 `--gtest_list_tests` 和实际运行结果确认；AICPU/其他算法基线也从当前目标读取，不复述历史固定数字。
