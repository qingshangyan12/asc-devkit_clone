<!--
@source: impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h
@source: include/adv_api/hccl/internal/hccl_msg.h
-->

# Commit/Wait 时序演示

本文档详细演示 Commit 和 Wait 机制的时序和计数逻辑。

## 基本概念

### TurnCnt 结构

> `hccl_msg.h:136-140`

```cpp
struct TurnCnt {
    uint64_t valid;   // COMMIT_VALID_MASK (987654321)
    uint64_t cnt;     // 计数值
    uint64_t reserved[6];
};
```

### 两种 TurnCnt

| 类型 | 写入方 | 读取方 | 说明 |
|------|--------|--------|------|
| `commitTurnCnt` | Client (Commit) | Server (轮询) | 表示需要执行的任务数 |
| `finishedTurnCnt` | Server (完成后) | Client (Wait) | 表示已完成的任务数 |

### 计数关系

```
handleIdRepeat_[handle]      — Prepare 时设置的 repeat 值
handleIdCommitTurnCnt_[handle] — Client 累计的 commit 计数
finishedTurnCnt[pos].cnt      — Server 累计的完成计数

约束: commitCnt <= repeat * stepCntsPerRepeat
等待: finishedTurnCnt >= waitCnt
```

## 单次 Prepare + Commit + Wait

```mermaid
sequenceDiagram
    autonumber
    participant C as Client (AICore)
    participant SM as 共享内存
    participant S as Server (AICPU)

    Note over C: Prepare(repeat=1)
    C->>SM: 轮询 sendMsgs[0].valid
    SM-->>C: 可用 (valid != MASK)
    C->>SM: 写入 HcclMsg
    C->>SM: valid = HCCL_MSG_VALID_MASK
    C->>C: handleIdCommitTurnCnt_[h] = 0
    C->>C: handleIdRepeat_[h] = 1

    Note over C: Commit(handle)
    C->>SM: 轮询 commitTurnCnt[0].cnt < expected
    SM-->>C: 可写
    C->>SM: commitTurnCnt[0].cnt = 1
    C->>SM: valid = COMMIT_VALID_MASK
    C->>C: handleIdCommitTurnCnt_[h] = 1

    Note over S: Server 检测 commit
    S->>SM: 轮询 commitTurnCnt[0].cnt > 0
    SM-->>S: cnt=1 (有任务)
    S->>SM: 读取 sendMsgs[0]
    S->>S: 执行算法...
    S->>SM: finishedTurnCnt[0].cnt = 1

    Note over C: Wait(handle)
    C->>C: waitCnt = 0 → 1
    C->>SM: 轮询 finishedTurnCnt[0].cnt >= 1
    SM-->>C: cnt=1 (完成)
    C-->>C: Wait 返回 0
```

## repeat=3 场景

```mermaid
sequenceDiagram
    participant C as Client
    participant SM as 共享内存
    participant S as Server

    Note over C: Prepare(repeat=3) → handle
    C->>SM: 写入 HcclMsg (repeatCnt=3)
    C->>C: handleIdRepeat_[h] = 3
    C->>C: handleIdCommitTurnCnt_[h] = 0

    Note over C: Commit #1
    C->>SM: commitTurnCnt[0].cnt = 1
    S->>SM: 检测 cnt=1
    S->>S: 执行第 1 次
    S->>SM: finishedTurnCnt[0].cnt = 1

    Note over C: Wait #1
    C->>SM: 轮询 finishedTurnCnt >= 1
    SM-->>C: 完成
    C-->>C: Wait 返回

    Note over C: Commit #2
    C->>SM: commitTurnCnt[0].cnt = 2
    S->>SM: 检测 cnt=2
    S->>S: 执行第 2 次
    S->>SM: finishedTurnCnt[0].cnt = 2

    Note over C: Wait #2
    C->>SM: 轮询 finishedTurnCnt >= 2
    SM-->>C: 完成

    Note over C: Commit #3
    C->>SM: commitTurnCnt[0].cnt = 3
    S->>SM: 检测 cnt=3
    S->>S: 执行第 3 次
    S->>SM: finishedTurnCnt[0].cnt = 3

    Note over C: Wait #3
    C->>SM: 轮询 finishedTurnCnt >= 3
    SM-->>C: 完成
```

## commit=true 自动提交

当 Prepare 的 `commit` 模板参数为 `true` 时，`CommonPrepareImpl` 内部自动调用 `SetCommitTurnCntToGm`：

```mermaid
flowchart TD
    P["CommonPrepareImpl(commit=true)"] --> S1["校验参数"]
    S1 --> S2["分配 handleId"]
    S2 --> S3["SendMsgToServer()"]
    S3 --> S4{"commit == true?"}
    S4 -->|是| S5["SetCommitTurnCntToGm()<br>commitTurnCnt = repeat * stepCnts"]
    S4 -->|否| S6["跳过"]
    S5 --> S7["返回 handleId"]
    S6 --> S7
```

> 见 `hccl_aicpu_impl.h:274-276`。

## commit=false 手动提交

```mermaid
sequenceDiagram
    participant C as Client
    participant SM as 共享内存

    C->>C: Prepare<false>(repeat=1) → handle
    C->>SM: 写入 HcclMsg (valid=MASK)
    Note over C: 此时 commitTurnCnt 仍为 0

    Note over C: 可以做其他计算...

    C->>C: Commit(handle)
    C->>SM: commitTurnCnt[pos].cnt = 1
    Note over C: Server 开始执行

    C->>C: Wait(handle)
    C->>SM: 轮询 finishedTurnCnt >= 1
```

## 多 Handle 并发场景

```mermaid
sequenceDiagram
    participant C as Client
    participant SM as 共享内存
    participant S as Server

    C->>SM: Prepare handle1 → sendMsgs[0]
    C->>SM: Prepare handle2 → sendMsgs[1]
    C->>SM: Commit(handle1) → commitTurnCnt[0]=1
    C->>SM: Commit(handle2) → commitTurnCnt[1]=1

    par Server 处理 handle1
        S->>SM: 读取 sendMsgs[0]
        S->>S: 执行算法
        S->>SM: finishedTurnCnt[0]=1
    and Server 处理 handle2
        S->>SM: 读取 sendMsgs[1]
        S->>S: 执行算法
        S->>SM: finishedTurnCnt[1]=1
    end

    C->>SM: Wait(handle1) → 轮询 finishedTurnCnt[0]>=1 ✓
    C->>SM: Wait(handle2) → 轮询 finishedTurnCnt[1]>=1 ✓
```

## 计数校验逻辑

### Commit 校验

> `hccl_aicpu_impl.h:469-477`

```cpp
const uint16_t commitCnt = handleIdCommitTurnCnt_[handleId];
if (commitCnt >= handleIdRepeat_[handleId] * GetStepCntsPerRepeatByHandle(handleId)) {
    // 错误：Commit 次数超过 repeat 上限
    return;
}
```

### Wait 校验

> `hccl_aicpu_impl.h:431-438`

```cpp
uint16_t& waitCnt = handleIdWaitCallNum_[handleId];
if (waitCnt >= handleIdCommitTurnCnt_[handleId]) {
    // 错误：Wait 次数超过 Commit 次数
    return HCCL_FAILED;
}
```

## Finalize 时序

```mermaid
sequenceDiagram
    participant C as Client
    participant SM as 共享内存
    participant S as Server

    Note over C: Finalize<sync=true>()
    C->>C: 等待最后一个 handle 完成
    C->>SM: Query(lastHandle) 检查完成

    Note over C: 发送 Finalize 消息
    C->>SM: SendFinalizeMsg()
    C->>SM: sendMsgs[pos].commType = HCCL_CMD_FINALIZE

    Note over S: Server 检测 Finalize
    S->>SM: 读取 FINALIZE 消息
    S->>S: 执行完剩余任务
    S->>SM: finishedTurnCnt[pos].cnt = FINALIZE_FINISH_CNT

    Note over C: 等待 Server 退出
    C->>SM: 轮询 finishedTurnCnt == FINALIZE_FINISH_CNT
    SM-->>C: 确认退出

    C->>SM: ResetFinishedTurnCnt()
    Note over C: 清理完成，可开始新通信
```

> `FINALIZE_FINISH_CNT = 1234567899999999999`，见 `hccl_msg.h:29`。

## 多队列模式

当 `queueNum_ > 0` 时，Commit/Wait 行为变化：

| 行为 | 单队列 (queueNum_=0) | 多队列 (queueNum_>0) |
|------|---------------------|---------------------|
| Commit | 写入 `commitTurnCnt[pos]` | 不写 commitTurnCnt（消息自带 valid） |
| Wait | 阻塞轮询 `finishedTurnCnt` | 直接返回 `HCCL_SUCCESS` |
| Query | 查询 `finishedTurnCnt` | 返回 0 |
| 消息位置 | `curMsgPosition_[0]` | `curMsgPosition_[queId]` |

> 多队列模式下 Wait 不阻塞，依赖队列自身的同步机制。
