# MC2 CCU ST 专项

## 1. 当前结构

```text
testcase/mc2_ccu/
├── host/all_to_all_v_mc2_host_testcase.cpp
└── device/
    ├── all_to_all_v_kernel_device_testcase.cpp
    ├── all_to_all_v_kfc_device_testcase.cpp
    └── shim/
```

目标是 `ascendc_ut_adv_api_hccl_ccu_mc2_host` 和 `ascendc_ut_adv_api_hccl_ccu_mc2_device`。旧 direct 目录不再存在。

## 2. 三校验域

- Host：真实 `hccl_mc2.cc`/`hccl_alloc_ctx_res.cc` 的资源、tiling、cache 和错误传播。
- Kernel Device：真实 collective kernel 的 Program、taskArg、同步和结果。
- KFC Device：真实 server kernel 的消息、队列、dispatch、完成状态和结果。

当前 AllToAllV 最近基线是 9/8/10，共 27 case。执行前用 `--gtest_list_tests` 重新确认。

## 3. adapter 边界

`ccu_all_to_all_v_mesh1d_adapter` 仅调用具体产品 kernel、准备普通 taskArgs、将 KFC 消息映射到同一 ABI。禁止复制数据面算法。

当前不能直接用普通 Host API 消除 adapter，因为 `hcomm_ccu_stub.cc` 尚未执行任意 `kernelFunc`、保存 `KernelHandle -> Program`、绑定 taskArgs 或 Launch，且产品函数签名未形成统一 callable ABI。

## 4. 验证重点

- Host 成功之外必须断言资源产物。
- Kernel 成功之外必须断言 Program 与最终数据。
- KFC 必须断言当前 `FREE/READY/COMPLETED/FINALIZED` 槽状态、opId、start/completion CKE、taskArg 和结果。
- 普通 kernel 与 KFC builder 必须生成同一 collective ABI。

## 5. 长期演进

先实现通用 Register/Launch backend，再把结构化 Program 降低为 Task DAG 做多 Mission/多 LoopEngine 冲突检查。PR 2273 的普通 API 集成和 DAG 映射可作参考，但不能替代 MC2/KFC 模型。
