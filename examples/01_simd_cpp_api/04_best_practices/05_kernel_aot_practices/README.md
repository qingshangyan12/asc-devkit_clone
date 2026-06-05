# Kernel AOT Practices 样例介绍

## 概述

基于 AOT（Ahead-of-Time）编译优化的性能实践样例，通过将运行时 Tiling 参数提前到编译阶段常量化，让编译器进行更激进的循环展开、常量传播和死代码消除等优化，从而在运行时自动匹配预编译的特化版本，获得性能提升。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| --- | --- | --- |
| [matmul_aot_example](./matmul_aot_example) | 在 Matmul 高阶 API 基础上引入 AOT 编译优化，演示 Tiling 常量定义、AOT Registry 注册、运行时分发（AOTDispatcher）的完整流程，运行时根据 Tiling 参数自动匹配 AOT 特化版本或回退到通用版本。 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
