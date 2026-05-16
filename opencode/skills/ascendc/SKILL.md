---
name: ascendc-master
description: 通过AscendC编程语言开发高精度、高性能API, 使用SIMD编程模式完成UB(United Buffer)上LocalTensor到寄存器RegTensor的拷贝，在RegTensor中完成计算，之后从RegTensor存回到UB。本技能仅考虑NPU架构为3510场景的AscendC API开发。当用户提到AscendC开发时，应使用此技能。
---

# 需求分析
分析用户的需求，明确函数原型，输入数据类型，输出数据类型，计算逻辑。重点关注函数标识符。

实现一个AscendC高阶api的过程如下：
- 处理输入，调用Reg::Load接口将输入数据从LocalTensor<T>加载到RegTensor<T>中。该部分函数的标识符应为__aicore__ inline, 标识函数在device侧执行，返回值应为void。
- 计算逻辑，根据用户需求实现计算逻辑。该部分函数的标识符应为__simd_vf__ inline, 标识函数在device侧执行，返回值应为void。
- 处理输出，调用Reg::Store接口将RegTensor<T>中的结果存储到LocalTensor<T>中。

# AscendC API 开发技能

## 概述

本技能用于 AscendC 昇腾AI处理器算子程序开发。AscendC 是 CANN 推出的面向昇腾AI处理器的算子开发语言，原生支持 C/C++ 标准规范，提供多层级 API 接口以覆盖从快速验证到极致性能优化的全场景开发需求。本技能仅考虑RegBase的AscendC的高阶API开发。

## 编程模式

- **SIMD（单指令多数据）**：本技能仅使用SIMD编程模式，即一次操作一个长度为256B的Tensor数据。

## 内存层次

- GM (Global Memory / DDR)  ←→  UB (Unified Buffer / 片上缓存)  ←→  Reg (寄存器)
- 本技能仅考虑UB (Unified Buffer / 片上缓存)  ←→  Reg (寄存器)的数据转移，不考虑GM (Global Memory / DDR)  ←→  UB (Unified Buffer / 片上缓存)的数据转移。

## 开发流程

1. **理解算法逻辑**：先用标准 C++ 写出标量的参考实现
2. **验证C++算法**：验证C++算法在不同场景下的正确性
3. **编写实现代码**：将C++代码转换为AscendC代码，注意函数标识符和编译器限制。

## 知识库文件

本技能附带以下知识库文件，开发时应根据需要查阅：

| 文件 | 内容 |
|------|------|
| [reference/api-guide.md](reference/api-guide.md) | C++ 代码与 AscendC API 的完整对应关系 |
| [reference/bisheng-guide.md](reference/bisheng-guide.md) | 毕昇编译器相关知识与约束 |
| [reference/arch-guide.md](reference/arch-guide.md) | Ascend 950 硬件架构及编程应用 |
| [examples/tensor-ops.md](examples/tensor-ops.md) | C++ 到 AscendC 的 Tensor 操作转换示例 |

## 工作准则

1. 在编写 AscendC 代码前，先查阅 `reference/` 下的相关知识库文件
2. 新编写的内核代码优先参考 `examples/` 中的示例模式