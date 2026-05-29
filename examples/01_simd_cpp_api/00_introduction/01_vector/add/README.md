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
    其中`block_idx`为当前核的索引号，用于多核场景下标识核的身份，详见[GetBlockIdx接口](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/基础API/系统变量访问/GetBlockIdx.md)；`blockLength`为每个核处理的元素个数，本样例中为2048。

    Add样例的实现流程分为3个步骤：

    **第一步：搬运数据到UB（Unified Buffer）**
    
    将[GM（Global Memory）](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/高级编程/硬件实现/基本架构.md)上的输入x和y搬运到[UB（Unified Buffer）](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/高级编程/硬件实现/基本架构.md)上的xLocal、yLocal中。GM是AI Core外部的大容量存储，UB是AI Core内部供Vector计算单元使用的高速缓存。数据搬运通过[DataCopy](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/DataCopy简介.md)接口完成。
    
    **第二步：执行向量加法**
    
    对xLocal、yLocal执行加法操作，计算结果存储在UB上的zLocal中。向量加法通过[Add](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/基础API/Memory矢量计算/基础算术/Add.md)接口完成。步骤之间通过[PipeBarrier](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/基础API/同步控制/核内同步/PipeBarrier(ISASI).md)进行流水同步，确保前序指令全部完成后再执行后续指令。

    **第三步：搬运结果到GM（Global Memory）**
    
    将输出数据从zLocal搬运至GM上的输出z中。

  - 调用实现  
    使用[内核调用符](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/编程模型/AI-Core-SIMD编程/核函数.md)`<<<>>>`调用[核函数](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/编程模型/AI-Core-SIMD编程/核函数.md)。核函数是Ascend C算子的入口函数，在AI Core上执行；内核调用符用于配置核函数的执行参数（如核数、stream等）。

  - 数据结构说明  
    - [LocalTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/基础数据结构/LocalTensor/LocalTensor简介.md)：用于存放AI Core内部Local Memory（如UB）的数据，支持向量计算单元直接访问。
    - [GlobalTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/基础数据结构/GlobalTensor/GlobalTensor简介.md)：用于存放Global Memory上的全局数据，核函数通过GlobalTensor来读取外部输入和写入外部输出。

## 功能调试

### printf

[printf](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/基础API/调试接口/上板打印/printf.md)接口提供CPU域/NPU域调试场景下的格式化输出功能，可在算子kernel侧代码中输出日志信息，辅助调试。

在算子kernel侧实现代码中需要输出日志信息的地方调用printf接口打印相关内容。

示例如下：

```cpp
#include "kernel_operator.h"
AscendC::printf("fmt string %d\n", 0x123);
AscendC::PRINTF("fmt string %d\n", 0x123);
```

### DumpTensor

[DumpTensor](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMD-API/基础API/调试接口/上板打印/DumpTensor.md)接口用于Dump指定Tensor的内容，支持打印自定义的附加信息（仅支持uint32\_t数据类型），比如打印当前行号等。

在算子kernel侧实现代码中需要打印Tensor数据的地方调用DumpTensor接口打印相关内容。样例如下：

```cpp
// 向量计算: z = x + y
AscendC::Add(zLocal, xLocal, yLocal, DATA_LENGTH);
AscendC::PipeBarrier<PIPE_ALL>();
AscendC::DumpTensor(zLocal, 1, 16);
```

打印示例如下：

```
DumpTensor: desc=1, addr=0x4000, data_type=float32, position=UB, dump_size=16
[0.000000, 0.300000, 0.600000, 0.900000, 1.200000, 1.500000, 1.800000, 2.100000, 2.400000, 2.700000, 3.000000, 3.300000, 3.600000, 3.900000, 4.200000, 4.500000]
```

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
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：dav-2201对应Atlas A2/A3系列，dav-3510对应Ascend 950PR/950DT |

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 性能调试

### msProf工具介绍

[msProf](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/调试调优/性能调优.md)是单算子性能分析工具，包含`msprof op`和`msprof op simulator`两种使用方式，协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

- 上板性能采集

  通过上板性能采集，可以直接测定算子在昇腾AI处理器上的运行时间。该方式适合在板环境中快速定位算子性能问题。

  基于可执行文件demo通过msprof op执行算子调优：
  ```bash
  msprof op ./demo
  ```

  - 性能数据说明  
    命令完成后，会在默认目录下生成以"OPPROF\_{timestamp}\_XXX"命名的文件夹，性能数据文件夹结构示例如下：

    ```bash
    ├──dump                       # 原始的性能数据，用户无需关注
    ├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
    ├──L2Cache.csv                # L2 Cache命中率，影响MTE2，建议合理规划数据搬运逻辑，增加命中率
    ├──Memory.csv                 # UB，L1和主存储器读写带宽速率
    ├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
    ├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
    ├──OpBasicInfo.csv            # 算子基础信息
    ├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
    ├──ResourceConflictRatio.csv  # UB上的 bank group、bank conflict和资源冲突率在所有指令中的占比
    └──visualize_data.bin         # MindStudio Insight呈现文件
    ```

更多msProf工具使用方式请参考[性能调优](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/调试调优/性能调优.md)和[MindStudio工具](https://www.hiascend.com/document/redirect/CannCommercialToolOpDev)算子调优（msProf）中的内容。
