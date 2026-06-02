# Ascend C API Programming Guide

**Ascend C API and the Ascend C Programming Guide**

Ascend C API is a low-level programming interface for Ascend NPU (Neural Processing Unit) developed by Huawei that enables high-performance computing by leveraging the AI Core architecture. It provides direct access to hardware resources through C-language function calls, offering fine-grained control over memory hierarchy, data movement, and computational units.

This Ascend C API Programming Guide is the official, comprehensive resource on the Ascend C programming model and how to write code that executes on the AI Core using the Ascend C API. This guide covers everything from the Ascend C programming model and the AI Core hardware to the details of API usage and covers how to make use of specific computational units and memory features.

**Organization of This Guide**

This guide follows the CUDA C Programming Guide structure and is organized into five primary parts:

- **Part 1: Introduction to Ascend C**
    
    - A language-agnostic overview of the Ascend C programming model and the AI Core architecture.
        
    - This section is meant to be read by anyone wanting to understand AI Core architecture and the concepts of executing code on Ascend NPU, even if they are not developers.
        
- **Part 2: Programming AI Core in Ascend C**
    
    - The basics of programming AI Core using Ascend C API, including kernel definition, memory management, data movement, and synchronization.
        
    - This section is meant to be read by anyone wanting to get started in Ascend NPU programming.
        
    - This section is meant to be instructional, not complete, and teaches the most important and common parts of Ascend C API programming.
        
- **Part 3: Advanced Ascend C**
    
    - Introduces some more advance features of Ascend C API that enable fine-grained control and performance optimization, including pipeline optimization and multi-core parallelism.
        
- **Part 4: Ascend C API Features**
    
    - This section contains complete coverage of specific Ascend C API features such as different data movement patterns, reduction operations, and quantization.
        
    - This section should be consulted when knowing the complete picture of a specific feature is needed.
        
- **Part 5: Technical Appendices**
    
    - The technical appendices provide reference documentation on Ascend C API specifications, hardware architecture versions, and API complete lists.
        
    - This section is meant as technical reference for specific description of API syntax, semantics, and technical behavior.
        

Parts 1-2 provide a guided learning experience for developers new to Ascend C API.

Parts 3-5 provide comprehensive information about advanced features and detailed topics, and are intended to provide a reference for developers needing more details as they write Ascend C applications.

**Target Audience**

This guide is written for **beginners who are starting from scratch** with Ascend C API programming. It assumes:

- Basic C programming knowledge
- Familiarity with parallel computing concepts (helpful but not required)
- No prior experience with Ascend NPU or GPU programming required

Each concept is introduced with clear definitions and practical examples.

**How to Use This Guide**

**For beginners**: Read Part 1 to understand core concepts, then Part 2 to learn practical programming. Start with simple examples and gradually progress to advanced topics.

**For experienced developers**: Use Part 2 as a quick reference for API usage, Part 3-4 for advanced features, and Part 5 for technical specifications.

**Key Concepts**

Throughout this guide, you will encounter these key concepts:

- **AI Core**: The computational unit in Ascend NPU
- **Kernel**: The entry function that executes on AI Core
- **Global Memory (GM)**: Off-chip memory accessible to all AI Cores
- **Unified Buffer (UB)**: On-chip memory for vector computation
- **L0/L1/L2/L3**: Memory hierarchy levels
- **AI Vector**: SIMD vector computation unit
- **AI Cube**: Matrix computation unit
- **Pipeline**: Data movement and computation pipeline
- **Sync**: Synchronization mechanisms

**Document Structure**

```
Ascend-C-API编程指南/
├── README.md                          # 本文件，指南概述
├── Part1-Introduction/                # 第一部分：编程模型介绍
│   ├── 1.1-Introduction.md           # Ascend C概述
│   ├── 1.2-Programming-Model.md      # 编程模型（语言无关）
│   └── 1.3-Ascend-Platform.md        # Ascend平台组成
├── Part2-Basics/                      # 第二部分：基础编程
│   ├── 2.1-Intro-to-Ascend-C.md      # Ascend C基础
│   ├── 2.2-Kernels.md                # 核函数定义与调用
│   ├── 2.3-Memory-Management.md      # 内存管理
│   ├── 2.4-Data-Movement.md          # 数据搬运
│   ├── 2.5-Vector-Compute.md         # 向量计算
│   ├── 2.6-Cube-Compute.md           # 矩阵计算
│   ├── 2.7-Synchronization.md        # 同步机制
│   └── 2.8-Complete-Examples.md      # 完整示例
├── Part3-Advanced/                    # 第三部分：高级特性
│   ├── 3.1-Pipeline-Optimization.md  # 流水线优化
│   ├── 3.2-Multi-Core.md             # 多核并行
│   └── 3.3-Performance-Tuning.md     # 性能调优
├── Part4-Features/                    # 第四部分：特性详解
│   ├── 4.1-Data-Movement-Patterns.md # 数据搬运模式
│   ├── 4.2-Reduction.md              # 规约操作
│   ├── 4.3-Quantization.md           # 量化与反量化
│   └── 4.4-Atomic-Operations.md      # 原子操作
└── Part5-Appendices/                  # 第五部分：技术附录
    ├── 5.1-Hardware-Specs.md         # 硬件架构版本
    ├── 5.2-API-Reference.md          # API完整列表
    └── 5.3-Best-Practices.md         # 最佳实践
```

**Related Documents**

- **Ascend C API Reference**: Detailed API documentation in `docs/api/c_api/`
- **Ascend C Examples**: Practical examples in `examples/01_simd_cpp_api/`
- **Ascend NPU Architecture**: Hardware architecture documentation
- **Performance Optimization Guide**: Performance tuning best practices

**Getting Started**

To get started with Ascend C API programming:

1. Install Ascend Toolkit and ensure Ascend NPU is available
2. Read Part 1 to understand core concepts
3. Follow Part 2 examples to write your first kernel
4. Use API reference in Part 5 for detailed API information
5. Explore advanced features in Part 3-4 for performance optimization

**Feedback and Support**

For questions, issues, or feedback about this programming guide or Ascend C API:

- Ascend Community: https://www.hiascend.com/
- GitCode Repository: https://gitcode.com/cann/asc-devkit
- Technical Support: Contact Huawei Ascend support team