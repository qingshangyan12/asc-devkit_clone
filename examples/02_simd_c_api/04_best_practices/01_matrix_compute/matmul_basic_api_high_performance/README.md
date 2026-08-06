# Matmul基础API高性能样例

## 概述

本样例使用Ascend C C API实现FP16矩阵乘，并提供单缓冲基线和L1/L0 Ping-Pong双缓冲两个场景。两个场景使用相同的矩阵规格、Cube分块、核数和计算接口，用于对比三级流水重叠带来的性能变化。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
matmul_basic_api_high_performance
├── scripts
│   ├── gen_data.py          # 生成输入数据和真值数据
│   └── verify_result.py     # 校验输出结果
├── CMakeLists.txt           # 编译工程文件
├── data_utils.h             # 二进制数据读写工具
├── mmad_double_buffer.asc   # Kernel及Host侧实现
├── README.md
└── README_en.md
```

## 样例描述

样例计算`C = A x B`，矩阵规格如下：

| 矩阵 | 数据类型 | 逻辑shape | GM排布 |
| --- | --- | --- | --- |
| A | FP16 | `[1024, 1024]` | ND，按`[M, K]`存储 |
| B | FP16 | `[1024, 1024]` | 转置后的ND，按`[N, K]`存储 |
| C | FP32 | `[1024, 1024]` | ND，按`[M, N]`存储 |

基本块为`[baseM, baseN, baseK] = [128, 128, 64]`。M轴和N轴各切分为8块，共启动64个逻辑Cube核，每个核计算一个`128 x 128`输出块；K轴循环16轮并在L0C中累加。

数据搬运路径及排布如下：

```text
GM(ND) --MTE2/ND2NZ--> L1(Nz) --MTE1--> L0A/L0B --M--> L0C(Nz) --FIX/NZ2ND--> GM(ND)
```

编译参数`SCENARIO_NUM`的取值如下。

| SCENARIO_NUM | 场景 | L1/L0缓冲区 | 流水关系 |
| --- | --- | --- | --- |
| 0 | 单缓冲基线 | A、B各一组L1和L0缓冲区 | 每个K块按搬入、加载、MMAD顺序串行完成 |
| 1 | 双缓冲优化 | A、B各两组L1和L0 Ping/Pong缓冲区 | MTE2、MTE1、M流水处理不同K块 |

### 场景0：单缓冲基线

基线场景在L1中各分配一块A/B缓冲区，在L0A/L0B中各分配一块缓冲区。每轮K循环依次执行：

1. `asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_nd2nz`完成GM到L1搬运；
2. `asc_copy_l12l0a`和`asc_copy_l12l0b`完成L1到L0搬运；
3. `asc_mmad`完成当前K块计算并累加到L0C；
4. 等待当前轮全部流水结束，再复用同一组L1/L0地址。

单缓冲存在MTE2等待MTE1、MTE1等待M、M完成后才能进入下一轮的问题，作为双缓冲优化的性能基线。

### 场景1：L1/L0双缓冲

双缓冲场景为A和B分别分配L1 Ping/Pong以及L0A/L0B Ping/Pong，偶数K轮使用Ping，奇数K轮使用Pong。两组Event ID管理以下依赖：

- `MTE2 -> MTE1`：L1数据就绪后才能加载L0；
- `MTE1 -> MTE2`：L1数据消费完成后允许MTE2覆盖；
- `MTE1 -> M`：L0数据就绪后才能执行MMAD；
- `M -> MTE1`：MMAD消费完成后允许MTE1覆盖L0。

稳定流水阶段中，MTE2搬入下一K块、MTE1加载当前K块、M流水计算上一K块可以并行执行，从而减少流水等待。全部K块累加后，`asc_copy_l0c2gm`将L0C结果转换为ND格式并写回GM。

## 性能对比方法

两个场景除缓冲区数量和同步方式外，其余计算条件一致。分别编译`SCENARIO_NUM=0`和`SCENARIO_NUM=1`，使用性能分析工具采集Kernel的Task Duration，以单缓冲场景作为基线：

```text
性能提升比例 = 基线耗时 / 双缓冲耗时 - 1
```

同时可对比MTE2、MTE1和Cube流水的利用率与等待时间，确认收益是否来自搬运、加载和计算的重叠，而不是shape、分块或核数变化。

## 编译运行

配置CANN环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

在本样例目录编译并运行指定场景：

```bash
SCENARIO_NUM=0
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j
python3 ../scripts/gen_data.py
./c_api_mmad_double_buffer_example
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

将`SCENARIO_NUM`改为`1`并重新执行CMake、编译、运行和校验，即可测试双缓冲场景。切换运行模式前需要清理build目录中的CMake缓存。

| 编译选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式 |
| `SCENARIO_NUM` | `0`、`1`（默认） | 单缓冲基线、双缓冲优化 |

精度校验成功时输出：

```text
test pass!
```
