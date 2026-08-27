# MC2 CCU 函数级接入流程

## 1. 先固定 ABI

建立两张表：

```text
argId -> 产品 LoadArg -> 普通 LaunchConfig 字段 -> KFC message/startup 字段 -> 语义
byteOffset -> peer 参数字段 -> Host writer -> kernel Load -> Checker
```

普通 builder、KFC builder、产品 kernel 和 Checker 必须共用同一顺序。超过 13 个 taskArg 时当前 ST 会分 `TaskParam`，但不代表真实多 Mission 并发已被模拟。

## 2. 新增 collective adapter

在 `utils/src/ccu/ccu_xxx_adapter.{h,cc}` 定义：

```cpp
struct XxxKernelConfig; // 注册期 Rank/Channel/静态参数
struct XxxLaunchConfig; // 启动期地址/token/count/offset/peer
Result CaptureXxxKernel(void *kernelArg);
std::vector<uint64_t> PrepareXxxTaskArgs(const XxxLaunchConfig &config);
Result BuildXxxKfcTaskArgs(const CcuKfcMessage &message,
    const CcuKfcStartupArgs &startup, std::vector<uint64_t> &taskArgs);
```

- `CaptureXxxKernel`：校验静态 config，创建与产品 LoadArg 同下标的 Variable，`ConfigureProgram`，调用真实产品 kernel 捕获 Program。
- `PrepareXxxTaskArgs`：普通 Launch ABI 唯一出口。禁止 testcase 手写第二套 vector。
- `BuildXxxKfcTaskArgs`：校验 identity/finalize/startup，把消息映射成同一 ABI。

adapter 不实现 peer 循环、GroupCopy/Reduce、Read/Write、分块或同步。产品内部原语变化但函数签名/LoadArg 不变时，不改 adapter。

## 3. 接入 KFC server

公共 `CcuKfcServerKernel` 负责消息循环，不为每个 collective 复制 server kernel。通过：

```cpp
MakeKfcRegistration(instance, serverHandle, collectiveHandle,
    opId, startup, BuildXxxKfcTaskArgs, registration);
```

注册算法专用 builder。不要在 `CcuKfcSimWorld::DispatchOne` 堆 collective 分支。

当前 `CaptureKfcServerKernel` 把 `OpParam.opType` 写死为 AllToAllV。接入第二种 collective 时，让 `KfcServerKernelConfig` 显式携带当前 Server 绑定的 opType，再填入单个 `OpParam`。产品框架按 primitive/algTag 分别申请 KFC Server 资源，ST 不应自行扩展为多 primitive 共享 Server。

## 4. 新增 Checker

`CcuXxxSemanticsChecker` 使用四阶段接口：

```text
CheckRegistration          # 公共 Program + collective 静态契约
CheckTaskArgs              # 公共 TaskArg + Prepare 精确比较
CaptureRuntimeExpectation  # Launch 前独立计算输出
CheckRuntime               # completed + output 比较
```

`CcuXxxMc2Checker` 提供 `CheckMessage`，有参数表时提供 `CheckPeerTable`。算法 taskArg 下标、消息字段和 peer 布局不得进入公共 `CcuProgramChecker/CcuKfcChecker`。

## 5. 新增三类 testcase

Host：直接调用真实 `hccl_mc2.cc/hccl_alloc_ctx_res.cc`，断言 tiling 分支、OpResCtx、XN/CKE/token、cache 和失败回滚。

Kernel Device 建议函数骨架：

```text
BuildKernelConfig
-> RegisterAllRanks(CaptureXxxKernel)
-> BuildScenario(PrepareXxxTaskArgs)
-> VerifyScenario(CheckRegistration/TaskArgs/Expectation/Launch/Runtime)
```

KFC Device 建议函数骨架：

```text
CaptureKernels(server + collective)
-> BuildKfcScenario(queue/startup/message/registration)
-> DispatchAndVerify(KFC checker -> expectation -> DispatchOne -> runtime -> Wait)
```

必须有普通 `PrepareXxxTaskArgs == BuildXxxKfcTaskArgs` 等价性断言，以及输出篡改后 `CheckRuntime` 失败的反例。只为当前交付 collective 补专用反例。

## 6. CMake

- Host target：加入 `xxx_mc2_host_testcase.cpp`；真实 Host 源有新外部符号时才补 `mc2_ccu` Stub。
- Device target：加入 adapter、普通/KFC testcase 和真实 product collective kernel。
- `hccl-verifier`：加入 `ccu_xxx_semantics_checker.cc` 和 `ccu_xxx_mc2_checker.cc`。
- 复用已有 KFC server kernel，不重复编译算法副本。

## 7. 兼容层和边界

weak/dlsym 只解决可选装载，fallback 返回成功不能作为功能通过证据。当前 Host 模型不验证 Translator 微码、真实资源分配、设备常驻时序、硬件并发和性能。
