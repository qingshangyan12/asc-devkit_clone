# MatmulReduceScatter CCU 快速路径设计

## 1. 背景与目标

### 1.1 问题

MatmulReduceScatter 融合算子在 CCU KFC 展开模式下，ReduceScatter 执行 5 步数据搬运：

| 步骤 | 操作 | 源 → 目的 |
|------|------|-----------|
| 1 | Read | remote peer → scratchMem[peer] |
| 2 | LocalCopy | myInput → ccuBuf[0] |
| 3 | LocalCopy | scratchMem[peer] → ccuBuf[1] |
| 4 | LocalReduce | ccuBuf[0] := ccuBuf[0] + ccuBuf[1] |
| 5 | LocalCopy | ccuBuf[0] → output |

### 1.2 优化思路

2-rank SUM 场景下，加法可交换（A+B == B+A）。若 matmul 直接将计算结果写入 output buffer，ReduceScatter 可跳过所有中间搬运，用 `ccu::ReadReduce` 一步完成读+归约：

| 步骤 | 操作 | 源 → 目的 |
|------|------|-----------|
| 1 | ReadReduce | output := output + remote (融合读+归约) |

### 1.3 目标场景

- rankSize = 2
- reduceOp = SUM
- matmul 输出已写入 output buffer（sendBuf == recvBuf）

## 2. 数据流对比

### 2.1 当前流程

```
buffer = [slice_0 | slice_1]   (matmul 写入完整张量)
     rank0           rank1

rank0:
  Read(peer1, scratch[1])                    → 网络读到 scratch
  LocalCopy(myInput[0], ccuBuf[0])           → 本地数据搬入 buffer
  LocalCopy(scratch[1], ccuBuf[1])           → 远端数据搬入 buffer
  LocalReduce(ccuBuf[0,1])                   → 归约
  LocalCopy(ccuBuf[0], output)               → 结果搬出
```

### 2.2 快速路径

```
buffer = [slice_0 | slice_1]   (matmul 已写入)
     rank0           rank1
     output          output
     +local          +local

rank0: ReadReduce(output[0], peer_output[0])  → output[0] = A_0 + B_0
rank1: ReadReduce(output[1], peer_output[1])  → output[1] = A_1 + B_1
```

关键改动：`currentRankSliceOutputOffset = currentRankSliceInputOffset`（而非硬编码 0）。

## 3. 设计方案（仅改 kernel .cc，不改框架）

### 3.1 核心原则

**不修改任何框架层代码**（Hccl API、struct、prepare、KFC server、kernel header 全部不动）。

仅修改 1 个文件：`ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc`。

### 3.2 自动检测机制

利用已有参数 `inputAddr`（xnData_[1] = sendBuf + offset）和 `outputAddr`（xnData_[2] = recvBuf + offset）：
- 融合算子调用 `ReduceScatter(workspace, workspace, ...)` 时，sendBuf == recvBuf
- prepare 加相同 offset，所以 `inputAddr == outputAddr`
- kernel 中用 `CCU_IF(inputAddr == outputAddr)` 检测，无需新参数

```cpp
// LoadArgs 中：
ctx.currentRankSliceOutputOffset = 0;  // 默认值
CCU_IF(inputAddr == outputAddr)
{
    ctx.currentRankSliceOutputOffset = currentRankSliceInputOffset;
}
```

### 3.3 分支模式（flag 互斥）

`CCU_IF` 是宏展开的 for 循环，不能在内部 `return`（会跳过 `CcuFlushPendingIfs` 清理）。
使用 flag 互斥模式：

```cpp
ccu::Variable useFast = 0;
if (ctx.rankSize == 2) {           // 普通 C++ if，rankSize 是运行时常量
    CCU_IF(ctx.input[ctx.rankId] == ctx.output)
    {
        useFast = 1;
    }
}

CCU_IF(useFast != 0)
{
    CCU_CHK_RET(DoReduceScatterReadReduceFast(ctx));
}

CCU_IF(useFast == 0)
{
    // 现有正常路径（Read → Wait → ReduceLoopGroup）
}
```

- `rankSize != 2`：外层 `if` 不进入，`useFast` 保持 0，走正常路径
- `rankSize == 2` 但 `inputAddr != outputAddr`：`CCU_IF` 条件不满足，`useFast` 保持 0，走正常路径
- `rankSize == 2` 且 `inputAddr == outputAddr`：`useFast = 1`，走快速路径

### 3.4 快速路径执行流程（DoReduceScatterReadReduceFast）

```
1. 计算 output 地址: output + currentRankSliceOutputOffset (= rankId * sliceSize)
2. 计算 peer 远端地址: input[peerId] + currentRankSliceInputOffset
3. 按 4KB 分块:
   totalChunks = sliceSize / 4096
   tailSize = sliceSize % 4096
   hasTail = (tailSize != 0) ? 1 : 0
   totalChunks += hasTail
4. CCU_WHILE(chunkIdx != totalChunks):
   - 最多发 16 个 ReadReduce (RS_UNROLL_NUM)
   - 每个 chunk: ReadReduce(output[offset], peer[offset], chunkLen, SUM)
   - waitCount 递减等待已发事件
```

### 3.5 兼容性

- `sendBuf != recvBuf`（普通 ReduceScatter）→ `inputAddr != outputAddr` → 走现有路径
- `rankSize > 2` → 外层 `if` 不进入 → 走现有路径
- 现有所有测试和调用方不受影响

## 4. 修改文件清单

| # | 文件 | 改动 |
|---|------|------|
| 1 | `ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc` | LoadArgs 加 inputAddr==outputAddr 检测；新增 DoReduceScatterReadReduceFast；DoRepeatReduceScatter 改 flag 互斥模式 |

**仅此 1 个文件。**

## 5. 资源节省

| 资源 | 现有 2-rank | 快速路径 |
|------|-----------|---------|
| scratch | 2 × sliceSize | 0 |
| CcuBuffer 池 | 128 block MS | 0 |
| LocalCopy | 3 次 | 0 |
| LocalReduce | 1 次 (N-buffer) | 0 |
| Read | 1 次 | 0 |
| ReadReduce | 0 | N 次 (N = sliceSize/4KB) |
| 总数据搬运 | 5 次 | 1 次 (ReadReduce 内含) |

## 6. 融合算子使用示例

```cpp
void MatmulReduceScatterFP16BF16::Process() {
    mmv3.Process(workspace, ...);

    // sendBuf == recvBuf == workspace，kernel 自动检测走快速路径
    handles_[i] = hccl_.ReduceScatter<true>(
        workspace, workspace, recvCount, dataType_, HCCL_REDUCE_SUM, ...);
    hccl_.Wait(handles_[i]);
}
```

无需任何额外 API 调用或标志设置。
