# HCCL链路选择样例

## 功能

本样例演示如何通过HCCL Resource API筛选`COMM_PROTOCOL_UB_MEM`链路，在一个通信域中批量创建
`pathMode=2`和`pathMode=3`两条Channel，并通过两个Stream并发搬运不同数据分片。

每个rank先用自己的rank ID填充本端HCCL Buffer，再从其他rank读取4 KB数据并校验：

```text
对端HCCL Buffer -> UB_MEM链路 -> Ascend C DataCopy -> 本端校验Buffer -> Host校验
```

样例采用一进程一Device的运行方式。`run.sh`负责启动本机rank进程，rank 0进程负责分发`HcclRootInfo`。

## 运行前准备

- 支持Ascend 950PR/Ascend 950DT，NPU架构为`dav-3510`。
- 已安装CANN开发套件，以及包含HCCL Resource API和`ubMemAttr.pathMode`字段的HCCL头文件与库。
- 默认场景需要Device 0和Device 1可用，且两者之间存在UB_MEM链路。

## 编译运行

在本样例目录下配置CANN环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

编译并运行默认的2-rank双Channel验证场景：

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
bash run.sh
```

默认情况下，脚本启动2个进程，分别使用Device 0和Device 1。每个进程只初始化一个通信域，
一次申请两条Channel，并将4 KB验证数据平均分成两个2 KB分片并发搬运。

## 结果示例

日志顺序可能因多进程并发而不同。每个rank都应完成到其他rank的数据校验，例如：

```text
rank 0/2: device=0, channels_per_peer=2, acquire_rounds=1
rank 0: peer=1, rank_graph_topo=5, protocol=UB_MEM, path_modes=2/3
rank 0: dual-path data copy from remote rank 1 passed, bytes=4096
rank 0: dual-path validation passed
```

`rank_graph_topo`显示UB_MEM链路实际所在的RankGraph层，单机场景和跨机场景可能不同，不参与
`pathMode`的选择。程序会检查两条Channel Handle必须不同；所有rank均完成分片数据校验且脚本返回0，
表示样例执行成功。

## 工作原理

1. rank 0生成并分发一份`HcclRootInfo`，各rank初始化一个通信域。
2. 每个rank遍历RankGraph各层，为每个peer筛选一条`COMM_PROTOCOL_UB_MEM`链路。
3. 使用分轮配对算法执行`rankSize - 1`轮建链；每轮每个rank只连接一个peer，所有轮次覆盖任意两个rank。
4. 每轮基于同一链路构造`pathMode=2`和`pathMode=3`两个描述，通过一次`HcclChannelAcquire`创建。
5. 两个Channel分别在两个Stream上传输前后两个分片，全部下发后再同步并完成Host侧校验。

| Channel顺序 | 链路筛选 | `pathMode` | 数据范围 |
| --- | --- | --- | --- |
| 1 | `COMM_PROTOCOL_UB_MEM` | `2` | 前2 KB |
| 2 | `COMM_PROTOCOL_UB_MEM` | `3` | 后2 KB |

详细接口调用和内存交换流程参见[方案概述](./方案概述.md)。

## 跨机运行

两机各4卡、总共8卡时，先在rank 0所在机器执行第一条命令，再在另一台机器执行第二条：

```bash
# 机器A：rank 0-3，Device 0-3
bash run.sh -pes 8 -ipport tcp://<机器A_IP>:8899 -gnpus 4 -fpe 0 -fnpu 0

# 机器B：rank 4-7，Device 0-3
bash run.sh -pes 8 -ipport tcp://<机器A_IP>:8899 -gnpus 4 -fpe 4 -fnpu 0
```

两机各8卡、总共16卡时，将`-pes`改为`16`，两机分别使用`-fpe 0`和`-fpe 8`，并将`-gnpus`改为`8`。
rank总数必须是偶数。8卡执行7轮、16卡执行15轮；最终每个rank仍会与其余所有rank分别建立两条Channel。

## 单链路调试文件

`debug.asc`是独立的两rank调试程序，一次只选择MESH或CLOS中的一种链路。它不改变
上述`hccl_link_select`样例，也不使用`run.sh`。配置和手工运行方法参见[调试说明](./debug.md)。

## 常见问题

| 现象 | 检查项 |
| --- | --- |
| `no UB_MEM link found` | 当前rank与目标rank之间没有`COMM_PROTOCOL_UB_MEM`链路。 |
| `HcclChannelAcquire failed` | 根据日志中的`round`和`peer`定位失败的rank对。 |
| `connect ipport failed` | 确认rank 0已启动，其他机器可以访问`-ipport`指定的IP和端口。 |
| 进程等待或参数校验失败 | 所有机器必须使用相同的`-pes`，rank范围应完整且不能重复。 |

`-ipport`仅用于分发`HcclRootInfo`，不参与HCCL数据链路选择。
