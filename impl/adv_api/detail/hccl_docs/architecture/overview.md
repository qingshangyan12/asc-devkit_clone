<!--
@source: include/adv_api/hccl/hccl.h
@source: include/adv_api/hccl/hccl_common.h
@source: impl/adv_api/detail/hccl/impl/hccl_impl_def.h
@source: impl/adv_api/detail/hccl/impl/hccl_impl.h
@source: impl/adv_api/detail/hccl/common/hccl_base.h
@source: impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h
@source: impl/adv_api/detail/hccl/cc/src/ops/
@source: impl/adv_api/detail/hccl/cc/src/aicpu_kfc/
-->

# 整体架构总览

## 架构分层

HCCL 采用 **四层架构**，从上到下依次为公共 API 层、设备侧实现层、Host 侧算法层、AICPU 服务端层。

```mermaid
flowchart TD
    subgraph L1["公共API层"]
        A1["Hccl 模板类<br>hccl.h"]
        A2["类型定义<br>hccl_common.h"]
        A3["Tiling 配置<br>hccl_tiling.h"]
        A4["Host C 接口<br>internal/hccl.h"]
    end

    subgraph L2["设备侧实现层"]
        B0["HcclImpl 基类<br>common/hccl_base.h"]
        B1["平台分发<br>impl/hccl_impl_def.h"]
        B2["V220 AICPU<br>impl/platform_v220/"]
        B3["V310 AICPU<br>impl/platform_v310/hccl_aicpu.h"]
        B4["V310 CCU<br>impl/platform_v310/hccl_ccu_v0.h"]
        B5["公共 AICPU 实现<br>common/hccl_aicpu_impl.h"]
        B6["CCU Prepare<br>ccu/hccl_ccu_v0_prepare.h"]
    end

    subgraph L3["Host侧算法层"]
        C1["算子入口<br>all_gather_op.cc / reduce_scatter_op.cc"]
        C2["算法选择器<br>selector/"]
        C3["执行器<br>executor/"]
        C4["算法模板<br>template/"]
        C5["拓扑识别<br>topo/"]
    end

    subgraph L4["AICPU服务端"]
        D1["RPC 入口<br>hccl_aicpu_interface.cc"]
        D2["HCCL Process<br>aicpu_hccl_process.cc"]
        D3["通信器<br>aicpu_communicator.cc"]
        D4["DFX<br>debug/dfx/"]
    end

    A1 --> B0
    A1 --> B1
    B1 --> B2
    B1 --> B3
    B1 --> B4
    B2 --> B5
    B3 --> B5
    B4 --> B6
    B5 -->|共享内存 HcclMsgArea| D2
    D2 --> D3
    D2 --> C1
    C1 --> C2
    C2 --> C3
    C3 --> C4
    C1 --> C5
```

## 核心设计思想

### 1. Client-Server 异步模型

设备侧（AICore）作为 Client，AICPU 作为 Server，通过共享内存 `HcclMsgArea` 进行通信：

- **Client**（AICore）：调用 `Prepare` 写消息 → 调用 `Commit` 通知执行 → 调用 `Wait` 等待完成
- **Server**（AICPU）：轮询共享内存 → 解析消息 → 算法选择 → 执行数据传输 → 写回完成计数

### 2. 编译期平台分发

通过 `__NPU_ARCH__` 宏在编译期选择对应平台的实现，无运行时开销：

```mermaid
flowchart LR
    H["hccl.h<br>Hccl 模板类"] --> D["hccl_impl_def.h"]
    D -->|__NPU_ARCH__==2201| V220["hccl_v220_impl.h"]
    D -->|__NPU_ARCH__==3510| V310["hccl_v310_impl.h"]
    V220 --> P220["platform_v220/hccl_aicpu.h"]
    V310 --> P310A["platform_v310/hccl_aicpu.h"]
    V310 --> P310C["platform_v310/hccl_ccu_v0.h"]
```

> 分发逻辑见 `impl/adv_api/detail/hccl/impl/hccl_impl_def.h:20-26`。

### 3. 注册机制驱动

Host 侧算法层使用三种注册机制实现可扩展性：

| 注册类型 | 注册表 | 宏 | 说明 |
|----------|--------|-----|------|
| 选择器注册 | `SelectorRegistry` | `REGISTER_SELECTOR_BY_OPTYPE` | 按算子类型注册算法选择器 |
| 执行器注册 | `CollAlgExecRegistryV2` | `REGISTER_EXEC_V2` | 按算法名注册执行器 |
| 模板注册 | `AlgTemplateRegistry` | `REGISTER_TEMPLATE` | 按模板类型注册算法模板 |

### 4. 算法选择 Fallback 链

`AutoSelectorBase::Select` 按以下优先级尝试匹配算法，未匹配则降级：

```mermaid
flowchart LR
    DPU["SelectDPUAlgo"] -->|NOT_MATCH| CCU_MS["SelectCcuMsAlgo"]
    CCU_MS -->|NOT_MATCH| CCU_SCHED["SelectCcuScheduleAlgo"]
    CCU_SCHED -->|NOT_MATCH| AIV["SelectAivAlgo"]
    AIV -->|NOT_MATCH| AICPU["SelectAicpuAlgo"]
    AICPU -->|NOT_MATCH| FAIL["HCCL_E_NOT_SUPPORT"]
```

> 逻辑见 `impl/.../cc/src/ops/op_common/selector/auto_selector_base.cc:15-91`。

## 模块间数据流

HCCL 存在两条执行路径，需严格区分：

### 路径一：Host 侧算子执行（Host C API 调用链）

当 Host 侧调用 `HcclAllGather()` 等 C API 时，在 Host CPU 上执行算法选择和编排：

```mermaid
flowchart TD
    HAPI["Host C API<br>HcclAllGather()"] --> AGOP["AllGatherOutPlace<br>all_gather_op.cc"]
    AGOP --> SEL_FN["Selector()<br>op_common.cc:111"]
    SEL_FN --> ES["ExecuteSelector::Run()<br>execute_selector.cc"]
    ES --> AS["AllGatherAutoSelector::Select()<br>all_gather_auto_selector.cc"]
    AS -->|algName| HEOP["HcclExecOp()<br>op_common.cc:315"]
    HEOP --> REG2["CollAlgExecRegistryV2::GetAlgExec<br>op_common.cc:341"]
    REG2 --> EXEC1["InsCollAlgBase::Orchestrate()"]
    EXEC1 --> TMPL1["AlgTemplateBase::RunAsync()"]
    TMPL1 --> CH1["Channel 数据传输"]
```

> 依据：`all_gather_op.cc:225-237`（AllGatherOutPlace 调用 Selector 和 HcclExecOp）、`op_common.cc:111-132`（Selector 调用 ExecuteSelector::Run）、`op_common.cc:341`（HcclExecOp 调用 CollAlgExecRegistryV2）

### 路径二：AICPU 服务端执行（设备侧 Client-Server 调用链）

当设备侧 Kernel 通过 `Hccl::AllGather()` 下发任务时，AICPU Server 轮询消息后执行：

```mermaid
flowchart TD
    UK["用户 Kernel"] --> HC["Hccl::AllGather 模板类"]
    HC --> HI["HcclImpl::CommonPrepareImpl"]
    HI --> SM["SendMsgToServer 写入 HcclMsg"]
    SM -->|共享内存| HM["HcclMsgArea"]
    HM -->|AICPU 轮询检测消息| R2["RunAicpuRpcSrvLaunchV2<br>hccl_aicpu_interface.cc:40"]
    R2 --> AP["AicpuHcclProcess::AicpuRunRpcServerV2<br>aicpu_hccl_process.cc:376"]
    AP --> EO["HcclCommAicpu::ExecOp<br>aicpu_communicator.cc:2014"]
    EO --> OR["HcclCommAicpu::Orchestrate<br>aicpu_communicator.cc:2291"]
    OR -->|algName from tilingData| REG1["CollAlgExecRegistry::GetAlgExec<br>aicpu_communicator.cc:2314"]
    REG1 --> EXEC2["CollExecutorBase 执行器"]
    EXEC2 --> TMPL2["算法模板执行"]
    TMPL2 --> CH2["Channel 数据传输"]
    CH2 -->|写回 finishedTurnCnt| HM2["HcclMsgArea"]
    HM2 -->|WaitFinishCntFromGm| HI2["HcclImpl::Wait"]
    HI2 -->|返回| UK
```

> 依据：`aicpu_hccl_process.cc:556`（AicpuRunRpcServerV2 调用 ExecOp）、`aicpu_communicator.cc:2053`（ExecOp 调用 Orchestrate）、`aicpu_communicator.cc:2314`（Orchestrate 通过 CollAlgExecRegistry 获取执行器）

**关键区别**：
- Host 侧路径使用 `CollAlgExecRegistryV2`（V2 注册表），通过 `ExecuteSelector` 选择算法
- AICPU 服务端路径使用 `CollAlgExecRegistry`（V1 注册表），算法名直接从 `tilingData->algName` 获取，不经选择器

## 关键文件索引

| 文件 | 行数 | 说明 |
|------|------|------|
| `include/adv_api/hccl/hccl.h` | 427 | Hccl 模板类公共 API |
| `include/adv_api/hccl/hccl_common.h` | 105 | 枚举与类型定义 |
| `include/adv_api/hccl/hccl_tiling.h` | 57 | Mc2CcTilingConfig |
| `include/adv_api/hccl/internal/hccl.h` | 261 | Host C 接口 |
| `include/adv_api/hccl/internal/hccl_msg.h` | 227 | 消息结构定义 |
| `impl/.../hccl/impl/hccl_impl.h` | 214 | Hccl→HcclImpl 转发 |
| `impl/.../hccl/common/hccl_aicpu_impl.h` | 542 | AICPU 设备侧核心实现 |
| `impl/.../hccl/common/hccl_inner_def.h` | 190 | 内部数据结构 |
| `impl/.../hccl/cc/src/ops/op_common/selector/auto_selector_base.cc` | 416 | 算法选择基类 |
| `impl/.../hccl/cc/src/ops/op_common/selector/execute_selector.cc` | 70 | 选择器执行入口 |
| `impl/.../hccl/cc/src/ops/op_common/template/alg_template_base.h` | 206 | 算法模板基类 |
| `impl/.../hccl/cc/src/ops/op_common/executor/executor_v2_base.h` | 76 | V2 执行器基类 |
| `impl/.../hccl/cc/src/ops/op_common/inc/alg_param.h` | 612 | OpParam 等核心参数 |
