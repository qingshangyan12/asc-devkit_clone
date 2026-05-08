# AscendC API 开发技能

## 概述

本技能用于 AscendC 昇腾AI处理器算子程序开发。AscendC 是 CANN 推出的面向昇腾AI处理器的算子开发语言，原生支持 C/C++ 标准规范，提供多层级 API 接口以覆盖从快速验证到极致性能优化的全场景开发需求。

## 适用场景

当用户需要以下任务时，应调用本技能：

1. 编写 AscendC 算子内核代码（`.asc` 文件）
2. 将标准 C++ 代码转换为 AscendC API 调用
3. 理解 AscendC 多层级 API 的选择与使用
4. 编译、调试 AscendC 算子程序
5. 优化 AscendC 算子的性能
6. 了解 Ascend 950 硬件架构及其编程约束

## 核心概念

### 多层级 API 体系

AscendC 提供三层API，根据开发需求灵活选择：

| API层级 | 语言 | 编程范式 | 同步管理 | 适用场景 |
|---------|------|---------|---------|---------|
| **语言扩展层 C API** | C | 指针 + 数组 `[]` | 自主管理 | 极致性能、C语言习惯 |
| **基础API** | C++ | Tensor (LocalTensor) | 自主管理 | 极致性能、C++ Tensor习惯 |
| **Tpipe/Tque 框架API** | C++ | Tensor (LocalTensor) | 框架自动 | 易用性优先、快速开发 |

此外还有：
- **高阶API**：封装通用单核算法的预置接口，快速算法验证
- **算子模板库 (CATLASS/ATVOSS)**：典型算子的完整实现参考

### 编程模式

- **SIMD（单指令多数据）**：向量化计算，适合连续、规整的算子
- **SIMT（单指令多线程）**：类CUDA编程，适合离散、不规则算子
- **混合编程**：Ascend 950PR 支持 SIMD + SIMT 混合，同一算子内同时使用

### 内存层次

```
GM (Global Memory / DDR)  ←→  UB (Unified Buffer / 片上缓存)  ←→  Reg (寄存器)
```

### 内核调用语法

```cpp
kernel_name<<<blocks, threads, dyn_ubuf, stream>>>(args...);
```

## 开发流程

1. **理解算法逻辑**：先用标准 C++ 写出参考实现
2. **选择 API 层级**：参考 [API选择指南](docs/asc_how_to_choose_api.md)
3. **编写内核代码**：在 `.asc` 文件中实现 `__global__` 内核函数
4. **编写 Host 端代码**：在 `.cpp` 文件中实现数据准备和内核调用
5. **编译运行**：使用 CMake + 毕昇编译器构建，在 NPU 或 CPU/SIM 仿真模式运行
6. **调试优化**：基于 profiler 数据迭代优化

## 知识库文件

本技能附带以下知识库文件，开发时应根据需要查阅：

| 文件 | 内容 |
|------|------|
| [reference/api-guide.md](reference/api-guide.md) | C++ 代码与 AscendC API 的完整对应关系 |
| [reference/bisheng-guide.md](reference/bisheng-guide.md) | 毕昇编译器相关知识与约束 |
| [reference/optimize-guide.md](reference/optimize-guide.md) | 性能优化注意事项与最佳实践 |
| [reference/arch-guide.md](reference/arch-guide.md) | Ascend 950 硬件架构及编程应用 |
| [examples/tensor-ops.md](examples/tensor-ops.md) | C++ 到 AscendC 的 Tensor 操作转换示例 |

## 工作准则

1. 在编写 AscendC 代码前，先查阅 `reference/` 下的相关知识库文件
2. 新编写的内核代码优先参考 `examples/` 中的示例模式
3. 注意 AscendC 的 API 命名约定（区分C API和C++ API）
4. 保持对 API 层级的清晰认知：从高层级到低层级，控制粒度递增
5. 涉及编译器问题时，查阅 `bisheng-guide.md`
6. 涉及性能问题时，查阅 `optimize-guide.md`
7. 涉及硬件特性利用时，查阅 `arch-guide.md`
