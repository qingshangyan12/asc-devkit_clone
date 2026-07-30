# SyncAll 接口 flagId 使用分析

## 问题描述

已知 `CrossCoreSetFlag` 和 `CrossCoreWaitFlag` 硬件底层提供的 flagId 范围：

- 核间同步模式0、1、2时，支持的取值范围为 **0-15**。
- 针对 Ascend 950PR/Ascend 950DT，核间同步模式4时：
  - AIV0 发起的 flagId 0-15 对应 AIC `CrossCoreWaitFlag` 的 flagId 0-15。
  - AIV1 发起的 flagId 0-15 对应 AIC `CrossCoreWaitFlag` 的 flagId 16-31。
  - AIC 发起的 flagId 0-15 对应 AIV0 `CrossCoreWaitFlag` 的 flagId 0-15。
  - AIC 发起的 flagId 16-31 对应 AIV1 `CrossCoreWaitFlag` 的 flagId 0-15。

`SyncAll` 硬同步接口内部使用了 `CrossCoreSetFlag`/`CrossCoreWaitFlag`。需要分析其实现中具体使用了哪些 flagId。

## 分析结果

### 涉及的源文件

| 文件 | 路径 |
|------|------|
| 接口声明 | `impl/basic_api/kernel_operator_block_sync_intf_impl.h` |
| dav_c220 实现 | `impl/basic_api/dav_c220/kernel_operator_sync_impl.h` |
| dav_3510 实现 | `impl/basic_api/dav_3510/kernel_operator_sync_impl.h` |
| dav_m510 实现 | `impl/basic_api/dav_m510/kernel_operator_sync_impl.h` |

### Flag ID 常量定义

在所有三组架构实现中，flagId 常量定义一致：

| 常量名 | 值 | 含义 |
|--------|-----|------|
| `SYNC_AIC_FLAG` | **11** | AIC 核间同步 flag |
| `SYNC_AIV_FLAG` | **12** | AIV 核间同步 flag |
| `SYNC_AIC_AIV_FLAG` | **13** | AIC↔AIV 核内同步 flag |
| `SYNC_AIV_ONLY_ALL` | **14** | 纯 Vector 全核同步 flag |
| `SYNC_FLAG_ID_MAX` | **16** | 偏移量（仅 dav_3510/dav_m510） |

**结论：SyncAll 接口总共占用了 flagId 范围 [11, 14]，共 4 个 flagId。**

> 这与文档中的约束说明一致：*"SyncAll 硬件同步接口 flagId 占用范围为 [11-14]。"*

---

## 各架构详细实现

### 1. dav_c220（Atlas A2/A3 系列：910b, A3, 310p, 310b 等）

**函数：`SyncAllImpl<isAIVOnly>()`**

#### isAIVOnly = true（纯 Vector 算子全核同步）

```cpp
// impl/basic_api/dav_c220/kernel_operator_sync_impl.h 行 384-388
PipeBarrier<PIPE_ALL>();
if constexpr (isAIVOnly) {
    ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0x0, SYNC_AIV_ONLY_ALL));  // flagId = 14
    wait_flag_dev(SYNC_AIV_ONLY_ALL);                                      // flagId = 14
    return;
}
```

使用的 flagId：**14**

#### isAIVOnly = false（Mix 算子全核同步）

AIC 侧：

```cpp
// impl/basic_api/dav_c220/kernel_operator_sync_impl.h 行 391-396
if ASCEND_IS_AIC {
    wait_flag_dev(SYNC_AIV_FLAG);                                          // flagId = 12（等所有AIV就绪）
    ffts_cross_core_sync(PIPE_FIX, GetffstMsg(0x0, SYNC_AIC_FLAG));       // flagId = 11（所有AIC间同步）
    wait_flag_dev(SYNC_AIC_FLAG);                                          // flagId = 11（等所有AIC就绪）
    ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0x02, SYNC_AIC_AIV_FLAG)); // flagId = 13（通知AIV可以继续）
    return;
}
```

AIV 侧：

```cpp
// impl/basic_api/dav_c220/kernel_operator_sync_impl.h 行 398-402
if ASCEND_IS_AIV {
    ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0x02, SYNC_AIV_FLAG));     // flagId = 12（通知AIC就绪）
    wait_flag_dev(SYNC_AIC_AIV_FLAG);                                      // flagId = 13（等AIC通知）
    return;
}
```

使用的 flagId：**11, 12, 13**

**同步流程**：AIV 通知就绪(flagId=12) → AIC 等待 AIV(12) → AIC 核间同步(11) → AIC 通知 AIV(13) → AIV 等待 AIC(13)

---

### 2. dav_3510（Ascend 950PR/Ascend 950DT）

**函数：`SyncAllImpl<isAIVOnly, triggerPipe, waitPipe>()`**

#### isAIVOnly = true（纯 Vector 算子全核同步）

```cpp
// impl/basic_api/dav_3510/kernel_operator_sync_impl.h 行 138-158
if constexpr (isAIVOnly) {
    if ASCEND_IS_AIV {
        PipeBarrier<triggerPipe>();
        if constexpr (triggerPipe == PIPE_ALL) {
            ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0x0, AscendC::SYNC_AIV_ONLY_ALL));  // flagId = 14
        } else {
            ffts_cross_core_sync(triggerPipe, GetffstMsg(0x0, AscendC::SYNC_AIV_ONLY_ALL)); // flagId = 14
        }
        if constexpr (waitPipe == PIPE_ALL) {
            wait_flag_dev(PIPE_S, AscendC::SYNC_AIV_ONLY_ALL);                               // flagId = 14
        } else {
            wait_flag_dev(waitPipe, AscendC::SYNC_AIV_ONLY_ALL);                              // flagId = 14
        }
    }
    return;
}
```

使用的 flagId：**14**（支持用户通过 `SyncAllConfig` 指定所使用的流水 `triggerPipe` / `waitPipe`）

#### isAIVOnly = false（Mix 算子全核同步）

##### 情况 A：MIX_CORE_AIC_RATION == 1（松耦合模式）

与 dav_c220 实现逻辑完全一致：

```cpp
// impl/basic_api/dav_3510/kernel_operator_sync_impl.h 行 163-175
if ASCEND_IS_AIC {
    wait_flag_dev(PIPE_S, AscendC::SYNC_AIV_FLAG);                                           // flagId = 12
    ffts_cross_core_sync(PIPE_FIX, GetffstMsg(0x0, AscendC::SYNC_AIC_FLAG));                // flagId = 11
    wait_flag_dev(PIPE_S, AscendC::SYNC_AIC_FLAG);                                           // flagId = 11
    ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0x02, AscendC::SYNC_AIC_AIV_FLAG));          // flagId = 13
    return;
}
if ASCEND_IS_AIV {
    ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0x02, AscendC::SYNC_AIV_FLAG));              // flagId = 12
    wait_flag_dev(PIPE_S, AscendC::SYNC_AIC_AIV_FLAG);                                       // flagId = 13
    return;
}
```

使用的 flagId：**11, 12, 13**

##### 情况 B：其他 MIX 配比（紧密耦合模式，需同时处理 AIV0 和 AIV1）

AIC 侧：

```cpp
// impl/basic_api/dav_3510/kernel_operator_sync_impl.h 行 178-185
if ASCEND_IS_AIC {
    wait_intra_block(PIPE_S, AscendC::SYNC_AIV_FLAG);                                        // flagId = 12 (等待 AIV0)
    wait_intra_block(PIPE_S, AscendC::SYNC_AIV_FLAG + AscendC::SYNC_FLAG_ID_MAX);            // flagId = 28 (等待 AIV1)
    ffts_cross_core_sync(PIPE_FIX, GetffstMsg(0x0, AscendC::SYNC_AIC_FLAG));                // flagId = 11 (AIC核间同步)
    wait_flag_dev(PIPE_S, AscendC::SYNC_AIC_FLAG);                                           // flagId = 11 (等所有AIC)
    set_intra_block(PIPE_S, AscendC::SYNC_AIC_AIV_FLAG);                                     // flagId = 13 (通知 AIV0)
    set_intra_block(PIPE_S, AscendC::SYNC_AIC_AIV_FLAG + AscendC::SYNC_FLAG_ID_MAX);         // flagId = 29 (通知 AIV1)
    return;
}
```

AIV 侧：

```cpp
// impl/basic_api/dav_3510/kernel_operator_sync_impl.h 行 187-191
if ASCEND_IS_AIV {
    set_intra_block(PIPE_MTE3, AscendC::SYNC_AIV_FLAG);                                      // flagId = 12 (AIV0/AIV1各自通知)
    wait_intra_block(PIPE_S, AscendC::SYNC_AIC_AIV_FLAG);                                    // flagId = 13 (AIV0/AIV1各自等待)
    return;
}
```

使用的 flagId：**11, 12, 13, 28, 29**

> 注释：在紧密耦合模式下，AIC 需同时等待 AIV0（flagId=12）和 AIV1（flagId=28=12+16）。实际硬件上 AIV1 通过 flagId=12 发起通知时，对应 AIC 视角的 flagId=28。同理 AIC 通知 AIV1 时使用 flagId=29=13+16。

---

### 3. dav_m510

**函数：`SyncAllImpl()`**

```cpp
// impl/basic_api/dav_m510/kernel_operator_sync_impl.h 行 123-128
__aicore__ inline void SyncAllImpl()
{
    PipeBarrier<PIPE_ALL>();
    ffts_cross_core_sync(PIPE_MTE3, GetffstMsg(0x0, SYNC_AIV_ONLY_ALL));  // flagId = 14
    wait_flag_dev(PIPE_S, SYNC_AIV_ONLY_ALL);                              // flagId = 14
    return;
}
```

使用的 flagId：**14**

> dav_m510 仅支持 AIV Only 的硬同步，不支持 Mix 模式的硬同步。

---

## 汇总

| 芯片架构 | isAIVOnly=true | isAIVOnly=false（松耦合） | isAIVOnly=false（紧密耦合） |
|----------|:-------------:|:----------------------:|:--------------------------:|
| dav_c220 | **14** | 11, 12, 13 | — |
| dav_3510 | **14** | 11, 12, 13 | 11, 12, 13, 28, 29 |
| dav_m510 | **14** | — | — |

**所有 flagId 汇总：{11, 12, 13, 14}**（紧密耦合模式下额外使用 {28, 29}，分别对应 AIV1 视角下的 flagId=12 和 flagId=13）

### SyncAll 同步流程示意（Mix 模式）

```
AIV: set flag 12 ─────────────────────────────────────────────┐
                                                               ├→ wait flag 13
AIC:                                wait flag 12               │
      AIC间同步: set flag 11 → wait flag 11 → set flag 13 ────┘
```

### 注意事项

- SyncAll 接口已占用 flagId [11-14]，开发者若在同一 kernel 中同时使用 `CrossCoreSetFlag` / `CrossCoreWaitFlag`，应避免使用此范围内的 flagId，防止冲突。
- 在 dav_3510 紧密耦合模式下，除基础范围外，还可能使用 flagId 28 和 29（对应 AIV1 通道），开发者需注意不要占用。

---

## <span style="color:red;">补充说明：flagId 28、29 的来源与 AIV1 偏移机制</span>

<span style="color:red;">

flagId **28** 和 **29** 仅在 dav_3510（Ascend 950PR/Ascend 950DT）的**紧密耦合模式**（`MIX_CORE_AIC_RATION != 1`）中出现：

```
flagId 28 = SYNC_AIV_FLAG(12)      + SYNC_FLAG_ID_MAX(16)
flagId 29 = SYNC_AIC_AIV_FLAG(13)  + SYNC_FLAG_ID_MAX(16)
```

这正好对应 `CrossCoreSetFlag` 文档中描述的 Ascend 950PR/950DT **模式 4** 的硬件行为：

| 发起方 | 发起 flagId | 接收方 | 接收方感知 flagId |
|--------|:----------:|--------|:-----------------:|
| AIV0 | 0-15 | AIC | 0-15（直接对应） |
| AIV1 | 0-15 | AIC | **16-31**（自动 +16 偏移） |
| AIC | 0-15 | AIV0 | 0-15（直接对应） |
| AIC | 16-31 | AIV1 | **0-15**（自动 -16 偏移） |

因此在紧密耦合模式下，AIC 要同时等待 AIV0 和 AIV1 时就形成了如下调用序列：

```cpp
// AIC 等待 AIV0 就绪
wait_intra_block(PIPE_S, AscendC::SYNC_AIV_FLAG);                   // flagId = 12
// AIC 等待 AIV1 就绪 —— 硬件自动映射 AIV1 的 flagId=12 到 AIC 视角的 12+16=28
wait_intra_block(PIPE_S, AscendC::SYNC_AIV_FLAG + SYNC_FLAG_ID_MAX); // flagId = 28

// ... AIC 核间同步 ...

// AIC 通知 AIV0 可继续
set_intra_block(PIPE_S, AscendC::SYNC_AIC_AIV_FLAG);                 // flagId = 13
// AIC 通知 AIV1 可继续 —— AIC 发 flagId=29，硬件自动映射为 AIV1 视角的 29-16=13
set_intra_block(PIPE_S, AscendC::SYNC_AIC_AIV_FLAG + SYNC_FLAG_ID_MAX); // flagId = 29
```

dav_c220 每个 AI Core 只有 1 个 AIV，不存在这个多层偏移，因此无需使用 flagId 28 和 29。

</span>
