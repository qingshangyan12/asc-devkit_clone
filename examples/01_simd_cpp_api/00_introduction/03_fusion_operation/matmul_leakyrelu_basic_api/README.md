# Matmul和LeakyRelu融合计算样例

## 概述

本样例基于[Tensor的C++编程](../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的C++编程/基于Tensor的C++编程.md)模式实现Matmul和LeakyRelu融合计算，展示[Cube](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)单元（矩阵计算单元）和[Vector](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/抽象硬件架构.md)单元（矢量计算单元）协同计算的编程模式。Cube核完成矩阵乘计算，Vector核完成LeakyRelu激活计算。

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

  LeakyRelu计算：
  $$
  C = \begin{cases}
  C & \text{if } C \geq 0 \\
  C \times 0.001 & \text{if } C < 0
  \end{cases}
  $$

  其中，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]；C为输出矩阵，形状为[M, N]。

- 样例规格：

  本样例参数M = 512, K = 128, N = 128，调用2个Cube核和4个Vector核完成计算，输入规格如下表所示：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul+LeakyRelu融合</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">A（左矩阵）</td><td align="center">[512, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">B（右矩阵）</td><td align="center">[128, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[512, 128]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_vec_custom</td></tr>
  </table>

- 样例实现：

  **硬件约束**：
  - **baseM/baseK/baseN**（单次Cube计算子块大小）必须满足16对齐：因为Cube计算单元单次计算的最小粒度为16×16，所以子块大小必须是16的倍数
  - **singleCoreM/singleCoreK/singleCoreN**（单核处理的矩阵分块大小）必须能被M/K/N整除：因为整矩阵需均匀切分到各核，若无法整除则会出现部分核多算或越界
  - `__mix__(1,2)`模式下Vector核数必须是Cube核数的2倍：`__mix__(1,2)`表示1个Cube核配2个Vector核的混合模式，核数比例必须匹配
  - Scenario 2（CV融合-UB直通）仅Ascend 950PR/Ascend 950DT支持，A2/A3架构仅支持Scenario 1（CV融合-GM中转）：UB直通依赖L0C→UB的直连硬件通路，该通路仅950PR/950DT具备；A2/A3架构L0C只能先搬回GM再由Vector读取

  **分核逻辑**：

  输出矩阵C按M方向分为2个分块，N方向不分块，使用2个Cube核完成Matmul计算；每个Cube核对应2个Vector核，因此使用4个Vector核完成LeakyRelu计算。每个Cube核产生一个baseM×baseN结果块，对应2个Vector核各处理baseM/2×baseN的数据。
  - M方向分块数：M / singleCoreM = 512 / 256 = 2
  - N方向分块数：N / singleCoreN = 128 / 128 = 1
  - 总Cube核数：2 × 1 = 2
  - 总Vector核数：2 × 2 = 4

  - **整体计算流程**：

    样例整体流程如下（展示Cube核和Vector核协同计算）：

      **1）Cube核：Matmul矩阵乘**
      ```
      GM(A:ND,half) -> L1(A:Nz,half) -> L0A(A:Nz,half) -
                  │                  │                 │
                DataCopy            LoadData            │
                ND->Nz              Nz->Zz/Nz           │
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

      数据流向说明：
      - [DataCopy](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/DataCopy.md)（MTE2引擎）：GM → L1，同时完成[ND→Nz格式转换](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)
      - [LoadData](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadData/LoadData.md)（MTE1引擎）：L1 → L0A/L0B，同时完成Nz→Zz/Nz→Zn格式转换
      - [Mmad](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算/Mmad.md)（Cube引擎）：L0A/L0B → L0C，矩阵乘计算
      - [Fixpipe](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)：L0C → GM，同时完成Nz→ND格式转换和float32→half精度转换

      > **注意**：L0A分型在不同产品上有差异：
      > - Ascend 950PR/Ascend 950DT产品：L0A的分型为Nz
      > - Atlas A2/A3系列产品：L0A的分型为Zz


      **2）核间同步**

      ```
      Cube核:  Fixpipe -> GM(C:ND,half)
                          │
                          │ CrossCoreSetFlag / CrossCoreWaitFlag
                          ▼
      Vector核: GM(C:ND,half) -> UB(VECCALC,half) -> UB(VECCALC,half) -> GM(C:ND,half)
                            │                   │                   │
                        DataCopyPad          LeakyRelu          DataCopyPad
                  MTE2搬运（baseM/2×baseN）    VEC计算            MTE3写出
      ```

      同步机制说明：Cube核完成Fixpipe将结果写入GM后，通过 [CrossCoreSetFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md) 设置标志通知Vector核；Vector核通过 [CrossCoreWaitFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md) 等待该标志，确保Matmul计算完成后才开始LeakyRelu。这样做是因为Vector核依赖Cube核的输出作为输入，必须在Cube核写入完成后才能读取，否则会读到未完成的数据。

      **Vector核（1个Cube核的结果由2个Vector核处理）：LeakyRelu激活**<br>
      上图中，Vector核需要等待对应Cube核完成结果写回后，再读取GM中的Matmul结果并执行 [LeakyRelu](../../../../../docs/api/SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md) 激活。[DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md) 通过MTE2引擎从GM搬运到UB，[LeakyRelu](../../../../../docs/api/SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md) 通过Vector引擎在UB上计算，再通过MTE3引擎将结果从UB写回GM。

      **3）流程详解**：

      1. **Cube核计算阶段**：
          - **LocalTensor创建**：使用[LocalMemAllocator](../../../../../docs/api/SIMD-API/基础API/资源管理/内存管理/LocalMemAllocator/LocalMemAllocator.md)创建A1、B1、A2、B2和CO1位置的[LocalTensor](../../../../../docs/api/SIMD-API/基础数据结构/LocalTensor/LocalTensor简介.md)。其中A、B矩阵在L1中的临时空间由同一个L1 allocator按申请顺序分配，避免手动维护L1地址偏移
          - **GM → L1**：使用DataCopy将A、B矩阵从GM搬运到L1，由MTE2引擎执行，同时完成ND到Nz格式转换。之所以需要格式转换，是因为后续的LoadData和Mmad接口要求输入数据为Nz/Zz/Zn分形格式
          - **L1 → L0A/L0B**：使用LoadData将数据搬运到L0A和L0B，由MTE1引擎执行，B矩阵需要转置（Nz→Zn）。B矩阵转置是因为Mmad接口要求右矩阵以列优先的Zn格式输入
          - **L0A/L0B → L0C**：使用Mmad在Cube引擎上执行矩阵乘加，累加K轴方向的所有数据块，结果存入L0C
          - **L0C → GM**：使用Fixpipe将结果搬出到GM，完成Nz到ND格式转换和float32到half类型转换。格式转换是为了将Cube内部的分形数据恢复为用户可理解的行优先ND格式

      2. **核间同步**：
          - Cube核完成一个baseM×baseN的Matmul结果后，通过CrossCoreSetFlag设置标志
          - Vector核通过CrossCoreWaitFlag等待标志，确保Matmul计算完成后才开始LeakyRelu

      3. **Vector核计算阶段**：
          - **LocalTensor创建**：使用UB allocator创建[VECCALC](../../../../../docs/api/SIMD-API/其他数据类型/TPosition.md)位置的LocalTensor，用于承载当前Vector核处理的半块结果
          - **GM → UB**：使用DataCopyPad将Matmul结果通过MTE2引擎搬运到UB，每个Vector核处理baseM/2×baseN的数据
          - **UB计算**：使用LeakyRelu在Vector引擎上执行激活计算，负值部分乘以0.001
          - **UB → GM**：使用DataCopyPad通过MTE3引擎将结果写回GM，完成融合计算

  - **调用实现**：
    [核调用符](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/核函数.md)`__mix__(1, 2)`实现Cube和Vector核的协同调用，其中参数`(1, 2)`表示Cube:Vector核数比例为1:2。之所以采用1:2的比例，是因为每个Cube核产生的Matmul结果需要2个Vector核并行处理LeakyRelu，以平衡Cube与Vector的计算吞吐。

- **接口参数说明**

  以下结构体均以花括号`{}`方式传参，各字段含义如下（字段顺序与API文档保持一致，实际struct声明中部分字段顺序可能不同）：

  **`AscendC::Nd2NzParams`** — [DataCopy](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/DataCopy.md)接口使用，描述[ND→Nz格式转换](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)参数：
  ```cpp
  struct Nd2NzParams {
      int32_t  ndNum;              // 传输ND矩阵的数目，[0, 4095]
      uint16_t nValue;             // ND矩阵的行数，[0, 16384]
      int32_t  dValue;             // ND矩阵的列数，[0, 65535]
      int32_t  srcNdMatrixStride;  // 相邻ND矩阵起始地址偏移，单位：元素，[0, 65535]
      int32_t  srcDValue;          // 同一ND矩阵相邻行偏移，单位：元素，[1, 65535]
      uint16_t dstNzC0Stride;      // 目的Nz中同源行转换后多行相邻偏移，单位：C0_SIZE(32B)，[1, 16384]
      uint16_t dstNzNStride;       // 目的Nz中Z型矩阵相邻行偏移，单位：C0_SIZE(32B)，[1, 16384]
      int32_t  dstNzMatrixStride;  // 目的Nz中相邻Nz矩阵起始地址偏移，单位：元素，[1, 65535]
  };
  ```
  例如搬运A矩阵时`{1, baseM, baseK, 0, K, baseM, 1, 0}`，将baseM×baseK的ND数据转为Nz格式。

  **`AscendC::LoadData2DParams`** — [LoadData](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadData/LoadData.md)接口使用，描述Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品中A矩阵L1到L0A和B矩阵L1到L0B的数据搬运参数：
  ```cpp
  struct LoadData2DParams {
      int32_t startIndex;   // 分形矩阵ID（0为第1个），单位：512B，[0, 65535]
      int32_t repeatTimes;  // 迭代次数，每个迭代处理512B，[1, 255]
      int32_t srcStride;    // 相邻迭代源分形起始地址间隔，单位：512B，[0, 65535]
      int32_t sid;          // 预留，配置为0
      int32_t dstGap;       // 目的端相邻迭代分形间隔，单位：512B，[0, 65535]
      bool    ifTranspose;  // 是否转置每个分形，默认false
      bool    addrMode;     // 地址更新方式，false=递增，true=递减，默认false
  };
  ```
  例如：Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品中，L0A上的排布格式为Zz，搬运A矩阵时`{0, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, 0, 0, false, 0}`；<br>
  搬运B矩阵时`ifTranspose=true`，完成Nz到Zn的转置搬运。

  **`AscendC::LoadData2DParamsV2`** — [LoadData](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadData/LoadData.md)接口使用，描述Ascend 950PR/Ascend 950DT产品中A矩阵L1到L0A和B矩阵L1到L0B的数据搬运参数：
  ```cpp
  struct LoadData2DParamsV2 {
      uint32_t mStartPosition;  // M方向起始位置，单位：512B
      uint32_t kStartPosition;  // K方向起始位置，单位：512B
      uint16_t mStep;           // M方向搬运分形数
      uint16_t kStep;           // K方向搬运分形数
      int32_t  srcStride;       // 源端相邻K方向分形间隔，单位：512B
      uint16_t dstStride;       // 目的端相邻K方向分形间隔，单位：512B
      bool     ifTranspose;     // 是否转置每个分形，默认false
      uint8_t  sid;             // 预留，配置为0
  };
  ```
  Ascend 950PR/Ascend 950DT产品中，L0A上的排布格式为Nz，搬运A矩阵时使用`{0, 0, baseM / CUBE_BLOCK, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, baseM / CUBE_BLOCK, false, 0}`，一次完成A矩阵Nz到Nz搬运；搬运B矩阵时使用`{0, 0, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, true, 0}`，一次完成B矩阵Nz到Zn搬运。

  **`AscendC::MmadParams`** — [Mmad](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算/Mmad.md)接口使用，描述矩阵乘参数：
  ```cpp
  struct MmadParams {
      uint16_t m;               // 左矩阵Height（M维），[0, 4095]
      uint16_t n;               // 右矩阵Width（N维），[0, 4095]
      uint16_t k;               // 左矩阵Width/右矩阵Height（K维），[0, 4095]
      uint16_t unitFlag;        // Mmad与Fixpipe细粒度并行控制，默认0
      bool     cmatrixSource;   // C矩阵初始值来源，false=CO1，true=C2，默认false
      bool     cmatrixInitVal;  // C矩阵初始值是否为0，默认true
  };
  ```
  例如`{baseM, baseN, baseK, 0, false, true}`，计算baseM×baseN输出块并在K方向累加baseK长度。

  **`AscendC::FixpipeParamsV220`** — [Fixpipe](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)接口使用，描述L0C到GM的数据搬运和精度转换参数：
  ```cpp
  struct FixpipeParamsV220 {
      int32_t     nSize;        // 源Nz矩阵N方向大小，[1, 4095]
      uint16_t    mSize;        // 源Nz矩阵M方向大小（Nz2ND时[1, 8192]）
      uint16_t    srcStride;    // 源Nz相邻Z排布起始偏移，单位：C0_SIZE，[0, 65535]
      int32_t     dstStride;    // Nz2ND时目的ND矩阵每行元素数，单位：element
      bool        reluEn;       // 是否使能ReLU
      QuantMode_t quantPre;     // 量化模式，F322F16表示float→half
      uint64_t    deqScalar;    // scalar量化参数，单个scale值
      int32_t     ndNum;        // 源Nz矩阵数目，[1, 65535]
      int32_t     srcNdStride;  // 不同Nz矩阵起始地址间隔，单位：16×C0_SIZE，[1, 512]
      int32_t     dstNdStride;  // 目的相邻ND矩阵偏移，单位：element，[1, 65535]
      int32_t     unitFlag;     // Mmad与Fixpipe并行控制
  };
  ```
  例如`{baseN, baseM, baseM, N, false, F322F16, 0, 1, 0, 0, 0}`，将L0C中的baseM×baseN float32结果转为half并写回GM。

  **`AscendC::DataCopyExtParams`** — [DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)接口使用，描述GM与UB之间按块搬运的参数：
  ```cpp
  struct DataCopyExtParams {
      uint16_t blockCount;  // 连续传输数据块个数
      uint32_t blockLen;    // 每个数据块长度，单位：Byte
      uint32_t srcStride;   // 源端相邻数据块间隔，单位：Byte
      uint32_t dstStride;   // 目的端相邻数据块间隔，单位：Byte
      uint32_t rsv;         // 预留字段，配置为0
  };
  ```
  例如GM搬运到UB时`{static_cast<uint16_t>(baseM / 2), blockLen, srcStride, 0, 0}`，每个Vector核读取baseM/2行结果。

  **`AscendC::DataCopyPadExtParams<half>`** — [DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)接口使用，描述尾块补齐参数：
  ```cpp
  template <typename T>
  struct DataCopyPadExtParams {
      bool isPad;       // 是否开启补齐
      uint8_t leftPad;  // 左侧补齐长度
      uint8_t rightPad; // 右侧补齐长度
      T paddingValue;   // 补齐值
  };
  ```
  例如`{true, 0, 0, 0}`表示开启补齐能力，但左右两侧均不额外补齐。

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
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：Ascend 950PR/Ascend 950DT，Atlas A3 训练系列产品/Atlas A3 推理系列产品，Atlas A2 训练系列产品/Atlas A2 推理系列产品 |

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
AscendC::printf("matmul blockIdx=%d\n", AscendC::GetBlockIdx());
```

> **注意：** printf（PRINTF）接口打印功能会对算子实际运行的性能带来一定影响，通常在调测阶段使用。开发者可以按需通过设置ASCENDC_DUMP=0的方式关闭打印功能。

### DumpTensor

基于算子工程开发的算子，可以使用该接口Dump指定[LocalTensor](../../../../../docs/api/SIMD-API/基础数据结构/LocalTensor/LocalTensor简介.md)的内容。同时支持打印自定义的附加信息（仅支持uint32\_t数据类型的信息），比如打印当前行号等。

在算子kernel侧实现代码中需要打印Tensor数据的地方调用DumpTensor接口打印相关内容。样例如下：

```cpp
// Cube核L0C结果输出
AscendC::Fixpipe(...);
AscendC::DumpTensor(cLocal, baseM * baseN);
```

```cpp
// Vector核LeakyRelu结果
AscendC::LeakyRelu(zLocal, xLocal, ..., len);
AscendC::DumpTensor(zLocal, 0, 16);
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
