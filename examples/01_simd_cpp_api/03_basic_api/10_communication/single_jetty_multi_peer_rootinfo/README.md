# Hcomm 单 Jetty 多对端 RootInfo 样例

本样例展示 host 控制面如何通过 `HcclRootInfo` 初始化多卡通信域，为全部 peer 创建 logical Channel，并将
属于同一本地 Endpoint group 的 Channel 放入同一次 `HcclChannelAcquireWithConfig`，共享一个底层 Jetty。
样例不依赖 ranktable JSON；rank 0 生成 RootInfo，并通过独立的 TCP bootstrap 通道分发给其他 rank。

## 本样例支持的产品及 CANN 软件版本

| 产品 | CANN 软件版本 |
|------|---------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 资源模型

```text
本地 rank
  Endpoint group 0
    Channel(peer A) --+
    Channel(peer B) --+-- shared Jetty 0

  Endpoint group 1
    Channel(peer C) ----- shared Jetty 1
```

控制面先从 RankGraph 中为每个 peer 选择一条 `COMM_PROTOCOL_UBC_CTP` link，再按 link 的本地 Endpoint
分组。Endpoint 是否相同使用与 HCCL 底层一致的字段：`protocol`、`commAddr.type` 和
`commAddr.eid`。

- 同一 group 的全部 `HcclChannelDesc` 具有相同 `localEndpoint`，一次批量调用
  `HcclChannelAcquireWithConfig`，共享一个 Jetty。
- 不同 group 的本地 Endpoint 不同，分别创建，不能放进同一次 shared-Jetty Acquire。
- group 只有一个 peer 时仍使用相同创建流程，该 Jetty 只服务一个 Channel。

## 控制面流程

```text
rank 0: HcclGetRootInfo
  -> TCP 分发同一份 HcclRootInfo
  -> 各 rank 调用 HcclCommInitRootInfo
  -> HcclCommMemReg
  -> HcclRankGraphGetLayers / GetRanksByLayer / GetLinks
  -> 为每个 peer 选择最近层的一条 UBC_CTP link
  -> 按 localEndpoint 对 Channel descriptor 分组
  -> 每个 group 设置 shared queue config
  -> 每个 group 调用一次 HcclChannelAcquireWithConfig
  -> 使用 ring 路径启动 Hcomm kernel
  -> 等待远端 notify 并校验接收数据
```

TCP 地址只用于分发 `HcclRootInfo`，不参与 HCCL 数据链路、Endpoint 或 Jetty 选择。所有 rank 必须使用
相同的 `rank_size` 和同一份 RootInfo。

device kernel 使用 `COMM_PROTOCOL_UBC_CTP`，控制面创建 Channel 时使用 `COMM_ENGINE_AIV`。发送端完成
`Drain` 后，接收端轮询本地注册内存中的 notify 值；数据同步不依赖 TCP 或 `HcclBarrier`。
样例支持 `INT8`、`INT16`、`INT32`、`UINT32`、`FP16`、`FP32`、`BFP16` 数据类型和
`MAX`、`MIN`、`SUM` 操作的任意组合。`WriteReduceNbi` 的 `len` 参数按字节传入。

样例在正式传输前执行10次 `WriteReduceNbi<false> + Commit + Drain` 预热，随后采样100次。每次性能计时
仅覆盖一次 `WriteReduceNbi<false>` 调用，从进入接口前开始，到接口返回后结束；计时区间不包含
doorbell、`Commit`、`Drain` 和 notify。样例输出该接口准备并写入WQE耗时的最小值、中位数和平均值，
Ascend 950PR/Ascend 950DT上1 cycle对应1 ns。

## 编译运行

配置 CANN 环境变量后，运行同机多卡样例：

```bash
source <install-path>/cann/set_env.sh
cd examples/01_simd_cpp_api/03_basic_api/10_communication/single_jetty_multi_peer_rootinfo
bash run.sh
```

`run.sh` 参数：

| 参数 | 说明 |
|------|------|
| `$1` | 通信字节数，默认 `4096` |
| `$2` | 数据类型，默认 `INT32` |
| `$3` | Reduce 操作，默认 `SUM` |
| `$4` | rank 0 分发 RootInfo 的 TCP 地址，默认 `tcp://127.0.0.1:8899` |

例如验证 `FP16 + MAX`：

```bash
bash run.sh 4096 FP16 MAX
```

单机运行时，全局 rank 与本机 device 编号相同。也可以手动启动：

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j

./build/demo 2 0 0 4096 BFP16 MIN &
./build/demo 2 1 1 4096 BFP16 MIN &
wait
```

`demo` 参数顺序为
`<rank_size> <rank> <device> <bytes> <data_type> <reduce_op> [ipport]`。不传 `ipport` 时使用默认地址。

## 约束

- 当前样例固定使用 2 个 rank。
- `bytes` 必须是所选数据类型字节数的整数倍。
- RootInfo 的 TCP bootstrap 通道必须在所有 rank 调用 `HcclCommInitRootInfo` 前完成分发。
- 是否真正发生多 Channel 共享取决于 RankGraph 返回的 link 是否具有相同本地 Endpoint；样例不会强行
  合并不同 Endpoint。
- 实际运行需要可用的 CANN、HCCL 和 NPU 环境。
