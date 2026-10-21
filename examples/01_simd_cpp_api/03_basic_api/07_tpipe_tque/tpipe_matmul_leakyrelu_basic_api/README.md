# 基于TPipe-TQue框架的Matmul和LeakyRelu融合计算样例

## 算子概述

本样例基于[TPipe和TQue编程范式](../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simd_programming/tpipe_tque_programming/tpipe_tque_paradigm.md)（由TPipe统一管理片上资源、由TQue管理数据同步和缓冲复用的编程方式）实现Matmul和LeakyRelu融合计算并开启[Double Buffer](../../../../../docs/zh/guide/technical_appendix/concepts_and_terms/performance_optimization/double_buffer.md)，展示Cube单元和Vector单元协同计算的编程模式。Cube核完成矩阵乘计算，Vector核完成LeakyRelu激活计算。

在本样例中，A1（物理位置为L1 Buffer）、A2（物理位置为L0A Buffer）、CO1（物理位置为L0C Buffer）、VECIN（物理位置为UB）和VECOUT（物理位置为UB）队列启用Double Buffer，使不同分块的搬运和计算具备重叠执行的条件；右矩阵B在各M分块间复用，仅搬运一次并驻留L0B Buffer。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.3.0 |
| Atlas A3系列产品 | >= CANN 9.3.0 |
| Atlas A2系列产品 | >= CANN 9.3.0 |

## 目录结构介绍

```
├── tpipe_matmul_leakyrelu_basic_api
│   ├── scripts
│   │   ├── gen_data.py                       // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py                  // 真值对比文件
│   ├── CMakeLists.txt                        // 编译工程文件
│   ├── data_utils.h                          // 数据读入写出函数
│   ├── tpipe_matmul_leakyrelu_basic_api.asc  // Ascend C样例实现 & 调用样例
│   ├── README_en.md                          // 英文样例说明文档
│   └── README.md                             // 中文样例说明文档
```

## 算子描述

- 样例功能：
  实现Matmul和LeakyRelu融合计算，计算公式如下：

  Matmul计算：
  $$
  C_{ij} = \sum_{k} A_{ik} \times B_{kj}
  $$

  LeakyRelu计算：
  $$
  C_{ij} = \begin{cases}
  C_{ij} & \text{if } C_{ij} \geq 0 \\
  C_{ij} \times 0.001 & \text{if } C_{ij} < 0
  \end{cases}
  $$

  上式描述数学功能。实际计算按FP32累加、Fixpipe转FP16、FP16 LeakyRelu的顺序执行，负半轴斜率使用`(half)0.001`，最终输出为half。不能把FP32激活后再转FP16视为相同的舍入过程。

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

  **分核逻辑**：

  输出矩阵C按M方向（shape第一维，行方向）分为2个分块，N方向（shape第二维，列方向）不分块，使用2个Cube核完成Matmul计算；每个Cube核对应2个Vector核，因此使用4个Vector核完成LeakyRelu计算。每个Cube核负责singleCoreM=256行，按baseM=128拆为两个baseM×baseN结果块；每个分块由配对的2个Vector核各处理baseM/2×baseN=64×128个元素。baseK=K=128、baseN=N=128，K/N方向不分块。

  **M/N/K参数图解**：

  在矩阵乘 C = A × B 中，M/N/K分别对应矩阵的不同维度，分核偏移也围绕这些维度展开：

  ```
           K                N
       ┌─────────┐      ┌──────────┐
       │         │      │          │
     M │    A    │  × K │    B     │  =  C (M × N)
       │         │      │          │
       └─────────┘      └──────────┘
  ```

  - M方向分块数：M / singleCoreM = 512 / 256 = 2
  - N方向分块数：N / singleCoreN = 128 / 128 = 1
  - 总Cube核数：2 × 1 = 2
  - 总Vector核数：2 × 2 = 4
  - 每Cube核分块数：singleCoreM / baseM = 256 / 128 = 2

  | Cube核 | mIndex | Cube输出行范围（含两端） | 配对第一个Vector核 | 配对第二个Vector核 |
  |--------|--------|--------------------------|-------------------|-------------------|
  | AIC0 | 0 | 0~127 | AIV0：0~63 | AIV1：64~127 |
  | AIC0 | 1 | 128~255 | AIV0：128~191 | AIV1：192~255 |
  | AIC1 | 0 | 256~383 | AIV2：256~319 | AIV3：320~383 |
  | AIC1 | 1 | 384~511 | AIV2：384~447 | AIV3：448~511 |

- 算子实现：
  - **整体计算流程**：

    样例整体流程如下（展示Cube核和Vector核协同计算）：

      > **前置说明：Ascend AI Core内存层级与数据通路**
      >
      > Ascend AI Core内部有多级片上缓存，数据从外部全局内存逐级搬运至计算单元：
      > - **GM（Global Memory）**：芯片外部全局内存，存放输入/输出数据，容量最大但访问延迟最高
      > - **L1 Buffer**：片上缓存（Cube侧主要存放A/B矩阵数据），由MTE2引擎从GM搬入
      > - **L0A/L0B Buffer**：Cube矩阵计算单元的输入缓存，由MTE1引擎从L1 Buffer搬入
      > - **L0C Buffer**：Cube矩阵计算单元的输出缓存，存放Mmad计算结果
      > - **UB（Unified Buffer，本样例使用TPosition::VECIN和VECOUT）**：Vector侧片上缓存，存放Vector计算所需的输入/输出数据
      >
      > 数据需在搬运过程中进行格式转换（ND→Nz/Zn），因为Cube计算单元要求特定的分形（Fractal）排布格式，而非ND排布。
      >
      > 同步机制说明：
      > - **核内同步**：使用[TPipe](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/TPipe/TPipe.md)和[TQue](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/TQue/TQue.md)管理资源；EnQue/DeQue建立写后读数据依赖，FreeTensor/AllocTensor解决读后写内存复用冲突，无需手动设置HardEvent同步事件。
      > - **核间同步**：[CrossCoreSetFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md)/[CrossCoreWaitFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md)用于不同核之间的依赖同步，如Cube核完成计算后通知Vector核可以开始读取数据

      **1）数据流路径**

      Cube侧数据流：

      ```
      GM(A:ND,half) -> L1(A1:Nz,half) -> L0A(A2:Zz/Nz,half)
                    │                  │                │
                DataCopy            LoadData            │
                ND->Nz              Nz->Zz/Nz           │
                                                        │--->L0C(CO1,float) -> GM(C:ND,half)
                                                        │  │                 │
                                                        │ Mmad             Fixpipe
                                                        │ C=A×B       Nz->ND, float32->half
                                                        │
      GM(B:ND,half) -> L1(B1:Nz,half)  ->  L0B(B2:Zn,half)
                    │                  │
                DataCopy            LoadData
                ND->Nz              Nz->Zn(转置)
      ```

      > **注意**：L0A Buffer上的分形在不同产品上有差异：
      > - Ascend 950PR&950DT系列产品：L0A的分形为Nz
      > - Atlas A2系列产品和Atlas A3系列产品：L0A的分形为Zz

      核间同步与Vector侧数据流（1个Cube核的结果由2个Vector核处理）：

      ```
      Cube核:   Fixpipe -> GM(C:ND,half)
                           │
                           │ CrossCoreSetFlag / CrossCoreWaitFlag
                           ▼
      Vector核:     GM(C:ND,half) -> UB(VECIN,half) -> UB(VECOUT,half) -> GM(C:ND,half)
                                  │                 │                   │
                            DataCopyPad          LeakyRelu          DataCopyPad
                      MTE2搬运（baseM/2×baseN）    Vector计算            MTE3写出
      ```

      **2）流程详解**：

      1. **Cube核计算阶段**：

          **GlobalTensor定义与分核偏移**：使用[GlobalTensor](../../../../../docs/zh/api/SIMD-API/basic_api/data_structures/GlobalTensor/GlobalTensor_intro.md)（全局内存张量）访问芯片外部全局内存中的输入/输出数据，并通过[GetBlockIdx](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetBlockIdx.md)（获取当前核的逻辑编号）计算分核偏移：

          TPipe实例在Kernel入口创建，通过`Init(..., &pipe)`传入算子对象，类内仅保存指针。该写法参考仓内[避免TPipe在对象内创建和初始化](../../../../../docs/zh/guide/operator_practice/simd_operator_optimization/overhead_optimization/avoid_tpipe_init_in_object.md)有利于编译器对类内Scalar变量进行常量折叠和常量传播。

          队列成员和公共常量如下，片段中的模板尺寸来自KernelMatmul的九个模板参数：

          ```cpp
          constexpr uint32_t CUBE_BLOCK = 16;
          constexpr uint32_t BUFFER_NUM = 2;
          constexpr uint16_t READY_FLAG = 8;
          constexpr uint16_t FREE_FLAG = 10;

          // 以下成员位于KernelMatmul类的private区域。
          AscendC::TPipe* pipe;
          // A1预取两个分块，队列深度为2；其余队列深度为1。
          // 队列深度与分配的内存块数量不同，双缓冲通过InitBuffer的BUFFER_NUM参数开启。
          AscendC::TQue<AscendC::TPosition::A1, BUFFER_NUM> a1Queue;
          AscendC::TQue<AscendC::TPosition::B1, 1> b1Queue;
          AscendC::TQue<AscendC::TPosition::A2, 1> a2Queue;
          AscendC::TQue<AscendC::TPosition::B2, 1> b2Queue;
          AscendC::TQue<AscendC::TPosition::CO1, 1> cQueue;
          AscendC::TQue<AscendC::TPosition::VECIN, 1> vecInQueue;
          AscendC::TQue<AscendC::TPosition::VECOUT, 1> vecOutQueue;
          ```

          以下为模板类中的Init方法；完整类模板见源码。

          ```cpp
          __aicore__ inline void Init(
              __gm__ uint8_t* xMatrix, __gm__ uint8_t* yMatrix, __gm__ uint8_t* zMatrix, AscendC::TPipe* pipeIn)
          {
              pipe = pipeIn;
              // 本样例仅沿M方向分核和分块，K/N方向整块计算，不处理尾块。
              static_assert(M % singleCoreM == 0 && singleCoreM % baseM == 0, "M must be divisible by tiles");
              static_assert(
                  singleCoreK == K && baseK == K && singleCoreN == N && baseN == N, "This sample only partitions M");
              static_assert(baseM == 32 || baseM == 64 || baseM == 128, "Supported baseM: 32, 64, 128");
              static_assert(K == 128 && N == 128, "This sample requires K=N=128");
              static_assert(singleCoreM / baseM >= BUFFER_NUM, "At least two tiles are required for double buffer");
              aGM.SetGlobalBuffer((__gm__ half*)xMatrix);
              bGM.SetGlobalBuffer((__gm__ half*)yMatrix);
              cGM.SetGlobalBuffer((__gm__ half*)zMatrix);
              // Cube核按M方向分核，每个Cube核负责singleCoreM * N大小输出块对应的A输入和C输出。
              if ASCEND_IS_AIC {
                  aGM = aGM[AscendC::GetBlockIdx() * singleCoreM * K];
                  cGM = cGM[AscendC::GetBlockIdx() * singleCoreM * N];
                  // A1/A2/CO1使用双缓冲，内存分配和核内流水同步均由TPipe-TQue管理。
                  pipe->InitBuffer(a1Queue, BUFFER_NUM, baseM * baseK * sizeof(half));
                  pipe->InitBuffer(a2Queue, BUFFER_NUM, baseM * baseK * sizeof(half));
                  pipe->InitBuffer(cQueue, BUFFER_NUM, baseM * baseN * sizeof(float));
                  // B在不同M分块间保持不变，只搬运一次并驻留L0B，无需重复搬运或双缓冲。
                  pipe->InitBuffer(b1Queue, 1, baseK * baseN * sizeof(half));
                  pipe->InitBuffer(b2Queue, 1, baseK * baseN * sizeof(half));
              }
              if ASCEND_IS_AIV {
                  // Vector核数是Cube核数的2倍，1个Cube核的计算结果由2个Vector核继续完成LeakyRelu。
                  // 因此这里用GetBlockIdx() / 2映射回对应的Cube核输出块起始地址。
                  cGM = cGM[AscendC::GetBlockIdx() / 2 * singleCoreM * N];
                  pipe->InitBuffer(vecInQueue, BUFFER_NUM, baseM / 2 * baseN * sizeof(half));
                  pipe->InitBuffer(vecOutQueue, BUFFER_NUM, baseM / 2 * baseN * sizeof(half));
              }
          }
          ```

          `GlobalTensor`切片偏移以元素为单位。Cube核的A/C基址分别前移`GetBlockIdx()*singleCoreM*K`和`GetBlockIdx()*singleCoreM*N`；Vector核通过`GetBlockIdx()/2`映射到对应Cube。B不按M方向偏移，两个Cube核各加载一次完整B。

          **内存搬运与计算流程**：

          - **LocalTensor创建**：在Init中用TPipe::InitBuffer初始化队列，运行时用AllocTensor申请LocalTensor；写入后EnQue，消费前DeQue，读取提交后FreeTensor。A1、A2、CO1队列各分配两个内存块，B1、B2队列各分配一个内存块。
          - **GM → L1**：使用[DataCopy](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_GMToL1_ND2NZ.md)将B搬入L1一次，并预取两个A分块，完成ND到Nz格式转换（Cube计算单元要求Nz分形排布，因此需在搬运时将ND格式转为Nz格式）
          - **L1 → L0A/L0B**：使用[LoadData](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D.md)将A分块搬运到L0A，将B一次性转置加载到L0B（Nz→Zn）并持有到全部M分块计算结束
          - **L0A/L0B → L0C**：使用[Mmad](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/Mmad.md)（矩阵乘累加指令）执行每个M分块的完整K维度的矩阵乘计算；每次均初始化累加器，不进行跨分块的K累加
          - **L0C → GM**：使用[Fixpipe](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)将结果搬出到GM，完成Nz到ND格式转换和float32到half类型转换

          代码示例如下（类内片段，完整定义见[tpipe_matmul_leakyrelu_basic_api.asc](tpipe_matmul_leakyrelu_basic_api.asc)）：

          ```cpp
          __aicore__ inline void Process()
          {
              constexpr uint32_t mLoops = singleCoreM / baseM;
              if ASCEND_IS_AIC {
                  // B矩阵GM->L1调用DataCopy接口 + Nd2NzParams结构体参数：
                  // {ndNum, nValue, dValue, srcNdMatrixStride, srcDValue, dstNzC0Stride, dstNzNStride, dstNzMatrixStride}
                  AscendC::LocalTensor<half> b1Local = b1Queue.AllocTensor<half>();
                  AscendC::DataCopy(b1Local, bGM, AscendC::Nd2NzParams{1, baseK, baseN, 0, N, baseK, 1, 0});
                  b1Queue.EnQue(b1Local);
                  // 预取两个A分块，使后续分块的搬入可以与当前分块的计算重叠。
                  for (uint32_t mIndex = 0; mIndex < BUFFER_NUM; ++mIndex) {
                      CopyInA(mIndex);
                  }
                  LoadB();
                  AscendC::LocalTensor<half> b2Local = b2Queue.DeQue<half>();
                  for (uint32_t mIndex = 0; mIndex < mLoops; ++mIndex) {
                      // A1出队保证MTE2->MTE1依赖，A2入队/出队保证MTE1->M依赖。
                      AscendC::LocalTensor<half> a1Local = a1Queue.DeQue<half>();
                      AscendC::LocalTensor<half> a2Local = a2Queue.AllocTensor<half>();
          #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
                      // Nz->Zz, L0A上的排布格式为Zz。
                      // LoadData2DParams: {startIndex, repeatTimes, srcStride, sid, dstGap, ifTranspose, addrMode}
                      for (int i = 0; i < baseM / CUBE_BLOCK; ++i) {
                          AscendC::LoadData(
                              a2Local[i * baseK * CUBE_BLOCK], a1Local[i * 512 / sizeof(half)],
                              AscendC::LoadData2DParams{0, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, 0, 0, false, 0});
                      }
          #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
                      // Nz->Nz, L0A上的排布格式为Nz。
                      // LoadData2DParamsV2: {mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, ifTranspose,
                      // sid}
                      AscendC::LoadData(
                          a2Local, a1Local,
                          AscendC::LoadData2DParamsV2{
                              0, 0, baseM / CUBE_BLOCK, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, baseM / CUBE_BLOCK, false,
                              0});
          #endif
                      a2Queue.EnQue(a2Local);
                      a1Queue.FreeTensor(a1Local);
                      // 已用A1缓冲由队列安全回收，继续预取后续分块。
                      if (mIndex + BUFFER_NUM < mLoops) {
                          CopyInA(mIndex + BUFFER_NUM);
                      }
                      a2Local = a2Queue.DeQue<half>();
                      AscendC::LocalTensor<float> cLocal = cQueue.AllocTensor<float>();
                      // MmadParams: {m, n, k, unitFlag, cmatrixSource, cmatrixInitVal}
                      // 每个M分块独立计算完整K，cmatrixInitVal=true，不跨分块累加。
                      AscendC::Mmad(cLocal, a2Local, b2Local, AscendC::MmadParams{baseM, baseN, baseK, 0, false, true});
                      cQueue.EnQue(cLocal);
                      a2Queue.FreeTensor(a2Local);

                      // CO1队列保证M->FIX依赖以及L0C双缓冲复用时的FIX->M依赖。
                      cLocal = cQueue.DeQue<float>();
                      uint32_t flagOffset = mIndex % BUFFER_NUM;
                      if (mIndex >= BUFFER_NUM) {
                          // 复用flagId前，等待两个Vector核完成上一次对应分块的读取。
          #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
                          AscendC::CrossCoreWaitFlag<0x2, PIPE_FIX>(FREE_FLAG + flagOffset);
          #else
                          AscendC::CrossCoreWaitFlag<0x2>(FREE_FLAG + flagOffset);
          #endif
                      }
                      // 先将FP32矩阵乘结果转为FP16，再进行LeakyRelu。
          #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
                      AscendC::FixpipeParamsV220 fixpipeParams{baseN, baseM, baseM, N, false, QuantMode_t::F322F16,
                                                               0,     1,     0,     0, 0};
          #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
                      AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams{baseN, baseM, baseM, N};
                      fixpipeParams.quantPre = QuantMode_t::F322F16;
                      fixpipeParams.reluEn = false;
                      fixpipeParams.params = {1, 0, 0};
          #endif
                      AscendC::Fixpipe<half, float, AscendC::CFG_ROW_MAJOR>(cGM[mIndex * baseM * N], cLocal, fixpipeParams);
                      AscendC::CrossCoreSetFlag<0x2, PIPE_FIX>(READY_FLAG + flagOffset);
                      cQueue.FreeTensor(cLocal);
                  }
                  // 等待最后两个分块的FREE同步完成，避免核函数结束时遗留同步事件。
                  for (uint32_t flagOffset = 0; flagOffset < BUFFER_NUM; ++flagOffset) {
          #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
                      AscendC::CrossCoreWaitFlag<0x2, PIPE_FIX>(FREE_FLAG + flagOffset);
          #else
                      AscendC::CrossCoreWaitFlag<0x2>(FREE_FLAG + flagOffset);
          #endif
                  }
                  b2Queue.FreeTensor(b2Local);
              }
          }
          ```

          CopyInA和LoadB作为私有辅助函数，分别完成A分块搬入和B的相关加载：

          ```cpp
          __aicore__ inline void CopyInA(uint32_t mIndex)
          {
              AscendC::LocalTensor<half> a1Local = a1Queue.AllocTensor<half>();
              AscendC::DataCopy(a1Local, aGM[mIndex * baseM * K], AscendC::Nd2NzParams{1, baseM, baseK, 0, K, baseM, 1, 0});
              a1Queue.EnQue(a1Local);
          }

          // 将B矩阵从B1队列的L1内存块搬入B2队列的L0B内存块，完成Nz到Zn的格式转换。
          // 搬运后将B2 Tensor入队并释放B1 Tensor；Process在M方向分块循环前调用本函数一次，
          // 随后将B2 Tensor出队，供各M分块的Mmad复用，循环结束后再释放。
          __aicore__ inline void LoadB()
          {
              AscendC::LocalTensor<half> b1Local = b1Queue.DeQue<half>();
              AscendC::LocalTensor<half> b2Local = b2Queue.AllocTensor<half>();
          #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
              // B矩阵L1->L0B（Nz->Zn）。
              for (int i = 0; i < baseK / CUBE_BLOCK; ++i) {
                  AscendC::LoadData(
                      b2Local[i * baseN * CUBE_BLOCK], b1Local[i * 512 / sizeof(half)],
                      AscendC::LoadData2DParams{0, baseN / CUBE_BLOCK, baseK / CUBE_BLOCK, 0, 0, true, 0});
              }
          #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
              // B矩阵L1->L0B（Nz->Zn），一次LoadData完成整块加载。
              AscendC::LoadData(
                  b2Local, b1Local,
                  AscendC::LoadData2DParamsV2{
                      0, 0, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, true, 0});
          #endif
              b2Queue.EnQue(b2Local);
              b1Queue.FreeTensor(b1Local);
          }
          ```

      2. **Vector核计算阶段**：
          - **核间同步**：Vector核通过[CrossCoreWaitFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md)等待当前分块的READY同步事件；读取完成后通过MTE2发送FREE同步事件，AIC复用flagId前等待两个配对AIV完成同步
          - **LocalTensor创建**：使用VECIN和VECOUT双缓冲队列分配输入、输出[LocalTensor](../../../../../docs/zh/api/SIMD-API/basic_api/data_structures/LocalTensor/LocalTensor_intro.md)，用于承载当前Vector核处理的半块结果
          - **GM → UB**：使用[DataCopyPad](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)（带Padding的GM与UB之间数据搬运）将Matmul结果搬运到UB，每个Vector核处理baseM/2×baseN的数据
          - **UB计算**：使用[LeakyRelu](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/LeakyRelu.md)（LeakyReLU激活函数，负值部分乘以negativeSlope）执行激活计算，负值部分乘以0.001
          - **UB → GM**：使用[DataCopyPad](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_UBToGM.md)将结果写回对应分块的GM半块区域，释放输出Tensor，继续下一分块

          代码实例如下：

          ```cpp
          if ASCEND_IS_AIV {
              for (uint32_t mIndex = 0; mIndex < mLoops; ++mIndex) {
                  // Cube:Vector=1:2，每个Vector核处理当前分块的baseM / 2 * baseN个元素。
                  LeakyReluCompute(mIndex, 0, baseM / 2 * baseN);
              }
          }
          ```

          ```cpp
          __aicore__ inline void LeakyReluCompute(uint32_t mIndex, uint32_t nIndex, uint32_t len)
          {
              uint32_t blockLen = baseN * sizeof(half);
              uint32_t srcStride = (N - baseN) * sizeof(half);
              // 两个Vector核分别处理当前Cube分块的上、下半块。
              uint32_t gmOffset = (mIndex * baseM * N + nIndex * baseN) + AscendC::GetBlockIdx() % 2 * (baseM / 2 * N);
              uint32_t flagOffset = mIndex % BUFFER_NUM;
              AscendC::LocalTensor<half> vecInLocal = vecInQueue.AllocTensor<half>();
          #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
              AscendC::CrossCoreWaitFlag<0x2, PIPE_MTE2>(READY_FLAG + flagOffset);
          #else
              AscendC::CrossCoreWaitFlag<0x2>(READY_FLAG + flagOffset);
          #endif
              // GM->UB调用DataCopyPad。
              AscendC::DataCopyPad<half>(
                  vecInLocal, cGM[gmOffset],
                  AscendC::DataCopyExtParams{static_cast<uint16_t>(baseM / 2), blockLen, srcStride, 0, 0},
                  AscendC::DataCopyPadExtParams<half>{true, 0, 0, 0});
              // 读取完成后发送FREE同步事件；每个分块在GM中有独立区域，不会被后续Cube分块覆盖。
              AscendC::CrossCoreSetFlag<0x2, PIPE_MTE2>(FREE_FLAG + flagOffset);
              vecInQueue.EnQue(vecInLocal);

              // 输入队列保证MTE2->V依赖，输出队列保证V->MTE3依赖。
              vecInLocal = vecInQueue.DeQue<half>();
              AscendC::LocalTensor<half> vecOutLocal = vecOutQueue.AllocTensor<half>();
              AscendC::LeakyRelu(vecOutLocal, vecInLocal, (half)0.001, len);
              vecInQueue.FreeTensor(vecInLocal);
              vecOutQueue.EnQue(vecOutLocal);

              vecOutLocal = vecOutQueue.DeQue<half>();
              AscendC::DataCopyPad<half>(
                  cGM[gmOffset], vecOutLocal,
                  AscendC::DataCopyExtParams{static_cast<uint16_t>(baseM / 2), blockLen, 0, srcStride, 0});
              vecOutQueue.FreeTensor(vecOutLocal);
          }
          ```

          `gmOffset`包含分块偏移和上下半块偏移，不能沿用单分块实现中仅按`GetBlockIdx()%2`偏移的写法。当前nIndex始终为0；两个AIV写入互不重叠的行区域。默认每次搬运64行、每行256B，共16 KiB。

  - **约束条件**：
    1. K=singleCoreK=baseK=128，N=singleCoreN=baseN=128，仅沿M方向分核和分块。
    2. baseM可取32、64或128，默认128；baseM/baseK/baseN满足16对齐。
    3. M能被singleCoreM整除，singleCoreM能被baseM整除，不支持尾块。
    4. singleCoreM/baseM至少为BUFFER_NUM=2，保证初始双分块预取有效。
    5. Vector核数是Cube核数的2倍；更改配比需同时修改半块划分和核间同步逻辑。
    6. 输入输出为连续ND的half矩阵，生成脚本固定M/K/N=512/128/128；模板形式不代表任意形状均支持。

  - **调用实现**：
    函数执行空间限定符`__mix__(1, 2)`实现Cube和Vector核的协同调用，其中参数`(1, 2)`表示每个逻辑核由1个AIC（Cube核）和2个AIV（Vector核）组成，Cube:Vector核数比例为1:2。

    ```cpp
    template <
        uint32_t M, uint32_t K, uint32_t N, uint32_t singleCoreM, uint32_t singleCoreK, uint32_t singleCoreN,
        uint32_t baseM, uint32_t baseK, uint32_t baseN>
    __global__ __mix__(1, 2) void mmad_vec_custom(__gm__ uint8_t* xMatrix, __gm__ uint8_t* yMatrix, __gm__ uint8_t* zMatrix)
    {
        AscendC::TPipe pipe;
        KernelMatmul<M, K, N, singleCoreM, singleCoreK, singleCoreN, baseM, baseK, baseN> op;
        op.Init(xMatrix, yMatrix, zMatrix, &pipe);
        op.Process();
    }
    ```

- **接口参数说明**

  以下分别说明源码中花括号`{}`调用的构造参数和显式赋值的成员字段。构造参数顺序不一定等于成员声明顺序，且部分字段类型随架构变化，所以下表说明调用语义，不作为完整struct定义；产品取值范围以链接中的仓库API文档为准。

  **TPipe和TQue资源管理接口** — 在Init中分配缓冲，在各阶段传递Tensor：

  | 接口 | 本样例含义 |
  |------|------------|
  | `TQue<TPosition, depth>` | position指定逻辑位置，depth是最大待出队Tensor数；A1深度为2，其余为1 |
  | `pipe->InitBuffer(queue, num, len)` | num是分配的内存块数量，len是每个内存块的大小，单位为字节；双缓冲num=2 |
  | `AllocTensor<T>()` | 获取可写的片上Tensor，类型必须与容量和计算匹配 |
  | `EnQue(tensor)` | 发布生产任务及数据依赖 |
  | `DeQue<T>()` | 按队列顺序取得Tensor，建立消费前的依赖 |
  | `FreeTensor(tensor)` | 归还缓冲使用权，后续AllocTensor复用时保护未完成的读取 |

  例如`pipe->InitBuffer(a2Queue, 2, baseM * baseK * sizeof(half))`在L0A中分配两个32 KiB的内存块。a2Queue的模板深度为1，并不意味着只分配一个内存块。相关说明见[TPipe](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/TPipe/TPipe.md)和[TQue](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/TQue/TQue.md)。

  **[AscendC::Nd2NzParams](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_GMToL1_ND2NZ.md)** — `DataCopy`接口使用，描述ND→Nz格式转换参数：

  | 构造参数 | 含义 |
  |----------|------|
  | `ndNum` | 传输ND矩阵的数目 |
  | `nValue` | ND矩阵的行数 |
  | `dValue` | ND矩阵的列数 |
  | `srcNdMatrixStride` | 相邻ND矩阵起始地址偏移，单位：元素 |
  | `srcDValue` | 同一ND矩阵相邻行偏移，单位：元素 |
  | `dstNzC0Stride` | 目的Nz中同源行转换后多行相邻偏移，单位：C0_SIZE(32B) |
  | `dstNzNStride` | 目的Nz中Z型矩阵相邻行偏移，单位：C0_SIZE(32B) |
  | `dstNzMatrixStride` | 目的Nz中相邻Nz矩阵起始地址偏移，单位：元素；本例单矩阵设0 |

  例如搬运A矩阵时`{1, baseM, baseK, 0, K, baseM, 1, 0}`，将baseM×baseK的ND数据转为Nz格式。

  **[AscendC::LoadData2DParams](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D.md)** — `LoadData`接口使用，描述Atlas A2系列产品、Atlas A3系列产品中A矩阵L1到L0A和B矩阵L1到L0B的数据搬运参数：

  | 构造参数 | 含义 |
  |----------|------|
  | `startIndex` | 分形矩阵ID（0为第1个），单位：512B |
  | `repeatTimes` | 迭代次数，每个迭代处理512B |
  | `srcStride` | 相邻迭代源分形起始地址间隔，单位：512B |
  | `sid` | 预留，配置为0 |
  | `dstGap` | 目的端相邻迭代分形间隔，单位：512B |
  | `ifTranspose` | 是否转置每个分形，默认false |
  | `addrMode` | 地址更新方式，本样例为0（递增） |

  例如：Atlas A2系列产品、Atlas A3系列产品中，L0A上的排布格式为Zz，搬运A矩阵时`{0, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, 0, 0, false, 0}`；<br>
  搬运B矩阵时`ifTranspose=true`，完成Nz到Zn的转置搬运。

  **[AscendC::LoadData2DParamsV2](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_V2.md)** — `LoadData`接口使用，描述Ascend 950PR&950DT系列产品中A矩阵L1到L0A和B矩阵L1到L0B的数据搬运参数：

  | 构造参数 | 含义 |
  |----------|------|
  | `mStartPosition` | M方向起始位置，单位：512B |
  | `kStartPosition` | K方向起始位置，单位：512B |
  | `mStep` | M方向搬运分形数 |
  | `kStep` | K方向搬运分形数 |
  | `srcStride` | 源端相邻K方向分形间隔，单位：512B |
  | `dstStride` | 目的端相邻K方向分形间隔，单位：512B |
  | `ifTranspose` | 是否转置每个分形，默认false |
  | `sid` | 预留，配置为0 |

  Ascend 950PR&950DT系列产品中，L0A上的排布格式为Nz，搬运A矩阵时使用`{0, 0, baseM / CUBE_BLOCK, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, baseM / CUBE_BLOCK, false, 0}`，一次完成A矩阵Nz到Nz搬运；搬运B矩阵时使用`{0, 0, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, true, 0}`，一次完成B矩阵Nz到Zn搬运。

  **[AscendC::MmadParams](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/Mmad.md)** — `Mmad`接口使用，描述矩阵乘参数：

  | 构造参数 | 含义 |
  |----------|------|
  | `m` | 左矩阵Height（M维） |
  | `n` | 右矩阵Width（N维） |
  | `k` | 左矩阵Width/右矩阵Height（K维） |
  | `unitFlag` | Mmad与Fixpipe细粒度并行控制，默认0 |
  | `cmatrixSource` | C矩阵初始值来源，false=CO1，true=C2，默认false |
  | `cmatrixInitVal` | 本例每个M分块都设true，独立初始化累加结果 |

  例如`{baseM, baseN, baseK, 0, false, true}`，计算baseM×baseN输出块并在K方向累加baseK长度。

  **[AscendC::FixpipeParamsV220](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)** — Atlas A2系列产品和Atlas A3系列产品使用，描述L0C到GM的数据搬运和精度转换参数：

  | 构造参数 | 含义 |
  |----------|------|
  | `nSize` | 源Nz矩阵N方向大小 |
  | `mSize` | 源Nz矩阵M方向大小 |
  | `srcStride` | 源Nz相邻Z排布起始偏移，单位：C0_SIZE |
  | `dstStride` | Nz2ND时目的ND矩阵每行元素数，单位：element |
  | `reluEn` | 是否使能ReLU；本例为false，保留负值供LeakyRelu处理 |
  | `quantPre` | 量化模式，F322F16表示float→half |
  | `deqScalar` | scalar量化参数，单个scale值 |
  | `ndNum` | 源Nz矩阵数目 |
  | `srcNdStride` | 不同Nz矩阵起始地址间隔，单位：16×C0_SIZE；本例ndNum=1时设0 |
  | `dstNdStride` | 目的相邻ND矩阵偏移，单位：element；本例ndNum=1时设0 |
  | `unitFlag` | Mmad与Fixpipe并行控制 |

  例如`{baseN, baseM, baseM, N, false, QuantMode_t::F322F16, 0, 1, 0, 0, 0}`，将L0C中的baseM×baseN float32结果转为half并写回`cGM[mIndex * baseM * N]`。srcStride的C0_SIZE对float源为16×4=64B。

  **[AscendC::FixpipeParamsArch3510](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)** — Ascend 950PR&950DT系列产品使用：

  使用`FixpipeParamsArch3510<CO2Layout::ROW_MAJOR>`与`Fixpipe<half, float, CFG_ROW_MAJOR>`，完成NZ2ND格式转换，将FP32结果转为FP16并搬出至GM。

  | 参数 | 本样例配置及含义 |
  |------|------------------|
  | `nSize`、`mSize` | 构造参数前两项为baseN、baseM，即输出分块的列数和行数 |
  | `srcStride` | 第三项为baseM，源NZ相邻Z排布的起始偏移，单位为C0_SIZE；float源的C0_SIZE为64B |
  | `dstStride` | 第四项为N，目的ND相邻行起始偏移，单位为元素 |
  | `quantPre` | 显式配置为`QuantMode_t::F322F16` |
  | `reluEn` | 显式配置为false，保留负值供LeakyRelu处理 |
  | `params` | `Nz2NdParams{1, 0, 0}`，依次为ndNum、srcNdStride、dstNdStride；单矩阵搬运不使用两个矩阵间隔 |
  | `unitFlag` | 保持默认值0，与Mmad一致 |
  | `preReluMode`、`preClipReluMode` | 保持默认值`NO_RELU`、`NOCLIP_RELU`，不使能前置激活 |

  与FixpipeParamsV220不同，3510的NZ2ND参数位于`params`成员中；其中srcNdStride的单位为C0_SIZE，dstNdStride的单位为元素。本样例未使用随路量化scale，不设置deqScalar。

  **[AscendC::DataCopyExtParams](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)** — `DataCopyPad`接口使用，描述GM与UB之间按块搬运的参数：

  | 构造参数 | 含义 |
  |----------|------|
  | `blockCount` | 连续传输数据块个数 |
  | `blockLen` | 每个数据块长度，单位：Byte |
  | `srcStride` | 源端前一块结束至下一块起点的间隔：GM端单位为字节，UB端单位为32B数据块 |
  | `dstStride` | 目的端前一块结束至下一块起点的间隔：GM端单位为字节，UB端单位为32B数据块 |
  | `rsv` | 预留字段，配置为0 |

  例如GM搬运到UB时`{static_cast<uint16_t>(baseM / 2), blockLen, srcStride, 0, 0}`，每个Vector核读取baseM/2=64行，每行blockLen=128×2=256B。`srcStride=(N-baseN)*sizeof(half)=0`，UB间隔也为0；写回时该GM间隔作为目的端dstStride使用。

  **[AscendC::DataCopyPadExtParams\<half\>](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)** — `DataCopyPad`接口使用，描述尾块补齐参数：

  | 构造参数 | 含义 |
  |----------|------|
  | `isPad` | 是否开启补齐 |
  | `leftPadding` | 左侧补齐长度，单位：元素 |
  | `rightPadding` | 右侧补齐长度，单位：元素 |
  | `paddingValue` | 补齐值 |

  例如`{true, 0, 0, 0}`表示开启补齐配置，但左右两侧均为0个元素；当前每行256B已满足32B对齐，不增加补齐元素。

  **LeakyRelu和核间同步参数**：

  - `LeakyRelu(vecOutLocal, vecInLocal, (half)0.001, len)`依次传入输出、输入、负半轴斜率及元素数；len=8192，不是字节数。
  - `CrossCoreSetFlag<0x2, PIPE_FIX>(READY_FLAG + flagOffset)`在FIX完成后通知两个配对AIV；`CrossCoreSetFlag<0x2, PIPE_MTE2>(FREE_FLAG + flagOffset)`在AIV读取完成后确认。
  - READY_FLAG=8、FREE_FLAG=10是本样例定义的flagId起始值；`flagOffset=mIndex%BUFFER_NUM`，READY使用8/9，FREE使用10/11。AIC的一次模式2等待要求两个配对AIV均完成同步。
  - Ascend 950PR&950DT系列产品的`pipe`模板参数生效，省略时仅阻塞默认的`PIPE_S`。本样例在AIV侧使用`CrossCoreWaitFlag<0x2, PIPE_MTE2>`等待READY，保证GM→UB搬运在Fixpipe完成后执行；AIC侧使用`CrossCoreWaitFlag<0x2, PIPE_FIX>`等待FREE，保证复用flagId前两个AIV均完成读取，并约束后续Fixpipe与READY发送的顺序。末尾的FREE等待同样使用`PIPE_FIX`，由Kernel退出时TPipe析构调用`Destroy()`中的全流水同步保证完成。
  - Atlas A2系列产品和Atlas A3系列产品的`pipe`参数不影响硬件指令功能，等待会阻塞全部流水，按仓内接口建议保留`CrossCoreWaitFlag<0x2>`。两个架构分支均与Set端的模式2匹配。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
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
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2系列产品和 Atlas A3系列产品，dav-3510 对应 Ascend 950PR&950DT系列产品 |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 实现流程解析

下表按时间顺序详细拆解Cube核和Vector核的每一步操作：

### Cube核流程

| 阶段 | 数据流动/行为 | 实现目的/原因 |
|:---|:---|:---|
| 初始化 | InitBuffer为A1/A2/CO1分配双缓冲，为B1/B2分配单缓冲 | 统一管理L1/L0资源和核内同步 |
| GM → L1 | B搬入一次，A预取两个分块，DataCopy完成ND→Nz | 提前提交独立搬运 |
| L1 → L0B | B1出队、LoadB、B2入队/出队 | B在不同M分块中不变，驻留复用 |
| L1 → L0A | A1出队、LoadData、A2入队，释放A1 | 根据架构完成Nz→Zz/Nz，并在需要时预取后续A分块 |
| L0A/L0B → L0C | A2出队、分配CO1、Mmad、CO1入队、释放A2 | 每个M分块独立完成完整K维度的矩阵乘计算 |
| 核内同步 | 队列入队/出队管理生产消费依赖，释放/分配管理复用冲突 | 覆盖MTE2→MTE1、MTE1→M、M→FIX等依赖 |
| L0C → GM | CO1出队、Fixpipe转half并写当前分块、释放CO1 | 双缓冲允许不同分块的M/FIX重叠 |
| 核间同步及收尾 | FIX发送READY，复用flagId前等待FREE；结束时等待最后两个分块的FREE并释放B2 | 保证核间同步事件成对完成，并释放B2缓冲 |

### Vector核流程

| 阶段 | 数据流动/行为 | 实现目的/原因 |
|:---|:---|:---|
| 初始化 | InitBuffer为VECIN/VECOUT分别分配双缓冲 | 输入、输出生命周期分离 |
| 核间同步 | 申请VECIN Tensor，等待当前分块的READY | 确保Fixpipe已写回 |
| GM → UB | DataCopyPad读64行×128元素，MTE2发布FREE，VECIN入队 | 确认读取完成，建立MTE2→V依赖 |
| UB计算 | VECIN出队、申请VECOUT、LeakyRelu、释放VECIN、VECOUT入队 | FP16负值乘以(half)0.001 |
| UB → GM | VECOUT出队、DataCopyPad写当前半块、释放VECOUT | 建立V→MTE3及后续复用依赖 |

### 双缓冲资源与时序

| 存储 | 默认队列配置 | 总占用 |
|------|--------------|--------|
| L1 | A1：2×32 KiB，B1：1×32 KiB | 96 KiB/AIC |
| L0A | A2：2×32 KiB | 64 KiB/AIC |
| L0B | B2：1×32 KiB | 32 KiB/AIC |
| L0C | CO1：2×64 KiB | 128 KiB/AIC |
| UB | VECIN：2×16 KiB，VECOUT：2×16 KiB | 64 KiB/AIV |

同一分块内部仍按依赖执行；不同数据分块可使用不同缓冲区使MTE1加载与Mmad、Mmad与Fixpipe、Vector搬运与计算重叠。TQue的FreeTensor并非立即销毁底层片上内存，后续复用仍受队列依赖保护。双缓冲提供重叠机会，不保证性能翻倍。

默认每核两个分块，AIC发送flagId为8/9的READY同步事件，在末尾等待flagId为10/11的FREE同步事件。如果baseM=64，每核四个分块，分块2复用flagId=8前等待分块0的FREE（flagId=10），分块3复用flagId=9前等待分块1的FREE（flagId=11），然后在末尾等待最后两个分块的FREE。

FREE表示对应分块的GM读取已完成，不代表激活和最终写回完成。每个分块的GM区域独立，后续Cube分块不会覆盖前一个Vector输出；最终写回由队列同步和Kernel退出时TPipe析构中的全流水同步保证完成，无额外GM workspace。
