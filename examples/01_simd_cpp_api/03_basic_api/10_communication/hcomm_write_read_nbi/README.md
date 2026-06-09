# hcomm_write_read_nbi 样例

## 概述

本样例展示如何在 Ascend C kernel 中使用 `Hcomm` 类的 `WriteNbi` 和 `ReadNbi` 接口实现 NPU 间的点对点（P2P）通信。

样例在一个流程中依次执行 WriteNbi（写入远端）和 ReadNbi（从远端拉取），覆盖两个核心 P2P 通信接口的完整用法。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

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
                                       6. WriteNbi 写入远端 + Drain
                                       7. ReadNbi 拉取远端 + Drain
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

Kernel 侧的通信流程分为三步：

```text
Init() → WriteNbi()/ReadNbi() → Drain()
  │              │                  │
  分配UB工作空间   入队通信任务       阻塞等待完成
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

通信任务入队到 SQ（Send Queue）。默认 `commit=true`，入队后立即触发 doorbell 通知 server 执行。如需批量优化，可改用 `commit=false` 多次入队后统一调用 `Commit()` 提交，减少 doorbell 次数。

```cpp
// 写入远端
hcomm_.WriteNbi(channel, remoteDst, localSrc, len);

// 从远端拉取
hcomm_.ReadNbi(channel, localDst, remoteSrc, len);
```

**Step 3：Drain — 阻塞等待完成**

`Drain` 通过轮询 CQ（Completion Queue）等待所有已提交的通信任务完成。在 `Drain` 返回前，不能保证数据已就绪。

```cpp
hcomm_.Drain(channel);
// Drain 返回后，远端数据已写入完成（Write）或本地数据已就绪（Read）
```

### 3. 本样例的通信流程

本样例在一个流程中先写后读，覆盖两个接口：

```text
本地 GM ──WriteNbi──→ 远端 GM ──Drain──→ 校验写入结果
远端 GM ──ReadNbi──→  本地 GM ──Drain──→ 校验读取结果
```

1. **WriteNbi**：将本地 buffer 填充本 rank 的 pattern 后，写入下一个 rank 的远端 buffer。`Drain` 返回后验证远端 buffer 数据正确。
2. **ReadNbi**：从下一个 rank 的远端 buffer 拉取数据到本地 buffer。`Drain` 返回后验证本地 buffer 包含源 rank 的 pattern。

WriteNbi 和 ReadNbi 使用 buffer 的不同偏移段（offset 0 和 offset DATA_SIZE），避免互相覆盖。

### 4. 关键约束

| 约束项 | 说明 |
|--------|------|
| Init buffer 大小 | >= 512 字节，且起始地址需 32 字节对齐 |
| 地址范围 | `WriteNbi` 的 `dst` 和 `ReadNbi` 的 `src` 必须在 channel 注册的 remote buffer 范围内，否则返回 `HCOMM_FAILED (-1)` |
| 协议选择 | URMA 协议使用 `COMM_PROTOCOL_UBC_CTP`，RoCE 协议使用 `COMM_PROTOCOL_ROCE` |
| Drain 必要性 | 无论 `commit` 取值如何，都必须调用 `Drain` 等待完成，否则无法保证数据可见性 |

### 5. 校验机制

本样例在 Kernel 侧内置了自校验逻辑，无需外部 golden 文件：

1. **填充 pattern**：各 rank 在 buffer 中填充基于 `rankId` 的确定性 pattern（值为 `(rankId + 1) & 0xFF`），使通信后的数据来源可区分。
2. **通信后比对**：
   - **WriteNbi**：`Drain` 返回后，从远端 buffer 读回数据，验证是否与发送方 pattern 一致。
   - **ReadNbi**：`Drain` 返回后，检查本地 buffer 是否包含源 rank 的 pattern。
3. **结果回传**：Kernel 将校验结果写入 `CommContext::testResult`（0=通过，1=失败），Host 侧通过 `aclrtMemcpy` 回读并打印 `"test pass!"` 或 `"test failed!"`。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。
- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510;make -j;    # 编译工程
  ./demo                           # 执行编译生成的可执行程序，执行样例
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU 架构：dav-3510 对应 Atlas A5 训练系列产品和 Atlas A5 推理系列产品 |

- 执行结果

  执行结果如下，说明通信成功。

  ```bash
  test pass!
  ```
