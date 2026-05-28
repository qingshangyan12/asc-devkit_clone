# Matmul Gelu最佳实践样例

## 概述

本样例基于静态Tensor编程范式，展示了[Cube](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（矩阵乘计算单元）-[Vector](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（向量计算单元）融合（CV融合）的实现方法，将Matmul矩阵乘法与GELU激活函数融合到同一AI Core中并行执行。AIC侧完成Matmul计算后通过[Fixpipe](../../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)将结果输出到[GM](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Global Memory，全局内存）或[UB](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)（Unified Buffer，向量计算统一缓冲区），AIV侧从GM或UB读取数据完成GELU向量计算。其中Matmul部分的优化细节参考[Matmul基础API最佳实践样例](../../01_matrix_compute_practices/matmul_basic_api_high_performance/README.md)，GELU部分的优化细节参考[Gelu性能调优样例](../../02_reg_vector_compute_practices/gelu_high_performance/README.md)。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── matmul_gelu_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── figures                 // 图示
│   └── mmad_gelu.asc           // Ascend C样例实现
```

## 样例描述

Matmul + GELU计算公式：

$$
C = GELU(A * B)
$$

$$
GELU(x) \approx \frac{x}{1 + e^{-1.595769 \cdot (x + 0.044715 \cdot x^3)}}
$$

- A为左矩阵，形状为[M, K]，数据类型为float16；B为右矩阵，形状为[K, N]，数据类型为float16，B矩阵以转置方式存储
- C为目的操作数，存放Matmul+GELU计算结果，形状为[M, N]，数据类型为float32

- 样例规格：

<table>
<tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="5" align="center">Matmul + GELU</td></tr>
<tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
<tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">float16</td><td align="center">ND</td><td align="center">false</td></tr>
<tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">float16</td><td align="center">ND</td><td align="center">true</td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">float32</td><td align="center">ND</td><td align="center">-</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">mmad_vector_custom</td></tr>
</table>

**硬件约束条件**：
- baseM/baseK/baseN须满足16对齐（[Cube](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)矩阵计算单元最小计算块为16×16）
- baseM/baseK/baseN能被singleCoreM/singleCoreK/singleCoreN整除（保证单核计算量完整切分）
- singleCoreM/singleCoreK/singleCoreN能被M/K/N整除（不支持非整切场景，非整切会产生无效数据影响计算正确性）
- AIV核数必须是AIC核数的2倍（由[核调用符`__mix__`](../../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)的1:2配比约束决定）
- UB直通模式（Scenario 2）仅[Ascend 950PR](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)架构支持，因为只有该架构具备L0C→UB的硬件通路
- B矩阵转置存储时，K维度须满足`baseK * sizeof(half)`对齐到512B的LoadData搬运要求

## 样例实现

### 性能指标说明

|| 指标                    | 说明                                                                 |
|------------------------|----------------------------------------------------------------------|
| Task Duration(μs)      | 整个任务执行的总时间，算子执行时间以该参数为准                         |
| Block Num              | 使用的核数（Block 数量）                                              |
| aicore_time(μs)        | AI Core 的平均执行时间                                                |
| aic_mac_time(μs)       | [cube](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md) 类型指令（矩阵类运算指令）耗时                                    |
| aic_mac_ratio          | cube 类型指令的 cycle 数在 total cycle 数中的占用比    |
| aic_mte1_time(μs)      | [mte1](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md) 类型指令（[L1](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md) → [L0A/L0B](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md) 搬运）耗时                               |
| aic_mte1_ratio         | mte1 类型指令的 cycle 数在 total cycle 数中的占用比 |
| aic_mte2_time(μs)      | [mte2](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md) 类型指令（GM → L1 搬运）耗时                                    |
| aic_mte2_ratio         | mte2 类型指令的 cycle 数在 total cycle 数中的占用比    |
| aic_fixpipe_time(μs)   | [fixpipe](../../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md) 类型指令（[L0C](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md) → GM/UB 搬运）耗时                             |
| aic_fixpipe_ratio      | fixpipe 类型指令的 cycle 数在 total cycle 数中的占用比 |
| aiv_time(μs)           | Task 在 AI Vector Core 上的执行时间                               |
| aiv_vec_time(μs)       | vec 类型指令（[Vector](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)向量类运算指令）耗时                                    |
| aiv_vec_ratio          | vec 类型指令的 cycle 数在 total cycle 数中的占用比    |
| aiv_scalar_time(μs)    | scalar 类型指令（标量类运算指令）耗时                                 |
| aiv_scalar_ratio       | scalar 类型指令的 cycle 数在 total cycle 数中的占用比 |
| aiv_mte2_time(μs)      | mte2 类型指令（GM → UB 搬运类指令）耗时                              |
| aiv_mte2_ratio         | mte2 类型指令的 cycle 数在 total cycle 数中的占用比 |
| aiv_mte3_time(μs)      | [mte3](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md) 类型指令（UB → GM 搬运类指令）耗时                              |
| aiv_mte3_ratio         | mte3 类型指令的 cycle 数在 total cycle 数中的占用比 |

### 数据流路径

本样例提供两种CV融合场景：

**Scenario 1：CV融合-GM中转**（A2/A3/Ascend 950PR均支持）

AIC侧通过[Fixpipe](../../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md)将[L0C](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)结果写入[GM](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)，AIV侧从GM读取数据到[UB](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)后进行GELU计算，再将结果写回GM。这种方式所有架构均支持，但需要经过GM中转，产生额外的MTE2/MTE3搬运开销。

```
GM ──(MTE2)──> L1 ──(MTE1)──> L0A/L0B ──(Cube)──> L0C ──(Fixpipe)──> GM ──(MTE2)──> UB ──(Vector)──> UB ──(MTE3)──> GM
```

**Scenario 2：CV融合-UB直通**（仅Ascend 950PR支持）

Ascend 950PR新增L0C→UB的硬件通路，A2/A3不支持。AIC侧通过Fixpipe将L0C计算结果直接写入UB，AIV侧无需从GM读取数据，直接在UB上进行GELU计算后将结果写回GM。这种方式省去了GM中转的读写开销，但仅Ascend 950PR架构支持Fixpipe从L0C到UB。

```
GM ──(MTE2)──> L1 ──(MTE1)──> L0A/L0B ──(Cube)──> L0C ──(Fixpipe)──> UB ──(Vector)──> UB ──(MTE3)──> GM
```

### 核心特性

本样例的Matmul部分采用与[Matmul基础API最佳实践样例](../../01_matrix_compute_practices/matmul_basic_api_high_performance/README.md)相同的优化手段（L1/L0双缓冲、大包搬运、细粒度流水同步、UnitFlag等），GELU部分在Ascend 950PR架构上采用RegBase方式，在A2/A3架构上采用MemBase方式。以下重点说明CV融合的实现机制。

以下重点说明AIC↔AIV的CV融合机制。

#### 1. CV融合架构：`__mix__(1,2)` 核函数

本样例使用 [`__global__ __mix__(1, 2)`](../../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md)声明核函数，表示每个逻辑核由1个AIC和2个AIV组成。AIC侧负责Matmul计算，AIV侧负责GELU向量计算，两者在同一AI Core内按(baseM × baseN)块粒度流水并行。之所以采用1:2的配比，是因为单个Cube核的Matmul产出速率较快，需要2个Vector核并行处理才能匹配Cube核的输出吞吐，避免Vector侧成为瓶颈。

```cpp
__global__ __mix__(1, 2) void mmad_vector_custom(__gm__ uint8_t* a, __gm__ uint8_t* b, __gm__ uint8_t* c)
{
    AscendC::InitSocState();
    KernelMmadVector<...> op;
    op.Init(a, b, c);
    op.Process();
    AscendC::PipeBarrier<PIPE_ALL>();
}
```

在 `Process()` 中，通过 `ASCEND_IS_AIC` 和 `ASCEND_IS_AIV` 宏区分AIC和AIV的执行路径：

```cpp
if ASCEND_IS_AIC {
    // AIC侧：Buffer分配 + 计算循环 + Fixpipe搬出
    ProcessLoopAic(...);
}
if ASCEND_IS_AIV {
    // AIV侧：GELU向量计算 + 搬出至GM
    ProcessLoopAiv();
}
```

#### 2. 跨核同步：AIC→AIV 数据就绪通知

AIC侧每完成一个 (baseM × baseN)大小子块的Fixpipe输出后，通过 [CrossCoreSetFlag](../../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreSetFlag(ISASI).md) 通知AIV侧数据已就绪；AIV侧通过 [CrossCoreWaitFlag](../../../../../../docs/api/SIMD-API/基础API/同步控制/核间同步/CrossCoreWaitFlag(ISASI).md) 等待该信号，然后开始GELU计算。这一同步机制是必须的，因为AIC和AIV各自独立执行，若AIV在AIC写出结果之前就开始读取，会读到未初始化或部分写入的错误数据。

```cpp
// AIC侧 CopyOutAic():
AscendC::CrossCoreSetFlag<0x2, PIPE_FIX>(0x8);  // 通知AIV数据就绪

// AIV侧 GeluAndCopyOutAivSeparate() / GeluAndCopyOutAiv():
AscendC::CrossCoreWaitFlag(0x8);  // 等待AIC侧Fixpipe完成
```

#### 3. Scenario 1：CV融合-GM中转

AIC侧通过Fixpipe将L0C计算结果写入GM，AIV侧从GM读取数据到UB后进行GELU计算，再将结果写回GM。这种方式所有架构均支持。

**AIC侧**：[Fixpipe](../../../../../../docs/api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/Fixpipe.md) L0C → GM

Fixpipe由[FIX流水](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)执行，将L0C中的Nz分形float32结果搬运到GM，同时完成格式转换。输出到GM时指定`CFG_ROW_MAJOR_GM`配置，表示目标为GM并以行优先排布。

```cpp
// A2/A3架构，使用FixpipeParamsV220参数结构体
AscendC::FixpipeParamsV220 fixpipeParams;
fixpipeParams.nSize = curN;
fixpipeParams.mSize = curM;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = N;
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_GM>(cGM[...], cLocal, fixpipeParams);

// Ascend 950PR架构，使用FixpipeParamsArch3510参数结构体
AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
fixpipeParams.mSize = curM;
fixpipeParams.nSize = curN;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = N;
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_GM>(cGM[...], cLocal, fixpipeParams);
```

**AIV侧**：GM → xUB → GELU → geluOutUB → GM

[`__mix__(1,2)`](../../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md) 模式下每个逻辑核有2个AIV，Fixpipe输出到GM的M行数据按M/2行拆分给两个AIV处理。每个AIV通过 [`GetSubBlockIdx()`](../../../../../../docs/api/SIMD-API/基础API/系统变量访问/GetSubBlockIdx(ISASI).md) `% 2` 获取自己的子块索引，独立完成各自M/2行的GELU计算。之所以按M/2拆分而非按N拆分，是因为按行拆分可以保证每行数据在GM中连续存储，使得DataCopyPad的每行搬运都是连续的，搬运效率更高。

```cpp
uint32_t localSubIdx = AscendC::GetSubBlockIdx() % 2;
uint32_t gmOffset = mBlockIdx * baseM * N + nBlockIdx * baseN + localSubIdx * baseM / 2 * N;

// Step 1: DataCopyPad GM → xUB，由MTE2引擎将数据从GM搬运到UB
AscendC::DataCopyPad<float>(xUB, cGM[gmOffset], copyInParams, padParams);

// Step 2: GELU计算，在Vector单元上执行
// A2/A3: GeluMemBaseCompute(xUB, geluOutUB, computeLen);
// Ascend 950PR:    GeluRegBaseCompute(xUB, geluOutUB, computeLen);

// Step 3: DataCopyPad geluOutUB → GM，由MTE3引擎将结果从UB写回GM
AscendC::DataCopyPad<float>(cGM[gmOffset], geluOutUB, copyOutParams);
```

#### 4. Scenario 2：CV融合-UB直通（仅Ascend 950PR支持）

AIC侧通过Fixpipe将L0C计算结果直接写入UB，AIV侧无需从GM读取数据，直接在UB上进行GELU计算后将结果写回GM。这种方式省去了GM中转的读写开销，但仅Ascend 950PR架构支持Fixpipe从L0C到UB的硬件通路。

**AIC侧**：Fixpipe L0C → UB（通过 `dualDstCtl=0b01` 开启双目标模式，按M维度拆分给2个AIV）

`dualDstCtl=0b01` 表示Fixpipe输出时按M维度将数据拆分到2个AIV各自的UB区域，这样每个AIV可以直接在自己的UB分区中获取数据，无需额外地址计算。之所以需要`dualDstCtl`参数，是因为`__mix__(1,2)`模式下2个AIV共享同一个逻辑核的输出，但各自拥有独立的UB空间，必须由硬件按规则拆分数据到各自的UB。

```cpp
AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
fixpipeParams.mSize = AscendC::DivCeil(curM, 2) * 2;
fixpipeParams.nSize = curN;
fixpipeParams.srcStride = curMAlign;
fixpipeParams.dstStride = curN;
fixpipeParams.dualDstCtl = 0b01;  // 按M维度拆分到2个AIV的UB
AscendC::Fixpipe<float, float, CFG_ROW_MAJOR_UB>(xUB, cLocal, fixpipeParams);
```

**AIV侧**：UB → GELU → geluOutUB → GM

AIV侧无需[DataCopyPad](../../../../../../docs/api/SIMD-API/基础API/Memory数据搬运/DataCopyPad(ISASI).md) GM→UB，直接对xUB进行GELU计算。省去了MTE2搬运环节，因此AIV侧的`aiv_mte2_time`几乎降为零。

```cpp
// GELU计算：xUB作为输入，geluOutUB作为输出
GeluRegBaseCompute(xUB, geluOutUB, computeLen);

// DataCopyPad: geluOutUB → GM，由MTE3引擎写回
uint32_t localSubIdx = AscendC::GetSubBlockIdx() % 2;
uint32_t offset = baseM / 2 * N;
AscendC::DataCopyPad<float>(cGM[mBlockIdx * baseM * N + nBlockIdx * baseN + localSubIdx * offset], geluOutUB, copyParams);
```

#### 5. GELU计算实现

**A2/A3架构**：使用MemBase方式（向量基础API + [PipeBarrier](../../../../../../docs/api/SIMD-API/基础API/同步控制/核内同步/PipeBarrier(ISASI).md)），无需额外中间缓冲区，直接复用输出缓冲区。MemBase方式下每个向量指令操作UB上的整块数据，指令间需要插入PipeBarrier同步，确保前一条指令写入完成后再开始下一条指令的读取，否则可能读到不完整的中间结果。

```cpp
__aicore__ inline void GeluMemBaseCompute(
    const AscendC::LocalTensor<float>& xLocal, const AscendC::LocalTensor<float>& yLocal, uint32_t n)
{
    AscendC::Mul(yLocal, xLocal, xLocal, n);        // x²，结果写入yLocal
    AscendC::PipeBarrier<PIPE_V>();                  // 等待Vector流水完成，确保x²写入完毕
    AscendC::Mul(yLocal, yLocal, xLocal, n);        // x³ = x² * x
    ……
}
```

**Ascend 950PR架构**：使用RegBase方式（VF函数），中间计算在[寄存器](../../../../../../docs/api/SIMD-API/基础API/Reg矢量计算/Reg矢量计算.md)（Reg）内完成，无需额外UB缓冲区。RegBase方式的优势在于：VF函数内部自动管理寄存器分配和流水同步，所有中间结果暂存在寄存器中，避免了MemBase方式下每步计算都需要将中间结果写回UB再读出的开销，减少了UB的读写次数和PipeBarrier开销。

```cpp
__simd_vf__ inline void GeluVf(__ubuf__ float* xAddr, __ubuf__ float* yAddr, uint32_t n, uint32_t loopNum)
{
    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(float);
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<float> xReg, yReg;

    for (uint16_t i = 0; i < loopNum; ++i) {
        mask = AscendC::Reg::UpdateMask<float>(n);           // 更新掩码，处理尾块
        AscendC::Reg::LoadAlign(xReg, xAddr + i * oneRepeatSize);  // 从UB加载到Reg
        AscendC::Reg::Mul(yReg, xReg, xReg, mask);           // x²，在Reg中完成
        ……
        AscendC::Reg::StoreAlign(yAddr + i * oneRepeatSize, yReg, mask);  // 从Reg写回UB
    }
}
```

GELU公式的化简、RegBase API与VF融合的详细优化分析请参考[Gelu性能调优样例](../../02_reg_vector_compute_practices/gelu_high_performance/README.md)。

#### 6. `__mix__(1,2)` 模式下的多核切分

[`__mix__(1,2)`](../../../../../../docs/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md) 模式下，[`GetBlockIdx()`](../../../../../../docs/api/SIMD-API/基础API/系统变量访问/GetBlockIdx.md) 在AIC侧和AIV侧各自独立编号：AIC侧返回0~numAICores-1，AIV侧返回0~numAIVCores-1。由于每个逻辑核对应2个AIV，AIV侧需要将 `GetBlockIdx() / 2` 映射回逻辑核ID，从而与AIC侧使用相同的多核切分策略。之所以需要除以2映射，是因为硬件为每个逻辑核分配了2个AIV物理核，AIV侧的BlockIdx是物理核编号，而计算切分策略是基于逻辑核编号设计的。

```cpp
uint32_t logicCoreId;
if ASCEND_IS_AIC {
    logicCoreId = AscendC::GetBlockIdx();
} else {
    logicCoreId = AscendC::GetBlockIdx() / 2;  // __mix__(1,2): 每个逻辑核对应2个AIV
}
uint32_t mIterIdx = logicCoreId % mIter;
uint32_t nIterIdx = logicCoreId / mIter;
```

### 性能数据分析

#### Atlas A2训练系列芯片性能数据

- Scenario 1：CV融合-GM中转，singleCoreM=2048, singleCoreN=1536，24核

|| Scenario | Task Type | Task Duration(μs) | Block Num | Mix Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|------|-----------|-------------------|-----------|---------------|-----------------|------------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|---------------------|-------------------|--------------|-----------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|
|Gelu计算 | AI_VECTOR_CORE | 369.827 | 48 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 358.91 | 236.506 | 0.659 | 8.293 | 0.023 | 316.672 | 0.882 | 150.033 | 0.418 |
| Matmul计算 | AI_CORE | 4227.605 | 24 | 0 | 3760.55 | 3083.197 | 0.82 | 338.636 | 0.09 | 2516.081 | 0.669 | 3685.713 | 0.98 | 193.704 | 0.052 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Scenario 1 | MIX_AIC | 4236.944 | 24 | 48 | 3770.16 | 3083.235 | 0.818 | 531.971 | 0.141 | 2520.852 | 0.669 | 3687.839 | 0.978 | 193.933 | 0.051 | 3773.6 | 174.327 | 0.046 | 2.016 | 0.001 | 90.315 | 0.024 | 66.373 | 0.018 |

#### Ascend 950PR芯片性能数据

- Scenario 1：CV融合-GM中转，singleCoreM=2048, singleCoreN=1024，32核
- Scenario 2：CV融合-UB直通，singleCoreM=2048, singleCoreN=1024，32核

|| Scenario | Task Type | Task Duration(μs) | Block Num | Mix Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|------|-----------|-------------------|-----------|---------------|-----------------|------------------|---------------|---------------------|------------------|------------------|----------------|------------------|----------------|------------------|----------------|---------------------|-------------------|--------------|-----------------|---------------|---------------------|------------------|------------------|----------------|
| Gelu计算(RegBase) | AI_VECTOR_CORE | 348.868 | 64 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 347.99 | 66.277 | 0.19 | 3.03 | 0.009 | 320.543 | 0.921 | 314.547 | 0.904 |
| Matmul计算 | AI_CORE | 2601.311 | 32 | 0 | 2600.55 | 2593.264 | 0.997 | 104.582 | 0.04 | 816.91 | 0.314 | 1875.057 | 0.721 | 252.163 | 0.097 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| Scenario 1 | MIX_AIC | 2606.91 | 32 | 64 | 2605.9 | 2597.856 | 0.997 | 105.131 | 0.04 | 818.781 | 0.314 | 1917.737 | 0.736 | 252.231 | 0.097 | 2605.93 | 59.897 | 0.023 | 1.783 | 0.001 | 57.145 | 0.022 | 50.516 | 0.019 |
| Scenario 2 | MIX_AIC | 2573.584 | 32 | 64 | 2572.67 | 2565.251 | 0.997 | 104.669 | 0.041 | 819.934 | 0.319 | 2096.38 | 0.815 | 212.403 | 0.083 | 2572.69 | 60.438 | 0.023 | 1.138 | 0 | 0.005 | 0 | 68.76 | 0.027 |

#### 性能数据解读

**Scenario 1：CV融合-GM中转**

以Ascend 950PR芯片数据为例：
- 单独Gelu任务耗时348.868μs，单独Matmul任务耗时2601.311μs
- Scenario 1融合后总耗时2606.91μs，与Matmul单独执行时间基本一致

这表明采用CV融合方式编写时，AIV的Vector计算时间基本被AIC的Cube计算时间完全覆盖。由于[Cube](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)和[Vector](../../../../../../docs/guide/编程指南/高级编程/硬件实现/基本架构.md)可以并行执行，整体任务执行时间不是简单的C+V叠加，而是基本等于Cube计算时间（即Matmul时间），实现了高效的流水并行。

下图展示了Scenario 1（CV融合-GM中转）场景下AIC和AIV的流水并行执行过程。AIC侧完成Matmul计算后通过Fixpipe将结果写入GM，AIV侧从GM读取数据完成GELU计算，两者按(baseM × baseN)块粒度流水并行：

![Scenario1 CV融合-GM中转流水并行](figures/CVParallell_L0C_GM_UB.png)
> 使用 [msProf](../../../../../../docs/guide/编程指南/调试调优/性能调优.md) 工具需安装 CANN 商用/社区版，详细信息可参考[msOpProf工具安装指南](https://www.hiascend.com/document/detail/zh/canncommercial/900/devaids/optool/docs/zh/install_guide/msopprof_install_guide.md)。流水图基于msprof工具上板获取，获取命令：`msprof op --aic-metrics=PipeTimeline ./demo`

**MTE2时间显著减少的原因：**

对比数据可以发现：
- 单独Gelu计算的aiv_mte2_time为320.543μs
- Scenario 1中aiv_mte2_time仅为57.145μs，减少约82.2%

原因分析：虽然Scenario 1中AIV也需要从GM将数据读取到UB，但AIC的Fixpipe将Matmul计算结果写入GM后，数据会被缓存在L2 Cache中。当AIV发起MTE2操作从GM读取数据时，由于数据已在L2 Cache中命中，实际是从L2 Cache读取而非从GM读取，因此MTE2时间显著减少。这也是CV融合场景下的一个隐式性能优化点。

**Scenario 2：CV融合-UB直通**

以Ascend 950PR芯片数据为例：
- Scenario 2融合后总耗时2573.584μs，相比Scenario 1的2606.91μs进一步减少约33μs
- Scenario 2中aiv_mte2_time仅为0.005μs，相比Scenario 1的57.145μs几乎降为零

下图展示了Scenario 2（CV融合-UB直通）场景下AIC和AIV的流水并行执行过程。AIC侧完成Matmul计算后通过Fixpipe将结果直接写入UB（不经过GM中转），AIV侧直接在UB上进行GELU计算，省去了GM→UB的MTE2搬运开销：

![Scenario2 CV融合-UB直通流水并行](figures/CVParallell_L0C_UB.png)


## 功能调试

### printf

[printf](../../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/printf.md)提供CPU域/NPU域调试场景下的格式化输出功能，可在算子kernel侧实现代码中需要输出日志信息的地方调用，帮助定位计算逻辑问题。

在AIC侧或AIV侧的计算流程中插入printf，可打印核索引、计算参数、子块偏移等信息，用于验证CV融合流程中的数据切分和同步逻辑是否正确。示例如下：

```cpp
// 在AIC侧Matmul计算流程中打印调试信息
if ASCEND_IS_AIC {
    AscendC::printf("AIC block %d: mBlockIdx=%d, nBlockIdx=%d, kLoopCount=%d\n",
        AscendC::GetBlockIdx(), mBlockIdx, nBlockIdx, kLoopCount);
}
// 在AIV侧GELU计算流程中打印调试信息
if ASCEND_IS_AIV {
    AscendC::printf("AIV block %d, subIdx=%d: gmOffset=%d, computeLen=%d\n",
        AscendC::GetBlockIdx(), AscendC::GetSubBlockIdx() % 2, gmOffset, computeLen);
}
```

> **注意**：printf仅用于调试，会导致流水线断流，不要在性能测试时使用。

### DumpTensor

[DumpTensor](../../../../../../docs/api/SIMD-API/基础API/调试接口/上板打印/DumpTensor.md)可以在算子kernel侧打印指定Tensor的内容，同时支持打印自定义的附加信息（仅支持uint32\_t数据类型的信息），比如打印当前行号等，用于验证中间计算结果的正确性。

在关键计算节点插入DumpTensor，可检查Matmul输出和GELU中间结果是否符合预期。示例如下：

```cpp
// AIC侧Mmad计算后，Dump L0C结果验证矩阵乘正确性
if ASCEND_IS_AIC {
    AscendC::Mmad(cLocal, a2Local, b2Local, mmadParams);
    AscendC::DumpTensor(cLocal, 1, 16);  // desc=1标识为Mmad输出，dump前16个元素
}
// AIV侧GELU计算后，Dump UB结果验证激活计算正确性
if ASCEND_IS_AIV {
    // GM中转模式：DataCopyPad GM→xUB后
    AscendC::DataCopyPad<float>(xUB, cGM[gmOffset], copyInParams, padParams);
    AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
    AscendC::DumpTensor(xUB, 2, 16);  // desc=2标识为GELU输入，dump前16个元素

    // GELU计算完成后
    GeluMemBaseCompute(xUB, geluOutUB, computeLen);
    AscendC::DumpTensor(geluOutUB, 3, 16);  // desc=3标识为GELU输出，dump前16个元素
}
```

打印示例如下：

```
DumpTensor: desc=1, addr=0x2000, data_type=float32, position=CO1, dump_size=16
[1.023438, 0.875000, -0.250000, 2.062500, ...]
DumpTensor: desc=2, addr=0x1000, data_type=float32, position=VECCALC, dump_size=16
[1.023438, 0.875000, -0.250000, 2.062500, ...]
DumpTensor: desc=3, addr=0x1800, data_type=float32, position=VECCALC, dump_size=16
[0.841028, 0.731129, -0.062250, 0.979924, ...]
```

> **注意**：DumpTensor仅用于调试，会中断正常流水执行，不要在性能测试时使用。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 切换Case

  在 cmake 编译时通过 `-DSCENARIO_NUM=N` 指定要编译的场景，各场景说明：
  - `1`: CV融合-GM中转（A2/A3/Ascend 950PR均支持）
  - `2`: CV融合-UB直通（仅Ascend 950PR支持）

  > **注意：** A2/A3架构仅支持Scenario 1（GM中转），选择Scenario 2会触发编译期 `static_assert` 报错。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如：
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式或Scenario前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`、`dav-3510` | NPU硬件架构：dav-2201对应A2/A3，dav-3510对应Ascend 950PR |
  | `SCENARIO_NUM` | `1`、`2` | 场景编号：1=CV融合-GM中转，2=CV融合-UB直通（仅Ascend 950PR支持） |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 性能调试

### msProf工具介绍

[msProf](../../../../../../docs/guide/编程指南/调试调优/性能调优.md)是单算子性能分析工具，包含msprof op和msprof op simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

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

      针对CV融合算子的性能分析，重点关注以下指标：
      - **ArithmeticUtilization.csv**：查看Cube指令（Mmad）和Vector指令（GELU）的cycle占比，判断计算单元利用率是否充分
      - **PipeUtilization.csv**：查看MTE2/MTE3搬运耗时占比和Cube/Vector并行度，若搬运占比过高，说明数据搬运是瓶颈，可考虑优化搬运策略或增大计算粒度
      - **Memory.csv**：查看GM与L1/UB之间的带宽利用率，判断是否存在带宽瓶颈

- 流水线时间线采集

    通过`--aic-metrics=PipeTimeline`参数可获取AIC和AIV各流水阶段的详细时间线数据，用于可视化分析Cube与Vector的并行效率：

    ```bash
    msprof op --aic-metrics=PipeTimeline ./demo
    ```

    采集结果可通过MindStudio Insight工具打开`visualize_data.bin`文件，查看AIC（MTE2→MTE1→M→FIX）和AIV（MTE2→V→MTE3）各流水阶段的时序关系，直观判断Cube与Vector是否充分并行。

更多msProf工具使用方式请参考[性能调优](../../../../../../docs/guide/编程指南/调试调优/性能调优.md)中的内容。
