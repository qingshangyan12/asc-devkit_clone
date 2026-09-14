# MC2纯通信AllGather拓展样例

## 概述

本样例基于 [mc2_allgather](../mc2_allgather/README.md) 基础样例扩展而来，同样基于 Ascend C SIMD C++ API
和 HCCL MC2 接口实现不带计算的 AllGather 集合通信，展示单进程多线程模拟多 rank 场景下的纯通信流程。

相对基础样例，本拓展样例的主要差异为：

- **数据类型可指定**：通过 `--dtype` 命令行参数指定参与通信的数据类型（默认 `fp16`），在基础样例支持的
  `int8`、`uint8`、`int16`、`uint16`、`fp16`、`int32`、`uint32`、`fp32`、`int64`、`uint64`、`fp64`
  基础上，额外支持 `bfp16`（按原始 bit 生成、通信和校验）。
- **数据规模不写死**：每卡参与通信的元素个数 `sendCount` 完全由输入文件大小推导，
  `sendCount = fileSize / sizeof(dtype)`。要求所有 rank 的输入文件存在且大小一致，否则直接报错退出，
  不再使用固定元素个数的兜底数据。
- **输入输出目录可配置**：通过 `--input-dir`、`--output-dir` 指定输入输出目录，默认分别为 `./input`、
  `./output`。

样例由 Host 侧和 Device 侧两部分组成，通信流程与基础样例一致：

- Host 侧初始化 HCCL 通信域，通过 `Mc2AcquireCcResCtx` 申请通信资源，并通过 `Mc2AicpuKernelLaunch` 下发
  AICPU KFC server。
- Device 侧在 AIV 核上仅使用 HCCL 高阶 API 下发 AllGather 任务，并通过 `Wait`、`SyncAll` 和 `Finalize`
  完成通信，不执行 Add、Matmul 等计算。数据类型和元素个数均以运行时参数传入，Device 侧不写死数据规格。
- 主程序根据当前环境中的可用设备数动态设置 `rankSize`，每个设备由一个线程绑定并作为一个 rank 参与通信。

## 前置条件

- 已安装支持 Ascend C SIMD C++ API、ACL、HCCL 和 HCCL MC2 接口的 CANN 开发环境。
- 运行时至少有 1 个可用 NPU；AllGather 的多 rank 验证需要多个可用 NPU。
- 各 rank 使用同一通信域，并能够通过 HCCL 建立设备间通信。
- 运行前必须通过 `scripts/gen_data.py` 生成各 rank 的输入文件，本样例不提供兜底数据。
- 本样例面向 A5/`dav-3510` 架构。实际可用的产品以当前 CANN 安装包及硬件环境为准。

## 目录结构

```text
├── CMakeLists.txt            // ASC/C++ 编译工程文件
├── all_gather.asc            // Device 侧 AllGather 核函数及 Host 下发入口
├── main.asc                  // Host 侧初始化、资源管理、核函数下发和结果校验
├── scripts
│   └── gen_data.py           // 按卡生成任意类型、任意规模的输入和CPU标准输出并校验NPU输出
└── README.md                 // 样例说明文档
```

## 样例规格

| 项目 | 说明 |
|------|------|
| 通信操作 | AllGather |
| 输入数据类型 | 由 `--dtype` 指定（默认 `fp16`），支持 12 种数据类型 |
| 每个 rank 的输入元素数 | `sendCount = fileSize / sizeof(dtype)`，由输入文件大小推导，不写死 |
| 每个 rank 的输入大小 | 输入文件大小，要求为元素大小的正整数倍 |
| 每个 rank 的输出元素数 | `sendCount * rankSize` 个元素 |
| 每个 rank 的输出大小 | `fileSize * rankSize` 字节 |
| rank 数量 | 当前可用设备数 `rankSize` |
| AICore 核函数名 | `all_gather_custom`（仅通信） |
| Host 下发入口 | `all_gather_do` |
| 通信算法配置 | `AllGather=level0:doublering` |
| 通信引擎 | `AICPU_TS` |

输入数据由 `scripts/gen_data.py` 生成到 `<input-dir>/<rank>/x1.bin`，Host 侧读取后拷贝到 Device。
AllGather 完成后，输出按照 rank ID 顺序拼接，满足：

```text
y[rank * sendCount + i] = <input-dir>/<rank>/x1.bin[i]
其中 rank ∈ [0, rankSize)，i ∈ [0, sendCount)
```

数据流为：

```text
各rank输入文件 -> 各rank输入Buffer -> HCCL AllGather -> 各rank输出Buffer -> 输出文件
```

输入填充和输出校验仅用于区分 rank 和验证通信结果，不属于 Device 侧通信核的计算逻辑。
`bfp16` 无 numpy 原生类型，由于 AllGather 为纯数据搬运，脚本按 `uint16` 原始 bit 生成和校验，
Host 侧打印时同样按原始 bit 输出。

## 实现流程

### Host 侧流程

`main.asc` 中每个 rank 的执行流程如下。除输入准备和结果校验外，Host 侧不对通信数据执行计算：

1. 解析命令行参数 `--dtype`、`--input-dir`、`--output-dir`，得到数据类型描述、输入输出目录。
2. 校验所有 rank 的输入文件 `<input-dir>/<rank>/x1.bin`：文件必须存在、大小一致，且为元素大小的
   正整数倍，由此推导 `sendCount = fileSize / sizeof(dtype)`。
3. 调用 `aclrtSetDevice` 绑定当前设备，并使用所有 rank 共享的 `HcclRootInfo` 初始化 HCCL 通信域。
4. 创建两条 ACL stream，分别用于 AICPU KFC server 和 AICore client。
5. 创建 `Mc2CcArgs`，设置通信域名称、AllGather 算法配置、指定数据类型的输入输出类型和 `AICPU_TS`
   通信引擎。
6. 在 Device 内存中申请输入和输出缓冲区，并从输入文件读取数据拷贝到 Device。
7. 调用 `Mc2AcquireCcResCtx` 申请通信资源，获得 Device 侧 `OpResCtx` 及其大小。
8. 调用 `Mc2AicpuKernelLaunch` 在 AICPU stream 上下发 AICPU KFC server。样例使用
   `blockDim = 0`，由接口按 1 个 block 处理。
9. 调用 `all_gather_do` 在独立的 AICore stream 上下发 AICore 核函数，使 client 能与
   阻塞等待消息的 server 并发执行。
10. 先同步 AICore stream，再同步 AICPU stream；AICore client 完成并发送 `Finalize`
    后，AICPU server 才能正常退出。
11. 将输出拷贝回 Host，卡 0 和卡 1 的部分输出会打印到标准输出，并写入 `<output-dir>/<rank>/output_npu.bin`；
    随后重新读取各 rank 的输入文件，检查每个 rank 对应的数据块是否逐字节一致。
12. 释放 Host/Device 内存、通信域和两条 stream，并重置设备。

主线程先调用 `aclrtGetDeviceCount` 获取设备数，再为每个设备创建一个线程。所有线程完成后，主线程根据各
rank 的校验结果输出样例是否执行成功。

### Device 侧流程

`all_gather.asc` 中的 `all_gather_custom` 只在 AIV 核上调用 HCCL 客户端，不包含任何计算算子或数据变换。
通信接口的调用顺序为：

```text
InitV2 -> AllGather<true> -> Wait -> SyncAll -> Finalize
```

- `InitV2` 使用 Host 侧 `Mc2AcquireCcResCtx` 返回的 `OpResCtx`，`initTiling` 传入 `nullptr`。
- `AllGather<true>` 每个 rank 发送 `sendCount` 个指定类型元素，`sendCount` 和 `dataType` 均为运行时参数；
  `strideCount = 0` 表示输出中的相邻 rank 数据块连续存放。
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

### 2. 创建构建目录并生成输入和真值数据

```bash
mkdir -p build
cd build
python3 ../scripts/gen_data.py
```

该脚本会生成按卡组织的输入和 CPU 标准输出，默认 `rankSize = 2`、`dtype = fp16`、`sendCount = 128`：

```text
input/0/x1.bin
input/1/x1.bin
output/0/output_cpu_low.bin
output/1/output_cpu_low.bin
```

拓展样例支持按需指定数据类型和每卡元素个数，例如生成 `int32`、每卡 1000 个元素的数据：

```bash
python3 ../scripts/gen_data.py --dtype int32 --send-count 1000
```

如果实际参与 rank 数不是 2，需要显式传入与运行时一致的 rank 数，例如：

```bash
python3 ../scripts/gen_data.py --rank-size 4
```

脚本参数说明：

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `--dtype` | `fp16` | 数据类型，可选 `int8`、`uint8`、`int16`、`uint16`、`fp16`、`bfp16`、`int32`、`uint32`、`fp32`、`int64`、`uint64`、`fp64` |
| `--send-count` | `128` | 每卡元素个数，任意正整数，程序侧由文件大小反推 |
| `--rank-size` | `2` | 参与通信的 rank 数 |
| `--input-dir` | `./input` | 输入文件目录 |
| `--output-dir` | `./output` | 输出文件目录 |

### 3. 编译样例

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
```

`CMAKE_ASC_ARCHITECTURES` 可设置为：

| 选项 | 说明 |
|------|------|
| `dav-3510` | 编译面向 dav-3510 架构的 Device 代码，未指定时由工程默认值决定 |

### 4. 执行样例

在 `build` 目录下执行：

```bash
./demo
```

程序支持以下命令行参数：

| 参数 | 默认值 | 说明 |
|------|--------|------|
| `--dtype <name>` | `fp16` | 参与通信的数据类型，需与生成数据时的 `--dtype` 一致 |
| `--input-dir <path>` | `./input` | 输入目录，读取 `<path>/<rank>/x1.bin` |
| `--output-dir <path>` | `./output` | 输出目录，写入 `<path>/<rank>/output_npu.bin` |

例如以 `int32` 类型运行，并使用自定义目录：

```bash
./demo --dtype int32
./demo --dtype fp16 --input-dir ./input --output-dir ./output
```

`sendCount` 由输入文件大小和 `--dtype` 指定的元素大小共同决定，无需在命令行传入；输入文件缺失、
大小不是元素大小整数倍或各 rank 大小不一致时，程序会报错退出。

本样例面向实际 NPU 上的 HCCL 多设备通信。由于运行时需要初始化 HCCL 通信域、下发 AICPU KFC server 并访问多个设备，不建议使用 CPU 调试或 NPU 仿真模式验证本样例的通信行为。

### 5. 对比输出和真值数据

在 `build` 目录下执行（`--dtype`、`--send-count`、`--rank-size` 需与生成数据时一致）：

```bash
python3 ../scripts/gen_data.py --verify
```

脚本会读取 `output/cardN/output_npu.bin`，并与对应的 `output/cardN/output_cpu_low.bin` 逐元素比较。
全部一致时输出：

```text
test pass!
```

## 预期输出

不同 rank 的日志顺序可能不同。所有 rank 校验通过后，输出中应包含类似以下信息：

```text
AllGather ext sample start, rankSize = <rankSize>, dtype = <dtype>, sendCount = <sendCount>, inputDir = ./input, outputDir = ./output
rank0 AllGather verify pass
...
rank<rankSize - 1> AllGather verify pass
AllGather ext sample pass
```

其中 `<rankSize>` 为程序启动时通过 `aclrtGetDeviceCount` 获取的可用设备数，`<dtype>` 和 `<sendCount>`
分别为命令行指定的数据类型和由输入文件推导的每卡元素个数。若任一 rank 的输出数据不符合预期，程序会打印
`AllGather verify failed` 或 `AllGather ext sample failed`。

## 相关接口

- [AllGather](../../../../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AllGather.md)
- [HCCL Context](../../../../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL-Context/HCCL-Context.md)
