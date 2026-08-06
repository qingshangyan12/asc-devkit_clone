<!--
@source: include/adv_api/hccl/hccl.h
@source: include/adv_api/hccl/hccl_common.h
-->

# 查询与工具 API

## GetWindowsInAddr

> `include/adv_api/hccl/hccl.h:380`

```cpp
__aicore__ inline GM_ADDR GetWindowsInAddr(uint32_t rankId);
```

**功能**：获取指定 rank 的输入窗口地址。窗口地址在同一通信组的所有 rank 间共享。

**参数**：`rankId` — 要查询的 rank ID。

**返回值**：该 rank 的 WindowsIn 地址。

## GetWindowsOutAddr

> `hccl.h:391`

```cpp
__aicore__ inline GM_ADDR GetWindowsOutAddr(uint32_t rankId);
```

**功能**：获取指定 rank 的输出窗口地址。

## GetRankId

> `hccl.h:400`

```cpp
__aicore__ inline uint32_t GetRankId();
```

**功能**：获取当前 rank 的 ID。

**实现**：返回 `hcclContext_->rankId`。

## GetRankDim

> `hccl.h:409`

```cpp
__aicore__ inline uint32_t GetRankDim();
```

**功能**：获取通信组中的 rank 总数。

**实现**：返回 `hcclContext_->rankSize`。

## GetQueueNum

> `hccl.h:411`

```cpp
__aicore__ inline uint16_t GetQueueNum();
```

**功能**：获取当前通信组的队列数。

## InterHcclGroupSync

> `hccl.h:337`

```cpp
__aicore__ inline void InterHcclGroupSync(int8_t srcGroupID, HcclHandle srcHandleID);
```

**功能**：启动一个依赖任务消息，指示当前通信组的后续任务必须在 `srcGroupID` 通信组的 `srcHandleID` 任务执行完成后才能开始。

**参数**：

| 参数 | 说明 |
|------|------|
| `srcGroupID` | 被依赖的通信组 ID |
| `srcHandleID` | 被依赖的任务 Handle ID |

## QueueBarrier

> `hccl.h:344-345`

```cpp
template <ScopeType type = ScopeType::ALL>
__aicore__ inline void QueueBarrier(uint16_t queueID);
```

**功能**：数据同步屏障。

**模板参数**：

| 值 | 说明 |
|----|------|
| `ScopeType::ALL` | 所有范围同步（默认） |
| `ScopeType::QUEUE` | 队列范围同步 |
| `ScopeType::BLOCK` | 块范围同步 |

**参数**：`queueID` — 屏障的队列 ID。

## Iterate

> `hccl.h:356-357`

```cpp
template <bool sync = true>
__aicore__ inline int32_t Iterate(
    HcclHandle handleId, uint16_t* seqSlices, uint16_t seqSliceLen);
```

**功能**：获取某个细粒度通信生成的数据块序列。

**参数**：

| 参数 | 方向 | 说明 |
|------|------|------|
| `handleId` | in | 通信任务标识符 |
| `seqSlices` | out | 存储序列的数组（不能为空） |
| `seqSliceLen` | in | 序列数组长度（不能为 0） |

**模板参数**：`sync` — true 表示等待通信结果。

**返回值**：序列实际长度，-1 表示失败。
