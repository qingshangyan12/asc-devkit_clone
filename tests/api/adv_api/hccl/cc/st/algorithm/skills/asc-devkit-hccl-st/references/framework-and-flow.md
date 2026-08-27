# ST 框架与调用流程

## 1. 当前目标

算法 ST 根目录是 `tests/api/adv_api/hccl/cc/st/algorithm`。先读取根、`utils`、`testcase` 三层 CMake，禁止依据历史文档猜目标。

当前主要目标：

| 目标 | 内容 |
|---|---|
| `ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC` | 真实 AllGather/ReduceScatter AICPU selector、executor、template 与 Task/DAG testcase |
| `ascendc_ut_adv_api_hccl_ccu_mc2_host` | 真实 MC2 Host 资源源文件及 Host testcase |
| `ascendc_ut_adv_api_hccl_ccu_mc2_device` | 真实 collective kernel、KFC server kernel、Device testcase |
| `hccl-model-proxy` | Stub、Proxy、SimWorld |
| `hccl-verifier` | DAG/内存/语义及 CCU Checker |

当前源码基线为 AICPU 37 case（AllGather 15、ReduceScatter 22），MC2 CCU 27 case（Host 9、Kernel 8、KFC 10）。执行时重新用 `--gtest_list_tests` 统计。

## 2. AICPU 链路

```text
多Rank GTest
 -> real selector/executor/template
 -> HcommLocalCopy/Read/Write/Reduce/Notify
 -> hccl_proxy/hccl_stub.cc
 -> SimTaskQueue
 -> Task DAG
 -> single-task/memory-conflict/collective semantics checker
```

AICPU adapter 是 HCOMM 原语级通用层。因此新增只使用已有原语的算法不需要逐 collective adapter。

Checker 通常依次检查从流、生成 DAG、单 Task 地址、双边通信图、无序内存冲突和最终符号数据语义。多 Rank 是通信参与者并行，不是多 Mission。

`SimWorld` 只提供拓扑、Rank/NPU、stream、notify 和虚拟内存环境，不逐条执行 Task。`SimTaskQueue` 按 Rank/stream 保存录制日志，`TaskGraphGenerator` 根据同 stream 顺序、Rank 内 local post/wait 和 Rank 间 post/wait 建立 happens-before，Verifier 再做冲突和符号语义检查。

## 3. MC2 CCU 链路

### Host 域

```text
host testcase
 -> real hccl_mc2.cc / hccl_alloc_ctx_res.cc
 -> tiling/commEngine/resource/cache/error rollback assertions
```

`mc2_ccu/cann_host_bridge_stub.cc` 关闭真实 CANN bridge；`hccl_op_common_stub.cc` 只补真实 Host 源的最小链接依赖，不表示完整 selector/executor 已验证。

### Kernel Device 域

```text
device testcase
 -> thin collective adapter
 -> real product CCU kernel
 -> ccu primitive stub/proxy
 -> structured Program
 -> RuntimeWorld(taskArgs, RankMemory)
 -> common + collective checker
```

### KFC Device 域

```text
device testcase
 -> thin server adapter -> real KFC server kernel Program
 -> registration + ccu_kfc_proxy / CcuKfcSimWorld
 -> queue/message/state/dispatch
 -> collective taskArg builder -> same RuntimeWorld/Program
 -> common KFC + MC2 collective checker
```

CCU 结论依赖五层证据：真实产品 Host/kernel、薄捕获桥、Program/registration/queue 模型、RuntimeWorld/KFC 解释执行、公共与 collective Checker。Host、Kernel Device、KFC Device 是三个不同校验域，不能用其中一个成功替代另外两个。

## 4. 目录依赖

```text
testcase
  -> product source + utils/src/ccu thin adapter
  -> hccl_depends_stub
  -> hccl_proxy
  -> sim_world
  -> hccl_verifier
       -> semantics_check
```

- `hccl_proxy` 定义公共表示，不判断 collective 结果。
- `sim_world` 推进状态，不硬编码算法期望值。
- 公共 `ccu_checker` 不依赖 collective adapter。
- 固定 XN、taskArg 下标、peer 布局进入 `semantics_check`。

## 5. 当前边界

当前 CCU 路径能校验结构化 Program、LoadArg/taskArg、Channel/Event、GroupCopy loop 描述、KFC 协议和多 Rank 结果。当前 Program 尚未把 Mission/LoopEngine 作为完整一等运行对象；它也不具备普通 HCCL Host API 到真实 Register/Launch 仿真的完整闭环和通用 `Program -> DAG` 内存冲突后端。

`hcomm_ccu_stub.cc` 若只返回成功，不得据此声明算法已执行。必须断言 Program、状态变化或最终数据。

Host 模型均不验证 Translator 微码、驱动、物理资源容量、硬件时序和性能。
