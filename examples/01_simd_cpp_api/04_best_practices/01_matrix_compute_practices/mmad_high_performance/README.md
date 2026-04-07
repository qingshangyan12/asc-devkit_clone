# MMAD 高性能算子样例

## 概述

本样例实现了一个高性能矩阵乘算子，采用三层流水线并行设计，充分利用昇腾 AI 处理器的硬件并行能力。

## 核心特性

### 1. 三层流水线设计

- **GM → L1**：大包搬运，使用 stepKa/stepKb 减少 GM→L1 搬运次数
- **L1 → L0**：小包搬运，每次循环都执行
- **L0 → CO1**：矩阵乘计算，每次循环都执行

### 2. 双缓冲优化

- **L1 双缓冲**：隐藏 GM→L1 搬运延迟
- **L0 双缓冲**：隐藏 L1→L0 搬运延迟
- **CO1 单缓冲**：每个 m,n 块只计算一次

### 3. 内存布局

| 逻辑位置 | 物理内存 | 大小 | 用途 |
|----------|----------|------|------|
| A1 | L1 Buffer | 512KB | 左矩阵 L1 缓冲区（与 B1 共享） |
| B1 | L1 Buffer | 512KB | 右矩阵 L1 缓冲区 |
| A2 | L0A Buffer | 64KB | 左矩阵 L0 缓冲区 |
| B2 | L0B Buffer | 64KB | 右矩阵 L0 缓冲区 |
| CO1 | L0C Buffer | 128KB | 结果缓冲区 |

### 4. 性能优化

- **大包搬运优化**：CopyInA 减少 87.5%，CopyInB 减少 75%
- **流水线并行**：GM→L1、L1→L0、计算并行执行
- **内存复用**：A1 和 B1 共享 L1 Buffer，A2 和 B2 使用独立的 L0 Buffer

## 算子规格

| 参数 | 说明 |
|------|------|
| 矩阵维度 | M = 8192, K = 8192, N = 8192 |
| 基础块大小 | baseM = 128, baseK = 64, baseN = 256 |
| 单核计算量 | singleCoreM = 2048, singleCoreK = 8192, singleCoreN = 1536 |
| 大包搬运步长 | stepKa = 8, stepKb = 4 |
| 核函数名 | mmad_custom |

## 循环结构

```cpp
for (uint32_t mIndex = 0; mIndex < mLoopCount; mIndex++) {
    for (uint32_t nIndex = 0; nIndex < nLoopCount; nIndex++) {
        for (uint32_t kIndex = 0; kIndex < kLoopCount; kIndex++) {
            // 1. GM → L1 大包搬运（只在 kIndex % stepKa == 0 时执行）
            CopyInA(a1Local, kIndex, mIndex);
            CopyInB(b1Local, kIndex, nIndex);

            // 2. L1 → L0 小包搬运（每次循环都执行）
            DataLoadA(a1LocalTmp, a2Local, mIndex, kIndex);
            DataLoadB(b1LocalTmp, b2Local, nIndex, kIndex);

            // 3. L0 → CO1 计算（每次循环都执行）
            Compute(cLocal, a2Local, b2Local, kIndex, mIndex, nIndex);
        }
        // 4. CO1 → GM 写回（k 循环结束后执行）
        CopyOut(cLocal, mIndex, nIndex);
    }
}
```

## 同步机制

本样例使用 SetFlag/WaitFlag 实现流水线同步：

- **MTE1_MTE2**：MTE1 等待 MTE2（L0 等待 L1 数据搬运完成）
- **M_MTE1**：M 等待 MTE1（计算等待 L0 数据加载完成）
- **MTE2_MTE1**：MTE2 等待 MTE1（L1 等待 L0 数据消费完成）
- **MTE1_M**：MTE1 等待 M（L0 等待计算完成）

## 编译运行

### 配置环境变量

```bash
source /usr/local/Ascend/cann/set_env.sh
```

### 样例执行

```bash
mkdir -p build && cd build
cmake ..; make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/golden.bin output/output.bin
```

### 编译参数

| 参数 | 说明 | | 可选值 | 默认值 |
|------|------|----------|----------|
| RUN_MODE | 运行模式 | npu、cpu、sim | npu |
| NPU_ARCH | NPU 硬件架构 | dav-2201、dav-3510 | dav-2201 |

## 技术细节

详细的技术分析请参考 [mmad_loop_level_analysis.md](mmad_loop_level_analysis.md)，包含：

- 硬件架构与内存布局
- 循环结构与粒度设计
- 同步机制深度分析
- 循环执行流程详细分析
- 性能优化分析
- 代码正确性验证
- 流水线并行示意图