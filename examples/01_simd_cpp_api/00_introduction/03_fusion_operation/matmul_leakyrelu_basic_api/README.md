# Matmul和LeakyRelu融合计算样例

## 概述

本样例基于[静态Tensor编程模式](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的C++编程/静态Tensor编程.md)（编译期确定Tensor的shape和数据类型，由开发者自行管理内存分配和数据搬运）实现Matmul和LeakyRelu融合计算，展示Cube单元（AI Core上的矩阵计算单元，负责矩阵乘加等密集计算）和Vector单元（AI Core上的向量计算单元，负责向量运算和数据搬运）协同计算的编程模式。硬件Cube:Vector核数比例为1:2，Cube核完成矩阵乘计算，Vector核完成LeakyRelu激活计算。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── matmul_leakyrelu_basic_api
│   ├── scripts
│   │   ├── gen_data.py                 // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py            // 真值对比文件
│   ├── CMakeLists.txt                  // 编译工程文件
│   ├── data_utils.h                    // 数据读入写出函数
│   └── matmul_leakyrelu_basic_api.asc  // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：
  实现Matmul和LeakyRelu融合计算，计算公式如下：

  Matmul计算：
  $$
  C = A \times B
  $$

  [LeakyRelu](../../../../../docs/api/SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md)（带泄露的ReLU激活函数，正值保持不变，负值乘以一个小的斜率因子）计算：
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
  <tr><td align="center">A（左矩阵）</td><td align="center">[512, 512]</td><td align="center">[half](../../../../../docs/api/SIMD-API/基础数据结构/内置数据类型.md)（半精度浮点，float16，2字节）</td><td align="center">[ND](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)（标准N维数据排布格式）</td></tr>
  <tr><td align="center">B（右矩阵）</td><td align="center">[512, 1024]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[512, 1024]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_vec_custom</td></tr>
  </table>

  **分核逻辑**

  输出矩阵C按M和N方向分块，共分为4个分块，每个分块由1个Cube核完成Matmul计算，每个Cube核的计算结果由2个Vector核协同完成LeakyRelu计算。
  分核参数说明：
  - M方向分块数：M / singleCoreM = 512 / 256 = 2
  - N方向分块数：N / singleCoreN = 1024 / 512 = 2
  - 总Cube核数：2 × 2 = 4
  - 总Vector核数：4 × 2 = 8（Cube:Vector = 1:2）
  - 每个Cube核负责singleCoreM\*singleCoreN的Matmul计算，每次计算产生baseM\*baseN的结果
  - 每个Vector核负责singleCoreM/2*singleCoreN的计算，每次计算产生baseM/2\*baseN的结果

  > 其中，**singleCoreM/singleCoreK/singleCoreN** 为单个Cube核负责计算的总矩阵块大小，整个矩阵按该尺寸切分到多个Cube核；**baseM/baseK/baseN** 为Cube核单次[Mmad](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算/Mmad.md)运算处理的基本分块大小，需满足16对齐。

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
          ND->Nz              Nz->Zn(转置)
```

  > **注意**：L0A分型在不同产品上有差异：
  > - Ascend 950PR/Ascend 950DT产品：L0A的分型为[Nz格式](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)（针对Cube计算优化的分块数据排布格式）
  > - Atlas A2/A3系列产品：L0A的分型为Zz格式（Nz格式的另一种变体，分块方式不同）

  > 流程图中涉及的存储层级：**GM**（Global Memory，全局内存/HBM）、**L1**（Cube核片内缓冲区）、**L0A/L0B**（Cube核矩阵输入缓冲区）、**L0C**（Cube核矩阵输出缓冲区）、**UB**（Unified Buffer，Vector核统一缓冲区）。

**核间同步**
  ```
  Cube核产生计算结果 -> CrossCoreSetFlag -> Vector核等待(CrossCoreWaitFlag)
  ```
  [CrossCoreSetFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md) / [CrossCoreWaitFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md) 为核间同步接口，Cube核通过SetFlag设置标志通知Vector核，Vector核通过WaitFlag等待标志，确保Cube计算完成后再开始Vector计算。

  **Vector核（1个cube核的结果结果传递给2个vector核完成计算）：LeakyRelu激活**
  ```
  GM(C:ND,half) -> UB(VECCALC,half) -> UB(VECCALC,half) -> GM(C:ND,half)
                │                   │                   │
            DataCopyPad          LeakyRelu          DataCopyPad
      MTE2搬运（baseM/2×baseN）    VEC计算            MTE3写出

  ```

  > **VECCALC** 为Vector核在UB中的计算存储区域；**MTE2**（Memory Transfer Engine 2）为GM→UB的搬运指令类型；**MTE3**（Memory Transfer Engine 3）为UB→GM的搬运指令类型。

  **流程详解**：

  1. **Cube核计算阶段**：
      - **GM → L1**：使用[DataCopy](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/DataCopy.md)（数据搬运接口，用于在不同存储层级间搬运数据）将A、B矩阵从GM搬运到L1，完成[ND](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)到[Nz](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)格式转换
      - **L1 → L0A/L0B**：使用[LoadData](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadData/LoadData.md)（数据加载接口，将L1数据搬运到L0A/L0B，支持格式转换）将数据搬运到L0A和L0B，B矩阵需要转置（Nz→Zn，Zn为Nz的转置变体，用于右矩阵在L0B中的转置读取）
      - **L0A/L0B → L0C**：使用[Mmad](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算/Mmad.md)（矩阵乘加接口，执行 \(C = A \times B\) 计算）执行矩阵乘加，累加K轴方向的所有数据块
      - **L0C → GM**：使用[Fixpipe](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)（矩阵结果搬出接口，将L0C结果从Nz格式转为ND格式并写回GM，同时支持数据类型转换）将结果搬出到GM，完成Nz到ND格式转换和float32到half类型转换

  2. **核间同步**：
      - Cube核完成一个baseM×baseN的Matmul结果后，通过[CrossCoreSetFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md)设置标志
      - Vector核通过[CrossCoreWaitFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md)等待标志，确保Matmul计算完成后才开始LeakyRelu

  3. **Vector核计算阶段**：
      - **GM → UB**：使用[DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)（带Padding的数据搬运接口，与DataCopy类似，但支持对不满足对齐要求的数据进行补齐）将Matmul结果搬运到UB，每个Vector核处理baseM/2×baseN的数据
      - **UB计算**：使用[LeakyRelu](../../../../../docs/api/SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md)执行激活计算，负值部分乘以0.001
      - **UB → GM**：使用DataCopyPad将结果写回GM，完成融合计算

  4. **核数比例**：
      - Cube:Vector核数比例为1:2，每个Cube核产生baseM×baseN的结果
      - 2个Vector核各处理baseM/2×baseN的数据，共同完成一个Matmul块的激活计算

- Cube与Vector协同机制：
  - **核数比例**：Cube:Vector核数比例为1:2，每个Cube核的计算结果由2个Vector核协同完成LeakyRelu计算
  - **核间同步**：Cube核完成Matmul计算后，通过CrossCoreSetFlag通知Vector核开始计算；Vector核通过CrossCoreWaitFlag等待Cube核完成
  - **数据切分**：每个Vector核处理baseM/2 × baseN大小的数据，两个Vector核共同处理一个baseM × baseN的Matmul结果块

  - 约束条件：
    1. baseM/baseK/baseN满足16对齐
    2. baseM/baseK/baseN能被singleCoreM/singleCoreK/singleCoreN整除
    3. singleCoreM/singleCoreK/singleCoreN能被M/K/N整除，不支持非整切场景
    4. Vector核数是Cube核数的2倍

  - 调用实现
    核调用符[`__mix__(1, 2)`](../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)（用于声明Cube核与Vector核的协同调用，参数(1, 2)表示Cube:Vector核数比例为1:2）实现Cube和Vector核的协同调用。

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

## 功能调试

### printf

[printf](../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/printf.md)（提供CPU域/NPU域调试场景下的格式化输出功能）可在算子kernel侧实现代码中需要输出日志信息的地方调用。

**使用方式**：

```cpp
#include "kernel_operator.h"
AscendC::printf("fmt string %d\n", 0x123);
AscendC::PRINTF("fmt string %d\n", 0x123);
```

**说明**：
- 支持格式控制符：`%d`、`%f`、`%x`、`%s`、`%u`、`%p`
- 单次打印数据量不超过1MB
- CPU调试模式和NPU上板模式均可使用

> 详细接口说明请参考[printf接口文档](../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/printf.md)和[NPU域上板调试](../../../../../docs/guide/编程指南/调试调优/功能调试/NPU域上板调试.md)。

### DumpTensor

[DumpTensor](../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/DumpTensor.md)（用于Dump指定Tensor内容的接口，支持打印自定义附加信息）仅支持NPU上板调试场景。

**使用方式**：

```cpp
// LeakyRelu计算完成后，Dump输出结果
AscendC::LeakyRelu(zLocal, xLocal, (half)0.001, calcSize);
AscendC::DumpTensor(zLocal, 1, 16);
```

**输出示例**：

```
DumpTensor: desc=1, addr=0x1000, data_type=float16, position=UB, dump_size=16
[0.000000, 1.523438, -0.001526, 3.875000, ...]
```

**说明**：
- `desc`：自定义附加信息，仅支持`uint32_t`类型
- `dumpSize`：Dump的元素个数
- 单次数据量不超过1MB
- 不支持入图场景

> 详细接口说明请参考[DumpTensor接口文档](../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/DumpTensor.md)和[NPU域上板调试](../../../../../docs/guide/编程指南/调试调优/功能调试/NPU域上板调试.md)。

---

## 性能调试

### msprof op 上板性能采集

[msprof op](../../../../../docs/guide/编程指南/调试调优/性能调优.md)（单算子上板性能分析工具，可直接测定算子在昇腾AI处理器上的运行时间）适合在板环境中快速定位算子性能问题。

**使用方式**：

```bash
msprof op ./demo   # 上板性能采集
```

命令完成后，会在当前目录下生成以`OPPROF_{timestamp}_XXX`命名的文件夹，性能数据文件结构如下：

```bash
OPPROF_{timestamp}_XXX
├── dump                       # 原始的性能数据，用户无需关注
├── ArithmeticUtilization.csv  # cube/vector指令cycle占比
├── L2Cache.csv                # L2 Cache命中率，影响MTE2，建议合理规划数据搬运逻辑，增加命中率
├── Memory.csv                 # UB，L1和主存储器读写带宽速率
├── MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├── MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├── OpBasicInfo.csv            # 算子基础信息
├── PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├── ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
└── visualize_data.bin         # MindStudio Insight呈现文件
```

### msprof 仿真性能采集

[msprof op simulator](../../../../../docs/guide/编程指南/调试调优/性能调优.md)（仿真性能分析工具，可在不依赖NPU硬件的仿真环境中采集算子性能数据）适用于无板环境的性能分析。

**使用方式**：

```bash
msprof op simulator ./demo --soc-version=Ascend920B1   # 仿真性能采集
```

> 更多msprof工具使用方式请参考[性能调优](../../../../../docs/guide/编程指南/调试调优/性能调优.md)和[获取性能数据](../../../../../docs/guide/算子实践参考/性能分析/获取性能数据.md)。
