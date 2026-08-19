# CCU ST 方案：MC2 路径 selector/CalcRes 全链 + Program IR + DAG 并发冲突校验

## 目录

- [1. MC2 端到端流程与 op 交互边界](#1-mc2-端到端流程与-op-交互边界)
- [2. 与 PR_4721 的差异与共同点](#2-与-pr_4721-的差异与共同点)
- [3. ST 方案设计](#3-st-方案设计)
- [4. Program IR 机制](#4-program-ir-机制)
- [5. DAG 并发冲突校验](#5-dag-并发冲突校验)
- [6. 测试框架封装](#6-测试框架封装)
- [7. 通用与专属代码](#7-通用与专属代码)
- [8. 实现分阶段](#8-实现分阶段)
- [9. 目录架构](#9-目录架构)
- [10. HcommCcuKernelRegister 桥接机制](#10-hcommccukernelregister-桥接机制)
- [11. 边界声明](#11-边界声明)
- [12. 新增算子接入指南](#12-新增算子接入指南)
- [13. PR_4721 Host 域与 Device 域用例梳理](#13-pr_4721-host-域与-device-域用例梳理)
- [14. 方案 A 与 PR_4721 优缺点对比](#14-方案-a-与-pr_4721-优缺点对比)
- [15. 迁移到 feat/alltoall-migration 分支的文件清单](#15-迁移到-featalltoall-migration-分支的文件清单)

---

## 1. MC2 端到端流程与 op 交互边界

### 1.1 MC2 不走普通 Host API

MC2 路径不走 `HcclAlltoAllV()` 入口，有独立的两个 Host 阶段 + 一个 Device 阶段：

```
Phase 1: HcclAllocComResourceByTiling (资源准备 + kernel 注册)
Phase 2: CcuKernelLaunch (启动 KFC server)
Phase 3: Device 侧 KFC server 执行微码 → DispatchKfcSubKernel → collective kernel
```

### 1.2 端到端调用链（以 AllToAllV 为例）

★ = AllToAll 算法交互边界，🔶 = 微码录制位置

```
Phase 1: HcclAllocComResourceByTiling [hccl_mc2.cc:299]
│
├─ AllocCcuOpResCtx                          [hccl_alloc_ctx_res.cc:122]
│   分配 workspace/XN/CKE device 内存
│
├─ CcuSelectAlg                              [hccl_alloc_ctx_res.cc:820]
│   │
│   ├─ SelectAlgAndPrepareEngine             [:849]
│   │   → AlltoAllAutoSelector::SelectCcuScheduleAlgo
│   │     → algName = "CcuAlltoAllVMesh1D"
│   │
│   ├─ ConfigureKfcServerOps (通用)          [:905]
│   │
│   └─ GetCcuOpParamResCtx                   [:731]
│       └─ HcclGetAlgRes                     [op_common.cc:731]
│           │
│           ├─ ★ executor->CalcRes          [:783]
│           │   → CcuTempAlltoAllMesh1D::CalcRes
│           │     ★ kernelFunc = CcuAlltoAllMesh1DKernel
│           │     ★ kernelArg = CcuKernelArgAlltoAllMesh1D{...}
│           │
│           ├─ ccuKfcExecutor->CalcRes       [:799]
│           │   → InheritKfcServerKernelArg (替换 collective → KFC server)
│           │
│           └─ GetAlgResCcu                  [op_common.cc:1255]
│               ├─ HcclGetChannelForCcu (通用)
│               │   channel 写入 kernelArg
│               └─ HcclGetCcuKernel          [:1378]
│                   ├─ HcommCcuKernelRegisterStart
│                   ├─ HcommCcuKernelRegister(kernelFunc=CcuKfcServerKernel)
│                   │   → 执行 CcuKfcServerKernel
│                   │     → KfcServerGenCircularQueue
│                   │       → ★ DispatchKfcSubKernel
│                   │         → ★ CcuAlltoAllVMesh1DKernel(...)
│                   │           → CCU API 录制 Rep
│                   ├─ 🔶 HcommCcuKernelRegisterEnd
│                   │   → TransRepSequenceToMicrocode → CcuInstr
│                   │   → LoadInstruction 装载到 device
│                   └─ 返回 kernelHandle
│
└─ aclrtMemcpy H2D

Phase 2: CcuKernelLaunch [hccl_mc2.cc:387]
  → HcommCcuKernelLaunch(KFC server, 6 startup args)

Phase 3: Device 侧 (CCU 硬件执行微码)
  → KFC server 微码循环
    → ★ CCU_IF(param[0] == opId)
      → AllToAllV 微码段 (来自注册时录制的 Rep → Translate)
```

### 1.3 op 交互边界分类

**ops ↔ hcomm 库交界（通用）**：

| 位置 | 代码 | 交界内容 |
|------|------|---------|
| `op_common.cc:1458` | `HcommCcuKernelRegister` | ops 把 kernelFunc + kernelArg 交给 hcomm 库 |
| `op_common.cc:1468` | `HcommCcuKernelRegisterEnd` | hcomm 库完成微码录制+装载 |
| `hccl_mc2.cc:480` | `HcommCcuKernelLaunch` | ops 把 kernelHandle + taskArgs 交给 hcomm 库启动 |

**KFC server ↔ AllToAll 算法交界**：

| 位置 | 通用/专属 | 交界内容 |
|------|:---:|---------|
| `hccl_alloc_ctx_res.cc:905` ConfigureKfcServerOps | 通用 | 注册 opId/opParam |
| `op_common.cc:1370` HcclGetChannelForCcu | 通用 | channel 写入 kernelArg |
| `ccu_temp_all_to_all_mesh_1D.cc:49` kernelFunc 设置 | **专属** | AllToAll kernel 函数指针 |
| `ccu_temp_kfc_server.cc:37` InheritKfcServerKernelArg | **专属** | 按算子名分支继承 |
| `ccu_kernel_kfc_server.cc:240` DispatchKfcSubKernel→AllToAllV | **专属** | 按 opType 调 collective kernel |
| `ccu_kernel_kfc_server.cc:258` DispatchKfcSubKernel→AllToAll | **专属** | 按 opType 调 collective kernel |

### 1.4 从 AllToAllV 视角看参与 MC2 的 ops 代码

参与 MC2 的 4 块：

| 代码 | 作用 | MC2 调用位置 |
|------|------|-------------|
| `selector/alltoall_auto_selector.cc` | 选算法 | CcuSelectAlg → SelectAlgAndPrepareEngine |
| `executor/ins_v2_all_to_all_v_sole_executor.cc` | CalcRes 构建模板 | HcclGetAlgRes → executor->CalcRes |
| `template/ccu/ccu_temp_all_to_all_mesh_1D.cc` | CalcRes 设置 kernelFunc+kernelArg | executor->CalcRes → template->CalcRes |
| `template/ccu/kernel/ccu_kernel_all_to_all_v_mesh1d.cc` | collective kernel 函数 | Register 时执行 / DispatchKfcSubKernel 调用 |

不参与 MC2 的代码：

| 代码 | 为什么不参与 |
|------|-------------|
| `all_to_all_v_op.cc` (HcclAlltoAllV 入口) | MC2 不走普通 Host API |
| executor 的 Orchestrate | MC2 不做数据切分 |
| executor 的 FastLaunch | MC2 不走普通 Launch |
| template 的 KernelRun | MC2 Launch 只启动 KFC server |

### 1.5 selector/executor/template/kernel 的关联机制

| 关联 | 机制 | 时机 |
|------|------|------|
| opType → selector | `SelectorRegistry` 查表 | 运行时 |
| 引擎 → SelectXxxAlgo | `AutoSelectorBase::Select` if 分支 | 运行时 |
| topo+opType → algName | `SelectCcuScheduleAlgo` 条件逻辑 | 运行时 |
| opType+algName → executor | `CollAlgExecRegistryV2` 查表 | 运行时 |
| executor → template | `REGISTER_EXEC_V2` 模板参数 | **编译期** |
| template → kernel | `#include` + 取函数指针 | **编译期** |

selector 和 executor 之间没有直接调用，只通过 algName 字符串间接耦合。

---

## 2. 与 PR_4721 的差异与共同点

### 2.1 PR_4721 (a4d9590) 现状

PR_4721 有三个校验域，27 个 case：

- **Host 域 (9 case)**：验证 `CcuSelectAlg`→资源分配/cache/opId/startup args，但 `HcommCcuKernelRegister` 是 stub（只计数不执行 kernelFunc）
- **Kernel Device 域 (8 case)**：adapter 直接调 kernel 函数，捕获 Program IR，RuntimeWorld 执行+数据结果对比。**不走 selector/template.CalcRes**，adapter 硬编码 config
- **KFC Device 域 (10 case)**：验证 KFC server 的 queue/CKE/dispatch 协议 + message→taskArgs 映射

### 2.2 共同点

| 维度 | 方案 A | PR_4721 |
|------|--------|---------|
| Program IR | ✅ 19 种 OpCode，保留 LoadArg/Variable/Address/IF/WHILE | ✅ 相同 |
| Program 结构校验 | ✅ CcuProgramChecker | ✅ 相同 |
| 操作计数校验 | ✅ SemanticsChecker | ✅ 相同 |
| taskArgs ABI 校验 | ✅ CcuTaskArgChecker (13 槽) | ✅ 相同 |
| RuntimeWorld 执行 | ✅ 轮询+deadlock | ✅ 相同 |
| 独立期望字节对比 | ✅ CaptureRuntimeExpectation | ✅ 相同 |
| CCU API stub 捕获 | ✅ ccu_primitives_stub | ✅ 相同 |
| RegisterManager | ✅ 录制一次执行多次 | ✅ 相同 |

### 2.3 差异点

| 维度 | 方案 A | PR_4721 |
|------|--------|---------|
| **config 来源** | 从真实 selector/template.CalcRes 产出 | adapter 硬编码 AllToAllVKernelConfig |
| **selector 验证** | ✅ 调真实 SelectAlgAndPrepareEngine | ❌ 不走 selector |
| **template.CalcRes 验证** | ✅ 调真实 executor->CalcRes | ❌ 不走 CalcRes |
| **channel 分配验证** | ✅ 调真实 HcclGetChannelForCcu | ❌ adapter 硬编码 channels |
| **DAG 并发冲突** | ✅ 新增 | ❌ 无 |
| **KFC server 验证** | ❌ 不覆盖 | ✅ KFC Device 域 |
| **Host 资源验证** | ✅ 集成在 VerifyScenario 里 | ✅ Host 域（独立 target） |

### 2.4 核心差异

PR_4721 的 Kernel Device 域本质是**对 kernel 函数做 UT**——手工构造 config，直接调 kernel，检查输出。方案 A 要改的是 **config 的来源**：从硬编码变成真实 selector/template 产出。如果 template.CalcRes 构建了错误的 kernelArg（channel 顺序错、rankSize 不匹配），方案 A 能发现，PR_4721 不能。

### 2.5 与 HCCL 仓方案 (e28970ad) 的对比

| 维度 | 方案 A | HCCL 仓 |
|------|--------|---------|
| 验证路径 | MC2 (selector→CalcRes→kernel) | 普通 Host API (HcclAlltoallv→Launch) |
| 中间表示 | Program IR (结构化) | SymOp→TaskStub (降低后) |
| ABI/操作计数/同步契约 | ✅ 保留 | ❌ 降低后丢失 |
| DAG 并发冲突 | ✅ 借鉴 HCCL 思路 | ✅ 成熟 |
| 数据结果 | ✅ 独立期望字节对比 | ✅ 符号语义传播 |
| MC2 路径适配 | ✅ | ❌ |

---

## 3. ST 方案设计

### 3.1 方式 A：调真实 selector + CalcRes + channel + kernel 注册（经 HcommCcuKernelRegister 桥接）

ST 调用产品的真实函数链，通过 `HcommCcuKernelRegister` 桥接到 `RegisterManager` 自动捕获 Program IR：

```
ST testcase:
  1. 构造 tiling + 虚拟拓扑
  2. 调真实 SelectAlgAndPrepareEngine → algName
  3. 调真实 GetCcuOpParamResCtx → HcclGetAlgRes
     → executor->CalcRes → CcuKernelInfo{kernelFunc, kernelArg}
     → HcclGetChannelForCcu → channel 写入 kernelArg
     → HcclGetCcuKernel → HcommCcuKernelRegister
       → 桥接到 RegisterManager（查 capture 注册表 → adapter capture → Program IR）
  4. 从 RegisterManager::GetLastCompiledGroup() 取 handles
  5. 校验 + RuntimeWorld + 数据结果
```

不调 `ccuKfcExecutor->CalcRes`（不替换 collective kernel 为 KFC server kernel）。

### 3.2 验证范围

| 组件 | 验证什么 | 怎么验证 |
|------|---------|---------|
| selector | 选对 algName | 断言 algName == "CcuAlltoAllVMesh1D" |
| template.CalcRes | 构建 CcuKernelInfo | 断言 kernelFunc/kernelArg/channelDescs |
| channel 分配 | channel 写入 kernelArg | 断言 channels[] |
| collective kernel | CCU API 调用正确 | Program IR 捕获 + 6 层校验 |

### 3.3 不验证

- KFC server kernel（queue/CKE/wrap/finalize）
- InheritKfcServerKernelArg（KFC server 替换逻辑）
- CcuSelectAlg 编排（cache hash/共享 server）— Host 域已有覆盖
- HcommCcuKernelRegister 桥接到 RegisterManager（真实微码录制不覆盖，详见 §10）
- 微码/Translator/CcuInstr
- 普通 Host API 路径

### 3.4 ST 流程

```
VerifyScenario
│
├── [1] 初始化: Reset + 构造 topo/tiling
│
├── [2] per-rank host flow (全自动, 每个 rank 独立跑一遍):
│      SimWorld::Init → aclrtSetDevice(rank) → aclrtCreateStream
│      → Sim_HcclCommInitClusterInfo(rank) → AllocCcuOpResCtx
│      → InitOpParamByTiling → SelectAlgAndPrepareEngine (真实 selector 选 "CcuAlltoAllMesh1D")
│      → GetCcuOpParamResCtx → 真实 HcclGetAlgRes (op_common.cc)
│        → executor->CalcRes (真实 template 构建 CcuKernelInfo)
│        → HcclGetChannelForCcu (真实 channel 分配 → HcclChannelAcquire)
│        → HcclGetCcuKernel → HcommCcuKernelRegister
│          → 桥接到 RegisterManager (查 capture 注册表 → adapter)
│          → adapter 调真实 CcuAlltoAllMesh1DKernel
│          → CCU API 被 ccu_primitives_stub 拦截 → Program IR
│      → CleanupHostEnv (销毁 comm + Deinit SimWorld)
│      产出: handles[rank] (KernelHandle, 含 Program IR)
│
├── [3] 构造测试数据 + 静态校验:
│      BuildScenarioData (RankMemory/input/output + taskArgs + RankLaunch)
│      → CheckRegistration (Program 结构 + 操作计数 + 同步契约)
│      → CheckTaskArgs (taskArgs ABI, 如 AllToAll 11 个参数)
│
├── [4] 反例分支 (根据 scenario bool 字段):
│      swapTaskArgs    → 交换两个 taskArg, 期望 CheckTaskArgs 失败
│      missingTaskArg  → 删一个 taskArg, 期望 Launch 返回 CONTRACT_ERROR
│      (反例直接 return, 不走执行)
│
└── [5] 执行 + 校验:
       RuntimeWorld::Launch (执行 Program IR, 多 rank 轮询 + deadlock 检测)
       → CcuDagChecker::Check (DAG 并发冲突检测: 程序顺序边 + 同步边 + 区间重叠)
       → CheckRuntime (逐字节对比 output vs 期望)
       (corruptOutput 反例: 篡改 output 字节, 期望 CheckRuntime 失败)
```

---

## 4. Program IR 机制

### 4.1 模拟 CCU 提前录制

```
真实: kernelFunc → Rep → CcuInstr 微码 → device 执行微码
ST:   kernelFunc → Program IR → RuntimeWorld 解释执行
```

ST 不经过 Rep 和 CcuInstr，直接用 Program IR 作为中间表示。

### 4.2 Program IR 结构

```cpp
struct Program {
    uint32_t rankId, rankSize;
    std::vector<ChannelHandle> channels;
    std::vector<Operation> operations;           // 操作序列 (19 种 OpCode)
    std::vector<ArgDescriptor> arguments;         // argId → Variable 绑定
    std::set<ResourceHandle> variables, addresses, events, buffers;
    std::map<(peerRank, index), Handle> channelVariables;
    std::map<handle, (addr, token)> localAddresses, remoteAddresses;
    std::vector<GroupCopyDescriptor> groupCopies;
};
```

### 4.3 录制过程

kernel 代码的每个 CCU API 被 `ccu_primitives_stub` 拦截，转换成 `Operation` 追加到 `Program::operations[]`：

```
ccu::LoadArg(var, argId)        → Operation{LOAD_ARG, dst=var, index=argId}
ccu::Load(addr, var)            → Operation{LOAD_MEMORY, dst=var, src0=addr}
ccu::Write(ch, dst, src, len)   → Operation{WRITE, dst, src0, src1=len, channel}
ccu::NotifyWait(ch, idx, mask)  → Operation{NOTIFY_WAIT, channel, index, mask}
CCU_IF(cond)                    → Operation{IF_BEGIN, src0=cond, peerIndex}
```

### 4.4 13 槽 SQE 分组

CCU 硬件 SQE 的 `args[13]`（`CCU_SQE_ARGS_LEN=13`）。taskArgs 按 13 个一组打包成 Mission：

```
missionIndex = argId / 13
slotIndex    = argId % 13

AllToAll 11 个 taskArgs → 1 个 Mission (11 ≤ 13)
```

### 4.5 RuntimeWorld 执行

轮询调度，每轮每个 rank 执行一条 operation：

```
LOAD_ARG → 从 TaskParam.args 取值
LOAD_MEMORY → ResolveMemory 解析地址, 从 RankMemory 读 8 字节
WRITE → ResolveMemory 解析 src/dst, memmove 跨 rank 搬数据
NOTIFY_WAIT → 检查 notify mask, 不满足则阻塞
IF_BEGIN → 条件 false 跳到 IF_END
全部 completed → SUCCESS
全部阻塞 → DEADLOCK
```

录制一次、执行多次（和真实 CCU 一样：Register 一次，Launch 多次）。

---

## 5. DAG 并发冲突校验

### 5.1 为什么需要 DAG

RuntimeWorld 是轮询串行执行，**不能发现无同步的并发内存访问**——按固定顺序执行，碰巧不冲突就过了。DAG 通过可达性分析判断两个操作是否可能并发。

### 5.2 实现策略：RuntimeWorld 执行时记录，事后建 DAG

RuntimeWorld 执行时已经解析了地址、走了同步路径、选择了 IF 分支。只需在执行过程中多记一步：每个数据操作的已解析读写区间。事后用这些记录建 DAG 做冲突检查。

```
RuntimeWorld 执行 (现有 + 新增记录):
  每条数据操作 (WRITE/LOCAL_COPY) 执行时:
    → 已有: ResolveMemory 解析地址, memmove 搬数据
    → 新增: 记录 {(rank, opIndex, srcInterval, dstInterval)} 到执行日志

执行完成后:
  用执行日志建 DAG:
    节点 = 每个数据操作 (含已解析读写区间)
    边 = 程序顺序 + 同步 (执行时的实际同步路径)
  冲突检查:
    两节点区间重叠 + 至少一个写 + DAG 不可达 → 冲突
```

### 5.3 优势

| 优势 | 说明 |
|------|------|
| 地址解析 | 直接用执行时的已解析地址，不需要预执行 |
| 同步边 | 执行时已走过同步路径，记录实际 happens-before |
| 控制流 | 只记录实际执行过的操作，不误报不执行的分支 |
| 复杂度 | 低，RuntimeWorld 已有所有信息 |

### 5.4 借鉴 HCCL 仓 DAG

借鉴校验思路，不复用录制体系：

| 借鉴点 | HCCL 仓机制 | 方案 A 适配 |
|--------|------------|------------|
| happens-before | 程序顺序边 + post/wait 同步边 | 执行日志中的程序顺序 + notify/event 同步 |
| 并发判定 | Fragment 矩阵 + 沿 DAG 走标非并发 | 相同思路 |
| 冲突判定 | 区间重叠 + 至少一个写 + 不可达 | 相同 |
| 地址解析 | SymVariable::Resolve | RuntimeWorld 的 ResolveMemory |

不复用：SymRecorder、TaskStub、SimTaskQueue、BufferSemantic 符号传播。

### 5.5 DAG 校验能力

| 能力 | 覆盖 |
|------|:---:|
| 无同步的并发内存冲突 | ✅ |
| happens-before 重建 | ✅ |
| post/wait 配对 | ✅ |
| 死锁检测 | ✅ (RuntimeWorld 也能检测) |
| 符号语义传播 | ❌ (用独立期望字节对比替代) |

---

## 6. 测试框架封装

> **本节已更新为实际实现，与 §15 一致。** 早期设计的 `PrepareKernel`/`CheckProgramStructure` 等基类工具函数已重构为 5 个纯虚函数接口。

### 6.1 设计目标

用例只关注场景差异（rankSize、数据量、拓扑类型），不重复写 selector/CalcRes/channel/捕获/校验的样板代码。

### 6.2 框架结构

#### 6.2.1 设计思路

基类 `CcuStFixture` 提供**通用 5 阶段流程**（`VerifyScenario`）+ **5 个纯虚函数接口**。子类实现虚函数填充专属逻辑。通用流程在基类中，扩展新算法只需写子类。

```
CcuStFixture (基类, 通用 5 阶段 VerifyScenario + 5 纯虚函数)
  ├─ VerifyScenario(scenario)           ← 通用: 5 阶段全链流程
  ├─ BuildTiling(opType, dataType, algConfig)  ← 通用: 构造 tiling
  ├─ BuildTopoMeta(rankSize, topoType)          ← 通用: 构造拓扑 (3 种)
  ├─ RegisterKernelForRank(...)                 ← 通用: selector→CalcRes→channel→捕获 IR
  ├─ CleanupHostEnv(...)                       ← 通用: 清理
  │
  │ 纯虚函数（子类必须实现）:
  ├─ BuildScenarioData(scenario, handles) → ScenarioData  ← ① 构造测试数据
  ├─ DoCheckRegistration(program, rank, rankSize)         ← ② 注册校验
  ├─ DoCheckTaskArgs(program, rank, data)                 ← ③ taskArgs 校验
  ├─ DoCaptureRuntimeExpectation(data)                    ← ④ 期望数据计算
  └─ DoCheckRuntime(data, world)                          ← ⑤ 运行时校验
  │
  │ 可覆盖虚函数:
  └─ SwapTaskArgIndices() → {5, 6}                       ← 反例: 交换哪两个 taskArg

CcuStAllToAll (子类, 实现 5 个虚函数)
  ├─ BuildScenarioData()     ← 构造 RankMemory + AllToAllLaunchConfig + RankLaunch
  ├─ DoCheckRegistration()   ← 调 CcuAllToAllSemanticsChecker.CheckRegistration
  ├─ DoCheckTaskArgs()       ← 调 CcuAllToAllSemanticsChecker.CheckTaskArgs
  ├─ DoCaptureRuntimeExpectation() ← 调 checker.CaptureRuntimeExpectation (转置语义)
  └─ DoCheckRuntime()        ← 调 checker.CheckRuntime (逐字节对比)
```

#### 6.2.2 CcuStFixture — 公共测试基类

```cpp
struct ScenarioData {
    std::vector<RankMemory> memories;
    std::vector<RankLaunch> launches;
};

class CcuStFixture : public ::testing::Test {
protected:
    void SetUp() override { RegisterManager::Global().Reset(); }
    void TearDown() override { RegisterManager::Global().Reset(); }

    // 通用流程入口 — 子类测试用例调这个
    void VerifyScenario(const CcuStScenario &scenario);

    // 5 个纯虚函数 — 子类必须实现
    virtual ScenarioData BuildScenarioData(const CcuStScenario &scenario,
                                           const std::vector<KernelHandle> &handles) = 0;
    virtual CheckResult DoCheckRegistration(const Program &program, uint32_t rank,
                                            uint32_t rankSize) = 0;
    virtual CheckResult DoCheckTaskArgs(const Program &program, uint32_t rank,
                                        const ScenarioData &data) = 0;
    virtual CheckResult DoCaptureRuntimeExpectation(const ScenarioData &data) = 0;
    virtual CheckResult DoCheckRuntime(const ScenarioData &data, RuntimeWorld &world) = 0;

    // 可覆盖 — 反例控制
    virtual std::pair<size_t, size_t> SwapTaskArgIndices() const { return {5, 6}; }

    // 通用工具函数
    static Mc2CcTilingInner BuildTiling(HcclCMDType opType, HcclDataType dataType,
                                         const std::string &algConfig);
    static TopoMeta BuildTopoMeta(uint32_t rankSize, StTopoType topoType);
    static void CleanupHostEnv(HcclComm &comm);
    static KernelHandle RegisterKernelForRank(uint32_t rank, uint32_t rankSize,
                                               const CcuStScenario &scenario,
                                               const TopoMeta &topoMeta,
                                               const Mc2CcTilingInner &tiling);
};
```

#### 6.2.3 CcuStScenario — 用例只填这个

```cpp
enum class StTopoType {
    SINGLE_DIE_MESH1D = 0,    // 1 server, single die (default)
    TWO_DIE_REGULAR,          // 1 server, 2D phyId layout, 2 die
    MESH_1D_CLOS,             // 2 servers, cross-server CLOS
};

struct CcuStScenario {
    uint32_t rankSize = 2;
    HcclDataType dataType = HCCL_DATA_TYPE_FP16;
    HcclCMDType opType = HcclCMDType::HCCL_CMD_ALLTOALLV;
    StTopoType topoType = StTopoType::SINGLE_DIE_MESH1D;

    std::string expectedAlgName;   // 期望 selector 选出的算法名（空=跳过断言）
    std::string algConfig;         // 强制指定算法（空=default 走自动选择）

    std::vector<std::vector<uint64_t>> sizes;  // sizes[src][dst] = sendSize (bytes)
    uint64_t count = 0;                        // 单 rank 元素数 (AllGather/ReduceScatter 用)

    HcclReduceOp reduceType = HCCL_REDUCE_SUM;
    HcclDataType outputType = HCCL_DATA_TYPE_RESERVED;

    // 反例控制
    bool corruptOutput = false;
    bool swapTaskArgs = false;
    bool missingTaskArg = false;
    bool outOfRangeParam = false;
};
```

#### 6.2.4 VerifyScenario 5 阶段流程

```
VerifyScenario(scenario)
│
├── 阶段 1：per-rank 注册 kernel（通用，不需要虚函数）
│   for (rank = 0..N-1):
│     RegisterKernelForRank(rank, ...)
│       → 设环境变量 (CCU_SCHED + CCU_KFC_BRANCH)
│       → SimWorld::Init + aclrtSetDevice + aclrtCreateStream
│       → Sim_HcclCommInitClusterInfo → AllocCcuOpResCtx
│       → InitOpParamByTiling → TryForcedAlgAndPrepareEngine (强制)
│       → if (!forced) SelectAlgAndPrepareEngine (自动)
│       → 断言 algName == expectedAlgName
│       → HandleSingleRankAndCommMode → GetCcuOpParamResCtx
│         → 真实 CalcRes + HcclGetChannelForCcu + HcclGetCcuKernel
│         → HcommCcuKernelRegister → 桥接到 RegisterManager → 捕获 Program IR
│       → CleanupHostEnv
│     handles[rank] = GetLastCompiledGroup().back()
│
├── 阶段 2：构造测试数据（虚函数 ①）
│   data = BuildScenarioData(scenario, handles)
│     → 构造 RankMemory (input/output + guard 字节)
│     → 构造 configs_ (专属 LaunchConfig)
│     → 构造 RankLaunch (handle + taskArgs + memory)
│       RankLaunch = {kernel handle, taskArgs, &memory}
│       此时不执行，只是打包好供阶段 5 使用
│
├── 阶段 3：静态校验（虚函数 ②③，不执行 RankLaunch）
│   for (rank = 0..N-1):
│     DoCheckRegistration(*program, rank, rankSize)
│       → 调专属 SemanticsChecker.CheckRegistration
│       → 校验 channel 数量/PreSync/PostSync/操作计数
│       → 只看 Program IR 结构，不执行
│     DoCheckTaskArgs(*program, rank, data)
│       → 调专属 SemanticsChecker.CheckTaskArgs
│       → 校验 data.launches[rank].taskArgs 的数量/映射/值匹配
│       → 只看 taskArgs 值，不执行 RankLaunch
│
├── 阶段 4：反例分支（通用）
│   if (swapTaskArgs):
│     swap taskArgs[i] ↔ taskArgs[j]
│     EXPECT_FALSE(DoCheckTaskArgs(...).Ok())  ← 期望校验失败（不执行）
│     return
│   if (missingTaskArg):
│     删一个 taskArg
│     EXPECT_EQ(world.Launch(...), CONTRACT_ERROR)  ← 期望 Launch 失败（执行但报错）
│     return
│
└── 阶段 5：执行 + DAG + 数据校验（虚函数 ④⑤，执行 RankLaunch）
    DoCaptureRuntimeExpectation(data)  → 计算期望 output（转置/拼接/分片）
    world.Launch(data.launches)        → 执行 Program IR（轮询+死锁检测）
      ← 这里才真正使用 RankLaunch：取 IR + 取 taskArgs + 操作 RankMemory
    CcuDagChecker().Check(...)         → DAG 并发冲突检测
    DoCheckRuntime(data, world)        → 逐字节对比 output vs 期望
    if (corruptOutput):
      篡改 output[0]
      EXPECT_FALSE(DoCheckRuntime(...).Ok())  ← 期望校验失败
```

#### 6.2.5 基类和子类的分工

| 内容 | CcuStFixture (基类) | CcuStAllToAll (子类) | CcuStAllGather (子类) |
|------|:---:|:---:|:---:|
| SetUp/TearDown | ✅ | — | — |
| **VerifyScenario** 5 阶段 | ✅ 通用 | — | — |
| **RegisterKernelForRank** | ✅ 通用 | — | — |
| **BuildTopoMeta** (3 种拓扑) | ✅ 通用 | — | — |
| **BuildTiling** | ✅ 通用 | — | — |
| **BuildScenarioData** | — (纯虚) | ✅ AllToAll 类型 | ✅ AllGather 类型 |
| **DoCheckRegistration** | — (纯虚) | ✅ AllToAll 契约 | ✅ AllGather 契约 |
| **DoCheckTaskArgs** | — (纯虚) | ✅ 11 参数 | ✅ 不同参数 |
| **DoCaptureRuntimeExpectation** | — (纯虚) | ✅ 转置语义 | ✅ 拼接语义 |
| **DoCheckRuntime** | — (纯虚) | ✅ 逐字节对比 | ✅ 逐字节对比 |
| **SwapTaskArgIndices** | ✅ 默认 {5,6} | 可覆盖 | 可覆盖 |

新增算法只需写子类（实现 5 个虚函数），不需要改基类。

### 6.3 算法专属测试类

每个算法一个测试类，继承公共基类 `CcuStFixture`，实现 5 个虚函数：

```cpp
// ===== AllToAll 专属测试类（当前实现） =====
class CcuStAllToAll : public CcuStFixture {
protected:
    ScenarioData BuildScenarioData(const CcuStScenario &scenario,
                                   const std::vector<KernelHandle> &handles) override;
    CheckResult DoCheckRegistration(const Program &program, uint32_t rank,
                                    uint32_t rankSize) override;
    CheckResult DoCheckTaskArgs(const Program &program, uint32_t rank,
                                const ScenarioData &data) override;
    CheckResult DoCaptureRuntimeExpectation(const ScenarioData &data) override;
    CheckResult DoCheckRuntime(const ScenarioData &data, RuntimeWorld &world) override;

private:
    std::vector<AllToAllLaunchConfig> configs_;
    AllToAllRuntimeExpectation expectation_;
    CcuAllToAllSemanticsChecker checker_;
};
```

用例只需要做两件事：
1. 构造 `CcuStScenario`（填 rankSize、数据量、拓扑类型、反例标志）
2. 调 `VerifyScenario(s)`

框架自动完成 selector/CalcRes/channel/捕获/校验/执行/对比全部步骤。

```cpp
// ===== 正例: 2-rank 均匀数据 =====
TEST_F(CcuStAllToAll, HostSelectorPicksMesh1DAndRuntimeDeliversTwoRankEqualLength) {
    VerifyScenario(MakeScenario(2, 100));
}

// ===== 正例: 4-rank 均匀数据 =====
TEST_F(CcuStAllToAll, FourRankEqualLengthTransfersCorrectly) {
    VerifyScenario(MakeScenario(4, 64));
}

// ===== 反例: 篡改 output =====
TEST_F(CcuStAllToAll, RuntimeCheckerRejectsOutputCorruption) {
    CcuStScenario scenario = MakeScenario(2, 80);
    scenario.corruptOutput = true;
    VerifyScenario(scenario);
}

// ===== 反例: 交换 taskArgs =====
TEST_F(CcuStAllToAll, CheckerRejectsTaskArgumentRebinding) {
    CcuStScenario scenario = MakeScenario(2, 80);
    scenario.swapTaskArgs = true;
    VerifyScenario(scenario);
}

// ===== 反例: 缺少 taskArg =====
TEST_F(CcuStAllToAll, LaunchRejectsMissingDynamicArgument) {
    CcuStScenario scenario = MakeScenario(2, 80);
    scenario.missingTaskArg = true;
    VerifyScenario(scenario);
}

// ===== 强制算法 =====
TEST_F(CcuStAllToAll, ForcedAlgConfigOverridesSelector) {
    CcuStScenario scenario = MakeScenario(2, 100);
    scenario.algConfig = "CcuAlltoAllMesh1D";
    VerifyScenario(scenario);
}
```

#### 用例分类

| 类型 | 用例数 | 说明 |
|------|:---:|------|
| 正例 (数据正确性) | 2 | 2-rank/4-rank 均匀数据 |
| 反例 (数据结果) | 1 | 篡改 output |
| 反例 (taskArgs) | 1 | 交换 taskArgs |
| 反例 (Launch) | 1 | 缺少 taskArg |
| 强制算法 | 1 | algConfig 指定 |
| DAG 专项正例 | 3 | 空日志/不重叠/有同步 |
| DAG 专项反例 | 4 | 无同步 WAW/WAR/未匹配同步 |

---

## 7. 通用与专属代码

### 7.1 通用代码（新算法不需要重写）

| 代码 | 作用 |
|------|------|
| CcuProgramChecker | Program 结构校验 (argId/资源/操作类型) |
| CcuTaskArgChecker | taskArgs ABI 校验 (13 槽/argId 绑定) |
| CcuRuntimeChecker | 执行完成检测 |
| RuntimeWorld | 轮询执行 Program IR |
| CcuDagChecker | DAG 并发冲突校验 |
| CcuStFixture | 测试基类 (通用 5 阶段 VerifyScenario + 5 纯虚函数 + 通用工具函数) |

### 7.2 collective 专属代码（新算法需要写）

| 代码 | 作用 | AllToAllV 示例 |
|------|------|---------------|
| 期望逻辑 | 算法语义的 output 计算 | sourceRank input[srcBase+sendOff] → dstRank output[dstBase+recvOff] |
| 操作计数契约 | 该算法 kernel 的操作数量 | LOAD_MEMORY=ranks×4, WRITE=channels, IF=ranks... |
| adapter | 调真实 kernel 函数 | 从 CalcRes 拿 kernelFunc + kernelArg |

---

## 8. 实现分阶段

### 阶段 1：selector/CalcRes/channel 链

- 在 PR_4721 Kernel Device 域基础上，把 adapter 硬编码 config 改为从真实 selector/template 产出
- 编译真实 selector/executor/template 产品代码
- 新增 selector/CalcRes/channel 相关验证点
- 保持现有 Program/ABI/RuntimeWorld/数据结果校验不变

### 阶段 2：DAG 并发冲突

- RuntimeWorld 执行时新增执行日志记录（读写区间）
- 实现 `CcuDagChecker`：建 DAG + happens-before + 区间重叠 + 可达性
- 借鉴 HCCL 仓 `CheckRankMem` 的 Fragment 并发矩阵思路

### 阶段 3：DAG 专项用例

- 正例：有同步无冲突
- 反例：无同步的并发 WRITE 访问重叠区间

### 阶段 4：扩展 collective

- AllGather CCU 接入（新写期望逻辑 + 操作计数 + adapter）
- ReduceScatter CCU 接入
- 每个 collective 新写 SemanticsChecker + DAG 用例

---

## 9. 目录架构

> **本节已更新为实际实现。** 早期设计包含 3 个 target（Host/Device/Selector），实际只实现了 selector target。adapter 已从 `ccu/` 合并到 `ccu_scenario/`，`ccu_primitives_stub` 已从 `hccl_depends_stub/` 移到 `hccl_proxy/`。

ST 代码分两层：测试用例层（testcase）和框架层（utils/src）。框架层编译成 `hccl-model-proxy` 和 `hccl-verifier` 两个静态库，被 selector target 链接复用。

### 9.1 整体结构

```
tests/api/adv_api/hccl/cc/st/algorithm/
│
├── CMakeLists.txt                          ← 全局变量 + include 路径
│
├── testcase/mc2_ccu/                       ← CCU ST（只有 selector target）
│   ├── CMakeLists.txt                      ← selector target 构建
│   └── selector/
│       ├── all_to_all_selector_testcase.cpp  ← AllToAll 端到端测试（6 个 TEST_F）
│       ├── dag_conflict_testcase.cpp         ← DAG 冲突检测测试（7 个 TEST）
│       ├── shim/ccu_primitives_dl.hpp        ← 重定向 CCU DL 头到 stub
│       └── stubs.cc                          ← 产品函数 stub
│
└── utils/src/                              ← ST 框架
    │
    ├── hccl_proxy/                          ← 桥接层 + CCU API stub（通用）
    │   ├── ccu_st_types.h                  ← Program IR 类型：OpCode/Operation/Program
    │   ├── ccu_kernel_proxy.{h,cc}         ← RegisterManager：注册→捕获 IR
    │   ├── ccu_primitives_stub.{h,cc}       ← CCU API stub：LoadArg/Write/Notify→Operation
    │   ├── hcomm_ccu_stub.cc                ← HcommCcuKernelRegister→RegisterManager 桥接
    │   ├── hccl_stub.cc                     ← CCU channel + dieId stub
    │   ├── hccl_profiling_dfx_stub.cc       ← profiling stub
    │   ├── aclrt_stub.cc                    ← RT stub
    │   └── communicator/                    ← SimChannel/SimThread + CCU channel 支持
    │
    ├── sim_world/                          ← 执行引擎（通用）
    │   ├── ccu_sim_world.{h,cc}            ← RuntimeWorld：轮询执行 IR + 死锁检测
    │   └── (旧 ST 的 sim_world/sim_npu/sim_task 等)
    │
    ├── hccl_verifier/                      ← 校验器（通用 + 专属）
    │   ├── ccu_checker.{h,cc}              ← CcuProgramChecker/CcuTaskArgChecker（通用）
    │   ├── ccu_dag_types.h                 ← DAG 类型（通用）
    │   ├── ccu_dag_checker.{h,cc}          ← DAG 冲突检测（通用）
    │   └── semantics_check/
    │       └── ccu_all_to_all_semantics_checker.{h,cc}  ← AllToAll 专属
    │
    ├── ccu_scenario/                       ← 场景编排（通用 + AllToAll 专属）
    │   ├── ccu_scenario.h                  ← CcuStScenario + StTopoType（通用）
    │   ├── ccu_fixture.{h,cc}              ← CcuStFixture 基类（通用 5 阶段 + 5 虚函数）
    │   ├── ccu_all_to_all.{h,cc}           ← CcuStAllToAll 子类（AllToAll 专属 5 虚函数）
    │   └── ccu_all_to_all_mesh1d_adapter.{h,cc}  ← AllToAll adapter（capture + taskArgs）
    │
    ├── mc2_ccu/                            ← 产品函数 stub
    │   └── (cann_host_bridge_stub.cc + hccl_op_common_stub.cc)
    │
    ├── common/                             ← 旧 ST 异常/工具
    └── aicpu/                              ← 旧 ST AICPU stub
```

### 9.2 构建目标

只有 1 个 target：

| Target | 可执行文件名 | 链接库 | 测试用例 |
|--------|------------|--------|---------|
| selector | `ascendc_ut_adv_api_hccl_ccu_selector_st` | hccl-model-proxy + hccl-verifier + gtest | 6 AllToAll + 7 DAG = 13 |

### 9.3 分层职责

| 层 | 目录 | 职责 | 编译进 |
|----|------|------|--------|
| 拦截层 | `hccl_proxy/` | CCU API stub + RegisterManager + 桥接 | hccl-model-proxy |
| 执行层 | `sim_world/` | RuntimeWorld 执行引擎 | hccl-model-proxy |
| 校验层 | `hccl_verifier/` | 通用 checker + DAG + AllToAll 语义 | hccl-verifier |
| 编排层 | `ccu_scenario/` | 基类 + AllToAll 子类 + adapter | selector target（直接编译） |
| 用例层 | `testcase/mc2_ccu/` | 测试用例 + shim + stubs | selector target |
    │       ├── ccu_all_to_all_semantics_checker.h/.cc    ← (新增) AllToAll 语义校验
    │       └── ccu_all_to_all_v_mc2_checker.*            ← (已有) MC2 checker
    │
    ├── sim_world/                          ← 运行时模拟
    │   ├── ccu_sim_world.h/.cc             ← (已有+新增执行日志) RuntimeWorld + RankMemory + RankLaunch
    │   ├── ccu_kfc_sim_world.h/.cc         ← (已有) KFC 模拟世界 (selector target 不用)
    │   └── sim_world.* / sim_npu.* / ...   ← (已有) SimWorld/SimNpu 等 host 拓扑模拟
    │
    ├── hccl_proxy/                         ← CCU API 拦截 + host 模拟
    │   ├── ccu_st_types.h                  ← Program/Operation/OpCode/TaskParam 类型定义
    │   ├── ccu_kernel_proxy.h/.cc          ← RegisterManager (kernel 注册 + Program 存储) + capture 注册表接口
    │   ├── ccu_kfc_proxy.h/.cc             ← KFC proxy
    │   ├── hcomm_ccu_stub.cc               ← HcommCcuKernelRegister 桥接到 RegisterManager (查 capture 注册表)
    │   ├── communicator/                   ← SimCommunicator (comm 模拟, 含 ChannelCommCreateForCcu)
    │   ├── topo_model/                     ← TopoModel (拓扑模拟)
    │   └── *_stub.cc                       ← aclrt/log/mmpa 等 stub
    │
    ├── hccl_depends_stub/                  ← CCU API stub
    │   ├── ccu_primitives_stub.h/.cc       ← ccu::LoadArg/Write/LocalCopy 拦截 → CompilerContext → Program IR
    │   └── hccl_depends_stub.cc            ← 其他依赖 stub
    │
    ├── mc2_ccu/                            ← host stub (Target 1 用, Target 3 不编译)
    │   ├── hccl_op_common_stub.cc          ← ExecuteSelector/CollAlgExecRegistryV2 stub
    │   └── cann_host_bridge_stub.cc        ← CANN bridge stub
    │
    ├── common/                             ← 公共工具 (异常/字符串)
    └── aicpu/                              ← AICPU stub (已有, selector target 不用)
```

### 9.2 三个构建目标

| Target | 名称 | 编译内容 | 验证深度 |
|--------|------|---------|---------|
| 1 | `ascendc_ut_adv_api_hccl_ccu_mc2_host` | `hccl_op_common_stub.cc` (stub selector) + `hccl_mc2.cc` + `hccl_alloc_ctx_res.cc` | tiling→selector→资源序列化 |
| 2 | `ascendc_ut_adv_api_hccl_ccu_mc2_device` | kernel + adapter + RuntimeWorld (直接调 kernel) | kernel 数据面 + KFC server |
| 3 | `ascendc_ut_adv_api_hccl_ccu_selector_st` | **真实** selector/executor/template + `op_common.cc` (真实 HcclGetAlgRes) + adapter + RuntimeWorld + DAG | selector→CalcRes→channel→kernel注册→数据→DAG 端到端 |

### 9.3 分层职责

| 层 | 目录 | 职责 | 被谁复用 |
|----|------|------|---------|
| 用例层 | `testcase/mc2_ccu/selector/` | TEST_F 入口 + target 专属 shim/stub | 仅 Target 3 |
| 测试编排层 | `utils/src/ccu_scenario/` | VerifyScenario (host→selector→CalcRes→捕获→执行→校验) | Target 3 |
| adapter 层 | `utils/src/ccu/` | kernel 捕获 (CCU API → Program IR) + taskArgs 构造 | Target 2+3 |
| 校验器层 | `utils/src/hccl_verifier/` | Program 结构/ABI/语义/DAG 校验 | Target 2+3 |
| 运行时层 | `utils/src/sim_world/` | RuntimeWorld (Program IR 解释执行) + SimWorld (拓扑模拟) | 全部 |
| 拦截层 | `utils/src/hccl_proxy/` + `hccl_depends_stub/` | RegisterManager + ccu_primitives_stub | 全部 (编译成 hccl-model-proxy 库) |

---

## 10. HcommCcuKernelRegister 桥接机制

### 10.1 背景

初始实现中 `HcommCcuKernelRegister`（`hcomm_ccu_stub.cc`）只计数++，不调 `RegisterManager`。导致真实 CalcRes 产出的 `CcuKernelInfo`（含 kernelFunc/kernelArg）没喂给 `RegisterManager`，`VerifyScenario` 需要手动 `RegisterAllRanks` 调 adapter capture function。

改造后 `HcommCcuKernelRegister` 桥接到 `RegisterManager`，真实 CalcRes 自动注册 kernel，`VerifyScenario` 不再需要手动 `RegisterAllRanks`。

### 10.2 改造方案（不耦合方案）

`RegisterManager::Register` 签名不变（收 `KernelFunction = std::function<Result(void*)>`）。`HcommCcuKernelRegister` 通过 capture function 注册表查到 adapter 的 capture function，转换 kernelArg 后调 `RegisterManager::Register`。

```
真实 CalcRes → HcclGetCcuKernel → HcommCcuKernelRegister
  → 查注册表: kernelFuncName="CcuKernelAlltoAllMesh1D" → (captureFunction, argConverter)
  → argConverter: 产品 CcuKernelArgAlltoAllMesh1D* → ST AllToAllKernelConfig*
  → RegisterManager::Register(insHandle, name, captureFunction, stArg, handle)
    → captureFunction(stArg) 调 adapter:
      → LoadArg 加载参数
      → ConfigureProgram(rankId, rankSize, channels)
      → 调产品 CcuAlltoAllMesh1DKernel
      → CCU API 被 ccu_primitives_stub 拦截 → Program IR
    → Program 存入 RegisterManager
```

### 10.3 capture function 注册表

在 `hcomm_ccu_stub.cc` 里：

```cpp
using CaptureFunction = HcclSim::CcuSt::RegisterManager::KernelFunction;
using ArgConverter = std::function<void*(const void*, uint32_t)>;

static std::map<std::string, std::pair<CaptureFunction, ArgConverter>>& CaptureRegistry() {
    static std::map<std::string, std::pair<CaptureFunction, ArgConverter>> registry;
    return registry;
}

extern "C" void RegisterCaptureFunction(const char *name, CaptureFunction capture, ArgConverter convert) {
    CaptureRegistry()[name] = {capture, convert};
}
```

adapter 在静态初始化时注册：

```cpp
// ccu_all_to_all_mesh1d_adapter.cc
static bool registered = []() {
    RegisterCaptureFunction("CcuKernelAlltoAllMesh1D",
        CaptureAllToAllMesh1DKernel, ConvertAllToAllArg);
    return true;
}();
```

### 10.4 kernelArg 转换

产品 `CcuKernelArgAlltoAllMesh1D`（继承 `CcuKernelArgBase`）→ ST `AllToAllKernelConfig`：

| 产品字段 | ST 字段 | 转换 |
|---------|--------|------|
| `rankSize` (uint64_t) | `rankSize` (uint32_t) | 直接转 |
| `rankId` (uint32_t) | `rankId` (uint32_t) | 直接传 |
| `loadFromMem` (bool) | `loadFromMem` (bool) | 直接传 |
| `channels[64]` + `channelCount` | `channels` (vector) | 遍历拷贝，用 MakeChannel 重新编码 |

### 10.5 channel handle 编码转换

产品 `HcclGetChannelForCcu` → `HcclChannelAcquire` → `SimChannelCommCreate` 返回的 channel handle 是 `SimChannel*` 指针。

ST `RuntimeWorld` 和 `ccu_primitives_stub` 用 `MakeChannel(localRank, remoteRank)` 编码 `(localRank << 32) | remoteRank`。

转换方式：按 channels 数组顺序推导 peer rank（`CalcChannelRequestMesh1D` 按 peer rank 升序排列，跳过 self rank），用 `MakeChannel` 重新编码。

### 10.6 当前状态

| 算法 | 状态 | 说明 |
|------|------|------|
| AllToAll | ✅ 完全自动注册 | 每个 rank 跑完整 host flow（SimWorld→comm→CalcRes→HcommCcuKernelRegister），5 个测试通过 |
| AllToAllV | ✅ 完全自动注册 | CalcRes 按 opType 分支注册 `CcuKernelAlltoAllVMesh1D`，6 个测试通过 |
| DAG | ✅ 通过 | 7 个测试通过 |

### 10.7 改造中解决的问题

| 问题 | 解决方式 |
|------|---------|
| 静态初始化顺序（SIOF） | 注册表用函数内局部 static |
| `COMM_ENGINE_CCU` 未处理 | `SimThreadMgr::CommEngineToNotifyLoadType` 加 case |
| Channel 创建超时（等对端） | 新增 `ChannelCommCreateForCcu` 同时建双向 channel |
| `HcclDfxRegOpInfoByCommId` weak 符号未链接 | stubs.cc 加强定义 |
| 多 rank kernel 注册 | 每个 rank 跑完整 host flow（Init→comm→CalcRes→Register），无需手动注册。`aclrtCreateStream` 必须在 `aclrtSetDevice` 之后调 |

---

## 11. 边界声明

### 11.1 覆盖

- 真实 selector 算法选择逻辑
- 真实 template.CalcRes 构建 CcuKernelInfo
- 真实 channel 分配
- 真实 collective kernel 的 CCU API 调用形态
- Program 结构、ABI、collective 契约校验
- DAG 并发冲突检查
- RuntimeWorld 多 Rank 执行 + deadlock 检测
- 独立期望字节结果对比

### 11.2 不覆盖

- KFC server kernel（queue/CKE/wrap/finalize）
- `HcclAllocComResourceByTiling` 全链编排（cache/共享 server）
- `CcuKernelLaunch` 的 D2H copy + Launch 编排
- InheritKfcServerKernelArg（KFC server 替换逻辑）
- HcommCcuKernelRegister 真实微码录制（桥接到 RegisterManager，但不录制真实微码/Translator/CcuInstr）
- 真实 Translator、微码、CcuInstr
- 真实 XN/GSA/CKE/MS 物理资源分配
- 多 Mission/多 LoopEngine 真实并发
- GroupCopy 内部 MS/LoopEngine pipeline 逐节点执行
- 驱动、硬件时序、性能
- 浮点逐元素数值精度

---

## 12. 新增算子接入指南

> **本节为早期设计。最新的新增算法指南见 §15.15，以 §15.15 为准。**

以 AllToAllV 为例，新增一个 CCU 算子的 ST 接入需要修改以下内容。

### 12.1 产品代码（如需修复）

如果 CalcRes 没有按 opType 注册正确的 kernel（如 AllToAllV 之前的 bug），需要在 `ccu_temp_all_to_all_mesh_1D.cc` 的 CalcRes 里分支：

```cpp
if (param.opType == HcclCMDType::HCCL_CMD_ALLTOALLV) {
    strcpy(kernelInfo.kernelFuncName, "CcuKernelAlltoAllVMesh1D");
    kernelInfo.kernelFunc = reinterpret_cast<void *>(CcuAlltoAllVMesh1DKernel);
} else {
    strcpy(kernelInfo.kernelFuncName, "CcuKernelAlltoAllMesh1D");
    kernelInfo.kernelFunc = reinterpret_cast<void *>(CcuAlltoAllMesh1DKernel);
}
```

### 12.2 ST adapter 层

| 文件 | 动作 | 说明 |
|------|------|------|
| `utils/src/ccu/ccu_<algo>_adapter.h` | **新增** | 定义 `<Algo>KernelConfig`、`<Algo>LaunchConfig`、`Capture<Algo>Kernel`、`Prepare<Algo>TaskArgs` |
| `utils/src/ccu/ccu_<algo>_adapter.cc` | **新增** | 实现 capture function + `Convert<Algo>Arg`（产品 kernelArg → ST config）+ 静态注册 `RegisterCaptureFunction("CcuKernel<Algo>", ...)` |

adapter 需要做的 3 件事：
1. **Capture function**：LoadArg 加载参数 → ConfigureProgram → 调真实 kernel → CCU API 被 stub 拦截成 Program IR
2. **Arg converter**：产品 `CcuKernelArg<Algo>*` → ST `<Algo>KernelConfig*`（含 rankId/rankSize/channels 转换）
3. **静态注册**：`RegisterCaptureFunction("CcuKernel<Algo>", capture, convert, deleter)`，让 `HcommCcuKernelRegister` 能按 `kernelFuncName` 查到

### 12.3 校验器层

| 文件 | 动作 | 说明 |
|------|------|------|
| `utils/src/hccl_verifier/semantics_check/ccu_<algo>_semantics_checker.h` | **新增** | 定义 `<Algo>RuntimeExpectation` + `CheckRegistration`/`CheckTaskArgs`/`CaptureRuntimeExpectation`/`CheckRuntime` |
| `utils/src/hccl_verifier/semantics_check/ccu_<algo>_semantics_checker.cc` | **新增** | 实现算法专属校验逻辑 |

需要实现 4 个方法：
- **CheckRegistration**：Program 结构 + 操作计数 + 同步契约（每种 OpCode 的数量和 mask 校验）
- **CheckTaskArgs**：taskArgs ABI（数量 + 值匹配）
- **CaptureRuntimeExpectation**：按算法语义独立计算期望 output（等长用 sliceSize/srcStride，变长用参数表 sendSize/sendOffset/recvOffset）
- **CheckRuntime**：逐字节对比实际 output vs 期望 output

### 12.4 测试编排层

| 文件 | 动作 | 说明 |
|------|------|------|
| `utils/src/ccu_scenario/ccu_<algo>.h` | **新增** | 定义 `CcuSt<Algo>` 类，继承 `CcuStFixture`，声明 `VerifyScenario` |
| `utils/src/ccu_scenario/ccu_<algo>.cc` | **新增** | 实现 `VerifyScenario` + `BuildScenarioData` + `RegisterKernelForRank` + `Build<Algo>Tiling` + `BuildTopoMeta` + `CleanupHostEnv` |

VerifyScenario 的 5 个阶段（详见 §3.4）：
1. 初始化：Reset + 构造 topo/tiling
2. per-rank host flow：SimWorld → comm → selector → CalcRes → HcommCcuKernelRegister → RegisterManager（全自动）
3. 构造测试数据 + 静态校验：BuildScenarioData + CheckRegistration + CheckTaskArgs
4. 反例分支：swapTaskArgs/missingTaskArg/outOfRangeParam
5. 执行 + 校验：RuntimeWorld::Launch → CcuDagChecker → CheckRuntime

### 12.5 用例层

| 文件 | 动作 | 说明 |
|------|------|------|
| `testcase/mc2_ccu/selector/<algo>_selector_testcase.cpp` | **新增** | TEST_F 用例，调 `VerifyScenario(MakeScenario(...))` |

典型用例：
- 正例：2-rank / 4-rank 正常数据传输
- 反例：corruptOutput（篡改 output）、swapTaskArgs（交换参数）、missingTaskArg（缺参数）、outOfRangeParam（参数越界）

### 12.6 CMake

| 文件 | 动作 | 说明 |
|------|------|------|
| `testcase/mc2_ccu/CMakeLists.txt` | **修改** | `HCCL_SELECTOR_PRODUCT_SOURCES` 加新算法的 selector/executor/template/kernel `.cc` + adapter `.cc` + scenario `.cc`；`add_executable` 加 testcase `.cpp` |

### 12.7 CcuStScenario 扩展

如果新算法需要额外字段（如 `reduceType`/`count`/`outputType`/`rootRank`），在 `ccu_scenario.h` 的 `CcuStScenario` 里加。

### 12.8 完整清单

新增一个 CCU 算子的完整改动清单：

```
产品代码 (如需修复):
  ☐ ccu_temp_<algo>.cc CalcRes 按 opType 注册正确 kernelFuncName/kernelFunc

ST adapter:
  ☐ utils/src/ccu/ccu_<algo>_adapter.h    — KernelConfig/LaunchConfig/Capture/PrepareTaskArgs
  ☐ utils/src/ccu/ccu_<algo>_adapter.cc   — capture + ConvertArg + RegisterCaptureFunction

校验器:
  ☐ utils/src/hccl_verifier/semantics_check/ccu_<algo>_semantics_checker.h
  ☐ utils/src/hccl_verifier/semantics_check/ccu_<algo>_semantics_checker.cc

测试编排:
  ☐ utils/src/ccu_scenario/ccu_<algo>.h
  ☐ utils/src/ccu_scenario/ccu_<algo>.cc

用例:
  ☐ testcase/mc2_ccu/selector/<algo>_selector_testcase.cpp

CMake:
  ☐ testcase/mc2_ccu/CMakeLists.txt — 加源文件 + include path

场景参数 (如需):
  ☐ utils/src/ccu_scenario/ccu_scenario.h — 加算法专属字段
```

---

## 13. PR_4721 Host 域与 Device 域用例梳理

PR_4721 包含三个构建目标，各自验证不同层面。本章梳理 Host 域和 Device 域（含 KFC Device）的用例与验证点。

### 13.1 Host 域（Target 1: `ascendc_ut_adv_api_hccl_ccu_mc2_host`）

**文件**: `testcase/mc2_ccu/host/all_to_all_v_mc2_host_testcase.cpp`

**入口**: MC2 路径（`CcuSelectAlg` / `HcclAllocComResourceByTiling`），selector 是 stub（`hccl_op_common_stub.cc`）

**验证深度**: tiling → selector → 资源分配 → 资源序列化（不执行 kernel）

**用例与验证点**:

| 用例 | 验证点 |
|------|--------|
| `AllocCcuOpResCtxInitializesQueueResources` | `AllocCcuOpResCtx` 返回成功；`resCtx.rankId`/`rankSize`/`workSpaceSize` 正确；`workSpace`/`xnAddr`/`ckeAddr` 非零且互不相同 |
| `SelectsAllToAllVMesh1D` | `CcuSelectAlg` 返回成功；`opType` = `HCCL_CMD_ALLTOALLV`；`algorithmType` = `CcuAlltoAllVMesh1D`；`GetCcuKernelRegisterCount` = 1（kernel 注册计数） |
| `MultipleAllToAllVConfigsShareOneServer` | 2 个 tiling 共享 1 个 KFC server；`GetCcuKernelRegisterCount` = 1；两个 opParam 的 `resCtx`/`ctxSize` 相同 |
| `OpIdContainsAllToAllVMessageIdentity` | `BuildKfcOpId` 返回值：低 7 位 = opType；8-15 位 = sendType；16-23 位 = 0；24-31 位 = recvType |
| `MultiConfigCacheDoesNotReuseSingleConfigServer` | 单 tiling 和多 tiling 不复用 KFC server；`GetCcuKernelRegisterCount` = 2；`resCtx` 不同 |
| `AllocatesAllToAllVResourceFromTiling` | `HcclAllocComResourceByTiling` 返回成功；`opType`/`algorithmType`/`rankId`/`rankSize` 正确 |
| `KfcServerArgsUseAllocatedQueueAndToken` | `SelectAlgAndPrepareEngine` 选出 `CcuAlltoAllVMesh1D`；`HandleSingleRankAndCommMode` 不跳过；`GetCcuOpParamResCtx` 返回非空；反序列化 `kfcServerArgs`：size=6，`[0]`=xnAddr，`[1]`=ckeAddr，`[2]`=1(dieNum)，`[3]`=1(missionNum)，`[4]`=0(missionIndex)，`[5]`=token |
| `AcceptsBothCcuTilingEngineValues` | `ObtainCommEngine` 接受 `CCU_MS` 和 `CCU_SCHED` 两种 engine 值 |
| `ValidatesNullInputs` | `HcclAllocComResourceByTiling` 拒绝 null comm/stream/tiling |

### 13.2 Device 域（Target 2: `ascendc_ut_adv_api_hccl_ccu_mc2_device`）

**文件**: `testcase/mc2_ccu/device/all_to_all_v_kernel_device_testcase.cpp`

**入口**: 直接调 kernel + adapter（不走 selector/CalcRes），通过 `RegisterManager` 手动捕获

**验证深度**: kernel CCU API 调用形态 → Program IR → 校验 → RuntimeWorld 执行 → 数据对比

**用例与验证点**:

| 用例 | 验证点 |
|------|--------|
| `registration_ir_matches_current_mesh1d_kernel` | `RegisterManager::Register` 捕获成功；Program 非空；`CcuProgramChecker` 结构校验通过；`CcuAllToAllVSemanticsChecker::CheckRegistration` 通过（操作计数 + 同步契约） |
| `ten_dynamic_arguments_fit_one_task_mission` | `CcuTaskArgChecker` 校验 10 个 taskArgs 放入 1 个 Mission（13 槽，用 10 槽） |
| `two_rank_asymmetric_sizes_and_offsets` | 2-rank 非对称数据；`CheckTaskArgs` 通过；`CaptureRuntimeExpectation` 成功；`RuntimeWorld::Launch` 成功；`CheckRuntime` 逐字节对比通过 |
| `four_rank_supports_zero_length_peers` | 4-rank 含零长度 peer；执行 + 数据对比通过 |
| `launch_rejects_out_of_range_parameter_table` | parameterAddress 越界；`Launch` 返回 `OUT_OF_RANGE` |
| `runtime_checker_rejects_output_corruption` | 篡改 output 字节；`CheckRuntime` 检测到不匹配 |
| `checker_rejects_task_argument_rebinding` | 交换两个 taskArg；`CheckTaskArgs` 检测到不匹配 |
| `launch_rejects_missing_dynamic_argument` | 删一个 taskArg；`Launch` 返回 `CONTRACT_ERROR` |

### 13.3 KFC Device 域（Target 2 含 KFC 用例）

**文件**: `testcase/mc2_ccu/device/all_to_all_v_kfc_device_testcase.cpp`

**入口**: 手动构造 KFC server kernel + collective kernel + queue + message → `CcuKfcSimWorld::DispatchOne`

**验证深度**: KFC server IR → queue 管理 → dispatch → collective kernel 执行 → 数据对比

**用例与验证点**:

| 用例 | 验证点 |
|------|--------|
| `real_server_ir_matches_current_queue_and_dispatch_contract` | KFC server kernel 注册成功；Program 非空；`CcuKfcChecker::CheckServerProgram` 校验通过（server IR 结构 + opId） |
| `startup_arguments_bind_real_server_and_queue_resources` | `MakeKfcRegistration` 成功；`CcuKfcChecker::CheckRegistration` 通过（kernel 注册 + queue 绑定） |
| `two_rank_asymmetric_dispatch_end_to_end` | 2-rank 非对称；`DispatchOne` 成功；`CcuAllToAllVSemanticsChecker::CheckRuntime` 逐字节对比通过；`CheckQueue` 通过 |
| `four_rank_zero_length_peer_dispatch_end_to_end` | 4-rank 含零长度 peer；dispatch + 数据对比通过 |
| `peer_table_uses_current_send_reserved_send_offset_recv_offset_abi` | `CheckPeerTable` 校验参数表 ABI（sendSize/reserved/sendOffset/recvOffset 4 字段 × rankSize） |
| `queue_executes_multiple_slots_and_wraps_after_64` | queue 64 槽循环：提交 65 条消息，第 65 条回到 slot 0；`CheckQueue` 校验所有 slot 状态正确 |
| `queue_rejects_reuse_of_uncompleted_slot` | 重复提交未完成的 slot；queue 返回错误 |
| `dispatch_rejects_unregistered_op_id` | dispatch 收到未注册的 opId；返回错误 |
| `message_checker_rejects_truncated_peer_table` | peer table 被截断；`CheckMessage` 检测到错误 |
| `collective_finalize_stops_without_completion_signal` | 无 finalize 信号；dispatch 不完成 |

### 13.4 三个域验证深度对比

| 维度 | Host 域 | Device 域 | KFC Device 域 | Selector 域 (我们) |
|------|---------|-----------|---------------|-------------------|
| 入口 | `CcuSelectAlg` (MC2) | 手动调 kernel | 手动构造 KFC message | `SelectAlgAndPrepareEngine` + `GetCcuOpParamResCtx` (MC2) |
| selector | stub | ❌ 不走 | ❌ 不走 | **真实** |
| CalcRes | stub | ❌ 不走 | ❌ 不走 | **真实** |
| channel 分配 | stub | ❌ 不走 | ❌ 不走 | **真实** |
| kernel 注册 | 计数++ | 手动 RegisterManager | 手动 RegisterManager | **真实 HcommCcuKernelRegister 桥接** |
| kernel 执行 | ❌ | RuntimeWorld | CcuKfcSimWorld + RuntimeWorld | RuntimeWorld |
| Program IR | ❌ | ✅ | ✅ (server + collective) | ✅ |
| 数据对比 | ❌ | ✅ 逐字节 | ✅ 逐字节 | ✅ 逐字节 |
| DAG | ❌ | ❌ | ❌ | ✅ |
| KFC server | 序列化校验 | ❌ | ✅ queue + dispatch | ❌ |
| 用例数 | 9 | 8 | 10 | 18 |

### 13.5 各域互补关系

```
Host 域:
  验证 tiling→selector→资源序列化 (不执行 kernel)
  └─ kfcServerArgs 6 字段、算法选择、资源复用、null 检查

Device 域:
  验证 kernel CCU API 调用形态 + 数据搬运正确性 (不走 selector/CalcRes)
  └─ Program IR 结构、操作计数、同步契约、taskArgs ABI、逐字节数据

KFC Device 域:
  验证 KFC server queue + dispatch + collective kernel 端到端
  └─ server IR、queue 64 槽循环、dispatch、peer table ABI、finalize

Selector 域 (我们):
  验证 selector→CalcRes→channel→kernel 注册→执行→数据→DAG 全链
  └─ 真实 selector 选算法、真实 CalcRes 构建 kernelInfo、真实 channel 分配、
      HcommCcuKernelRegister 桥接 RegisterManager、DAG 并发冲突检测
```

---

## 14. 方案 A 与 PR_4721 优缺点对比

### 14.1 架构对比

| 维度 | PR_4721 (原始) | 方案 A (我们) |
|------|---------------|--------------|
| selector | stub（`ExecuteSelector` 硬编码返回 algName） | 真实 `AlltoAllAutoSelector::SelectCcuScheduleAlgo` |
| CalcRes | stub（`HcclGetAlgRes` 不调 `executor->CalcRes`） | 真实 `executor->CalcRes` 构建 `CcuKernelInfo` |
| channel 分配 | adapter 硬编码 `MakeChannel(rank, peer)` | 真实 `HcclGetChannelForCcu` → `HcclChannelAcquire` |
| kernel 注册 | 手动 `RegisterManager::Register` + adapter capture | 真实 `HcommCcuKernelRegister` 桥接到 RegisterManager |
| kernel 执行 | RuntimeWorld（Program IR 解释执行） | 同 |
| 数据校验 | 逐字节对比 | 同 |
| DAG | ❌ 无 | ✅ 并发冲突检测 |
| KFC server | ✅ KFC Device 域 | ❌ 不覆盖 |
| Host 资源 | ✅ Host 域 | ✅ 集成在 VerifyScenario 里 |
| 多 rank | 手动 RegisterAllRanks | per-rank host flow 全自动 |

### 14.2 PR_4721 优点

| 优点 | 理由 |
|------|------|
| **简单直接** | adapter 硬编码 config，直接调 kernel，不需要 host 环境（SimWorld/comm/tiling），用例代码量少 |
| **kernel 隔离测试** | 直接调 kernel 函数，不受 selector/CalcRes/channel 的 bug 影响，能精确定位 kernel 自身的问题 |
| **KFC server 完整覆盖** | KFC Device 域验证了 queue 64 槽循环、dispatch、peer table ABI、finalize 等 KFC 协议细节 |
| **编译依赖少** | 只编译 kernel + adapter + ST 框架，不需要编译 selector/executor/template/op_common.cc，编译快 |
| **用例独立** | 每个 target 独立编译，Host/Device/KFC 三个域互不影响，一个域的编译失败不影响其他域 |
| **反例覆盖全** | Device 域有 4 个反例（corruptOutput/swapTaskArgs/missingTaskArg/outOfRangeParam），KFC 域有 5 个反例 |

### 14.3 PR_4721 缺点

| 缺点 | 理由 |
|------|------|
| **不验证 selector** | `ExecuteSelector` 是 stub，硬编码返回 algName。selector 的拓扑判断/算法选择逻辑未被覆盖。如果 selector 在特定拓扑下选了错误算法，PR_4721 检测不到 |
| **不验证 CalcRes** | `HcclGetAlgRes` 是 stub，不调 `executor->CalcRes`。template 构建 `CcuKernelInfo`（kernelFunc/kernelArg/channels）的逻辑未被覆盖。如果 CalcRes 构建了错误的 kernelArg（channel 顺序错、rankSize 不匹配、loadFromMem 设错），PR_4721 检测不到 |
| **channel 硬编码** | adapter 用 `MakeChannel(rankId, peer)` 硬编码 channel handle，不走 `HcclGetChannelForCcu`。如果真实 channel 分配产出的 handle 编码和 adapter 不一致，PR_4721 检测不到 |
| **kernel 注册不验证** | `HcommCcuKernelRegister` 只计数++，不调 `RegisterManager`。kernel 注册时序（RegisterStart/Register/RegisterEnd）未被覆盖。如果 CalcRes 产出的 kernelFuncName 和注册表不匹配，PR_4721 检测不到 |
| **config 和产品代码脱节** | adapter 硬编码的 `AllToAllVKernelConfig`（rankId/rankSize/channels）和产品 CalcRes 产出的 `CcuKernelArgAlltoAllMesh1D` 字段可能不一致。产品代码改了 kernelArg 结构体，adapter 不会自动同步 |
| **无 DAG** | 不检查并发内存冲突。如果 kernel 的同步逻辑有 bug（如 PreSync 的 notify index 写错导致同步边缺失），PR_4721 的 RuntimeWorld 能检测到 deadlock，但不能检测"不 deadlock 但有数据竞争"的隐蔽 bug |
| **Host/Device 割裂** | Host 域验证资源序列化，Device 域验证 kernel 执行，但两者之间没有端到端串联。Host 域产出的资源没有被 Device 域使用，Device 域的 config 是自己构造的 |

### 14.4 方案 A 优点

| 优点 | 理由 |
|------|------|
| **端到端覆盖** | 从 selector → CalcRes → channel → kernel 注册 → 执行 → 数据 → DAG 全链贯通。任何一个环节的 bug 都能被后续环节检测到 |
| **真实 selector** | 调真实 `AlltoAllAutoSelector::SelectCcuScheduleAlgo`，覆盖拓扑判断/算法选择逻辑。如果 selector 在特定拓扑下选了错误算法，`expectedAlgName` 断言会失败 |
| **真实 CalcRes** | 调真实 `executor->CalcRes`，覆盖 template 构建 `CcuKernelInfo` 的逻辑。如果 CalcRes 构建了错误的 kernelArg，kernel 执行时数据搬运会出错，`CheckRuntime` 逐字节对比会检测到 |
| **真实 channel 分配** | 调真实 `HcclGetChannelForCcu` → `HcclChannelAcquire`，channel handle 是 `SimChannel*` 指针，adapter 做 `MakeChannel` 转换。如果 channel 分配有 bug（如 channel 数量不对、peer 映射错），kernel 执行时会 deadlock 或数据错 |
| **HcommCcuKernelRegister 桥接** | 真实 `HcommCcuKernelRegister` 桥接到 `RegisterManager`，通过 capture 注册表自动分发到 adapter。如果 CalcRes 产出的 `kernelFuncName` 和注册表不匹配（如 AllToAllV 的 template bug），注册失败，用例会报错 |
| **DAG 并发冲突检测** | 执行时记录每个数据操作的读写区间，事后建 DAG 检查无同步的并发内存访问。能检测"不 deadlock 但有数据竞争"的隐蔽 bug |
| **config 自动同步** | kernelArg 从产品 CalcRes 产出，通过 `ConvertAllToAllArg` 转换成 ST config。产品代码改了 kernelArg 结构体，只要转换函数同步改，config 就自动正确 |
| **per-rank 全自动** | 每个 rank 独立跑完整 host flow（SimWorld → comm → CalcRes → Register），不需要手动构造 config |
| **集成 Host 验证** | VerifyScenario 里包含 kfcServerArgs 6 字段校验、algName 断言、资源序列化校验，不需要单独的 Host target |
| **algConfig 控制** | 支持 `algConfig` 字段强制指定算法，测试不同算法的同一原语 |
| **可扩展** | 新增算法只需加 adapter + checker + 用例，通用层（期望计算 + 数据对比 + DAG）不需要改 |

### 14.5 方案 A 缺点

| 缺点 | 理由 |
|------|------|
| **编译依赖多** | 需要编译真实 selector/executor/template/op_common.cc + 大量 shim/stub 解决 transitive include，编译慢 |
| **host 环境复杂** | 需要 SimWorld/comm/tiling/topo 环境，`aclrtCreateStream` 必须在 `aclrtSetDevice` 之后调，SimWorld Init/Deinit 有生命周期约束 |
| **channel handle 转换** | 产品 channel handle 是 `SimChannel*` 指针，ST 用 `MakeChannel` 编码。`ConvertAllToAllArg` 按数组顺序推导 peer rank，不是从 handle 解析——如果 channel 顺序不是按 peer rank 升序排列，转换会错 |
| **adapter 仍然需要** | 因为 asc-devkit 的 kernel 签名是展开参数（不是 hccl 仓的 `CcuKernelArg arg` 单参数），需要 adapter 做签名适配。如果 kernel 签名改了，adapter 要同步改 |
| **KFC server 不覆盖** | 不验证 KFC server 的 queue/dispatch/finalize 协议。如果 KFC server 的 dispatch 逻辑有 bug，方案 A 检测不到 |
| **DAG 难以通过产品代码注入触发** | CheckRegistration 的操作计数校验太严格，大部分同步相关的 bug 会先被操作计数拦住，到不了 DAG 层。DAG 主要靠直接构造 ExecutionLog 的专项用例验证 |
| **per-rank host flow 较慢** | 每个 rank 都要 Init/Deinit SimWorld + 跑完整 host flow，4-rank 测试比 PR_4721 的手动注册慢 |
| **产品代码耦合** | 改了产品 kernel 签名/kernelArg 结构体/CalcRes 逻辑，ST 可能编译失败或运行失败。PR_4721 的 adapter 硬编码 config 不受产品代码变更影响（只要 kernel 函数签名不变） |

### 14.6 总结

| 维度 | PR_4721 | 方案 A | 谁更好 |
|------|---------|--------|--------|
| **selector 覆盖** | ❌ stub | ✅ 真实 | 方案 A |
| **CalcRes 覆盖** | ❌ stub | ✅ 真实 | 方案 A |
| **channel 覆盖** | ❌ 硬编码 | ✅ 真实 | 方案 A |
| **kernel 注册覆盖** | ❌ 手动 | ✅ 桥接 | 方案 A |
| **kernel 执行** | ✅ RuntimeWorld | ✅ 同 | 平 |
| **数据校验** | ✅ 逐字节 | ✅ 同 | 平 |
| **DAG** | ❌ 无 | ✅ 有 | 方案 A |
| **KFC server** | ✅ 完整 | ❌ 无 | PR_4721 |
| **Host 资源** | ✅ 独立 target | ✅ 集成 | 平 |
| **编译复杂度** | ✅ 简单 | ❌ 复杂 | PR_4721 |
| **用例独立性** | ✅ 三域独立 | ❌ 一体化 | PR_4721 |
| **config 同步** | ❌ 硬编码可能脱节 | ✅ 从 CalcRes 产出 | 方案 A |
| **可扩展性** | ❌ 每个算法独立硬编码 | ✅ 通用层复用 | 方案 A |
| **bug 检测能力** | kernel 自身 bug | 全链 bug | 方案 A |

**核心差异**：PR_4721 是"对 kernel 做 UT"——隔离测试 kernel 函数自身的正确性。方案 A 是"对全链做 ST"——端到端验证 selector→CalcRes→channel→kernel→数据，能检测全链任何一个环节的 bug。

**互补关系**：PR_4721 的 KFC Device 域覆盖了方案 A 不覆盖的 KFC server 协议。方案 A 覆盖了 PR_4721 不覆盖的 selector/CalcRes/channel。两者合并后覆盖最完整。

### 14.7 复杂度量化对比

对方案 A 简化为 PR_4721 模式的可行性和收益进行量化分析。

#### 代码量对比

| 组件 | PR_4721（Host + Device + KFC 三域） | 方案 A（Selector 单域） |
|------|-------------------------------------|----------------------|
| testcase | 300 + 240 + 331 = 871 行 | 68 + 65 + 216 = 349 行 |
| stubs | hccl_op_common_stub 366 + cann_bridge 25 + hcomm_ccu_stub 407 = 798 行 | stubs.cc ~350 行 |
| adapter | 已有（ccu_all_to_all_v_adapter） | + AllToAll adapter ~90 行 |
| scenario/fixture | ❌ 无 | ~600 行 |
| DAG | ❌ 无 | checker 215 行 + types.h 50 行 = 265 行 |
| shim | ❌ 无 | ~10 行 |
| **总计** | **~1669 行** | **~1763 行** |

**结论**：代码量基本持平（1669 vs 1763），简化不会显著减少代码。

#### 全链验证必要性审视

| 环节 | 有复杂逻辑/分支吗 | Host 域已覆盖吗 | ST 全链能发现新 bug 吗 |
|------|-------------------|-----------------|---------------------|
| selector | 纯 if/else 拓扑判断 | ✅ | ❌ |
| CalcRes | 静态字段赋值 | ✅ | ❌ |
| channel | SimChannel 创建 | ✅ | ❌ |
| kernel 注册 | stub 计数 | ✅ | ❌ |

全链验证的价值不在发现这四个环节自身的 bug（它们逻辑简单，Host 域已覆盖），而在于证明"Host 产出的结果能被 kernel 正确使用"——即串联 Host 和 Device。

#### Host/Device 割裂问题

PR_4721 的 Host 域和 Device 域是割裂的：

```
Host 域:                          Device 域:
  selector → CalcRes → channel      adapter 硬编码 config
  → kernel 注册 → 序列化             → RegisterManager → RuntimeWorld
  → kfcServerArgs 校验               → 数据对比
  ↓                                  ↑
  产出的 kernelArg 没有被 Device 用   Device 用的 config 不是 Host 产出的
```

Host 域验证了"资源序列化正确"，Device 域验证了"kernel 执行正确"，但没有人验证"Host 产出的资源能被 Device 正确使用"。

方案 A 的 Selector 域把这个缝隙补上：

```
Selector 域:
  selector → CalcRes → channel → kernel 注册 → RegisterManager → RuntimeWorld → 数据对比
  ↑                                                                    ↑
  Host 产出的 kernelArg 直接被 Device 使用（通过 HcommCcuKernelRegister 桥接）
```

但这正是复杂度的来源——串联 Host 和 Device 需要跑完整 host flow + 桥接机制。

#### 简化方案评估

| 简化方案 | 做法 | 代码减少 | 代价 |
|---------|------|---------|------|
| 方案 1: rank 0 全链 + rank 1..N-1 手动 | rank 0 走真实 host flow，其他 rank adapter 手动注册 | ~100 行 | rank 1..N-1 不覆盖真实 CalcRes（但 rank 0 覆盖了） |
| 方案 2: 全部简化成 PR_4721 模式 + DAG | 删掉 host flow / stubs.cc / shim，adapter 手动注册 | ~460 行 | Host/Device 割裂，丢掉全链串联 |
| 方案 3: 保持现状 + 拆分函数 | 不改逻辑，VerifyScenario 拆成 4 个子函数 | 0 行 | 可读性提升，复杂度不变 |

#### 结论

- **代码量角度**：简化收益不大（1669 vs 1763），只是把复杂度从"host flow + 桥接"换成了"三个独立 target"
- **可维护性角度**：三个独立 target 确实比一个一体化 target 更容易维护
- **覆盖角度**：简化后丢掉全链串联和 DAG，但 Host/Device 割裂问题依然存在
- **建议**：如果简化，推荐方案 1（rank 0 全链 + rank 1..N-1 手动）——保留全链验证能力，减少 per-rank host flow 的复杂度；或方案 3（拆分函数）——不改逻辑，只提升可读性

---

## 15. 迁移到 feat/alltoall-migration 分支的文件清单

### 15.1 迁移范围

从 `feat/ccu-st-selector-calcres` 分支迁移 ST 框架到 `feat/alltoall-migration` 分支。

**迁移原则**：
- 只迁移 selector target 端到端用例（方案 A），不迁移 Host target 和 Device target
- 只迁移 AllToAll 相关文件，不迁移 AllToAllV
- 不迁移 KFC server 相关文件（adapter/sim/checker/proxy）
- 不迁移 docs/skills
- 产品代码零修改，所有适配在 ST 侧完成

**去掉的内容**：
- AllToAllV testcase/adapter/scenario/checker（13 个文件）
- KFC server adapter/sim_world/checker/proxy（10 个文件）
- Host target testcase + Host stubs（3 个文件）
- Device target testcase + Device shim + link stub（10 个文件）
- docs/skills（19 个文件）

### 15.2 基于 master 的新增文件（22 个）

#### A. Program IR / Stub 层（5 个）— ST 框架核心基础设施

| # | 文件路径 | 行数 | 作用 |
|---|---------|------|------|
| 1 | `utils/src/hccl_proxy/ccu_st_types.h` | 156 | **Program IR 类型定义**：`Result` 枚举（SUCCESS/PARAM_ERROR/DEADLOCK 等）、`OpCode` 枚举（19 种操作：LOAD_ARG/WRITE/NOTIFY_RECORD/IF_BEGIN/WHILE_BEGIN/GROUP_COPY 等）、`Operation` 结构体（一条 IR 指令）、`Program` 结构体（operations/arguments/variables/addresses/events/buffers/channelVariables/groupCopies） |
| 2 | `utils/src/hccl_proxy/ccu_kernel_proxy.h` | 68 | **RegisterManager 单例声明**：`Register()` 接收 kernel 函数指针+参数执行捕获、`GetProgram(handle)` 取回 IR、`RegisterCaptureFunction()` 注册 adapter 捕获函数（按 kernelFuncName 查找） |
| 3 | `utils/src/hccl_proxy/ccu_kernel_proxy.cc` | 156 | **RegisterManager 实现**：创建 CompilerContext → 设置 Program 的 rankId/rankSize/channels → 调用 kernel 函数（CCU API 被 stub 拦截写入 Program）→ Finalize 校验 IF/WHILE 嵌套闭合 + argId 连续性 |
| 4 | `utils/src/hccl_depends_stub/ccu_primitives_stub.h` | 208 | **CCU primitives stub 声明**：`CompilerContext` 类（IR 构建器）、`Variable`/`Address`/`Event`/`LocalAddr`/`RemoteAddr` 等 CCU 类型的 stub（内部存 ResourceHandle）、`CCU_IF`/`CCU_WHILE` 宏的 stub（生成 IF_BEGIN/WHILE_BEGIN Operation） |
| 5 | `utils/src/hccl_depends_stub/ccu_primitives_stub.cc` | 495 | **CCU primitives stub 实现**：`ccu::LoadArg/Load/Store/Write/LocalCopy/WriteVariableWithNotify/NotifyRecord/NotifyWait/EventRecord/EventWait/GetResByChannel/GroupCopy` 全部 CCU API 的 stub（每个调用转成 Operation 追加到 Program） |

#### B. Sim World（2 个）— IR 执行引擎

| # | 文件路径 | 行数 | 作用 |
|---|---------|------|------|
| 6 | `utils/src/sim_world/ccu_sim_world.h` | 71 | **RuntimeWorld 声明**：`RankMemory`（input/output/parameter 内存）、`RankLaunch`（kernelHandle+taskArgs+memory）、`RuntimeWorld` 类（Launch 执行多 rank Program、ExecutionLogData 取日志、Stopped 检查死锁） |
| 7 | `utils/src/sim_world/ccu_sim_world.cc` | 393 | **RuntimeWorld 实现**：轮询调度每 rank 执行一条 Operation（LOAD_ARG→取 taskArgs 值、WRITE→跨 rank memmove、NOTIFY_WAIT→检查 mask 阻塞、IF→条件跳转）；记录 ExecutionLogEntry（含已解析内存区间）；全部完成→SUCCESS，全部阻塞→DEADLOCK |

#### C. Checkers（7 个）— 校验层

| # | 文件路径 | 行数 | 作用 |
|---|---------|------|------|
| 8 | `utils/src/hccl_verifier/ccu_checker.h` | 56 | **通用 checker 声明**：`CheckResult`（result+message）、`CcuLogicalResourceChecker`（资源 handle 唯一性检查）、`CcuProgramChecker`（IF/WHILE 嵌套+argId 连续+LoadArg 绑定一致性）、`CcuTaskArgChecker`（taskArgs 数量/13 槽分组校验） |
| 9 | `utils/src/hccl_verifier/ccu_checker.cc` | 246 | **通用 checker 实现**：遍历 variables/addresses/events/buffers 检查 handle 唯一性；检查 operations 序列嵌套/连续/绑定；按 TASK_PARAM_SLOT_COUNT=13 分组校验 taskArgs 映射 |
| 10 | `utils/src/hccl_verifier/ccu_dag_checker.h` | 36 | **DAG 冲突检测声明**：`CcuDagChecker::Check(ExecutionLog)` 从执行日志构建 DAG 检测无同步的并发内存冲突 |
| 11 | `utils/src/hccl_verifier/ccu_dag_checker.cc` | 215 | **DAG 冲突检测实现**：BuildDag（程序顺序边+NOTIFY/EVENT 同步边）、HasConflict（区间重叠+至少一个写+DAG 不可达→冲突）、ReachableFrom（BFS 可达性分析） |
| 12 | `utils/src/hccl_verifier/ccu_dag_types.h` | 72 | **DAG 类型定义**：`MemoryInterval`（ownerRank/address/size/isWrite）、`ExecutionLogEntry`（rank/operationIndex/source/destination）、`DagNode`（entry+predecessors/successors） |
| 13 | `utils/src/hccl_verifier/semantics_check/ccu_all_to_all_semantics_checker.h` | 38 | **AllToAll 语义 checker 声明**：`AllToAllRuntimeExpectation`（期望 output 字节）、4 个方法：CheckRegistration/CheckTaskArgs/CaptureRuntimeExpectation/CheckRuntime |
| 14 | `utils/src/hccl_verifier/semantics_check/ccu_all_to_all_semantics_checker.cc` | 200 | **AllToAll 语义 checker 实现**：验证 PreSync（WriteVariableWithNotify output+token）、PostSync（NotifyRecord+NotifyWait）、11 个 taskArgs 映射 LoadArg slot、AllToAll 转置语义期望计算、逐字节 memcmp |

#### D. Adapters（2 个）— kernel 捕获桥接

| # | 文件路径 | 行数 | 作用 |
|---|---------|------|------|
| 15 | `utils/src/ccu/ccu_all_to_all_mesh1d_adapter.h` | 48 | **AllToAll adapter 声明**：`ALL_TO_ALL_TASK_ARG_COUNT=11`、`AllToAllKernelConfig`（rankId/rankSize/channels）、`AllToAllLaunchConfig`（inputAddr/outputAddr/token/sliceSize/srcStride/srcOffset/dstOffset/goSize）、`CaptureAllToAllMesh1DKernel()` + `PrepareAllToAllTaskArgs()` |
| 16 | `utils/src/ccu/ccu_all_to_all_mesh1d_adapter.cc` | 92 | **AllToAll adapter 实现**：`ConvertAllToAllArg()` 把产品 `CcuKernelArgAlltoAllMesh1D` 转成 ST `AllToAllKernelConfig`；`CaptureAllToAllMesh1DKernel()` 调真实 `CcuAlltoAllMesh1DKernel()`（CCU API 被 stub 拦截成 Program IR）；`PrepareAllToAllTaskArgs()` 打包 11 个 taskArgs。**需适配**：参数顺序匹配 feat/alltoall-migration 的 kernel 签名 |

#### E. Scenarios/Fixtures（4 个）— 测试编排

| # | 文件路径 | 行数 | 作用 |
|---|---------|------|------|
| 17 | `utils/src/ccu_scenario/ccu_scenario.h` | 61 | **CcuStScenario 结构体**：测试场景参数（rankSize/dataType/opType/sizes 矩阵/expectedAlgName/algConfig 强制算法/负面测试标志 corruptOutput/swapTaskArgs/missingTaskArg/outOfRangeParam） |
| 18 | `utils/src/ccu_scenario/ccu_fixture.h` | 58 | **CcuStFixture GTest 基类**：SetUp/TearDown 重置 RegisterManager；公共工具 CheckProgramStructure（调 CcuProgramChecker）、CheckAbi（调 CcuTaskArgChecker）、ExecuteAndCheckDag（RuntimeWorld.Launch+CcuDagChecker） |
| 19 | `utils/src/ccu_scenario/ccu_all_to_all.h` | 29 | **CcuStAllToAll 声明**：继承 CcuStFixture，提供 `VerifyScenario(CcuStScenario)` |
| 20 | `utils/src/ccu_scenario/ccu_all_to_all.cc` | 270 | **AllToAll VerifyScenario 全链**：5 阶段 — ①per-rank 调真实 SelectAlgAndPrepareEngine+GetCcuOpParamResCtx（CalcRes+channel+HcommCcuKernelRegister→RegisterManager 捕获 IR）；②构造 RankMemory+taskArgs+RankLaunch；③静态校验；④反例分支；⑤RuntimeWorld.Launch+DAG+CheckRuntime。**需适配**：注释掉 kfcServerArgs 验证（line 181-187） |

#### F. Test Cases（5 个）— 测试用例

| # | 文件路径 | 行数 | 作用 |
|---|---------|------|------|
| 21 | `testcase/mc2_ccu/CMakeLists.txt` | ~50 | **selector target 构建配置**：定义 `HCCL_SELECTOR_PRODUCT_SOURCES`（编译真实产品 .cc）、include dirs/compile options/link libraries。**需适配**：去掉 AllToAllV/KFC/Host/Device target 和 `ccu_kernel_all_to_all_v_mesh1d.cc` 引用 |
| 22 | `testcase/mc2_ccu/selector/all_to_all_selector_testcase.cpp` | 68 | **AllToAll 端到端测试**（6 个 TEST_F）：2-rank 均匀传输、4-rank 均匀传输、corruptOutput 反例、swapTaskArgs 反例、missingTaskArg 反例、forcedAlgConfig 强制算法 |
| 23 | `testcase/mc2_ccu/selector/dag_conflict_testcase.cpp` | 215 | **DAG 冲突检测测试**（7 个 TEST）：正例（空日志/相邻区间/有同步）、反例（无同步 WAW/WAR/未匹配同步）— 直接构造 ExecutionLog 测试 CcuDagChecker |
| 24 | `testcase/mc2_ccu/selector/shim/ccu_primitives_dl.hpp` | 27 | **selector shim**：重定向 `AscendC::ccu::CcuBuffer`/`Loop`/`LoopGroup` 到 ST stub（只存 ResourceHandle），避免链接真实 CCU primitives 库 |
| 25 | `testcase/mc2_ccu/selector/stubs.cc` | 218 | **产品函数 stub**：stub `UseCannBridge`（返回 false）、`ExecuteSelector::Run`（字符串映射代替真实 selector）、`HcclCheckLogLevel`/`HcclDfxRegOpInfoByCommId` 等 — 让 selector target 不依赖未链接的产品函数 |

### 15.3 基于 master 的改动文件（21 个）

#### G. CMakeLists 修改（7 个）

| # | 文件路径 | 改动内容 | 为什么需要 |
|---|---------|----------|-----------|
| 1 | `algorithm/CMakeLists.txt` | +4：新增 include dirs（`hccl_depends_stub`/`ccu`） | ST 编译需要找到新的 stub/adapter 头文件 |
| 2 | `algorithm/testcase/CMakeLists.txt` | +2：`add_subdirectory(mc2_ccu)` | 把 mc2_ccu 子目录加入构建 |
| 3 | `algorithm/utils/src/CMakeLists.txt` | +4/-4：加 `ccu_primitives_stub.cc` 到 model-proxy 库；移除 `target_link_libraries` 到 kernel target | model-proxy 包含 primitives stub；移除 kernel target 依赖因为 ST 提供自己的 stub |
| 4 | `algorithm/utils/src/hccl_proxy/CMakeLists.txt` | +1：加 `ccu_kernel_proxy.cc` | RegisterManager 编译进 model-proxy |
| 5 | `algorithm/utils/src/sim_world/CMakeLists.txt` | +1：加 `ccu_sim_world.cc` | RuntimeWorld 编译进 sim_world 库 |
| 6 | `algorithm/utils/src/hccl_verifier/CMakeLists.txt` | +2：加 `ccu_checker.cc`/`ccu_dag_checker.cc` | 通用 checker 编译进 verifier 库 |
| 7 | `algorithm/utils/src/hccl_verifier/semantics_check/CMakeLists.txt` | +1：加 `ccu_all_to_all_semantics_checker.cc` | AllToAll 语义 checker 编译进 verifier |

#### H. hccl_proxy 修改（5 个）

| # | 文件路径 | 改动内容 | 为什么需要 |
|---|---------|----------|-----------|
| 8 | `utils/src/hccl_proxy/hcomm_ccu_stub.cc` | **+85/-15**：加 capture registry + `RegisterCaptureFunction` + `HcommCcuKernelRegister` 桥接到 RegisterManager + `HcclCommQueryCcuIns` 返回真实 handle + `GetTokenInfo` 返回非零 token | **核心桥接**：产品代码调 HcommCcuKernelRegister 时 ST 拦截并路由到 RegisterManager 捕获 Program IR |
| 9 | `utils/src/hccl_proxy/hccl_stub.cc` | +4：加 CCU engine 分支到 `HcclChannelAcquire` → `ChannelCommCreateForCcu` | CCU channel 创建走不同路径 |
| 10 | `utils/src/hccl_proxy/hccl_profiling_dfx_stub.cc` | +6：加 `HcclDfxRegOpInfoByCommId` stub | 产品代码在 CalcRes 时调用此函数 |
| 11 | `utils/src/hccl_proxy/aclrt_stub.cc` | +2/-2：`BufferType` → `HcclSim::BufferType` | sim_common.h 把 BufferType 包入 HcclSim namespace |
| 12 | `utils/src/hccl_proxy/hccl_algorithm_stub.cc` | +3/-1：格式化 | 无功能影响 |

#### I. communicator 修改（6 个）

| # | 文件路径 | 改动内容 | 为什么需要 |
|---|---------|----------|-----------|
| 13 | `utils/src/hccl_proxy/communicator/sim_channel_manager.cc` | +29：加 `ChannelCommCreateForCcu` 方法 | CCU channel 创建（forward+reverse SimChannel 对） |
| 14 | `utils/src/hccl_proxy/communicator/sim_channel_manager.h` | +4：加声明 | 同上 |
| 15 | `utils/src/hccl_proxy/communicator/sim_communicator.cc` | +7：加委托 | 同上 |
| 16 | `utils/src/hccl_proxy/communicator/sim_communicator.h` | +3：加声明 | 同上 |
| 17 | `utils/src/hccl_proxy/communicator/sim_context_manager.cc` | +2/-3：加 `COMM_ENGINE_CCU` 到 host-memory 分配 | CCU engine context 需要 host 内存 |
| 18 | `utils/src/hccl_proxy/communicator/sim_thread_manager.cc` | +3：加 `COMM_ENGINE_CCU → HOST_NOTIFY` 映射 | CCU 使用 host-based notify |

#### J. sim_world 修改（3 个）

| # | 文件路径 | 改动内容 | 为什么需要 |
|---|---------|----------|-----------|
| 19 | `utils/src/sim_world/sim_common.h` | +8：`BufferType`/`NpuPos`/`MemBlock` 包入 `namespace HcclSim` + using 别名 | 防止 CCU ST 类型与旧 ST 类型名冲突 |
| 20 | `utils/src/sim_world/data_slice.h` | +1/-1：`StringFormat` → `::StringFormat` | namespace 消歧 |
| 21 | `utils/src/sim_world/sim_task.h` | +1/-1：`StringFormat` → `::StringFormat` | namespace 消歧 |

### 15.4 ST 文件适配（2 处）

从 ccu-st 分支复制后需要做的适配修改：

| # | 文件 | 适配内容 | 原因 |
|---|------|----------|------|
| 1 | `ccu_all_to_all_mesh1d_adapter.cc` | 调整 `CaptureAllToAllMesh1DKernel` 调用参数顺序 | feat/alltoall-migration 的 kernel 签名是 `(inputAddr, outputAddr, tokenInfo, sliceSize, srcStride, srcOffset, dstOffset, goSize1-4, arg)`，adapter 原来的调用顺序不同，需匹配 |
| 2 | `ccu_all_to_all.cc` | 注释掉 `kfcServerArgs` 验证（line 181-187） | 不迁移 KFC server，不验证 kfcServerArgs |

### 15.5 产品代码适配

**零修改**。所有适配都在 ST 侧完成。

验证依据：
- CCU primitives 层（17 个文件）两个分支仅格式差异，API 完全兼容
- selector target 不定义 `AICPU_COMPILE`，executor 的 `#ifndef AICPU_COMPILE` 守卫不会跳过 include
- ST 的 `stubs.cc` 用字符串映射 stub selector 行为，不引用 `AlgorithmType` 枚举
- ST 的 adapter 可调整参数顺序匹配我们的 kernel 签名，不需要改产品 kernel

### 15.6 汇总

| 类型 | 文件数 | 说明 |
|------|--------|------|
| **新增** | 22 | 全新文件，从 ccu-st 分支复制（2 个需适配） |
| **改动** | 21 | master 已有文件，应用 ccu-st 的 diff |
| **产品代码** | 0 | 零修改 |
| **总计** | 43 | |

### 15.7 ST 构建结构

ST 产出物为**可执行文件**（gtest 二进制），不是 .so。

#### 产出物

```
ascendc_ut_adv_api_hccl_ccu_selector_st  ← 可执行文件
```

#### 链接的库

| 库名 | 类型 | 包含什么 |
|------|------|----------|
| `hccl-model-proxy` | STATIC | ST 框架核心：ccu_primitives_stub + ccu_kernel_proxy + hcomm_ccu_stub + sim_world + communicator + topo_model + 旧 ST 的 hccl_stub/sim_world 等 |
| `hccl-verifier` | STATIC | 校验器：ccu_checker + ccu_dag_checker + 旧 verifier + 所有 semantics_checker（含 ccu_all_to_all_semantics_checker） |
| `intf_llt_pub_basic` | 外部 | gtest 框架 |
| `c_sec` | 外部 | 安全库 |
| `pthread` | 系统 | 线程 |
| `dl` | 系统 | 动态链接 |

#### 直接编译的源文件

selector target 可执行文件直接编译两部分源码：

**第 1 部分：测试代码 + stub**

| 文件 | 说明 |
|------|------|
| `tests/main.cpp` | gtest main |
| `selector/all_to_all_selector_testcase.cpp` | AllToAll 测试用例（6 个 TEST_F） |
| `selector/dag_conflict_testcase.cpp` | DAG 测试用例（7 个 TEST） |
| `selector/stubs.cc` | 产品函数 stub |

**第 2 部分：真实产品代码**（`HCCL_SELECTOR_PRODUCT_SOURCES`）

| 文件 | 说明 |
|------|------|
| `ops/all_to_all_v/selector/alltoall_auto_selector.cc` | 真实 selector |
| `ops/all_to_all_v/executor/ins_v2_all_to_all_v_sole_executor.cc` | 真实 executor |
| `ops/all_to_all_v/template/ccu/ccu_temp_all_to_all_mesh_1D.cc` | 真实 template |
| `ops/all_to_all_v/template/ccu/kernel/ccu_kernel_all_to_all_mesh1d.cc` | 真实 kernel |
| `ops/op_common/op_common.cc` | 真实 op_common |
| `ops/op_common/selector/*.cc`（3 个） | selector 基础设施 |
| `ops/op_common/executor/*.cc`（5 个） | executor 基础设施 |
| `ops/op_common/template/*.cc`（5 个） | template 基础设施 |
| `ops/op_common/topo/*.cc`（8 个） | 拓扑匹配 |
| `common/hccl_alloc_ctx_res.cc` | 资源分配 |
| `common/hccl_mc2.cc` | MC2 主流程 |

#### 编译选项

```
-std=c++17
-DMC2_CLIENT_ENABLE_CCU=1    ← 启用 CCU 代码路径
-DUT_TEST                     ← 测试模式
-D_GLIBCXX_USE_CXX11_ABI=0   ← ABI 兼容
-O0 -g                        ← 不优化带调试
-Wno-unused-variable/parameter/attributes/unknown-pragmas  ← 放宽警告
```

#### 整体结构图

```
ascendc_ut_adv_api_hccl_ccu_selector_st (可执行文件)
├── tests/main.cpp (gtest main)
├── selector/all_to_all_selector_testcase.cpp (6 个 TEST_F)
├── selector/dag_conflict_testcase.cpp (7 个 TEST)
├── selector/stubs.cc (产品函数 stub)
├── 真实产品 .cc (selector/executor/template/kernel/op_common/...)
├── ← 链接 → hccl-model-proxy (static lib)
│   ├── ccu_primitives_stub.cc (CCU API → Program IR)
│   ├── ccu_kernel_proxy.cc (RegisterManager)
│   ├── hcomm_ccu_stub.cc (HcommCcuKernelRegister 桥接)
│   ├── ccu_sim_world.cc (RuntimeWorld 执行引擎)
│   ├── sim_communicator.cc (SimChannel/SimThread)
│   └── 旧 ST 的 hccl_stub.cc/sim_world.cc 等
├── ← 链接 → hccl-verifier (static lib)
│   ├── ccu_checker.cc (Program 结构校验)
│   ├── ccu_dag_checker.cc (DAG 冲突检测)
│   ├── ccu_all_to_all_semantics_checker.cc (AllToAll 语义)
│   └── 旧 verifier/checker 等
├── ← 链接 → intf_llt_pub_basic (gtest)
├── ← 链接 → c_sec
└── ← 链接 → pthread, dl
```

**关键点**：不链接 `libmc2_client.so`（产品库），而是直接编译产品 .cc 源文件 + ST 的 stub/proxy/checker 静态库。这样 ST 可以用 `ccu_primitives_stub` 拦截 CCU API 调用，捕获成 Program IR。

### 15.8 shim 和 stubs.cc 的作用

#### shim/ccu_primitives_dl.hpp

selector target 编译真实 kernel 代码时，kernel 内部引用 `ccu::CcuBuffer`/`ccu::Loop`/`ccu::LoopGroup` 类型。这些类型的真实定义在 `ccu_buffer_dl.hpp`/`ccu_loop_dl.hpp` 中（通过 dlsym 动态加载），selector target 不链接这些库。

shim 文件在 include path 中优先被找到，提供空壳定义（只存 `ResourceHandle handle`），让编译通过。

**迁移优化**：可将 3 个空壳类直接合并到 `ccu_primitives_stub.h` 末尾，删掉 shim 文件和对应 include path。

#### stubs.cc

selector target 编译真实产品代码（`op_common.cc`/`selector_registry.cc` 等）时，这些代码调用了不在 selector target 中编译的函数。`stubs.cc` 提供这些函数的假实现：

| 类别 | 函数 | stub 行为 |
|------|------|----------|
| 日志 | `HcclCheckLogLevel`/`IsErrorToWarn` | 返回 false（不打印/不升级） |
| AICPU | `HcclLaunchAicpuKernel`/`ConvertCcuToHccl` | no-op / 返回 SUCCESS |
| profiling | `HcclDfxRegOpInfoByCommId` | no-op |
| 参数校验 | `HcomCheckDataType`/`HcomCheckReductionOp`/`HcomCheckUserRank`/`CheckAsymmetricTopoSupport` | 返回 SUCCESS（不校验） |
| 环境变量 | `GetExternalInputHcclCcuMSMode`/`GetExternalInputHcclCcuSchedMode` 等 | **有真实实现**（读 `HCCL_OP_EXPANSION_MODE` 环境变量），ST 通过设环境变量控制 selector 走 CCU_SCHED 路径 |
| CANN bridge | `UseCannBridge`/`SelectViaCann`/`GetAlgExecViaCann` | 返回 false / nullptr（不走 CANN bridge） |
| CCU 工具 | `CalGoSize`/`GroupCopy`/`LocalCopy`/`DataTypeSizeGet` | 简化或 no-op（`DataTypeSizeGet` 有真实实现） |
| DlHcomm | `DlHcommFunction::GetInstance`/`Init` | 空单例 + no-op |

**关键设计**：环境变量函数有真实实现（读 `HCCL_OP_EXPANSION_MODE`），ST 通过设环境变量控制 selector 走 CCU_SCHED 路径。其他函数要么 no-op 要么返回固定值。

### 15.9 迁移完成记录

| 日期 | Task | Commit | 状态 |
|------|------|--------|------|
| 2026-08-10 | Task 1+2: 复制 22 个 NEW 文件（含合并 shim） | `e8f849727` | ✅ |
| 2026-08-10 | Task 3: 适配 adapter 参数顺序 + 去 kfcServerArgs | `4481207b5` | ✅ |
| 2026-08-10 | Task 4+5: 创建 CMakeLists + 应用 21 个 MODIFIED 文件 | subagent commit | ✅ |
| 2026-08-10 | Task 6: 编译验证（shim 重定向 + adapter/scenario 加入源文件） | `3d4e3ccd7` | ✅ |
| 2026-08-10 | Task 7: 运行测试（设 ASCEND_ENABLE_CCU_KFC_BRANCH + opExecuteConfig） | `cd65c2734` | ✅ |
| 2026-08-10 | Task 8: 推送 + 更新 spec | — | ✅ |

**测试结果**：13/13 通过（6 AllToAll 端到端 + 7 DAG 冲突检测）

### 15.10 迁移中的编译/运行修复

| # | 问题 | 修复 |
|---|------|------|
| 1 | 产品代码 `ccu_kernel_alg_base.h` include `ccu_primitives_dl.hpp` 找不到 | 创建 `selector/shim/ccu_primitives_dl.hpp` 重定向到 `ccu_primitives_stub.h` |
| 2 | `ccu_all_to_all.cc` 和 `ccu_all_to_all_mesh1d_adapter.cc` 没编译进 selector target | 加到 `add_executable` 源文件列表 |
| 3 | selector 选了 AICPU 算法而不是 CCU 算法 | 设 `ASCEND_ENABLE_CCU_KFC_BRANCH=1` 环境变量 + `opParam.opExecuteConfig = CCU_SCHED` |
| 4 | adapter 调用 kernel 参数顺序不匹配 | 改 adapter 调用顺序匹配我们的 kernel 签名 |

### 15.11 框架完善记录

| 日期 | 改动 | Commit | 状态 |
|------|------|--------|------|
| 2026-08-10 | 重构 CcuStFixture 基类（提取通用 VerifyScenario 5 阶段 + 5 虚函数接口） | `024ff3773` | ✅ |
| 2026-08-10 | 3 个框架缺口修复（dieId/多拓扑/强制算法） | `da2016fa4` | ✅ |

### 15.12 IR 捕获完整流程

以 AllToAll Mesh1D 为例，从产品代码到 IR 捕获的完整链路：

#### 第 1 步：静态注册（程序启动时）

adapter 通过静态变量初始化，把 capture/convert/deleter 注册到全局 registry：

```cpp
// ccu_all_to_all_mesh1d_adapter.cc
bool g_registered = []() {
    RegisterCaptureFunction(
        "CcuKernelAlltoAllMesh1D",       // key：产品代码的 kernelFuncName
        CaptureAllToAllMesh1DKernel,      // capture 函数
        ConvertAllToAllArg,               // convert 函数：产品 arg → ST config
        [](void *p) { delete static_cast<AllToAllKernelConfig *>(p); });  // deleter
    return true;
}();
```

registry 内容：
```
registry = {
    "CcuKernelAlltoAllMesh1D" → {
        capture: CaptureAllToAllMesh1DKernel,
        convert: ConvertAllToAllArg,
        deleter: delete AllToAllKernelConfig*
    }
}
```

#### 第 2 步：产品代码调 HcommCcuKernelRegister

```cpp
// 产品代码 op_common.cc：
HcommCcuKernelRegisterStart(insHandle);
HcommCcuKernelRegister(insHandle, dieId, "CcuKernelAlltoAllMesh1D",
    kernelFunc, kernelArgs, argNum, &kernelHandle);
HcommCcuKernelRegisterEnd(insHandle);
```

#### 第 3 步：ST 桥接拦截（hcomm_ccu_stub.cc）

```cpp
HcommCcuKernelRegister(insHandle, dieId, "CcuKernelAlltoAllMesh1D", ...)
{
    // 3a. 查 registry
    auto it = registry.find("CcuKernelAlltoAllMesh1D");  // 找到

    // 3b. 转换产品 arg → ST config
    void *stArg = it->second.convert(kernelArgs[0], argNum);
    // ConvertAllToAllArg 把 CcuKernelArgAlltoAllMesh1D* → AllToAllKernelConfig*
    // 提取 rankId/rankSize/loadFromMem + channels 转 MakeChannel 编码

    // 3c. 调 RegisterManager::Register
    RegisterManager::Global().Register(
        insHandle, "CcuKernelAlltoAllMesh1D",
        CaptureAllToAllMesh1DKernel,   // capture 函数
        stArg,                          // AllToAllKernelConfig*
        handle);                        // 输出 KernelHandle

    // 3d. 析构 ST config
    it->second.deleter(stArg);

    // 3e. 返回产品 handle
    *kernelHandle = handle;
}
```

#### 第 4 步：RegisterManager::Register 执行

```cpp
Register(instance, name, function, kernelArg, handle)
{
    // 4a. 创建 CompilerContext + Program
    KernelRecord record;
    CompilerContext compiler(record.program);
    CompilerContext::SetCurrent(&compiler);  // 设为线程局部活跃上下文

    // 4b. 调 capture 函数（见第 5 步）
    result = function(kernelArg);

    // 4c. 清除上下文
    CompilerContext::SetCurrent(nullptr);

    // 4d. 保存 record
    handle = nextKernelHandle_++;
    kernels_.emplace(handle, std::move(record));
    currentGroup.push_back(handle);
}
```

#### 第 5 步：CaptureAllToAllMesh1DKernel 执行

```cpp
CaptureAllToAllMesh1DKernel(void *kernelArg)
{
    // 5a. 取出 ST config
    auto &config = *static_cast<AllToAllKernelConfig *>(kernelArg);

    // 5b. LoadArg 11 次 — 声明 kernel 需要的参数绑定
    std::array<Variable, 11> arguments;
    for (index = 0..10) {
        LoadArg(arguments[index], index);
        // → Append(LOAD_ARG, dst=arguments[index].handle, index=index)
        // → RegisterArgument(arguments[index].handle, index)
        // 作用：声明 argId 0-10 分别绑定到 11 个 Variable
        // 这些 Variable 后面作为函数参数传给真实 kernel
    }

    // 5c. 配置 Program 的 rank 信息
    CompilerContext::Current().ConfigureProgram(config.rankId, config.rankSize, config.channels);

    // 5d. 用 config 重建产品 kernelArg
    CcuKernelArgAlltoAllMesh1D kernelArgData;
    kernelArgData.rankSize = config.rankSize;
    kernelArgData.rankId = config.rankId;
    kernelArgData.loadFromMem = config.loadFromMem;
    kernelArgData.channelCount = config.channels.size();
    for (i = 0..channels.size()-1) {
        kernelArgData.channels[i] = config.channels[i];  // ST 编码的 channel
    }

    // 5e. 调真实 kernel
    CcuKernelArg argPtr = &kernelArgData;
    CcuAlltoAllMesh1DKernel(
        arguments[0], ..., arguments[10], argPtr);
}
```

**为什么需要 ConvertAllToAllArg 转换再转回？**

产品代码传入的 `CcuKernelArgAlltoAllMesh1D` 的 channels 是产品分配的 handle（`i+1` 编号），RuntimeWorld 无法从中解码出 `(localRank, remoteRank)`。`ConvertAllToAllArg` 把 channels 转成 `MakeChannel(localRank, remoteRank)` 编码（`localRank<<32 | remoteRank`），让 RuntimeWorld 能从 channel handle 解码出 rank 对，路由跨 rank 数据。

**LoadArg 的两个阶段**：

| 阶段 | LoadArg 做什么 | 值从哪来 |
|------|---------------|---------|
| 注册阶段（adapter） | `Append(LOAD_ARG)` + `RegisterArgument` → 记录 argId→Variable 绑定到 `Program.arguments` | 没有实际值，只是声明绑定关系 |
| 执行阶段（RuntimeWorld） | 从 `TaskParam.args[argId]` 取实际值赋给 Variable | 测试用例构造的 `RankMemory` 地址 |

#### 第 6 步：真实 kernel 执行（CCU API 被 stub 拦截）

```cpp
CcuAlltoAllMesh1DKernel(inputAddr, outputAddr, ..., argPtr)
{
    // 6a. InitResource — 从 channel 取资源
    for (peerId != rankId) {
        ctx.input[peerId]  = GetResByChannel(channels[i], INPUT_XN_ID);
        // → GetChannelVariable(channel, 0) → AllocateVariable + channelVariables[(peer,0)]=handle

        ctx.output[peerId] = GetResByChannel(channels[i], OUTPUT_XN_ID);
        // → channelVariables[(peer,1)]=handle

        ctx.token[peerId]  = GetResByChannel(channels[i], TOKEN_XN_ID);
        // → channelVariables[(peer,2)]=handle
    }

    // 6b. LoadArgs — 直接赋值（展开参数版，不再调 ccu::LoadArg）
    ctx.input[rankId]  = inputAddr;   // 函数参数直接赋值
    ctx.output[rankId] = outputAddr;
    ctx.token[rankId]  = tokenInfo;
    ctx.sliceSize      = sliceSize;
    // ...

    // 6c. PreSync — 地址交换
    for (channel : channels) {
        WriteVariableWithNotify(channel, ctx.output[rankId], OUTPUT_XN_ID, CKE_IDX_0, 1<<OUTPUT_XN_ID);
        → Append(WRITE_VARIABLE_NOTIFY, channel, src0=output_handle, index=1, immediate=0, mask=2)

        WriteVariableWithNotify(channel, ctx.token[rankId], TOKEN_XN_ID, CKE_IDX_0, 1<<TOKEN_XN_ID);
        → Append(WRITE_VARIABLE_NOTIFY, channel, src0=token_handle, index=2, immediate=0, mask=4)
    }
    for (channel : channels) {
        NotifyWait(channel, CKE_IDX_0, 1<<OUTPUT_XN_ID | 1<<TOKEN_XN_ID);
        → Append(NOTIFY_WAIT, channel, index=0, mask=6)
    }

    // 6d. DoAlltoAll — 数据传输
    for (rankIdx != rankId) {
        Write(channel, dst[rankIdx], src[rankIdx], sliceSize, event, 1<<rankIdx);
        → Append(WRITE, channel, dst=remote_handle, src0=local_handle, src1=length_handle, ...)
    }
    GroupCopy(ctx, localDst, src[rankId], goSize);
    → AppendGroupCopy(dst, src, length, groupSize)
    → Append(GROUP_COPY, dst, src0, src1, index=descriptor_index)

    EventWait(event, allBit);
    → Append(EVENT_WAIT, src0=event_handle, mask=allBit)

    // 6e. PostSync — 后同步
    NotifyRecord(channel, CKE_IDX_1, 1<<5);
    → Append(NOTIFY_RECORD, channel, index=1, mask=32)
    NotifyWait(channel, CKE_IDX_1, 1<<5);
    → Append(NOTIFY_WAIT, channel, index=1, mask=32)
}
```

#### 第 7 步：保存 IR + RegisterEnd 校验

```cpp
// Register 中：
kernels_[handle=1] = {
    name = "CcuKernelAlltoAllMesh1D",
    program = {
        rankId = 0, rankSize = 2, channels = [MakeChannel(0,1)],
        operations = [
            LOAD_ARG(handle_A, 0),     // argId 0 → inputAddr
            LOAD_ARG(handle_B, 1),     // argId 1 → outputAddr
            ... 11 个 LOAD_ARG ...
            WRITE_VARIABLE_NOTIFY(ch, output_handle, 1, 0, 2),
            WRITE_VARIABLE_NOTIFY(ch, token_handle, 2, 0, 4),
            NOTIFY_WAIT(ch, 0, 6),
            WRITE(ch, remote_handle, local_handle, length_handle, ...),
            GROUP_COPY(dst, src, length, index=0),
            EVENT_WAIT(event_handle, allBit),
            NOTIFY_RECORD(ch, 1, 32),
            NOTIFY_WAIT(ch, 1, 32),
        ],
        arguments = [(0,handle_A), (1,handle_B), ...],
        variables = {handle_A, handle_B, ...},
        channelVariables = {(1,0):handle, (1,1):handle, (1,2):handle},
        groupCopies = [GroupCopyDescriptor{...}],
    },
    compiled = false
}

// RegisterEnd：
Finalize(program)
  → 校验 IF/WHILE 嵌套闭合（stack 为空 → ✅）
  → 设置 peerIndex（IF_BEGIN↔IF_END 配对）
  → 校验 argId 连续（0,1,2,...,10 → ✅）
  → compiled = true
  → lastCompiledGroup_ = [1]
```

#### 完整流程图

```
产品代码
  HcommCcuKernelRegister("CcuKernelAlltoAllMesh1D", kernelArgs)
    │
    ▼
ST 桥接 (hcomm_ccu_stub.cc)
  查 registry["CcuKernelAlltoAllMesh1D"]
    │
    ▼
  ConvertAllToAllArg(产品 CcuKernelArgAlltoAllMesh1D*)
    │ 提取 rankId/rankSize/channels
    │ channels 转 MakeChannel 编码
    ▼
  AllToAllKernelConfig* (ST config)
    │
    ▼
  RegisterManager::Register(capture, stArg)
    │
    ├── 创建 CompilerContext + Program
    ├── SetCurrent(&compiler)
    │
    ▼
  CaptureAllToAllMesh1DKernel(stArg)
    │
    ├── LoadArg × 11          → Program 记录 argId 绑定
    ├── ConfigureProgram      → Program 设置 rankId/rankSize/channels
    ├── 重建 CcuKernelArgAlltoAllMesh1D
    │
    ▼
  CcuAlltoAllMesh1DKernel(...)  ← 真实 kernel 执行
    │
    ├── GetResByChannel × 3   → AllocateVariable + channelVariables
    ├── WriteVariableWithNotify × 2  → Append(WRITE_VARIABLE_NOTIFY) × 2
    ├── NotifyWait × 1        → Append(NOTIFY_WAIT)
    ├── Write × (N-1)         → Append(WRITE) × (N-1)
    ├── GroupCopy × 1         → AppendGroupCopy + Append(GROUP_COPY)
    ├── EventWait × 1         → Append(EVENT_WAIT)
    ├── NotifyRecord × 1      → Append(NOTIFY_RECORD)
    └── NotifyWait × 1        → Append(NOTIFY_WAIT)
    │
    ▼
  SetCurrent(nullptr)
  保存 Program 到 kernels_[handle=1]
    │
    ▼
  RegisterEnd → Finalize（校验嵌套 + argId）
  → compiled = true
  → lastCompiledGroup_ = [1]
    │
    ▼
  返回 kernelHandle = 1
```

### 15.13 BufferType/StringFormat 命名冲突分析

#### 冲突现象

CCU ST 编译 `ccu_fixture.cc` 时报 `BufferType` is ambiguous 和 `StringFormat` is ambiguous。

#### 根本原因

产品代码 `hccl_alloc_ctx_res.h:30` 在**头文件**中写了 `using namespace mc2_ops_hccl;`，这把 `mc2_ops_hccl` 命名空间中的所有符号引入全局命名空间，污染了所有 include 该头文件的编译单元。

#### 冲突的两个 BufferType

| # | 定义位置 | 定义方式 | namespace | 枚举值 |
|---|---------|---------|-----------|--------|
| 1 | ST `sim_common.h:18` | `MAKE_ENUM(BufferType, ...)` | 全局（master）/ `HcclSim`（我们修复后） | INPUT/OUTPUT/CCL/RESERVED |
| 2 | 产品 `template_utils.h:36` | `enum class BufferType` | `mc2_ops_hccl` | INPUT/OUTPUT/HCCL_BUFFER/DEFAULT |

#### 冲突的两个 StringFormat

| # | 定义位置 | namespace |
|---|---------|-----------|
| 1 | ST `string_util.h:20` | 全局 |
| 2 | 产品 `utils.h:34` | `mc2_ops_hccl` |

#### 冲突触发路径

```
ccu_fixture.cc（在 namespace HcclSim 中）
  → ccu_fixture.h
    → hccl_alloc_ctx_res.h
      → using namespace mc2_ops_hccl;     ← 污染全局命名空间
      → template_utils.h                   ← 定义 mc2_ops_hccl::BufferType
  → sim_communicator.h
    → sim_channel.h
      → sim_task.h
        → data_slice.h                     ← 在 namespace HcclSim 中引用 BufferType
          → 编译器查找 BufferType：
            1. ::BufferType（ST 的 sim_common.h）          ← candidate 1
            2. mc2_ops_hccl::BufferType（通过 using 引入全局） ← candidate 2
            → ambiguous！
```

#### 为什么 master 上不冲突

master 上旧 AICPU ST 不编译产品代码（`op_common.cc`/`template_utils.h` 等），只有 `libHcomm.so`（已编译）。CCU ST 直接编译产品源码 → 引入 `template_utils.h` → 冲突。

#### 修复方式

把 ST 的 `BufferType`/`NpuPos`/`MemBlock` 包入 `namespace HcclSim`，和产品的 `mc2_ops_hccl::BufferType` 隔离：

| 文件 | 改动 | 作用 |
|------|------|------|
| `sim_common.h` | `BufferType`/`NpuPos`/`MemBlock` 包入 `namespace HcclSim` + `using` 别名 | ST 类型从全局移到 `HcclSim`，不影响旧代码（using 别名保持全局可用） |
| `aclrt_stub.cc` | `BufferType` → `HcclSim::BufferType` | `aclrt_stub.cc` 在全局命名空间，包入后需要加前缀 |
| `data_slice.h` | `StringFormat` → `::StringFormat` | `HcclSim` namespace 内 `StringFormat` 也受 `using namespace mc2_ops_hccl` 污染，加 `::` 强制用全局版本 |
| `sim_task.h` | `StringFormat` → `::StringFormat` | 同上 |

这 4 个是配套改动，缺一不可。

#### 根本问题

`hccl_alloc_ctx_res.h:30` 的 `using namespace mc2_ops_hccl;` 写在头文件中是不好的实践 — 头文件中不应该用 `using namespace`，它污染了所有 include 该头文件的编译单元。这是产品代码的问题，ST 只能通过 namespace 隔离来规避。

### 15.14 COMM_ENGINE_AIV context 存储细节

#### 现象

`sim_context_manager.cc` 的 `CreateCommEngineCtx` 中加了 `COMM_ENGINE_AIV` 和 `COMM_ENGINE_CCU` 到 host 内存分配分支。

#### 调用链

```
RegisterKernelForRank（ccu_fixture.cc）
  → GetCcuOpParamResCtx（产品代码 hccl_alloc_ctx_res.cc）
    → GetAlgResCcu（op_common.cc:1255）
      → 序列化资源 ctx
      → HcclEngineCtxGet(comm, algTag, COMM_ENGINE_AIV, ...)   ← 尝试复用
      → HcclEngineCtxCreate(comm, algTag, COMM_ENGINE_AIV, ...) ← 没有则创建
        → ST stub（hccl_stub.cc）→ SimContextMgr::CreateCommEngineCtx(tag, AIV, ...)
          → 检查 engine 是否在 host 内存分配分支中
```

#### 为什么用 COMM_ENGINE_AIV 而不是 COMM_ENGINE_CCU

产品代码 `op_common.cc:1289/1294` 中资源 context 的存储 key 用的是 `COMM_ENGINE_AIV`，不是 `COMM_ENGINE_CCU`：

```cpp
// op_common.cc:1289-1294：
if (HcclEngineCtxGet(comm, param.algTag, COMM_ENGINE_AIV, &ctx, &actualSize) == HCCL_SUCCESS) {
    // 复用已有 context
} else {
    CHK_RET(HcclEngineCtxCreate(comm, param.algTag, COMM_ENGINE_AIV, size, &ctx));
}
```

这是产品代码的历史设计 — 资源 context 存储原本为 AIV 设计，CCU 后来复用了这个存储路径但没改 key 名。

产品代码中 context 存储用了两个不同的 key：

| 用途 | key | 位置 |
|------|-----|------|
| FastLaunch context | `COMM_ENGINE_CCU` | `op_common.cc:293` |
| 资源序列化 context（GetAlgResCcu） | `COMM_ENGINE_AIV` | `op_common.cc:1289/1294` |

#### 必须 vs 可选

| engine | 必须？ | 原因 |
|--------|--------|------|
| `COMM_ENGINE_AIV` | ✅ 必须 | 产品代码 `GetAlgResCcu` 传 `AIV`，每次注册 kernel 都会走到 |
| `COMM_ENGINE_CCU` | ⚠️ 可选 | 当前产品代码不传 `CCU` 给 `HcclEngineCtxCreate`，但保留无害（未来可能改用 `CCU`） |

#### 不加会怎样

`CreateCommEngineCtx` 不处理 `AIV` → context 分配走 else 分支（报错或默认处理）→ `GetCcuOpParamResCtx` 失败 → `RegisterKernelForRank` 返回 0 → 测试失败。

### 15.15 新增算法的修改点（基于最新架构）

#### 扩展新算法只需 6 个文件（3 个专属 + 3 个 CMakeLists 修改）

当前架构（adapter 合并到 `ccu_scenario/`、fixture 基类有 5 个虚函数接口）下，新增一个算法（如 AllGather 或 AllToAll 2Die）的完整步骤：

#### 第 1 步：新增 3 个专属文件（~350 行）

| 文件 | 目录 | 作用 | 行数 |
|------|------|------|------|
| `ccu_<alg>_adapter.{h,cc}` | `utils/src/ccu_scenario/` | ①`ConvertArg`：产品 `CcuKernelArg<Alg>` → ST `<Alg>KernelConfig`（提取 rankId/rankSize/channels + 专属字段）；②`Capture<Alg>Kernel`：LoadArg 声明 + ConfigureProgram + 重建产品 arg + 调真实 kernel；③`Prepare<Alg>TaskArgs`：打包 N 个 taskArgs；④静态注册 `RegisterCaptureFunction("CcuKernel<Alg>", ...)` | ~100 |
| `ccu_<alg>_semantics_checker.{h,cc}` | `utils/src/hccl_verifier/semantics_check/` | ①`CheckRegistration`：channel 数量/分组 + PreSync/PostSync 契约 + 操作计数；②`CheckTaskArgs`：N 个参数映射 LoadArg slot；③`CaptureRuntimeExpectation`：**核心差异** — 按算法语义计算期望 output（转置/拼接/按 die 分片）；④`CheckRuntime`：逐字节 memcmp | ~150 |
| `ccu_<alg>.{h,cc}` | `utils/src/ccu_scenario/` | 继承 `CcuStFixture`，实现 5 个虚函数：`BuildScenarioData`（构造 memories/configs/launches）+ `DoCheckRegistration` + `DoCheckTaskArgs` + `DoCaptureRuntimeExpectation` + `DoCheckRuntime` | ~100 |

#### 第 2 步：修改 3 个 CMakeLists（~6 行）

| 文件 | 修改 |
|------|------|
| `testcase/mc2_ccu/CMakeLists.txt` | `add_executable` 源文件加 `<alg>_testcase.cpp` + `ccu_<alg>.cc` + `ccu_<alg>_adapter.cc`；`HCCL_SELECTOR_PRODUCT_SOURCES` 加产品 .cc |
| `utils/src/hccl_verifier/semantics_check/CMakeLists.txt` | 加 `ccu_<alg>_semantics_checker.cc` |
| （如需新增产品代码 include path）`testcase/mc2_ccu/CMakeLists.txt` | 加 `target_include_directories` |

#### 第 3 步：新增测试用例（~70 行）

| 文件 | 作用 |
|------|------|
| `testcase/mc2_ccu/selector/<alg>_testcase.cpp` | `TEST_F(CcuSt<Alg>, ...)` — 正例（2-rank/4-rank）+ 反例（corruptOutput/swapTaskArgs/missingTaskArg） |

#### 不需要改的（通用框架 22 个文件不动）

| 通用框架 | 为什么不动 |
|---------|-----------|
| `ccu_st_types.h`（Program IR） | 19 种 OpCode 已覆盖全部 CCU 原语 |
| `ccu_primitives_stub.{h,cc}`（CCU API stub） | 全部 CCU API 已 stub |
| `ccu_kernel_proxy.{h,cc}`（RegisterManager） | 按 kernelFuncName 路由，新算法注册 adapter 即可 |
| `ccu_sim_world.{h,cc}`（RuntimeWorld） | 轮询执行引擎，不关心算法种类 |
| `ccu_checker.{h,cc}`（通用 checker） | Program 结构校验通用 |
| `ccu_dag_checker.{h,cc}`（DAG 冲突检测） | happens-before + 内存冲突通用 |
| `ccu_dag_types.h` | 类型定义不变 |
| `ccu_fixture.{h,cc}`（基类） | 5 阶段通用流程 + 5 个虚函数接口 |
| `ccu_scenario.h`（场景参数） | 通用字段（rankSize/dataType/opType/topoType/反例控制） |
| `hcomm_ccu_stub.cc`（桥接） | 按 kernelFuncName 自动路由 |
| `stubs.cc`（产品函数 stub） | 通用 |
| `shim/ccu_primitives_dl.hpp` | 通用 |

#### 5 个虚函数和基类 VerifyScenario 的关系

```
VerifyScenario（基类，通用 5 阶段）
│
├── 阶段 1：RegisterKernelForRank × N        ← 通用，不需要虚函数
│
├── 阶段 2：BuildScenarioData(scenario, handles)  ← 虚函数 ①
│   └── 构造 memories + configs_ + launches（专属数据布局/config/taskArgs）
│
├── 阶段 3：DoCheckRegistration × N          ← 虚函数 ②
│   └── 构造期望 kernelConfig + 调专属 checker.CheckRegistration
│
├── 阶段 3：DoCheckTaskArgs × N              ← 虚函数 ③
│   └── 调专属 checker.CheckTaskArgs
│
├── 阶段 4：DoCaptureRuntimeExpectation      ← 虚函数 ④
│   └── 调专属 checker.CaptureRuntimeExpectation → 存到 expectation_
│
├── 阶段 5：world.Launch                     ← 通用
├── 阶段 5：CcuDagChecker.Check              ← 通用
├── 阶段 5：DoCheckRuntime                   ← 虚函数 ⑤
│   └── 调专属 checker.CheckRuntime（用 expectation_ 对比）
│
└── 反例：corruptOutput → 篡改 output → DoCheckRuntime 期望失败
```

#### 各算法的专属差异

以 AllToAll 系列为例：

| 算法 | adapter 差异 | semantics_checker 差异 | scenario 差异 |
|------|-------------|----------------------|--------------|
| Mesh1D（已有） | 11 taskArgs，channel = rankSize-1 | 转置语义，Write 数 = rankSize-1，GroupCopy = 1 | 均匀 sizes 矩阵 |
| Mesh1D2Die | 可能 13+ taskArgs（加 withMyRank/rankGroup），channel 按 die 分组 | 按 die 分片后转置，操作计数不同 | sizes 矩阵按 die 分 |
| MultiJetty | 可能加 jettyNums 字段，channel 按 jetty 分 | 按 jetty 分流后转置 | sizes 矩阵按 jetty 分 |
| Mesh2Die | channel 按 die 分，参数布局不同 | 按 die 分片 | sizes 矩阵按 die 分 |
| Concurrent | 双 kernel 并行，两个 adapter | 两组期望数据 | 双 sizes 矩阵 |

#### 不同通信原语的差异

| 算法大类 | 通信语义 | 期望数据计算 | 场景参数 |
|---------|---------|------------|---------|
| AllToAll | 转置：每个 rank 给每个 rank 发不同数据 | `recv[i*stride] = send_j[myRank*stride]` | `sizes` 矩阵 |
| AllGather | 拼接：每个 rank 把自己的数据广播给所有 rank | `recv = [rank0 全部][rank1 全部]...` | `count`（单 rank 数据量） |
| ReduceScatter | 归约后切片：每个 rank 收到归约结果的一部分 | `recv = reduce(sendBuf_0[i] + sendBuf_1[i] + ...)` | `count` + `reduceType` |
