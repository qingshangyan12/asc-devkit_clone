# 新增用例与算法适配流程

## 1. 改动分类

| 需求 | 主要修改位置 |
|---|---|
| AICPU 已有算法新增场景 | testcase，必要时 collective semantics checker |
| AICPU 新 collective/已有原语 | 真实 selector/executor/template CMake、testcase、semantics checker |
| AICPU/CCU 新公共原语 | stub、proxy/IR、sim world、公共 checker、collective checker、testcase |
| MC2 CCU 新 collective | 薄 adapter、Host/Kernel/KFC 三类 testcase、两个 collective checker、CMake/KFC dispatch |

## 2. AICPU

新增场景覆盖 Rank、拓扑、大小/尾块、in-place、dataType/reduceOp 和 selector 分支。新增 collective 时提供 `Check<Collective>` 入口及最终数据来源规则。

若只使用已有 HCOMM 原语，不新增 `utils/src/aicpu/<collective>_adapter`。新增原语时必须纵向打通任务表示、执行、DAG 边、读写集合和语义。

## 3. MC2 CCU 新 collective

先建立 `argId -> 产品 LoadArg -> 普通 config -> KFC message/startup -> 语义` 表，以及 peer 参数表的 byte stride。再新增：

```text
utils/src/ccu/ccu_<collective>_adapter.{h,cc}
utils/src/hccl_verifier/semantics_check/ccu_<collective>_semantics_checker.{h,cc}
utils/src/hccl_verifier/semantics_check/ccu_<collective>_mc2_checker.{h,cc}
testcase/mc2_ccu/host/<collective>_mc2_host_testcase.cpp
testcase/mc2_ccu/device/<collective>_kernel_device_testcase.cpp
testcase/mc2_ccu/device/<collective>_kfc_device_testcase.cpp
```

adapter 头文件至少定义：

```cpp
struct XxxKernelConfig; // 注册期 Rank/Channel/静态 kernelArg
struct XxxLaunchConfig; // 运行期地址/token/count/offset/peer 信息
Result CaptureXxxKernel(void *kernelArg);
std::vector<uint64_t> PrepareXxxTaskArgs(const XxxLaunchConfig &config);
Result BuildXxxKfcTaskArgs(const CcuKfcMessage &message,
    const CcuKfcStartupArgs &startup, std::vector<uint64_t> &taskArgs);
```

- `CaptureXxxKernel`：校验静态 config，逐 argId 创建 `Variable/LoadArg`，配置 Program Rank/Channel，调用真实产品 kernel。禁止手工补 operation 或复制算法。
- `PrepareXxxTaskArgs`：普通启动的唯一 ABI 展开函数。testcase 不得重复手写 vector。
- `BuildXxxKfcTaskArgs`：校验 identity/finalize/token，把 message/startup 映射成同一 vector。用例必须断言它与普通 Prepare 等价。

collective semantics checker 沿用四阶段：

```text
CheckRegistration
CheckTaskArgs
CaptureRuntimeExpectation  # Launch 前独立生成
CheckRuntime               # completed + output 对比
```

MC2 checker 提供 `CheckMessage`，有参数表时再提供 `CheckPeerTable`。公共 `CcuKfcChecker` 只检查 registration、startup、queue/CKE 和 server Program。

Kernel testcase 建议实现 `BuildKernelConfig -> RegisterAllRanks -> BuildScenario -> VerifyScenario`；KFC testcase 实现 `CaptureKernels -> BuildKfcScenario -> DispatchAndVerify`。CMake 直接编译真实 collective/KFC kernel；adapter 进 Device target，两个 checker 进 `hccl-verifier`。

接入第二种 collective 时，当前 `CaptureKfcServerKernel` 的硬编码 `HCCL_CMD_ALLTOALLV` 必须改为由 server config 提供 opId/opType。继续通过 `MakeKfcRegistration(..., BuildXxxKfcTaskArgs, ...)` 注册 builder，不在 `CcuKfcSimWorld::DispatchOne` 堆算法分支。

## 4. 修改矩阵

| 改动 | 必查/必改 |
|---|---|
| 算法 IR/同步 | `CheckRegistration`、Kernel/KFC testcase；ABI 不变时不改 adapter |
| TaskArg/LoadArg | `CaptureXxxKernel`、`PrepareXxxTaskArgs`、`BuildXxxKfcTaskArgs`、`CheckTaskArgs`、两个 Device testcase |
| peer 表 | `XxxPeerParam`、场景 parameter writer、`CheckPeerTable`、Device testcase |
| 最终数据规则 | `CaptureRuntimeExpectation`、`CheckRuntime`、输出篡改反例 |
| 新 CCU 原语 | `ccu_st_types.h`、`ccu_primitives_stub.*`、`ccu_kernel_proxy.*`、`ccu_sim_world.*`、`ccu_checker.*`、collective checker |
| KFC 协议/opId/队列 | `ccu_kfc_proxy.*`、server adapter、taskArg builder、`ccu_kfc_sim_world.*`、`ccu_kfc_checker.*`、MC2 checker/testcase |
| MC2 tiling/resource/cache | Host testcase；真实 Host 源增加依赖时才改 `mc2_ccu` Stub |
| Mission/Loop 公共模型 | types、proxy、sim world、公共 checker、并发 testcase |

## 5. 用例边界

Host：资源类型/数量、cache key、重复申请、依赖失败和回滚。

Kernel Device：非对称 count/offset、零长度、尾块、in/out-place、Program/LoadArg/同步及最终源 Rank/offset。

KFC Device：消息长度/Rank/opId/状态、队列连续操作/finalize、start/completion CKE、taskArg 映射和最终结果。

只为当前交付 collective 添加专用正例和关键 ABI/协议反例，不为未交付的其他原语堆拒绝反例。
