# MC2纯通信AllGather+ReduceScatter复合样例

## 概述

本样例基于 Ascend C SIMD C++ API 和 HCCL MC2 接口实现不带计算的
AllGather+ReduceScatter复合通信。程序在单进程中使用多线程模拟多 rank，每个可用 NPU
对应一个 rank。

复合流程分为两个阶段：

- AllGather：每个 rank 将 128 个 FP16 元素发送到所有 rank，输出按 rank 顺序拼接。
- ReduceScatter：将上一阶段的 AllGather 输出作为输入，按 SUM 归约后按 rank 分散，
  每个 rank 接收 128 个 FP16 元素。

Host 侧分别为两个通信阶段申请独立的 `OpResCtx`，不能在两个阶段之间复用同一个通信上下文。
两个阶段使用同一个 AICore stream，依靠 stream 顺序保证 AllGather 输出先于 ReduceScatter
读取。AICPU stream 参考 A5 MC2 AICPU 场景，通过申请 unfold thread 并从 thread 资源中
获取 HCCL 管理的 stream；该 stream 由 HCCL/HCOMM 在通信域销毁时释放，样例不调用
`aclrtDestroyStream`。

## 前置条件

- 已安装支持 Ascend C SIMD C++ API、ACL、HCCL 和 HCCL MC2 接口的 CANN 开发环境。
- 本样例位于 asc-devkit 仓内，`adv_api/hccl/hccl_mc2.h` 头文件和 `Mc2*` Host 接口的实现
  均来自本仓。若 CANN 包未内置 `Mc2*` 接口，需先构建 `mc2_client` 目标。
- 运行时至少有 1 个可用 NPU；多 rank 验证需要多个可用 NPU。
- 各 rank 使用同一通信域，并能够通过 HCCL 建立设备间通信。
- 本样例面向 A5/`dav-3510` 架构。实际可用产品以当前 CANN 安装包及硬件环境为准。

## 目录结构

```text
├── CMakeLists.txt                    // ASC/C++ 编译工程文件
├── all_gather_reduce_scatter.asc    // Device 侧两阶段通信核函数
├── main.asc                          // Host 侧初始化、资源管理、下发和校验
├── scripts
│   └── gen_data.py                   // 生成输入、CPU标准输出并校验NPU输出
└── README.md                         // 样例说明文档
```

## 样例规格

| 项目 | 说明 |
|------|------|
| 通信操作 | AllGather + ReduceScatter（SUM归约） |
| 输入数据类型 | FP16（`HCCL_DATA_TYPE_FP16`） |
| 每个 rank 的输入元素数 | 128 个 half |
| AllGather 输出元素数 | `128 * rankSize` 个 half |
| ReduceScatter 输出元素数 | 128 个 half |
| rank 数量 | 当前可用设备数 `rankSize` |
| AllGather 算法配置 | `AllGather=level0:doublering` |
| ReduceScatter 算法配置 | `ReduceScatter=level0:doublering` |
| 通信引擎 | `AICPU_TS` |

输入文件为 `input/<rank>/x1.bin`，每个文件包含 128 个 FP16 元素。若输入文件缺失，Host
侧使用第 `rank + 1 + (index mod 32)` 个整数值生成兜底输入。

AllGather 阶段的结果满足：

```text
gather[rank * 128 + i] = input[rank][i]
```

ReduceScatter 阶段将每个 rank 的 `gather` 作为输入，输出满足：

```text
output[i] = sum_rank(gather[rankId * 128 + i])
           = rankSize * input[rankId][i]
```

其中 `i` 的范围为 `[0, 128)`。样例输入为可精确表示的整数，FP16 SUM 结果可直接校验。

## 实现流程

### Host 侧流程

每个 rank 的执行流程如下：

1. 绑定当前设备并初始化 HCCL 通信域。
2. 申请 unfold thread，并通过 `HcclThreadResGetInfo` 获取 HCCL 管理的 AICPU stream；
   另行创建 AICore stream。
3. 申请输入、AllGather 中间结果和 ReduceScatter 输出的 Device buffer。
4. 按 `HCCL_CMD_ALLGATHER` 申请第一阶段 `OpResCtx`，下发 AICPU KFC server 和 AllGather
   AICore kernel，依次同步两个 stream。
5. 按 `HCCL_CMD_REDUCE_SCATTER` 申请第二阶段 `OpResCtx`，下发第二个 AICPU KFC server
   和 ReduceScatter AICore kernel，依次同步两个 stream。
6. 回读 AllGather 中间结果和最终输出，在 Host 侧逐阶段校验，并写入
   `output/<rank>/output_npu.bin`。
7. 释放 Device buffer，销毁 HCCL 通信域和 AICore stream，最后重置设备。AICPU stream
   随 HCCL/HCOMM 通信域销毁。

### Device 侧流程

两个 Device 核函数均只调用 HCCL 客户端，不执行额外计算，调用顺序为：

```text
InitV2 -> AllGather<true>/ReduceScatter<true> -> Wait -> SyncAll -> Finalize
```

AllGather 的输出 `gatherDev` 直接作为 ReduceScatter 的输入。两个核函数在同一 AICore
stream 上顺序下发。

## 编译运行

### 1. 配置 CANN 环境

```bash
source ${install_path}/cann/set_env.sh
```

其中 `${install_path}` 为 CANN 包安装目录，通常为 `/usr/local/Ascend`。

### 2. 构建 asc-devkit 的 mc2_client

当 CANN 包未内置 `Mc2*` 接口时，在 asc-devkit 根目录执行：

```bash
mkdir -p build
cd build
cmake -DASCEND_CANN_PACKAGE_PATH=${install_path} ..
make mc2_client -j
```

也可以在本样例配置时通过 `-DMC2_CLIENT_LIB=<path>/libmc2_client.so` 指定库路径。

### 3. 生成输入和 CPU 标准输出

在本样例目录下执行：

```bash
mkdir -p build
cd build
python3 ../scripts/gen_data.py --rank-size 2
```

运行时参与通信的 rank 数由可用设备数决定；生成数据时应使用相同的 `--rank-size`。

### 4. 编译并执行

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
./demo
```

执行 `./demo` 前，程序会检查 `ASCEND_PROCESS_LOG_PATH`。如果该环境变量非空且路径
存在，则递归删除该路径；路径不存在或未设置时跳过。该处理发生在 `aclInit` 前，因此
可以避免本次运行混入上一次执行留下的日志。

### 5. 校验输出

```bash
python3 ../scripts/gen_data.py --rank-size 2 --verify
```

校验脚本比较 `output/<rank>/output_npu.bin` 和
`output/<rank>/output_cpu_low.bin`，全部一致时输出 `test pass!`。

## 预期输出

以 2 卡为例，输出中应包含以下信息，rank 间的打印顺序可能不同：

```text
AllGather+ReduceScatter composite sample start, rankSize = 2
rank0 AllGather stage verify pass
rank0 ReduceScatter stage verify pass
rank1 AllGather stage verify pass
rank1 ReduceScatter stage verify pass
AllGather+ReduceScatter composite sample pass
```

## 相关接口

- [AllGather](../../../../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AllGather.md)
- [ReduceScatter](../../../../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/ReduceScatter.md)
- [HCCL Context](../../../../docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL-Context/HCCL-Context.md)
