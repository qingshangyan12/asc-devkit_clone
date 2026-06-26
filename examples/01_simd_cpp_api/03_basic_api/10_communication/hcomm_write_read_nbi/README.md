# hcomm_write_read_nbi样例

## 概述
本样例展示如何在Ascend C kernel中使用`Hcomm`类的`WriteNbi`和`ReadNbi`接口实现NPU间点对点（P2P）通信，并配套完整的通信域创建流程。卡0通过`WriteNbi`写入数据，卡1通过`ReadNbi`读出数据并校验结果一致。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍
```
├── hcomm_write_read_nbi
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── README.md                   // 样例说明文档
│   └── hcomm_write_read_nbi.asc    // Ascend C样例实现（Kernel + Host）
```

## 样例描述
- 样例功能：  
  在两张卡上分工演示两个核心P2P通信接口：卡0（writer）通过`WriteNbi`将本地数据写入卡1显存，卡1（reader）通过`ReadNbi`从卡0显存读出数据并校验一致性。与集合通信（如`AlltoAll`）不同，P2P通信直接向目标NPU发起RDMA操作，无需等待所有rank同步，延迟更低，适用于MoE Dispatch/Combine、Pipeline并行等场景。

  | 接口 | 数据流向 | 语义 |
  |------|---------|------|
  | `WriteNbi` | 本地→远端 | 将本地GM数据写入远端NPU指定地址 |
  | `ReadNbi` | 远端→本地 | 从远端NPU指定地址读取数据到本地GM |

- 样例实现：  
  - Host侧通信域准备  
    950上通信域需多进程方式创建：每个进程对应一个rank，通过TCP交换rootInfo后各自调用`HcclCommInitRootInfo`。关键步骤：
    1. rank 0调用`HcclGetRootInfo`获取root信息，通过TCP发送给rank 1。
    2. 各rank各自调用`HcclCommInitRootInfo`创建通信域。
    3. `HcclCommMemReg`向通信域注册本卡的通信buffer，channel创建时将该内存信息交换给对端。
    4. `HcclRankGraphGetLayers`+`HcclRankGraphGetLinks`获取本rank到对端的链路endpoint信息。
    5. `HcclChannelAcquire`创建到对端的P2P通道，指定远端rank、协议（`COMM_PROTOCOL_UBC_CTP`对应URMA）、endpoint和待交换内存句柄。
    6. `HcclChannelGetRemoteMems`获取对端注册的内存地址，作为kernel侧`WriteNbi`/`ReadNbi`的远端地址。
    7. 将`ChannelHandle`和buffer地址封装到`CommContext`下发到各卡GM。

    ```cpp
    // 多进程：rank 0生成rootInfo通过TCP发送，rank 1接收
    HcclGetRootInfo(&rootInfo);  // rank 0
    // 各rank各自创建通信域
    HcclCommInitRootInfo(nranks, &rootInfo, rank, &comm);
    // 注册通信内存
    HcclCommMemReg(comm, "shareBuf", &regMem, &memHandle);
    // 获取链路endpoint
    HcclRankGraphGetLinks(comm, layerId, rank, peerRank, &links, &linkNum);
    // 创建P2P通道
    HcclChannelAcquire(comm, COMM_ENGINE_AIV, &channelDesc, 1, &channel);
    // 获取对端内存地址
    HcclChannelGetRemoteMems(comm, channel, &memNum, &remoteMems, &memTags);
    ```

  - Kernel侧执行  
    通信流程分为三步：`Init()`→`WriteNbi()`/`ReadNbi()`→`Drain()`。
    - `Init`：分配UB工作空间（>=512字节），存放WQE/CQE等内部状态。
    - `WriteNbi`/`ReadNbi`：将通信任务入队到SQ。默认`commit=true`入队后立即doorbell；如需批量优化可改用`commit=false`多次入队后统一`Commit()`。
    - `Drain`：轮询CQ等待通信任务完成，返回后数据可见性才有保证。

    ```cpp
    // 卡0(writer)：本地→对端
    hcomm_.WriteNbi(channel, remoteBuf, localSrc, DATA_SIZE);
    hcomm_.Drain(channel);
    // 卡1(reader)：对端→本地
    hcomm_.ReadNbi(channel, localDst, remoteBuf, DATA_SIZE);
    hcomm_.Drain(channel);
    ```

  - 调用实现  
    多进程分阶段执行：rank0（writer）先执行`WriteNbi`，完成后通过TCP通知rank1；rank1（reader）等待通知后执行`ReadNbi`并校验。使用内核调用符`<<<>>>`调用核函数。

- 校验机制：  
  - writer在buffer中填充基于`rankId`的确定性pattern（`(rankId + 1) & 0xFF`），通信后可区分数据来源。
  - reader校验本地buffer两段数据：前半段（对端`WriteNbi`写入）和后半段（本rank `ReadNbi`读回）是否都为对端pattern，验证一张卡写入、另一张卡读出结果一致。
  - Kernel将校验结果写入`CommContext::testResult`，Host侧通过`aclrtMemcpy`回读，为0时打印`test pass!`。

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
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # 编译工程
  ```

  本样例为多进程方式，直接执行即可自动fork两个子进程（rank0=writer, rank1=reader）：
  ```bash
  ./demo
  ```

  也可以手动指定参数单独运行某个rank：
  ```bash
  # 终端1：启动writer（卡0）
  ./demo 0 2 tcp://127.0.0.1:29621

  # 终端2：启动reader（卡1）
  ./demo 1 2 tcp://127.0.0.1:29621
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构：dav-3510对应Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明通信成功（一张卡写入、另一张卡读出、结果一致）。
  ```bash
  rank 0 test pass!
  rank 1 test pass!
  test pass!
  ```

  单卡环境仅支持编译验证，运行时需至少2张NPU。
