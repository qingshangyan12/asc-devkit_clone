# HcommImpl\<COMM_PROTOCOL_UBC_CTP\> AIV直驱URMA 需求&设计文档

## 1. 需求及场景分析

### 1.1 背景说明

在Ascend C高级API体系中，通信模块（Hcomm）负责实现多芯片/多核之间的远程数据搬运。`HcommImpl<COMM_PROTOCOL_UBC_CTP>` 是基于URMA（Unified Remote Memory Access）协议的通信实现，采用AIV引擎直接驱动硬件SQ/CQ队列完成远程读写操作，无需CPU/TS中转，适用于V310平台。

### 1.2 典型场景

| 场景 | 描述 |
|------|------|
| 远程写（Remote Write） | 将本地Buffer数据通过URMA协议写入远端节点的注册内存区域 |
| 远程读（Remote Read）  | 从远端节点的注册内存区域读取数据到本地Buffer |
| 批量提交（Batch Commit） | 多次WriteNbi/ReadNbi积累请求后统一Commit，减少doorbell开销 |
| 同步等待（Drain） | 等待所有已提交的URMA请求全部完成，通过轮询CQ实现 |
| SQ溢出保护 | 当SQ队列接近满时自动PollCq腾出空间，保证后续请求可继续发送 |

### 1.3 核心设计目标

- AIV引擎直接操作硬件队列，零拷贝路径
- 支持强序（odr=5）和fence屏障保证数据一致性
- 通过token机制保护远端内存访问安全
- 提供同步/异步两种提交模式（commit=true/false模板参数）

---

## 2. 接口描述

### 2.1 Init 初始化

#### 函数签名

```cpp
// 重载版本1：原始指针
__aicore__ inline int32_t Init(__ubuf__ uint8_t* buff, uint32_t len);

// 重载版本2：LocalTensor
template <typename T>
__aicore__ inline int32_t Init(const LocalTensor<T>& buff, uint32_t len);
```

#### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| buff | `__ubuf__ uint8_t*` 或 `LocalTensor<T>` | UB工作缓冲区起始地址 |
| len  | uint32_t | 缓冲区长度（字节），最小512字节 |

#### 功能描述

初始化Hcomm实例的UB工作区，将缓冲区划分为以下内部区域：
- wqeItem\_（128B）：WQE组装暂存区
- cqeItem\_（64B）：CQE读取暂存区
- sqPI\_（32B）：SQ生产者索引缓存
- sqCI\_（32B）：SQ消费者索引缓存
- cqCI\_（32B）：CQ消费者索引缓存

原始指针版本会将buffer地址对齐到32字节边界后再进行分区；LocalTensor版本直接使用LocalTensor的地址进行分区。

#### 返回值说明

| 返回值 | 含义 |
|--------|------|
| HCOMM_SUCCESS (0) | 初始化成功 |
| HCOMM_FAILED (-1) | 初始化失败（len < 512，或LocalTensor版本len > buff.GetSize()） |

---

### 2.2 WriteNbi 远程写

#### 函数签名

```cpp
template <bool commit = true, pipe_t commitPipe = PIPE_S, pipe_t reqPipe = PIPE_MTE3,
    auto const &config = URMA_DEFAULT_CFG>
__aicore__ inline int32_t WriteNbi(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len);
```

#### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| channel | ChannelHandle | 通信通道句柄 |
| dst | GM_ADDR | 远端目的地址（必须在远端注册Buffer范围内） |
| src | GM_ADDR | 本地源数据地址 |
| len | uint64_t | 数据长度（字节） |
| commit | bool（模板参数） | true=自动提交doorbell，false=仅填充SQ不提交 |
| commitPipe | pipe_t（模板参数） | 提交使用的pipe，默认PIPE_S |
| reqPipe | pipe_t（模板参数） | 请求使用的pipe，默认PIPE_MTE3 |
| config | UrmaWqeEntry（模板参数） | WQE配置，默认odr=5, fence=1, cqe=1 |

#### 功能描述

将本地数据远程写入远端节点的注册内存。内部流程：
1. 验证远端目的地址合法性（必须在已注册Buffer范围内）
2. 读取当前sqPI（SQ头指针）
3. 检测SQ是否接近满，满时自动PollCq排水
4. 在UB工作区中填充SQE（opcode=WRITE）和SGE（src地址+len）
5. 将WQE从UB拷贝到SQ对应位置
6. sqPI递增1，回写GM
7. 若commit=true，自动调用Commit写doorbell

#### 返回值说明

| 返回值 | 含义 |
|--------|------|
| HCOMM_SUCCESS (0) | 操作成功 |
| HCOMM_FAILED (-1) | 远端地址不在注册Buffer范围内 |

---

### 2.3 ReadNbi 远程读

#### 函数签名

```cpp
template <bool commit = true, pipe_t commitPipe = PIPE_S, pipe_t reqPipe = PIPE_MTE3,
    auto const &config = URMA_DEFAULT_CFG>
__aicore__ inline int32_t ReadNbi(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len);
```

#### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| channel | ChannelHandle | 通信通道句柄 |
| dst | GM_ADDR | 本地目的地址 |
| src | GM_ADDR | 远端源数据地址（必须在远端注册Buffer范围内） |
| len | uint64_t | 数据长度（字节） |
| commit | bool（模板参数） | true=自动提交doorbell，false=仅填充SQ不提交 |
| commitPipe | pipe_t（模板参数） | 提交使用的pipe，默认PIPE_S |
| reqPipe | pipe_t（模板参数） | 请求使用的pipe，默认PIPE_MTE3 |
| config | UrmaWqeEntry（模板参数） | WQE配置，默认odr=5, fence=1, cqe=1 |

#### 功能描述

从远端节点的注册内存读取数据到本地。内部流程：
1. 验证远端源地址合法性（必须在已注册Buffer范围内）
2. 读取当前sqPI
3. 检测SQ是否接近满，满时自动PollCq排水
4. 在UB工作区中填充SQE（opcode=READ）和SGE（dst本地地址+len）
5. 将WQE从UB拷贝到SQ对应位置
6. sqPI递增1，回写GM
7. 若commit=true（默认），自动调用Commit写doorbell

#### 返回值说明

| 返回值 | 含义 |
|--------|------|
| HCOMM_SUCCESS (0) | 操作成功 |
| HCOMM_FAILED (-1) | 远端地址不在注册Buffer范围内 |

---

### 2.4 Commit 提交

#### 函数签名

```cpp
template <pipe_t pipe = PIPE_S>
__aicore__ inline int32_t Commit(ChannelHandle channel);
```

#### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| channel | ChannelHandle | 通信通道句柄 |
| pipe | pipe_t（模板参数） | 使用的pipe，默认PIPE_S |

#### 功能描述

通知硬件处理已填充到SQ中的所有待发送请求。内部流程：
1. 读取当前sqPI（SQ头指针）值
2. 通过 `st_dev` 指令将sqPI写入SQ的doorbell寄存器

当多次WriteNbi/ReadNbi以commit=false方式调用后，可通过一次Commit统一通知硬件处理所有积累的请求，减少doorbell写入次数，提高批量操作效率。

#### 返回值说明

| 返回值 | 含义 |
|--------|------|
| HCOMM_SUCCESS (0) | 提交成功 |

---

### 2.5 Drain 同步等待

#### 函数签名

```cpp
template <pipe_t pipe = PIPE_MTE3>
__aicore__ inline int32_t Drain(ChannelHandle channel);
```

#### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| channel | ChannelHandle | 通信通道句柄 |
| pipe | pipe_t（模板参数） | 使用的pipe，默认PIPE_MTE3 |

#### 功能描述

等待所有已提交请求全部完成。内部流程：
1. 读取当前sqPI作为期望完成数
2. 调用PollCq轮询CQ，逐条检查CQE的owner位翻转和status
3. 所有CQE完成后：更新cqCI、写CQ doorbell、更新sqCI

#### 返回值说明

| 返回值 | 含义 |
|--------|------|
| HCOMM_SUCCESS (0) | 所有请求已完成，CQE status和substatus均为0 |
| HCOMM_FAILED (-1) | PollCq超时（CQE owner位未在1000000次重试内翻转），或CQE status非零（硬件报错） |

---

## 3. 约束条件

### 3.1 Buffer大小约束

- Init缓冲区长度必须 ≥ 512字节（HCOMM_URMA_TMP_BUF_SIZE）
- 内部分区使用：wqeItem\_ 128字节（32 × uint32_t）、cqeItem\_ 64字节（16 × uint32_t）、sqPI\_ 32字节、sqCI\_ 32字节、cqCI\_ 32字节

### 3.2 地址对齐约束

- 原始指针Init时，buffer地址会被对齐到32字节边界后再进行分区
- 这是为满足硬件DMA传输对数据地址的对齐要求

### 3.3 远端Buffer注册范围约束

- 远程操作（WriteNbi/ReadNbi）的目标地址必须在已预注册的RegedBufferEntity范围内
- 验证条件：`addr <= targetAddr` 且 `offset + len <= size`（其中offset = targetAddr - addr）
- 若远端buffer查找失败，返回HCOMM_FAILED(-1)并记录KERNEL_ERROR日志

### 3.4 SQ深度溢出保护

- 当sqPI接近sqCI（差值在POLL_CQ_THRESHOLD=10条以内），自动触发PollCq排水
- 单次排水最多处理100条CQE（NUM_CQE_PER_POLL_CQ=100）
- 防止SQ队列溢出导致请求丢失

### 3.5 WQE默认配置约束

- 默认使用URMA_DEFAULT_CFG配置：odr=5（强序）、fence=1（栅栏）、se=0、cqe=1（生成完成通知）、inlineEn=0
- 通过模板参数可自定义WQE配置，自定义值会编码到SQE的flag字段

---

## 4. 方案概述

### 4.1 「填充-提交-等待」三阶段模型

Hcomm AIV URMA的数据面操作遵循三阶段模型：

1. **初始化阶段**：用户调用 `Init` 分配UB工作区，划分wqeItem、cqeItem、sqPI、sqCI、cqCI区域
2. **请求阶段**：用户调用 `WriteNbi` / `ReadNbi` 将RDMA请求填入SQ
3. **提交阶段**：通过doorbell通知硬件（自动或手动Commit）
4. **完成阶段**：用户调用 `Drain` 轮询CQ直到所有请求完成

所有操作（Init、WriteNbi、ReadNbi、Commit、Drain）均为数据面操作，由AIV引擎直接执行。

### 4.2 代码逻辑调用图

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        用户调用层                                         │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  ┌──────┐    ┌──────────┐    ┌──────────┐    ┌────────┐    ┌───────┐  │
│  │ Init │    │ WriteNbi │    │ ReadNbi  │    │ Commit │    │ Drain │  │
│  └──┬───┘    └────┬─────┘    └────┬─────┘    └───┬────┘    └───┬───┘  │
│     │              │               │              │             │       │
├─────┼──────────────┼───────────────┼──────────────┼─────────────┼───────┤
│     │         内部实现层            │              │             │       │
│     │              │               │              │             │       │
│     ▼              ▼               ▼              │             │       │
│ ┌────────┐   ┌──────────────────────────┐        │             │       │
│ │分配UB  │   │      PostSend<OpCode>    │        │             │       │
│ │工作区  │   │                          │        │             │       │
│ │wqeItem │   │ 1. HcommFindBufferIdx   │        │             │       │
│ │cqeItem │   │    (查找远端buffer索引)   │        │             │       │
│ │sqPI    │   │         │                │        │             │       │
│ │sqCI    │   │         ▼                │        │             │       │
│ │cqCI    │   │ 2. Gm2Ub(sqPI)          │        │             │       │
│ └────────┘   │    (读取SQ头指针)         │        │             │       │
│              │         │                │        │             │       │
│              │         ▼                │        │             │       │
│              │ 3. PollCqWhenSqOverflow  │        │             │       │
│              │    (SQ满时排水)           │        │             │       │
│              │         │                │        │             │       │
│              │         ▼                │        │             │       │
│              │ 4. HcommUrmaFillSqeCtx  │        │             │       │
│              │    (填充SQE结构)          │        │             │       │
│              │         │                │        │             │       │
│              │         ▼                │        │             │       │
│              │ 5. HcommUrmaFillSgeCtx  │        │             │       │
│              │    (填充SGE结构)          │        │             │       │
│              │         │                │        │             │       │
│              │         ▼                │        │             │       │
│              │ 6. DataCopy(UB→SQ)      │        │             │       │
│              │    (WQE从UB拷贝到SQ)     │        │             │       │
│              │         │                │        │             │       │
│              │         ▼                │        │             │       │
│              │ 7. 更新sqPI              │        │             │       │
│              │    Ub2Gm(sqPI→GM)       │        │             │       │
│              │         │                │        │             │       │
│              │         ▼                │        │             │       │
│              │ 8. if(commit)──────────────────►  │             │       │
│              │         │                │     ┌──▼──────────┐ │       │
│              └─────────┘                │     │ 读取sqPI    │ │       │
│                                         │     │ st_dev(DB)  │ │       │
│                                         │     │ (写doorbell)│ │       │
│                                         │     └─────────────┘ │       │
│                                         │                      │       │
│                                         │              ┌───────▼─────┐ │
│                                         │              │   PollCq    │ │
│                                         │              │             │ │
│                                         │              │ 1.读cqCI    │ │
│                                         │              │ 2.循环等待  │ │
│                                         │              │   CQE owner │ │
│                                         │              │   翻转      │ │
│                                         │              │ 3.检查status│ │
│                                         │              │ 4.更新cqCI  │ │
│                                         │              │ 5.ring CQ DB│ │
│                                         │              │ 6.更新sqCI  │ │
│                                         │              └─────────────┘ │
├─────────────────────────────────────────────────────────────────────────┤
│                        硬件层 (URMA Engine)                              │
│  ┌──────────────────────────────────────────────────────────────────┐  │
│  │  SQ Ring Buffer  ──doorbell──►  URMA HW  ──完成──►  CQ Ring Buffer │ │
│  └──────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────┘
```

### 4.3 关键数据流描述

#### WriteNbi 流程

```
localSrc(GM) ──[SGE描述]──► SQE ──[DataCopy]──► SQ ──[Doorbell]──► HW ──► remoteDst(GM)
```

1. 用户调用WriteNbi，指定远端目的地址dst、本地源地址src和数据长度len
2. 内部验证dst在远端注册Buffer范围内，获取对应的tokenId/tokenValue
3. 在UB中组装SQE（opcode=WRITE，远端地址、token信息）和SGE（本地src地址、len）
4. DataCopy将WQE从UB拷贝到SQ环形缓冲区的sqPI位置
5. sqPI递增1并回写GM
6. 若commit=true，写doorbell通知硬件执行RDMA写操作

#### ReadNbi 流程

```
remoteSrc(GM) ──[SQE请求]──► SQ ──[Doorbell]──► HW ──► localDst(GM)
                                                        ──[CQE通知]──► CQ
```

1. 用户调用ReadNbi，指定本地目的地址dst、远端源地址src和数据长度len
2. 内部验证src在远端注册Buffer范围内，获取对应的tokenId/tokenValue
3. 在UB中组装SQE（opcode=READ，远端地址src）和SGE（本地dst地址、len）
4. DataCopy将WQE从UB拷贝到SQ
5. sqPI递增1并回写GM
6. 若commit=true，写doorbell通知硬件；硬件执行RDMA读后将数据写入本地dst，并在CQ中生成CQE

#### Drain 流程

```
sqPI(期望完成数) ──► PollCq循环 ──► 逐条检查CQE.owner+status ──► 更新cqCI/sqCI
```

1. 读取当前sqPI作为期望完成的条目数
2. 进入PollCq循环，从cqCI开始逐条读取CQE
3. 每条CQE：等待owner位翻转（表示硬件已写入）→ 检查status和substatus
4. 所有CQE处理完成后：更新cqCI → 写CQ doorbell → 更新sqCI
5. 若任何CQE超时或status非零，返回失败

---

## 5. 可维可测

### 5.1 错误提示场景一览

| 序号 | 错误场景 | 返回值 | KERNEL_LOG级别 | 日志内容 |
|------|----------|--------|---------------|----------|
| 1 | Init buffer长度不足512字节 | HCOMM_FAILED(-1) | - | 无日志，直接返回 |
| 2 | 远端地址不在注册Buffer范围 | HCOMM_FAILED(-1) | KERNEL_ERROR | "HcommFindBufferIdx failed addr=%llu len=%llu bufferNum=%u" |
| 3 | Buffer指针为空 | HCOMM_FAILED(-1) | KERNEL_ERROR | "HcommFindBufferIdx failed with null bufferAddr" |
| 4 | PollCq超时（CQE owner未翻转超1000000次） | 0xFF | KERNEL_ERROR | "Hcomm URMA Poll CQ timeout curTail=%u expectIdx=%u" |
| 5 | CQE status非0（硬件报错） | (status\<\<8)\|substatus | KERNEL_ERROR | "Hcomm URMA CQE failed status=%u subStatus=%u" |
| 6 | Drain中PollCq失败 | HCOMM_FAILED(-1) | KERNEL_ERROR | "Hcomm URMA Drain by channel failed channel=%lu pollRet=%u" |

### 5.2 正常流程日志（KERNEL_INFO级别）

| 场景 | 日志内容 |
|------|----------|
| Buffer查找命中 | "HcommFindBufferIdx hit idx=%u addr=%llu len=%llu base=%llu size=%llu" |
| PostSend开始 | "Hcomm URMA PostSend resolved remoteIdx=%d curHead=%u sqDepth=%u" |
| SQ溢出触发PollCq | "Hcomm URMA SQ overflow curHead=%u curTail=%u idx=%u cqDepth=%u" |
| PostSend完成 | "Hcomm URMA PostSend finish new curHead=%u wqeBbCnt=%u" |
| Commit完成 | "Hcomm URMA Commit by channel finish channel=%lu curHead=%u" |
| PollCq进入 | "Hcomm URMA PollCq enter expectIdx=%u curTail=%u cqDepth=%u" |
| PollCq完成 | "Hcomm URMA PollCq finish expectIdx=%u curTail=%u" |
| Drain进入 | "Hcomm URMA Drain by channel enter channel=%lu curHead=%u" |
| Drain完成 | "Hcomm URMA Drain by channel finish channel=%lu curHead=%u" |

### 5.3 WQE/CQE Dump能力

系统提供 `HcommUrmaDumpWqeCtx` 和 `HcommUrmaDumpCqeCtx` 两个诊断函数，在错误路径中自动打印完整的WQE/CQE字段值（KERNEL_INFO级别），支持离线诊断通信故障。

Dump内容包括：
- **WQE Dump**：sqeBbIdx、flag、owner、opcode、tpId、sgeNum、rmtJettyOrSegId、rmtEid、rmtTokenValue、rmtAddr，以及SGE的va和len字段；对于WRITE_WITH_NOTIFY操作还会dump NotifyCtx中的notifyTokenId、notifyTokenValue、notifyAddr、notifyData
- **CQE Dump**：sR、isJetty、owner、inlineEn、opcode、fd、substatus、status、entryIdx、localNum、rmtIdx、tpn、byteCnt、userData、rmtEid、data、inlineData

---

## 6. 测试用例场景说明

### 6.1 已有UT用例概览（test_hcomm_urma_950.cpp）

| 用例名称 | 测试场景 | 验证要点 |
|----------|----------|----------|
| Aiv_Urma_Read | ReadNbi默认commit=true | Init成功→ReadNbi返回0→Drain返回0，验证读操作端到端流程 |
| Aiv_Urma_Write | WriteNbi commit=false + 手动Commit | Init成功→WriteNbi(commit=false)返回0→手动Commit返回0→Drain返回0 |
| Aiv_Urma_WriteWithNotify | WriteWithNotifyNbi commit=false | Init→WriteWithNotifyNbi返回0→Commit→Drain成功（本文档不涉及该接口详细描述） |
| Aiv_Urma_WriteWithNotify_WqeBbCnt | WQE BB count验证 | WriteNbi推进sqPI+1, WriteWithNotifyNbi推进sqPI+2, 验证队列指针递增正确（本文档不涉及WriteWithNotifyNbi接口详细描述） |
| Aiv_Urma_RemoteBufferNotFound | 远端buffer查找失败 | 地址完全越界→返回HCOMM_FAILED; 地址在范围内但len越界→返回HCOMM_FAILED |
| Aiv_Urma_BatchCommitDrain | 批量混合操作 | 多次WriteNbi+WriteWithNotifyNbi(commit=false)→单次Commit→Drain成功 |
| Aiv_Urma_InitLocalTensor | LocalTensor重载Init | LocalTensor Init成功→len不足时Init失败→初始化后可正常WriteNbi+Commit+Drain |

### 6.2 测试场景分类

#### 正向功能验证

1. **单次Read操作**（Aiv_Urma_Read）：验证ReadNbi默认自动Commit，Drain等待完成
2. **单次Write操作**（Aiv_Urma_Write）：验证WriteNbi手动Commit模式的完整流程
3. **批量操作**（Aiv_Urma_BatchCommitDrain）：验证多请求累积后统一Commit+Drain

#### 异常/边界验证

4. **远端地址越界**（Aiv_Urma_RemoteBufferNotFound）：验证两种越界情况的错误返回
5. **Init长度不足**（Aiv_Urma_InitLocalTensor中的子case）：验证len < 512时返回失败

#### 接口兼容性验证

6. **LocalTensor Init**（Aiv_Urma_InitLocalTensor）：验证LocalTensor重载与原始指针Init功能等价

#### 内部状态验证

7. **sqPI递增正确性**（Aiv_Urma_WriteWithNotify_WqeBbCnt）：通过直接读取UB中sqPI值验证WriteNbi推进1、WriteWithNotifyNbi推进2
