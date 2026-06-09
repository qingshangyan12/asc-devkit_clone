# hcomm_write_read_nbi Sample

## Overview
This sample demonstrates how to use the `WriteNbi` and `ReadNbi` interfaces of the `Hcomm` class in an Ascend C kernel to implement point-to-point (P2P) communication between NPUs, along with the complete communicator creation workflow. Card 0 writes data using `WriteNbi`, and card 1 reads the data using `ReadNbi` and verifies that the results match.

## Supported Products and CANN Software Version

| Product | CANN Software Version |
|---------|-----------------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure
```
├── hcomm_write_read_nbi
│   ├── CMakeLists.txt              // Build project file
│   ├── README.md                   // Sample documentation
│   └── hcomm_write_read_nbi.asc    // Ascend C sample implementation (Kernel + Host)
```

## Sample Description
- Sample function:  
  This sample demonstrates two core P2P communication interfaces across two cards: card 0 (writer) writes local data to card 1 memory using `WriteNbi`, and card 1 (reader) reads data from card 0 memory using `ReadNbi` and verifies consistency. Unlike collective communication (such as `AlltoAll`), P2P communication initiates RDMA operations directly to the target NPU without waiting for all ranks to synchronize, providing lower latency. It is suitable for scenarios such as MoE Dispatch/Combine and pipeline parallelism.

  | Interface | Data Flow | Semantics |
  |-----------|-----------|-----------|
  | `WriteNbi` | Local to remote | Writes local GM data to a specified address on the remote NPU |
  | `ReadNbi` | Remote to local | Reads data from a specified address on the remote NPU to the local GM |

- Sample implementation:  
  - Host-side communicator preparation  
    On the 950, the communicator must be created in multi-process mode: each process corresponds to one rank, and rootInfo is exchanged over TCP before each rank calls `HcclCommInitRootInfoConfig`. Key steps:
    1. Rank 0 calls `HcclGetRootInfo` to obtain the root information and sends it to rank 1 over TCP.
    2. Each rank calls `HcclCommInitRootInfoConfig` to create the communicator.
    3. `HcclCommMemReg` registers the local communication buffer with the communicator. When a channel is created, this memory information is exchanged with the peer.
    4. `HcclRankGraphGetLayers` and `HcclRankGraphGetLinks` obtain the link endpoint information from the local rank to the peer.
    5. `HcclChannelAcquire` creates a P2P channel to the peer, specifying the remote rank, protocol (`COMM_PROTOCOL_UBC_CTP` for URMA), endpoint, notify count, and memory handle to exchange.
    6. `HcclChannelGetRemoteMems` obtains the memory address registered by the peer, which serves as the remote address for the kernel-side `WriteNbi`/`ReadNbi`.
    7. The `ChannelHandle` and buffer address are encapsulated into `CommContext` and written to the GM of each card.

    ```cpp
    // Multi-process: rank 0 generates rootInfo and sends it over TCP; rank 1 receives it
    HcclGetRootInfo(&rootInfo);  // rank 0
    // Each rank creates the communicator
    HcclCommConfigInit(&config);
    config.hcclWorldRankID = rank;
    config.hcclOpExpansionMode = HCCL_OP_EXPANSION_CCU_SCHED;
    HcclCommInitRootInfoConfig(nranks, &rootInfo, rank, &config, &comm);
    // Register communication memory
    HcclCommMemReg(comm, "shareBuf", &regMem, &memHandle);
    // Obtain the link endpoint
    HcclRankGraphGetLinks(comm, layerId, rank, peerRank, &links, &linkNum);
    // Create the P2P channel
    channelDesc.notifyNum = 3;
    HcclChannelAcquire(comm, COMM_ENGINE_AIV, &channelDesc, 1, &channel);
    // Obtain the peer memory address
    HcclChannelGetRemoteMems(comm, channel, &memNum, &remoteMems, &memTags);
    ```

  - Kernel-side execution  
    The communication flow consists of three steps: `Init()` -> `WriteNbi()`/`ReadNbi()` -> `Drain()`.
    - `Init`: Allocates a UB workspace (>= 512 bytes) to store internal state such as WQE/CQE.
    - `WriteNbi`/`ReadNbi`: Enqueues communication tasks to the SQ. By default, `commit=true` triggers a doorbell immediately after enqueue. For batch optimization, use `commit=false` to enqueue multiple tasks and then call `Commit()` once.
    - `Drain`: Polls the CQ to wait for communication tasks to complete. Data visibility is guaranteed only after this call returns.

    ```cpp
    // Card 0 (writer): local to peer
    hcomm_.WriteNbi(channel, remoteBuf, localSrc, DATA_SIZE);
    hcomm_.Drain(channel);
    // Card 1 (reader): peer to local
    hcomm_.ReadNbi(channel, localDst, remoteBuf, DATA_SIZE);
    hcomm_.Drain(channel);
    ```

  - Invocation implementation  
    The sample executes in multi-process mode in stages: rank 0 (writer) first executes `WriteNbi`, and then notifies rank 1 over TCP upon completion. Rank 1 (reader) waits for the notification, executes `ReadNbi`, and verifies the result. The kernel is invoked using the `<<<>>>` kernel invocation operator.

- Verification mechanism:  
  - The writer fills the buffer with a deterministic pattern based on `rankId` (`(rankId + 1) & 0xFF`), so the data source can be distinguished after communication.
  - The reader verifies two segments of the local buffer: the first half (written by the peer using `WriteNbi`) and the second half (read back by the local rank using `ReadNbi`). Both segments must match the peer pattern, verifying that data written by one card and read by another card are consistent.
  - The kernel writes the verification result to `CommContext::testResult`. The host reads it back using `aclrtMemcpy` and prints `test pass!` when the value is 0.

## Build and Run
Perform the following steps in the sample root directory to build and run the sample.
- Configuring environment variables
  Configure the environment variables according to the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` indicates the CANN installation directory. If no installation directory is specified, the package is installed in `/usr/local/Ascend` by default.

- Running the sample

  Run the following commands in the sample directory.
  ```bash
  mkdir -p build && cd build;                                               # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # Build the project
  ```

  This sample runs in multi-process mode. Running the binary directly forks two child processes (rank 0 = writer, rank 1 = reader):
  ```bash
  ./demo
  ```

  You can also manually specify parameters to run a single rank:
  ```bash
  # Terminal 1: start the writer (card 0)
  ./demo 0 2 tcp://127.0.0.1:29621

  # Terminal 2: start the reader (card 1)
  ./demo 1 2 tcp://127.0.0.1:29621
  ```

- Build options

  | Option | Value | Description |
  |--------|-------|-------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` (default) | NPU architecture: dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution result

  The following output indicates successful communication (one card writes, the other card reads, and the results match):
  ```bash
  rank 0 test pass!
  rank 1 test pass!
  test pass!
  ```

  A single-card environment supports only build verification. At least 2 NPUs are required to run the sample.
