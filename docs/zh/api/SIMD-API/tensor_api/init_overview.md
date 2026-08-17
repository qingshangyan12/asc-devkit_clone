# init概述

Tensor API接口可能与前序算子共享AI Core上的持久控制状态。为避免前序算子遗留的原子操作、Mask模式或控制寄存器状态影响当前算子，可在支持的产品上配合使用C API提供的初始化接口。使用该接口时，需要额外引入`c_api/asc_simd.h`，具体初始化范围、约束和产品支持情况以对应C API资料为准。

初始化相关C API如下表所示。

| 接口 | 用途 |
| :--- | :--- |
| [asc_init](../c_api/misc/asc_init.md) | 将当前AI Core上可能由前序算子遗留的全局控制状态恢复为接口约定的初始状态，不会清零Global Memory、Local Memory或用户数据。 |
