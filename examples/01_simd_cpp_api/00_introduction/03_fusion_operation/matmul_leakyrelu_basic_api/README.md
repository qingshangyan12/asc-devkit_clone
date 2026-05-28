# Matmul和LeakyRelu融合计算样例

## 概述

本样例基于静态Tensor编程模式实现Matmul和LeakyRelu融合计算，展示[Cube](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（矩阵乘计算单元）和[Vector](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（向量计算单元）协同计算的编程模式。硬件Cube:Vector核数比例为1:2，Cube核完成矩阵乘计算，Vector核完成LeakyRelu激活计算。

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
  本样例参数M = 256, K = 64, N = 256，调用2个Cube核和4个Vector核完成计算，输入规格如下表所示：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul+LeakyRelu融合</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">A（左矩阵）</td><td align="center">[256, 64]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">B（右矩阵）</td><td align="center">[64, 256]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[256, 256]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_vec_custom</td></tr>
  </table>

  **硬件约束条件**：
  - baseM/baseK/baseN须满足16对齐（因为[Cube](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)矩阵计算单元以16×16为最小计算块）
  - baseM/baseK/baseN能被singleCoreM/singleCoreK/singleCoreN整除（保证单个核的计算量可被完整切分）
  - singleCoreM/singleCoreK/singleCoreN能被M/K/N整除，不支持非整切场景（否则会产生无效数据影响计算结果）
  - Vector核数必须是Cube核数的2倍（由[核调用符`__mix__`](../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)的1:2配比约束决定）

  **分核逻辑**：

  输出矩阵C按M方向分块，共分为2个分块，每个分块由1个Cube核完成Matmul计算，每个Cube核的计算结果由2个Vector核协同完成LeakyRelu计算。
  分核参数说明：
  - M方向分块数：M / singleCoreM = 256 / 128 = 2
  - N方向分块数：N / singleCoreN = 256 / 256 = 1
  - 总Cube核数：2 × 1 = 2
  - 总Vector核数：2 × 2 = 4（Cube:Vector = 1:2）
  - 每个Cube核负责singleCoreM\*singleCoreN的Matmul计算，每次计算产生baseM\*baseN的结果
  - 每个Vector核负责singleCoreM/2*singleCoreN的计算，每次计算产生baseM/2\*baseN的结果

- 样例实现：
  - **整体计算流程**：

    样例整体流程如下（展示Cube核和Vector核协同计算）：

      **1）Cube核：Matmul矩阵乘**
      ```
      GM(A:ND,half) -> L1(A:Nz,half) -> L0A(A:Nz,half) -
                  │                  │                  │
                DataCopy            LoadData            │
               (MTE2搬运)          (MTE1搬运)            │
              ND->Nz              Nz->Zz/Nz             │
                                                        │--->L0C(Nz,float) -> GM(C:ND,half)
                                                        │  │                  │
                                                        │ Mmad             Fixpipe
                                                        │(Cube计算)     (FIX流水搬运)
                                                        │ C=A×B       Nz->ND, float32->half
                                                        │
      GM(B:ND,half) -> L1(B:Nz,half) -> L0B(B:Zn,half) -
                  │                  │
                DataCopy            LoadData
              (MTE2搬运)          (MTE1搬运)
              ND->Nz              Nz->Zn(转置)
      ```

      > **注意**：L0A分型在不同产品上有差异：
      > - Ascend 950PR/Ascend 950DT产品：L0A的分型为[Nz](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)（N轴居中的Z型分形排布）
      > - Atlas A2/A3系列产品：L0A的分型为[Zz](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)（Z轴居中的Z型分形排布）

      **2）核间同步**
        ```
        Cube核产生计算结果 -> CrossCoreSetFlag -> Vector核等待(CrossCoreWaitFlag)
        ```

        **Vector核（1个Cube核的结果传递给2个Vector核完成计算）：LeakyRelu激活**
        ```
        GM(C:ND,half) -> UB(VECCALC,half) -> UB(VECCALC,half) -> GM(C:ND,half)
                      │                   │                   │
                  DataCopyPad          LeakyRelu          DataCopyPad
                MTE2搬运              VEC计算            MTE3写出
          (baseM/2×baseN)

        ```

      **3）流程详解**：

      1. **Cube核计算阶段**：
          - **LocalTensor创建**：使用[LocalMemAllocator](../../../../../docs/api/SIMD-API/基础API/资源管理/内存管理/LocalMemAllocator/LocalMemAllocator简介.md)创建A1、B1、A2、B2和CO1位置的[LocalTensor](../../../../../docs/api/SIMD-API/基础数据结构/LocalTensor/LocalTensor简介.md)。其中A、B矩阵在L1中的临时空间由同一个L1 allocator按申请顺序分配，避免手动维护L1地址偏移，因为不同矩阵共享同一片L1存储空间，手动计算偏移容易出错且不利于维护
          - **GM → L1**：使用[DataCopy](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/DataCopy简介.md)将A、B矩阵从[GM](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Global Memory，全局内存）搬运到[L1](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Local Memory 1，Cube侧本地存储），由[MTE2](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Memory Transfer Engine 2，外部存储搬运引擎）执行搬运，同时完成[ND](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)（N-Dimension，标准多维排布）到[Nz](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)（N轴居中的Z型分形排布）格式转换。之所以需要格式转换，是因为Cube计算单元要求数据在L0A/L0B中以分形排布存放，而ND格式无法直接被LoadData正确读取
          - **L1 → L0A/L0B**：使用[LoadData](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadData/Load2D.md)将数据从L1搬运到[L0A](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)和[L0B](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)，由[MTE1](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Memory Transfer Engine 1，内部存储搬运引擎）执行搬运。B矩阵需要转置（Nz→[Zn](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)），因为Mmad接口要求右矩阵以Zn格式存放在L0B中，转置后的排布使得矩阵乘的K维度内积计算可以被Cube单元高效执行
          - **L0A/L0B → L0C**：使用[Mmad](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算/Mmad.md)在[Cube](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)单元上执行矩阵乘加，将结果写入[L0C](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)，累加K轴方向的所有数据块。Mmad是Cube单元的核心计算指令，之所以在L0C中累加K方向，是因为K方向切片的中间结果需要float32精度以保证累加精度，避免half精度下的累加误差
          - **L0C → GM**：使用[Fixpipe](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)将结果从L0C搬出到GM，由[FIX流水](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)执行搬运，同时完成Nz到ND格式转换和float32到half类型转换。之所以在搬运过程中做格式和精度转换，是因为L0C中的结果以Nz分形+float32存放，而最终输出要求ND排布+half类型，Fixpipe可以在一次搬运中完成这两种转换，避免额外的转换开销

      2. **核间同步**：
          - Cube核完成一个baseM×baseN的Matmul结果后，通过[CrossCoreSetFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md)设置标志
          - Vector核通过[CrossCoreWaitFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md)等待标志，确保Matmul计算完成后才开始LeakyRelu。这一同步机制是必须的，因为Cube核和Vector核各自独立执行，若Vector核在Cube核写出结果之前就开始读取，会读到未初始化或部分写入的错误数据

      3. **Vector核计算阶段**：
          - **LocalTensor创建**：使用UB allocator创建`VECCALC`位置的[LocalTensor](../../../../../docs/api/SIMD-API/基础数据结构/LocalTensor/LocalTensor简介.md)，用于承载当前Vector核处理的半块结果。每个Vector核只处理baseM/2×baseN的数据，因此只需分配该大小的[UB](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Unified Buffer，向量计算统一缓冲区）空间
          - **GM → UB**：使用[DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)将Matmul结果从GM搬运到UB，由[MTE2](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)执行搬运，每个Vector核处理baseM/2×baseN的数据。使用DataCopyPad而非普通DataCopy，是因为DataCopyPad支持补齐（padding）能力，当数据长度不满足对齐要求时自动补齐，确保搬运不会因对齐问题出错
          - **UB计算**：使用[LeakyRelu](../../../../../docs/api/SIMD-API/基础API/Memory矢量计算/基础算术/LeakyRelu.md)在[Vector](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)单元上执行激活计算，负值部分乘以0.001。LeakyRelu相比ReLU的优势在于负值区域有非零梯度，避免神经元"死亡"问题
          - **UB → GM**：使用[DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)将结果写回GM，由[MTE3](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Memory Transfer Engine 3，写出搬运引擎）执行写出，完成融合计算

      4. **核数比例**：
          - Cube:Vector核数比例为1:2，每个Cube核产生baseM×baseN的结果
          - 2个Vector核各处理baseM/2×baseN的数据，共同完成一个Matmul块的激活计算

- **Cube与Vector协同机制**：
  - **核数比例**：Cube:Vector核数比例为1:2，每个Cube核的计算结果由2个Vector核协同完成LeakyRelu计算
  - **核间同步**：Cube核完成Matmul计算后，通过[CrossCoreSetFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md)通知Vector核开始计算；Vector核通过[CrossCoreWaitFlag](../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md)等待Cube核完成
  - **数据切分**：每个Vector核处理baseM/2 × baseN大小的数据，两个Vector核共同处理一个baseM × baseN的Matmul结果块

  - **约束条件**：
    1. baseM/baseK/baseN满足16对齐（[Cube](../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)矩阵计算单元最小计算块为16×16）
    2. baseM/baseK/baseN能被singleCoreM/singleCoreK/singleCoreN整除（保证单核计算量完整切分）
    3. singleCoreM/singleCoreK/singleCoreN能被M/K/N整除，不支持非整切场景（非整切会产生无效数据影响计算正确性）
    4. Vector核数是Cube核数的2倍（[核调用符`__mix__`](../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)的1:2配比约束）

    - **调用实现**：
      核调用符[`__mix__(1, 2)`](../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)实现Cube和Vector核的协同调用，其中参数(1, 2)表示Cube:Vector核数比例为1:2，即每个Cube核搭配2个Vector核，硬件会自动按此比例分配物理核。

- **接口参数说明**

  以下结构体均以花括号`{}`方式传参，各字段含义如下（字段顺序与API文档保持一致，实际struct声明中部分字段顺序可能不同）：

  **[`AscendC::Nd2NzParams`](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/随路转换ND2NZ搬运.md)** — [DataCopy](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopy/DataCopy简介.md)接口使用，描述ND→NZ格式转换参数：
  ```cpp
  struct Nd2NzParams {
      int32_t  ndNum;              // 传输ND矩阵的数目，[0, 4095]
      uint16_t nValue;             // ND矩阵的行数，[0, 16384]
      int32_t  dValue;             // ND矩阵的列数，[0, 65535]
      int32_t  srcNdMatrixStride;  // 相邻ND矩阵起始地址偏移，单位：元素，[0, 65535]
      int32_t  srcDValue;          // 同一ND矩阵相邻行偏移，单位：元素，[1, 65535]
      uint16_t dstNzC0Stride;      // 目的NZ中同源行转换后多行相邻偏移，单位：C0_SIZE(32B)，[1, 16384]
      uint16_t dstNzNStride;       // 目的NZ中Z型矩阵相邻行偏移，单位：C0_SIZE(32B)，[1, 16384]
      int32_t  dstNzMatrixStride;  // 目的NZ中相邻NZ矩阵起始地址偏移，单位：元素，[1, 65535]
  };
  ```
  例如搬运A矩阵时`{1, baseM, baseK, 0, K, baseM, 1, 0}`，将baseM×baseK的ND数据转为NZ格式。其中`srcDValue=K`是因为A矩阵在GM中按行连续存储，相邻行起始偏移为K个元素。

  **[`AscendC::LoadData2DParams`](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadData/Load2D.md)** — [LoadData](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadData/Load2D.md)接口使用，描述L1到L0A/L0B的数据搬运参数：
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
  例如：dav-2201架构L0A上的排布格式为[Zz](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)，搬运A矩阵时`{0, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, 0, 0, false, 0}`；<br>
  dav-3510架构L0A上的排布格式为[Nz](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)，搬运A矩阵时`{0, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, 0, (baseM / CUBE_BLOCK - 1), false, 0}`，`dstGap`不同是因为两种架构下L0A分形排布方式不同；<br>
  搬运B矩阵时`ifTranspose=true`，完成Nz到[Zn](../../../../../docs/guide/技术附录/概念原理和术语/神经网络和算子/数据排布格式.md)的转置搬运，转置是为了使B矩阵的K维度行数据在L0B中连续排列，适配Mmad接口对右矩阵的排布要求。

  **[`AscendC::MmadParams`](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算/Mmad.md)** — [Mmad](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算/Mmad.md)接口使用，描述矩阵乘参数：
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
  例如`{baseM, baseN, baseK, 0, false, true}`，计算baseM×baseN输出块并在K方向累加baseK长度。`cmatrixInitVal=true`表示L0C初始值清零，因为这是第一个K分块的累加，不需要保留前次结果；`cmatrixSource=false`表示C矩阵初始值来自CO1位置（L0C的低半部分）。

  **[`AscendC::FixpipeParamsV220`](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)** — [Fixpipe](../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)接口使用，描述L0C到GM的数据搬运和精度转换参数：
  ```cpp
  struct FixpipeParamsV220 {
      int32_t     nSize;        // 源NZ矩阵N方向大小，[1, 4095]
      uint16_t    mSize;        // 源NZ矩阵M方向大小（NZ2ND时[1, 8192]）
      uint16_t    srcStride;    // 源NZ相邻Z排布起始偏移，单位：C0_SIZE，[0, 65535]
      int32_t     dstStride;    // NZ2ND时目的ND矩阵每行元素数，单位：element
      bool        reluEn;       // 是否使能ReLU
      QuantMode_t quantPre;     // 量化模式，F322F16表示float→half
      uint64_t    deqScalar;    // scalar量化参数，单个scale值
      int32_t     ndNum;        // 源NZ矩阵数目，[1, 65535]
      int32_t     srcNdStride;  // 不同NZ矩阵起始地址间隔，单位：16×C0_SIZE，[1, 512]
      int32_t     dstNdStride;  // 目的相邻ND矩阵偏移，单位：element，[1, 65535]
      int32_t     unitFlag;     // Mmad与Fixpipe并行控制
  };
  ```
  例如`{baseN, baseM, baseM, N, false, F322F16, 0, 1, 0, 0, 0}`，将L0C中的baseM×baseN float32结果转为half并写回GM。`quantPre=F322F16`完成float32到half的精度转换，因为L0C中累加结果为float32精度，而最终输出要求half类型以节省存储和带宽；`dstStride=N`指定输出矩阵在GM中每行的元素数，用于将Nz分形数据正确还原为ND行排布。

  **[`AscendC::DataCopyExtParams`](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)** — [DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)接口使用，描述GM与UB之间按块搬运的参数：
  ```cpp
  struct DataCopyExtParams {
      uint16_t blockCount;  // 连续传输数据块个数
      uint32_t blockLen;    // 每个数据块长度，单位：Byte
      uint32_t srcStride;   // 源端相邻数据块间隔，单位：Byte
      uint32_t dstStride;   // 目的端相邻数据块间隔，单位：Byte
      uint32_t rsv;         // 预留字段，配置为0
  };
  ```
  例如GM搬运到UB时`{static_cast<uint16_t>(baseM / 2), blockLen, srcStride, 0, 0}`，每个Vector核读取baseM/2行结果。`srcStride`为相邻行在GM中的字节间隔（`(N - baseN) * sizeof(half)`），因为Matmul结果在GM中按完整的N列存储，每个Vector核只读取其中baseN列宽的数据，行间需跳过多余的列。

  **[`AscendC::DataCopyPadExtParams<half>`](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)** — [DataCopyPad](../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md)接口使用，描述尾块补齐参数：
  ```cpp
  template <typename T>
  struct DataCopyPadExtParams {
      bool isPad;       // 是否开启补齐
      uint8_t leftPad;  // 左侧补齐长度
      uint8_t rightPad; // 右侧补齐长度
      T paddingValue;   // 补齐值
  };
  ```
  例如`{true, 0, 0, 0}`表示开启补齐能力，但左右两侧均不额外补齐。开启补齐是为了满足MTE2搬运引擎对数据长度的对齐要求，当实际数据长度不满足32字节对齐时，DataCopyPad会自动在尾部补齐零值。

## 功能调试

### printf

[printf](../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/printf.md)提供CPU域/NPU域调试场景下的格式化输出功能，可在算子kernel侧实现代码中需要输出日志信息的地方调用，帮助定位计算逻辑问题。

在Cube核或Vector核的计算流程中插入printf，可打印核索引、计算参数、中间结果地址等信息。示例如下：

```cpp
// 在Cube核Matmul计算流程中打印调试信息
if ASCEND_IS_AIC {
    AscendC::printf("Cube block %d: baseM=%d, baseN=%d, baseK=%d\n",
        AscendC::GetBlockIdx(), baseM, baseN, baseK);
}
// 在Vector核LeakyRelu计算流程中打印调试信息
if ASCEND_IS_AIV {
    AscendC::printf("Vector block %d: processing tile offset=%d\n",
        AscendC::GetBlockIdx(), gmOffset);
}
```

> **注意**：printf仅用于调试，会导致流水线断流，不要在性能测试时使用。

### DumpTensor

[DumpTensor](../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/DumpTensor.md)可以在算子kernel侧打印指定Tensor的内容，同时支持打印自定义的附加信息（仅支持uint32\_t数据类型的信息），比如打印当前行号等，用于验证中间计算结果的正确性。

在关键计算节点插入DumpTensor，可检查数据搬运和计算结果是否符合预期。示例如下：

```cpp
// 在Cube核Mmad计算后，Dump L0C结果验证矩阵乘正确性
if ASCEND_IS_AIC {
    AscendC::Mmad(cLocal, a2Local, b2Local, AscendC::MmadParams{baseM, baseN, baseK, 0, false, true});
    AscendC::DumpTensor(cLocal, 1, 16);  // desc=1标识为Mmad输出，dump前16个元素
}
// 在Vector核LeakyRelu计算后，Dump UB结果验证激活计算正确性
if ASCEND_IS_AIV {
    AscendC::LeakyRelu(vecLocal, vecLocal, (half)0.001, tileLen);
    AscendC::DumpTensor(vecLocal, 2, 16);  // desc=2标识为LeakyRelu输出，dump前16个元素
}
```

打印示例如下：

```
DumpTensor: desc=1, addr=0x2000, data_type=float32, position=CO1, dump_size=16
[1.023438, 0.875000, -0.250000, 2.062500, ...]
DumpTensor: desc=2, addr=0x1000, data_type=float16, position=VECCALC, dump_size=16
[1.023438, 0.875000, -0.000250, 2.062500, ...]
```

> **注意**：DumpTensor仅用于调试，会中断正常流水执行，不要在性能测试时使用。

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

## 性能调试

### msProf工具介绍

[msProf](../../../../../docs/guide/编程指南/调试调优/性能调优.md)是单算子性能分析工具，包含msprof op和msprof op simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

- 上板性能采集

    通过上板性能采集，可以直接测定算子在昇腾AI处理器上的运行时间。该方式适合在板环境中快速定位算子性能问题。

    基于可执行文件demo通过msprof op执行算子调优：
    ```bash
    msprof op ./demo
    ```

    - 性能数据说明  
      命令完成后，会在默认目录下生成以"OPPROF_{timestamp}_XXX"命名的文件夹，性能数据文件夹结构示例如下：

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

      针对融合算子的性能分析，重点关注以下指标：
      - **ArithmeticUtilization.csv**：查看Cube指令（Mmad）和Vector指令（LeakyRelu）的cycle占比，判断计算单元利用率是否充分
      - **PipeUtilization.csv**：查看MTE2/MTE3搬运耗时占比，若搬运占比过高，说明数据搬运是瓶颈，可考虑优化搬运策略或增大计算粒度
      - **Memory.csv**：查看GM与L1/UB之间的带宽利用率，判断是否存在带宽瓶颈

更多msProf工具使用方式请参考[性能调优](../../../../../docs/guide/编程指南/调试调优/性能调优.md)中的内容。
