# OnePath单链路调试说明

本文只说明如何使用`debug.asc`验证两个Device之间的MESH或CLOS UB_MEM链路。调试程序固定使用两个rank，
一次只创建一种拓扑的Channel，不依赖`main.cpp`、`ubmem_data_copy.asc`或`run.sh`。

## 1. 环境要求

- Ascend 950PR或Ascend 950DT，NPU架构为`dav-3510`。
- CANN头文件和动态库包含HCCL Resource API以及`HcclChannelDesc::ubMemAttr.pathMode`字段。
- 两个Device之间存在准备验证的`COMM_PROTOCOL_UB_MEM`链路。
- 跨机调试时，rank 1所在机器可以访问rank 0机器的TCP端口，默认端口为`8899`。

先加载CANN环境：

```bash
source ${install_path}/cann/set_env.sh
```

## 2. 选择MESH或CLOS

打开`debug.asc`，找到以下配置：

```cpp
constexpr CommTopo TARGET_TOPO = COMM_TOPO_1DMESH;
// constexpr CommTopo TARGET_TOPO = COMM_TOPO_CLOS;
```

上面的配置验证MESH，对应`pathMode=2`。验证CLOS时改为：

```cpp
// constexpr CommTopo TARGET_TOPO = COMM_TOPO_1DMESH;
constexpr CommTopo TARGET_TOPO = COMM_TOPO_CLOS;
```

必须保证只有一个`TARGET_TOPO`定义生效。修改后需要重新编译。

## 3. 配置RootInfo地址

同一文件中配置RootInfo地址和端口：

```cpp
constexpr const char* ROOT_INFO_IP = "127.0.0.1";
constexpr uint16_t ROOT_INFO_PORT = 8899;
```

- 单机两卡：保持`127.0.0.1`。
- 跨机两卡：两台机器都填写rank 0所在机器的可访问IP。
- 两端必须使用相同的端口和目标拓扑。

该TCP地址只用于rank 0向rank 1发送`HcclRootInfo`，不参与HCCL链路选择和数据搬运。

## 4. 编译调试程序

在`hccl_link_select`目录执行：

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build --target hccl_link_select_debug -j
```

生成的程序为：

```text
./build/hccl_link_select_debug
```

程序参数为：

```text
./build/hccl_link_select_debug <rank_id> <device_id>
```

其中`rank_id`只能是`0`或`1`，`device_id`是当前机器上的本地Device编号。

## 5. 单机两卡调试

配置：

```cpp
constexpr const char* ROOT_INFO_IP = "127.0.0.1";
```

先在终端1启动rank 0：

```bash
./build/hccl_link_select_debug 0 0
```

再在终端2启动rank 1：

```bash
./build/hccl_link_select_debug 1 1
```

对应关系为：

```text
rank 0 -> Device 0
rank 1 -> Device 1
```

rank 0会等待rank 1连接；rank 1在rank 0尚未监听时会自动重试约30秒。

## 6. 跨机两卡调试

假设：

```text
机器A：192.168.1.10，运行rank 0
机器B：运行rank 1
```

两台机器的`debug.asc`都设置：

```cpp
constexpr const char* ROOT_INFO_IP = "192.168.1.10";
```

两边使用相同的`TARGET_TOPO`配置并重新编译。然后在机器A执行：

```bash
./build/hccl_link_select_debug 0 0
```

在机器B执行：

```bash
./build/hccl_link_select_debug 1 0
```

跨机场景中，两台机器可以同时使用各自的Device 0，因为Device ID是机器内编号；rank ID必须分别为0和1。

## 7. 成功日志

MESH验证应包含：

```text
rank 0/2: device=0, topo=mesh, root=127.0.0.1:8899
rank 0: select mesh link to rank 1, protocol=UB_MEM, path_mode=2
rank 0: data copy from remote rank 1 passed, bytes=4096
```

CLOS验证应包含：

```text
rank 0/2: device=0, topo=clos, root=127.0.0.1:8899
rank 0: select clos link to rank 1, protocol=UB_MEM, path_mode=3
rank 0: data copy from remote rank 1 passed, bytes=4096
```

两个rank均输出`data copy from remote rank ... passed`并返回0，表示所选OnePath链路可以完成远端数据读取。

## 8. 常见问题

| 现象 | 检查方法 |
| --- | --- |
| `bind port 8899 failed` | 检查rank 0机器的8899端口是否被占用，或修改`ROOT_INFO_PORT`后重新编译两端。 |
| `connect root info endpoint failed` | 检查两端配置的rank 0 IP、路由和防火墙；确认rank ID没有填反。 |
| `no UB_MEM mesh link found` | 当前两个Device之间不存在MESH UB_MEM链路，或`TARGET_TOPO`配置错误。 |
| `no UB_MEM clos link found` | 当前两个Device之间不存在CLOS UB_MEM链路，或`TARGET_TOPO`配置错误。 |
| `HcclChannelAcquire`失败或进程等待 | 确认两端运行相同的程序版本、拓扑和端口，并且rank ID分别为0和1。 |
| `device ... is out of range` | 使用`npu-smi info`确认当前机器可用的Device ID。 |
| 数据校验失败 | 检查两端是否使用相同CANN/HCCL版本，并确认所选OnePath链路支持远端HCCL Buffer访问。 |

完成MESH调试后，注释MESH定义、启用CLOS定义、重新编译并重复上述步骤，即可分别确认两条链路。
