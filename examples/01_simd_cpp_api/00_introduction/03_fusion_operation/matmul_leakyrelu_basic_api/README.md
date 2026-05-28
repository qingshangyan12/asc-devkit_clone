# Matmul和LeakyRelu融合计算样例

## 概述

本样例基于静态Tensor编程模式实现Matmul和LeakyRelu融合计算，展示Cube单元和Vector单元协同计算的编程模式。硬件Cube:Vector核数比例为1:2，Cube核完成矩阵乘计算，Vector核完成LeakyRelu激活计算。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── matmul_leakyrelu_basic_api
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── matmul_leakyrelu_basic_api.asc  // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：
  实现Matmul和LeakyRelu融合计算，计算公式如下：

  Matmul计算：
  $$
  C = A \times B
  $$

  LeakyRelu计算：
  $$
  C = \begin{cases}
  C & \text{if } C \geq 0 \\
  C \times 0.001 & \text{if } C < 0
  \end{cases}
  $$

  其中，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]；C为输出矩阵，形状为[M, N]。

- 样例规格：
  本样例参数M = 512, K = 512, N = 1024，调用4个Cube核和8个Vector核完成计算，输入规格如下表所示：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul+LeakyRelu融合</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">A（左矩阵）</td><td align="center">[512, 512]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">B（右矩阵）</td><td align="center">[512, 1024]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[512, 1024]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_vec_custom</td></tr>
  </table>

  **硬件约束**：
  - baseM/baseK/baseN必须满足16对齐，因为Cube计算单元以16×16为最小计算块
  - singleCoreM/singleCoreK/singleCoreN必须能被baseM/baseK/baseN整除，否则单核内无法完整切分计算任务
  - M/K/N必须能被singleCoreM/singleCoreK/singleCoreN整除，不支持非整切场景，否则会有部分数据无法被计算覆盖
  - Vector核数必须是Cube核数的2倍，由[`__mix__(1, 2)`](../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)调用符约束

  **分核逻辑**

  输出矩阵C按M和N方向分块，共分为4个分块，每个分块由1个Cube核完成Matmul计算，每个Cube核的计算结果由2个Vector核协同完成LeakyRelu计算。
  分核参数说明：
  - M方向分块数：M / singleCoreM = 512 / 256 = 2
  - N方向分块数：N / singleCoreN = 1024 / 512 = 2
  - 总Cube核数：2 × 2 = 4
  - 总Vector核数：4 × 2 = 8（Cube:Vector = 1:2）
  - 每个Cube核负责singleCoreM\*singleCoreN的Matmul计算，每次计算产生baseM\*baseN的结果
  - 每个Vector核负责singleCoreM/2\*singleCoreN的计算，每次计算产生baseM/2\*baseN的结果

- 样例实现：
  - 整体计算流程：

    样例整体流程如下（展示Cube核和Vector核协同计算）：

**Cube核：Matmul矩阵乘**
```
GM(A:ND,half) -> L1(A:Nz,half) -> L0A(A:Nz,half) -
             │                  │                 │
          DataCopy            LoadData            │
          ND->Nz              Nz->Nz              │
                                                  │--->L0C(Nz,float) -> GM(C:ND,half)
                                                  │  │                  │
                                                  │ Mmad             Fixpipe
                                                  │ C=A×B       Nz->ND, float32->half
                                                  │
GM(B:ND,half) -> L1(B:Nz,half) -> L0B(B:Zn,half) -
             │                  │
          DataCopy            LoadData
          MTE2搬运            MTE1搬运
          ND->Nz              Nz->Zn(转置)
```

  > **注意**：L0A分型在不同产品上有差异：
  > - Ascend 950PR/Ascend 950DT产品：L0A的分型为Nz
  > - Atlas A2/A3系列产品：L0A的分型为Zz

**核间同步**
  ```
  Cube核产生计算结果 -> CrossCoreSetFlag -> Vector核等待(CrossCoreWaitFlag)
  ```

  **Vector核（1个cube核的结果传递给2个vector核完成计算）：LeakyRelu激活**
  ```
  GM(C:ND,half) -> UB(VECCALC,half) -> UB(VECCALC,half) -> GM(C:ND,half)
                │                   │                   │
            DataCopyPad          LeakyRelu          DataCopyPad
      MTE2搬运（baseM/2×baseN）  VEC计算           MTE3写出

  ```

  **流程详解**：

  1. **Cube核计算阶段**：
      - **GM → L1**：使用[DataCopy](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/随路转换ND2NZ搬运.md)将A、B矩阵从[GM（Global Memory）](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)搬运到[L1](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)，由MTE2（Memory Transfer Engine 2）引擎执行搬运，同时完成[ND](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)到[Nz](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)格式转换。ND格式是用户侧的自然矩阵排布，Nz格式是Cube计算单元要求的分块排布，格式转换是为了适配硬件计算需求。
      - **L1 → L0A/L0B**：使用[LoadData](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadData/LoadData.md)将数据从L1搬运到[L0A](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)和[L0B](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)，由MTE1（Memory Transfer Engine 1）引擎执行搬运。B矩阵需要转置（Nz→Zn），因为Cube计算单元要求左矩阵按行优先（Nz）、右矩阵按列优先（Zn）排布。
      - **L0A/L0B → L0C**：使用[Mmad](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算/Mmad.md)执行矩阵乘加，由Cube引擎执行计算，结果累加到[L0C](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)。K轴方向的所有数据块通过循环累加（`cmatrixInitVal`在首次迭代时初始化L0C为0，后续迭代累加），这样分块计算才能得到完整的矩阵乘结果。
      - **L0C → GM**：使用[Fixpipe](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)将结果从L0C搬到GM，由FIX引擎执行搬运，同时完成Nz到ND格式转换和float32到half类型转换。因为Matmul在L0C上以float32累加以保证精度，而输出要求half类型，所以需要在搬出时进行类型转换。

  2. **核间同步**：
      - Cube核完成一个baseM×baseN的Matmul结果后，通过[CrossCoreSetFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md)设置标志，通知Vector核数据已就绪
      - Vector核通过[CrossCoreWaitFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md)等待标志，确保Matmul计算完成后才开始LeakyRelu，避免读取到未完成的中间结果

  3. **Vector核计算阶段**：
      - **GM → UB**：使用[DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)将Matmul结果从GM搬运到[UB（Unified Buffer）](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)，由MTE2引擎执行搬入。每个Vector核处理baseM/2×baseN的数据，DataCopyPad支持非对齐场景的搬运，自动补齐不足部分。搬运完成后通过[SetFlag/WaitFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核内同步/SetFlag-WaitFlag(ISASI).md)的`MTE2_V`事件同步，确保数据搬入完成后再触发Vector计算，否则Vector可能读取到未搬入完成的数据。
      - **UB计算**：使用[LeakyRelu](../../../../../docs/api/SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md)执行激活计算，由Vector引擎执行，负值部分乘以0.001。计算完成后通过`V_MTE3`事件同步，确保计算完成后再触发数据搬出。
      - **UB → GM**：使用DataCopyPad将结果从UB写回GM，由MTE3（Memory Transfer Engine 3）引擎执行写出。

  4. **核数比例**：
      - Cube:Vector核数比例为1:2，每个Cube核产生baseM×baseN的结果
      - 2个Vector核各处理baseM/2×baseN的数据，共同完成一个Matmul块的激活计算
      - 这样分工是因为Vector核的计算速度通常快于Cube核，2个Vector核正好可以跟上1个Cube核的产出速率

  5. **事件同步机制**：
      - 本样例在Cube核计算阶段使用[SetFlag/WaitFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核内同步/SetFlag-WaitFlag(ISASI).md)进行流水线事件同步，包括`MTE2_MTE1`（MTE2搬入完成后MTE1才能开始搬运）、`MTE1_M`（MTE1搬运完成后Cube才能开始计算）、`M_MTE2`（Cube计算完成后MTE2才能开始下一轮搬入）、`M_FIX`（Cube计算完成后FIX才能开始搬出），确保各引擎间的数据依赖关系正确，避免读写冲突。
      - 在Vector核计算阶段使用`MTE2_V`（MTE2搬入完成后Vector才能开始计算）和`V_MTE3`（Vector计算完成后MTE3才能开始写出），确保数据就绪后再操作。

- Cube与Vector协同机制：
  - **核数比例**：Cube:Vector核数比例为1:2，每个Cube核的计算结果由2个Vector核协同完成LeakyRelu计算
  - **核间同步**：Cube核完成Matmul计算后，通过CrossCoreSetFlag通知Vector核开始计算；Vector核通过CrossCoreWaitFlag等待Cube核完成
  - **数据切分**：每个Vector核处理baseM/2 × baseN大小的数据，两个Vector核共同处理一个baseM × baseN的Matmul结果块

  - 调用实现
    使用[内核调用符](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/核函数.md)`<<<>>>`调用[核函数](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/核函数.md)。核函数是Ascend C算子的入口函数，在AI Core上执行；内核调用符用于配置核函数的执行参数（如核数、stream等）。本样例使用[`__mix__(1, 2)`](../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)混合核调用符，其中参数(1, 2)表示Cube:Vector核数比例为1:2，即每1个Cube核搭配2个Vector核。

  - 数据结构说明
    - [GlobalTensor](../../../../../docs/api/SIMD-API/基础数据结构/GlobalTensor/GlobalTensor简介.md)：用于存放[GM（Global Memory）](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)上的全局数据，核函数通过GlobalTensor来读取外部输入和写入外部输出。通过[SetGlobalBuffer](../../../../../docs/api/SIMD-API/基础数据结构/GlobalTensor/SetGlobalBuffer.md)接口关联GM上的实际数据地址。
    - [LocalTensor](../../../../../docs/api/SIMD-API/基础数据结构/LocalTensor/LocalTensor简介.md)：用于存放AI Core内部Local Memory（如L1、L0A、L0B、L0C、UB）的数据。本样例中Cube核侧通过TPosition（如A1=L1、A2=L0A、B1=L1、B2=L0B、CO1=L0C）指定LocalTensor所在的存储位置，Vector核侧使用VECCALC位置（即UB）存放待计算的向量数据，支持Vector计算单元直接高速访问。

  - 关键变量说明
    - `singleCoreM/singleCoreK/singleCoreN`：单核处理的矩阵分块大小，决定每个核负责的计算范围
    - `baseM/baseK/baseN`：Cube计算单元每次执行的基本块大小，Matmul以baseM×baseK和baseK×baseN的粒度进行一次矩阵乘，在K方向循环累加得到完整结果
    - `CUBE_BLOCK`：Cube计算单元的最小块大小，固定为16，对应16×16的矩阵块
    - `kLoopCount/mLoopCount/nLoopCount`：K/M/N方向的循环次数，由singleCore值除以base值得到，控制分块循环的迭代次数
    - `ASCEND_IS_AIC/ASCEND_IS_AIV`：编译期宏，用于区分当前代码运行在Cube核还是Vector核上，使得Cube和Vector核可以共享同一个Kernel类但执行不同的计算逻辑

## 功能调试

### printf

[printf](../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/printf.md)接口提供CPU域/NPU域调试场景下的格式化输出功能，可在算子kernel侧代码中输出日志信息，辅助调试。

在算子kernel侧实现代码中需要输出日志信息的地方调用printf接口打印相关内容。

示例如下：

```cpp
#include "kernel_operator.h"
AscendC::printf("fmt string %d\n", 0x123);
AscendC::PRINTF("fmt string %d\n", 0x123);
```

> **注意：** printf接口仅在CPU调试模式（`-DCMAKE_ASC_RUN_MODE=cpu`）和NPU仿真模式（`-DCMAKE_ASC_RUN_MODE=sim`）下生效，NPU运行模式下不支持。

### DumpTensor

[DumpTensor](../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/DumpTensor.md)接口用于Dump指定Tensor的内容，支持打印自定义的附加信息（仅支持uint32\_t数据类型），比如打印当前行号等。

在算子kernel侧实现代码中需要打印Tensor数据的地方调用DumpTensor接口打印相关内容。样例如下：

```cpp
// Cube核计算完成后，Dump L0C结果查看Matmul计算是否正确
AscendC::Mmad(cLocal, a2Local, b2Local, mmadParams);
AscendC::DumpTensor(cLocal, 1, 16);

// Vector核LeakyRelu计算完成后，Dump UB结果查看激活计算是否正确
AscendC::LeakyRelu(vecLocal, vecLocal, (half)0.001, tileLen);
AscendC::DumpTensor(vecLocal, 2, 16);
```

打印示例如下：

```
DumpTensor: desc=1, addr=0x1000, data_type=float32, position=L0C, dump_size=16
[1.015015, 2.238228, 3.882874, 4.875984, 5.287174, 6.760258, 7.856659, 8.620880, 9.610844, 10.915154, 11.371704, 12.811401, 13.124611, 14.990570, 15.930355, 16.091831]
```

> **注意：** DumpTensor接口仅在CPU调试模式和NPU仿真模式下生效，NPU运行模式下不支持。

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
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项　　　　　 | 可选值　　　　　　　　　　　| 说明　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真　　　　　　　　　　　　　　　　　　　　　　　　 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 性能调试

### msProf工具介绍

[msProf](../../../../../docs/guide/编程指南/调试调优/性能调优.md)是单算子性能分析工具，包含`msprof op`和`msprof op simulator`两种使用方式，协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

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

更多msProf工具使用方式请参考[性能调优](../../../../../docs/guide/编程指南/调试调优/性能调优.md)和[MindStudio工具](https://www.hiascend.com/document/redirect/CannCommercialToolOpDev)算子调优（msProf）中的内容。
