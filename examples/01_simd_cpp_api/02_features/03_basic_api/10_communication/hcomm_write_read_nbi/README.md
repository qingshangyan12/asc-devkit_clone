# hcomm_write_read_nbi 样例

## 概述

本样例展示如何在 Ascend C kernel 中使用 `Hcomm` 类的 `WriteNbi` 和 `ReadNbi` 接口实现 NPU 间的点对点（P2P）通信。

样例覆盖三种典型调用模式：单次写入、批量写入和远程读取，重点说明 `WriteNbi`、`ReadNbi`、`Commit` 和 `Drain` 接口的配合使用方法。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── hcomm_write_read_nbi
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── README.md                   // 样例说明文档
│   └── hcomm_write_read_nbi.asc   // Ascend C 样例实现
```

## 样例描述

### 1. 背景与适用场景

在多 NPU 协同计算场景中（如 MoE Dispatch/Combine、Pipeline 并行、自定义 AlltoAll），算子需要在不同 NPU 之间直接搬运数据。与集合通信（如 `AlltoAll`、`AllGather`）不同，P2P 通信具有以下优势：

- **更低延迟**：直接向目标 NPU 发起 RDMA 操作，无需等待所有 rank 同步。
- **更细粒度控制**：可以精确控制每次搬运的数据量、目标地址和提交时机。
- **支持批量优化**：多个通信任务可以批量入队后统一提交，减少 doorbell 开销。

`WriteNbi` 和 `ReadNbi` 是 Hcomm 提供的两个核心 P2P 通信接口：

| 接口 | 数据流向 | 语义 |
|------|---------|------|
| `WriteNbi` | 本地 → 远端 | 将本地 GM 数据写入远端 NPU 的指定地址 |
| `ReadNbi` | 远端 → 本地 | 从远端 NPU 的指定地址读取数据到本地 GM |

### 2. 整体流程

一次完整的 P2P 通信包含 Host 侧准备和 Kernel 侧执行两个阶段：

```text
Host 侧（C++）                          Kernel 侧（Ascend C）
─────────────                          ─────────────────────
1. HcclCommInitRootInfo()
2. HcclCommMemReg() 注册对称内存
3. HcclChannelAcquire() 获取 Channel
4. 将 ChannelHandle 传入 kernel  ───→  5. Hcomm::Init() 初始化工作空间
                                       6. WriteNbi / ReadNbi 发起通信
                                       7. Commit() 提交任务（批量模式）
                                       8. Drain() 等待完成
```

#### 2.1 Host 侧准备

Host 侧需要完成通信资源的初始化，主要包括以下步骤：

```cpp
// 1. 初始化 HCCL 通信域
HcclComm hcclComm;
HcclCommInitRootInfo(nranks, rootInfo, rank, &hcclComm);

// 2. 注册对称内存（各 rank 分配相同大小的 GM buffer 作为通信窗口）
void *shareBuf;
aclrtMalloc(&shareBuf, bufSize, ACL_MEM_MALLOC_HUGE_ONLY);
CommMem regMem{COMM_MEM_TYPE_DEVICE, shareBuf, bufSize};
HcclMemHandle memHandle;
HcclCommMemReg(hcclComm, "ShareBuf", &regMem, &memHandle);

// 3. 创建通信通道描述，指定远端 rank、协议类型和注册的内存
HcclChannelDesc channelDesc;
HcclChannelDescInit(&channelDesc, 1);
channelDesc.remoteRank = targetRankId;
channelDesc.channelProtocol = link.linkAttr.linkProtocol;
channelDesc.memHandles = &memHandle;
channelDesc.memHandleNum = 1;

// 4. 获取 ChannelHandle，传入 kernel 使用
CommEngine engine = CommEngine::COMM_ENGINE_AIV;
ChannelHandle channel;
HcclChannelAcquire(hcclComm, engine, &channelDesc, 1, &channel);
```

`HcclChannelAcquire` 返回的 `ChannelHandle` 封装了 SQ/CQ 上下文、远端 buffer 地址等通信所需的全部信息。Kernel 通过该 handle 发起通信操作。

#### 2.2 Kernel 侧执行

Kernel 侧的通信流程分为四步：

```text
Init() → WriteNbi()/ReadNbi() → Commit() → Drain()
  │              │                  │          │
  分配UB工作空间   入队通信任务       通知执行    阻塞等待完成
```

**Step 1：Init — 初始化 Hcomm 工作空间**

`Init` 需要一块 >= 512 字节的 UB buffer，用于存放 WQE（Work Queue Entry）、CQE（Completion Queue Entry）、SQ PI（Producer Index）等内部状态。

```cpp
tpipe_->InitBuffer(hcommBuf_, 512);
hcommTensor_ = hcommBuf_.Get<uint8_t>();
__ubuf__ uint8_t *bufPtr = reinterpret_cast<__ubuf__ uint8_t *>(hcommTensor_.GetPhyAddr());
hcomm_.Init(bufPtr, 512);
```

**Step 2：WriteNbi / ReadNbi — 发起通信任务**

通信任务入队到 SQ（Send Queue），不会立即执行。通过 `commit` 模板参数控制是否自动提交：

- `commit=true`（默认）：入队后立即触发 doorbell 通知 server 执行。
- `commit=false`：仅入队，需要后续手动调用 `Commit()` 批量提交。

```cpp
// 单次写入，自动提交
hcomm_.WriteNbi(channel, remoteDst, localSrc, len);

// 批量写入，手动提交
hcomm_.WriteNbi<false>(channel, dst1, src1, len);
hcomm_.WriteNbi<false>(channel, dst2, src2, len);
hcomm_.Commit(channel);
```

**Step 3：Commit — 通知 server 执行（仅 commit=false 时需要）**

`Commit` 通过 doorbell 机制通知通信 server 开始处理已入队的任务。批量模式下，多个任务只需一次 doorbell，显著降低开销。

**Step 4：Drain — 阻塞等待完成**

`Drain` 通过轮询 CQ（Completion Queue）等待所有已提交的通信任务完成。在 `Drain` 返回前，不能保证远端数据已可见。

```cpp
hcomm_.Drain(channel);
// Drain 返回后，远端数据已写入完成（Write）或本地数据已就绪（Read）
```

### 3. 场景说明

#### 场景1：单次 WriteNbi + Drain

最简单的使用模式。将本地 GM 数据写入一个远端 rank，使用默认的 `commit=true` 自动提交。

```text
本地 GM ──WriteNbi(commit=true)──→ 远端 GM
                                      │
                                  Drain() 等待完成
```

适用场景：单次小数据量传输，如状态同步、flag 通知。

代码要点：
- `WriteNbi` 默认 `commit=true`，入队后立即提交。
- 仍需调用 `Drain` 等待完成，确保数据对远端可见。

```cpp
AscendC::ChannelHandle channel = GetChannel(dstRankId);
GM_ADDR localSrc = GetLocalAddr(epRankId_, 0);
GM_ADDR remoteDst = GetRemoteAddr(dstRankId, 0);

// commit=true（默认），入队后立即提交
hcomm_.WriteNbi(channel, remoteDst, localSrc, DATA_SIZE);

// 阻塞等待通信完成
hcomm_.Drain(channel);
```

#### 场景2：批量 WriteNbi + Commit + Drain

向多个远端 rank 发送多块数据，使用 `commit=false` 延迟提交，最后统一 `Commit` + `Drain`。

```text
本地 GM ──WriteNbi<false>──→ SQ（入队，不提交）
本地 GM ──WriteNbi<false>──→ SQ（入队，不提交）
本地 GM ──WriteNbi<false>──→ SQ（入队，不提交）
本地 GM ──WriteNbi<false>──→ SQ（入队，不提交）
                                │
                           Commit() 一次 doorbell 提交全部
                                │
                           Drain() 等待全部完成
```

适用场景：MoE Dispatch 中向多个 expert rank 发送 token 数据。

代码要点：
- `WriteNbi<false>` 仅入队不提交，避免每个任务都触发 doorbell。
- 所有任务入队后，一次 `Commit` 批量提交，减少通信开销。
- `Drain` 等待该 channel 上所有任务完成。

```cpp
for (uint32_t dstRankId = 0; dstRankId < epWorldSize_; dstRankId++) {
    if (dstRankId == epRankId_) continue;

    AscendC::ChannelHandle channel = GetChannel(dstRankId);

    // commit=false：仅入队，不触发 doorbell
    for (uint32_t i = 0; i < BATCH_NUM; i++) {
        hcomm_.WriteNbi<false>(channel, dst + i * DATA_SIZE,
                               src + i * DATA_SIZE, DATA_SIZE);
    }

    // 一次 doorbell 提交该 channel 上的全部任务
    hcomm_.Commit(channel);
    // 等待完成
    hcomm_.Drain(channel);
}
```

#### 场景3：ReadNbi + Drain

从远端 rank 拉取数据到本地 GM。

```text
远端 GM ──ReadNbi──→ 本地 GM
                        │
                   Drain() 等待完成
```

适用场景：从远端 rank 获取计算结果或参数数据。

代码要点：
- `ReadNbi` 的参数语义与 `WriteNbi` 相同：`dst` 是本地地址，`src` 是远端地址。
- `ReadNbi` 默认 `commit=true`，自动提交。
- `Drain` 返回后，本地 `dst` 中的数据已就绪。

```cpp
AscendC::ChannelHandle channel = GetChannel(srcRankId);
GM_ADDR localDst = GetLocalAddr(epRankId_, 0);
GM_ADDR remoteSrc = GetRemoteAddr(srcRankId, 0);

// dst=本地地址, src=远端地址
hcomm_.ReadNbi(channel, localDst, remoteSrc, DATA_SIZE);

// 阻塞等待，Drain 返回后 localDst 数据已就绪
hcomm_.Drain(channel);
```

### 4. 关键约束

| 约束项 | 说明 |
|--------|------|
| Init buffer 大小 | >= 512 字节，且起始地址需 32 字节对齐 |
| 地址范围 | `WriteNbi` 的 `dst` 和 `ReadNbi` 的 `src` 必须在 channel 注册的 remote buffer 范围内，否则返回 `HCOMM_FAILED (-1)` |
| 协议选择 | URMA 协议使用 `COMM_PROTOCOL_UBC_CTP`，RoCE 协议使用 `COMM_PROTOCOL_ROCE` |
| Drain 必要性 | 无论 `commit` 取值如何，都必须调用 `Drain` 等待完成，否则无法保证数据可见性 |
