# MC2 CCU ST 方案串讲与 HCCL PR 2273 评审

## 1. 文档目的

本文用于团队评审和串讲，重点回答四个问题：

1. PR 2273 的“通用 CCU 原语录制”是什么；
2. 它解决了什么问题，哪些设计值得复用；
3. 为什么它不能直接作为 MC2/KFC CCU 的正式 ST；
4. asc-devkit 当前方案如何工作、仍有哪些缺口、两者长期怎样合并优势。

本文不是要证明 PR 2273 “不可用”。客观结论是：

> PR 2273 是普通 HCCL CCU Host 全链接入 `SimTaskQueue` 的有价值原型，尤其适合作为未来通用 Register/Launch 与 `Program -> DAG` 后端的设计输入；但它没有覆盖 MC2 Host 资源面、KFC 常驻调度和 Mission/taskArg ABI，不能独立承担 MC2/KFC 正式 ST。asc-devkit 当前设计对 MC2/KFC 和 CCU 结构覆盖更强，但普通 HCCL CCU Host 全链与通用 DAG/内存冲突仍需补齐。

## 2. 背景概念

### 2.1 多 Rank 与 Mission 并发

多 Rank 并行调用是测试进程同时驱动多个逻辑通信参与者。每个 Rank 有自己的输入、输出、stream 和通信端点，多 Rank 共同完成一次 collective。

Mission 是 CCU 可启动的程序及其资源作用域。一个 Rank 可以先后或同时启动多条 Mission，一条 Mission 内又可以有多个 LoopGroup/LoopEngine。两者不是同一维度：

```text
collective instance
├── Rank 0
│   ├── Mission A -> LoopGroup 0 -> LoopEngine 0/1
│   └── Mission B
└── Rank 1
    └── Mission C
```

“测试启动了多个 Rank”不能证明“同一 Rank 的多 Mission 资源并发安全”。

### 2.2 Mission/taskArg ABI

ABI 是 Host/KFC 参数生产者与 CCU kernel 参数消费者之间必须稳定一致的二进制契约，包含：

- taskArgs 数量、顺序、位宽和对齐；
- 每个 `LoadArg` Variable 读取哪个下标；
- 地址、token、Rank、长度和 peer 表字段的含义；
- 普通 Launch 与 KFC 消息转换是否生成同一布局。

参数数量正确但顺序错一位，kernel 仍可能运行，却会把长度当地址或把 peer offset 当 Rank，因此必须在 ST 中逐项校验。

### 2.3 Loop、LoopGroup 和 LoopEngine

`Loop` 描述循环体和迭代；`LoopGroup` 组织可克隆或协同的 Loop；LoopEngine 是硬件执行绑定。算法代码通过创建 Loop/LoopGroup、加入 loop、设置 engine/bind 信息和 clone offset 表达并行意图。

源码中的声明顺序不是硬件完成顺序。没有 Event/CKE/wait 或资源独占关系时，不应因为 Host 仿真恰好串行展开，就推断两个 LoopEngine 没有竞争。

### 2.4 GSA offset 与 `goSize.addrOffset`

GSA offset 是地址表达式的一部分，用于从某个 GSA/基地址解析实际数据位置。`goSize` 通常描述 Group 操作的规模和按 group/iteration 推进的地址变化，`goSize.addrOffset` 是该公共 GroupCopy/GroupReduce 操作的地址步进配置。

两者可能共同参与最终地址：GSA offset 解决“从哪个基地址的什么位置开始”，`goSize.addrOffset` 解决“多次/group 展开时下一块怎样推进”。mem2mem 模式仍可能使用这些参数；它不因没有远端 Channel 就消失。Checker 应比较解析后的访问区间，而不是只比较某个裸 offset。

## 3. 两条产品执行链

### 3.1 普通 HCCL CCU

```text
HcclAlltoallv
 -> selector/executor/template
 -> HcommCcuKernelRegisterStart
 -> HcommCcuKernelRegister(kernelFunc, kernelArgs)
 -> HcommCcuKernelRegisterEnd
 -> HcommCcuKernelLaunch(kernelHandle, taskArgs)
 -> CCU Mission
```

这条链关心一次 Host API 如何选算法、注册 kernel、准备参数并 Launch。

### 3.2 MC2/KFC CCU

```text
MC2 Host tiling
 -> HcclAllocComResourceByTiling
 -> CCU instance/channel/kernel handle/message area
 -> device KFC persistent server
 -> queue message/opId/state
 -> message-to-taskArg
 -> dispatch collective CCU Program
 -> completion/finalize
```

MC2 不是普通 Host API 的另一种 testcase 参数。它增加独立资源控制面和设备侧常驻调度面。

## 4. PR 2273 的方案

### 4.1 “通用原语录制”的含义

PR 2273 让真实 CCU kernel 在 Host ST 注册阶段执行一遍。此时 CCU API 不访问硬件，而把 `LocalCopy`、`Read`、`Write`、`Reduce`、Event/Loop 等调用记录成统一操作，再转换成现有 `SimTaskQueue`：

```text
ordinary HCCL Host API
 -> real selector/executor/template
 -> real CCU kernel function
 -> generic CCU primitive recorder
 -> SimTaskQueue
 -> Task DAG / memory conflict / semantics checker
```

“通用”指 recorder 面向 CCU 公共原语，不为 AllGather、AllToAllV 分别重写数据搬运；它不等于已通用覆盖所有 collective、Mission、KFC 和硬件并发。

### 4.2 优点

- 入口真实：从普通 HCCL API 经过真实 selector/executor/template；
- 适配成本低：已有原语覆盖后，新算法可复用 recorder；
- Checker 复用好：直接复用成熟的 DAG、冲突和符号数据框架；
- recorder 隔离：不同 kernel 的录制状态可以独立管理；
- 适合作为普通 HCCL CCU 全路径 ST 的原型。

### 4.3 对并发的实际覆盖

PR 2273 的 `SimTaskQueue` 能表达 stream/notify/通信依赖并检查没有 happens-before 的内存冲突。这比只看最终数据更强。

但 LoopGroup 的 clone、loop、iteration 被展开后顺序追加为 task，无法仅凭这一执行结果证明多个 LoopEngine 的真实并行安全；Mission、SQE、taskArg 资源作用域也没有作为一等对象保留。因此它覆盖的是“降低后的 task 并发关系”，不是完整 CCU 执行层级。

## 5. PR 2273 不能独立覆盖 MC2 的原因

| 评审维度 | PR 2273 | MC2 正式 ST 要求 |
|---|---|---|
| 普通 Host API/selector/executor | 强 | 有价值，但不是 MC2 唯一入口 |
| MC2 tiling 和资源上下文 | 无 | 必须校验 |
| KFC persistent server | 无 | 必须校验 |
| 队列槽/opId/状态机/finalize | 无 | 必须校验 |
| 消息到 taskArg | 无 | 必须逐字段校验 |
| Mission/LoadArg/LoopEngine 结构 | 降低后基本丢失 | 必须保留 |
| Task DAG/内存冲突 | 强 | 长期需要 |
| collective 覆盖 | 少量正例 | 需覆盖边界和协议反例 |

仅给 PR 2273 增加 KFC testcase 也不够。其核心中间表示已经把 CCU 结构降低成 task，无法自然表达队列消息属于哪个 Mission、taskArgs 怎样绑定 LoadArg、LoopEngine 如何拥有资源。

另一个风险是 success-only Stub：若 Register/Launch、动态符号或资源接口只返回成功，但没有产生 Program、任务或状态变化，case 可能假通过。正式方案要求成功返回之外必须断言可观测产物和最终结果。

## 6. asc-devkit 当前正式方案

### 6.1 两个目标、三个校验域

| 目标 | 校验域 | 当前 AllToAllV 基线 |
|---|---|---:|
| `ascendc_ut_adv_api_hccl_ccu_mc2_host` | Host tiling/资源/cache/错误回滚 | 9 |
| `ascendc_ut_adv_api_hccl_ccu_mc2_device` | 真实普通 kernel 的 Program/taskArg/结果 | 8 |
| 同一 Device 目标 | 真实 KFC server 的消息/dispatch/结果 | 10 |

历史最近回归为 27/27，提交前仍应重新运行。目录统一在：

```text
testcase/mc2_ccu/
├── host/
└── device/
```

旧的 direct 目录已删除。Kernel Device 和 KFC Device 仍是不同校验域，只是共享一个构建目标和公共模型。

### 6.2 Host 域

Host 目标直接编译产品 `hccl_mc2.cc`、`hccl_alloc_ctx_res.cc`，检查 CCU engine 分支、资源类型和数量、cache key、重复申请、失败传播和回滚。

`mc2_ccu/cann_host_bridge_stub.cc` 关闭真实 CANN bridge；`hccl_op_common_stub.cc` 补真实 Host 源所需的最小 selector/executor/资源依赖。它们不代表普通 HCCL selector/executor 全链已被验证。

### 6.3 Kernel Device 域

`ccu_all_to_all_v_mesh1d_adapter` 按产品 kernel 的具体函数签名调用真实实现，CCU primitive Stub 将它捕获成结构化 Program。随后绑定 taskArgs，由 `CcuSimWorld` 执行多 Rank 数据传输，公共和 AllToAllV Checker 分别检查结构、ABI 和结果。

adapter 只做三件事：捕获真实 kernel、准备普通 taskArgs、构造 KFC taskArgs。算法 peer 循环和数据搬运仍在真实产品 kernel 中。

### 6.4 KFC Device 域

该域编译真实 KFC server kernel。`CcuKfcSimWorld`/`ccu_kfc_proxy` 模拟队列消息、状态、start/completion CKE 和 dispatch；公共 KFC Checker 与 `ccu_all_to_all_v_mc2_checker` 检查消息协议、taskArg 映射和最终结果。

### 6.5 Checker 分层

```text
ccu_checker
  Program/Mission/Variable/Loop/Channel/Event 公共结构

ccu_kfc_checker
  KFC 队列、消息生命周期和公共状态机

ccu_all_to_all_v_semantics_checker
  AllToAllV Program、LoadArg、peer 数据布局和结果

ccu_all_to_all_v_mc2_checker
  AllToAllV KFC 消息、peer 表、taskArg 映射和 MC2 结果
```

公共 Checker 不允许 include collective 专用 adapter，也不允许写死 AllToAllV 的 XN 或 taskArg 下标。

## 7. 当前方案也有边界

当前 asc-devkit 方案不能被描述为“已经全面优于 PR 2273”。它仍缺少：

- 从普通 `HcclAlltoallv` 到 CCU Register/Launch/RuntimeWorld 的完整闭环；
- 通用 `KernelHandle -> Program` registry；
- CCU `Program -> DAG` 和通用内存冲突 Checker；
- 多个 MissionInstance 与多 LoopEngine 的受控 interleaving；
- Translator、真实微码、驱动、设备资源和链路时序验证。

当前 `hcomm_ccu_stub.cc` 的 Register/Launch 主要返回成功并记录注册事实，不会执行任意 `kernelFunc`、保存 Program、绑定 taskArgs 或 Launch。因此现在直接从普通 Host API 跑 case，可能只证明接口返回成功。

## 8. adapter 能否被普通 Host API 取代

长期可以，但必须先满足两个条件：

1. 产品 kernel 有统一 callable ABI，或 Register 时提供可类型安全调用的 wrapper；
2. ST backend 真正实现 Register 捕获、`KernelHandle -> Program` 保存、Launch 参数绑定和 RuntimeWorld 执行。

完成后，普通 kernel 的 `CaptureXxxKernel` 和手工 `PrepareTaskArgs` 可逐步删除。KFC 消息到 collective taskArgs 的映射仍是 collective 专用 ABI，除非产品层也提供统一 builder 注册机制。

真实硬件 Register/Launch 则属于另一测试层：它能移除 Host 仿真 adapter，但需要设备环境，不能替代离线 ST 的快速结构和错误注入能力。

## 9. 两种方案的长期合并方向

推荐目标不是二选一，而是形成一条保留结构、可选择后端的链：

```text
ordinary HCCL Host API              MC2 Host/KFC message
          |                                  |
 generic Register/Launch backend       KFC protocol model
          +----------------+-----------------+
                           |
                  structured CCU Program
                  Mission + LoadArg + Loop
                           |
              +------------+-------------+
              |                          |
       structure/ABI checker       Program -> Task DAG
                                         |
                              conflict/semantic checker
```

其中可复用 PR 2273 的部分是普通 API 集成经验、通用原语语义映射和 DAG 后端；asc-devkit 保留 MC2 Host、KFC、Mission/taskArg 和 Program-first 架构。

## 10. 并发能力扩展

为了覆盖多个单 Mission kernel 同时运行、多 Mission 和多 LoopGroup/LoopEngine，需要：

- 每次 Launch 创建带 Rank、Program、taskArgs、launch id 的 `MissionInstance`；
- 将 LoopEngine/iteration 建为独立执行节点；
- 从 Event/CKE、Channel completion、KFC 队列和 wait 构造 happens-before；
- 为每个节点解析读写地址集合，包括 GSA offset、`goSize.addrOffset` 和 clone offset；
- 同时支持确定性拓扑执行与受控 interleaving；
- 检查没有依赖关系的重叠读写，而不是依赖某次串行执行顺序。

当前 AllToAllV 无 slave thread，但数据面会先为所有 peer 提交 Write，本地段使用 GroupCopy，再按每 16 个 Rank
合并 Event mask 等待。现有 RuntimeWorld 能验证批量提交、零长度完成位和最终结果；它仍按确定顺序解释 Program，
因此不足以作为任意硬件 interleaving 下无冲突的完整证据。

## 11. 沟通建议

评审时建议使用以下表述：

> PR 2273 已验证普通 HCCL CCU kernel 到现有 SimTask Checker 的桥接可行性，其 Host API 集成、通用原语和 DAG 复用值得保留。MC2 CCU 还包含独立 Host 资源控制、KFC 常驻调度、消息到 taskArg ABI 和 CCU 结构契约，PR 当前没有这些对象，因此不能直接作为 MC2 正式骨架。asc-devkit 采用 Host/Device 双目标和三校验域先覆盖这些风险；下一阶段应补通用 Register/Launch，并把 PR 的 DAG 思路接在统一 Program IR 之后。

不要使用“PR 完全不可行”或“asc-devkit 已覆盖所有并发”的绝对结论。两者解决的问题和当前成熟度不同。

## 12. 常见问题

### Q1：为什么不直接给 PR 2273 加 KFC case？

因为缺的不只是入口 case，而是 KFC 消息、Mission/taskArg、队列状态和结构化 Program 等一等模型。继续在已降低的 `SimTask` 上补字段会形成旁路模型。

### Q2：当前 ST 有 DAG/内存冲突校验吗？

AICPU 路径有。当前 CCU 路径以结构、ABI、状态机和结果校验为主，还没有通用 `Program -> DAG` 冲突后端。

### Q3：为什么 AICPU 不需要逐算法 adapter，CCU 需要？

AICPU 在统一 HCOMM 原语边界录制。CCU 产品 kernel 函数签名和 taskArg ABI 当前仍可能随 collective 变化，通用 Register/Launch Stub 又没有真正执行 kernel，所以暂时需要薄 adapter。

### Q4：一个 Mission 能驱动多个 LoopEngine 吗？

可以。Mission 是启动和资源作用域，LoopEngine 是其内部执行资源。多 Rank、多 Mission、多 LoopEngine 是不同层级。

### Q5：最终数据正确是否足够？

不够。串行展开可能掩盖同步缺失，success-only Stub 可能掩盖算法未执行。必须同时检查 Program/Task 产物、ABI、同步/状态和最终语义。

## 13. 当前代码索引

```text
testcase/mc2_ccu/host/
testcase/mc2_ccu/device/

utils/src/ccu/
  ccu_all_to_all_v_mesh1d_adapter.*
  ccu_kfc_server_adapter.*

utils/src/hccl_proxy/
  ccu_st_types.h
  ccu_kernel_proxy.*
  ccu_kfc_proxy.*
  hcomm_ccu_stub.cc

utils/src/sim_world/
  ccu_sim_world.*
  ccu_kfc_sim_world.*

utils/src/hccl_verifier/
  ccu_checker.*
  ccu_kfc_checker.*
  semantics_check/ccu_all_to_all_v_semantics_checker.*
  semantics_check/ccu_all_to_all_v_mc2_checker.*

utils/src/mc2_ccu/
  cann_host_bridge_stub.cc
  hccl_op_common_stub.cc
```

关联文档：

- 仓级现状：[HCCL_ST整体原理与迁移适配指南.md](HCCL_ST整体原理与迁移适配指南.md)
- Devkit 总体设计：[Devkit_HCCL_ST整体设计与演进方案.md](Devkit_HCCL_ST整体设计与演进方案.md)
- 新算法接入：[MC2_CCU算法适配原理与ST指南.md](MC2_CCU算法适配原理与ST指南.md)
- CCU 原理：[CCU算法原理.md](CCU算法原理.md)
