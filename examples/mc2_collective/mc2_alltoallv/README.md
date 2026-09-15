# MC2纯通信AlltoAllV样例

## 概述

本样例基于 Ascend C SIMD C++ API 和 HCCL MC2 接口实现不带计算的 AlltoAllV 集合通信，
在 `mc2_allgather` 纯AllGather样例的基础上扩展，展示单进程多线程模拟多 rank 场景下的
纯通信流程。AlltoAllV 是 AlltoAll 的推广：通信域内的卡互相收发数据时，可定制每张卡给
其它各卡发送的数据量和数据在内存中的偏移。第 `i` 张卡将 sendBuf 中偏移 `sdispls[j]`
处的 `sendCounts[j]` 个数据发送到第 `j` 张卡，第 `j` 张卡将其存放到本卡 recvBuf 中偏移
`rdispls[i]` 的位置。

样例由 Host 侧和 Device 侧两部分组成：

- Host 侧初始化 HCCL 通信域，通过 `Mc2AcquireCcResCtx` 申请通信资源，并通过 `Mc2AicpuKernelLaunch` 下发 AICPU KFC server。
- Device 侧在 AIV 核上仅使用 HCCL 高阶 API 下发 AlltoAllV 任务，并通过 `Wait`、`SyncAll` 和 `Finalize` 完成通信，不执行 Add、Matmul 等计算。
- 主程序根据当前环境中的可用设备数动态设置 `rankSize`，每个设备由一个线程绑定并作为一个 rank 参与通信。
- 支持通过 `--dtype` 命令行参数指定参与通信的数据类型（默认 `fp16`），可选 `int8`、`uint8`、`int16`、`uint16`、`fp16`、`bf16`、`int32`、`uint32`、`fp32`、`int64`、`uint64`、`fp64`、`fp8_e4m3`、`fp8_e5m2`、`fp8_e8m0`。
- 输入文件为必选项：各 rank 的输入文件缺失或大小与该 rank 的发送总量不符时直接报错退出，不提供兜底数据。

## 前置条件

- 已安装支持 Ascend C SIMD C++ API、ACL、HCCL 和 HCCL MC2 接口的 CANN 开发环境。
- 本样例位于 asc-devkit 仓内，`adv_api/hccl/hccl_mc2.h` 头文件和 `Mc2*` Host 接口的实现
  均来自本仓。若 CANN 包未内置 `Mc2*` 接口，需先在本仓构建 `mc2_client` 目标（见编译章节）。
- 运行时至少有 1 个可用 NPU；AlltoAllV 的多 rank 验证需要多个可用 NPU。
- 各 rank 使用同一通信域，并能够通过 HCCL 建立设备间通信。
- 运行前必须通过 `scripts/gen_data.py` 生成各 rank 的输入文件，本样例不提供兜底数据。
- 本样例面向 A5/`dav-3510` 架构。实际可用的产品以当前 CANN 安装包及硬件环境为准。
- 本样例的收发数组按最大 32 个 rank 定义，`rankSize` 超过 32 时程序报错退出。

## 目录结构

```text
├── CMakeLists.txt            // ASC/C++ 编译工程文件
├── all_to_all_v.asc          // Device 侧 AlltoAllV 核函数及 Host 下发入口
├── main.asc                  // Host 侧初始化、资源管理、核函数下发和结果校验
├── scripts
│   └── gen_data.py           // 按卡生成输入、CPU标准输出并校验NPU输出
└── README.md                 // 样例说明文档
```

## 样例规格

| 项目 | 说明 |
|------|------|
| 通信操作 | AlltoAllV |
| 输入数据类型 | 由 `--dtype` 指定（默认 `fp16`），支持 15 种数据类型（含 `bf16`、`fp8_e4m3`、`fp8_e5m2`、`fp8_e8m0`） |
| 每个 rank 的输入元素数 | `8 * (rankSize + Σ_j ((2·rank + j) mod rankSize))` 个元素 |
| 每个 rank 的输出元素数 | `8 * (rankSize + Σ_j ((rank + 2·j) mod rankSize))` 个元素 |
| rank 数量 | 当前可用设备数 `rankSize`（不超过 32） |
| AICore 核函数名 | `all_to_all_v_custom`（仅通信） |
| Host 下发入口 | `all_to_all_v_do` |
| 通信算法配置 | `AlltoAll=level0:fullmesh;level1:pairwise` |
| 通信引擎 | `AICPU_TS` |

其中 rank `i` 发往 rank `j` 的数据量为 `8 * (1 + ((2 * i + j) mod rankSize))` 个元素，
rank `i` 从 rank `j` 接收的数据量为 `8 * (1 + ((i + 2 * j) mod rankSize))` 个元素，二者
满足 AlltoAllV 的收发匹配约束（rank `i` 发往 rank `j` 的数据量等于 rank `j` 收自
rank `i` 的数据量）。发送块和接收块分别在 sendBuf/recvBuf 中按目的/来源 rank 顺序紧凑
排列，即 `sdispls`/`rdispls` 为对应数据量的前缀和。

输入数据由 `scripts/gen_data.py` 生成到 `input/<rank>/x1.bin`，Host 侧读取后拷贝到
Device，输入文件缺失或大小与该 rank 的发送总量不符时直接报错退出。AlltoAllV 完成后，
每个 rank 的输出满足：

```text
y[rdispls[i] + m] = x_i[sdispls[i] + m]，其中 m ∈ [0, recvCounts[i])
```

即本卡从 rank `i` 接收的数据块来自 rank `i` 输入中偏移 `sdispls[i]` 处。数据流为：

```text
各rank输入Buffer(不均匀分块) -> HCCL AlltoAllV -> 各rank输出Buffer(不均匀分块)
```

输入填充和输出校验仅用于区分 rank 和验证通信结果，不属于 Device 侧通信核的计算逻辑。
AlltoAllV 为纯数据搬运，`bf16` 与 `fp8` 类型无 numpy 原生类型，脚本按原始 bit（`uint16`/`uint8`）
生成和校验，Host 侧打印时同样按原始 bit 输出。

## 实现流程

### Host 侧流程

`main.asc` 中每个 rank 的执行流程如下。除输入准备和结果校验外，Host 侧不对通信数据执行计算：

1. 调用 `aclrtSetDevice` 绑定当前设备，并使用所有 rank 共享的 `HcclRootInfo` 初始化 HCCL 通信域。
2. 创建两条 ACL stream，分别用于 AICPU KFC server 和 AICore client。
3. 创建 `Mc2CcArgs`，设置通信域名称、AlltoAll 算法配置、`--dtype` 指定的输入输出类型和 `AICPU_TS` 通信引擎。
4. 在 Device 内存中申请输入和输出缓冲区，大小按收发数据量公式计算，并从输入文件读取数据。
5. 调用 `Mc2AcquireCcResCtx` 申请通信资源，获得 Device 侧 `OpResCtx` 及其大小。
6. 调用 `Mc2AicpuKernelLaunch` 在 AICPU stream 上下发 AICPU KFC server。样例使用
   `blockDim = 0`，由接口按 1 个 block 处理。
7. 调用 `all_to_all_v_do` 在独立的 AICore stream 上下发 AICore 核函数，使 client 能与
   阻塞等待消息的 server 并发执行。
8. 先同步 AICore stream，再同步 AICPU stream；AICore client 完成并发送 `Finalize`
   后，AICPU server 才能正常退出。
9. 将输出拷贝回 Host，卡 0 和卡 1 的输出会打印到标准输出（最多打印 32 个元素），并写入
   对应目录的 `output/<rank>/output_npu.bin`；随后重新读取各 rank 的输入文件，按 AlltoAllV
   语义逐字节校验。
10. 释放 Host/Device 内存、通信域和两条 stream，并重置设备。

主线程先调用 `aclrtGetDeviceCount` 获取设备数并校验不超过 32，再为每个设备创建一个线程。所有线程完成后，主线程根据各 rank 的校验结果输出样例是否执行成功。

### Device 侧流程

`all_to_all_v.asc` 中的 `all_to_all_v_custom` 只在 AIV 核上调用 HCCL 客户端，不包含任何计算算子或数据变换。通信接口的调用顺序为：

```text
InitV2 -> AlltoAllV<true> -> Wait -> SyncAll -> Finalize
```

- `InitV2` 使用 Host 侧 `Mc2AcquireCcResCtx` 返回的 `OpResCtx`，`initTiling` 传入 `nullptr`。
- `AlltoAllV<true>` 前，核内通过 `GetRankDim`/`GetRankId` 获取通信域信息，按公式生成
  `sendCounts/sdispls/recvCounts/rdispls` 四个 `uint64` 数组（数组长度为 rank 数，偏移为
  对应数据量的前缀和），数据量公式与 Host 侧真值计算保持一致；`sendType`/`recvType` 为
  运行时参数，来自 Host 侧 `--dtype` 指定的数据类型。
- `Wait` 等待通信任务完成。
- `SyncAll<true>` 确保所有 AIV 核完成通信任务后再调用 `Finalize`。

## 编译运行

在当前样例目录下执行以下步骤。

### 1. 配置 CANN 环境

根据 CANN 的安装路径加载环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

其中 `${install_path}` 为 CANN 包安装目录。未指定安装目录时，通常为
`/usr/local/Ascend`。

### 2. 构建 asc-devkit 的 mc2_client（仅 CANN 未内置 Mc2* 接口时）

本样例的 `Mc2GetCcArgs/Mc2AcquireCcResCtx/Mc2AicpuKernelLaunch` 等 Host 接口由
asc-devkit 仓 `impl/adv_api/detail/hccl/cc/src` 提供。若 CANN 包的 `libmc2_client.so`
未导出这些接口，需先在本仓根目录构建 `mc2_client` 目标：

```bash
cd <asc-devkit仓路径>
mkdir -p build
cd build
cmake -DASCEND_CANN_PACKAGE_PATH=${install_path} ..
make mc2_client -j
```

构建产物为 `build/impl/adv_api/detail/hccl/cc/src/libmc2_client.so`，样例编译时会自动
链接该库并将其拷贝到可执行文件目录。若使用其它路径的库，可通过
`-DMC2_CLIENT_LIB=<路径>/libmc2_client.so` 指定。

### 3. 创建构建目录并生成输入和真值数据

```bash
mkdir -p build
cd build
python3 ../scripts/gen_data.py
```

该脚本会生成按卡组织的输入和 CPU 标准输出，默认 `rankSize = 2`、`dtype = fp16`、
`baseCount = 8`：

```text
input/0/x1.bin
input/1/x1.bin
output/0/output_cpu_low.bin
output/1/output_cpu_low.bin
```

支持按需指定数据类型，例如生成 `int32` 类型的数据：

```bash
python3 ../scripts/gen_data.py --dtype int32
```

如果实际参与 rank 数不是 2，需要显式传入与运行时一致的 rank 数，例如：

```bash
python3 ../scripts/gen_data.py --rank-size 4
```

脚本参数说明：

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `--dtype` | `fp16` | 数据类型，可选 `int8`、`uint8`、`int16`、`uint16`、`fp16`、`bf16`、`int32`、`uint32`、`fp32`、`int64`、`uint64`、`fp64`、`fp8_e8m0`、`fp8_e5m2`、`fp8_e4m3` |
| `--base-count` | `8` | 每对 rank 收发数据量的基本单位 |
| `--rank-size` | `2` | 参与通信的 rank 数 |
| `--input-dir` | `./input` | 输入文件目录 |
| `--output-dir` | `./output` | 输出文件目录 |

`--dtype` 需与运行样例时的 `--dtype` 一致；输入文件缺失或大小不符时，程序会报错退出。

### 4. 编译样例

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
```

`CMAKE_ASC_ARCHITECTURES` 可设置为：

| 选项 | 说明 |
|------|------|
| `dav-3510` | 编译面向 dav-3510 架构的 Device 代码，未指定时由工程默认值决定 |

### 5. 执行样例

在 `build` 目录下执行：

```bash
./demo
```

程序支持 `--dtype` 命令行参数指定参与通信的数据类型（默认 `fp16`，需与生成数据时的
`--dtype` 一致），例如：

```bash
./demo --dtype int32
```

本样例面向实际 NPU 上的 HCCL 多设备通信。由于运行时需要初始化 HCCL 通信域、下发 AICPU KFC server 并访问多个设备，不建议使用 CPU 调试或 NPU 仿真模式验证本样例的通信行为。

### 6. 对比输出和真值数据

在 `build` 目录下执行：

```bash
python3 ../scripts/gen_data.py --verify
```

脚本会读取 `output/0/output_npu.bin`、`output/1/output_npu.bin`，打印卡 0、卡 1 的
NPU 输出，并与对应的 `output/<rank>/output_cpu_low.bin` 逐元素比较。全部一致时输出：

```text
test pass!
```

## 预期输出

不同 rank 的日志顺序可能不同。以 2 卡为例（rank 0 收发总量为 24/16 个元素，rank 1 为
24/32 个元素），所有 rank 校验通过后，输出中应包含类似以下信息：

```text
AlltoAllV sample start, rankSize = 2, dtype = fp16, baseCount = 8
input rank0: 32 33 34 ... 55
input rank1: 64 65 66 ... 87
output rank0: 32 33 34 ... 71
output rank1: 40 41 42 ... 79
rank0 AlltoAllV verify pass
rank1 AlltoAllV verify pass
AlltoAllV sample pass
```

其中 `rankSize` 为程序启动时通过 `aclrtGetDeviceCount` 获取的可用设备数，`dtype` 为命令行
指定的数据类型。若任一 rank 的输出数据不符合预期，程序会打印 `AlltoAllV verify failed` 或
`AlltoAllV sample failed`；输入文件缺失或大小不符时，程序会打印 `[ERROR]` 并退出。
