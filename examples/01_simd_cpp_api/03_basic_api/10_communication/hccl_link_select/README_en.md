# HCCL Link Selection Example

## Purpose

This example selects a `COMM_PROTOCOL_UB_MEM` link through HCCL Resource APIs, creates two channels with
`pathMode=2` and `pathMode=3` in one communication domain, and copies separate data shards concurrently on two
streams.

Each rank fills its local HCCL Buffer with its rank ID, reads 4 KB from every peer, and verifies the data:

```text
Peer HCCL Buffer -> UB_MEM link -> Ascend C DataCopy -> local verification buffer -> host verification
```

The example uses one process per device. `run.sh` starts local rank processes, and rank 0 distributes
`HcclRootInfo`.

## Prerequisites

- Ascend 950PR or Ascend 950DT with the `dav-3510` NPU architecture.
- A CANN development toolkit installation with HCCL headers and libraries that provide the HCCL Resource APIs
  and the `ubMemAttr.pathMode` field.
- For the default run, devices 0 and 1 must have a UB_MEM link between them.

## Build and Run

Set up the CANN environment in this directory:

```bash
source ${install_path}/cann/set_env.sh
```

Build and run the default two-rank dual-channel validation:

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
bash run.sh
```

The script starts two processes on devices 0 and 1. Each process initializes one communication domain, acquires
both channels in one call, and splits the 4 KB validation payload into two concurrent 2 KB copies.

## Expected Output

Log order can vary because the rank processes run concurrently. Every rank should validate every peer:

```text
rank 0/2: device=0, channels_per_peer=2, acquire_rounds=1
rank 0: peer=1, rank_graph_topo=5, protocol=UB_MEM, path_modes=2/3
rank 0: dual-path data copy from remote rank 1 passed, bytes=4096
rank 0: dual-path validation passed
```

`rank_graph_topo` reports the actual RankGraph layer containing the UB_MEM link. It can differ between
single-host and multi-host runs and does not select `pathMode`. The program verifies that the two Channel handles
are different. The example succeeds when every rank verifies the complete sharded payload and the script exits
with 0.

## How It Works

1. Rank 0 creates and distributes one `HcclRootInfo`, and every rank initializes one communication domain.
2. Each rank scans the RankGraph layers and selects one `COMM_PROTOCOL_UB_MEM` link for each peer.
3. A round-robin schedule runs `rankSize - 1` acquisition rounds. Each rank connects to one peer per round, and
   all rounds cover every rank pair.
4. Each round builds descriptors for `pathMode=2` and `pathMode=3` and acquires both channels in one call.
5. The two channels copy the first and second shards on separate streams before either stream is synchronized.

| Channel order | Link filter | `pathMode` | Data range |
| --- | --- | --- | --- |
| 1 | `COMM_PROTOCOL_UB_MEM` | `2` | First 2 KB |
| 2 | `COMM_PROTOCOL_UB_MEM` | `3` | Second 2 KB |

See the [design notes](./方案概述.md) for the complete API and memory exchange flow.

## Multi-Host Run

For eight devices across two hosts, start the first command on the rank 0 host, followed by the second command on
the other host:

```bash
# Host A: ranks 0-3, devices 0-3
bash run.sh -pes 8 -ipport tcp://<host_A_IP>:8899 -gnpus 4 -fpe 0 -fnpu 0

# Host B: ranks 4-7, devices 0-3
bash run.sh -pes 8 -ipport tcp://<host_A_IP>:8899 -gnpus 4 -fpe 4 -fnpu 0
```

For 16 devices with eight devices per host, use `-pes 16`, `-gnpus 8`, and first-rank values `-fpe 0` and
`-fpe 8`. The total rank count must be even. Eight ranks use seven acquisition rounds, while 16 ranks use 15;
every rank still creates two channels to every other rank.

## Single-Link Debug File

`debug.asc` is a separate two-rank debug program that selects either MESH or CLOS in one run.
It does not change the `hccl_link_select` example and does not use `run.sh`. See the
[debug instructions](./debug.md) for configuration and manual launch commands.

## Troubleshooting

| Symptom | What to check |
| --- | --- |
| `no UB_MEM link found` | There is no `COMM_PROTOCOL_UB_MEM` link between the local and target ranks. |
| `HcclChannelAcquire failed` | Use the logged `round` and `peer` values to identify the failing rank pair. |
| `connect ipport failed` | Ensure rank 0 is running and every host can reach the IP address and port specified by `-ipport`. |
| Processes wait indefinitely or reject arguments | All hosts must use the same `-pes`, and rank ranges must be complete and unique. |

`-ipport` distributes `HcclRootInfo` only. It does not select the HCCL data link.
