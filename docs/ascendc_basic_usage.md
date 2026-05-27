# Ascend C 基本使用方法

本文档基于 `examples/` 目录中的示例代码总结 Ascend C 的基本使用方法，覆盖 SIMD C++ API、C API、SIMT API、高阶 API、框架接入等完整使用场景。

## 1. Ascend C 概述

Ascend C 是昇腾 AI 处理器专用算子开发语言，原生支持 C/C++ 标准，提供多层 API：

| API 层级 | 编程模式 | 特点 | 适用场景 |
|---------|----------|------|---------|
| 高阶 API (Lib) | SIMD C++ | 封装常用算子，自动 Tiling | 快速开发，算法验证 |
| 基础 API | SIMD C++ | 基于 Tensor 编程，类库风格 | 算子库开发，追求极致性能 |
| C API | 语言扩展层 | 基于指针编程，纯 C 接口 | C 语言习惯，底层控制 |
| SIMT API | SIMT 编程 | 类 CUDA 的线程级编程 | 线程并行，灵活控制 |
| Reg API | Reg 编程 | 寄存器级向量计算 | 精细控制，高性能 |

## 2. 编程模型

### 2.1 核函数定义

**SIMD Vector 核函数（向量计算）**
```cpp
__global__ __vector__ void kernel_name(__gm__ float* input, __gm__ float* output)
{
    AscendC::InitSocState();  // 初始化核状态
    // Vector 核计算逻辑
}
```

**SIMD Cube 核函数（矩阵计算）**
```cpp
__global__ __cube__ void kernel_name(__gm__ half* a, __gm__ half* b, __gm__ half* c)
{
    AscendC::InitSocState();
    // Cube 核矩阵计算逻辑
}
```

**SIMT 核函数（线程级编程）**
```cpp
#include "simt_api/asc_simt.h"

__global__ void kernel_name(float* input, float* output, uint64_t length)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= length) {
        return;
    }
    output[idx] = input[idx];
}
```

### 2.2 核函数调用

使用 `<<<...>>>` 内核调用符（类似 CUDA）：

**SIMD Vector 调用**
```cpp
kernel<<<numBlocks, dynUBufSize, stream>>>(args...);
// 或使用 nullptr 替代 dynUBufSize
kernel<<<numBlocks, nullptr, stream>>>(args...);
```

**SIMD Cube 调用**
```cpp
kernel<<<numBlocks, nullptr, stream>>>(args...);
```

**SIMT 调用**
```cpp
kernel<<<block_num, thread_num, dyn_ubuf_size, stream>>>(args...);
// 或使用 dim3 结构
kernel<<<dim3(grid_x, grid_y), dim3(block_x, block_y), dyn_ubuf_size, stream>>>(args...);
```

参数说明：
- `numBlocks`：参与计算的核数（SIMD）或 block 数量（SIMT）
- `dynUBufSize`：动态 Unified Buffer 内存大小（字节），SIMD 通常为 0 或 nullptr
- `stream`：ACL stream，`nullptr` 表示默认 stream

### 2.3 核索引获取

**SIMD API**
```cpp
uint32_t block_idx = AscendC::GetBlockIdx();  // 当前核索引
uint32_t block_num = AscendC::GetBlockNum();  // 总核数
```

**SIMT API**
```cpp
uint32_t block_idx = blockIdx.x;    // 当前 block 索引
uint32_t block_num = gridDim.x;     // 总 block 数
uint32_t thread_idx = threadIdx.x;  // 当前线程索引
uint32_t thread_num = blockDim.x;   // block 内线程数

// 计算全局线程 ID
int32_t global_tid = blockIdx.x * blockDim.x + threadIdx.x;
```

## 3. SIMD C++ API 使用方法

### 3.1 基础流程

完整的向量计算流程：

```cpp
#include "kernel_operator.h"

__global__ __vector__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z, uint32_t length)
{
    AscendC::InitSocState();
    
    // 1. 定义 Global Tensor（GM 内存）
    AscendC::GlobalTensor<float> xGm, yGm, zGm;
    xGm.SetGlobalBuffer(x + AscendC::GetBlockIdx() * length, length);
    yGm.SetGlobalBuffer(y + AscendC::GetBlockIdx() * length, length);
    zGm.SetGlobalBuffer(z + AscendC::GetBlockIdx() * length, length);
    
    // 2. 分配 Local Tensor（UB 内存）
    AscendC::LocalMemAllocator<AscendC::Hardware::UB> allocator;
    AscendC::LocalTensor<float> xLocal = allocator.Alloc<float, length>();
    AscendC::LocalTensor<float> yLocal = allocator.Alloc<float, length>();
    AscendC::LocalTensor<float> zLocal = allocator.Alloc<float, length>();
    
    // 3. 数据搬运：GM -> UB
    AscendC::DataCopy(xLocal, xGm, length);
    AscendC::DataCopy(yLocal, yGm, length);
    AscendC::PipeBarrier<PIPE_ALL>();
    
    // 4. 计算
    AscendC::Add(zLocal, xLocal, yLocal, length);
    AscendC::PipeBarrier<PIPE_ALL>();
    
    // 5. 数据搬运：UB -> GM
    AscendC::DataCopy(zGm, zLocal, length);
    AscendC::PipeBarrier<PIPE_ALL>();
}
```

### 3.2 内存类型

| 内存类型 | 位置 | 说明 | 使用场景 |
|---------|------|------|---------|
| Global Memory (GM) | Device 全局内存 | 输入输出数据存储 | 存储算子输入输出 |
| Unified Buffer (UB) | AI Core 内部 | Vector 计算单元工作区 | 向量计算中间结果 |
| L1 Buffer | AI Core 内部 | Cube 计算单元输入缓存 | 矩阵计算输入数据 |
| L0A/L0B Buffer | AI Core 内部 | Cube 计算单元操作缓存 | 矩阵乘法操作数 |
| L0C Buffer (CO1) | AI Core 内部 | Cube 计算单元输出缓存 | 矩阵乘法结果 |

### 3.3 Tensor 操作

**Global Tensor 定义与设置**
```cpp
AscendC::GlobalTensor<float> gmTensor;
gmTensor.SetGlobalBuffer((__gm__ float*)addr, length);

// 带偏移设置
gmTensor.SetGlobalBuffer((__gm__ float*)addr + offset, length);

// 获取单个值
float value = gmTensor.GetValue(0);
```

**Local Tensor 分配**
```cpp
// 方法 1：使用 LocalMemAllocator
AscendC::LocalMemAllocator<AscendC::Hardware::UB> allocator;
AscendC::LocalTensor<float> localTensor = allocator.Alloc<float, length>();

// 方法 2：使用 TPipe/TQue（队列管理）
AscendC::TPipe pipe;
AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueue;
pipe.InitBuffer(inQueue, BUFFER_NUM, length * sizeof(float));
AscendC::LocalTensor<float> localTensor = inQueue.AllocTensor<float>();

// 方法 3：直接构造（用于 Cube 单元）
AscendC::LocalTensor<half> a1Local(AscendC::TPosition::A1, addr, size);
AscendC::LocalTensor<half> a2Local(AscendC::TPosition::A2, addr, size);
AscendC::LocalTensor<float> cLocal(AscendC::TPosition::CO1, addr, size);
```

### 3.4 数据搬运 API

**GM <-> UB 搬运**
```cpp
// GM -> UB
AscendC::DataCopy(localTensor, globalTensor, length);
AscendC::DataCopy(localTensor, globalTensor[offset], length);

// UB -> GM
AscendC::DataCopy(globalTensor, localTensor, length);
AscendC::DataCopy(globalTensor[offset], localTensor, length);
```

**GM <-> L1 搬运**
```cpp
AscendC::DataCopy(l1Tensor, globalTensor, params);  // GM -> L1
AscendC::DataCopy(globalTensor, l1Tensor, params);  // L1 -> GM
```

**矩阵数据搬运（Nd2Nz 格式转换）**
```cpp
AscendC::Nd2NzParams params;
params.ndNum = 1;
params.nValue = rows;
params.dValue = cols;
params.srcNdMatrixStride = 0;
params.srcDValue = totalCols;  // 原始矩阵列数
params.dstNzC0Stride = rows;
params.dstNzNStride = 1;
params.dstNzMatrixStride = 0;
AscendC::DataCopy(l1Tensor, globalTensor, params);
```

**L1 -> L0A/L0B 搬运（LoadData）**
```cpp
// 2201 架构
AscendC::LoadData2DParams loadDataParams;
loadDataParams.startIndex = 0;
loadDataParams.repeatTimes = baseK / CUBE_BLOCK;
loadDataParams.srcStride = baseM / CUBE_BLOCK;
loadDataParams.dstGap = 0;
loadDataParams.ifTranspose = false;
AscendC::LoadData(l0Tensor, l1Tensor, loadDataParams);

// 3510 架构
AscendC::LoadData2DParamsV2 loadDataParamsV2;
loadDataParamsV2.mStartPosition = 0;
loadDataParamsV2.kStartPosition = 0;
loadDataParamsV2.mStep = DivCeil(baseM, CUBE_BLOCK);
loadDataParamsV2.kStep = DivCeil(baseK * sizeof(half), 32);
loadDataParamsV2.srcStride = DivCeil(baseM, CUBE_BLOCK);
loadDataParamsV2.dstStride = DivCeil(baseM, CUBE_BLOCK);
loadDataParamsV2.ifTranspose = false;
AscendC::LoadData(l0Tensor, l1Tensor, loadDataParamsV2);
```

**L0C -> GM 搬运（Fixpipe）**
```cpp
AscendC::FixpipeParamsV220 fixpipeParams;
fixpipeParams.nSize = baseN;
fixpipeParams.mSize = baseM;
fixpipeParams.srcStride = baseM;
fixpipeParams.dstStride = N;
fixpipeParams.quantPre = QuantMode_t::F322F16;  // float -> half 转换
fixpipeParams.ndNum = 1;
fixpipeParams.srcNdStride = 0;
fixpipeParams.dstNdStride = 0;
AscendC::Fixpipe(globalTensor, cLocal, fixpipeParams);
```

### 3.5 计算 API

**向量计算**
```cpp
AscendC::Add(zLocal, xLocal, yLocal, length);       // 加法
AscendC::Sub(zLocal, xLocal, yLocal, length);       // 减法
AscendC::Mul(zLocal, xLocal, yLocal, length);       // 乘法
AscendC::Div(zLocal, xLocal, yLocal, length);       // 除法
AscendC::Abs(zLocal, xLocal, length);               // 绝对值
AscendC::Cast(zLocal, xLocal, length);              // 类型转换
AscendC::Exp(zLocal, xLocal, length);               // 指数函数
AscendC::Log(zLocal, xLocal, length);               // 对数函数
AscendC::Sqrt(zLocal, xLocal, length);              // 平方根
AscendC::Max(zLocal, xLocal, yLocal, length);       // 最大值
AscendC::Min(zLocal, xLocal, yLocal, length);       // 最小值
AscendC::Compare(zLocal, xLocal, yLocal, length, CMPMODE::EQ);  // 比较
```

**矩阵计算（Cube 单元）**
```cpp
AscendC::MmadParams params;
params.m = M;
params.n = N;
params.k = K;
params.cmatrixInitVal = true;  // 是否初始化 C 矩阵为 0
AscendC::Mmad(cLocal, aLocal, bLocal, params);  // 矩阵乘加 C = A * B + C

// 批量矩阵乘
AscendC::BatchMmadParams batchParams;
batchParams.batchNum = batchCount;
AscendC::BatchMmad(cLocal, aLocal, bLocal, batchParams);
```

**规约计算**
```cpp
AscendC::ReduceSum(sumLocal, inputLocal, length);   // 求和
AscendC::ReduceMax(maxLocal, inputLocal, length);   // 最大值
AscendC::ReduceMin(minLocal, inputLocal, length);   // 最小值
```

### 3.6 同步机制

**PipeBarrier（流水线同步）**
```cpp
AscendC::PipeBarrier<PIPE_ALL>();    // 全流水线同步
AscendC::PipeBarrier<PIPE_M>();      // MTE1 -> M 同步
AscendC::PipeBarrier<PIPE_MTE2>();   // M -> MTE2 同步
AscendC::PipeBarrier<PIPE_V>();      // Vector 计算同步
AscendC::PipeBarrier<PIPE_MTE3>();   // MTE3 同步
```

**HardEvent（硬件事件同步）- 推荐用于细粒度控制**
```cpp
// 设置事件标志
AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
// 等待事件完成
AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);

// 常用事件类型
// MTE2_MTE1: MTE2 -> MTE1 数据搬运完成
// MTE1_M: MTE1 -> M 数据加载完成
// M_MTE2: M 计算完成 -> MTE2
// M_FIX: M 计算完成 -> FIX 输出
```

### 3.7 TPipe/TQue 编程模式

使用 TPipe 和 TQue 进行内存和同步管理（推荐用于复杂算子）：

```cpp
class KernelAdd {
public:
    __aicore__ inline void Init(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z, 
                                uint32_t totalLength, uint32_t tileNum)
    {
        // 初始化 Global Tensor
        this->blockLength = totalLength / AscendC::GetBlockNum();
        this->tileLength = this->blockLength / tileNum / BUFFER_NUM;
        
        xGm.SetGlobalBuffer((__gm__ float*)x + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);
        yGm.SetGlobalBuffer((__gm__ float*)y + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);
        zGm.SetGlobalBuffer((__gm__ float*)z + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);
        
        // 初始化 Queue
        pipe.InitBuffer(inQueueX, BUFFER_NUM, this->tileLength * sizeof(float));
        pipe.InitBuffer(inQueueY, BUFFER_NUM, this->tileLength * sizeof(float));
        pipe.InitBuffer(outQueueZ, BUFFER_NUM, this->tileLength * sizeof(float));
    }
    
    __aicore__ inline void Process()
    {
        int32_t loopCount = tileNum * BUFFER_NUM;
        for (int32_t i = 0; i < loopCount; i++) {
            CopyIn(i);
            Compute(i);
            CopyOut(i);
        }
    }
    
private:
    __aicore__ inline void CopyIn(int32_t progress)
    {
        AscendC::LocalTensor<float> xLocal = inQueueX.AllocTensor<float>();
        AscendC::LocalTensor<float> yLocal = inQueueY.AllocTensor<float>();
        AscendC::DataCopy(xLocal, xGm[progress * this->tileLength], this->tileLength);
        AscendC::DataCopy(yLocal, yGm[progress * this->tileLength], this->tileLength);
        inQueueX.EnQue(xLocal);
        inQueueY.EnQue(yLocal);
    }
    
    __aicore__ inline void Compute(int32_t progress)
    {
        AscendC::LocalTensor<float> xLocal = inQueueX.DeQue<float>();
        AscendC::LocalTensor<float> yLocal = inQueueY.DeQue<float>();
        AscendC::LocalTensor<float> zLocal = outQueueZ.AllocTensor<float>();
        AscendC::Add(zLocal, xLocal, yLocal, this->tileLength);
        outQueueZ.EnQue<float>(zLocal);
        inQueueX.FreeTensor(xLocal);
        inQueueY.FreeTensor(yLocal);
    }
    
    __aicore__ inline void CopyOut(int32_t progress)
    {
        AscendC::LocalTensor<float> zLocal = outQueueZ.DeQue<float>();
        AscendC::DataCopy(zGm[progress * this->tileLength], zLocal, this->tileLength);
        outQueueZ.FreeTensor(zLocal);
    }
    
private:
    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueX, inQueueY;
    AscendC::TQue<AscendC::TPosition::VECOUT, BUFFER_NUM> outQueueZ;
    AscendC::GlobalTensor<float> xGm, yGm, zGm;
    uint32_t blockLength, tileLength;
};
```

## 4. 高阶 API (Lib 库) 使用方法

### 4.1 Matmul 高阶 API

Matmul 高阶 API 封装了矩阵乘法的完整流程，自动管理 Tiling 和内存：

```cpp
#include "lib/matmul_intf.h"
#include "kernel_tiling/kernel_tiling.h"
#include "tiling/tiling_api.h"

template <typename AType, typename BType, typename CType, typename BiasType>
class MatmulKernel {
public:
    // 定义 Matmul 对象
    AscendC::Matmul<AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, AType>,
                    AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BType>,
                    AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, CType>,
                    AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, BiasType>, CFG_NORM>
        matmulObj;
    
    __aicore__ inline void Init(__gm__ uint8_t* a, __gm__ uint8_t* b, __gm__ uint8_t* bias, 
                                __gm__ uint8_t* c, const TCubeTiling& tiling)
    {
        aGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ AType*>(a), tiling.M * tiling.Ka);
        bGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ BType*>(b), tiling.Kb * tiling.N);
        cGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ CType*>(c), tiling.M * tiling.N);
        biasGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ BiasType*>(bias), tiling.N);
    }
    
    __aicore__ inline void Process(AscendC::TPipe* pipe)
    {
        matmulObj.SetTensorA(aGlobal, IS_TRANS_A);
        matmulObj.SetTensorB(bGlobal, IS_TRANS_B);
        if (tiling.isBias) {
            matmulObj.SetBias(biasGlobal);
        }
        matmulObj.IterateAll(cGlobal);
        matmulObj.End();
    }
    
private:
    AscendC::GlobalTensor<AType> aGlobal;
    AscendC::GlobalTensor<BType> bGlobal;
    AscendC::GlobalTensor<CType> cGlobal;
    AscendC::GlobalTensor<BiasType> biasGlobal;
};

// 核函数
__global__ __cube__ void matmul_custom(__gm__ uint8_t* a, __gm__ uint8_t* b, 
                                       __gm__ uint8_t* bias, __gm__ uint8_t* c,
                                       __gm__ uint8_t* workspace, __gm__ uint8_t* tilingGm)
{
    TCubeTiling tiling;
    CopyTiling(&tiling, tilingGm);
    AscendC::TPipe pipe;
    
    MatmulKernel<half, half, float, float> matmulKernel;
    REGIST_MATMUL_OBJ(&pipe, GetSysWorkSpacePtr(), matmulKernel.matmulObj, &tiling);
    matmulKernel.Init(a, b, bias, c, tiling);
    matmulKernel.Process(&pipe);
}

// Host 端 Tiling 生成
void GenerateTiling(platform_ascendc::PlatformAscendC* ascendcPlatform, uint8_t* tilingBuf)
{
    optiling::TCubeTiling tilingData;
    matmul_tiling::MultiCoreMatmulTiling tilingApi(*ascendcPlatform);
    
    tilingApi.SetDim(ascendcPlatform->GetCoreNumAic());
    tilingApi.SetAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, 
                       matmul_tiling::DataType::DT_FLOAT16, IS_TRANS_A);
    tilingApi.SetBType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, 
                       matmul_tiling::DataType::DT_FLOAT16, IS_TRANS_B);
    tilingApi.SetCType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, 
                       matmul_tiling::DataType::DT_FLOAT);
    tilingApi.SetOrgShape(M, N, K);
    tilingApi.SetShape(M, N, K);
    tilingApi.EnableBias(IS_BIAS);
    
    tilingApi.GetTiling(tilingData);
    tilingData.SaveToBuffer(tilingBuf, tilingData.GetDataSize());
}
```

**Matmul 变体示例**

| 目录 | 功能 |
|-----|------|
| `matmul_fused` | Matmul 融合 ReLU/随路量化 |
| `matmul_quant` | 量化矩阵乘法 |
| `matmul_splitk` | SplitK 并行优化 |
| `matmul_l2cache` | L2 Cache 优化 |
| `matmul_async_iterate` | 异步迭代优化 |
| `batch_matmul` | 批量矩阵乘法 |

### 4.2 SoftMax 高阶 API

```cpp
#include "tiling/tiling_api.h"

// SoftMax Tiling 定义
BEGIN_TILING_DATA_DEF(SoftmaxCustomTilingData)
TILING_DATA_FIELD_DEF(uint32_t, columnLength);
TILING_DATA_FIELD_DEF(uint32_t, rowLength);
TILING_DATA_FIELD_DEF_STRUCT(SoftMaxTiling, softmaxTilingData);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(SoftmaxCustom, SoftmaxCustomTilingData)

// SoftMax 计算
AscendC::SoftMaxShapeInfo srcShape = {rowNum, columnLength, rowNum, columnLength};

// BasicBlock 高性能版本（满足对齐条件时使用）
if (rowNum % BASIC_BLOCK_ROW_FACTOR == 0 && columnLength % BASIC_BLOCK_COLUMN_FACTOR == 0 &&
    columnLength < BASIC_BLOCK_MAX_COLUMN_LENGTH) {
    AscendC::SoftMax<float, true, true>(xLocal, sumLocal, maxLocal, xLocal, tmpBuffer, softmaxTiling, srcShape);
} else {
    AscendC::SoftMax<float, true>(xLocal, sumLocal, maxLocal, xLocal, tmpBuffer, softmaxTiling, srcShape);
}

// AdjustSoftMaxRes 后处理
AscendC::AdjustSoftMaxRes<float, float>(xLocal, maxLocal, ADJUST_FROM, ADJUST_TO, srcShape);

// Tiling 生成
ge::Shape softmaxComputeShape({singleLoopCoreRowNum, colNum});
uint32_t localworkspaceSize = AscendC::GetSoftMaxMinTmpSize(softmaxComputeShape, sizeof(float), true);
AscendC::SoftMaxTilingFunc(softmaxComputeShape, sizeof(float), localworkspaceSize, tiling.softmaxTilingData);
```

### 4.3 其他高阶 API

| API 目录 | 功能 | 示例 |
|---------|------|------|
| `01_activation` | 激活函数 | Gelu, Sigmoid, Swish, LogSoftMax |
| `02_normalization` | 归一化 | LayerNorm, InstanceNorm |
| `04_reduce` | 规约操作 | ReduceSum, ReduceMax |
| `05_sort` | 排序操作 | TopK, Sort |
| `08_tensor_transformation` | 张量变换 | Reshape, Transpose |
| `12_math` | 数学计算 | Sin, Cos, Pow |

## 5. C API 使用方法

### 5.1 基础流程

C API 提供纯 C 语言风格的接口：

```cpp
#include "c_api/asc_simd.h"

__vector__ __global__ __aicore__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    asc_init();  // 初始化
    
    // 使用数组声明 UB 内存
    __ubuf__ float xLocal[LENGTH];
    __ubuf__ float yLocal[LENGTH];
    __ubuf__ float zLocal[LENGTH];
    
    // 数据搬运：GM -> UB
    asc_copy_gm2ub_sync((__ubuf__ void*)xLocal, (__gm__ void*)(x + offset), size);
    asc_copy_gm2ub_sync((__ubuf__ void*)yLocal, (__gm__ void*)(y + offset), size);
    
    // 计算
    asc_add_sync(zLocal, xLocal, yLocal, length);
    
    // 数据搬运：UB -> GM
    asc_copy_ub2gm_sync((__gm__ void*)(z + offset), (__ubuf__ void*)zLocal, size);
}
```

### 5.2 同步 vs 异步 API

**同步 API（推荐简单场景）**
```cpp
asc_copy_gm2ub_sync(dst, src, size);    // 阻塞式搬运
asc_add_sync(z, x, y, length);          // 阻塞式计算
```

**异步 API（推荐高性能场景）**
```cpp
asc_copy_gm2ub(dst, src, size);         // 非阻塞式搬运
asc_add(z, x, y, length);               // 非阻塞式计算
asc_sync();                              // 手动同步等待
```

**异步流程示例**
```cpp
__vector__ __global__ void add_async(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    asc_init();
    
    __ubuf__ float x_local[LENGTH];
    __ubuf__ float y_local[LENGTH];
    __ubuf__ float z_local[LENGTH];
    
    // 异步搬运
    asc_copy_gm2ub((__ubuf__ void*)x_local, (__gm__ void*)x_gm, size);
    asc_copy_gm2ub((__ubuf__ void*)y_local, (__gm__ void*)y_gm, size);
    asc_sync();  // 等待搬运完成
    
    // 异步计算
    asc_add(z_local, x_local, y_local, length);
    asc_sync();  // 等待计算完成
    
    // 异步搬运结果
    asc_copy_ub2gm((__gm__ void*)z_gm, (__ubuf__ void*)z_local, size);
    asc_sync();  // 等待搬运完成
}
```

### 5.3 内存修饰符

| 修饰符 | 说明 | 使用场景 |
|-------|------|---------|
| `__gm__` | Global Memory 指针 | 核函数参数、全局数据 |
| `__ubuf__` | Unified Buffer 数组 | 核内局部数据 |
| `__l1buf__` | L1 Buffer 数组 | Cube 计算中间数据 |

### 5.4 核心 C API

**初始化与核信息**
```cpp
asc_init();              // 初始化核状态
asc_get_block_idx();     // 获取当前核索引（等于 block_idx）
asc_get_block_num();     // 获取总核数
```

**数据搬运**
```cpp
asc_copy_gm2ub_sync(dst, src, size);       // GM -> UB 同步
asc_copy_ub2gm_sync(dst, src, size);       // UB -> GM 同步
asc_copy_gm2ub(dst, src, size);            // GM -> UB 异步
asc_copy_ub2gm(dst, src, size);            // UB -> GM 异步
```

**向量计算**
```cpp
asc_add_sync(z, x, y, length);             // 加法同步
asc_sub_sync(z, x, y, length);             // 减法同步
asc_mul_sync(z, x, y, length);             // 乘法同步
asc_div_sync(z, x, y, length);             // 除法同步
asc_add(z, x, y, length);                  // 加法异步
```

**同步**
```cpp
asc_sync();                                // 等待所有异步操作完成
```

## 6. SIMT API 使用方法

### 6.1 基础流程

SIMT API 提供类 CUDA 的线程级编程：

```cpp
#include "simt_api/asc_simt.h"

__global__ void add_kernel(float* x, float* y, float* z, uint64_t total_length)
{
    // 计算全局线程 ID
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    // 边界检查
    if (idx >= total_length) {
        return;
    }
    
    // 每个线程处理一个元素
    z[idx] = x[idx] + y[idx];
}
```

### 6.2 线程索引与同步

```cpp
// 线程索引
blockIdx.x       // 当前 block 索引（x 方向）
blockIdx.y       // 当前 block 索引（y 方向）
blockDim.x       // block 内线程数（x 方向）
threadIdx.x      // 当前线程索引（x 方向）
threadIdx.y      // 当前线程索引（y 方向）
gridDim.x        // 总 block 数（x 方向）

// 线程同步
asc_syncthreads();   // block 内线程同步（类似 CUDA __syncthreads）
```

### 6.3 Shared Memory

使用 `__ubuf__` 数组作为 block 内共享内存：

```cpp
__global__ void histogram_kernel(unsigned int* partial_histogram, const unsigned int* input_words, unsigned int word_count)
{
    // 声明 Shared Memory（block 内所有线程共享）
    __ubuf__ unsigned int s_mem_histogram[HISTOGRAM_TEMP_MEMORY];
    
    // 初始化（所有线程并行初始化）
    for (unsigned int i = threadIdx.x; i < HISTOGRAM_TEMP_MEMORY; i += blockDim.x) {
        s_mem_histogram[i] = 0;
    }
    asc_syncthreads();  // 等待初始化完成
    
    // 计算处理...
    
    asc_syncthreads();  // 等待计算完成
    
    // 合并结果...
}
```

### 6.4 原子操作

```cpp
// 原子加
asc_atomic_add(addr, value);       // 将 value 原子加到 addr

// 使用示例
__global__ void histogram_kernel(unsigned int* histogram, const unsigned int* input)
{
    __ubuf__ unsigned int local_hist[256];
    
    for (unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x; tid < word_count; 
         tid += blockDim.x * gridDim.x) {
        unsigned int value = input[tid];
        // 原子累加到对应 bin
        asc_atomic_add(local_hist + value, 1U);
    }
}
```

### 6.5 不规则内存访问（Gather）

```cpp
template <typename type_data, typename type_idx>
__global__ void gather_custom(type_data* input, type_idx* index, type_data* output, 
                              uint32_t in_width, uint32_t index_total_length)
{
    int32_t out_row = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (out_row >= index_total_length) {
        return;
    }
    
    // 使用索引访问不规则内存
    uint32_t in_row = index[out_row];
    for (int32_t col = 0; col < in_width; col++) {
        int input_idx = in_row * in_width + col;
        int output_idx = out_row * in_width + col;
        output[output_idx] = input[input_idx];
    }
}
```

### 6.6 Warp 级优化

```cpp
constexpr unsigned int WARP_SIZE = 32;  // 一个 warp 有 32 个线程

// Warp 内线程索引
unsigned int lane_id = threadIdx.x & (WARP_SIZE - 1);
unsigned int warp_id = threadIdx.x >> 5;  // threadIdx.x / 32

// Warp 内共享内存（每个 warp 独立）
__ubuf__ unsigned int warp_histogram[WARP_COUNT * BIN_COUNT];
unsigned int* my_warp_hist = warp_histogram + warp_id * BIN_COUNT;
```

## 7. Reg 编程 API

Reg API 提供寄存器级向量计算，适合精细控制场景：

```cpp
#include "kernel_operator.h"

__global__ __vector__ void reg_add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    // 使用 Reg Tensor
    AscendC::RegTensor<float> xReg, yReg, zReg;
    
    // Reg 级计算
    AscendC::Add(zReg, xReg, yReg);
}

// C API Reg 操作
asc_add_reg(z_reg, x_reg, y_reg, length);      // Reg 级加法
asc_mul_reg(z_reg, x_reg, y_reg, length);      // Reg 级乘法
asc_cast_reg(z_reg, x_reg, length);            // Reg 级类型转换
```

**Reg 示例目录**
| 目录 | 功能 |
|-----|------|
| `02_features/03_basic_api/03_reg_vector_compute/` | Reg 向量计算示例 |
| `04_best_practices/02_reg_vector_compute_practices/` | Reg 性能优化 |

## 8. Host 侧调用流程

### 8.1 基础流程

```cpp
#include "acl/acl.h"

int main(int argc, char* argv[])
{
    // 1. ACL 初始化
    aclInit(nullptr);
    int32_t deviceId = 0;
    aclrtSetDevice(deviceId);
    aclrtStream stream = nullptr;
    aclrtCreateStream(&stream);
    
    // 2. 内存分配
    float* xDevice = nullptr;
    uint8_t* xHost = nullptr;
    size_t byteSize = totalLength * sizeof(float);
    
    aclrtMallocHost((void**)(&xHost), byteSize);                      // Host 内存
    aclrtMalloc((void**)&xDevice, byteSize, ACL_MEM_MALLOC_HUGE_FIRST); // Device 内存
    
    // 3. 数据拷贝：Host -> Device
    aclrtMemcpy(xDevice, byteSize, xHost, byteSize, ACL_MEMCPY_HOST_TO_DEVICE);
    
    // 4. 核函数调用
    kernel<<<numBlocks, nullptr, stream>>>(xDevice, yDevice, zDevice);
    aclrtSynchronizeStream(stream);  // 同步等待
    
    // 5. 数据拷贝：Device -> Host
    aclrtMemcpy(zHost, byteSize, zDevice, byteSize, ACL_MEMCPY_DEVICE_TO_HOST);
    
    // 6. 内存释放
    aclrtFree(xDevice);
    aclrtFreeHost(xHost);
    
    // 7. ACL 反初始化
    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();
    
    return 0;
}
```

### 8.2 带 Tiling 和 Workspace 的流程

高阶 API 通常需要 Tiling 数据和 Workspace：

```cpp
int main(int argc, char* argv[])
{
    // 获取平台信息
    auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
    
    // 生成 Tiling 数据
    size_t tilingFileSize = sizeof(TCubeTiling);
    uint8_t* tilingBuf = (uint8_t*)malloc(tilingFileSize);
    GenerateTiling(ascendcPlatform, tilingBuf);
    
    // 计算 Workspace 大小
    size_t systemWorkspaceSize = ascendcPlatform->GetLibApiWorkSpaceSize();
    size_t workspaceSize = userWorkspaceSize + systemWorkspaceSize;
    
    uint32_t numBlocks = reinterpret_cast<TCubeTiling*>(tilingBuf)->usedCoreNum;
    
    // ACL 初始化
    aclInit(nullptr);
    aclrtSetDevice(deviceId);
    aclrtContext context;
    aclrtCreateContext(&context, deviceId);
    aclrtCreateStream(&stream);
    
    // 分配 Device 内存（包括 Workspace 和 Tiling）
    aclrtMalloc((void**)&workspaceDevice, workspaceSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void**)&tilingDevice, tilingFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(tilingDevice, tilingFileSize, tilingBuf, tilingFileSize, ACL_MEMCPY_HOST_TO_HOST);
    
    // 核函数调用（带 Workspace 和 Tiling）
    matmul_custom<<<numBlocks, nullptr, stream>>>(aDevice, bDevice, biasDevice, cDevice, 
                                                   workspaceDevice, tilingDevice);
    aclrtSynchronizeStream(stream);
    
    // 释放资源
    aclrtFree(workspaceDevice);
    aclrtFree(tilingDevice);
    aclrtDestroyContext(context);
    aclrtResetDevice(deviceId);
    aclFinalize();
    
    free(tilingBuf);
    return 0;
}
```

### 8.3 错误处理

```cpp
// 获取最近错误信息
const char* err = aclGetRecentErrMsg();
if (err != nullptr) {
    fprintf(stderr, "%s\n", err);
}

// 错误检查宏
#define CHECK_ACL(val) check_acl((val), #val, __FILE__, __LINE__)

void check_acl(aclError result, const char* func, const char* file, int line)
{
    if (result != ACL_SUCCESS) {
        fprintf(stderr, "ACL error at %s:%d code=%d(%s) \"%s\"\n", 
                file, line, result, aclGetRecentErrMsg(), func);
        exit(EXIT_FAILURE);
    }
}
```

## 9. 框架接入

### 9.1 PyTorch torch.library 接入

使用 PyTorch 的 `torch.library` 机制注册自定义算子：

```cpp
#include <torch/extension.h>
#include "acl/acl.h"
#include "kernel_operator.h"

// 核函数定义
__global__ __vector__ void add_custom(__gm__ half* x, __gm__ half* y, __gm__ half* z, uint32_t length);

// TORCH_LIBRARY 注册算子签名
TORCH_LIBRARY(ascendc_ops, m) {
    m.def("ascendc_add(Tensor x, Tensor y) -> Tensor");
}

// TORCH_LIBRARY_IMPL 绑定到 NPU 设备
TORCH_LIBRARY_IMPL(ascendc_ops, PrivateUse1, m) {
    m.impl("ascendc_add", TORCH_FN(ascendc_add_impl));
}

// 实现函数
at::Tensor ascendc_add_impl(const at::Tensor& x, const at::Tensor& y) {
    auto z = at::empty_like(x);
    
    // 获取 NPU Stream
    auto stream = c10_npu::getCurrentNPUStream();
    
    // 获取数据指针
    half* x_ptr = x.data_ptr<half>();
    half* y_ptr = y.data_ptr<half>();
    half* z_ptr = z.data_ptr<half>();
    
    // 调用核函数
    uint32_t length = x.numel();
    uint32_t numBlocks = 8;
    add_custom<<<numBlocks, nullptr, stream.stream>>>(x_ptr, y_ptr, z_ptr, length);
    
    return z;
}
```

**Python 测试脚本**
```python
import torch
import torch_npu

# 加载自定义算子库
torch.ops.load_library("./build/libascendc_add.so")

# 调用自定义算子
x = torch.randn(8, 2048, dtype=torch.float16).npu()
y = torch.randn(8, 2048, dtype=torch.float16).npu()
z = torch.ops.ascendc_ops.ascendc_add(x, y)

# 验证结果
expected = x + y
assert torch.allclose(z.cpu(), expected.cpu())
```

### 9.2 TensorFlow/ONNX 接入

框架接入示例目录：
- `examples/01_simd_cpp_api/02_features/02_framework/01_tensorflow/`
- `examples/01_simd_cpp_api/02_features/02_framework/02_onnx/`

## 10. 调试工具

### 10.1 printf 输出

```cpp
#include "debug/asc_printf.h"

// 支持多种格式
AscendC::printf("pointer %p\n", ptr);                      // 指针
AscendC::printf("blockIdx %u, blockIdx %u\n", mIterIdx, blockIdx);  // 无符号整型
AscendC::printf("value %f\n", value);                      // 浮点型
AscendC::printf("hex %x\n", value);                        // 十六进制
AscendC::printf("string %s\n", "[Debug info]");            // 字符串

// 在核函数中打印
__global__ __cube__ void kernel_func(...)
{
    AscendC::printf("Block %d processing...\n", AscendC::GetBlockIdx());
}
```

### 10.2 Assert 断言

```cpp
AscendC::ASSERT(condition && "Error message");
AscendC::ASSERT(GetBlockNum() != 0 && "block dim can not be zero!");
```

### 10.3 Tensor Dump

```cpp
// 输出 Tensor 数据
AscendC::DumpTensor(localTensor, 1, 32);  // 输出前 32 个元素，tag=1
AscendC::printf("[ DumpTensor in xLocal]\n");
AscendC::DumpTensor(xLocal, 1, 32);
```

### 10.4 CPU 调试模式

编译时添加 `-DCMAKE_ASC_RUN_MODE=cpu`：

```bash
cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..
make -j
./demo
```

**GDB 调试**
```bash
gdb --args ./demo

# GDB 命令
(gdb) set follow-fork-mode child    # 跟踪子进程（核函数）
(gdb) break Compute                   # 在 Compute 函数设置断点
(gdb) run
(gdb) print xLocal.GetValue(0)       # 打印 Tensor 值
(gdb) next                            # 单步执行
```

### 10.5 Profiling 性能分析

使用 msProf 工具采集性能数据：

```bash
msprof --application="./demo" --output=./prof_data
```

### 10.6 Sanitizer 内存检测

编译时添加 `--asan`：

```bash
bash build.sh --asan
```

### 10.7 Clock 计时

```cpp
uint64_t start_time = AscendC::GetClock();
// ... 计算 ...
uint64_t end_time = AscendC::GetClock();
uint64_t elapsed = end_time - start_time;
AscendC::printf("Kernel execution time: %lu cycles\n", elapsed);
```

### 10.8 Simulator 仿真

使用 CAmodel 进行 NPU 仿真（无需物理 NPU）：

```bash
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..
make -j
./demo
```

## 11. 编译选项

### 11.1 运行模式

| 选项 | 说明 |
|------|------|
| `-DCMAKE_ASC_RUN_MODE=npu` | NPU 真实运行（默认） |
| `-DCMAKE_ASC_RUN_MODE=cpu` | CPU 调试模式 |
| `-DCMAKE_ASC_RUN_MODE=sim` | NPU 仿真模式 |

### 11.2 架构选择

| 选项 | 架构 | 支持产品 |
|------|------|---------|
| `-DCMAKE_ASC_ARCHITECTURES=dav-2201` | 2201 | Atlas A2/A3 系列 |
| `-DCMAKE_ASC_ARCHITECTURES=dav-3510` | 3510 | Ascend 950PR/950DT |
| `-DCMAKE_ASC_ARCHITECTURES=dav-2002` | 2002 | Atlas 推理系列产品 |

### 11.3 条件编译

使用 `__NPU_ARCH__` 宏进行条件编译：

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
    // Atlas A2/A3 特定代码
    AscendC::LoadData2DParams loadDataParams;
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    // Ascend 950PR 特定代码
    AscendC::LoadData2DParamsV2 loadDataParams;
#endif
```

## 12. 数据类型

| 数据类型 | 说明 | 字节数 |
|---------|------|--------|
| `float` | 32 位浮点 | 4 |
| `half` | 16 位浮点 | 2 |
| `bfloat16_t` | 16 位脑浮点 | 2 |
| `int8_t` | 8 位整数 | 1 |
| `int16_t` | 16 位整数 | 2 |
| `int32_t` | 32 位整数 | 4 |
| `int64_t` | 64 位整数 | 8 |
| `uint8_t` | 8 位无符号整数 | 1 |
| `uint16_t` | 16 位无符号整数 | 2 |
| `uint32_t` | 32 位无符号整数 | 4 |

## 13. 性能优化最佳实践

### 13.1 矩阵计算优化

| 技术 | 说明 | 示例目录 |
|-----|------|---------|
| SplitK | K 维度并行切分 | `matmul_splitk` |
| SplitM | M 维度并行切分 | `matmul_splitm` |
| L2 Cache | L2 缓存优化 | `matmul_l2cache` |
| Double Buffer | 双缓冲优化 | `matmul_mn_double_buffer` |
| Async Iterate | 异步迭代 | `matmul_async_iterate` |
| 预加载 | 数据预加载 | `matmul_preload` |

### 13.2 向量计算优化

| 技术 | 说明 | 示例目录 |
|-----|------|---------|
| VF 循环优化 | VF 指令循环 | `02_reg_vector_compute_practices` |
| VF 双发 | 指令双发优化 | `VF指令双发优化` |
| VF 融合 | 融合优化 | `VF融合优化` |

### 13.3 内存访问优化

| 技术 | 说明 | 示例目录 |
|-----|------|---------|
| 减少无效搬运 | 避免冗余搬运 | `04_memory_access_practices` |
| UB 提升 | UB 缓冲优化 | `simt_gather_with_ub` |
| 连续访问 | 合并内存访问 | `memory_access_practices` |

### 13.4 SIMT/SIMD 混合编程

```cpp
// 使用 SIMT 处理分支逻辑，SIMD 处理计算密集部分
// 示例：matmul_gelu_high_performance, quant_group_matmul_high_performance
```

### 13.5 通用优化建议

1. **内存分配优化**
   - 使用 TPipe/TQue 管理内存队列，实现流水线并行
   - 合理设置 BUFFER_NUM（通常 2-3）平衡内存和性能

2. **同步优化**
   - 使用 `SetFlag/WaitFlag` 替代 `PipeBarrier` 实现细粒度同步
   - 避免不必要的全局同步

3. **计算优化**
   - 矩阵计算使用高阶 API（Matmul）
   - 满足对齐条件时使用 BasicBlock 版本

4. **SIMT 优化**
   - Warp 内线程协作（32 线程/warp）
   - 使用 Shared Memory 减少 GM 访问

## 14. 完整示例目录

### 14.1 SIMD C++ API 示例

| 目录 | 功能 |
|-----|------|
| `00_introduction/00_quickstart/hello_world` | Hello World |
| `00_introduction/01_vector/add` | 向量加法（基础 API） |
| `00_introduction/01_vector/add_tpipe_tque` | 向量加法（TPipe/TQue） |
| `00_introduction/02_matrix/matmul` | 矩阵乘法（基础 API） |
| `00_introduction/04_vector_reg/add` | Reg 向量计算 |
| `01_utilities/00_printf` | Printf 调试 |
| `01_utilities/03_cpudebug` | CPU 调试模式 |
| `01_utilities/04_profiling` | 性能分析 |
| `02_features/03_basic_api/00_data_movement` | 数据搬运示例 |
| `02_features/03_basic_api/01_matrix_compute` | 矩阵计算示例 |
| `02_features/02_framework/00_pytorch` | PyTorch 接入 |
| `03_libraries/00_matrix/matmul` | Matmul 高阶 API |
| `03_libraries/01_activation/softmax` | SoftMax 高阶 API |
| `04_best_practices/01_matrix_compute_practices` | 矩阵优化实践 |
| `04_best_practices/03_fusion_compute_practices` | 融合计算实践 |

### 14.2 C API 示例

| 目录 | 功能 |
|-----|------|
| `00_introduction/00_quickstart/hello_world_npu` | Hello World |
| `00_introduction/01_add/c_api_sync_add` | 同步 API 加法 |
| `00_introduction/01_add/c_api_async_add` | 异步 API 加法 |
| `00_introduction/04_reg_base_add_compute` | Reg API 加法 |

### 14.3 SIMT API 示例

| 目录 | 功能 |
|-----|------|
| `00_introduction/00_quickstart/hello_world_simt` | Hello World |
| `00_introduction/01_add` | SIMT 加法 |
| `01_utilities/00_printf` | Printf 调试 |
| `02_features/00_resource_management/basic_gather` | Gather 操作 |
| `02_features/01_api_features/02_atomic_operation/histogram` | 原子操作 Histogram |
| `03_best_practices/01_simt_matrix_transpose` | 矩阵转置 |

更多详细示例请参考 `examples/` 目录。