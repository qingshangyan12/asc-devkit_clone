# MC2 多集合通信样例

## 概述

本样例基于 `mc2_allgather` 的 A5/`dav-3510` MC2 路径，使用一个统一的 Device
kernel 支持以下集合通信：

- `allgather`
- `allreduce`
- `alltoall`
- `alltoallv`
- `reducescatter`

运行时通过 `--ops` 选择算子，通过 `--data-type` 选择数据类型。`allreduce` 和
`reducescatter` 另外通过 `--reduce-op` 选择 `sum`、`max` 或 `min`，默认值为
`sum`。

## 目录结构

```text
mc2_collective/
├── CMakeLists.txt
├── collective.asc
├── collective_common.h
├── main.asc
├── README.md
└── scripts/
    └── gen_data.py
```

## 输入布局

普通算子使用：

```text
input/<rank>/x1.bin
```

其中输入文件是不带头的原始字节流，Host 侧根据 `--data-type` 解释元素宽度。

| 算子 | 每个 rank 的输入元素数 | 每个 rank 的输出元素数 |
| --- | ---: | ---: |
| `allgather` | `sendCount` | `sendCount * rankSize` |
| `allreduce` | `count` | `count` |
| `alltoall` | `dataCount * rankSize` | `dataCount * rankSize` |
| `reducescatter` | `recvCount * rankSize` | `recvCount` |

`alltoallv` 除了 `x1.bin` 外，还需要以下四个文件，每个文件包含
`rankSize` 个 `uint64_t`：

```text
input/<rank>/send_counts.bin
input/<rank>/send_displs.bin
input/<rank>/recv_counts.bin
input/<rank>/recv_displs.bin
```

数组下标表示目标或源 rank，计数和偏移的单位均为数据元素，而不是字节。必须满足：

```text
send_counts[source][destination] == recv_counts[destination][source]
```

输入文件大小至少覆盖本 rank 的所有 `send_displs[i] + send_counts[i]`，
输出 buffer 覆盖所有 `recv_displs[i] + recv_counts[i]`；偏移之间允许存在空洞，
样例会将空洞初始化为 0。

## 命令行

```bash
./demo --ops allgather --data-type fp16
./demo --ops allreduce --data-type fp32 --reduce-op sum
./demo --ops alltoall --data-type int32
./demo --ops alltoallv --data-type fp16
./demo --ops reducescatter --data-type fp16 --reduce-op max
```

输入和输出目录可通过以下参数覆盖：

```bash
./demo --ops allreduce --data-type int32 \
    --input-dir /path/to/input --output-dir /path/to/output
```

支持的数据类型包括：

```text
int8 int16 int32 fp16 fp32 int64 uint64 uint8 uint16 uint32
fp64 bf16 hif8 fp8e4m3 fp8e5m2 fp8e8m0
```

`allreduce` 和 `reducescatter` 当前按 HCCL Device API 文档限制为
`int8`、`int16`、`int32`、`fp16`、`fp32`、`bf16`。

## 生成输入和校验

脚本会按当前可用 rank 数生成示例输入、AlltoAllV 元数据和 CPU 标准输出：

```bash
python3 scripts/gen_data.py --ops allgather --rank-size 4 --count 128 --data-type fp16
python3 scripts/gen_data.py --ops alltoallv --rank-size 4 --data-type fp16
```

NPU 运行结束后可校验输出：

```bash
python3 scripts/gen_data.py --ops allreduce --rank-size 4 --count 128 \
    --data-type fp32 --reduce-op sum --verify
```

校验脚本按字节比较 `output/<rank>/output_npu.bin` 和
`output/<rank>/output_cpu_low.bin`。

## 实现流程

Host 侧每个 rank：

1. 初始化 HCCL 通信域和 AICPU/AICore 两条 stream。
2. 根据输入文件和元数据推导 count、buffer 大小，并检查 rank 间布局。
3. 设置 `Mc2CcArgs` 的算子算法、输入输出数据类型和归约类型。
4. 使用 `Mc2AcquireCcResCtx` 申请通信上下文，通过 `Mc2CcKernelLaunch` 启动 AICPU
   KFC server。
5. 通过独立 AICore stream 调用统一 `collective_custom` kernel。
6. 回读输出、写文件并按算子构造 CPU 预期结果进行校验。

Device 侧统一遵循：

```text
InitV2 -> selected collective API<true> -> Wait -> SyncAll -> Finalize
```

AlltoAllV 的四个计数/偏移数组由 Host 拷贝到 GM，再传给
`Hccl::AlltoAllV<true>`。

## 编译运行环境

样例默认使用 `dav-3510`：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
```

执行前应加载对应 CANN 环境，并确保有足够的可用 NPU。每个可用设备作为一个
rank 参与通信。

## 参考 API

- [AllGather](../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AllGather.md)
- [AllReduce](../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AllReduce.md)
- [AlltoAll](../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AlltoAll.md)
- [AlltoAllV](../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AlltoAllV.md)
- [ReduceScatter](../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/ReduceScatter.md)
