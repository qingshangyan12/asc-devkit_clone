# AllToAllV MC2 CCU ST

## 1. 当前文件

```text
utils/src/ccu/ccu_all_to_all_v_mesh1d_adapter.*
utils/src/ccu/ccu_kfc_server_adapter.*
utils/src/hccl_verifier/semantics_check/ccu_all_to_all_v_semantics_checker.*
utils/src/hccl_verifier/semantics_check/ccu_all_to_all_v_mc2_checker.*
testcase/mc2_ccu/host/all_to_all_v_mc2_host_testcase.cpp
testcase/mc2_ccu/device/all_to_all_v_kernel_device_testcase.cpp
testcase/mc2_ccu/device/all_to_all_v_kfc_device_testcase.cpp
```

ST 直接编译真实 AllToAllV 和 KFC server kernel。adapter 只做函数 ABI 和 taskArg 桥接。

adapter 的四个实际函数职责：

| 函数 | 职责 |
|---|---|
| `CaptureAllToAllVMesh1DKernel` | 创建 10 个 LoadArg Variable、配置 Rank/Channel、调用真实 product kernel 捕获 Program |
| `PrepareAllToAllVTaskArgs` | 普通 Kernel Launch 的 10 项 ABI 唯一出口 |
| `BuildAllToAllVKfcTaskArgs` | 将 message/startup token 映射成同一 10 项 ABI |
| `CaptureKfcServerKernel` | 构造 server kernelArg 并捕获真实 KFC server Program |

10 项映射是：

```text
arg 0 input:                 config.inputAddress       <- message[1]
arg 1 output:                config.outputAddress      <- message[2]
arg 2 token:                 config.token              <- startup.token
arg 3 sourceBaseOffset:      config.sourceBaseOffset   <- message[3]
arg 4 destinationBaseOffset: config.destinationBaseOffset <- message[4]
arg 5 parameterAddress:      config.parameterAddress   <- message[9]
arg 6..9 goSize[0..3]:       config.goSize[0..3]       <- message[5..8]
```

修改任一 LoadArg/message 字段时同时更新三个 adapter 函数、`CheckTaskArgs/CheckMessage` 和普通/KFC 等价性 case。只调整 kernel 内部 primitive 次序时不改 adapter。

## 2. peer 语义

逐 `(src,dst)` 检查：

```text
sendCount[src][dst] == recvCount[dst][src]
srcBytes = sendDispl[src][dst] * typeSize .. + sendCount * typeSize
dstBytes = recvDispl[dst][src] * typeSize .. + recvCount * typeSize
```

覆盖非对称 count/displacement、零 peer、尾块、非连续区域和支持的 in-place 路径。

## 3. 分层检查

- 公共 `ccu_checker`：Program、Variable/Address/Channel/Event 资源和控制流引用；当前不验证完整 Mission/LoopEngine 并发。
- AllToAllV semantics：LoadArg、peer 表、操作方向、同步和最终 V 布局。
- 公共 `ccu_kfc_checker`：队列和状态生命周期。
- AllToAllV MC2 checker：消息字段、taskArg 映射、peer 表和结果。

`CcuAllToAllVSemanticsChecker` 必须按 `CheckRegistration -> CheckTaskArgs -> CaptureRuntimeExpectation -> RuntimeWorld::Launch -> CheckRuntime` 使用。expectation 必须在 Launch 前由初始 memory/config 独立生成。

## 4. 修改规则

算法原语顺序变化但 ABI 不变：改 checker/testcase，不改 adapter。TaskArg 或 peer 表变化：同时改 adapter、两个 Device testcase 和对应 checker。KFC 协议变化：再改 proxy/sim world/common KFC checker。

当前只交付 `CcuAlltoallVMesh1D`。普通 AllToAll selector/executor/kernel 和其他 collective 不在此次交付范围，也不需要添加它们的拒绝反例；框架保留未来接入点。
