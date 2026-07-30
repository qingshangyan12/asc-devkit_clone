# MC2 CCU ST 与验证

## 1. 当前三校验域

| 校验域 | 产品代码 | 重点 |
|---|---|---|
| Host | `hccl_mc2.cc`、`hccl_alloc_ctx_res.cc` | tiling、资源、cache、错误回滚 |
| Kernel Device | 真实 collective CCU kernel | Program、LoadArg/taskArg、同步、结果 |
| KFC Device | 真实 KFC server kernel | 消息、队列、dispatch、完成、结果 |

构建目标是 `ascendc_ut_adv_api_hccl_ccu_mc2_host` 和 `ascendc_ut_adv_api_hccl_ccu_mc2_device`。当前 AllToAllV 最近基线为 9/8/10，共 27 case，执行时重新统计。

## 2. 文件落点

```text
testcase/mc2_ccu/host/
testcase/mc2_ccu/device/
utils/src/ccu/                         # 薄 kernel/taskArg adapter
utils/src/hccl_proxy/                  # Program/KFC 公共表示
utils/src/sim_world/                   # CCU/KFC 执行模型
utils/src/hccl_verifier/               # 公共结构和协议
utils/src/hccl_verifier/semantics_check/ # collective 规则
utils/src/mc2_ccu/                     # Host 产品源最小链接桩
```

## 3. 必要用例

Host：资源选择和数量、cache key、重复调用、依赖失败、回滚。

Kernel：用 `BuildKernelConfig -> RegisterAllRanks -> BuildScenario -> VerifyScenario` 组织；覆盖多 Rank 非对称 peer、零长度、尾块、LoadArg/taskArg、Program 同步和最终结果。

KFC：用 `CaptureKernels -> BuildKfcScenario -> DispatchAndVerify` 组织；覆盖合法 dispatch、错误 Rank/opId/状态、队列连续操作、start/completion CKE、64 槽 wrap、finalize、消息到 taskArg 及最终结果。当前槽状态是 `FREE/READY/COMPLETED/FINALIZED`。

## 4. 防止假通过

- Stub 返回成功不算算法执行。
- 必须观察资源对象、Program/operation、状态迁移或最终内存。
- `hccl_op_common_stub.cc` 只是 Host 链接环境，不代表完整 selector/executor。
- KFC 层不能重写一套 collective 数据面。
- `CaptureRuntimeExpectation` 必须在 Launch 前根据初始 input/config 独立生成。
- 必须断言普通 `PrepareXxxTaskArgs` 与 KFC `BuildXxxKfcTaskArgs` 生成同一 ABI。

## 5. 当前 adapter 与长期方向

当前 product kernel 函数签名和 taskArg ABI 随 collective 变化，普通 `hcomm_ccu_stub.cc` 又没有完整 Register/Launch backend，因此 Device ST 使用逐 collective 薄 adapter。

adapter 固定拆成：`CaptureXxxKernel` 调真实 kernel、`PrepareXxxTaskArgs` 生成普通 ABI、`BuildXxxKfcTaskArgs` 映射 KFC ABI。产品 primitive 顺序变化但 LoadArg 不变时，只改 collective Checker/testcase。

长期在统一 callable ABI 后实现 `KernelHandle -> Program` registry 和 Launch 执行，可移除普通 capture/prepare adapter；KFC message-to-taskArg builder 仍是 collective 契约。随后增加 `Program -> DAG` 后端校验多 Mission/多 LoopEngine 内存冲突。

## 6. 能力边界

Host ST 不证明真实 AICore tiling、Translator 微码、驱动装载、物理 Channel、设备常驻时序、硬件性能。报告逐项说明实际覆盖层级。
