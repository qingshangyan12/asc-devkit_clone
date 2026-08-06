<!--
@source: include/adv_api/hccl/hccl.h
@source: impl/adv_api/detail/hccl/impl/hccl_impl.h
@source: impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h
@source: impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/execute_selector.cc
@source: impl/adv_api/detail/hccl/cc/src/ops/op_common/selector/auto_selector_base.cc
@source: impl/adv_api/detail/hccl/cc/src/ops/all_gather/
-->

# 完整调用流程

以 AllGather 为例，展示从用户 Kernel 调用到数据传输完成的完整调用链。

## 全局时序图

```mermaid
sequenceDiagram
    autonumber
    participant User as 用户 Kernel
    participant Hccl as Hccl 类
    participant Impl as HcclImpl
    participant Mem as HcclMsgArea共享内存
    participant Srv as AICPU Server
    participant Sel as ExecuteSelector
    participant Reg as ExecRegistry
    participant Exec as Executor
    participant Tmpl as Template
    participant Ch as Channel

    Note over User,Mem: 阶段 1: 初始化
    User->>Hccl: InitV2(context, initTiling)
    Hccl->>Impl: impl_.InitV2()
    Impl->>Mem: 设置 hcclMsgArea_ 指针
    Impl->>Impl: InitWorkingFlag()
    Impl-->>User: 初始化完成

    User->>Hccl: SetCcTilingV2(offset)
    Hccl->>Impl: impl_.SetCcTilingV2(offset)
    Impl->>Impl: ccOpTilingDataTable_[opType] = offset
    Impl-->>User: 设置完成

    Note over User,Ch: 阶段 2: Prepare（异步下发任务）
    User->>Hccl: AllGather<true>(sendBuf, recvBuf, count, ...)
    Hccl->>Impl: impl_.AllGather<true>()
    Impl->>Impl: 构造 CommonPrepareParam
    Impl->>Impl: CommonPrepareImpl()
    Note right of Impl: 1.校验参数 2.分配handleId 3.获取消息槽位

    Impl->>Mem: 轮询 sendMsgs[pos].valid
    Mem-->>Impl: valid != MASK (可用)
    Impl->>Mem: 写入 HcclMsg (commType, buffers, ...)
    Impl->>Mem: 设置 valid = HCCL_MSG_VALID_MASK
    Impl->>Mem: FlushDataCache

    Note right of Impl: commit=true, 自动提交
    Impl->>Mem: 写入 commitTurnCnt[pos].cnt
    Impl->>Mem: 设置 valid = COMMIT_VALID_MASK

    Impl-->>User: 返回 handleId

    Note over User,Ch: 阶段 3: Server 处理（并行进行）
    Srv->>Mem: 轮询 sendMsgs[pos].valid
    Mem-->>Srv: valid == MASK (有消息)
    Srv->>Srv: 解析 HcclMsg, 从 tilingData 获取 algName
    Srv->>Mem: 重置 valid（消息已读）

    Note over Srv: AICPU 服务端不经 ExecuteSelector
    Note over Srv: algName 已在 Host 侧 Tiling 阶段确定
    Srv->>Reg: CollAlgExecRegistry::GetAlgExec(algName)
    Reg-->>Srv: CollExecutorBase 执行器

    Srv->>Exec: executor->Orchestrate(param, resCtx)
    Note over Srv,Exec: 申请 notify, thread, channel

    Exec->>Tmpl: AlgTemplateBase::Prepare(inputMem, outputMem, ...)
    Exec->>Tmpl: RunAsync(rank, rankSize, channels)

    loop 数据传输
        Tmpl->>Ch: 通过 Channel 发送/接收数据
        Ch->>Ch: SDMA/RDMA 传输
    end

    Srv->>Mem: 递增 finishedTurnCnt[pos].cnt

    Note over User,Ch: 阶段 4: Wait（等待完成）
    User->>Hccl: Wait(handleId)
    Hccl->>Impl: impl_.Wait(handleId)
    Impl->>Mem: 轮询 finishedTurnCnt[pos].cnt >= expected
    Note right of Impl: 阻塞等待...
    Mem-->>Impl: cnt >= expected (完成)
    Impl-->>User: 返回 0 (成功)

    Note over User,Ch: 阶段 5: Finalize
    User->>Hccl: Finalize<true>()
    Hccl->>Impl: impl_.Finalize<true>()
    Impl->>Mem: 等待最后一个任务完成
    Impl->>Mem: 发送 HCCL_CMD_FINALIZE 消息
    Note over Impl,Mem: 等待 finishedTurnCnt == FINALIZE_FINISH_CNT
    Mem-->>Impl: Server 已退出
    Impl->>Mem: ResetFinishedTurnCnt()
    Impl-->>User: Finalize 完成
```

## 简化流程图

```mermaid
flowchart LR
    subgraph 设备侧
        A1["InitV2"] --> A2["SetCcTilingV2"]
        A2 --> A3["Prepare<br>写 HcclMsg"]
        A3 --> A4["Commit<br>写 commitTurnCnt"]
        A4 --> A5["Wait<br>轮询 finishedTurnCnt"]
        A5 --> A6["Finalize<br>发送退出消息"]
    end

    subgraph 共享内存
        M1["sendMsgs"]
        M2["commitTurnCnt"]
        M3["finishedTurnCnt"]
    end

    subgraph AICPU Server
        B1["轮询 sendMsgs"] --> B2["从 tilingData 获取 algName<br>不经选择器"]
        B2 --> B3["CollAlgExecRegistry<br>获取执行器并编排"]
        B3 --> B4["模板执行<br>Channel 传输"]
        B4 --> B5["写回 finishedTurnCnt"]
    end

    A3 -->|写入| M1
    A4 -->|写入| M2
    M1 -->|轮询| B1
    M2 -->|轮询| B1
    B5 -->|写入| M3
    M3 -->|轮询| A5
```

## 关键步骤说明

### 1. 初始化阶段

- `InitV2` 设置共享内存地址 `hcclMsgArea_`，初始化 Handle 表
- `SetCcTilingV2` 建立算子类型到 Tiling 数据偏移的映射
- 这两个步骤只需执行一次

### 2. Prepare 阶段（异步）

- 构造 `CommonPrepareParam`，包含算子类型、缓冲区地址、数据量等
- 轮询共享内存中消息槽位是否可用（Server 已读取上一条消息）
- 写入 `HcclMsg` 并设置 `valid` 标志
- 分配 Handle ID 并记录元信息
- 如果 `commit=true`，自动写入 commit 计数

### 3. Commit 阶段

- 递增 `commitTurnCnt`，通知 Server 执行
- 每次 Commit 对应一次任务执行
- Commit 次数不超过 `repeat * stepCntsPerRepeat`

### 4. Server 处理阶段（并行）

- Server 轮询检测 `sendMsgs` 中的 `valid` 标志
- 检测到消息后，执行算法选择 → 执行器编排 → 模板执行
- 数据通过 Channel（SDMA/RDMA）传输
- 完成后递增 `finishedTurnCnt`

### 5. Wait 阶段（阻塞）

- 轮询 `finishedTurnCnt` 是否达到预期值
- 达到后返回成功
- Wait 调用次数应与 Commit 一致

### 6. Finalize 阶段

- 等待最后一个任务完成
- 发送 `HCCL_CMD_FINALIZE` 控制消息
- 等待 Server 写入 `FINALIZE_FINISH_CNT`
- 重置 `finishedTurnCnt` 供下次使用

## 多次 Prepare 场景

```mermaid
sequenceDiagram
    participant User
    participant Impl
    participant Mem
    participant Srv

    User->>Impl: AllGather<false>(...) → handle1
    Impl->>Mem: 写入 sendMsgs[0]
    User->>Impl: AllGather<false>(...) → handle2
    Impl->>Mem: 写入 sendMsgs[1]

    User->>Impl: Commit(handle1)
    Impl->>Mem: commitTurnCnt[0]++
    User->>Impl: Commit(handle2)
    Impl->>Mem: commitTurnCnt[1]++

    par Server 并行处理
        Srv->>Mem: 处理 sendMsgs[0]
        Srv->>Mem: finishedTurnCnt[0]++
    and
        Srv->>Mem: 处理 sendMsgs[1]
        Srv->>Mem: finishedTurnCnt[1]++
    end

    User->>Impl: Wait(handle1)
    Impl->>Mem: 轮询 finishedTurnCnt[0]
    Mem-->>Impl: 完成
    User->>Impl: Wait(handle2)
    Impl->>Mem: 轮询 finishedTurnCnt[1]
    Mem-->>Impl: 完成
```

> 注意：Wait 必须按 Prepare 的顺序调用（见 `hccl.h:314` 的注释）。
