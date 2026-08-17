# CCU Register、Program 与 TaskArg 校验

## 1. 产品生命周期

```text
RegisterStart(instance)
 -> Register(kernelFunc, kernelArgs)
 -> RegisterEnd(instance)
 -> KernelHandle
 -> Launch(handle, taskArgs)
 -> Mission
```

注册期 kernelArgs 决定 Program 结构；运行期 taskArgs 给 `LoadArg` 绑定地址、token、Rank、长度和 peer 参数。taskArgs 的顺序、位宽和语义是 ABI。

## 2. 当前 ST 实现边界

当前 Device ST 通过 collective 薄 adapter 类型安全地调用真实 kernel，primitive proxy 捕获结构化 Program，再由 `RuntimeWorld` 绑定 taskArgs 并解释执行。

当前 `hcomm_ccu_stub.cc` 的普通 Register/Launch 尚不是完整 backend：不能假定它会执行无类型 `kernelFunc`、保存 `KernelHandle -> Program` 或启动 RuntimeWorld。只断言返回成功会产生假通过。

## 3. 校验层级

- 公共 Program：对象引用、资源类型、Event/Channel、IF/WHILE 和 GroupCopy 描述关系；当前没有完整 Mission/LoopEngine 所有权模型。
- collective Program：固定 LoadArg、peer 顺序、操作和同步语义。
- 运行时 ABI：taskArgs 无缺失/交换/截断，普通与 KFC builder 一致。
- 结果：解析 GSA/address/`goSize.addrOffset` 后的访问范围和最终数据来源。

逐 collective adapter 的固定职责是：

- `CaptureXxxKernel(void *)`：创建与产品 LoadArg 同下标的 Variable，配置 Rank/Channel，调用真实 kernel 捕获 Program。
- `PrepareXxxTaskArgs(config)`：按 LoadArg 顺序生成普通 Launch ABI。
- `BuildXxxKfcTaskArgs(message, startup, out)`：把 KFC 字段映射成同一 ABI。

adapter 不生成 peer 循环、Write/Reduce、GroupCopy 或同步。产品内部原语变化而函数签名/LoadArg 不变时，改 collective Checker 和 testcase，不改 adapter。

若 taskArgs 被分成多个 13-slot `TaskParam`，同时检查完整 argId、逻辑参数组和槽内位置。当前模型不能仅凭这种分组宣称已模拟真实多 Mission/SQE 并发。

## 4. 长期 backend

通用 backend 应在 Register 时执行受支持 callable、原子保存 handle/program，在 Launch 时验证归属、绑定参数并创建 `MissionInstance`。产品 kernel ABI 统一后，删除 adapter 中可通用化的 Capture/普通 taskArg 逻辑；KFC 消息映射仍可由 collective builder 负责。

## 5. 硬件边界

Host Program 不验证真实资源 ID、Translator 指令编码、驱动装载、物理并发和性能。报告只描述逻辑 Program/ABI/仿真结果。
