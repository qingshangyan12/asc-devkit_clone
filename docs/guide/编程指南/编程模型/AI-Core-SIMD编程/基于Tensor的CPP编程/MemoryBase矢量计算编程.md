# MemoryBase矢量计算编程

## 简介

MemoryBase矢量计算API是面向内存架构开发的API，用户可以通过该API直接在Unified Buffer上进行矢量计算，无需显式管理寄存器数据搬运。MemoryBase矢量计算API与RegBase API功能相似，但输入和输出数据均为LocalTensor（存储在Unified Buffer），而非寄存器。

对于计算类API，其功能是从给定的LocalTensor获取数据，进行计算，并将结果保存到目标LocalTensor。对于搬运类API，其功能是实现Global Memory和Unified Buffer之间的数据搬运。MemoryBase矢量计算API相较于RegBase API，由框架自动管理数据搬运和计算过程，从而简化开发流程，降低编程难度。

---

## MemoryBase编程模型

基于内存的编程模型是Ascend C的传统编程模型，用户通过基础API和高阶API直接操作Unified Buffer进行矢量计算。在Membase架构中，所有操作均基于内存进行，每次计算都直接在Unified Buffer上完成，中间计算结果也暂存在Unified Buffer上。

在SIMD Vector的内存架构中，Unified Buffer是Vector计算单元的数据枢纽，位于AI Core内部，具有高带宽、低延迟的特点。Unified Buffer与Global Memory之间通过DMA搬运单元进行数据传输，Vector计算单元直接访问Unified Buffer进行矢量计算。

**图1 SIMD MemoryBase矢量计算内存层级**

![](../../../../figures/SIMD-MemoryBase矢量计算内存层级.png)

SIMD MemoryBase矢量计算编程架构中，通过发出指令到Vector计算单元，执行单元直接从Unified Buffer读取数据，进行计算，计算结果写回Unified Buffer。DMA搬运单元负责在Global Memory和Unified Buffer之间搬运数据。

**图2 SIMD MemoryBase矢量计算编程架构**

![](../../../../figures/抽象硬件架构.png)

---

## MemoryBase和RegBase编程对比

### 数据处理流程对比

**MemoryBase架构**：
- 所有操作均基于Unified Buffer进行
- 无需数据切分，可直接处理完整长度的LocalTensor
- 中间计算结果暂存在Unified Buffer
- 由框架自动管理数据搬运和计算过程

**RegBase架构**：
- 计算操作基于寄存器进行
- 需要数据切分，每次只能处理VL长度数据
- 中间计算结果可暂存在寄存器
- 用户自主控制数据搬运和计算过程

### API调用层级对比

在MemoryBase架构中：
- 高阶API调用基础API实现功能
- 基础API调用编译器BuiltIn API实现功能
- 用户可以直接调用基础API或高阶API

在RegBase架构中：
- Reg矢量计算API直接调用编译器BuiltIn API
- 高阶API和基础API可以调用Reg矢量计算API
- 用户可以在simd vf函数内调用Reg矢量计算API

![](../../../../figures/API-调用对比.png)

### 性能对比

| 特性 | MemoryBase | RegBase |
|------|-----------|---------|
| **开发难度** | 低，自动管理数据搬运 | 高，手动管理寄存器 |
| **数据切分** | 不需要，直接处理完整数据 | 需要，按VL切分数据 |
| **中间结果存储** | Unified Buffer | 寄存器或Unified Buffer |
| **数据搬运开销** | 较高，每次计算需访问UB | 较低，寄存器间操作 |
| **适用场景** | 通用算子开发 | 极致性能优化 |

---

## MemoryBase调用层次

MemoryBase编程模型的调用层次如下：

- **核函数**：使用`__global__ __aicore__`标识，是Device侧的入口函数，Host侧可以通过`<<<...>>>`语法进行调用。
- **`__aicore__`函数**：使用`__aicore__`标识，在Device侧执行。核函数内可以调用`__aicore__`函数。
- **Compute函数**：在`__aicore__`函数内调用基础API或高阶API进行矢量计算。

具体的调用关系图如下：

![](../../../../figures/memorybase_call_hierarchy.png)

以下为MemoryBase编程的典型调用链：

```
核函数(__global__ __aicore__)
  → __aicore__函数
    → Process函数
      → CopyIn函数（数据搬入）
      → Compute函数（矢量计算）
      → CopyOut函数（数据搬出）
```

---

## MemoryBase数据结构

### LocalTensor

LocalTensor是MemoryBase矢量计算的基本数据结构，用于表示存储在Unified Buffer上的数据。LocalTensor通过队列管理，支持自动内存分配和释放。

**特点**：
- 数据存储在Unified Buffer
- 支持多种数据类型（half、float、int8_t等）
- 通过Que队列管理内存生命周期
- 地址自动32字节对齐

**使用示例**：
```cpp
AscendC::LocalTensor<half> dst = outQueue.AllocTensor<half>();
AscendC::LocalTensor<half> src0 = inQueue0.DeQue<half>();
AscendC::LocalTensor<half> src1 = inQueue1.DeQue<half>();
```

### GlobalTensor

GlobalTensor用于表示存储在Global Memory上的数据，主要用于数据搬运操作。

**特点**：
- 数据存储在Global Memory
- 支持与LocalTensor之间的数据搬运
- 支持多种数据布局（ND、NZ等）

**使用示例**：
```cpp
AscendC::GlobalTensor<half> xGlobal;
xGlobal.SetGlobalBuffer((__gm__ half*)xGm);
AscendC::DataCopy(xLocal, xGlobal, 512);
```

### TPipe & TQue

TPipe和TQue是MemoryBase编程模型的内存管理框架，用于管理数据在Unified Buffer上的存储和流动。

**TPipe**：管道管理器，负责协调多个队列之间的数据流动。

**TQue**：队列管理器，负责管理Unified Buffer上的内存分配和释放。

**使用示例**：
```cpp
AscendC::TPipe pipe;
AscendC::TQue<AscendC::TPosition::VECIN, 2> inQueue;
AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueue;

pipe.InitBuffer(inQueue, BUFFER_NUM, BUFFER_SIZE);
pipe.InitBuffer(outQueue, BUFFER_NUM, BUFFER_SIZE);
```

---

## MemoryBase基础API

MemoryBase矢量计算API包括以下几类：

### 1. 数据搬运API

**DataCopy**：实现Global Memory和Unified Buffer之间的数据搬运。

```cpp
// GM → UB
AscendC::DataCopy(dstLocal, srcGlobal, dataSize);

// UB → GM
AscendC::DataCopy(dstGlobal, srcLocal, dataSize);
```

**特点**：
- 自动管理DMA搬运
- 支持多种数据布局
- 支持批量搬运和stride搬运

---

### 2. 矢量计算API

**Add**：矢量加法。

```cpp
AscendC::Add(dstLocal, src0Local, src1Local, count);
```

**Sub**：矢量减法。

```cpp
AscendC::Sub(dstLocal, src0Local, src1Local, count);
```

**Mul**：矢量乘法。

```cpp
AscendC::Mul(dstLocal, src0Local, src1Local, count);
```

**Div**：矢量除法。

```cpp
AscendC::Div(dstLocal, src0Local, src1Local, count);
```

**特点**：
- 输入输出均为LocalTensor
- 自动管理计算流程
- 支持多种数据类型

---

### 3. 高阶API

高阶API封装了复杂的计算逻辑，简化开发流程。

**AddCustom**：自定义矢量加法算子。

```cpp
AscendC::AddCustom<T>(dstLocal, src0Local, src1Local, count);
```

**MatMul**：矩阵乘法。

```cpp
AscendC::MatMul(dstLocal, src0Local, src1Local, m, n, k);
```

**特点**：
- 封装复杂计算逻辑
- 提供更高级的抽象
- 减少用户代码量

---

## 流水线同步控制

在MemoryBase编程模型中，数据搬运和矢量计算分别在不同的流水线上执行。为了保证数据一致性，需要插入同步指令。

### 流水线类型

| 流水线 | 说明 |
|--------|------|
| **PIPE_MTE2** | GM → UB数据搬运流水 |
| **PIPE_MTE3** | UB → GM数据搬运流水 |
| **PIPE_V** | 矢量计算流水 |

### 同步指令

**PipeBarrier**：流水线同步指令，确保前序流水线操作完成后再执行后续操作。

```cpp
AscendC::PipeBarrier<PIPE_TYPE>();
```

**使用示例**：
```cpp
// 数据搬入（PIPE_MTE2）
AscendC::DataCopy(srcLocal, srcGlobal, dataSize);
AscendC::PipeBarrier<PIPE_MTE2>();

// 矢量计算（PIPE_V）
AscendC::Add(dstLocal, src0Local, src1Local, count);
AscendC::PipeBarrier<PIPE_V>();

// 数据搬出（PIPE_MTE3）
AscendC::DataCopy(dstGlobal, dstLocal, dataSize);
AscendC::PipeBarrier<PIPE_MTE3>();
```

---

## 如何使用MemoryBase API

基于内存的编程模型是指通过基础API或高阶API直接在Unified Buffer上进行矢量计算，无需显式管理寄存器。具体流程如下：

![](../../../../figures/memorybase_flow.png)

### 编程流程

**步骤1：初始化管道和队列**

```cpp
AscendC::TPipe pipe;
AscendC::TQue<AscendC::TPosition::VECIN, 2> inQueue;
AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueue;

pipe.InitBuffer(inQueue, BUFFER_NUM, BUFFER_SIZE);
pipe.InitBuffer(outQueue, BUFFER_NUM, BUFFER_SIZE);
```

**步骤2：数据搬入**

```cpp
AscendC::LocalTensor<half> xLocal = inQueue.AllocTensor<half>();
AscendC::DataCopy(xLocal, xGlobal, TILE_LENGTH);
inQueue.EnQue(xLocal);
```

**步骤3：矢量计算**

```cpp
AscendC::LocalTensor<half> xLocal = inQueue.DeQue<half>();
AscendC::LocalTensor<half> yLocal = inQueue.DeQue<half>();
AscendC::LocalTensor<half> zLocal = outQueue.AllocTensor<half>();

AscendC::Add(zLocal, xLocal, yLocal, TILE_LENGTH);

outQueue.EnQue<half>(zLocal);
inQueue.FreeTensor(xLocal);
inQueue.FreeTensor(yLocal);
```

**步骤4：数据搬出**

```cpp
AscendC::LocalTensor<half> zLocal = outQueue.DeQue<half>();
AscendC::DataCopy(zGlobal, zLocal, TILE_LENGTH);
outQueue.FreeTensor(zLocal);
```

---

## MemoryBase编程示例

以下是一个完整的MemoryBase矢量加法算子示例：

### Add算子实现

```cpp
template <typename T>
class KernelAdd {
public:
    __aicore__ inline KernelAdd() {}
    __aicore__ inline void Init(GM_ADDR x, GM_ADDR y, GM_ADDR z, uint32_t totalLength, uint32_t tileLength);
    __aicore__ inline void Process();

private:
    __aicore__ inline void CopyIn(uint32_t loopIdx);
    __aicore__ inline void Compute(uint32_t loopIdx);
    __aicore__ inline void CopyOut(uint32_t loopIdx);

    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, 2> inQueueX, inQueueY;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueZ;

    AscendC::GlobalTensor<T> xGlobal, yGlobal, zGlobal;
    uint32_t totalLength;
    uint32_t tileLength;
    uint32_t tileNum;
};

template <typename T>
__aicore__ inline void KernelAdd<T>::Init(GM_ADDR x, GM_ADDR y, GM_ADDR z, uint32_t totalLength, uint32_t tileLength)
{
    this->totalLength = totalLength;
    this->tileLength = tileLength;
    this->tileNum = totalLength / tileLength;

    // 初始化管道
    pipe.InitBuffer(inQueueX, BUFFER_NUM, tileLength * sizeof(T));
    pipe.InitBuffer(inQueueY, BUFFER_NUM, tileLength * sizeof(T));
    pipe.InitBuffer(outQueueZ, BUFFER_NUM, tileLength * sizeof(T));

    // 初始化GlobalTensor
    xGlobal.SetGlobalBuffer((__gm__ T*)x);
    yGlobal.SetGlobalBuffer((__gm__ T*)y);
    zGlobal.SetGlobalBuffer((__gm__ T*)z);
}

template <typename T>
__aicore__ inline void KernelAdd<T>::Process()
{
    uint32_t loopNum = tileNum;

    for (uint32_t i = 0; i < loopNum; i++) {
        CopyIn(i);
        Compute(i);
        CopyOut(i);
    }
}

template <typename T>
__aicore__ inline void KernelAdd<T>::CopyIn(uint32_t loopIdx)
{
    // 从队列申请内存
    AscendC::LocalTensor<T> xLocal = inQueueX.AllocTensor<T>();
    AscendC::LocalTensor<T> yLocal = inQueueY.AllocTensor<T>();

    // 数据搬入GM → UB
    AscendC::DataCopy(xLocal, xGlobal[loopIdx * tileLength], tileLength);
    AscendC::DataCopy(yLocal, yGlobal[loopIdx * tileLength], tileLength);

    // 入队
    inQueueX.EnQue(xLocal);
    inQueueY.EnQue(yLocal);
}

template <typename T>
__aicore__ inline void KernelAdd<T>::Compute(uint32_t loopIdx)
{
    // 出队
    AscendC::LocalTensor<T> xLocal = inQueueX.DeQue<T>();
    AscendC::LocalTensor<T> yLocal = inQueueY.DeQue<T>();

    // 申请输出内存
    AscendC::LocalTensor<T> zLocal = outQueueZ.AllocTensor<T>();

    // 矢量计算
    AscendC::Add(zLocal, xLocal, yLocal, tileLength);

    // 入队并释放输入内存
    outQueueZ.EnQue<T>(zLocal);
    inQueueX.FreeTensor(xLocal);
    inQueueY.FreeTensor(yLocal);
}

template <typename T>
__aicore__ inline void KernelAdd<T>::CopyOut(uint32_t loopIdx)
{
    // 出队
    AscendC::LocalTensor<T> zLocal = outQueueZ.DeQue<T>();

    // 数据搬出UB → GM
    AscendC::DataCopy(zGlobal[loopIdx * tileLength], zLocal, tileLength);

    // 释放内存
    outQueueZ.FreeTensor(zLocal);
}
```

### 核函数入口

```cpp
extern "C" __global__ __aicore__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z, GM_ADDR tiling)
{
    GM_ADDR tiling_gm = (__gm__ uint32_t*)tiling;
    uint32_t totalLength = tiling_gm[0];
    uint32_t tileLength = tiling_gm[1];

    KernelAdd<half> op;
    op.Init(x, y, z, totalLength, tileLength);
    op.Process();
}
```

---

## MemoryBase vs RegBase选择指南

### 选择MemoryBase的场景

- 通用算子开发，追求开发效率
- 中间计算结果较多，需要频繁访问UB
- 数据长度不规则，无法按VL切分
- 初学者快速上手Ascend C开发

### 选择RegBase的场景

- 极致性能优化，追求硬件峰值性能
- 简单计算逻辑，中间结果少
- 数据长度固定，可以按VL切分
- 需要精细控制寄存器和流水线

---

## 总结

MemoryBase矢量计算编程是Ascend C的传统编程模型，通过基础API和高阶API直接在Unified Buffer上进行矢量计算。相较于RegBase编程模型，MemoryBase具有以下优势：

1. **开发简单**：无需显式管理寄存器，降低编程难度
2. **流程简化**：可直接处理完整数据，无需切分
3. **易于调试**：数据流转清晰，便于问题定位
4. **通用性强**：适用于各种算子开发场景

但MemoryBase也有一定的局限性：

1. **性能开销**：每次计算需访问UB，数据搬运开销较大
2. **灵活性低**：无法精细控制寄存器和流水线
3. **中间结果存储**：所有中间结果需暂存UB，占用内存资源

开发者应根据实际场景选择合适的编程模型，在开发效率和性能之间找到平衡。