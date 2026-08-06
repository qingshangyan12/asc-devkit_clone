<!--
@source: include/adv_api/hccl/hccl.h
@source: include/adv_api/hccl/hccl_common.h
@source: include/adv_api/hccl/internal/hccl.h
@source: impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h
-->

# 集合通信算子

本文档详细列出 HCCL 提供的所有集合通信算子 API。

## 设备侧 API（Hccl 类方法）

以下接口均在 `Hccl` 模板类中声明，运行在 AICore 上。

### AllReduce

> `include/adv_api/hccl/hccl.h:76-78`

```cpp
template <bool commit = false>
__aicore__ inline HcclHandle AllReduce(
    GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t count,
    HcclDataType dataType, HcclReduceOp op, uint8_t repeat = 1);
```

**功能**：对所有 rank 的 `sendBuf` 中 `count` 个数据执行 `op` 规约，结果复制到每个 rank 的 `recvBuf`。

**参数**：

| 参数 | 方向 | 说明 |
|------|------|------|
| `sendBuf` | in | 源数据缓冲区地址 |
| `recvBuf` | out | 结果数据缓冲区地址 |
| `count` | in | 参与运算的数据数量 |
| `dataType` | in | 数据类型 |
| `op` | in | 规约操作（SUM/PROD/MAX/MIN） |
| `repeat` | in | 连续内存场景下重复次数（默认 1） |

**repeat 机制**：当 `repeat > 1` 时，自动计算后续任务的缓冲区地址：
```
sendBuf[i] = sendBuf + count * sizeof(datatype) * i
recvBuf[i] = recvBuf + count * sizeof(datatype) * i
```

**实现**：见 `hccl_aicpu_impl.h:327-336`，构造 `CommonPrepareParam` 后调用 `CommonPrepareImpl`。

---

### AllGather

> `include/adv_api/hccl/hccl.h:102-105`

```cpp
template <bool commit = false>
__aicore__ inline HcclHandle AllGather(
    GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t sendCount,
    HcclDataType dataType, uint64_t strideCount, uint8_t repeat = 1);
```

**功能**：收集所有 rank 的 `sendCount` 个数据到每个 rank 的 `recvBuf`，按 rank 顺序排列。

**参数**：

| 参数 | 方向 | 说明 |
|------|------|------|
| `sendBuf` | in | 源数据缓冲区 |
| `recvBuf` | out | 结果缓冲区（大小 = sendCount * rankSize） |
| `sendCount` | in | 每次 AllGather 发送的数据数量 |
| `dataType` | in | 数据类型 |
| `strideCount` | in | 步长（>0 时从 rank i 接收数据偏移 i*strideCount；=0 时偏移 i*sendCount） |
| `repeat` | in | 重复次数 |

**实现**：见 `hccl_aicpu_impl.h:338-346`。

---

### ReduceScatter

> `include/adv_api/hccl/hccl.h:132-135`

```cpp
template <bool commit = false>
__aicore__ inline HcclHandle ReduceScatter(
    GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t recvCount,
    HcclDataType dataType, HcclReduceOp op, uint64_t strideCount, uint8_t repeat = 1);
```

**功能**：对所有 rank 的 `sendBuf` 数据执行 `op` 规约，结果分片散布到各 rank 的 `recvBuf`（rank i 获得 第 i 块结果）。

**参数**：

| 参数 | 方向 | 说明 |
|------|------|------|
| `sendBuf` | in | 源数据缓冲区（大小 = recvCount * rankSize） |
| `recvBuf` | out | 结果缓冲区（接收第 i 块） |
| `recvCount` | in | 接收数据数量 |
| `dataType` | in | 数据类型 |
| `op` | in | 规约操作 |
| `strideCount` | in | 步长计数 |
| `repeat` | in | 重复次数 |

**实现**：见 `hccl_aicpu_impl.h:348-360`。

---

### AlltoAll

> `include/adv_api/hccl/hccl.h:164-167`

```cpp
template <bool commit = false>
__aicore__ inline HcclHandle AlltoAll(
    GM_ADDR sendBuf, GM_ADDR recvBuf, uint64_t dataCount,
    HcclDataType dataType, uint64_t strideCount = 0, uint8_t repeat = 1);
```

**功能**：全互连数据交换。sendBuf 的第 j 块发送到 rank j，recvBuf 的第 i 块接收来自 rank i 的数据。

**参数**：

| 参数 | 方向 | 说明 |
|------|------|------|
| `sendBuf` | in | 源数据缓冲区 |
| `recvBuf` | out | 结果缓冲区 |
| `dataCount` | in | 发送给每个 rank 的数据量（也等于从每个 rank 接收的数据量） |
| `dataType` | in | 数据类型 |
| `strideCount` | in | 步长（>0 时第 j 块偏移 j*strideCount；=0 时偏移 j*dataCount） |
| `repeat` | in | 重复次数 |

**实现**：见 `hccl_aicpu_impl.h:362-370`。

---

### AlltoAllV

> `include/adv_api/hccl/hccl.h:202-205`

```cpp
template <bool commit = false>
__aicore__ inline HcclHandle AlltoAllV(
    GM_ADDR sendBuf, void* sendCounts, void* sdispls, HcclDataType sendType,
    GM_ADDR recvBuf, void* recvCounts, void* rdispls, HcclDataType recvType,
    uint8_t repeat = 1);
```

**功能**：变长全互连数据交换，每个 rank 发送/接收不同数量的数据。

**参数**：

| 参数 | 说明 |
|------|------|
| `sendBuf` | 源数据缓冲区 |
| `sendCounts` | uint64 数组，`sendCounts[i]` = 发送给 rank i 的数据量 |
| `sdispls` | uint64 数组，`sdispls[i]` = 发送给 rank i 的数据在 sendBuf 中的偏移 |
| `sendType` | 发送数据类型 |
| `recvBuf` | 结果缓冲区 |
| `recvCounts` | uint64 数组，`recvCounts[i]` = 从 rank i 接收的数据量 |
| `rdispls` | uint64 数组，`rdispls[i]` = 从 rank i 接收的数据在 recvBuf 中的偏移 |
| `recvType` | 接收数据类型 |
| `repeat` | 重复次数 |

**约束**：`sendType` 必须等于 `recvType`。

**实现**：见 `hccl_aicpu_impl.h:372-394`。AlltoAllV 会额外通过 `SendMsgToServer(AlltoAllVParamExt)` 发送扩展参数到 `HcclMsgExt`。

---

### BatchWrite

> `include/adv_api/hccl/hccl.h:229-230`

```cpp
template <bool commit = false>
__aicore__ inline HcclHandle BatchWrite(
    GM_ADDR batchWriteInfo, uint32_t itemNum, uint16_t queueID = 0U);
```

**功能**：批量写入指定 rank 的内存地址。每次通信发送一段数据到指定 rank 的指定地址。

**参数**：

| 参数 | 说明 |
|------|------|
| `batchWriteInfo` | GM 内存地址，存储 BatchWriteItem 结构数组 |
| `itemNum` | 批量通信项数 |
| `queueID` | 队列 ID |

**BatchWriteItem 结构**（见 hccl.h 注释）：

```cpp
struct BatchWriteItem {
    uint64_t localBuf;      // 本地窗口缓冲区 GM 地址
    uint64_t remoteBuf;     // 远端窗口缓冲区 GM 地址
    uint64_t count;         // 传输数据数量
    uint32_t dataType;      // 数据类型
    uint32_t remoteRankId;  // 目标 rank ID
};
```

---

### AlltoAllvWrite

> `include/adv_api/hccl/hccl.h:252-254`

```cpp
template <bool commit = false>
__aicore__ inline HcclHandle AlltoAllvWrite(
    GM_ADDR usrIn, GM_ADDR sendOffsets, GM_ADDR sendSizes,
    uint64_t remoteWinOffset, uint64_t localDataSize);
```

**功能**：批量写入远端 rank 窗口，用于 sendOffsets/sendSizes 较大的场景。

**参数**：

| 参数 | 说明 |
|------|------|
| `usrIn` | 源数据缓冲区 |
| `sendOffsets` | uint64 数组，`sendOffsets[i]` = 发送给 rank i 的数据偏移（字节） |
| `sendSizes` | uint64 数组，`sendSizes[i]` = 发送给 rank i 的数据大小（字节） |
| `remoteWinOffset` | 远端 rank 上的数据偏移（字节） |
| `localDataSize` | 发送到本地 rank 的数据大小（字节） |

> 注：`sendSizes` 大小为 rankDim，`sendSizes[localRank]` 不使用，用 `localDataSize` 代替。

---

## Host 侧 C API

以下接口在 `include/adv_api/hccl/internal/hccl.h` 中声明，运行在 Host CPU 上。

| 函数 | 签名 | 说明 |
|------|------|------|
| `HcclAllReduce` | `(sendBuf, recvBuf, count, dataType, op, comm, stream)` | AllReduce |
| `HcclBroadcast` | `(buf, count, dataType, root, comm, stream)` | Broadcast |
| `HcclReduceScatter` | `(sendBuf, recvBuf, recvCount, dataType, op, comm, stream)` | ReduceScatter |
| `HcclReduceScatterV` | `(sendBuf, sendCounts, sendDispls, recvBuf, recvCount, dataType, op, comm, stream)` | ReduceScatterV |
| `HcclScatter` | `(sendBuf, recvBuf, recvCount, dataType, root, comm, stream)` | Scatter |
| `HcclAllGather` | `(sendBuf, recvBuf, sendCount, dataType, comm, stream)` | AllGather |
| `HcclAllGatherV` | `(sendBuf, sendCount, recvBuf, recvCounts, recvDispls, dataType, comm, stream)` | AllGatherV |
| `HcclSend` | `(sendBuf, count, dataType, destRank, comm, stream)` | Send |
| `HcclRecv` | `(recvBuf, count, dataType, srcRank, comm, stream)` | Recv |
| `HcclAlltoAllVC` | `(sendBuf, sendCountMatrix, sendType, recvBuf, recvType, comm, stream)` | AlltoAllVC |
| `HcclAlltoAllV` | `(sendBuf, sendCounts, sdispls, sendType, recvBuf, recvCounts, rdispls, recvType, comm, stream)` | AlltoAllV |
| `HcclAlltoAll` | `(sendBuf, sendCount, sendType, recvBuf, recvCount, recvType, comm, stream)` | AlltoAll |
| `HcclReduce` | `(sendBuf, recvBuf, count, dataType, op, root, comm, stream)` | Reduce |
| `HcclBatchSendRecv` | `(sendRecvInfo, itemNum, comm, stream)` | BatchSendRecv |

> 定义见 `internal/hccl.h:16-260`。
