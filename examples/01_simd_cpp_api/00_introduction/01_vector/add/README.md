# Add样例

## 概述

本样例展示了Ascend C向量加法的基本用法。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── add
│   ├── CMakeLists.txt      // 编译工程文件
│   ├── add.asc             // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  Add样例实现了两个数据相加，返回相加结果的功能。对应的数学表达式为：  
  ```
  z = x + y
  ```

- 样例规格：

  **硬件约束**：
  - 数据量需满足32B对齐（即元素个数需为32B/sizeof(dataType)的整数倍，float类型需8个元素对齐）
  - 本样例shape为[8, 2048]，总元素数16384，满足32B对齐要求

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Add</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

- 样例实现：
  - Kernel实现  
    本样例使用8个核完成计算，每个核处理2048个元素。计算偏移量为：
    ```
    block_idx * blockLength
    ```
    其中 [`block_idx`](../../../../../docs/api/SIMD-API/基础API/系统变量访问/GetBlockIdx.md) 为当前核的编号，`blockLength` 为每个核处理的数据长度。

    Add样例的实现流程分为3个步骤：

    **第一步：搬运数据到UB（[Unified Buffer](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)，AI Core内部的向量计算存储，简称UB）**
    
    通过 [DataCopy](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/DataCopy.md) 接口将 [GM](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Global Memory，AI Core外部的大容量存储）上的输入x和y搬运到UB上的xLocal、yLocal中。此搬运由MTE2（Memory Transfer Engine 2，GM→UB搬运引擎）执行，数据流向为 GM → UB。
    
    **第二步：执行向量加法**
    
    对xLocal、yLocal执行 [Add](../../../../../docs/api/SIMD-API/基础API/Memory矢量计算/基础算术/Add.md) 向量加法操作，由Vector（矢量计算单元）执行，计算结果存储在UB上的zLocal中。
    
    **第三步：搬运结果到GM（Global Memory）**
    
    通过DataCopy接口将输出数据从zLocal搬运至GM上的输出z中。此搬运由MTE3（Memory Transfer Engine 3，UB→GM搬运引擎）执行，数据流向为 UB → GM。

    整体数据流向：`GM ──(MTE2)──> UB ──(Vector)──> UB ──(MTE3)──> GM`

    各步骤之间通过 [PipeBarrier](../../../../../docs/api/SIMD-API/基础API/同步控制/核内同步/PipeBarrier(ISASI).md)<PIPE_ALL> 设置流水同步屏障，确保前一阶段数据写入完成后再开始下一阶段读取，保证数据依赖正确。

- 调用实现  
  使用内核调用符<<<>>>调用[核函数](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/核函数.md)。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行
  ```bash
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;    # 编译工程，默认npu模式
  ./demo                           # 执行样例
  ```
  
  使用 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：Ascend 950PR/Ascend 950DT，Atlas A3 训练系列产品/Atlas A3 推理系列产品，Atlas A2 训练系列产品/Atlas A2 推理系列产品 |

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 功能调试

### printf

该接口提供CPU域/NPU域调试场景下的格式化输出功能。

在算子kernel侧实现代码中需要输出日志信息的地方调用printf接口打印相关内容。

示例如下：

```cpp
AscendC::printf("fmt string %d\n", 0x123);
AscendC::PRINTF("fmt string %d\n", 0x123);
```

> **注意：** printf（PRINTF）接口打印功能会对算子实际运行的性能带来一定影响，通常在调测阶段使用。开发者可以按需通过设置ASCENDC_DUMP=0的方式关闭打印功能。

### DumpTensor

基于算子工程开发的算子，可以使用该接口Dump指定[LocalTensor](../../../../../docs/api/SIMD-API/基础数据结构/LocalTensor/LocalTensor简介.md)的内容。同时支持打印自定义的附加信息（仅支持uint32\_t数据类型的信息），比如打印当前行号等。

在算子kernel侧实现代码中需要打印Tensor数据的地方调用DumpTensor接口打印相关内容。样例如下：

```cpp
// 向量计算: z = x + y
AscendC::Add(zLocal, xLocal, yLocal, DATA_LENGTH);
AscendC::DumpTensor(zLocal, 1, 16);
```

> **注意：** DumpTensor接口打印功能会对算子实际运行的性能带来一定影响，通常在调测阶段使用。开发者可以按需通过设置ASCENDC_DUMP=0来关闭打印功能。

## 性能调试

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo   # 分析性能
```

当前目录下会生成PROF_前缀的文件夹，`mindstudio_profiler_output`目录保存Host和各个Device的性能数据汇总，性能数据分析推荐查看该目录下文件

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存Host和各个Device的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```
查看具体的性能分析结果：
```
# 查看Task Duration 以及各项数据
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
