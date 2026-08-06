# Add高性能样例

## 概述

本样例使用Ascend C C API实现half类型逐元素Add，并提供单缓冲基线和Ping-Pong双缓冲两个场景。两个场景使用相同的输入、核数、分块和矢量计算接口，用于观察双缓冲对GM/UB数据搬运与Vector计算流水重叠的作用。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
add_high_performance
├── figures
│   └── DoubleBuffer.png     # 双缓冲流水示意图
├── scripts
│   ├── gen_data.py          # 生成输入数据和真值数据
│   └── verify_result.py     # 校验输出结果
├── CMakeLists.txt           # 编译工程文件
├── c_api_add.asc            # Kernel及Host侧实现
├── data_utils.h             # 二进制数据读写工具
├── README.md
└── README_en.md
```

## 样例描述

样例计算两个形状为`[8192, 8192]`的half矩阵的逐元素加法：

```text
z = x + y
```

两个场景均使用64个核，每个核按M轴处理连续数据，每轮处理21760个元素。编译参数`SCENARIO_NUM`的取值如下。

| SCENARIO_NUM | 场景 | UB缓冲区 | 流水关系 |
| --- | --- | --- | --- |
| 0 | 单缓冲基线 | 一组x/y/z缓冲区 | 搬入、计算、搬出串行完成后再处理下一块 |
| 1 | 双缓冲优化 | Ping/Pong两组x/y/z缓冲区 | MTE2、Vector、MTE3处理不同缓冲区并行流水 |

### 场景0：单缓冲基线

基线场景只分配一组x、y、z UB缓冲区。每轮依次执行：

1. `asc_copy_gm2ub_align`将x、y从GM搬入UB；
2. VF函数使用寄存器版`asc_add`完成计算；
3. `asc_copy_ub2gm_align`将z搬回GM；
4. 等待所有流水完成后复用同一组UB地址。

由于下一轮必须等待当前轮的搬入、计算和搬出全部结束，MTE2、Vector和MTE3之间不能形成跨轮重叠。该场景作为双缓冲优化的性能基线。

### 场景1：Ping-Pong双缓冲

优化场景分配Ping/Pong两组x、y、z UB缓冲区，相邻轮次交替使用。`asc_sync_notify`和`asc_sync_wait`通过两组Event ID管理以下依赖：

- `MTE2 -> V`：输入搬入完成后开始计算；
- `V -> MTE2`：计算读取完成后允许覆盖对应输入缓冲区；
- `V -> MTE3`：计算完成后开始输出搬运；
- `MTE3 -> V`：输出搬运完成后允许复用对应输出缓冲区。

当Vector处理Ping时，MTE2和MTE3可以处理Pong，反之亦然，从而隐藏部分数据搬运等待时间。

<p align="center">
  <img src="figures/DoubleBuffer.png" width="50%">
</p>

## 性能对比方法

两个场景除缓冲区数量和同步方式外，其余计算条件一致。分别编译`SCENARIO_NUM=0`和`SCENARIO_NUM=1`，使用性能分析工具采集Kernel的Task Duration，以单缓冲场景作为基线：

```text
性能提升比例 = 基线耗时 / 双缓冲耗时 - 1
```

实际收益取决于数据搬运与计算耗时的比例。两者越接近，流水重叠通常越充分；如果瓶颈完全由GM带宽决定，双缓冲收益会受到限制。

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
./c_api_add_example
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
