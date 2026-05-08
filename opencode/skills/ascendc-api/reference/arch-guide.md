# Ascend 950 硬件架构知识

本文档介绍 Ascend 950 系列（代号 3510）昇腾AI处理器的硬件架构，以及在 AscendC 编程中的实际应用。

---

## 1. 芯片概览

### 1.1 架构代号对照

| 架构代号 | 产品系列 | __NPU_ARCH__ |
|---------|---------|-------------|
| 2001 | Atlas A2 系列 (Ascend 910B) | 2200/2201 |
| 3510 | Ascend 950 系列 (PR/DT) | 3510 |

### 1.2 Ascend 950 关键特性

- **SIMD + SIMT 混合编程**：同一芯片内同时支持向量计算和线程计算模式
- **Reg 编程模式**：新增基于寄存器的编程方式，提供 90+ Reg API 接口
- **CCU 通信**：片间高速通信，支持 Allreduce、Allgather 等集合通信原语
- **MXFP4/8**：低比特数据类型支持，矩阵运算内存占用减半
- **约 700 SIMT API**：包含 warp、atomic、数学计算、类型转换等

---

## 2. Da Vinci 架构核心组件

每个 AICore 包含以下计算单元：

```
┌──────────────────────────────────────────┐
│                  AICore                    │
│  ┌─────────┐ ┌──────────┐ ┌───────────┐  │
│  │  Scalar  │ │  Vector  │ │   Cube    │  │
│  │  单元    │ │   单元    │ │   单元    │  │
│  │ (标量)   │ │ (向量)   │ │ (矩阵乘)  │  │
│  └─────────┘ └──────────┘ └───────────┘  │
│  ┌─────────────────────────────────────┐  │
│  │         统一缓冲区 (UB)              │  │
│  │        (~192KB - 256KB)             │  │
│  └─────────────────────────────────────┘  │
│  ┌─────────────────────────────────────┐  │
│  │          寄存器文件 (Reg)            │  │
│  └─────────────────────────────────────┘  │
└──────────────────────────────────────────┘
         ↕ DMA
┌──────────────────────────────────────────┐
│           全局内存 (GM / DDR)             │
│           (HBM, 多GB级)                  │
└──────────────────────────────────────────┘
```

### 2.1 Scalar 单元（标量计算单元）

- 处理标量运算：地址计算、循环控制、分支判断
- 执行流水控制指令（SetFlag/WaitFlag）
- AscendC 中对应 `Scalar` 类和标量运算 API

### 2.2 Vector 单元（向量计算单元）

- 执行 SIMD 向量运算（逐元素运算）
- 支持数据类型：fp32, fp16, bf16, int32, int8 等
- 一个指令周期处理 256B 数据（具体取决于数据类型和架构版本）
- AscendC 中对应 `LocalTensor` 上的各种向量 API

### 2.3 Cube 单元（矩阵计算单元）

- 专门用于矩阵乘法和卷积运算
- 硬件实现的矩阵乘法加速器
- 最高效的数据格式：**NZ**（N1Z）格式
- AscendC 中对应 `Matmul`、`Conv2d` 等 API
- 支持低精度格式：MXFP4, MXFP8

---

## 3. 内存层次结构

```
寄存器 (Register)       < 1KB    延迟: 0 cycle     ---- 最快/最小
     ↕
统一缓冲区 (UB)        ~192-256KB  延迟: ~数十 cycles  片上SRAM
     ↕ DMA
全局内存 (GM/HBM)       多GB级     延迟: ~数百 cycles  片外内存   最慢/最大
```

### 3.1 统一缓冲区 (UB)

- 片上高速 SRAM，每个 AICore 独立
- **关键特性**：同时被 Scalar、Vector、Cube 单元共享访问
- 典型可用容量 ~192KB-256KB（部分预留给系统使用）
- 编程中通过 `__ubuf__` 修饰符或 `LocalTensor` 访问

### 3.2 全局内存 (GM/DDR)

- 片外 HBM 内存，由所有 AICore 共享
- Host 端通过 ACL API 分配和管理
- 编程中通过 `__gm__` 修饰符或 `GlobalTensor` 访问

### 3.3 UB 容量规划

在设计 tile 大小时需要考虑：

```
UB 可用空间 = 总UB容量 - 系统预留 - 程序其他数据
总tile大小  = (输入数据 + 输出数据) * 数据类型大小 + 中间缓冲区
```

对于 double buffer，tile 大小减半。

---

## 4. SIMD 编程模式

### 4.1 核心概念

- 一个 AICore 作为一个独立的计算单元（类似一个 CUDA block 但只有一个线程）
- 通过向量指令（一次操作 256B 数据）实现数据并行
- **没有**线程概念——不使用 blockIdx + threadIdx 模式

### 4.2 多核并行

```cpp
// 获取当前核索引
uint32_t blockIdx = AscendC::GetBlockIdx();
uint32_t blockNum = AscendC::GetBlockNum();

// 根据核索引分配数据范围
uint32_t start = blockIdx * perBlockLen;
uint32_t end = start + perBlockLen;
```

### 4.3 SIMD 编程模型图解

```
数据:  [======= 完整Tensor =======]
        |  Core0  |  Core1  | ... |
              ↓          ↓
        [搬运→UB]  [搬运→UB]
        [向量计算]  [向量计算]
        [搬回→GM]  [搬回→GM]
```

---

## 5. SIMT 编程模式（Ascend 950 新增）

### 5.1 核心概念

- 类 CUDA 编程模型：block + thread 二维索引
- 每个线程独立执行标量指令
- Warp 内线程 SIMT 并行

### 5.2 SIMT 内核示例

```cpp
__global__ void vec_add(float* x, float* y, float* z, uint64_t n) {
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= n) return;
    z[idx] = x[idx] + y[idx];
}

// 调用: vec_add<<<gridDim, blockDim, dynUbSize, stream>>>(...);
```

### 5.3 SIMD vs SIMT 选择

| 场景 | 推荐模式 | 原因 |
|------|---------|------|
| 连续密集计算（如 Add, Mul, Conv） | SIMD | 向量化效率最高 |
| 离散/不规则访问（如 Gather, Scatter） | SIMT | 线程可独立寻址 |
| 混合场景 | 混合编程 | 各取所长 |

---

## 6. Reg 编程模式（Ascend 950 新增）

### 6.1 概念

Reg API 提供对 Vector 计算中寄存器的直接操作能力，绕过 Tensor 抽象层，实现最大灵活性。

### 6.2 Reg API 使用场景

- 需要手动控制寄存器分配和重用
- 自定义数据通路和计算模式
- 与底层硬件指令直接映射的场景

### 6.3 Reg API 示例

```cpp
// Reg API：直接操作寄存器
Reg src0, src1, dst;
Reg::LoadAlign(src0, addr0);
Reg::LoadAlign(src1, addr1);
Reg::Add(dst, src0, src1, mask);   // mask 控制哪些 lane 参与计算
Reg::StoreAlign(addr_dst, dst, mask);
```

### 6.4 Mask 寄存器

Mask 是 Reg 编程的重要概念：

```cpp
// 创建全1 mask（所有 lane 生效）
Reg::CreateMask<T, Reg::MaskPattern::ALL>();

// 创建前N个 lane 生效的 mask
Reg::UpdateMask<T>(n);  // mask[i] = (i < n)

// 条件执行的 mask
Reg::Compare<T, CMPMODE::GT>(maskGt, a, b, inputMask);
// 只有 a[i] > b[i] 的 lane 在 maskGt 中为1
```

---

## 7. 数据格式

### 7.1 ND 格式（Normal Data）

- 标准的行主序排列
- 适用于常规向量计算
- 编程中对齐要求：32B

### 7.2 NZ 格式（N1Z）

- 专为 Cube 单元优化的数据布局
- 将矩阵分块重排提高矩阵乘效率
- 需要通过 `DataCopyPad` 在搬运时转换

```cpp
// ND → NZ 格式转换
AscendC::DataCopyPad(nzTensor, ndTensor);  // 搬运同时转换格式
```

### 7.3 低精度格式

| 格式 | 位宽 | 用途 | 支持架构 |
|------|------|------|---------|
| FP32 | 32b | 训练/高精推理 | 全部 |
| FP16 | 16b | 推理/训练 | 全部 |
| BF16 | 16b | 训练 | 全部 |
| INT8 | 8b | 量化推理 | 全部 |
| MXFP4 | 4b | 极低精度推理 | Ascend 950 |
| MXFP8 | 8b | 低精度推理 | Ascend 950 |

---

## 8. 同步与流水线

### 8.1 流水线类型

| 流水线类型 | PIPE_M | PIPE_V | PIPE_S |
|-----------|--------|--------|--------|
| 负责单元 | Cube | Vector | Scalar/DMA |
| 主要操作 | 矩阵乘 | 向量计算 | 数据搬运/标量 |

### 8.2 同步原语

```cpp
// 标量/搬运流水同步
AscendC::SetFlag<PIPE_S>(event);
AscendC::WaitFlag<PIPE_S>(event);

// 向量流水同步
AscendC::SetFlag<PIPE_V>(event);
AscendC::WaitFlag<PIPE_V>(event);

// Cube 流水同步
AscendC::SetFlag<PIPE_M>(event);
AscendC::WaitFlag<PIPE_M>(event);

// 全流水屏障（确保前面的所有操作完成）
AscendC::PipeBarrier<PIPE_ALL>();
```

### 8.3 块间同步（跨核同步）

```cpp
// 所有核到达此处后才继续执行
AscendC::BlockBarrier();
```

---

## 9. CCU 通信（Ascend 950）

### 9.1 概念

CCU（Cube Communication Unit）是 Ascend 950 的片间高速互连通道，支持高效的集合通信。

### 9.2 支持的通信原语

- `Allreduce`：所有核的数据归约后广播
- `Allgather`：所有核收集所有数据
- `ReduceScatter`：归约后分散到各核
- `AlltoAll`：全交换

### 9.3 使用要点

```cpp
// CCU 通信需使用高阶通信 API
// 具体接口参见 impl/adv_api/detail/hccl/impl/platform_v310
```

---

## 10. 架构特性在编程中的应用总结

| 架构特性 | 编程影响 | 参考API层级 |
|---------|---------|-----------|
| 三级内存层次 | tile 分块策略核心依据 | 所有层级 |
| Vector 单元 | 逐元素操作尽量用向量API | 基础API, C API |
| Cube 单元 | 矩阵乘必须用NZ格式 | 基础API |
| 多 AICore | 数据平均分配到各核 | 所有层级 |
| 三路流水线 | 计算/搬运可重叠 | 基础API |
| Reg 编程 | 极致优化场景使用 | Reg API |
| SIMT 模式 | 离散类算子用线程模式 | SIMT API |
| CCU 通信 | 多卡通信用集合原语 | 高阶API |
