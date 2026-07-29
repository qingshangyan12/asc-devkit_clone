---
name: mc2-ccu-integration
description: 为 MC2 高阶 API 接入 CCU 集合通信算法及 KFC server 调度链的函数级开发和测试指南。任务涉及 MC2 tiling、CcuKfcServerKernel、消息/队列/CKE、collective adapter的Capture/Prepare/BuildKfcTaskArgs、Program/RuntimeWorld、Host/Kernel/KFC三校验域、mc2_client、PR4261迁移或MC2 CCU ST时使用。
---

# MC2 CCU Integration

## 目标

在不复制数据面算法主体的前提下，把普通 HCCL CCU 算法接入 MC2 融合算子调用链，并分别验证 Host 资源准备、KFC 消息/调度、CCU 参数桥接和最终集合通信结果。

## 任务路由

- 理解普通 HCCL、MC2、KFC server 和 CCU 的层级：读 [references/execution-model.md](references/execution-model.md)。
- 判断普通 Kernel、KFC Server Kernel 与公共算法函数如何复用：读 [references/kernel-api-and-reuse.md](references/kernel-api-and-reuse.md)。
- 迁移 PR4261、新增 MC2 CCU collective 或修改 adapter/TaskArg：读 [references/integration-workflow.md](references/integration-workflow.md)，按函数清单实施。
- 设计和运行 MC2 CCU ST：读 [references/st-and-validation.md](references/st-and-validation.md)。

## 开始前

1. 明确用户调用的是普通 HCCL Host API，还是 MC2 AICore 高阶 API。
2. 明确执行模式（AICPU/CCU）和启动方式（Host直驱/KFC）是两个正交维度。
3. 先定位 tiling ABI、KFC 消息结构、队列槽、CCU kernel handle 和 taskArgs 构造。
4. 对照 PR4261 时只选择当前算法所需模块，不整包复制无关算子和兼容层。

## 标准接入流程

1. 定义 MC2 对外 tiling 和消息契约，包括版本、长度、Rank和算法字段。
2. 在 Host 资源路径创建通信域、Channel、CCU instance 和注册后的 kernel handle。
3. 新增 `XxxKernelConfig/XxxLaunchConfig`，实现 `CaptureXxxKernel` 调用真实产品 kernel。
4. 实现 `PrepareXxxTaskArgs` 作为普通启动 ABI 唯一出口，实现 `BuildXxxKfcTaskArgs` 把 message/startup 映射成同一 ABI。
5. KFC Server Kernel 只负责取消息、校验状态、通过 registration builder 装配参数、dispatch 和完成回写。
6. 使用新 `ccu::` API 注册算法，不回退到老 `CcuRep` 算法表面。
7. 扩展 Stub、Proxy、SimWorld 和 Checker，分别覆盖 Host、Kernel Device 和 KFC Device 三个校验域。
8. 运行两个 MC2 CCU 目标；共享算法主体变化时同时回归普通 Kernel 与 KFC Device 用例。

## 关键约束

- 普通数据面使用真实 `CcuAlltoallVMesh1DKernel`；当前 Device ST 由 `CaptureAllToAllVMesh1DKernel` 类型安全捕获。
- MC2/KFC 使用公共真实 `CcuKfcServerKernel` 处理常驻消息，再通过 registration 中的 `BuildAllToAllVKfcTaskArgs` 驱动同一个 collective Program。
- KFC server adapter 不应为每个 collective 复制数据面；差异落在 opId/opType、collective kernel handle 和 taskArg builder。
- KFC 层不得重新实现一套 collective 算法。
- `libmc2_compat.so` 或 weak fallback 只解决可选依赖兼容，不能代替真实 MC2 功能验证。
- Host ST 不代表真实 AICore tiling、设备常驻循环、驱动装载或硬件通信时序。

## 完成标准

- 普通 HCCL 与 MC2 两条入口及调用者清楚。
- tiling -> 消息 -> taskArgs/参数表 -> LoadArg 的字段映射可逐项核对。
- `Capture/Prepare/BuildKfcTaskArgs` 三个函数职责清楚，普通与 KFC taskArgs 有等价性断言。
- 队列槽、状态机、token、Mission和完成通知没有生命周期空洞。
- 普通 Kernel 与 KFC Kernel 的共享/适配代码边界清晰。
- ST 同时覆盖 Host 资源、合法消息、非法状态/长度/Rank、参数错位和最终结果。
- Host 与 Device 两个目标无回归；普通 HCCL CCU 全链若尚未接入，不得宣称已覆盖。
