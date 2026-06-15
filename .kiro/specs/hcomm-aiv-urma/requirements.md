# Requirements Document

## Introduction

在Ascend C高级API体系中，通信模块（Hcomm）负责实现多芯片/多核之间的远程数据搬运。`HcommImpl<COMM_PROTOCOL_UBC_CTP>` 是基于URMA（Unified Remote Memory Access）协议的通信实现，采用AIV引擎直接驱动硬件SQ/CQ队列完成远程读写操作，无需CPU/TS中转，适用于V310平台。

### 典型场景

| 场景 | 描述 |
|------|------|
| 远程写（Remote Write） | 将本地Buffer数据通过URMA协议写入远端节点的注册内存区域 |
| 远程读（Remote Read）  | 从远端节点的注册内存区域读取数据到本地Buffer |
| 批量提交（Batch Commit） | 多次WriteNbi/ReadNbi积累请求后统一Commit，减少doorbell开销 |
| 同步等待（Drain） | 等待所有已提交的URMA请求全部完成，通过轮询CQ实现 |
| SQ溢出保护 | 当SQ队列接近满时自动PollCq腾出空间，保证后续请求可继续发送 |

### 核心设计目标

- AIV引擎直接操作硬件队列，零拷贝路径
- 支持强序（odr=5）和fence屏障保证数据一致性
- 通过token机制保护远端内存访问安全
- 提供同步/异步两种提交模式（commit=true/false模板参数）

## Glossary

- **Hcomm_AIV_URMA**: 基于URMA协议、由AIV引擎直驱的通信模块实现类 `HcommImpl<COMM_PROTOCOL_UBC_CTP>`
- **ChannelHandle**: 通信通道句柄，指向 `ChannelEntity` 结构体的指针
- **SQ（Send Queue）**: 发送队列，存放待发送的WQE（Work Queue Entry）
- **CQ（Completion Queue）**: 完成队列，硬件写入已完成请求的CQE（Completion Queue Entry）
- **sqPI（SQ Producer Index）**: SQ生产者索引（头指针），由软件更新
- **sqCI（SQ Consumer Index）**: SQ消费者索引（尾指针），由硬件更新
- **cqCI（CQ Consumer Index）**: CQ消费者索引（尾指针），由软件消费后更新
- **Doorbell**: 门铃寄存器，软件写入通知硬件有新请求
- **WQE**: Work Queue Entry，描述一次RDMA操作的完整请求
- **SQE**: Send Queue Entry，WQE中的请求描述部分
- **SGE**: Scatter/Gather Entry，描述本地数据地址和长度
- **CQE**: Completion Queue Entry，硬件填充的完成通知
- **RegedBufferEntity**: 已注册的远端/本地内存Buffer描述
- **UrmaWqeEntry**: WQE配置参数（odr、fence、cqe等）
- **UB（Unified Buffer）**: 片上统一缓冲区，用作工作区
- **PollCq**: 轮询CQ等待完成的内部操作

## Requirements

### Requirement 1: Init初始化

**User Story:** 作为Kernel开发者，我需要初始化Hcomm实例工作区，以便后续执行URMA通信操作。

#### 接口描述

##### 函数签名

```cpp
__aicore__ inline int32_t Init(__ubuf__ uint8_t* buff, uint32_t len);

template <typename T>
__aicore__ inline int32_t Init(const LocalTensor<T>& buff, uint32_t len);
```

##### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| buff | `__ubuf__ uint8_t*` 或 `LocalTensor<T>` | UB工作缓冲区起始地址 |
| len  | uint32_t | 缓冲区长度（字节），最小512字节 |

#### Acceptance Criteria

1. WHEN Init is called with len >= 512 and a valid buffer, THE Hcomm_AIV_URMA SHALL partition the buffer into wqeItem_(128B), cqeItem_(64B), sqPI_(32B), sqCI_(32B), cqCI_(32B) regions and return HCOMM_SUCCESS(0)
2. IF Init is called with len < 512, THEN THE Hcomm_AIV_URMA SHALL return HCOMM_FAILED(-1) without modifying internal state
3. WHEN Init is called with LocalTensor overload and len <= buff.GetSize() and len >= 512, THE Hcomm_AIV_URMA SHALL directly use the LocalTensor's address and partition layout, and return HCOMM_SUCCESS(0)
4. IF Init LocalTensor overload is called with len > buff.GetSize(), THEN THE Hcomm_AIV_URMA SHALL return HCOMM_FAILED(-1)

---

### Requirement 2: WriteNbi远程写

**User Story:** 作为Kernel开发者，我需要将本地数据远程写入远端节点的注册内存，以便实现跨芯片数据传输。

#### 接口描述

##### 函数签名

```cpp
template <bool commit = true, pipe_t commitPipe = PIPE_S, pipe_t reqPipe = PIPE_MTE3,
    auto const &config = URMA_DEFAULT_CFG>
__aicore__ inline int32_t WriteNbi(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len);
```

##### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| channel | ChannelHandle | 通信通道句柄 |
| dst | GM_ADDR | 远端目的地址（必须在远端注册Buffer范围内） |
| src | GM_ADDR | 本地源数据地址 |
| len | uint64_t | 数据长度（字节） |
| commit | bool（模板） | true=自动提交doorbell，false=仅填充SQ不提交 |
| config | UrmaWqeEntry（模板） | WQE配置，默认odr=5, fence=1, cqe=1 |

#### Acceptance Criteria

1. WHEN WriteNbi is called with dst within remote buffer range, THE Hcomm_AIV_URMA SHALL fill SQE with opcode=WRITE and SGE with src address and len, copy WQE to SQ, advance sqPI by 1, and return HCOMM_SUCCESS(0)
2. IF dst is outside all remote buffer ranges (addr+len exceeds boundary), THEN THE Hcomm_AIV_URMA SHALL return HCOMM_FAILED(-1) without modifying SQ
3. WHEN commit template parameter is true, THE Hcomm_AIV_URMA SHALL automatically call Commit after filling SQ
4. WHEN commit template parameter is false, THE Hcomm_AIV_URMA SHALL only fill SQ and update sqPI without writing doorbell
5. WHEN SQ is near full (head within 10 entries of tail), THE Hcomm_AIV_URMA SHALL call PollCq to drain completed entries before proceeding
6. THE Hcomm_AIV_URMA SHALL advance sqPI by 1 after a successful WriteNbi call

---

### Requirement 3: ReadNbi远程读

**User Story:** 作为Kernel开发者，我需要从远端节点的注册内存读取数据到本地，以便获取远程计算结果。

#### 接口描述

##### 函数签名

```cpp
template <bool commit = true, pipe_t commitPipe = PIPE_S, pipe_t reqPipe = PIPE_MTE3,
    auto const &config = URMA_DEFAULT_CFG>
__aicore__ inline int32_t ReadNbi(ChannelHandle channel, GM_ADDR dst, GM_ADDR src, uint64_t len);
```

##### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| channel | ChannelHandle | 通信通道句柄 |
| dst | GM_ADDR | 本地目的地址（必须在本地注册Buffer范围内） |
| src | GM_ADDR | 远端源数据地址（必须在远端注册Buffer范围内） |
| len | uint64_t | 数据长度（字节） |

#### Acceptance Criteria

1. WHEN ReadNbi is called with src within remote buffer range, THE Hcomm_AIV_URMA SHALL fill SQE with opcode=READ, SGE with dst(local) address and len, copy WQE to SQ, advance sqPI by 1, and return HCOMM_SUCCESS(0)
2. IF src is outside all remote buffer ranges, THEN THE Hcomm_AIV_URMA SHALL return HCOMM_FAILED(-1) without modifying SQ
3. WHEN commit=true (default), THE Hcomm_AIV_URMA SHALL automatically call Commit after filling SQ
4. THE Hcomm_AIV_URMA SHALL advance sqPI by 1 after a successful ReadNbi call

---

### Requirement 4: Commit提交

**User Story:** 作为Kernel开发者，我需要通知硬件处理已填充的SQ请求，以便触发实际的远程数据传输。

#### 接口描述

##### 函数签名

```cpp
template <pipe_t pipe = PIPE_S>
__aicore__ inline int32_t Commit(ChannelHandle channel);
```

##### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| channel | ChannelHandle | 通信通道句柄 |

#### Acceptance Criteria

1. WHEN Commit is called, THE Hcomm_AIV_URMA SHALL read the current sqPI from the SQ head pointer address and write sqPI value to the SQ doorbell register via st_dev instruction
2. THE Hcomm_AIV_URMA SHALL return HCOMM_SUCCESS(0) after writing doorbell
3. WHEN multiple WriteNbi/ReadNbi calls are issued with commit=false followed by a single Commit, THE Hcomm_AIV_URMA SHALL notify hardware of all pending requests in one doorbell write

---

### Requirement 5: Drain同步等待

**User Story:** 作为Kernel开发者，我需要等待所有已提交请求全部完成，以便安全地使用远程读取的数据或确认远程写入已生效。

#### 接口描述

##### 函数签名

```cpp
template <pipe_t pipe = PIPE_MTE3>
__aicore__ inline int32_t Drain(ChannelHandle channel);
```

##### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| channel | ChannelHandle | 通信通道句柄 |

#### Acceptance Criteria

1. WHEN Drain is called and all CQEs complete with status=0 and substatus=0, THE Hcomm_AIV_URMA SHALL update cqCI, ring CQ doorbell, update sqCI, and return HCOMM_SUCCESS(0)
2. IF PollCq times out (CQE owner bit does not flip within 1000000 retries), THEN THE Hcomm_AIV_URMA SHALL return HCOMM_FAILED(-1)
3. IF any CQE has non-zero status, THEN THE Hcomm_AIV_URMA SHALL return HCOMM_FAILED(-1) with combined error code (status<<8 | substatus)

---

### Requirement 6: SQ溢出保护

**User Story:** 作为系统设计者，我需要防止SQ队列溢出，以便保证通信可靠性。

#### Acceptance Criteria

1. WHILE sqPI approaches sqCI within POLL_CQ_THRESHOLD(10) entries, THE Hcomm_AIV_URMA SHALL invoke PollCq to drain up to 100 completed entries before posting new WQE

---

### Requirement 7: Buffer大小约束

**User Story:** 作为系统设计者，我需要确保Init缓冲区满足最小容量要求，以便内部数据结构能够正确分配。

#### Acceptance Criteria

1. THE Hcomm_AIV_URMA SHALL require Init buffer length of at least 512 bytes (HCOMM_URMA_TMP_BUF_SIZE)
2. THE Hcomm_AIV_URMA SHALL use 128 bytes for wqeItem (32 x uint32_t), 64 bytes for cqeItem (16 x uint32_t), and remaining for sqPI, sqCI, cqCI index storage

---

### Requirement 8: 地址对齐约束

**User Story:** 作为系统设计者，我需要确保UB buffer地址对齐到32字节，以便满足硬件DMA传输要求。

#### Acceptance Criteria

1. WHEN raw pointer Init is called, THE Hcomm_AIV_URMA SHALL align the buffer address to 32-byte boundary before partitioning

---

### Requirement 9: 远端Buffer注册约束

**User Story:** 作为Kernel开发者，我需要确保远程操作的目标地址在预注册范围内，以便硬件token保护机制能够验证访问权限。

#### Acceptance Criteria

1. THE Hcomm_AIV_URMA SHALL verify that remote address falls within a registered RegedBufferEntity (addr <= targetAddr < addr+size, and offset+len <= size)
2. IF remote buffer lookup fails, THEN THE Hcomm_AIV_URMA SHALL return HCOMM_FAILED(-1) and log KERNEL_ERROR

---

### Requirement 10: WQE配置约束

**User Story:** 作为系统设计者，我需要默认WQE使用强序和fence，以便保证跨节点数据一致性。

#### Acceptance Criteria

1. THE Hcomm_AIV_URMA SHALL use URMA_DEFAULT_CFG (odr=5, fence=1, se=0, cqe=1, inlineEn=0) as default WQE configuration
2. WHERE custom config is provided via template parameter, THE Hcomm_AIV_URMA SHALL use the custom config values in SQE flag field

---

## 方案概述

### 数据面整体调用逻辑

Hcomm AIV URMA的数据面操作遵循「填充-提交-等待」三阶段模型：

1. **初始化阶段**：用户调用 `Init` 分配UB工作区，划分wqeItem、cqeItem、sqPI、sqCI、cqCI区域
2. **请求阶段**：用户调用 `WriteNbi` / `ReadNbi` 将RDMA请求填入SQ
3. **提交阶段**：通过doorbell通知硬件（自动或手动Commit）
4. **完成阶段**：用户调用 `Drain` 轮询CQ直到所有请求完成

### 代码逻辑调用图

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

### 关键数据流

```
WriteNbi流程:
  localSrc(GM) ──[SGE描述]──► SQE ──[DataCopy]──► SQ ──[Doorbell]──► HW ──► remoteDst(GM)

ReadNbi流程:
  remoteSrc(GM) ──[SQE请求]──► SQ ──[Doorbell]──► HW ──► localDst(GM)
                                                          ──[CQE通知]──► CQ

Drain流程:
  sqPI(期望完成数) ──► PollCq循环 ──► 逐条检查CQE.owner+status ──► 更新cqCI/sqCI
```

---

## 可维可测

### 错误提示场景一览

| 序号 | 错误场景 | 返回值 | KERNEL_LOG级别 | 日志内容 |
|------|----------|--------|---------------|----------|
| 1 | Init buffer长度不足512字节 | HCOMM_FAILED(-1) | - | 无日志，直接返回 |
| 2 | 远端地址不在注册Buffer范围 | HCOMM_FAILED(-1) | KERNEL_ERROR | "HcommFindBufferIdx failed addr=%llu len=%llu bufferNum=%u" |
| 3 | Buffer指针为空 | HCOMM_FAILED(-1) | KERNEL_ERROR | "HcommFindBufferIdx failed with null bufferAddr" |
| 4 | PollCq超时（CQE owner未翻转超1000000次） | 0xFF | KERNEL_ERROR | "Hcomm URMA Poll CQ timeout curTail=%u expectIdx=%u" |
| 5 | CQE status非0（硬件报错） | (status<<8)\|substatus | KERNEL_ERROR | "Hcomm URMA CQE failed status=%u subStatus=%u" |
| 6 | Drain中PollCq失败 | HCOMM_FAILED(-1) | KERNEL_ERROR | "Hcomm URMA Drain by channel failed channel=%lu pollRet=%u" |

### 正常流程日志（KERNEL_INFO）

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

### WQE/CQE Dump能力

THE Hcomm_AIV_URMA SHALL provide HcommUrmaDumpWqeCtx and HcommUrmaDumpCqeCtx functions that log complete WQE/CQE field values at KERNEL_INFO level, enabling offline diagnosis of communication failures.

---

## 测试用例场景说明

### 已有UT用例概览（test_hcomm_urma_950.cpp）

| 用例名称 | 测试场景 | 验证要点 |
|----------|----------|----------|
| Aiv_Urma_Read | ReadNbi默认commit=true | Init成功→ReadNbi返回0→Drain返回0，验证读操作端到端流程 |
| Aiv_Urma_Write | WriteNbi commit=false + 手动Commit | Init成功→WriteNbi(commit=false)返回0→手动Commit返回0→Drain返回0 |
| Aiv_Urma_WriteWithNotify | WriteWithNotifyNbi commit=false | Init→WriteWithNotifyNbi返回0→Commit→Drain成功（本文档不涉及该接口详细描述） |
| Aiv_Urma_WriteWithNotify_WqeBbCnt | WQE BB count验证 | WriteNbi推进sqPI+1, WriteWithNotifyNbi推进sqPI+2, 验证队列指针递增正确 |
| Aiv_Urma_RemoteBufferNotFound | 远端buffer查找失败 | 地址完全越界→返回HCOMM_FAILED; 地址在范围内但len越界→返回HCOMM_FAILED |
| Aiv_Urma_BatchCommitDrain | 批量混合操作 | 多次WriteNbi+WriteWithNotifyNbi(commit=false)→单次Commit→Drain成功 |
| Aiv_Urma_InitLocalTensor | LocalTensor重载Init | LocalTensor Init成功→len不足时Init失败→初始化后可正常WriteNbi+Commit+Drain |

### 测试场景分类

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
